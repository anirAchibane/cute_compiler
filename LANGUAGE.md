# CUTE: Language Definition

## Syntax
- Uppercase keywords, case-sensitive names
- One statement per line
- Blank lines and & Comments (# ...) featured
- Final line ends with EOF

## Values
- Signed 32-bit decimal integers
- Fractional & Out of range literal rejected
- -2147483648 a valid signed literal

## Arithmetic
- Operations in order of priority:
    - Unary signs bind
    - multiplication (*), Division (/)
    - Addition (+), Subtraction (-)
- Arithmetics wraps to 32 bits
- Signed division rounds towards 0

## Names
- All variables have global scope
- **LET** introduces new target
- **INPUT** introduces new target via user input
- Allocated variables start at zero

## Control & I/O
- Each condition has one comparison from: ==, !=, <, <=, >, >=
- **PRINT** takes integer expression or quoted string and ends the line
- **INPUT** reads one singed integer

Note that this current version serves as an MVP, other features and improvements will be added subsequently.