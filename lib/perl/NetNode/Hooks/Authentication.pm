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
    my ($server, %info) = @_;

    if ($server->test_any_hook('Authentication.Login.AuthCheck', %info)) {
        return $server->event_hook_success();
    } else {
        return $server->event_hook_error(ERROR_LOGIN_INVALID);
    }
}

sub hook_auth_check_pubkey {
    my ($server, %info) = @_;

    my $pubkey = $info{args}{public_key} or return 0;

    my %authorized_keys = $server->authorized_keys;
    return grep { $pubkey eq $_ } keys %authorized_keys;
}

1;
