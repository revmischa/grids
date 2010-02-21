package Grids::Node::Hooks::Authentication;

use Moose;
use Grids::Peer;

use constant {
    ERROR_LOGIN_INVALID => -1,
    ERROR_AUTHENTICATION_REQUIRED => -2,
};

Grids::Node->register_hooks(
    'Authentication.Login.AuthCheck' => \&hook_auth_check_pubkey,
    'Authentication.Login' => \&hook_login,
);

sub hook_login {
    my Grids::Node $node = shift;
    my Grids::Protocol::Event $evt = shift;

    if ($node->test_any_hook('Authentication.Login.AuthCheck', $evt)) {
        # return success and token
        return $node->event_hook_success(session_token => $evt->connection->peer->session_token);
    } else {
        return $node->event_hook_error(ERROR_LOGIN_INVALID);
    }
}

sub hook_auth_check_pubkey {
    my ($node, $evt) = @_;

    # FIXME
    return 1;

    # get peer public key
    my $peer_pubkey = $evt->proto->peer->id->pubkey;
    my $pubkey_str = $peer_pubkey->serialize;

    my %authorized_keys = $node->authorized_keys;
    return grep { $pubkey_str eq $_ } values %authorized_keys;
}

sub need_auth {
    my $self = shift;
    return $self->event_hook_error(ERROR_AUTHENTICATION_REQUIRED);
}

no Moose;
__PACKAGE__->meta->make_immutable;

