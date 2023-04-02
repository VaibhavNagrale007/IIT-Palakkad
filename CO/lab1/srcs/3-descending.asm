	.data
a:
	70
	80
	40
	20
	10
	30
	50
	60
n:
	8
	.text
main:
	load %x0, $n, %x1
loop:
	beq %x2, %x1, exit
	addi %x3, 1, %x3
	load %x2, $a, %x4
	load %x3, $a, %x5
	bgt %x1, %x3, swap
	addi %x2, 1, %x2
	addi %x2, 0, %x3
	beq %x2, %x1, exit
	jmp loop
swap:
	blt %x4, %x5, swap2
	jmp loop
swap2:
	store %x5, $a, %x2
	store %x4, $a, %x3
	jmp loop
exit:
	end
