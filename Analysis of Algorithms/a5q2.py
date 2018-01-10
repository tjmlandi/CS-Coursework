#Thomas Landi
#Helper class to keep track of vertex sets
class UnionFind(object):
	def __init__(self, V):
		self.Boss = []
		self.Size = []
		self.Set = []
		#Construct the three lists out of the provided vertices
		for vertex in V:
			self.Boss.append(vertex)
			self.Size.append(1)
			self.Set.append([vertex])
	
	#Returns the boss of the provided vertex
	def Find(self, u):
		return self.Boss[u]

	#Unions the sets of the two provided vertices
	def Union(self, u,v):
		#Checks which set is smaller, as we will only iterate through the smaller one
		if self.Size[self.Boss[u]]>self.Size[self.Boss[v]]:
			#Union the sets, and store the union in the bigger set
			self.Set[self.Boss[u]] = list(set().union(self.Set[self.Boss[u]], self.Set[self.Boss[v]]))
			#Update the size of the bigger set
			self.Size[self.Boss[u]] += self.Size[self.Boss[v]]
			#Loop through and update the boss of the smaller set
			for vert in self.Set[self.Boss[v]]:
				self.Boss[vert] = self.Boss[u]
		#Same as above, but the opposite case
		else:	
			self.Set[self.Boss[v]] = list(set().union(self.Set[self.Boss[v]], self.Set[self.Boss[u]]))
			self.Size[self.Boss[v]] += self.Size[self.Boss[u]]
			for vert in self.Set[self.Boss[u]]:
				self.Boss[vert] = self.Boss[v]	

class Solution(object):
	def Kruskal(self, V, E):
		#Sort the edges by wait
		#Python uses Timsort here, which has a time complexity of O(n log n)
		sortedE = sorted(E, key=lambda edge: edge[2])
		#Initialize list of vertices to return
		tree = []
		#Initialize the union-find array object
		unionFind = UnionFind(V)
		#Loop through the sorted edges in ascending order
		for edge in sortedE:
			#If the edge would not create a cycle
			if unionFind.Find(edge[0]) != unionFind.Find(edge[1]):
				#Add the edge to the tree
				tree.append(edge)
				#Union the two sets corresponding to the vertices of the edge
				unionFind.Union(edge[0], edge[1])
		#Return the list of edges
		return tree