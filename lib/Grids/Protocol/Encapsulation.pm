package Grids::Protocol::Encapsulation;

use Moose::Role;

requires qw/encapsulate decapsulate/;

around 'decapsulate' => sub {
    my ($orig, $self, $data) = @_;

    # lame hack to strip out inline unencrypted warnings from OTR (it makes the message unparsable)
    my $was_unencrypted = $data =~ s/^<b>The following message received from \S+ was <i>not<\/i> encrypted:<\/b>//;

    return $self->$orig($data);
};

1;
