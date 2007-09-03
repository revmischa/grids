sub run_event_hooks {
    my ($self, %info) = @_;

    return undef unless $self->{hooks};

    my $event = $info{event};
    my $hooks = $self->{hooks}->{"Event-$event"};
    return undef unless $hooks && @$hooks;

    my @res;

    foreach my $hook (@$hooks) {
        my $ret = $hook->($self, %info);
        push @res, $ret if $ret;
    }

    return @res || undef;
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
