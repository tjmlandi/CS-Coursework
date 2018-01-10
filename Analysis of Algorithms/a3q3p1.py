class Solution(object):
	def validArray(self, nums):
		#If the first value is zero, we can't move forward, so we already know it is not valid
		if nums[0] == 0:
			return False
		#If this number can get us to the end in a single hop, then we know we have a path, and the array is valid
		if nums[0] >= len(nums) - 1:
			return True
		#Recursively Check all possible paths, from hopping once, to hopping [the first value] times
		for x in range(1, nums[0] + 1):
			if self.validArray(nums[x:]):
				return True
		#If none of those returned a valid path, than this is an invalid array
		return False