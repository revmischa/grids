use strict;
use warnings;

use Test::More qw/no_plan/;
use lib 'lib';
use Grids::Hooks;

my ($test1, $test2);

Grids::Hooks->register_hook('test1' => \&test1);
Grids::Hooks->register_hook(qr/test\d/ => \&testn);

Grids::Hooks->run_hooks('test1');
ok($test1, "Got global hook");
ok($test2, "Got regex hook");

sub test1 {
    $test1 = 1;
}

sub testn {
    $test2 = 1;
}
