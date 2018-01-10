# File:		build.asm
# Author:	K. Reek
# Contributors:	P. White,
#		W. Carithers,
#		Thomas Landi
#
# Description:	Binary tree building functions.
#
# Revisions:	$Log$


	.text			# this is program code
	.align 2		# instructions must be on word boundaries

# 
# Name:		add_elements
#
# Description:	loops through array of numbers, adding each (in order)
#		to the tree
#
# Arguments:	a0 the address of the array
#   		a1 the number of elements in the array
#		a2 the address of the root pointer
# Returns:	none
#

	.globl	add_elements
	
add_elements:
	addi 	$sp, $sp, -16
	sw 	$ra, 12($sp)
	sw 	$s2, 8($sp)
	sw 	$s1, 4($sp)
	sw 	$s0, 0($sp)

#***** BEGIN STUDENT CODE BLOCK 1 ***************************
#
# Insert your code to iterate through the array, calling build_tree
# for each value in the array.  Remember that build_tree requires
# two parameters:  the address of the variable which contains the
# root pointer for the tree, and the number to be inserted.
#
	move	$s0, $a0
	move	$s1, $a1
	move	$a0, $a2	
	move	$s2, $zero
add_top:
	sub	$t0, $s1, $s2
	beq	$t0, $zero, add_end
	li	$t1, 4
	mul	$t2, $t1, $s2
	add	$t3, $t2, $s0
	lw	$a1, 0($t3)
	jal	build_tree
	addi	$s2, 1
	j	add_top
add_end:
#***** END STUDENT CODE BLOCK 1 *****************************

add_done:

	lw 	$ra, 12($sp)
	lw 	$s2, 8($sp)
	lw 	$s1, 4($sp)
	lw 	$s0, 0($sp)
	addi 	$sp, $sp, 16
	jr 	$ra

#***** BEGIN STUDENT CODE BLOCK 2 ***************************
#
# Put your build_tree subroutine here.
#

	.globl allocate_mem

build_tree:
	addi	$sp, $sp, -32
	sw	$ra, 28($sp)
	sw	$s6, 24($sp)
	sw	$s5, 20($sp)
	sw	$s4, 16($sp)
	sw	$s3, 12($sp)
	sw	$s2, 8($sp)
	sw	$s1, 4($sp)
	sw	$s0, 0($sp)

	move	$s5, $a0		#Save the params
	move	$s1, $a1
	
	
	lw	$s0, 0($s5)		#Move pointer to s0
	
	move	$s6, $a0		#Allocate the new node
	li	$a0, 3
	jal	allocate_mem
	move	$s3, $v0
	sw	$s1, 0($s3)
	move	$a0, $s6

	beq	$s0, $zero, no_tree	#If there is no root
	j	top
no_tree:				#Make the root of the tree
	sw	$s3, 0($s5)
	j	build_done
top:					
	lw	$t9, 0($s0)		#Check to see if number is already in tree
	beq	$s1, $t9, build_done	#If so, exit without entering value
	slt	$t9, $s1, $t9		#Else, determine if greater or less than current
	beq	$t9, $zero, right
left:					#If less, go left
	lw	$s4, 4($s0)
	beq	$s4, $zero, none_left	#If no left node, enter value as left node
	lw	$s0, 4($s0)		# and exit
	lw	$s4, 4($s0)
	j	top	
right:
	lw	$s4, 8($s0)		#If greater, go right
	beq	$s4, $zero, none_right	#If no right node, enter value as right 
	lw	$s0, 8($s0)		# and exit
	lw	$s4, 8($s0)
	j top

none_left:
	sw	$s3, 4($s0)
	j	build_done

none_right:
	sw	$s3, 8($s0)

	
build_done:
	lw	$ra, 28($sp)
	lw	$s6, 24($sp)
	lw	$s5, 20($sp)
	lw	$s4, 16($sp)
	lw	$s3, 12($sp)
	lw	$s2, 8($sp)
	lw	$s1, 4($sp)
	lw	$s0, 0($sp)
	addi	$sp, $sp, 32
	jr	$ra
	
#***** END STUDENT CODE BLOCK 2 *****************************
