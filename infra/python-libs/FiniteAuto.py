import pygraphviz as pgv


class Trans:
	def __init__( self, fromSt, toSt, cond= "", isToStAcceptAble = False):
		self.fromSt = fromSt
		self.toSt = toSt
		self.cond = cond
		self.isToStAcceptAble = isToStAcceptAble


class FiniteAuto:
	def __init__( self):
		self.transG = pgv.AGraph( strict = False, directed = True)
		self.edges = []
		self.acceptSts = set()
		pass

	def __addNode( self, node):
		"""Add one node to the graph, a node is added only once,
		with proper attribute set.
		"""
		if not self.transG.has_node( node):
			self.transG.add_node(
					node,
					shape = "doublecircle" if node in self.acceptSts else "circle")
		pass

	def __addEdges( self):
		"""Add edges to the graph
		"""
		for eachEdge in self.edges:
			n0 = eachEdge[ 0]
			n1 = eachEdge[ 1]
			label = eachEdge[ 2]

			self.__addNode( n0)
			self.__addNode( n1)
			self.transG.add_edge( n0, n1, label = label)
		pass

	def addTrans( self, transitions):
		"""Add a list of transitions to the finite automaton
		"""
		for eachTrans in transitions:
			fromSt = str( eachTrans.fromSt)
			toSt   = str( eachTrans.toSt)
			cond   = str( eachTrans.cond)

			self.edges.append( ( fromSt, toSt, cond))
			if eachTrans.isToStAcceptAble:
				self.acceptSts.add( toSt) 

		self.__addEdges()
		pass

	def draw( self):
		""" Draw the finite automaton on screen
		"""
		self.transG.layout( prog = "dot")
		try:
			self.transG.draw( format = "xlib")
		except IOError:
			pass	# ignore IOError, when the window is closed.
		pass


if __name__ == "__main__":
	fa = FiniteAuto()
	ts = [
		Trans( "a", "b", ""),
		Trans( "b", "b", "back_cond"),
		Trans( "b", "c", "cond1"),
		Trans( "c", "d", "cond2", True),
		Trans( "b", "f", "cond3"),
		Trans( "f", "d", "cond4", True)]
	fa.addTrans( ts)
	fa.draw()
	pass

