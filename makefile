all: a-star random_board

a-star: a-star.cpp
	g++ -std=c++11 a-star.cpp -o a-star

random_board: random_board.cpp
	g++ -std=c++11 random_board.cpp -o random_board
