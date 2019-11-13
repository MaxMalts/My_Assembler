in
pop ax
push 0
pop dx
: Label2
push dx
push ax
je Label1
in
pop [dx]
push dx
push 1
add
pop dx
jmp Label2
: Label1
push dx
push 1
sub
pop dx
: Label4
push dx
push 0
ju Label3
push [dx]
out
push dx
push 1
sub
pop dx
jmp Label4
: Label3
end
