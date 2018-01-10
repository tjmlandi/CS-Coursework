class Solution(object):
	def minStep(self, nums):
		#return -1 if the first value is zero
		if nums[0] == 0:
			return -1
		#return 1 if the first value can make a hop to the end
		if nums[0] >= len(nums) - 1:
			return 1
		#return 0 if the array is of size 1, as the beginning is the end in this case
		if len(nums) == 1:
			return 0
		#set a lowest marker
		lowest = -1
		#iterate through from the first value, down to 1
		for x in range(nums[0], 0, -1):
			#get the min steps for the current loop
			tmp = self.minStep(nums[x:])
			#if the lowest hasn't been replaced, replace it
			if lowest == -1:
				if tmp != -1:
					lowest = tmp + 1
			#otherwise compare it, and replace it if it is lower
			elif tmp + 1 < lowest:
				lowest = tmp + 1
		#return the lowest amount of steps (or -1 if no path was found)
		return lowest