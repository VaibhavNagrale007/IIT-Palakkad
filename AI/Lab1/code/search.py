# search.py
# ---------
# Licensing Information:  You are free to use or extend these projects for
# educational purposes provided that (1) you do not distribute or publish
# solutions, (2) you retain this notice, and (3) you provide clear
# attribution to UC Berkeley, including a link to http://ai.berkeley.edu.
# 
# Attribution Information: The Pacman AI projects were developed at UC Berkeley.
# The core projects and autograders were primarily created by John DeNero
# (denero@cs.berkeley.edu) and Dan Klein (klein@cs.berkeley.edu).
# Student side autograding was added by Brad Miller, Nick Hay, and
# Pieter Abbeel (pabbeel@cs.berkeley.edu).


"""
In search.py, you will implement generic search algorithms which are called by
Pacman agents (in searchAgents.py).
"""

import util

class SearchProblem:
    """
    This class outlines the structure of a search problem, but doesn't implement
    any of the methods (in object-oriented terminology: an abstract class).

    You do not need to change anything in this class, ever.
    """

    def getStartState(self):
        """
        Returns the start state for the search problem.
        """
        util.raiseNotDefined()

    def isGoalState(self, state):
        """
          state: Search state

        Returns True if and only if the state is a valid goal state.
        """
        util.raiseNotDefined()

    def getSuccessors(self, state):
        """
          state: Search state

        For a given state, this should return a list of triples, (successor,
        action, stepCost), where 'successor' is a successor to the current
        state, 'action' is the action required to get there, and 'stepCost' is
        the incremental cost of expanding to that successor.
        """
        util.raiseNotDefined()

    def getCostOfActions(self, actions):
        """
         actions: A list of actions to take

        This method returns the total cost of a particular sequence of actions.
        The sequence must be composed of legal moves.
        """
        util.raiseNotDefined()


def tinyMazeSearch(problem):
    """
    Returns a sequence of moves that solves tinyMaze.  For any other maze, the
    sequence of moves will be incorrect, so only use this for tinyMaze.
    """
    from game import Directions
    s = Directions.SOUTH
    w = Directions.WEST
    return  [s, s, w, s, w, w, s, w]

def depthFirstSearch(problem):
    """
    Search the deepest nodes in the search tree first.

    Your search algorithm needs to return a list of actions that reaches the
    goal. Make sure to implement a graph search algorithm.

    To get started, you might want to try some of these simple commands to
    understand the search problem that is being passed in:

    print "Start:", problem.getStartState()
    print "Is the start a goal?", problem.isGoalState(problem.getStartState())
    print "Start's successors:", problem.getSuccessors(problem.getStartState())
    """
    "*** YOUR CODE HERE ***"
    """
    Explanation:
    first check whether our start state is our goal state
    then apply DFS to reach our goal state (food)
    after getting goal state trace back from past vertex and store nodes in answer
    then return this answer
    """
    # util.raiseNotDefined()
    answer=[]	#list(value)
    visited={}	#dictionary(key,value) for visited nodes
    pastvertex={}	# for tracing back to source
    stack=util.Stack()	#since util has stack defined
    
    
    #print "Is the start a goal?", problem.isGoalState(problem.getStartState())
    #print "Start:", problem.getStartState()
    Start=problem.getStartState()
    if problem.isGoalState(Start):
    	return answer
    
    visited[Start] = 'No dirn'
    stack.push((Start, 'No dirn', 0))
    # DFS till we get goal state
    while stack:
    	temp=stack.pop()
    	visited[temp[0]]=temp[1]
    	if problem.isGoalState(temp[0]):
    		endnode=temp[0]
    		break
    	for vertex in problem.getSuccessors(temp[0]):	#go through all vertex
    		if vertex[0] not in visited.keys():			#if not visited push to stack
    			pastvertex[vertex[0]]=temp[0]
    			stack.push(vertex)
    			# print problem.getSuccessors(temp[0]),pastvertex[vertex[0]],vertex[0]
    # putting the path in answer stack
    while (endnode in pastvertex.keys()):
    	answer.insert(0,visited[endnode])
    	endnode=pastvertex[endnode]
    #print 'ans::::::::::',answer
    return answer
    

def breadthFirstSearch(problem):
    """Search the shallowest nodes in the search tree first."""
    "*** YOUR CODE HERE ***"
    """
    Explanation:
    first check whether our start state is our goal state
    then apply BFS to reach our goal state (food)
    after getting goal state trace back from past vertex and store nodes in answer
    then return this answer
    """
    # util.raiseNotDefined()
    answer=[]	#list(value)
    visited={}	#dictionary(key,value) for visited nodes
    pastvertex={}	# for tracing back to source
    queue=util.Queue()	#since util has stack defined
    
    
    #print "Is the start a goal?", problem.isGoalState(problem.getStartState())
    #print "Start:::::::::::::", problem.getStartState()
    Start=problem.getStartState()
    if problem.isGoalState(Start):
    	return answer
    
    visited[Start] = 'No dirn'
    queue.push((Start, 'No dirn', 0))
    #BFS till we get goal state
    while queue:
    	temp=queue.pop()
    	visited[temp[0]]=temp[1]
    	#print temp,problem.getSuccessors(temp[0])
    	if problem.isGoalState(temp[0]):
    		endnode=temp[0]
    		break
    	
    	for vertex in problem.getSuccessors(temp[0]):
    		if vertex[0] not in visited.keys() and vertex[0] not in pastvertex.keys():
    			pastvertex[vertex[0]]=temp[0]
    			queue.push(vertex)
    			#print problem.getSuccessors(temp[0]),pastvertex[vertex[0]],vertex[0]
    # putting the path in answer stack
    while (endnode in pastvertex.keys()):
    	# print visited[endnode],endnode
    	answer.insert(0,visited[endnode])
    	endnode=pastvertex[endnode]
    return answer

def uniformCostSearch(problem):
    """Search the node of least total cost first."""
    "*** YOUR CODE HERE ***"
    """
    Explanation:
    first check whether our start state is our goal state
    then apply UCS to reach our goal state (food)
    	calculate the cost 
    	if more cost then do nothing else push to queue and change the cost 
    after getting goal state trace back from past vertex and store nodes in answer
    then return this answer
    """
    # util.raiseNotDefined()
    answer=[]	#list(value)
    visited={}	#dictionary(key,value) for visited nodes
    pastvertex={}	# for tracing back to source
    Pqueue=util.PriorityQueue()	#since util has stack defined
    cost={}	#cost for UCS
    
    #print "Is the start a goal?", problem.isGoalState(problem.getStartState())
    #print "Start:", problem.getStartState()
    Start=problem.getStartState()
    if problem.isGoalState(Start):
    	return answer
    
    visited[Start] = 'No dirn'
    cost[Start]=0	#cost for starting state is 0
    Pqueue.push((Start, 'No dirn', 0), 0)
    #UCS till we get goal state using priority queue
    while Pqueue:
    	temp=Pqueue.pop()
    	visited[temp[0]]=temp[1]
    	if problem.isGoalState(temp[0]):
    		endnode=temp[0]
    		break
    	
    	for vertex in problem.getSuccessors(temp[0]):
    		if vertex[0] not in visited.keys():
    			#print '<',temp[0],temp[1],temp[2],vertex[0],vertex[1],vertex[2],'>'
    			g_n=temp[2]+vertex[2] # getting the cost of successor
    			f_n=g_n	#coming element cost
    			if vertex[0] in cost.keys() and f_n>=cost[vertex[0]]:
    				continue	#new cost is more then no push directly continue to check for another vertex
    			pastvertex[vertex[0]]=temp[0]
    			cost[vertex[0]]=f_n
    			Pqueue.push((vertex[0],vertex[1],g_n),f_n)
    # putting the path in answer stack
    while (endnode in pastvertex.keys()):
    	# print visited[endnode],endnode
    	answer.insert(0,visited[endnode])
    	endnode=pastvertex[endnode]
    return answer
    

def nullHeuristic(state, problem=None):
    """
    A heuristic function estimates the cost from the current state to the nearest
    goal in the provided SearchProblem.  This heuristic is trivial.
    """
    return 0

def aStarSearch(problem, heuristic=nullHeuristic):
    """Search the node that has the lowest combined cost and heuristic first."""
    "*** YOUR CODE HERE ***"
    """
    Explanation:
    first check whether our start state is our goal state
    then apply A* search to reach our goal state (food)
    	calculate the f_n = cost + heuristic
    	if more f_n then do nothing else push to queue and change the cost 
    after getting goal state trace back from past vertex and store nodes in answer
    then return this answer
    """
    #util.raiseNotDefined()
    """
    same as UCS but add a hueristic function
    """
    answer=[]	#list(value)
    visited={}	#dictionary(key,value) for visited nodes
    pastvertex={}
    Pqueue=util.PriorityQueue()	#since util has stack defined
    cost={}	#cost for UCS
    
    #print "Is the start a goal?", problem.isGoalState(problem.getStartState())
    #print "Start:", problem.getStartState()
    Start=problem.getStartState()
    if problem.isGoalState(Start):
    	return answer
    
    visited[Start] = 'No dirn'
    cost[Start]=0	#cost for starting state is 0
    Pqueue.push((Start, 'No dirn', 0), 0)
    # A* search till we get goal state using priority queue
    while Pqueue:
    	temp=Pqueue.pop()
    	visited[temp[0]]=temp[1]
    	if problem.isGoalState(temp[0]):
    		endnode=temp[0]
    		break
    	
    	for vertex in problem.getSuccessors(temp[0]):
    		if vertex[0] not in visited.keys():
    			g_n=temp[2]+vertex[2]
    			h_n=heuristic(vertex[0],problem)
    			f_n=g_n+h_n	#coming element cost+heuristic cost f(n)=g(n)+h(n)
    			if vertex[0] in cost.keys() and f_n>=cost[vertex[0]]:
    				continue	#new cost is more then no push directly continue to check for another vertex
    			pastvertex[vertex[0]]=temp[0]
    			cost[vertex[0]]=f_n
    			Pqueue.push((vertex[0],vertex[1],g_n),f_n)
    # putting the path in answer stack
    while (endnode in pastvertex.keys()):
    	# print visited[endnode],endnode
    	answer.insert(0,visited[endnode])
    	endnode=pastvertex[endnode]
    return answer
        


# Abbreviations
bfs = breadthFirstSearch
dfs = depthFirstSearch
astar = aStarSearch
ucs = uniformCostSearch
