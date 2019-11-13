jmp Begin

jmp InputJump
func Input
in
pop ax
in
pop bx
in
pop cx
ret
: InputJump

jmp DiscrJump
func Discr
push bx
push bx
mul
push 4
push ax
push cx
mul
mul
sub
pop dx
ret
: DiscrJump

jmp TwoRootsJump
func TwoRoots
push bx
push -1
mul
push dx
sqrt
add
push 2
push ax
mul
div

push bx
push -1
mul
push dx
sqrt
sub
push 2
push ax
mul
div
ret
: TwoRootsJump

jmp OneRootJump
func OneRoot
push bx
push -1
mul
push 2
push ax
mul
div
ret
: OneRootJump

jmp LinealEquationJump
func LinealEquation
push cx
push -1
mul
push bx
div
ret
: LinealEquationJump



: Begin

call Input

push ax
push 0
je aIsZero

call Discr

push dx
push 0
ju dIsLessZero

push dx
push 0
je dIsZero

push 2
out
call TwoRoots
out
out
jmp End

: dIsZero
push 1
out
call OneRoot
out out
jmp End

: dIsLessZero
push 0
out
jmp End

: aIsZero
push bx
push 0
je bIsZero
push 1
out
call LinealEquation
out
jmp End

: bIsZero
push cx
push 0
je cIsZero
push 0
out
jmp End

: cIsZero
push 1
push 0
div
out
jmp End


: End
end