#Thomas Landi
class Solution(object):
	def numOfJungles(self, matrix):
		#Initialize our jungle count
		count = 0
		#Loop through the matrix from the top left to the bottom right
		for n in range(len(matrix)):
			for m in range(len(matrix[0])):
				#Grab the values to the left and above the current index
				#using 0 if we're at an edge
				if n == 0:
					above = 0
				else:
					above = matrix[n - 1][m]
				if m == 0:
					left = 0
				else:
					left = matrix[n][m - 1]
				#If both are 0 and the current value is 1, increase the count
				if above == 0 and left == 0 and matrix[n][m] == 1:
					count += 1
		#Return the count of jungles
		return count