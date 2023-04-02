	.data
n:
	10
	.text
main:
	load %x0, $n, %x1
	addi %x0, 65535, %x2
	store %x4, 0, %x2
	subi %x2, 1, %x2
	addi %x0, 1, %x4
	addi %x0, 1, %x7
loop:
	addi %x7, 1, %x7
	store %x4, 0, %x2
	subi %x2, 1, %x2
	add %x0, %x3, %x5
	add %x0, %x4, %x3
	add %x4, %x5, %x4
	beq %x7, %x1, exit
	jmp loop
exit:
	end
