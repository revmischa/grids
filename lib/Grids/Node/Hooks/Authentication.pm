package Grids::Node;
use strict;
use warnings;

use constant {
    ERROR_LOGIN_INVALID => -1,
    ERROR_AUTHENTICATION_REQUIRED => -2,
};

__PACKAGE__->register_event_hooks(
                                  'Authentication.Login' => \&hook_login,
                                  );

__PACKAGE__->register_hooks(
                            'Authentication.Login.AuthCheck' => \&hook_auth_check_pubkey,
                            );

sub hook_login {
	my Grids::Node $node = shift;
	my %info = @_;

    if ($node->test_any_hook('Authentication.Login.AuthCheck', %info)) {
        # successful login, generate sessiont token
        my $session_token = time() . rand();

        # instantiate remote object representing this connection
        my $remote = __PACKAGE__::Remote->new(trans => $info{trans},
                                          session_token => $session_token,
                                          public_key => $info{args}{public_key});

        $node->sessions->{$session_token} = $remote;

        # return success and token
        return $node->event_hook_success(session_token => $session_token);
    } else {
        return $node->event_hook_error(ERROR_LOGIN_INVALID);
    }
}

sub hook_auth_check_pubkey {
    my ($node, %info) = @_;

    my $pubkey = $info{args}{public_key} or return 0;

    my %authorized_keys = $node->authorized_keys;
    return grep { $pubkey eq $_ } keys %authorized_keys;
}

sub need_auth {
    my $self = shift;
    return $self->event_hook_error(ERROR_AUTHENTICATION_REQUIRED);
}

1;
