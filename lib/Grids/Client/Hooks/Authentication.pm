package Grids::Client;
use strict;
use warnings;
use Carp;

__PACKAGE__->register_hooks(
                            'Authentication.Login' => \&hook_login,
                            );

sub hook_login {
    my Grids::Client $client = shift;
    my $evt = shift;

    my $args = $evt->args;

    if ($args->{success}) {
        my $session = $args->{session_token} or $client->warn("Did not receive session token in login success");
        $client->session_token($session);
        $client->dbg("Logged in successfully, session token: $session");
    } else {
        # invalid login, clear session
        $client->session_token('');
    }

    return 0;
}

1;
