#Thomas Landi

def reverseInt(i):
	#save whether the result is negative
	isNegative = (i < 0)
	result = 0
	i = abs(int(i))
	#loop through the digits of i
	while (i > 0):
		#get the next digit
		add = i%10
		#make sure that the result is still not greater than 20000
		if (result * 10 + add) > 20000:
			result = 0
			break
		#append the latest digit
		result *= 10
		result += add
		#remove the last digit from i
		i -= add
		i /= 10
	#restore negativity		
	if isNegative:
		result *= -1
	return int(result)