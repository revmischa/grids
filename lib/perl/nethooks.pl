sub run_event_hooks {
    my ($self, %info) = @_;

    return unless $self->{hooks};

    my $event = $info{event};
    my $hooks = $self->{hooks}->{"Event-$event"};
    return unless $hooks;

    my @res;

    foreach my $hook (@$hooks) {
        my $ret = $hook->($self, %info)
        push @res, $ret if $ret;
    }

    return @res;
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
