use Test::More qw(no_plan);
BEGIN { use_ok('NetReg') };

my $regs = new NetReg(32); # create storage for 32 registers
is($regs->size, 32, "initted register storage");

$regs->set(3, 0x12345678);
is($regs->get(3), 0x12345678, 'set/get');
is($regs->get_u(3), 0x12345678, 'get unsigned');

$regs->set_u(3, 0xFFFFFFFF);
is($regs->get_u(3), 0xFFFFFFFF, 'set/get unsigned');
is($regs->get(3), -1, 'set/get signed');

$regs->set(4, -1488);
is($regs->get(4), -1488, 'set/get signed');

$regs->set(1, 0b00000110);
is($regs->and(1, 3), 6, "and");

$regs->set(2, 0b10101010);
is($regs->or(1, 2), 0b10101110, "or");

is($regs->xor(1, 2), 0b10101100, "xor");

$regs->set_u(9, 0xabcdef12);
is($regs->get_u(9), 0xabcdef12, "set/get unsigned");

