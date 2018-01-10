class Solution(object):
	def exploreMatrixWithPits(self, matrix):
		#Check if the upper left corner is a pit, if so, return 0
		if matrix[0][0] == 1:
			return 0
		else:
		#Otherwise, initialize it to 1
			matrix[0][0] = 1
		m = len(matrix)
		n = len(matrix[0])
		#Loop through the matrix, from left to right and top to bottom
		for i in range(m):
			for j in range(n):
				if j != 0 or i != 0:
					#If the current location is a pit, set it to have 0 paths to it
					if matrix[i][j] == 1:
						matrix[i][j] = 0		
					#Otherwise, get the paths to the locations above and to the left
					#and set the value for the current path to their sum
					else:
						if i == 0:
							above = 0
						else:
							above = matrix[i - 1][j]
						if j == 0:
							left = 0
						else:
							left = matrix[i][j - 1]
						matrix[i][j] = above + left
		#Return the value in the lower right corner of the matrix
		return matrix[m - 1][n - 1]