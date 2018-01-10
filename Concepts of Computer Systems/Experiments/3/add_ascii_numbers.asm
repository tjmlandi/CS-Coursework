# File:		add_ascii_numbers.asm
# Author:	K. Reek
# Contributors:	P. White, W. Carithers
#		Thomas Landi
#
# Updates:
#		3/2004	M. Reek, named constants
#		10/2007 W. Carithers, alignment
#		09/2009 W. Carithers, separate assembly
#
# Description:	Add two ASCII numbers and store the result in ASCII.
#
# Arguments:	a0: address of parameter block.  The block consists of
#		four words that contain (in this order):
#
#			address of first input string
#			address of second input string
#			address where result should be stored
#			length of the strings and result buffer
#
#		(There is actually other data after this in the
#		parameter block, but it is not relevant to this routine.)
#
# Returns:	The result of the addition, in the buffer specified by
#		the parameter block.
#

	.globl	add_ascii_numbers

add_ascii_numbers:
A_FRAMESIZE = 40

#
# Save registers ra and s0 - s7 on the stack.
#
	addi 	$sp, $sp, -A_FRAMESIZE
	sw 	$ra, -4+A_FRAMESIZE($sp)
	sw 	$s7, 28($sp)
	sw 	$s6, 24($sp)
	sw 	$s5, 20($sp)
	sw 	$s4, 16($sp)
	sw 	$s3, 12($sp)
	sw 	$s2, 8($sp)
	sw 	$s1, 4($sp)
	sw 	$s0, 0($sp)
	
# ##### BEGIN STUDENT CODE BLOCK 1 #####
student:
	lw	$s1, 0($a0)		#first input
	lw	$s2, 4($a0)		#second input
	lw	$s3, 8($a0)		#result
	lw	$s4, 12($a0)		#length
	move	$s0, $zero
	addi	$s7, $zero, 57
loop:
	beq	$zero, $s4, end		#check if at end of numbers
	addi	$t3, $s4, -1		#get the index we will be working with
	add	$t1, $s1, $t3		#get the addresses for the numbers
	add	$t2, $s2, $t3		#we are working with
	lb	$t4, 0($t1)		#retreive the numbers we are 
	lb	$t5, 0($t2)		#working with
	add	$t6, $t4, $t5	
	add	$t6, $t6, $s0		#add the carry to the sum
	addi	$t6, $t6, -48		
	slt	$t7, $s7, $t6
	beq	$t7, $zero, else	#see if sum is greater than 9
	addi	$t6, $t6, -10		#save the difference and the carry
	addi	$s0, $zero, 1
	j	point
else:
	move	$s0, $zero		#if not, make the carry 0
point:
	add	$t3, $s3, $t3		
	sb	$t6, 0($t3)		#store the sum
	addi	$s4, $s4, -1		#decrement the counter
	j	loop
end:
# ###### END STUDENT CODE BLOCK 1 ######

#
# Restore registers ra and s0 - s7 from the stack.
#
	lw 	$ra, -4+A_FRAMESIZE($sp)
	lw 	$s7, 28($sp)
	lw 	$s6, 24($sp)
	lw 	$s5, 20($sp)
	lw 	$s4, 16($sp)
	lw 	$s3, 12($sp)
	lw 	$s2, 8($sp)
	lw 	$s1, 4($sp)
	lw 	$s0, 0($sp)
	addi 	$sp, $sp, A_FRAMESIZE

	jr	$ra			# Return to the caller.
