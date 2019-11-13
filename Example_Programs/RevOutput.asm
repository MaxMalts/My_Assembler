in
pop ax

push 0
pop dx

: nextIn
push dx
push ax
je stopIn

in
pop [dx]

push dx
push 1
add
pop dx
jmp nextIn
: stopIn


push dx
push 1
sub
pop dx

: nextOut
push dx
push 0
ju stopOut

push [dx]
out

push dx
push 1
sub
pop dx
jmp nextOut
: stopOut

end