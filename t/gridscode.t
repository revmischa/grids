use Test::More qw(no_plan);

use strict;
use warnings;

use lib 'lib';
use Grids::Code;

# test assembler
{
    is(asm(''), '', 'empty input');
    is(asm(';'), '', 'comment');
    is(asm('; blah blah blah'), '', 'comment');
    is(asm('add $t2, $t1, $t2'), pack("B48", '000000010010101001010000000001000000000000000000'), '');
    is(unpack("C", asm('.db 0x3f')), 0x3f, 'immediate byte');
    is(unpack("S", asm('.dh 0x1234')), 0x1234, 'immediate halfword');
    is(unpack("C", asm('.db 0b10110010')), 0b10110010, 'immediate binary byte unsigned');
    isnt(unpack("c", asm('.db 0b10110010')), 0b10110010, 'immediate binary byte signed');
    is(unpack("C", asm('.db 0b00001101')), 0b00001101, 'immediate binary byte');
    is(unpack("N", asm('.dw 0b00001101101100100000110110110010U')), 0b00001101101100100000110110110010, 'immediate binary word');
    is(unpack("L", asm('.dw 0xdeadbeef')), 0xdeadbeef, 'immediate word');
    is(unpack("A*", asm('.ds i love dongues')), 'i love dongues', 'immediate char string');
    is(unpack("Z*", asm('.dz "loldongs"')), 'loldongs', 'immediate zero-terminated string');
}

sub asm {
    my ($asm) = @_;
    return Grids::Code->assemble_simple($asm);
}



