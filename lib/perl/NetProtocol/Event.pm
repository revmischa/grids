package NetProtocol::Event;
use strict;
use warnings;

use base qw/Class::Accessor::Fast/;
__PACKAGE__->mk_accessors(qw/event_name args time expires/);

sub new {
    my ($class, %opts) = @_;

    my $time = delete $opts{time} || time();
    my $args = delete $opts{params} || {};
    my $expires = delete $opts{expires};
    my $evt_name = delete $opts{event_name} or return undef;

    my $self = {
        expires => $expires,
        time => $time,
        args => $args,
        event_name => $evt_name,
    };

    bless $self, $class;
    return $self;
}

sub expired {
    my ($self) = @_;

    return 0 unless $self->expires;

    my $elapsed = time() - $self->time;
    return 1 if $elapsed > $self->expires;

    return 0;
}

1;
