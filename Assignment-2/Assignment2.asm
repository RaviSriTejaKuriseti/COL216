.data
	msg1: .asciiz "Please input the postfix expression\n"
	msg2: .asciiz "Value of the expression is =  "
	msg3: .asciiz "Oops! It's an invalid postfix expression"

	stringSpace:  .space 100
	stack: .word 50

.text


## t0 - strt address, s1-length of string,t1--first while itr,t2--value,t3--string itr,t4-top index of stack


.globl main
main:

	li $v0, 4
	la $a0, msg1
	syscall			#asks for input

	li $v0, 8		#takes input string

	la $a0, stringSpace	# load byte space into address
	li $a1, 100		# allot the byte space for string

	move $t0, $a0	# save string start address to t0
	syscall


	addi $s1, $s1,0	#initializes $s0 to length--0

	addi $s7, $zero,0 
	addi $t2, $zero,0
	addi $t3 ,$zero, 0
	addi $s5,$t0,0          #Store string start address for future use

	li $s6,1

	stringWhile:
		lb $t1, 0($t0)	     # initialize the count to zero 
		beqz $t1, precursor # goes to checkvalid after loop ends
		addi $t0, $t0,1		 #iterating over string
		addi $s1, $s1,1		 #incements length
		j stringWhile


	precursor:
		addi $s2, $s1,-1
		add $t3 ,$zero,$s5
		addi $t2,$t2,0

		j checkvalid 



	

	checkvalid:

		beq $t2,$s2,checkvalue		

		lb $t5, 0($t3)  
		li $t6, '0'
		sge $t7,$t5,$t6
		li $t6, '9'
		sle $t8,$t5,$t6
		and $t7,$t7,$t8

		beq $t7,$zero,Symbol
		j NotSymbol

	NotSymbol:
		addi $s7, $s7, 1
		addi $t3, $t3, 1
		addi $t2, $t2, 1
		
		j checkvalid
		


	Symbol:
		addi $s7, $s7,-2
		blt $s7,$zero,invalid
		addi $s7, $s7,1
		addi $t3, $t3, 1
		addi $t2, $t2, 1
		
		j checkvalid



	checkvalue:
		li $s6,1
		bne $s7,$s6,invalid
		j proceed





	proceed:
		addi $t2 ,$zero, 0

		addi $t3,$zero, 0
		addi $t4,$zero, 0


		la $a0, stack	# load byte space into address
		li $a1, 200		# allot the byte space for stack

		move $t4, $a0	# save stack start address to t4		

		j evalWhile

	

	evalWhile:
		beq $t3, $s1, final

		lb $t5, stringSpace($t3)  # erroprone
		li $t6, '0'
		sge $t7,$t5,$t6
		li $t6, '9'
		sle $t8,$t5,$t6
		and $t7,$t7,$t8
		beq $t7, $zero ,Else

		addi $t5,$t5,-48 # where $t0 contains the ascii digit .
		sw $t5,0($t4) 	#errror prone
		addi $t4, $t4, 4
		addi $t3, $t3, 1
		j evalWhile


	Else:
		li $t6, '*'
		beq $t5, $t6, star

		li $t6, '-'
		beq $t5, $t6, minus

		addi $t4, $t4, -8
		lw $t7, 0($t4)

		addi $t4, $t4, 4
		lw $t8, 0($t4)

		add $t7,$t7,$t8	
		addi $t4, $t4, -4

		sw $t7,0($t4)

		addi $t4, $t4, 4
		addi $t3, $t3, 1
		j evalWhile



		

	star:
		addi $t4, $t4, -8
		lw $t7, 0($t4)

		addi $t4, $t4, 4
		lw $t8, 0($t4)

		mul $t7,$t7,$t8	
		addi $t4, $t4, -4

		sw $t7,0($t4)

		addi $t4, $t4, 4
		addi $t3, $t3, 1
		j evalWhile

	minus:
		addi $t4, $t4, -8
		lw $t7, 0($t4)

		addi $t4, $t4, 4
		lw $t8, 0($t4)

		sub $t7,$t7,$t8	
		addi $t4, $t4, -4

		sw $t7,0($t4)

		addi $t4, $t4, 4
		addi $t3, $t3, 1
		j evalWhile


	final:
		lw $t8, 0($t4)

		li $v0, 4						
		la $a0, msg2					
		syscall							# prints  the msg2

		li $v0, 1
		move $a0, $t8
		syscall							# prints the answer

		li $v0, 10
		syscall


	invalid:
		li $v0, 4						
		la $a0, msg3					
		syscall							# prints  the msg3

		li $v0, 10
		syscall