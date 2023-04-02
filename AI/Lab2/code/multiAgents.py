# multiAgents.py
# --------------
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


from util import manhattanDistance
from game import Directions
import random, util

from game import Agent
from pacman import GameState

class ReflexAgent(Agent):
    """
    A reflex agent chooses an action at each choice point by examining
    its alternatives via a state evaluation function.

    The code below is provided as a guide.  You are welcome to change
    it in any way you see fit, so long as you don't touch our method
    headers.
    """


    def getAction(self, gameState: GameState):
        """
        You do not need to change this method, but you're welcome to.

        getAction chooses among the best options according to the evaluation function.

        Just like in the previous project, getAction takes a GameState and returns
        some Directions.X for some X in the set {NORTH, SOUTH, WEST, EAST, STOP}
        """
        # Collect legal moves and successor states
        legalMoves = gameState.getLegalActions()

        # Choose one of the best actions
        scores = [self.evaluationFunction(gameState, action) for action in legalMoves]
        bestScore = max(scores)
        bestIndices = [index for index in range(len(scores)) if scores[index] == bestScore]
        chosenIndex = random.choice(bestIndices) # Pick randomly among the best

        "Add more of your code here if you want to"

        return legalMoves[chosenIndex]

    def evaluationFunction(self, currentGameState: GameState, action):
        """
        Design a better evaluation function here.

        The evaluation function takes in the current and proposed successor
        GameStates (pacman.py) and returns a number, where higher numbers are better.

        The code below extracts some useful information from the state, like the
        remaining food (newFood) and Pacman position after moving (newPos).
        newScaredTimes holds the number of moves that each ghost will remain
        scared because of Pacman having eaten a power pellet.

        Print out these variables to see what you're getting, then combine them
        to create a masterful evaluation function.
        """
        # Useful information you can extract from a GameState (pacman.py)
        successorGameState = currentGameState.generatePacmanSuccessor(action)
        newPos = successorGameState.getPacmanPosition()
        newFood = successorGameState.getFood()
        newGhostStates = successorGameState.getGhostStates()
        newScaredTimes = [ghostState.scaredTimer for ghostState in newGhostStates]

        "*** YOUR CODE HERE ***"
        '''
        here evaluation is done by sum of current_score and reciprocal of food distance
        and end if ghost is too close
        '''
        newfood=newFood.asList()	#gets new food location as a List
        #print (newfood)
        food_distance=1
        #calcualting manhattanDistance from newPos to food 
        for food in newfood:
        	food_distance=manhattanDistance(newPos,food)
        #print ('food_distance : ',food_distance)
        #avoiding ghost if too close
        for ghost in successorGameState.getGhostPositions():	
        	if (manhattanDistance(newPos,ghost)<2):
        		return -1
        # here score is calculated as current_score and reciprocal of food distance
        score=successorGameState.getScore()+1.0/food_distance
        return score

def scoreEvaluationFunction(currentGameState: GameState):
    """
    This default evaluation function just returns the score of the state.
    The score is the same one displayed in the Pacman GUI.

    This evaluation function is meant for use with adversarial search agents
    (not reflex agents).
    """
    return currentGameState.getScore()

class MultiAgentSearchAgent(Agent):
    """
    This class provides some common elements to all of your
    multi-agent searchers.  Any methods defined here will be available
    to the MinimaxPacmanAgent, AlphaBetaPacmanAgent & ExpectimaxPacmanAgent.

    You *do not* need to make any changes here, but you can if you want to
    add functionality to all your adversarial search agents.  Please do not
    remove anything, however.

    Note: this is an abstract class: one that should not be instantiated.  It's
    only partially specified, and designed to be extended.  Agent (game.py)
    is another abstract class.
    """

    def __init__(self, evalFn = 'scoreEvaluationFunction', depth = '2'):
        self.index = 0 # Pacman is always agent index 0
        self.evaluationFunction = util.lookup(evalFn, globals())
        self.depth = int(depth)

class MinimaxAgent(MultiAgentSearchAgent):
    """
    Your minimax agent (question 2)
    """

    def getAction(self, gameState: GameState):
        """
        Returns the minimax action from the current gameState using self.depth
        and self.evaluationFunction.

        Here are some method calls that might be useful when implementing minimax.

        gameState.getLegalActions(agentIndex):
        Returns a list of legal actions for an agent
        agentIndex=0 means Pacman, ghosts are >= 1

        gameState.generateSuccessor(agentIndex, action):
        Returns the successor game state after an agent takes an action

        gameState.getNumAgents():
        Returns the total number of agents in the game

        gameState.isWin():
        Returns whether or not the game state is a winning state

        gameState.isLose():
        Returns whether or not the game state is a losing state
        """
        "*** YOUR CODE HERE ***"
        #util.raiseNotDefined()
        '''
        minimax alternately goes to max_agent and min_agents
        chooses maximum value for max_agent and minimum value for min_agent
        it goes till depth
        and returns direction
        '''
        agent_index=0
        depth=0
        answer=self.minimax(gameState,agent_index,depth)[1]
        return answer	# [0]=>has score and [1]=>has action
        
    def minimax(self,gameState,agent_index,depth):
        if len(gameState.getLegalActions(agent_index)) == 0 or depth == self.depth:
            return gameState.getScore(), ""
		
        if agent_index==0:
            #for max_agent
            # gameState.getLegalActions(agent_index) ==> has moves
            best_score=float("-inf")
            best_action=""
            #setting successor
            for action in gameState.getLegalActions(agent_index):
                successor_state=gameState.generateSuccessor(agent_index,action)
                s_index=agent_index+1
                s_depth=depth
                # when max_player setting index to 0 and depth = +1
                if s_index==gameState.getNumAgents():
                    s_index=0
                    s_depth+=1
                score=self.minimax(successor_state,s_index,s_depth)[0]
                if best_score < score:	# choosing best action for max_player
                    best_score=score
                    best_action=action
            return best_score,best_action
            
        else:
            # for min_agent
            # gameState.getLegalActions(agent_index) ==> has moves
            best_score=float("inf")
            best_action=""
            #setting successor
            for action in gameState.getLegalActions(agent_index):
                successor_state=gameState.generateSuccessor(agent_index,action)
                s_index=agent_index+1
                s_depth=depth
                # when max_player setting index to 0 and depth = +1
                if s_index==gameState.getNumAgents():
                    s_index=0
                    s_depth+=1
                score=self.minimax(successor_state,s_index,s_depth)[0]
                if best_score > score:	# choosing worst action for min_player
                    best_score=score
                    best_action=action
            return best_score,best_action
     

class AlphaBetaAgent(MultiAgentSearchAgent):
    """
    Your minimax agent with alpha-beta pruning (question 3)
    """

    def getAction(self, gameState: GameState):
        """
        Returns the minimax action using self.depth and self.evaluationFunction
        """
        "*** YOUR CODE HERE ***"
        #util.raiseNotDefined()
        '''
        minimax alternately goes to max_agent and min_agents
        chooses maximum value for max_agent and minimum value for min_agent
        and prunes path if alpha > beta (alpha:max_agent and beta:min_agent)
        it goes till depth
        and returns direction
        '''
        agent_index=0
        depth=0
        alpha=float("-inf")	# max_agent
        beta=float("inf")	# min_agent
        answer=self.minimax_alpha_beta(gameState,agent_index,depth,alpha,beta)[1]
        return answer	# [0]=>has score and [1]=>has action
        
    def minimax_alpha_beta(self,gameState,agent_index,depth,alpha,beta):
        if len(gameState.getLegalActions(agent_index)) == 0 or depth == self.depth:
            return gameState.getScore(), ""
		
        if agent_index==0:
            #for max_agent
            # gameState.getLegalActions(agent_index) ==> has moves
            best_score=float("-inf")
            best_action=""
            for action in gameState.getLegalActions(agent_index):
                successor_state=gameState.generateSuccessor(agent_index,action)
                s_index=agent_index+1
                s_depth=depth
                # when max_player setting index to 0 and depth = +1
                if s_index==gameState.getNumAgents():
                    s_index=0
                    s_depth+=1
                score=self.minimax_alpha_beta(successor_state,s_index,s_depth,alpha,beta)[0]
                if best_score < score:	# choosing best action for max_player
                    best_score=score
                    best_action=action
                alpha = max(best_score,alpha)	# updating alpha
                if alpha>beta:					# condition for pruning
                    return alpha,best_action
            return best_score,best_action
            
        else:
            # for min_agent
            # gameState.getLegalActions(agent_index) ==> has moves
            best_score=float("inf")
            best_action=""
            for action in gameState.getLegalActions(agent_index):
                successor_state=gameState.generateSuccessor(agent_index,action)
                s_index=agent_index+1
                s_depth=depth
                # when max_player setting index to 0 and depth = +1
                if s_index==gameState.getNumAgents():
                    s_index=0
                    s_depth+=1
                score=self.minimax_alpha_beta(successor_state,s_index,s_depth,alpha,beta)[0]
                if best_score > score:	# choosing worst action for min_player
                    best_score=score
                    best_action=action
                beta = min(best_score,beta)	# updating beta
                if alpha>beta:				# condition for pruning
                    return beta,best_action
            return best_score,best_action
     

class ExpectimaxAgent(MultiAgentSearchAgent):
    """
      Your expectimax agent (question 4)
    """

    def getAction(self, gameState: GameState):
        """
        Returns the expectimax action using self.depth and self.evaluationFunction

        All ghosts should be modeled as choosing uniformly at random from their
        legal moves.
        """
        "*** YOUR CODE HERE ***"
        #util.raiseNotDefined()
        '''
        minimax alternately goes to max_agent and random(expected)
        (uncertain agent with probability)
        chooses maximum value for max_agent and expected value otherwise 
        (for uncertain agent with probability)
        it goes till depth
        and returns direction
        '''
        agent_index=0
        depth=0
        action,score=self.expectimax(gameState,agent_index,depth)
        return action	# [0]=>has score and [1]=>has action
        
    def expectimax(self,gameState,agent_index,depth):
        if len(gameState.getLegalActions(agent_index)) == 0 or depth == self.depth:
            return "", self.evaluationFunction(gameState)
		
        if agent_index==0:
            #for max_agent
            # gameState.getLegalActions(agent_index) ==> has moves
            best_score=float("-inf")
            best_action=""
            for action in gameState.getLegalActions(agent_index):
                successor_state=gameState.generateSuccessor(agent_index,action)
                s_index=agent_index+1
                s_depth=depth
                # when max_player setting index to 0 and depth = +1
                if s_index==gameState.getNumAgents():
                    s_index=0
                    s_depth+=1
                curr_action,score=self.expectimax(successor_state,s_index,s_depth)
                if best_score < score:	# choosing best action for max_player
                    best_score=score
                    best_action=action
            return best_action,best_score
            
        else:
            # for expected value
            # gameState.getLegalActions(agent_index) ==> has moves
            expected_score=0
            for action in gameState.getLegalActions(agent_index):
                successor_state=gameState.generateSuccessor(agent_index,action)
                s_index=agent_index+1
                s_depth=depth
                # when max_player setting index to 0 and depth = +1
                if s_index==gameState.getNumAgents():
                    s_index=0
                    s_depth+=1
                curr_action,score=self.expectimax(successor_state,s_index,s_depth)
                # probability will be : 1/total_moves
                # and expected_score : current score x probability caculated(of that edge)
                expected_score+=score * 1.0/len(gameState.getLegalActions(agent_index))
            return "",expected_score	# return expected_score and no action 
    

def betterEvaluationFunction(currentGameState: GameState):
    """
    Your extreme ghost-hunting, pellet-nabbing, food-gobbling, unstoppable
    evaluation function (question 5).

    DESCRIPTION: <write something here so we know what you did>
    """
    "*** YOUR CODE HERE ***"
    #util.raiseNotDefined()
    '''
    this evaluates 
    evaluation is done by adding current score, game_state, reciprocal of food_left,
    min_dist_from_food, capsule_left
    also checking every time whether ghost is near or not
    '''
    newpos=currentGameState.getPacmanPosition()
    newfood=currentGameState.getFood().asList()
    
    min_dist=float("inf")
    for i in newfood:	# for minimum food distance from all distances
        dist=manhattanDistance(newpos,i)
        if (min_dist>dist):
            min_dist=dist
    
    ghost_dist = 0
    for ghost in currentGameState.getGhostPositions():	# if ghost is too close
        ghost_dist = manhattanDistance(newpos, ghost)
        if (ghost_dist < 2):
            return -1
            
    # getting different values for evaluation function
    food_left=currentGameState.getNumFood()
    capsule_left=len(currentGameState.getCapsules())
    
    game_state=0
    if currentGameState.isLose():
        game_state=-50000
    elif currentGameState.isWin():
        game_state=50000
    # here i defined score as sum of reciprocal of [ food_left , min_dist ,
    # capsule_left , game_state ] multiplied with their appropriate multipliers
    score = currentGameState.getScore() + game_state + (1.0/(food_left+1))*50000 + (1.0/(capsule_left+1))*10000 + ghost_dist + (1.0/(min_dist+1))*1000
    return score

# Abbreviation
better = betterEvaluationFunction
