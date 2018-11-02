// Josh Morris
// CSCI 4160
// OLA 1 - Random Board
// 9/28/17


#include <iostream>
#include <stdlib.h>

using namespace std;

void swap(int board[3][3], int zeroCord[2], int swapCord[2]);

int main(int argc, char* argv[]) {
	int board[3][3];
	int zeroCord[2] = {0, 0}; // {x, y}

	// check number of arguements
	if (argc != 3) {
		cout << "usage: ./random_board rand_seed num_moves" << endl;
		exit(-1);
	}
	int randSeed = atoi(argv[1]);
	int numMoves = atoi(argv[2]);

	// read board from standard in
	for(int i = 0; i < 3; ++i){
		for(int j = 0; j < 3; ++j){
			int temp;
			cin >> temp;
			board[i][j] = temp;
		}
	}

	// random seed
	srand(randSeed);

	// randomize board
	for (int i = 0; i < numMoves; ++i){
		// gen random move
		// 0 = Up
		// 1 = Left
		// 2 = Right
		// 3 = down
		int randMove = int((4.0 * rand()) / (RAND_MAX + 1.0));

		// make move
		//up
		if (randMove == 0 && zeroCord[1] != 0) {
			int swapCord[2] = {zeroCord[0], zeroCord[1] - 1};
			swap(board, zeroCord, swapCord);
		} 
		else if (randMove == 1 && zeroCord[0] != 0) {
			int swapCord[2] = {zeroCord[0] - 1, zeroCord[1]};
			swap(board, zeroCord, swapCord);
		} 
		else if (randMove == 2 && zeroCord[0] != 2) {
			int swapCord[2] = {zeroCord[0] + 1, zeroCord[1]};
			swap(board, zeroCord, swapCord);
		} 
		else if (randMove == 3 && zeroCord[1] != 2) {
			int swapCord[2] = {zeroCord[0], zeroCord[1] + 1};
			swap(board, zeroCord, swapCord);
		} 
		else { //invalid move
			--i;
		}
	} // end for

	// print board 
	for(int i = 0; i < 3; ++i){
		for(int j = 0; j < 3; ++j){
			cout << board[i][j] << " ";
		}
		cout << endl;
	}

	return 0;
}

void swap(int board[3][3], int zeroCord[2], int swapCord[2]) {
	board[zeroCord[1]][zeroCord[0]] = board[swapCord[1]][swapCord[0]];
	board[swapCord[1]][swapCord[0]] = 0;

	zeroCord[0] = swapCord[0];
	zeroCord[1] = swapCord[1];
}