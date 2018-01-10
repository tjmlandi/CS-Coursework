class Solution(object):
	def exploreMatrix(self, m, n):
		#Matrix to store intermediate path numbers for sub matrices
		valMatrix = [[0 for x in range(n)] for y in range(m)]
		#Initialize the smallest sub matrix to one path
		valMatrix[0][0] = 1
		#Loop through the matrix, from left to right and then top to bottom
		for i in range(n):
			for j in range(m):
				#Ignore the very first
				if j != 0 or i != 0:
					#Take values of 0 for the left or above if we are 
					#at the top or left side of the matrix
					if i == 0:
						above = 0
					else:
						above = valMatrix[j][i - 1]
					if j == 0:
						left = 0
					else:
						left = valMatrix[j - 1][i]
					#The number of paths to this location will be the number of paths
					#to the one above it, plus the number of paths to the one to the left
					#of it 
					valMatrix[j][i] = above + left
		#Return the number of paths to the lower right corner
		return valMatrix[m - 1][n - 1]