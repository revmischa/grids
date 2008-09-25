package Grids::Node;
use strict;
use warnings;
use Grids::Node::Remote;

use constant {
    ERROR_LOGIN_INVALID => -1,
    ERROR_AUTHENTICATION_REQUIRED => -2,
};

__PACKAGE__->register_hooks(
                            'Authentication.Login.AuthCheck' => \&hook_auth_check_pubkey,
                            'Authentication.Login' => \&hook_login,
                            );

sub hook_login {
	my Grids::Node $node = shift;
	my Grids::Protocol::Event $evt = shift;

    if ($node->test_any_hook('Authentication.Login.AuthCheck', $evt)) {
        # successful login, generate sessiont token
        my $session_token = time() . rand();

		# instantiate remote object representing this connection
		my $remote = Grids::Node::Remote->new(
			trans => $evt->trans,
			session_token => $session_token,
			public_key => $evt->args->{public_key}
		);

        $node->sessions->{$session_token} = $remote;

        # return success and token
        return $node->event_hook_success(session_token => $session_token);
    } else {
        return $node->event_hook_error(ERROR_LOGIN_INVALID);
    }
}

sub hook_auth_check_pubkey {
    my ($node, $evt) = @_;

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

1;
