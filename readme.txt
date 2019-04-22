COMPILE:
Compile BestFirstSearch.cpp by either running make or:
	g++ -c BestFirstSearch.cpp 
	g++ -g BestFirstSearch.o -o BestFirstSearch

RUN:
Run BestFirstSearch by either running:
	./BestFirstSearch <input.txt> <output.txt>
	Where <input.txt> is the name of the input file and <output.txt> is the name of the output file

COMPUTATION TIME:
	The time the Best First Search Branch and Bound function takes is at worst case scenario O(nlg(n)), where n is the number of nodes visited. This is because worst case scenario, it runs as a Breadth First Search. However it uses pruning to remove some of the nodes that are not promising. It does this by finding an upperbound and first running the node with the highest upper bound, as that is most likely the most promising to give us the maximum profit. If the node does not have an upper bound greater than the maximum profit we have had, it will become a leaf node.

CLASSES:
	Item:
		Class for the items. Contains their item number, profit, weight, and profit/weight.
		We have a compare function based upon their profit/weight.
	Node:
		Class for nodes. Contains their level on tree, total profit, total weight, current knapsack, and bound.
		We overload the < operator in order for the priority queue to know what determines their priority (their bound)

DATA STRUCTURES:
	Priority queue:
		We use a priority queue with our nodes and give it a compare function. We compare the Nodes based upon their upper bounds. The node with the max bound is the most promising node and should be searched first.
	Vector:
		We use vectors to read in the file and store 0/1 for the knapsack.
