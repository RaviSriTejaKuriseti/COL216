

.data
	msg0: .asciiz "Input total no of points you want to compute in the graph -- "#asks for no of points on the graph

	msg1: .asciiz "Input X - coordinate " 			#asks for x coordinate

	msg2: .asciiz "Input Y - coordinate " 			#asks for y coordinate

	msg3: .asciiz "Area= " 							# gives the final answer

	msg4: .asciiz "Invalid Input\n" 					# inconsistent input data

	msg5: .asciiz "\n"								#new line

	

#f1-- area f2,f3,f4,f5--xy coordinates t0--iterator f6--x diff f7-mul of ys f8--y sums
#f9--half	f10,f11 -squares  f13-y mod sums




.text

.globl main

main:

	li $v0, 4						# preparing for printing  the msg0
	la $a0, msg0					# loading message address in $a0
	syscall							# prints  the msg0

	li $v0, 5						# preparing to take input
	syscall							# takes input
	move $s0, $v0					# stores the no of points in ($s0==no of points)

	

	ble $s0, 1, invalidPoints  		#if ($s0<=1) then goes to invlaidPoints

	li.s $f1, 0.0			# loads float ($f1==area) and initializes it to 0.0


	li $v0, 4						# asking x coordinate
	la $a0, msg1					
	syscall


	li $v0, 6						#storing ($f2==x coordinate) 
	syscall							
	mov.s $f2, $f0					



	li $v0, 4						# asking y coordinate
	la $a0, msg2					
	syscall


	li $v0, 6						#storing ($f3==y coordinate) 
	syscall							
	mov.s $f3, $f0	


	add $t0, $t0, 1						#iterator variable ($t0==1) initialised


	while:


		li $v0, 4						# asking x coordinate
		la $a0, msg1					
		syscall

		li $v0, 6						#storing ($f4==x coordinate) 
		syscall							
		mov.s $f4, $f0	



		li $v0, 4						# asking y coordinate
		la $a0, msg2					
		syscall

		li $v0, 6						#storing ($f5==y coordinate) 
		syscall							
		mov.s $f5, $f0	


		li.s $f9, 0.50					#loaded half
		sub.s $f6, $f4, $f2				#created ($f6) --- x coordinates differences
		mul.s $f7, $f3, $f5				#created ($f7) 


		li.s $f15,0.00
		c.lt.s $f7, $f15
		bc1t oppSidePointsArea		#check for opposite sides point

		bc1f sameSidePointsArea


		oppSidePointsArea:
			mul.s $f10, $f3, $f3			#created ($f10)
			mul.s $f11, $f5, $f5			#created ($f11)
			add.s $f11, $f11, $f10
			mul.s $f11, $f11, $f6
			mul.s $f11, $f11, $f9
			sub.s $f13, $f3, $f5             #created ($f13)
			abs.s $f13, $f13
			div.s $f11, $f11, $f13
			add.s $f1, $f1, $f11


			mov.s $f2, $f4					# updated $f2
			mov.s $f3, $f5					# updated $f3
			j comehere


		sameSidePointsArea:
			add.s $f8, $f3, $f5				#created ($f8)
			mul.s $f8, $f8, $f6				
							
			mul.s $f8, $f8, $f9				# calculated temp area   
			abs.s $f8, $f8					# found absolute value of area 
			add.s $f1, $f1, $f8				# added partial area to original area

			mov.s $f2, $f4					# updated $f2
			mov.s $f3, $f5					# updated $f3






			

		

		
	comehere:

		addi $t0, $t0, 1
		blt $t0, $s0, while	
		
		

	


	li $v0, 4						# preparing for printing  the msg3
	la $a0, msg3					# loading message address in $a0
	syscall							# prints  the msg3

	li $v0, 2
	mov.s $f12, $f1
	syscall

	li $v0, 10
	syscall



invalidPoints:
	li $v0, 4			# preparing for printing  the msg4
	la $a0, msg4		# loading message address in $a0
	syscall				# prints  the msg4

	li $v0, 10			
	syscall				#exits from process








