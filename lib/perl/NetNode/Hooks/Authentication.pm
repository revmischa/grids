package NetNode;
use strict;
use warnings;

use constant {
    ERROR_LOGIN_INVALID => -1,
};

__PACKAGE__->register_event_hooks(
                                  'Authentication.Login' => \&hook_login,
                                  );

__PACKAGE__->register_hooks(
                            'Authentication.Login.AuthCheck' => \&hook_auth_check_pubkey,
                            );

sub hook_login {
    my ($node, %info) = @_;

    if ($node->test_any_hook('Authentication.Login.AuthCheck', %info)) {
        # successful login, generate sessiont token
        my $session_token = time() . rand();
        $node->sessions->{$session_token} = 1;

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

1;
