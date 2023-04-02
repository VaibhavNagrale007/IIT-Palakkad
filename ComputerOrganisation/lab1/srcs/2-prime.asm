	.data
a:
	10
	.text
main:
	load %x0, $a, %x3
	addi %x0, 1, %x1
	addi %x0, 1, %x6
loop:
	addi %x1, 1, %x1
	beq %x3, %x1, prime
	div %x3, %x1, %x2
	mul %x2, %x1, %x2
	beq %x2, %x3, exit
	jmp loop
exit:
	subi %x0, 1, %x10
	end
prime:
	add %x4, %x6, %x10
	end
