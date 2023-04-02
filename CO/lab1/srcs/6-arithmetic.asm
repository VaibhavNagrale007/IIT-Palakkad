	.data
a:
	1
d:
	3
n:
	7
	.text
main:
	load %x0, $n, %x1
	load %x0, $d, %x2
	load %x0, $a, %x3
	addi %x0, 65535, %x6
	add %x3, %x5, %x5
loop:
	store %x5, 0, %x6
	subi %x6, 1, %x6
	addi %x4, 1, %x4
	mul %x4, %x2, %x5
	add %x3, %x5, %x5
	beq %x4, %x1, exit
	jmp loop
exit:
	end
