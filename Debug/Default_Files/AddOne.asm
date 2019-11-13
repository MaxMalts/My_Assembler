in
pop ax

jmp AddJmp
func Add
push 1
sum
ret
: AddJmp

push ax
call Add
out

end