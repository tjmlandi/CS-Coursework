#Thomas Landi

from PIL import Image
import heapq
import math

dataMatrix = []

#Reads in the data from the provided text and image files
def ReadMaps(textName, imageName):
	elevation = open(textName)
	elevationData = elevation.read().split()
	
	terrain = Image.open(imageName)
	terrain.load()
	terrainData = terrain.getdata()	
		
	#Build the data matrix containing the elevation and terrain data
	for line in range(0,500):
		dataMatrix.append([])
		for value in range(0,395):
			dataMatrix[line].append([])
			dataMatrix[line][value].append(float(elevationData[(line * 400) + value]))
			dataMatrix[line][value].append(terrainData[(line * 395) + value])
			
	return dataMatrix

#Returns multipliers for the distance for each terrain
def TerrainMultiplier(terrain):
	return {
		(248,148,18,255) : 1,
		(255,192,0,255) : 1.5,
		(255,255,255,255) : 1.1,
		(2,208,60,255) : 1.6,
		(2,136,40,255) : 2,
		(5,73,24,255) : 3,
		(0,0,255,255) : 4,
		(71,51,3,255) : 0.6,
		(0,0,0,255) : 0.8,
		(0,0,0,0) : 0.8,
		(205,0,101,255) : float("inf"),
	} [terrain]

#Given a current step and overall goal, calculate the heuristic/cost	
def Heuristic(start, next, end):
	eval = 0
	
	#First, figure out the actual distance moved
	if start[0] != next[0]:
		if start[1] != next[1]:
			eval += 12.76
		else:
			eval += 7.55
	else:
		eval += 10.29		
		
	#Weight the result heavier if moving further from the goal
	dX2 = (end[0] - next[0]) * (end[0] - next[0])
	dY2 = (end[1] - next[1]) * (end[1] - next[1])	
	eval += (dX2 + dY2) / 100000
		
	#Get the terrain for the next step, and multiply the current distance by it's corresponding multipler
	terrain = dataMatrix[next[0]][next[1]][1]
	eval *= TerrainMultiplier(terrain)	

	#Get the grade of the elevation change and weight the result based on how much it will change
	curElevation = dataMatrix[start[0]][start[1]][0]
	nextElevation = dataMatrix[next[0]][next[1]][0]
	
	dElevation = nextElevation - curElevation	
	grade = dElevation / eval
	eval *= math.pow(2, grade)
		
	return eval

#Returns a list of points to travel along to get from one point to another	
def FindPathTwoPoints(start, end):
	metaData = []
	nodeQueue = []
	#Fill in the matrix and queue keeping track of the distance to each point and the previous step to each point
	for x in range(0,500):
		metaData.append([])
		for y in range(0,395):
			metaData[x].append([])
			metaData[x][y].append(float("inf"))			
			metaData[x][y].append(None)
			if x == start[0] and y == start[1]:
				heapq.heappush(nodeQueue, [0, x, y])
			else:
				heapq.heappush(nodeQueue, [float("inf"), x, y])
	
	metaData[start[0]][start[1]][0] = 0
	
	prev = start[:]

	#Loop while there are points in our queue
	while len(nodeQueue) > 0:
		#Get the closest, un explored point
		currentNode = heapq.heappop(nodeQueue)
		
		#If it's our goal, exit the loop
		if currentNode[1] == end[0] and currentNode[2] == end[1]:
			prev[0] = currentNode[1]
			prev[1] = currentNode[2]
			break
		
		#Otherwise, look in the eight directions from the current points
		for x in range(-1,2):
			for y in range(-1,2):				
				if (x == 0 and y == 0) or currentNode[1]+x<0 or currentNode[2]+y<0 or currentNode[1]+x>499 or currentNode[2]+y>394:
					continue
				#Calculate our cost and current distance to point
				dist = Heuristic([currentNode[1],currentNode[2]],[currentNode[1]+x,currentNode[2]+y], end)
				currentValue = metaData[currentNode[1]][currentNode[2]][0] + dist
				#Determine if the meta data needs to be updated with a lower cost
				if currentValue < metaData[currentNode[1]+x][currentNode[2]+y][0]:									
					nodeQueue.remove([metaData[currentNode[1]+x][currentNode[2]+y][0], currentNode[1]+x, currentNode[2]+y])
					heapq.heapify(nodeQueue)
					metaData[currentNode[1]+x][currentNode[2]+y][0] = currentValue
					metaData[currentNode[1]+x][currentNode[2]+y][1] = [currentNode[1],currentNode[2]]
					heapq.heappush(nodeQueue, [currentValue, currentNode[1]+x, currentNode[2]+y])
	#Build the path to return
	path = []
	path.append(prev)
	while prev[0] != start[0] or prev[1] != start[1]:
		prev = [metaData[prev[0]][prev[1]][1][0], metaData[prev[0]][prev[1]][1][1]]
		path.append(prev)
		
	return path

#Draws a three pixel wide, light blue path in an output file based on the provided list of points	
def DrawPath(path):
	output = Image.open("terrain.png")
	pic = output.load()
	
	for pixel in path:
		for x in range(-1,2):
			for y in range(-1,2):
				pic[pixel[0]+x,pixel[1]+y] = (0,191,255,255)		
				
	output.save("output.png")
	
#Maps the provied course
def MapFullCourse(course):
	ReadMaps("mpp.txt", "terrain.png")
	courseData = open(course)
	points = courseData.read().split()
	pairs = []
	for x in range(0, len(points), 2):
		pairs.append([int(points[x]), int(points[x+1])])
		
	path = []
	for x in range(0, len(pairs) - 1):
		path.extend(FindPathTwoPoints(pairs[x],pairs[x+1]))
	DrawPath(path)
	
#This will run the program with the white trail	
MapFullCourse("white.txt")