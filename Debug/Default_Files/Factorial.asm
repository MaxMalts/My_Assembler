jmp FactJump
func Fact

pop bx
push bx
push 1
je stop
push bx
push bx
push 1
sub
call Fact
mul
jmp EndFact

: stop
push bx
: EndFact
ret
: FactJump

in
call Fact
out
end