#Thomas Landi - Code for Homework 4 problem 4

from math import log

def Decision(filename):
	dataFile = open(filename, 'r')	
	data = []

	for line in dataFile:
		data.append(line[:-1])

	positiveCount = 0.0
	totalCount = 0.0
	
	for datum in data:
		totalCount += 1.0
		datumSplit = datum.split(' ')
		if datumSplit[8] == 'A':
			positiveCount += 1.0
		
	q = positiveCount / totalCount	
	entropy = -((q * log(q,2)) + ((1-q) * log(1-q,2)))
		
	gain = []
	total = float(len(data))
	
	for x in range(0, 8):
		falses = 0.0
		truths = 0.0
		positiveTruth = 0.0
		positiveFalse = 0.0
		for datum in data:
			datumSplit = datum.split(' ')			
			if datumSplit[8] == "A":
				if datumSplit[x] == "True":
					positiveTruth += 1.0
					truths += 1.0
				else:
					positiveFalse += 1.0
					falses += 1.0
			else:			
				if datumSplit[x] == "True":
					truths += 1.0
				else:
					falses += 1.0
					
			
		outside1 = truths / total
		q1 = positiveTruth / truths
		entropy1 = -((q1 * log(q1,2)) + ((1-q1) * log(1-q1,2)))
		remainder1 = outside1 * entropy1
		
		outside2 = falses / total
		q2 = positiveFalse / falses
		entropy2 = -((q2 * log(q2,2)) + ((1-q2) * log(1-q2,2)))
		remainder2 = outside2 * entropy2
		
		gain.append(entropy - (remainder1 + remainder2))
		
	largest = 0.0	
		
	for gainx in gain:
		if gainx > largest:
			largest = gainx
			
	largestIndex = gain.index(largest)
	
	largest = 0.0
		
	for gainx in gain:
		if gainx > largest and gain.index(gainx) != largestIndex:
			largest = gainx
			
	secondIndex = gain.index(largest)
	
	aCountTT = 0
	aCountTF = 0
	aCountFT = 0
	aCountFF = 0
	
	for datum in data:
		datumSplit = datum.split(' ')
		if datumSplit[largestIndex] == "True":
			if datumSplit[secondIndex] == "True":
				aCountTT += 1
			else:
				aCountTF += 1
		else:
			if datumSplit[secondIndex] == "True":
				aCountFT += 1
			else:
				aCountFF += 1
				
				
	print largestIndex
	print secondIndex
	print aCountTT
	print aCountTF
	print aCountFT
	print aCountFF
	
		
Decision("dtree-data")