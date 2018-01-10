#Thomas Landi, Intro to Intelligent Systems, Lab 2

from math import log, log10
from collections import Counter
import string
import random
import sys

#These are the filenames of the text samples used for testing. In order to include different files, just remove the ones 
#you do not want to include from these lists, and include the filenames of desired ones, running the script in the 
#directory with all listed files
engSamples = ["eng1.txt", "eng2.txt", "eng3.txt", "eng4.txt", "eng5.txt", "eng6.txt", "eng7.txt", "eng8.txt", "eng9.txt", "eng10.txt", "eng11.txt", "eng12.txt", "eng13.txt", "eng14.txt", "eng15.txt"]
dutSamples = ["dut1.txt", "dut2.txt", "dut3.txt", "dut4.txt", "dut5.txt", "dut6.txt", "dut7.txt", "dut8.txt", "dut9.txt", "dut10.txt", "dut11.txt", "dut12.txt", "dut13.txt", "dut14.txt", "dut15.txt"]

#Depth parameter for AdaBoosting
K = 4

vowels = ["a", "e", "i", "o", "u"]

#Determines if using tree or boosting algorithm
NoBoost = True

#Main class for the decision trees
class decisionNode:
	def __init__(self, qN, tN, fN):
		self.questionNum = qN
		self.trueNode = tN
		self.falseNode = fN
		self.result = None
		
	def printTree(self):
		if self.questionNum is not None:
			print "Question Number: " + str(self.questionNum)
		if self.result is not None:
			print "Result: " + str(self.result)
		if self.trueNode is not None:
			print "Left Node"
			self.trueNode.printTree()
		else:
			print "no left node"
		if self.falseNode is not None:
			print "Right Node"
			self.falseNode.printTree()
		else:
			print "no right node"

#if NoBoost is true, runs the decision tree building
#else, runs the AdaBoost algorithm
def train():
	if NoBoost:
		return buildDecisionTree(engSamples, dutSamples)
	return adaboost(engSamples, dutSamples)
	
#Builds a decision tree with the list of input examples names	
def buildDecisionTree(engFilenames, dutFilenames):
	trainingData = []
	
	#Get the list of attributes for each file
	for filename in engFilenames:
		lst = analyzeFile(filename)
		lst.append("E")
		trainingData.append(lst)
		
	for filename in dutFilenames:
		lst = analyzeFile(filename)
		lst.append("D")
		trainingData.append(lst)
		
	#Recursively build the decision tree
	attr = []
	first = decision(trainingData, attr[:])
	attr.append(first)
	
	decisionTreeRoot = decisionNode(first, None, None)
	
	treeRecur(decisionTreeRoot, trainingData, attr)
	
	return decisionTreeRoot
	
#Returns the language of the given file, using the provided decision tree to determine
def testInput(treeRoot, filename):		
	attrLst = analyzeFile(filename)
	curNode = treeRoot
	
	while curNode.questionNum is not None and curNode.result is None:
		if attrLst[curNode.questionNum] == "true":
			if curNode.trueNode is not None:
				curNode = curNode.trueNode
			else:
				curNode = curNode.falseNode
		else:
			if curNode.falseNode is not None:
				curNode = curNode.falseNode
			else:
				curNode = curNode.trueNode
				
	return curNode.result
	
#Runs the AdaBoost algorithm on the provided files as examples	
def adaboost(engFilenames, dutFilenames):
	#Total number of examples
	N = len(engFilenames) + len(dutFilenames)
	#Weights of each example
	w = [(1.0/N) for x in range(N)]
	#Hypotheses
	h = [None for x in range(K)]
	#Weights of Hypotheses
	z = [None for x in range(K)]
	
	#Loop to our depth constant
	for k in range(0, K):
		weightedEngFilenames = []
		weightedDutFilenames = []
		
		#Calculate how many times each example should be put into the list going through the learning algorithm
		#in order to reflect their respective weights
		smallestWeight = min(w)
		
		for x in range(0, len(w)):
			num = w[x] / smallestWeight
			leftover = num % 1
			for y in range(0, int(num)):
				if x < len(engFilenames):
					weightedEngFilenames.append(engFilenames[x])
				else:
					weightedDutFilenames.append(dutFilenames[x - len(engFilenames)])
			if random.uniform(0,1) < leftover:				
				if x < len(engFilenames):
					weightedEngFilenames.append(engFilenames[x])
				else:
					weightedDutFilenames.append(dutFilenames[x - len(engFilenames)])
			
		#Get the current hypothesis
		h[k] = buildDecisionTree(weightedEngFilenames, weightedDutFilenames)
		error = 0.0
		#Adjust error rate for incorrect examples
		for j in range(0, N):
			if j < len(engFilenames):
				result = testInput(h[k], engFilenames[j])
				if result != "E":
					error += w[j]
			else:
				result = testInput(h[k], dutFilenames[j - len(engFilenames)])
				if result != "D":
					error += w[j]
			
		#Adjust weight for correct examples
		for j in range(0, N):
			if j < len(engFilenames):
				result = testInput(h[k], engFilenames[j])
				if result == "E":
					w[j] *= error/(1 - error)
			else:
				result = testInput(h[k], dutFilenames[j - len(engFilenames)])
				if result == "D":
					w[j] *= error/(1 - error)
					
		#Normalize the weights
		totalWeight = sum(w)
		for weight in w:
			weight /= totalWeight
		z[k] = log10((1-error)/error)
		
	#Return the weighted majority hypothesis
	return h[z.index(max(z))]

#Recursive algorithm for building a decision tree	
def treeRecur(root, data, attr):
	#End if we are out of attributes to test, saving the plurality result
	if len(attr) >= 10:
		eCount = 0
		dCount = 0
		for datum in data:
			if datum[10] == "E":
				eCount += 1
			else:
				dCount += 1
		if dCount > eCount:
			root.result = "D"
		else:
			root.result = "E"
		return
		
	#Get the data for if the question is answered true
	dataTrue = [datum for datum in data if datum[root.questionNum] == "true"]
	#Determine if true node needs to be split further, or if the result can be stored
	if len(dataTrue) > 0:
		trueDecided = True
		for datum in dataTrue:
			if datum[10] != dataTrue[0][10]:
				trueDecided = False
		
		if trueDecided:
			root.trueNode = decisionNode(None, None, None)
			root.trueNode.result = dataTrue[0][10]
		else:
			nextQ = decision(dataTrue, attr[:])
			attr.append(nextQ)
			root.trueNode = decisionNode(nextQ, None, None)
			treeRecur(root.trueNode, dataTrue, attr)		
	#Repeat above for false possibility
	dataFalse = [datum for datum in data if datum[root.questionNum] != "true"]
	if len(dataFalse) > 0:
		falseDecided = True
		for datum in dataFalse:
			if datum[10] != dataFalse[0][10]:
				falseDecided = False
				
		if falseDecided:
			root.falseNode = decisionNode(None, None, None)
			root.falseNode.result = dataFalse[0][10]
		else:
			nextQ = decision(dataFalse, attr[:])
			attr.append(nextQ)
			root.falseNode = decisionNode(nextQ, None, None)
			treeRecur(root.falseNode, dataFalse, attr)
		
#Calculates the most gainful attribute of the provided data
def decision(data, toOmit):
	#Calculate total entropy
	engCount = 0.0
	totalCount = 0.0
	for datum in data:
		totalCount += 1.0		
		if datum[10] == 'E':
			engCount += 1.0
	
	q = engCount / totalCount		
	entropy = -((q * log(q,2)) + ((1-q) * log(1-q,2)))
		
	gain = []
	total = float(len(data))

	#Loop through the attributes, but skipping already visited ones
	for x in range(0, 10):
		if x not in toOmit:
			#Tally up truths, falses, english/dutch results
			falses = 0.0
			truths = 0.0
			engTruth = 0.0
			engFalse = 0.0
			for datum in data:
				if datum[10] == "E":
					if datum[x] == "true":
						engTruth += 1.0
						truths += 1.0
					else:
						engFalse += 1.0
						falses += 1.0
				else:			
					if datum[x] == "true":
						truths += 1.0
					else:
						falses += 1.0
						
			#Calculate both entropies
			outside1 = truths / total
			if truths != 0 and engTruth != truths and engTruth != 0:
				q1 = engTruth / truths
				entropy1 = -((q1 * log(q1,2)) + ((1-q1) * log(1-q1,2)))
			else:
				entropy1 = 0
			remainder1 = outside1 * entropy1
			
			outside2 = falses / total
			if falses != 0 and engFalse != falses and engFalse != 0:
				q2 = engFalse / falses
				entropy2 = -((q2 * log(q2,2)) + ((1-q2) * log(1-q2,2)))
			else:
				entropy2 = 0
			remainder2 = outside2 * entropy2
			
			#Subtract the remainders from the overall entropy and store the gain
			gain.append(entropy - (remainder1 + remainder2))
		
	largest = -1.0	
	
	toOmit.sort()
	
	for omission in toOmit:
		gain.insert(omission, -1)

	#Find the largest gain index and return it
	for gainx in gain:
		if gainx > largest:
			largest = gainx
			
	largestIndex = gain.index(largest)
	
	return largestIndex

#Pulls in a file and returns a list of it's attributes	
def analyzeFile(filename):
	#Pull in file
	trainingFile = open(filename, 'r')
	trainingData = []	
	words = []
	
	for line in trainingFile:
		for word in line[:-1].split(' '):
			words.append(word.translate(None, string.punctuation).lower())
	
	trainingFile.close()
	
	#Make checks, saving as list	
	results = []
	
	#1. Double Vowels
	doubVowel = False
	
	for word in words:
		for x in range(0, len(word) - 1):
			if word[x] == word[x + 1] and word[x] in vowels:
				doubVowel = True

	if doubVowel:
		results.append("true")
	else:
		results.append("false")
	#2. >5 Consecutive Vowels
	consecVowel = 0
	
	for word in words:
		for x in range(0, len(word) - 1):
			if word[x] in vowels and word[x + 1] in vowels:
				consecVowel += 1
				
	if consecVowel > 5:
		results.append("true")
	else:
		results.append("false")
	
	#3. >15 Consecutive Consonants
	consecCons = 0
	
	for word in words:
		for x in range(0, len(word) - 1):
			if word[x] not in vowels and word[x + 1] not in vowels:
				consecCons += 1
				
	if consecCons > 15:
		results.append("true")
	else:
		results.append("false")
	
	#4. Double Consonants
	doubCons = False
	
	for word in words:
		for x in range(0, len(word) - 1):
			if word[x] == word[x + 1] and word[x] not in vowels:
				doubCons = True
				
	if doubCons:
		results.append("true")
	else:
		results.append("false")
	
	#5. T more frequent than N
	tCount = 0
	nCount = 0
	
	for word in words:
		for x in range(0, len(word)):
			if word[x] == "t":
				tCount += 1
			if word[x] == "n":
				nCount += 1
				
	if tCount > nCount:
		results.append("true")
	else:
		results.append("false")
	
	#6. >40% vowels
	vowelCount = 0.0
	totalCount = 0.0
	
	for word in words:
		for x in range(0, len(word)):
			totalCount += 1
			if word[x] in vowels:
				vowelCount += 1
				
	if vowelCount / totalCount > 0.4:
		results.append("true")
	else:
		results.append("false")
	
	#7. >15% letter e
	eCount = 0.0
	
	for word in words:
		for x in range(0, len(word)):
			if word[x] == "e":
				eCount += 1
	
	if eCount / totalCount > 0.15:
		results.append("true")
	else:
		results.append("false")
	
	#8. >5 words <4 characters long
	wordCount = 0
	
	for word in words:
		if len(word) < 4:
			wordCount += 1
			
	if wordCount > 5:
		results.append("true")
	else:
		results.append("false")
	
	#9. Most common first letter is T
	firstLetters = []
	
	for word in words:
		firstLetters.append(word[0])
		
	data = Counter(firstLetters)
	if data.most_common(1)[0][0] == "t":
		results.append("true")
	else:
		results.append("false")
	
	#10. Does it contain z
	doesZ = False
	
	for word in words:
		for x in range(0, len(word)):
			if word[x] == "z":
				doesZ = True
				
	if doesZ:
		results.append("true")
	else:
		results.append("false")
	
	
	#Return list
	return results

#Run the prediction on the hard coded decision tree
def predict(filename):
	sampleRoot = decisionNode(0, None, None)
	sampleRoot.trueNode = decisionNode(9, None, None)
	sampleRoot.trueNode.trueNode = decisionNode(None, None, None)
	sampleRoot.trueNode.trueNode.result = "D"
	sampleRoot.trueNode.falseNode = decisionNode(1, None, None)
	sampleRoot.trueNode.falseNode.trueNode = decisionNode(3, None, None)
	sampleRoot.trueNode.falseNode.trueNode.trueNode = decisionNode(4, None, None)
	sampleRoot.trueNode.falseNode.trueNode.trueNode.trueNode = decisionNode(None, None, None)
	sampleRoot.trueNode.falseNode.trueNode.trueNode.trueNode.result = "D"
	sampleRoot.trueNode.falseNode.trueNode.trueNode.falseNode = decisionNode(None, None, None)
	sampleRoot.trueNode.falseNode.trueNode.trueNode.falseNode.result = "E"
	sampleRoot.trueNode.falseNode.trueNode.falseNode = decisionNode(None, None, None)
	sampleRoot.trueNode.falseNode.trueNode.falseNode.result = "D"
	sampleRoot.trueNode.falseNode.falseNode = decisionNode(2, None, None)
	sampleRoot.trueNode.falseNode.falseNode.trueNode = decisionNode(6, None, None)
	sampleRoot.trueNode.falseNode.falseNode.trueNode.trueNode = decisionNode(None, None, None)
	sampleRoot.trueNode.falseNode.falseNode.trueNode.trueNode.result = "E"
	sampleRoot.trueNode.falseNode.falseNode.trueNode.falseNode = decisionNode(7, None, None)
	sampleRoot.trueNode.falseNode.falseNode.trueNode.falseNode.trueNode = decisionNode(None, None, None)
	sampleRoot.trueNode.falseNode.falseNode.trueNode.falseNode.trueNode.result = "E"
	sampleRoot.trueNode.falseNode.falseNode.trueNode.falseNode.falseNode = decisionNode(5, None, None)
	sampleRoot.trueNode.falseNode.falseNode.trueNode.falseNode.falseNode.falseNode = decisionNode(8, None, None)
	sampleRoot.trueNode.falseNode.falseNode.trueNode.falseNode.falseNode.falseNode.result = "E"
	sampleRoot.trueNode.falseNode.falseNode.falseNode = decisionNode(0, None, None)
	sampleRoot.trueNode.falseNode.falseNode.falseNode.result = "E"
	
	result = testInput(sampleRoot, filename)
	
	if result == "E":
		print "The text was in English."
	else:
		print "The text was in Dutch."
	
#Parse command arguments and standard input
if len(sys.argv) < 2:
	print "Usage: Lab2.py [train/predict] [filename(optional)]"
	sys.exit()
	
if sys.argv[1] == "train":
	treeRoot = train()
	print "Pre-fix navigation of resultant decision tree"
	treeRoot.printTree()

else:
	if len(sys.argv) > 2:
		predict(sys.argv[2])
	else:
		example = raw_input("Enter text here: ")
		f = open("tmp.txt", "w+")
		f.write(example)
		f.close()
		predict("tmp.txt")