use strict;
use Test::More qw(no_plan);
use lib 'lib/perl';
use NetCode;

# test assembler
{
    is(NetCode->assemble(''), '', 'empty input');
    is(NetCode->assemble(';'), '', 'comment');
    is(NetCode->assemble('; blah blah blah'), '', 'comment');
    is(NetCode->assemble('add $t2, $t1, $t2'), pack("B48", '000000010010101001010000000001000000000000000000'), '');
    is(unpack("C", NetCode->assemble('.db 0x3f')), 0x3f, 'immediate byte');
    is(unpack("s", NetCode->assemble('.dw 0x1234')), 0x1234, 'immediate word');
    is(unpack("L", NetCode->assemble('.dl 0xdeadbeef')), 0xdeadbeef, 'immediate long');
    is(unpack("A*", NetCode->assemble('.ds i love dongues')), 'i love dongues', 'immediate char string');
    is(unpack("Z*", NetCode->assemble('.dz "loldongs"')), 'loldongs', 'immediate zero-terminated string');
}




