package Grids::Hookable;

use strict;
use warnings;
use Class::Autouse;
use Carp qw/croak/;

our %HOOKS; # package->hookname

# load all hooks for this module
sub load_hooks {
	my $package = shift or croak "Need to pass in package to load hooks for";
	Class::Autouse->load_recursive($package);
}

# run hooks and require them all to return true
sub test_all_hooks {
    my ($self, $hookname, %info) = @_;

    return ! grep { ! $_ } $self->run_hooks($hookname, %info);
}

# run hooks and require at least one to return true
sub test_any_hook {
    my ($self, $hookname, %info) = @_;

    return grep { $_ } $self->run_hooks($hookname, %info);
}

# find hooks on instance/package
sub run_hooks {
    my ($self, $hookname, %info) = @_;

    my @res;
    if (ref $self) {
        # this is an instance, instance hooks if any
        my @instance_hooks = keys %{$self->{hooks}};

        # find regex hooknames
        foreach my $re (@instance_hooks) {
            push @res, $self->_run_hooks($self, $self->{hooks}->{$re}, %info)
                if $self->hook_matches($hookname, $re);
        }
    }

    # find regex hooknames
	my $package = ref $self || $self;
    my @package_hooks = keys %{$HOOKS{$package}};
    foreach my $re (@package_hooks) {
        push @res, $self->_run_hooks($self, $HOOKS{$package}->{$re}, %info)
            if $self->hook_matches($hookname, $re);
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

# actually runs hooks
sub _run_hooks {
    my ($pkg, $self, $hooks, %info) = @_;

    return () unless $hooks && @$hooks;

    my @res;

    foreach my $hook (@$hooks) {
        my $ret = $hook->($self, %info);
        push @res, $ret;
    }

    return @res;
}

sub run_event_hooks {
    my ($self, %info) = @_;

    my $event = $info{event};
    my $hookname = "Event-$event";

    return $self->run_hooks($hookname, %info);
}

sub register_hooks {
    my ($self, %hooks) = @_;

    $self->register_hook($_, $hooks{$_}) foreach keys %hooks;
}

sub register_event_hooks {
    my ($self, %hooks) = @_;

    $self->register_event_hook($_, $hooks{$_}) foreach keys %hooks;
}

sub register_event_hook {
    my ($self, $event, $cb, $cb_obj) = @_;

    my $hookname = "Event-$event";
    return $self->register_hook($hookname, $cb, $cb_obj);
}

sub register_hook {
    my ($self, $hookname, $cb, $cb_obj) = @_;

    if ($cb_obj) {
        my $_cb = $cb;
        $cb = sub { $cb->($cb_obj, @_) };
    }

    if (ref $self) {
        # adding hooks to an instance
        $self->{hooks} ||= {};
        $self->{hooks}->{$hookname} ||= [];

        push @{$self->{hooks}->{$hookname}}, $cb;
    } else {
		# register package hooks
        $HOOKS{$self}->{$hookname} ||= [];

        push @{$HOOKS{$self}->{$hookname}}, $cb;
    }
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
