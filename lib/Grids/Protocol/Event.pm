use strict;
use warnings;

package Grids::Protocol::Event;

use Carp;
use Grids::UUID;

use base qw/Class::Accessor::Fast/;

__PACKAGE__->mk_accessors(qw/event_name trans proto args time expires target source message_id signed_message_id/);

sub new {
    my ($class, %opts) = @_;

    my $time = delete $opts{time} || time();
    my $args = delete $opts{params} || delete $opts{args} || {};
    my $evt_name = delete $opts{event_name} or return undef;

    croak "Invalid args to Event->new: " . join ', ', keys %opts
        if %opts;

    my $self = {
        time => $time,
        args => $args,
        event_name => $evt_name,
    };

    foreach my $opt (qw/proto trans expires target source message_id signed_message_id/) {
        $self->{$opt} = $opts{$opt} if exists $opts{$opt};
    }

    bless $self, $class;
    return $self;
}

sub name { $_[0]->event_name }

# uuid identifying this event
sub message_id {
    my $self = shift;

    my $mid = $self->{message_id};
    return $mid if $mid;

    $self->{message_id} = Grids::UUID->new_id;
    return $self->{message_id};
}

sub expired {
    my ($self) = @_;

    return 0 unless $self->expires;

    my $elapsed = time() - $self->time;
    return 1 if $elapsed > $self->expires;

    return 0;
}

1;
