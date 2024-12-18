; calculate area and perimeter of a rectangle
IN A
IN B
; copy `A` to `D`
ADD D, A
; area
MUL A, B
OUT A
; perimeter
; reset `A`
SUB A, A
ADD A, D
; calculate perimeter
ADD A, B
MOV 2 ; store constant `2` in `D`
MUL A, D
OUT A
