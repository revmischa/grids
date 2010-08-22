package Grids::Protocol::Serializer;

use Moose::Role;

requires qw/serialize deserialize/;

around 'deserialize' => sub {
    my ($orig, $self, $data) = @_;

    # lame hack to strip out inline unencrypted warnings from OTR (it makes the message unparsable)
    my $was_unencrypted = $data =~ s/^<b>The following message received from \S+ was <i>not<\/i> encrypted:<\/b>//;

    my $evt = $self->$orig($data);
    
    $evt->was_encrypted(0) if $was_unencrypted;
    
    return $evt;
};

1;
