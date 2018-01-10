class Solution(object):
	def playOrNotPlay(self, n, m):
		#Initialize matrix of 0's
		valMatrix = [[0 for x in range(n+2)] for y in range(n+2)]
		#Loop through the matrix, from right to left, and diagonally top to bottom
		for l in range(n, 0, -1):
			for h in range(l+1, n+1):
				lst = []
				#Get the minimum of all of the left and right values
				for i in range(l, h+1):
					lst.append(i + max(valMatrix[l][i - 1], valMatrix[i + 1][h]))
				valMatrix[l][h] = min(lst)
		#Check if within budget
		if valMatrix[1][n] > m:
			return 0
		else:
			return 1