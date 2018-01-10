#Thomas Landi

class Solution (object):
	def findSmallest(self, arr):
		#Set our initial bounds
		left = 0
		middle = len(arr) // 2
		right = len(arr) - 1
		#Loop while we still have a range to check. Not necessary to have a condition here, can just loop on true
		while right - left > 0:
			#Check if the middle is our target
			if arr[middle] < arr[middle - 1] and arr[middle] < arr[middle + 1]:
				return arr[middle]
			#Check if the left is our target
			elif arr[left] < arr[middle] and arr[middle] < arr[right]:
				return arr[left]
			#Check if the right is our target
			elif arr[right] < arr[left] and arr[right] < arr[right - 1]:
				return arr[right]
			#We know that the partition containing the lowest will also have the highest value on its the left
			#so we reset our bounds based on which is higher, the left bound or the middle bound
			elif arr[left] < arr[middle]:
				left = middle
				middle = (right + left) // 2
			else:
				right = middle
				middle = (right + left) // 2