in
in
in
pop ax
pop bx
pop cx
push ax
push 0
je Label1
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
ju Label2
push dx
push 0
je Label3
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
jmp Label4
: Label3
push bx
push -1
mul
push 2
push ax
mul
div
out
jmp Label4
: Label2
push -6.66667e+07
out
jmp Label4
: Label1
push cx
push -1
mul
push bx
div
out
jmp Label4
: Label4
end
