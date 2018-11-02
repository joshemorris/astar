// Josh Morris
// CSCI 4350
// OLA 1 - a star
// 9/28/17

#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <queue>
#include <vector>
#include <stack>
#include <set>
#include "sort.cpp"

using namespace std;

struct node {
	int id;
	struct node* prevNode;
	int zeroCord[2];
	vector<vector<int> > board;
	int cost;
	int moves;

	node() {};

	node(int a, struct node* b, int c[2], 
			vector<vector<int> > d, int e, int f) {
		id = a;
		prevNode = b;
		zeroCord[0] = c[0];
		zeroCord[1] = c[1];
		board = d;
		cost = e;
		moves = f;
	}

	bool operator<(const node& rhs) const {
		if (cost == rhs.cost) {
			return id > rhs.id;
		}

		return cost < rhs.cost;
	}
};

struct nodeComp {
	bool operator() (const node* lhs, const node* rhs) const {
		return (*lhs) < (*rhs);
	}
};

const vector<vector<int> > GOAL = {{0, 1, 2},
																	 {3, 4, 5},
																	 {6, 7, 8}};

void printBoard(vector<vector<int> > board);
vector<vector<int> > swap(vector<vector<int> > board, const int zeroCord[2], int swapCord[2]);
vector<struct node*> genSuccesors(struct node* parent,int (*h)(const vector<vector<int> >&));
void astar(int (*h)(const vector<vector<int> >&));
int UCS(const vector<vector<int> >& cur);
int displaced(const vector<vector<int> >& cur);
int manhattan(const vector<vector<int> > & cur);
int heur3(const vector<vector<int> >& cur);

int main(int argc, char* argv[]) {

	// check number of arguements
	if (argc != 2) {
		cout << "usage: ./astar heuristic" << endl;
		exit(-1);
	}

	int heuristic = atoi(argv[1]);

	// find solution based on heuristic
	switch(heuristic) {
		case 0: astar(&UCS);
						break;
		case 1:	astar(&displaced);
						break;
		case 2: astar(&manhattan);
						break;
		case 3: astar(&heur3);
						break;
	}

	return 0;
}

void printBoard(vector<vector<int> > board){
	for(int i = 0; i < 3; ++i){
		for(int j = 0; j < 3; ++j){
			cout << board[i][j] << " ";
		}
		cout << endl;
	}

	cout << endl;
}

void astar(int (*h)(const vector<vector<int> >&)){
	int zeroCord[2] = {0, 0};
	int V = 0; // nodes expanded
	int d; // depth of solution
	int b; // branching factor
	struct node* curNode;
	set<struct node*, nodeComp> frontier;
	set<vector<vector<int> > > closed;
	vector<struct node*> successors;
	vector<struct node*> nodes;
	stack<struct node*> soln;

	//initialize starting state
	struct node* start = new node(0, NULL, zeroCord, GOAL, 0, 0);

	//read in board
	for(int i = 0; i < 3; ++i){
		for(int j = 0; j < 3; ++j){
			int temp;
			cin >> temp;
			start->board[i][j] = temp;
			if (temp == 0) {
				start->zeroCord[0] = j;
				start->zeroCord[1] = i;
			}
		}
	}

	frontier.insert(start);
	nodes.push_back(start);

	while(!frontier.empty()){
		// check if goal
		curNode = *frontier.begin();
		frontier.erase(frontier.begin());
		++V;

		if (curNode->board == GOAL){
			break;
		}

		// generate and add unique successors
		successors = genSuccesors(curNode, h);

		for (auto iter = successors.begin(); iter != successors.end(); ++iter) {
			if (closed.count((*iter)->board)){
				// config already reached
				delete *iter;
				*iter = NULL;
			} else {
				(*iter)->id = nodes.size();
				frontier.insert(*iter);
				nodes.push_back(*iter);
			}
		}

		// close current state
		closed.insert(curNode->board);
	}

	if (frontier.empty()) {
		cout << "No solution" << endl;
		return;
	} 

	// print out stats
	cout << "V=" << V << endl;
	cout << "N=" << nodes.size() << endl;
	cout << "d=" << curNode->moves << endl;
	cout << "b=" << pow(nodes.size(), 1.0 / curNode->moves) << endl << endl; 

	// print the solution
	while (curNode != NULL) {
		soln.push(curNode);
		curNode = curNode->prevNode;
	}

	while(!soln.empty()){
		printBoard(soln.top()->board);
		soln.pop();
	}

	// clean up this mess
	for(auto iter = nodes.begin(); iter != nodes.end(); ++iter) {
		delete *iter;
	}
}

// function to generate successors 
vector<struct node*> genSuccesors(struct node* parent, int (*h)(const vector< vector<int> >&)){
		vector<struct node*> successors;
		vector<vector<int> > tempBoard;
		int id = 0; // just a dummy value, need to assign after checking closed

		if (parent->zeroCord[1] != 0) {
			int swapCord[2] = {parent->zeroCord[0], parent->zeroCord[1] - 1};
			tempBoard = swap(parent->board, parent->zeroCord, swapCord);
			
			// calc cost
			float cost = parent->moves + 1 + h(tempBoard);
			int moves = parent->moves + 1;

			struct node* childUp = new node(id, parent, swapCord, tempBoard, cost, moves);
			successors.push_back(childUp);
		} 
		
		if (parent->zeroCord[0] != 0) {
			int swapCord[2] = {parent->zeroCord[0] - 1, parent->zeroCord[1]};
			tempBoard = swap(parent->board, parent->zeroCord, swapCord);


			// calc cost
			float cost = parent->moves + 1 + h(tempBoard);
			int moves = parent->moves + 1;

			struct node* childLeft = new node(id, parent, swapCord, tempBoard, cost, moves);
			successors.push_back(childLeft);
		} 
		
		if (parent->zeroCord[0] != 2) {
			int swapCord[2] = {parent->zeroCord[0] + 1, parent->zeroCord[1]};
			tempBoard = swap(parent->board, parent->zeroCord, swapCord);

			// calc cost
			float cost = parent->moves + 1 + h(tempBoard);
			int moves = parent->moves + 1;

			struct node* childRight = new node(id, parent, swapCord, tempBoard, cost, moves);
			successors.push_back(childRight);
		} 
		
		if (parent->zeroCord[1] != 2) {
			int swapCord[2] = {parent->zeroCord[0], parent->zeroCord[1] + 1};
			tempBoard = swap(parent->board, parent->zeroCord, swapCord);
			
			// calc cost
			float cost = parent->moves + 1 + h(tempBoard);
			int moves = parent->moves + 1;

			struct node* childDown = new node(id, parent, swapCord, tempBoard, cost, moves);
			successors.push_back(childDown);
		} 

		return successors;
}

vector<vector<int> > swap(vector<vector<int> > board,const int zeroCord[2], int swapCord[2]) {
	board[zeroCord[1]][zeroCord[0]] = board[swapCord[1]][swapCord[0]];
	board[swapCord[1]][swapCord[0]] = 0;

	return board;
}

// 	heuristic 0: no heuristic
int UCS(const vector<vector<int> >& cur) {return 0;}

//	heuristic 1: # of displaced
int displaced(const vector<vector<int> >& cur) {
	int count = 0;

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j)	{
			if (GOAL[i][j] != cur[i][j] && cur[i][j] != 0){
				++count;
			} 
		}
	}

	return count;
}

// heuristic 2: Sum of Manhattan
int manhattan(const vector<vector<int> > & cur) {
	int corPos[9][2] = {{0, 0}, {0, 1}, {0, 2},
							 {1, 0}, {1, 1}, {1, 2},
							 {2, 0}, {2, 1}, {2, 2}};
	int dist = 0;

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j)	{
			int tile = cur[i][j];

			if (GOAL[i][j] != tile && tile != 0){
				dist += abs(corPos[tile][0] - i) + abs(corPos[tile][1] - j);
			} 
		}
	}

	return dist;
}

// heuristic 3: 0.5(displaced) + 0.5(manhattan)
int heur3(const vector<vector<int> >& cur) {
	return (displaced(cur) + manhattan(cur));
}