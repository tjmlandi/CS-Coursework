#Thomas Landi

def candyRecursive(n):
	n = int(n)
	count = 0
	#end cases, once n hits zero, it is a valid path to empty the jar
	if n == 0:
		return 1
	elif n < 0:
		return 0
	else:
		#Recursively go call the function for each case of taking 1 or 2
		count += candyRecursive(n - 1)
		count += candyRecursive(n - 2)
	return count