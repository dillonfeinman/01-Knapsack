#include <string>
#include <fstream>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

//Nodes have a level on the tree, a total profit based on its current knapsack, a total weight based on knapsack, a bound that determines how promising it is, and a list of 0 or 1 to determine which items are also added to the knapsack
class Node{
public:
	int level; //level on tree
	int profit; //profit based on prior items
	int weight; //weight based on prior items
	float bound; //bound based on future items
	vector<int> list; //list of which items are in knapsack
};
bool operator<(const Node &a, const Node &b) { //this overloads the < operator for the priority queue. 
		return a.bound < b.bound; //It compares the bounds of Nodes a and b to determine which is most priority
	}

class item{ //this is a class for the items
public:
	int profit; //item profit
	int weight; //item weight
	float pw; //item profit/weight
	int index; //item index
};

int compItemByPW(item pw1, item pw2){ //this takes two items and compares then based upon profit/weight for sort function
	return pw1.pw > pw2.pw; 
}
//findBound calculates the upper bound for the node n given a sorted array of weights w and profits p and a capcity. Num is the number of items to be considered.
//This upper bound determines which node is the most promising based upon the total profit possible. findBound returns the upper bound as a float.
float findBound(int num, int profits[], int weights[], int capacity, Node n){
	int weight = 0; //initialize weight to 0
	float ret = 0; //initialize bound to 0
	if(n.weight >= capacity){ //if the weight is already equal to (or over) capacity
		return ret; //return 0
	} else { //if we can fit more
		ret = (float)n.profit; //set ret to profit
		int i = n.level; //start at level n is at
		weight = n.weight; //set weight to the weight of n
		while(i <= num && weight + weights[i] <= capacity){ //while i <= the index of last item and the weight + the weight of the item to be added is less than capacity
			weight += weights[i]; //add weight of item [i] to the current weight
			ret += profits[i]; //add the profit of item i to the bound
			i++; //next
		}
		if(i < num && weight < capacity){ //if we did not get to the final item, take a fraction of the item with the next greatest profit/weight if we are not full
			ret = ret + (capacity-weight) * (profits[i]/weights[i]); //bound = current bound + the remaining weight allowed * the profit/weight
		}
		return ret; //return bound
	}
}
//writeKnapsack takes in an int num, number of items, array of profits and weights that are sorted (ints), an int capacity (max weight), an int maxProfit (max profit we have gotten), and string ouput which is the file name to write to
//This function solves the 0 1 Knapsack problem. This is when we are given items each with a weight and a profit. We want to get the most profit given the amount of weight we can hold.
//This function uses the best-first-search branch and bound algorithm. It assumes sorted profits and weights. This means that we use bounds to determine if the node is promising.
void writeKnapsack(int num, int profits[], int weights[], int capacity, int maxProfit, string output){
	vector<int> sol; //vector to decide which are included in the knapsack
	int opt = 0; //number of optimal node
	int numNodes = 1; //number of nodes visited
	int visitedLeaves = 0; //number of leaves visited
	priority_queue<Node> pqueue; //priority queue of Nodes decided based upon their bounds
	Node x; //create a start Node
	x.level = 0; //init level to 0
	x.profit = 0; //init profit to 0
	x.weight = 0; //init weight to 0
	maxProfit = 0; //init max profit to 0
	x.bound = findBound(num, profits, weights, capacity, x); //find upper bound of x
	vector<int> newlist (num, 0); //create a list of 0's for each item
	x.list = newlist; //init x's list to the new list
	pqueue.push(x); //push x onto the priority queue
	while(pqueue.empty() == false){ //while the queue is not empty and everything has not been visited
		x = pqueue.top(); //set x to the top priority node
		pqueue.pop(); //pop the top priority node
		if(x.bound > maxProfit){ //if the upperbound of x is greter than the maximum profit we have had
			numNodes += 2; //add a yes child and a no child
			Node y; //yes child
			y.level = x.level + 1; //child is at the next level
			y.weight = weights[y.level - 1] + x.weight; //child has the weight of its parent + the weight of the child
			//cout << y.level << endl;
			cout << profits[y.level - 1]<<", ";
			cout << x.profit << endl;
			cout << endl;
			y.profit = profits[y.level - 1] + x.profit; //child has the profit of the parent + the profit of the child
			y.list = x.list; //child has the same knapsack as the parent
			y.list[y.level-1] = 1; //child is added to the knapsack
			if(y.weight <= capacity && y.profit > maxProfit){ //if the weight is less than capacity and the profit is greater than the maximum profit we have had
				// cout << maxProfit << "," << endl;
				maxProfit = y.profit; //new profit is max profit
				// cout << maxProfit << "," << endl;
				// cout << endl;
				sol = y.list; //optimal solution is y's knapsack
			}
			y.bound = findBound(num, profits, weights, capacity, y); //find the upper bound of yes child
			if(y.bound > maxProfit && y.weight < capacity){ //if the bound of y is > maxProfit and the weight is under the capacity
				pqueue.push(y); //add y to the priority queue
			} else { //it is not a promising node
				visitedLeaves++; //it is a leaf node so add to visited leaf nodes
			}
			Node z; //no child
			z.level = x.level + 1; //child is at the next level
			z.weight = x.weight; //child has the weight of the parent only. (Not taking item)
			z.profit = x.profit; //child has the profit of parent only. 
			z.list = x.list; //child has knapsack of parent
			z.list[z.level-1] = 0; //item is not being included in knapsack
			z.bound = findBound(num, profits, weights, capacity, z); //find upper bound of no child
			if(z.bound > maxProfit && z.weight < capacity){ //if the bound is greater than max profit and weight is under capacity. promising
				pqueue.push(z); //push no child onto priority queue
			} else { //it is not a promising node
				visitedLeaves++; //it is a leaf node. not promising
			}
		} else { //if the upperbound of x is not greater than the max profit
			visitedLeaves++; //it is a leaf node and not promising
		}
	}
	//priority queue is empty
	for(int i = 0; i < sol.size(); i++){ //for items
		if(sol[i] == 1){ //if sol[i] == 1, items[i] is included
			opt++; //add to num of items in optimal set
		}
	}
	ofstream file; //outfile stream
	file.open(output); //open output
	if(file.is_open()){ //if open
		file << num << "," << maxProfit << "," << opt << endl; //write number of items, max profit possible, number of items to get max profit on first line
		file << numNodes << "," << visitedLeaves << endl; //write number of nodes visited, number of leaves visited
		for(int i = 0; i < sol.size(); i++){ //for solution set
			if(sol[i] == 1){ //if in knapsack
				file << weights[i] << "," << profits[i] << endl; //write weight,profit of item i
			}
		}
	}
}
//this is driver code for the best-search first bound and branch 0/1 knapsack problem. It takes in
//an input file name and an output file name. 
int main(int argc, char * argv[]){
	if(argc != 3){ //need args input name and output name
		exit(1);
	} else {
		//read input and parse
		ifstream file; //create in stream
		string input = argv[1]; //input name
		string output = argv[2]; //output name
		file.open(input); //open input
		vector<string> in; //vector containing all of input file line by line
		if(file.is_open()){ //if opened
			string line; //read to line
			while(getline(file, line)){ //while getline != EOF
				in.push_back(line); //get line
			}
			file.close(); //close file
		}
		string n; //create string of num of items
		n = in[0].substr(0, in[0].find(",")); //substring from start til ,
		n += '\0'; //add end character
		int num = stoi(n); //convert string to int
		string c; //create string of capacity
		c = in[0].substr(in[0].find(",")+1, in[0].length()); //substring from char after , until end of string
		int cap = stoi(c); //convert string to int
		item items[num]; //create array of items
		int index = 1; //counter to skip the n,c
		int profits[num]; //create array of profits
		int weights[num]; //create array of weights
		float pw[num]; //creat array of profit/weight 
		for(int i = 0; i < in.size()-1; i++){ //initialize the items (in.size-1 because no first line)
			items[i].weight = stoi(in[index].substr(0, in[index].find(","))); //convert string to int for the weight at i
			items[i].profit = stoi(in[index].substr(in[index].find(",")+1, in[index].length())); //convert string to int for the profit at i
			profits[i] = items[i].profit; //add profit to array
			weights[i] = items[i].weight; //add weight to array
			pw[i] = (float)profits[i] / (float)weights[i]; //get the profit/weight
			items[i].pw = pw[i]; //put the profit/weight into item
			items[i].index = i; //record the index of item
			//cout << items[i].index << ": " << items[i].weight << "," << items[i].profit << "," << items[i].pw << endl;
			index++; //next index
		}
		cout << "here" << endl;
		int sProfits[num+1]; //sort profits
		int sWeights[num+1]; //sort weights
		sort(items, items+num, compItemByPW); //sort items by profit/weight
		cout << "here" << endl;
		for(int i = 0; i < num+1; i++){ 
			sProfits[i] = profits[items[i].index]; //sort profits using sorted items
			sWeights[i] = weights[items[i].index]; //sort weights using sorted items
		}
		// for(int i = 0; i < num; i++){
		// 	cout << sProfits[i] << ": " << sWeights[i] << endl;
		// }
		cout << "here" << endl;
		writeKnapsack(num, sProfits, sWeights, cap, 0, output); //begin knapsack function and write results
	}
}