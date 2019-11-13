in
pop ax
in
pop bx
in
pop cx

push ax
push 0
je aIsZero

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

push dx
push 0
ju dIsLessZero

push dx
push 0
je dIsZero

push bx
push -1
mul
push dx
sqrt
sum
push 2
push ax
mul
div
out

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
out
jmp End


: dIsZero
push bx
push -1
mul
push 2
push ax
mul
div
out
jmp End

: dIsLessZero
push -666
out
jmp End


: aIsZero
push cx
push -1
mul
push bx
div
out
jmp End


: End
end