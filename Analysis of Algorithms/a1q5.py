from heapq import heappush, heappop

def findKLoyalMatching(mLRisks, fLRisks, k):
	mIndex = 0
	fIndex = 0
	count = 0
	couples = []
	checkpoints = []
	#loop while we haven't found enough couples
	while count < k:
		#get the loyalty for our current position
		currentLoyalty = mLRisks[mIndex] + fLRisks[fIndex]
		#see if we have any 'checkpoints' for previous males we've passed over
		if len(checkpoints) > 0:
			#if so, loop while we still have them, making sure not to exceed our count
			while len(checkpoints) > 0 and count < k:
				temp = heappop(checkpoints)
				#if any are better than our current pair, add it to the list
				if temp[0] < currentLoyalty:
					#add checkpoint
					couples.append([temp[1], temp[2]])
					#print ('[m' + str(temp[1]) + ' , f' + str(temp[2]) + '],', end='')
					count += 1
					if temp[2] < len(fLRisks):
						tempLoyalty = mLRisks[temp[1]] + fLRisks[temp[2]]
						heappush(checkpoints, [tempLoyalty, temp[1], temp[2] + 1])
		#make sure we haven't exceeded the count since checking the checkpoints
		if count < k:
			#check the next male to see if they have a more loyal pair
			#however, if there is no next male, just proceed with adding pairs by incrementing the female count
			if mIndex < len(mLRisks) - 1:
				nextLoyalty = mLRisks[mIndex + 1] + fLRisks[0]
			else:
				nextLoyalty = currentLoyalty + 1 
			#if the next male isn't better, add our couple to the list, increment the female
			if currentLoyalty < nextLoyalty:
				#add current pair to list
				couples.append([mIndex, fIndex])
				#print ('[m' + str(mIndex) + ', f' + str(fIndex) + '],', end='') 
				fIndex += 1
				if (fIndex >= len(fLRisks)):
					mIndex += 1
					fIndex = 0
				count += 1
			#if they are the same, add them both, and move the next male
			#making sure to add the current male to the heap of checkpoints
			elif currentLoyalty == nextLoyalty:
				#add current pair to list
				couples.append([mIndex, fIndex])
				#print ('[m' + str(mIndex) + ', f' + str(fIndex) + '],', end='')
				#add next pair to list
				couples.append([mIndex + 1, 0])
				#print ('[m' + str(mIndex + 1) + ', f0],', end='')
				heappush(checkpoints, [currentLoyalty, mIndex, fIndex])
				mIndex += 1
				count += 1
				fIndex = 1
				if (mIndex >=  len(mLRisks)):
					count = k;
			#if the next male is better, move to the next male
			#add the pair and add the current one to the checkpoints
			else:
				#add next pair to list
				couples.append([mIndex + 1, 0])
				#print ('[m' + str(mIndex + 1) + ', f0],', end='')
				heappush(checkpoints, [currentLoyalty, mIndex, fIndex])
				mIndex += 1
				count += 1
				fIndex = 1
				if (mIndex >= len(mLRisks)):
					count = k;
	return couples