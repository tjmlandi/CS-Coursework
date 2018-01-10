# File:		life_age.asm
# Author:	Thomas Landi
# Section:	02
# Description:	This program takes user input to create an instance
#		of Conway's Game of Life with Age and run it for 
#		the user defined amount of generations



#syscall codes
PRINT_INT = 	1
PRINT_STRING = 	4
READ_INT = 	5
EXIT = 		10
PRINT_CHAR = 	11

#board pieces
CORNER = 	43
HORIZONTAL = 	45
VERTICAL = 	124
DEAD = 		32

	.data
	.align 2

board_size:
	.space 4

generations:
	.space 4

live_cells:
	.space 4

board:
	.space 30*30*4

board_b:
	.space 30*30*4

	.align 0


banner:
	.asciiz "\n*************************************\n****    Game of Life with Age    ****\n*************************************
	#
	#Prompts for input
	#

prompt_board_size:
	.asciiz "\n\nEnter board size: "

prompt_num_gen:
	.asciiz "\nEnter number of generations to run: "

prompt_live_cells:
	.asciiz "\nEnter number of live cells: "

prompt_start_loc:
	.asciiz "\nStart entering locations\n"



	#
	#Error messages
	#
	
illegal_size:
	.asciiz "\nWARNING: illegal board size, try again: "

illegal_generations:
	.asciiz "\nWARNING: illegal number of generations, try again: "

illegal_cells:
	.asciiz "\nWARNING: illegal number of live cells, try again: "

illegal_loc:
	.asciiz "\nERROR: illegal point location\n"

	#
	#Generational Gap
	#

first_piece:
	.asciiz "\n====    GENERATION "

second_piece:
	.asciiz "    ====\n"

newline:
	.asciiz "\n"

	.text
	.align 2
	.globl main
main:
	addi	$sp, $sp, -36			#Setting up the stack frame
	sw 	$ra, 0($sp)
	sw 	$s0, 4($sp)
	sw	$s1, 8($sp)
	sw	$s2, 12($sp)
	sw	$s3, 16($sp)
	sw	$s4, 20($sp)
	sw	$s5, 24($sp)
	sw	$s6, 28($sp)
	sw	$s7, 32($sp)

	li	$v0, PRINT_STRING		#Printing the banner
	la	$a0, banner
	syscall


	#
	#Get the Board Size
	#

	li	$v0, PRINT_STRING		#Print the prompt for the board size
	la	$a0, prompt_board_size
	syscall

read_size:
	li	$v0, READ_INT			#Read in the user's board size
	syscall
	move	$s0, $v0
	move	$t0, $zero
	slti	$t0, $s0, 4
	li	$t1, 1				
	beq	$t0, $t1, size_failure
	slti	$t0, $s0, 31
	beq	$t0, $zero, size_failure
	j	size_success
size_failure:					#If the board size is invalid, ask them to try again
	li	$v0, PRINT_STRING
	la	$a0, illegal_size
	syscall
	j	read_size
size_success:					#IF the board size is valid, save it in memory
	la	$t0, board_size
	sw	$s0, 0($t0)
	
						#
						#Get the Number of Generations
						#
	li	$v0, PRINT_STRING		#Print the prompt for the generations
	la	$a0, prompt_num_gen
	syscall

read_generations:
	li	$v0, READ_INT			#Read the user's generation number
	syscall
	move	$s0, $v0
	move	$t0, $zero
	slt	$t0, $s0, $zero
	li	$t1, 1
	beq	$t0, $t1, gen_failure
	slti	$t0, $s0, 21
	beq	$t0, $zero, gen_failure
	j	gen_success
gen_failure:					#If the generation number is invalid, ask them to try again
	li	$v0, PRINT_STRING
	la	$a0, illegal_generations
	syscall
	j	read_generations
gen_success:					#If the generation number is valid, save it in memory
	la	$t0, generations
	sw	$s0, 0($t0)


						#
						#Get the number of live cells to start
						#
	li	$v0, PRINT_STRING		#Print the prompt for the live cells
	la	$a0, prompt_live_cells
	syscall

read_cells:
	li	$v0, READ_INT			#Read the user's number of live cells
	syscall
	move	$s0, $v0
	move	$t0, $zero
	slt	$t0, $s0, $zero
	li	$t1, 1
	beq	$t0, $t1, cells_failure	
	la	$s1, board_size
	lw	$s1, 0($s1)
	mul	$s1, $s1, $s1
	addi	$s1, $s1, 1
	slt	$t0, $s0, $s1
	beq	$t0, $zero, cells_failure
	j	cells_success
cells_failure:					#If the number of live cells is invalid, ask them to try again
	li	$v0, PRINT_STRING
	la	$a0, illegal_cells
	syscall
	j	read_cells
cells_success:					#If the number of live cells is valif, save it in memory
	la	$t0, live_cells
	sw	$s0, 0($t0)


						#
						#Get the locations of the live cells
						#
	li	$v0, PRINT_STRING		#Print the prompt for the locations
	la	$a0, prompt_start_loc
	syscall
	
read_loc:
	la	$s0, board_size			#Read the board size and number of live cells
	lw	$s0, 0($s0)			#from memory
	la	$s1, live_cells
	lw	$s1, 0($s1)
read_loop:
	beq	$s1, $zero, read_end		#Loop as many times as there are live cells
	li	$v0, READ_INT			#Read in the row number 
	syscall
	move	$s2, $v0			#Read in the column number
	li	$v0, READ_INT
	syscall
	move	$s3, $v0
	addi	$t2, $s2, 1
	addi	$t3, $s3, 1
	slt	$t0, $zero, $t2			#Make sure the row and column
	beq	$t0, $zero, loc_failure		#are within the bounds of the board
	slt	$t0, $zero, $t3			 
	beq	$t0, $zero, loc_failure		
	slt	$t0, $s2, $s0
	beq	$t0, $zero, loc_failure
	slt	$t0, $s3, $s0
	beq	$t0, $zero, loc_failure
	la	$s4, board			#Get a pointer to the board
	mul	$s2, $s2, $s0			#and use the row and column numbers
	li	$t0, 4				#to move it to the correct location
	mul	$s2, $s2, $t0			#in memory
	mul	$s3, $s3, $t0
	add	$s4, $s4, $s2
	add	$s4, $s4, $s3
	li	$t0, 65
	lw	$t1, 0($s4)
	beq	$t0, $t1, loc_failure		#Make sure a cell doesn't already live at that loc
	sw	$t0, 0($s4)			#Save the cell to that location in memory
	addi	$s1, $s1, -1
	j	read_loop
loc_failure:					#Upon failure, print the error message
	li	$v0, PRINT_STRING		#and exit the program
	la	$a0, illegal_loc
	syscall
	li	$v0, EXIT
	syscall
read_end:
	li 	$a0, 0
	li	$a1, 0
	jal	draw_board
	la	$s0, generations
	lw	$s0, 0($s0)
	li	$a0, 1 
	li	$a1, 1
main_loop:					#Loop through the number of generations
	jal	process_generation		#Process and draw the board at each iteration
	jal	draw_board
	addi	$s0, $s0, -1
	addi	$a0, $a0, 1
	li	$t0, 1
	xor	$a1, $a1, $t0
	beq	$s0, $zero, main_end
	j	main_loop
main_end:

main_done:					#Move the stack frame back 
	lw	$ra, 0($sp)			#and restore the saved registers
	lw	$s0, 4($sp)
	lw	$s1, 8($sp)
	lw	$s2, 12($sp)
	lw	$s3, 16($sp)
	lw	$s4, 20($sp)
	lw	$s5, 24($sp)
	lw	$s6, 28($sp)
	lw	$s7, 32($sp)
	addi	$sp, $sp, 36
	jr	$ra	

#
#Draw Board Function
#
# a0- Generation Number
#
# a1- If 0, draw board A. If 1, draw board B.
#
# This function draws the board and generation header 
#
draw_board:
	addi	$sp, $sp, -44			#Move the stack frame
	sw	$ra, 0($sp)
	sw	$s0, 4($sp)
	sw	$s1, 8($sp)
	sw	$s2, 12($sp)
	sw	$s3, 16($sp)
	sw	$s4, 20($sp)
	sw	$s5, 24($sp)
	sw	$s6, 28($sp)
	sw	$s7, 32($sp)
	sw	$a0, 36($sp)
	sw	$a1, 40($sp)

	la	$s1, board_size			#Load the board size
	lw	$s1, 0($s1)	

	move	$s0, $a0			#Save the parameters
	move	$s7, $a1


	la	$a0, first_piece		#Draw the generation header
	li	$v0, PRINT_STRING
	syscall
	move	$a0, $s0
	li	$v0, PRINT_INT
	syscall
	la	$a0, second_piece
	li	$v0, PRINT_STRING
	syscall


						
	li	$a0, CORNER			#Draw the top of the board
	li	$v0, PRINT_CHAR
	syscall

	move	$s2, $zero
	li	$a0, HORIZONTAL
	li	$v0, PRINT_CHAR
top_loop:
	beq	$s2, $s1, top_end
	syscall
	addi	$s2, $s2, 1
	j	top_loop
top_end:
	li	$a0, CORNER
	syscall
	la	$a0, newline
	li	$v0, PRINT_STRING
	syscall



	beq	$s7, $zero, A			#Load the board pointer
	la	$s3, board_b
	j	next
A:
	la	$s3, board
next:
	move	$s2, $zero
	move	$s4, $zero



middle_loop:					#Loop through the rows of the board
	beq	$s4, $s1, middle_end
	li	$a0, VERTICAL			#Print the first line
	li	$v0, PRINT_CHAR
	syscall
middle_inner_loop:				#Loop through the columns in that row of the board
	beq	$s2, $s1, middle_inner_end
	lw	$s5, 0($s3)			#Load the cell
	beq	$s5, $zero, dead_cell		#If it's dead, print a space
	move	$a0, $s5
	li	$v0, PRINT_CHAR			#Else, print the letter representing its age
	syscall
	addi	$s2, $s2, 1			#Move the pointer to the next cell
	addi	$s3, $s3, 4
	j	middle_inner_loop
dead_cell:
	li	$a0, DEAD
	li	$v0, PRINT_CHAR
	syscall
	addi 	$s2, $s2, 1
	addi	$s3, $s3, 4
	j	middle_inner_loop
middle_inner_end:
	li	$a0, VERTICAL			#Print the last line in the row
	li	$v0, PRINT_CHAR
	syscall
	la	$a0, newline			#Print a new line
	li	$v0, PRINT_STRING
	syscall
	move	$s2, $zero			#Increment the counters for the loops
	addi	$s4, $s4, 1
	j	middle_loop
middle_end:


	li	$a0, CORNER			#Print the bottom line of the board
	li	$v0, PRINT_CHAR
	syscall

	move	$s2, $zero
	li	$a0, HORIZONTAL
	li	$v0, PRINT_CHAR
bot_loop:
	beq	$s2, $s1, bot_end
	syscall
	addi	$s2, $s2, 1
	j	bot_loop
bot_end:
	li	$a0, CORNER
	syscall
	la	$a0, newline
	li	$v0, PRINT_STRING
	syscall

	lw	$ra, 0($sp)			#Move the stack pointers back
	lw	$s0, 4($sp)
	lw	$s1, 8($sp)
	lw	$s2, 12($sp)
	lw	$s3, 16($sp)
	lw	$s4, 20($sp)
	lw	$s5, 24($sp)
	lw	$s6, 28($sp)
	lw	$s7, 32($sp)
	lw	$a0, 36($sp)
	lw	$a1, 40($sp)
	addi	$sp, $sp, 44
	jr	$ra	

#
#Process Generation Function
#
# a1- determines which board is being written to, and which is being read from
#
#
# The function will read from one board, and determine the next state of each
# cell and write it to the next board
#
process_generation:
	addi	$sp, $sp, -40			#Move the stack frame
	sw	$ra, 0($sp)
	sw	$s0, 4($sp)
	sw	$s1, 8($sp)
	sw	$s2, 12($sp)
	sw	$s3, 16($sp)
	sw	$s4, 20($sp)
	sw	$s5, 24($sp)
	sw	$s6, 28($sp)
	sw	$s7, 32($sp)
	sw	$a1, 36($sp)

	la	$s0, board_size
	lw	$s4, 0($s0)
	li	$s1, 0
	li	$s0, 0
	li	$s5, 0
	beq	$a1, $zero, process_a			#Use the param to determin
	la	$s3, board_b				#which board is being read from
	la	$s2, board				#and which is being written to
	j	after_load
process_a:
	la	$s3, board
	la	$s2, board_b
after_load:
		
process_loop:						#Loop through the columns
	beq	$s0, $s4, process_loop_end
process_inner_loop:					#Loop through the rows
	beq	$s1, $s4, process_inner_loop_end
	li	$t0, 4
	mul	$s7, $s4, $t0

							#In each of the next sections 
							#from Top Left to Bottom Right
							#the neighbors of the cell currently be examined will be checked
							#and if there is a cell inhabiting the space, one is added to the 
							#running tally of neighbors
	
							#Top Left
	sub	$s6, $s2, $t0
	sub	$s6, $s6, $s7
	beq	$s0, $zero, tl_top
	j	tl_post_top	
tl_top:
	mul	$t1, $s7, $s4
	add	$s6, $s6, $t1
tl_post_top:	
	beq	$s1, $zero, tl_left
	j	tl_post_left
tl_left:
	add	$s6, $s6, $s7
tl_post_left:
	lw	$t2, 0($s6)
	beq	$t2, $zero, tl_no_cell
	addi	$s5, 1
tl_no_cell:


							#Top
	sub	$s6, $s2, $s7
	beq	$s0, $zero, t_top
	j	t_post_top
t_top:
	mul	$t1, $s7, $s4
	add	$s6, $s6, $t1
t_post_top:
	lw	$t2, 0($s6)
	beq	$t2, $zero, t_no_cell
	addi	$s5, 1
t_no_cell:

	
							#Top Right
	add	$s6, $s2, $t0
	sub	$s6, $s6, $s7
	beq	$s0, $zero, tr_top
	j	tr_post_top
tr_top:
	mul	$t1, $s7, $s4
	add	$s6, $s6, $t1
tr_post_top:
	addi	$t7, $s4, -1	
	beq	$s1, $t7, tr_right
	j	tr_post_right
tr_right:
	sub	$s6, $s6, $s7
tr_post_right:
	lw	$t2, 0($s6)
	beq	$t2, $zero, tr_no_cell
	addi	$s5, 1
tr_no_cell:


							#Left
	sub	$s6, $s2, $t0	
	beq	$s1, $zero, l_left
	j	l_post_left
l_left:
	add	$s6, $s6, $s7
l_post_left:
	lw	$t2, 0($s6)
	beq	$t2, $zero, l_no_cell
	addi	$s5, 1
l_no_cell:

	
							#Right
	add	$s6, $s2, $t0
	addi	$t7, $s4, -1	
	beq	$s1, $t7, r_right
	j	r_post_right
r_right:
	sub	$s6, $s6, $s7
r_post_right:
	lw	$t2, 0($s6)
	beq	$t2, $zero, r_no_cell
	addi	$s5, 1
r_no_cell:


							#Bottom Left
	sub	$s6, $s2, $t0
	add	$s6, $s6, $s7
	addi	$t7, $s4, -1
	beq	$s0, $t7, bl_bot
	j	bl_post_bot
bl_bot:
	mul	$t1, $s7, $s4
	sub	$s6, $s6, $t1
bl_post_bot:	
	beq	$s1, $zero, bl_left
	j	bl_post_left
bl_left:
	add	$s6, $s6, $s7
bl_post_left:
	lw	$t2, 0($s6)
	beq	$t2, $zero, bl_no_cell
	addi	$s5, 1
bl_no_cell:

	
							#Bottom
	add	$s6, $s2, $s7
	addi	$t7, $s4, -1
	beq	$s0, $t7, b_bot
	j	b_post_bot
b_bot:
	mul	$t1, $s7, $s4
	sub	$s6, $s6, $t1
b_post_bot:	
	lw	$t2, 0($s6)
	beq	$t2, $zero, b_no_cell
	addi	$s5, 1
b_no_cell:


							#Bottom Right
	add	$s6, $s2, $t0
	add	$s6, $s6, $s7
	addi	$t7, $s4, -1
	beq	$s0, $t7, br_bot
	j	br_post_bot
br_bot:
	mul	$t1, $s7, $s4
	sub	$s6, $s6, $t1
br_post_bot:
	addi	$t7, $s4, -1	
	beq	$s1, $t7, br_right
	j	br_post_right
br_right:
	sub	$s6, $s6, $s7
br_post_right:
	lw	$t2, 0($s6)
	beq	$t2, $zero, br_no_cell
	addi	$s5, 1
br_no_cell:


						#Determine if cell lives or dies
	li	$t0, 65
	li	$t1, 1
	li	$t2, 2
	li	$t3, 3
	lw	$s6, 0($s2)
	beq	$s6, $zero, p_dead_cell		#Branch statements for each tally case
	beq	$s5, $zero, dying_cell
	beq	$s5, $t1, dying_cell
	beq	$s5, $t2, plus_age
	beq	$s5, $t3, plus_age
	j	dying_cell
plus_age:					#Add one generation to the cell
	lw	$t5, 0($s2)
	addi	$t5, 1
	sw	$t5, 0($s3)
	j	p_end
dying_cell:					#Cell dies
	sw	$zero, 0($s3)
	j	p_end
p_dead_cell:					#Determine if dead cell should be revivied
	beq	$s5, $t3, revived_cell
	sw	$zero, 0($s3)
	j	p_end
revived_cell:					#Dead cell is risen
	sw	$t0, 0($s3)
	j	p_end

p_end:
	li	$s5, 0				#Increment counters for loop
	addi	$s1, 1				#Increment board pointers
	addi	$s2, 4
	addi	$s3, 4
	j	process_inner_loop
process_inner_loop_end:
	addi	$s0, $s0, 1
	li	$s1, 0
	j	process_loop
process_loop_end:



	lw	$ra, 0($sp)			#Move the stack pointers back
	lw	$s0, 4($sp)
	lw	$s1, 8($sp)
	lw	$s2, 12($sp)
	lw	$s3, 16($sp)
	lw	$s4, 20($sp)
	lw	$s5, 24($sp)
	lw	$s6, 28($sp)
	lw	$s7, 32($sp)
	lw	$a1, 36($sp)
	addi	$sp, $sp, 40
	jr	$ra	


