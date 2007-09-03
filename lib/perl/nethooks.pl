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

sub run_hooks {
    my ($self, $hookname, %info) = @_;

    my $hooks = $self->{hooks}->{$hookname};
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

sub register_event_hook {
    my ($self, $event, $cb, $cb_obj) = @_;

    if ($cb_obj) {
        my $_cb = $cb;
        $cb = sub { $cb->($cb_obj, @_) };
    }

    $self->{hooks} ||= {};
    $self->{hooks}->{"Event-$event"} ||= [];

    push @{$self->{hooks}->{"Event-$event"}}, $cb;
}

1;
