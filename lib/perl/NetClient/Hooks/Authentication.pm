package Grids::Client;
use strict;
use warnings;

__PACKAGE__->register_event_hooks(
                                  'Authentication.Login' => \&hook_login,
                                  );

sub hook_login {
    my ($client, %info) = @_;

    my $args = $info{args};

    if ($args->{success}) {
        my $session = $args->{session_token} or $client->warn("Did not receive session token in login success");
        $client->session_token($session);
        $client->dbg("Logged in successfully");
    } else {
        # invalid login, clear session
        $client->session_token('');
    }

    return 0;
}

1;
