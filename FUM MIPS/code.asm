addi $8, $0, 10
add $9, $0, $0
loop:	
	beq $8 $0, exit
	add $9, $9, $8
	addi $8, $8, -1
j loop
exit: