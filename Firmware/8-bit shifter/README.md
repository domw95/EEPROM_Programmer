# Bit shifter for Logic module
## Functions
- Bit shift,    Either right or left 0-7 positions
- Bit set,      Set a chosen bit
- Bit clear,    Clear a chosen bit
- Bit extract,  Move chosen bit to position 0
- Bit rotate,   Shift with wrap around
- Invert,       Bitwise NOT
- Bit parity,   Parity of input
- Pop Count     Number of bits in input
- Increment     Add 1 to value
- Decrement     Subtract 1 from value

## Might add
- Add           Add a signed 4 bit integer
- Mathematical Shift? (keep sign bit)
- Masking from position up/down
- Clear from position up/down
- Set from position up/down
- Psuedo random sequence / LFSR

## Bit pattern
Function    15  14  13  12  11  10  9   8   7-0
Shift right 0   0   0   0   0   x   x   x   input byte  x = shift value
Shift left  0   0   0   0   1   x   x   x   "           x = shift value
Set         0   0   0   1   0   x   x   x   "           x = bit position
Clear       0   0   0   1   1   x   x   x   "           x = bit position
Rot right   0   0   1   0   0   x   x   x   "           x = shift value
Rot left    0   0   1   0   1   x   x   x   "           x = shift value
            0   0   1   1   0   
            0   0   1   1   1
Extract     0   1   0   0   0   x   x   x   "           x = bit position
Invert      0   1   0   0   1   0   0   0   "
Pop count   0   1   0   0   1   0   0   1   "
Parity      0   1   0   0   1   0   1   0   "
!Parity     0   1   0   0   1   0   1   1   "
Increment   0   1   0   0   1   1   0   0   "
Decrement   0   1   0   0   1   1   0   1   "
