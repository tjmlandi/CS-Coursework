#Thomas Landi

def reverseStr(str):
	resultStr = ""
	#loop backwards through original string
	for i in range(len(str) - 1, -1, -1):
		#append each character to the end of the results string
		resultStr += str[i]
	return resultStr
