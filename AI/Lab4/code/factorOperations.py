# factorOperations.py
# -------------------
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

from typing import List, ValuesView
from bayesNet import Factor
import functools
from util import raiseNotDefined

def joinFactorsByVariableWithCallTracking(callTrackingList=None):


    def joinFactorsByVariable(factors: List[Factor], joinVariable: str):
        """
        Input factors is a list of factors.
        Input joinVariable is the variable to join on.

        This function performs a check that the variable that is being joined on 
        appears as an unconditioned variable in only one of the input factors.

        Then, it calls your joinFactors on all of the factors in factors that 
        contain that variable.

        Returns a tuple of 
        (factors not joined, resulting factor from joinFactors)
        """

        if not (callTrackingList is None):
            callTrackingList.append(('join', joinVariable))

        currentFactorsToJoin =    [factor for factor in factors if joinVariable in factor.variablesSet()]
        currentFactorsNotToJoin = [factor for factor in factors if joinVariable not in factor.variablesSet()]

        # typecheck portion
        numVariableOnLeft = len([factor for factor in currentFactorsToJoin if joinVariable in factor.unconditionedVariables()])
        if numVariableOnLeft > 1:
            print("Factors failed joinFactorsByVariable typecheck: ", factors)
            raise ValueError("The joinBy variable can only appear in one factor as an \nunconditioned variable. \n" +  
                               "joinVariable: " + str(joinVariable) + "\n" +
                               ", ".join(map(str, [factor.unconditionedVariables() for factor in currentFactorsToJoin])))
        
        joinedFactor = joinFactors(currentFactorsToJoin)
        return currentFactorsNotToJoin, joinedFactor

    return joinFactorsByVariable

joinFactorsByVariable = joinFactorsByVariableWithCallTracking()

########### ########### ###########
########### QUESTION 2  ###########
########### ########### ###########

def joinFactors(factors: ValuesView[Factor]):
    """
    factors: can iterate over it as if it was a list, and convert to a list.
    
    You should calculate the set of unconditioned variables and conditioned 
    variables for the join of those factors.

    Return a new factor that has those variables and whose probability entries 
    are product of the corresponding rows of the input factors.

    You may assume that the variableDomainsDict for all the input 
    factors are the same, since they come from the same BayesNet.

    joinFactors will only allow unconditionedVariables to appear in 
    one input factor (so their join is well defined).

    Hint: Factor methods that take an assignmentDict as input 
    (such as getProbability and setProbability) can handle 
    assignmentDicts that assign more variables than are in that factor.

    Useful functions:
    Factor.getAllPossibleAssignmentDicts
    Factor.getProbability
    Factor.setProbability
    Factor.unconditionedVariables
    Factor.conditionedVariables
    Factor.variableDomainsDict
    """

    # typecheck portion
    setsOfUnconditioned = [set(factor.unconditionedVariables()) for factor in factors]
    if len(factors) > 1:
        intersect = functools.reduce(lambda x, y: x & y, setsOfUnconditioned)
        if len(intersect) > 0:
            print("Factors failed joinFactors typecheck: ", factors)
            raise ValueError("unconditionedVariables can only appear in one factor. \n"
                    + "unconditionedVariables: " + str(intersect) + 
                    "\nappear in more than one input factor.\n" + 
                    "Input factors: \n" +
                    "\n".join(map(str, factors)))


    "*** YOUR CODE HERE ***"
    # raiseNotDefined()
    
    # Join Factors
    # calculating the set of unconditioned variables and conditioned variables
    unconditioned_var = set({})
    conditioned_var = set({})
    variable_domains = {}
    
    for i in factors:
        for var in i.unconditionedVariables():
            unconditioned_var.add(var)
            if var in conditioned_var:
                conditioned_var.remove(var)
        
        for var in i.conditionedVariables():
            if var not in unconditioned_var:
                conditioned_var.add(var)
                
        assignment_dict = i.variableDomainsDict()
        key = list(assignment_dict.keys())
        val = list(assignment_dict.values())
        
        for j in range(len(key)):
            variable_domains[key[j]] = val[j]
            
    unconditioned_var = list(unconditioned_var)
    conditioned_var = list(conditioned_var)
    ans = Factor(unconditioned_var, conditioned_var, variable_domains)
    #print ("ans:::: ",ans)
    allpossible_assignment = ans.getAllPossibleAssignmentDicts()
    # print("unconditioned_var: ",unconditioned_var," conditioned_var: ",conditioned_var," variable_domains: ",variable_domains)
    # multiplying probability
    for i in range(len(allpossible_assignment)):
        dict = allpossible_assignment[i]
        probability = 1
        for j in factors:
            probability *= j.getProbability(dict)
            # print("P: ",probability)
        ans.setProbability(dict, probability)
    
    # print("ans : ",ans)    
    return ans
    "*** END YOUR CODE HERE ***"
    '''
    In joinFactors first set contains unconditioned variables, second set has
    conditioned variables and third set has Probability corresponding to 
    given variables
    Unconditioned variables = Variables - Conditioned variables
    '''

########### ########### ###########
########### QUESTION 3  ###########
########### ########### ###########

def eliminateWithCallTracking(callTrackingList=None):

    def eliminate(factor: Factor, eliminationVariable: str):
        """
        Input factor is a single factor.
        Input eliminationVariable is the variable to eliminate from factor.
        eliminationVariable must be an unconditioned variable in factor.
        
        You should calculate the set of unconditioned variables and conditioned 
        variables for the factor obtained by eliminating the variable
        eliminationVariable.

        Return a new factor where all of the rows mentioning
        eliminationVariable are summed with rows that match
        assignments on the other variables.

        Useful functions:
        Factor.getAllPossibleAssignmentDicts
        Factor.getProbability
        Factor.setProbability
        Factor.unconditionedVariables
        Factor.conditionedVariables
        Factor.variableDomainsDict
        """
        # autograder tracking -- don't remove
        if not (callTrackingList is None):
            callTrackingList.append(('eliminate', eliminationVariable))

        # typecheck portion
        if eliminationVariable not in factor.unconditionedVariables():
            print("Factor failed eliminate typecheck: ", factor)
            raise ValueError("Elimination variable is not an unconditioned variable " \
                            + "in this factor\n" + 
                            "eliminationVariable: " + str(eliminationVariable) + \
                            "\nunconditionedVariables:" + str(factor.unconditionedVariables()))
        
        if len(factor.unconditionedVariables()) == 1:
            print("Factor failed eliminate typecheck: ", factor)
            raise ValueError("Factor has only one unconditioned variable, so you " \
                    + "can't eliminate \nthat variable.\n" + \
                    "eliminationVariable:" + str(eliminationVariable) + "\n" +\
                    "unconditionedVariables: " + str(factor.unconditionedVariables()))

        "*** YOUR CODE HERE ***"
        # raiseNotDefined()
        
        # Eliminate
        # calculating set of unconditioned and conditioned variables
        unconditionedVariables = []
        conditionalVariables = []
        for var in factor.unconditionedVariables():
            if var != eliminationVariable:
                unconditionedVariables.append(var)
        for var in factor.conditionedVariables():
            if var != eliminationVariable:
                conditionalVariables.append(var)
                
        ans = Factor(unconditionedVariables, conditionalVariables, factor.variableDomainsDict())
        possible_assignment = ans.getAllPossibleAssignmentDicts()
        
        # adding probability
        # i = answer possibility , j = original possibility , k = key
        for i in possible_assignment:
            probability = 0
            key = list(i.keys())
            for j in factor.getAllPossibleAssignmentDicts():
                visited = True
                for k in key:
                    if i[k] != j[k]:
                        visited = False
                        break
                if visited:
                    probability += factor.getProbability(j)
            ans.setProbability(i, probability)
        return ans
        
        "*** END YOUR CODE HERE ***"
    '''
    In joinFactors first set contains unconditioned variables, second set has
    conditioned variables and third set has Probability corresponding to 
    given variables
    Unconditioned Variables = Unconditioned Variables - elimination variables
    Conditioned Variables = Conditioned Variables - elimination variables
    '''
    return eliminate

eliminate = eliminateWithCallTracking()

