in
pop dx
push 1
pop ax
: next
push ax
push dx
ja stop
push ax
push ax
mul
out
push ax
push 1
sum
pop ax
jmp next
: stop
end