from string import ascii_lowercase
	
def ValidSuccessors(dict, word):
	successors = []
	for x in range(0, len(word)):
		for letter in ascii_lowercase:
			if letter != word[x]:
				modified = list(word)
				modified[x] = letter
				modified = "".join(modified)
				if modified in dict:
					successors.append(modified)
	return successors

def WordPath (dict, startWord, endWord):
	if startWord == endWord:
		return endWord
	tentStates = []
	tentStates.append([startWord, 0])
	path = []
	notFound = True
	while len(tentStates) > 0 and notFound:
		currentState = tentStates[0]
		tentStates.remove(currentState)
		successors = ValidSuccessors(dict, currentState[0])
		for successor in successors:
			if successor == endWord:
				while len(currentState) > 2:
					path.insert(0,currentState[0])
					currentState = currentState[2]
					notFound = False
			tentStates.append([successor, currentState[1] + 1, currentState])
			dict.remove(successor)
	if len(path) > 0:
		print (startWord, end='')
		for word in path:
			print (' ' + word, end='')
		print (' ' + endWord)
	else:
		print ("Path Not Found")
	
dict = open('words', 'r')
dictRead = []

for line in dict:
	dictRead.append(line[:-1])
	
WordPath(dictRead, "cold", "warm")
WordPath(dictRead, "small", "short")
WordPath(dictRead, "here", "bore")