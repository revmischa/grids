package Grids::Hookable;

use strict;
use warnings;
use Class::Autouse;
use Grids::Hooks;
use Carp qw/croak/;

our %HOOKS; # package->hookname

# load all hooks for this module
sub load_hooks {
	my $package = shift or croak "Need to pass in package to load hooks for";
	Class::Autouse->load_recursive($package);

    # load global hooks too
	Class::Autouse->load_recursive("Grids::Hooks");
}

# run hooks and require them all to return true
sub test_all_hooks {
    my ($self, $hookname, $info) = @_;

    return ! grep { ! $_ } $self->run_hooks($hookname, $info);
}

# run hooks and require at least one to return true
sub test_any_hook {
    my ($self, $hookname, $info) = @_;

    return grep { $_ } $self->run_hooks($hookname, $info);
}

# find hooks on instance/package
sub run_hooks {
    my ($self, $hookname, $info) = @_;

    my @res;

    if (ref $self) {
        # look for hooks registered on this instance
        push @res, $self->run_hooks_on($self->{hooks}, $hookname, $info);
    }

    # look for hooks registered in this package
    my $package = ref $self || $self;
    push @res, $self->run_hooks_on($HOOKS{$package}, $hookname, $info)
        if (! ref $package && $package ne 'Grids::Hooks');

    # look for global hooks
    push @res, $self->run_hooks_on(\%Grids::Hooks::HOOKS, $hookname, $info);

    return @res;
}

# finds and runs hooks on all matching hooks stored in $hook_desc
sub run_hooks_on {
    my ($self, $hook_desc, $hookname, $info) = @_;

    my @res;
        
    foreach my $hooks (values %$hook_desc) {
        foreach my $hook_info (@$hooks) {
            push @res, $self->_run_hook($self, $hook_info->{callback}, $info)
                if $self->hook_matches($hookname, $hook_info->{hookname});
        }
    }

    return @res;
}

sub hook_matches {
    my ($pkg, $hookname, $match) = @_;

    if (ref $match) {
        if (ref $match eq 'Regexp') {
            return $hookname =~ $match;
        } else {
            warn "Warning, there is a handler for hook named $match, this is probably a mistake";
        }
    }

    return $hookname eq $match;
}

# actually runs hook
sub _run_hook {
    my ($pkg, $self, $cb, $info) = @_;

    return unless $cb;
    return $cb->($self, $info);
}

sub run_event_hooks {
    my ($self, $info) = @_;

    my $event = $info->{event};
    my @res = $self->run_hooks($event, $info);;
    use Data::Dumper;
    warn Dumper(\@res);
    return @res;
    return $self->run_hooks($event, $info);
}

sub register_hooks {
    my ($self, @hooks) = @_;

    # don't coerce args into hash because it will turn precompiled regexp's into strings
    for (my $i = 0; $i < $#hooks ; $i += 2) {
        $self->register_hook($hooks[$i], $hooks[$i + 1]);
    }
}

sub register_hook {
    my ($self, $hookname, $cb, $cb_obj) = @_;

    my $hook_save = $self->hook_save($hookname, $cb, $cb_obj);

    if (ref $self) {
        # adding hooks to an instance
        $self->{hooks} ||= {};
        $self->{hooks}->{$hookname} ||= [];

        push @{$self->{hooks}->{$hookname}}, $hook_save;
    } else {
		# register package hooks
        $HOOKS{$self}->{$hookname} ||= [];

        push @{$HOOKS{$self}->{$hookname}}, $hook_save;
    }
}

sub hook_save {
    my ($class, $hookname, $cb, $cb_obj) = @_;

    if ($cb_obj) {
        my $_cb = $cb;
        $cb = sub { $cb->($cb_obj, @_) };
    }

    return { hookname => $hookname, callback => $cb };
}

*hook_ok = \&event_hook_success;
sub event_hook_success {
    my ($self, %opts) = @_;
    return { success => 1, %opts };
}

sub event_hook_error {
    my ($self, $errcode, %opts) = @_;
    return { error => $errcode, %opts };
}

1;
