package Grids::Client;
use strict;
use warnings;
use Carp;

__PACKAGE__->register_hooks(
                            'Authentication.Login' => \&hook_login,
                            );

sub hook_login {
    my $client = shift;
    my $evt = shift;
    warn "login hook";
    if ($evt->is_success) {
        warn "login success, token: " . $evt->session_token;
        my $session = $evt->session_token or $client->warn("Did not receive session token in login success");
        $client->session_token($session);
        $client->dbg("Logged in successfully, session token: $session");
    } else {
        # invalid login, clear session
        $client->session_token('');
    }

    return 0;
}

1;
