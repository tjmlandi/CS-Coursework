#Thomas Landi

#Node class, holds the current number of candies, and references to it's root, and two leaves
class Node():
	def __init__(self, numCandies):
		self.numCandies = numCandies
		self.prev = None
		self.ref1 = None
		self.ref2 = None
		
def candyIterative(n):
	count = 0
	root = Node(n)
	current = root
	#loop while there is still a tree
	while (current is not None):
		#check 1 candy leaf if not yet checked
		if current.ref1 is None:
			#see if we can get to 0, if not, go further down 1 candy
			if (current.numCandies - 1 > 0):
				current.ref1 = Node(current.numCandies - 1)
				current.ref1.prev = current
				current = current.ref1
			#if so, increment the path count
			else:
				count += 1
				current = current.prev
		#check 2 candy leaf if not yet checked
		elif current.ref2 is None:
			#see if we can get to 0, if not, go further down 2 candies
			if (current.numCandies - 2 > 0):
				current.ref2 = Node(current.numCandies - 2)
				current.ref2.prev = current
				current = current.ref2
			#if so, increment the path count
			elif (current.numCandies - 2 == 0):
				count += 1
				current = current.prev
		else:
			current = current.prev
	return count