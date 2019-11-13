in
pop ax

jmp AddJmp
func Add
push 1
add
ret
: AddJmp

push ax
call Add
out

end