use strict;
use Class::Autouse;
# load all hooks for this module
Class::Autouse->load_recursive(__PACKAGE__);

our $HOOKS;

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
            push @res, $self->_run_hooks($self, $self->{hooks}->{$re}, %info) if $hookname =~ $re;
        }
    }

    # find regex hooknames
    my @package_hooks = keys %$HOOKS;
    foreach my $re (@package_hooks) {
        push @res, $self->_run_hooks($self, $HOOKS->{$re}, %info) if $hookname =~ $re;
    }

    return @res;
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
        # add hooks to package
        $HOOKS->{$hookname} ||= [];

        push @{$HOOKS->{$hookname}}, $cb;
    }
}

sub event_hook_success {
    my ($self, %opts) = @_;
    return { success => 1, %opts };
}

sub event_hook_error {
    my ($self, $errcode, %opts) = @_;
    return { error => $errcode, %opts };
}

1;
