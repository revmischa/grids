use strict;
use Test::More qw(no_plan);
use lib 'lib/perl';
use NetCode;

# test assembler
{
    is(asm(''), '', 'empty input');
    is(asm(';'), '', 'comment');
    is(asm('; blah blah blah'), '', 'comment');
    is(asm('add $t2, $t1, $t2'), pack("B48", '000000010010101001010000000001000000000000000000'), '');
    is(unpack("C", asm('.db 0x3f')), 0x3f, 'immediate byte');
    is(unpack("s", asm('.dw 0x1234')), 0x1234, 'immediate word');
    is(unpack("L", asm('.dl 0xdeadbeef')), 0xdeadbeef, 'immediate long');
    is(unpack("A*", asm('.ds i love dongues')), 'i love dongues', 'immediate char string');
    is(unpack("Z*", asm('.dz "loldongs"')), 'loldongs', 'immediate zero-terminated string');
}

sub asm {
    my ($asm) = @_;
    return NetCode->assemble_simple($asm);
}



