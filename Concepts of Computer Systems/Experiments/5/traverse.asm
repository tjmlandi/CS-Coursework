# File:		traverse_tree.asm
# Author:	K. Reek
# Contributors:	P. White,
#		W. Carithers,
#		Thomas Landi
#
# Description:	Binary tree traversal functions.
#
# Revisions:	$Log$


# CONSTANTS
#

# traversal codes
PRE_ORDER  = 0
IN_ORDER   = 1
POST_ORDER = 2

	.text			# this is program code
	.align 2		# instructions must be on word boundaries

	.globl	traverse_tree

#***** BEGIN STUDENT CODE BLOCK 3 *****************************
#
# Put your traverse_tree subroutine here.
#
traverse_tree:
	addi	$sp, $sp, -24
	sw	$ra, 20($sp)
	sw	$s4, 16($sp)
	sw	$s3, 12($sp)
	sw	$s2, 8($sp)
	sw	$s1, 4($sp)
	sw	$s0, 0($sp)

	move	$s2, $a2		#Save the params to s registers
	move	$s1, $a1
	move	$s0, $a0

	lw	$s3, 4($s0)		#Load the leaves of the current node
	lw	$s4, 8($s0)

	li	$t1, IN_ORDER		#Load the constants
	li	$t2, POST_ORDER

	beq	$s2, $zero, pre_order	#Check what type of traversal
	beq	$s2, $t1, in_order
	beq	$s2, $t2, post_order


#
# Pre Order Traversal
#
pre_order:
	jalr	$s1			#Node Process
	beq	$s3, $zero, after_0	#Left Child traversal
	move	$a0, $s3
	jal	traverse_tree
after_0:
	beq	$s4, $zero, after_1	#Right Child traversal
	move	$a0, $s4
	jal	traverse_tree
after_1:
	j end

#
# In Order Traversal
#
in_order:
	beq	$s3, $zero, after_2	#Left Child traversal
	move	$a0, $s3
	jal	traverse_tree
after_2:
	move	$a0, $s0		#Node Process
	jalr	$s1			
	beq	$s4, $zero, after_3	#Right Child traversal
	move	$a0, $s4
	jal	traverse_tree
after_3:
	j end

#
# Post Order Traversal
#
post_order:
	beq	$s3, $zero, after_4	#Left Child traversal
	move	$a0, $s3
	jal	traverse_tree
after_4:
	beq	$s4, $zero, after_5	#Right Child traversal
	move	$a0, $s4
	jal	traverse_tree
after_5:
	move	$a0, $s0		#Node Process
	jalr	$s1		
	j end

end:
	lw	$ra, 20($sp)
	lw	$s4, 16($sp)
	lw	$s3, 12($sp)
	lw	$s2, 8($sp)
	lw	$s1, 4($sp)
	lw	$s0, 0($sp)
	addi	$sp, $sp, 24
	jr	$ra

 
#***** END STUDENT CODE BLOCK 3 *****************************
