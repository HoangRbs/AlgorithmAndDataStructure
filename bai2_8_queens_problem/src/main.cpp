#include <iostream>
#include <limits.h>
#include <time.h>
#include <stdlib.h>
#include <vector>
#include "../includes/Timer.cpp"

using namespace std;

// 8x8 board
const int N_queen_board_size = 5;

class N_queens {
	public:

		int board[N_queen_board_size][N_queen_board_size];

		N_queens (int board[N_queen_board_size][N_queen_board_size]) {
			for (int i =  0; i < N_queen_board_size; i++) {
				for (int j = 0; j < N_queen_board_size; j++) {
					this->board[i][j] = board[i][j];
				}
			} 
		}

		struct boardPoint {
			boardPoint(int row, int col) { this->row = row; this->col = col; };

			int row;
			int col;
		};

		vector<boardPoint> placedPoints; // save points where boardPoint[i][j] = 1

		void execute();
		void solveNQueen(int currentRow);
		bool isSafe(int row, int col);
};


// Travelling Salesman Salesman Problem
class TSP {
	public:
		TSP() {
			srand(time(NULL));
			
			// randomize distance between nodes
			for (int i = 0; i < this->N; i++) {
				this->dist.push_back(vector<int>());
				
				for (int j = 0; j < this->N; j++) {
					this->dist[i].push_back(rand()%50 + 1);
				}
			}
		};
	
		struct Node {
			int currentPoint = 0;
			int bestNextPoint = 0;
			int bestDistance = INT_MAX;
			Node* nextNode = NULL;
		
			void setInfo (int currentPoint, int bestNextPoint, int bestDistance, Node* nextNode) {
				this->currentPoint = currentPoint;
				this->bestDistance = bestDistance;
				this->bestNextPoint = bestNextPoint;
				this->nextNode = nextNode;
			};
		};

		static const int N = 11; // number of nodes
		vector<vector<int>> dist;  // distances between nodes
		Node* startNode = NULL; // to save shortest path from A (start Node) -> ... A 

		int visitedAll();
		bool isVisited(int mask, int point_pos);
		void execute();
		int do_tsp(int mask, int pos, Node* tmpNode_out);
		void printNodes(Node* startNode); // print shortest path
};

// test fibonacci using dynamic programming
class Fibonacci {
	public:
		int cal_fibonacci(int idx, int memo[]);
		void execute();
};

void print2DArray (int board[N_queen_board_size][N_queen_board_size]) {
	for (int i =  0; i < N_queen_board_size ; i++) {
		for (int j = 0; j < N_queen_board_size ; j++) {
			cout << board[i][j] << " ";
		}
		cout << endl;
	}

	cout << endl << endl;
}

void print2DVector (vector<vector<int>> board) {

	for (int i = 0; i < board.size(); i++) {
		for (int j = 0; j < board[i].size(); j++) {
			cout << board[i][j] <<" ";
		}
		cout << endl;
	}
}

int main() {
	int board[N_queen_board_size][N_queen_board_size];

	for(int i = 0; i < N_queen_board_size; i++) {
		for (int  j = 0; j < N_queen_board_size; j++) {
			board[i][j] = 0;
		}
	}

	// N_queens m_program = N_queens(board);
	TSP m_program;
	// Fibonacci m_program = Fibonacci();

	m_program.execute();
};

// ----------------------------- N queens problem -------------------------------

void N_queens::execute() {
	solveNQueen(0);
}

void N_queens::solveNQueen (int currentRow) {
	if (currentRow >= N_queen_board_size) {
		// exceed the default board row -> all queens are valid
		// print final result

		print2DArray(this->board);	
	}

	for (int j = 0; j < N_queen_board_size; j++) {
		if (isSafe(currentRow, j)) {
			this->board[currentRow][j] = 1;
			this->placedPoints.push_back(boardPoint(currentRow, j));

			solveNQueen(currentRow + 1);

			// backtrack! -> reset
			this->board[currentRow][j] = 0;
			this->placedPoints.pop_back();			
		} 
	}
};

bool N_queens::isSafe(int row, int col) {

	boardPoint point = boardPoint(row, col); 
	
	// iterate all the placed points
	for (int i = 0; i < this->placedPoints.size(); i++)	{
		boardPoint currentPlacedPoint = this->placedPoints[i];

		// same col or same row
		if (point.col == currentPlacedPoint.col ||
				point.row == currentPlacedPoint.row) return false;

		// ------------ forward diagonal line ------------------ //

		// forward diagonal line 
		// get second point
		boardPoint* secondPoint = NULL;
		
		if (currentPlacedPoint.row + 1 < N_queen_board_size && currentPlacedPoint.col + 1 < N_queen_board_size) {
			
			secondPoint = new boardPoint(currentPlacedPoint.row + 1, currentPlacedPoint.col + 1);

		} else if (currentPlacedPoint.row - 1 >= 0 && currentPlacedPoint.col - 1 >= 0) {
			
			secondPoint = new boardPoint(currentPlacedPoint.row - 1, currentPlacedPoint.col - 1);
		
		}
		
		if (secondPoint != NULL) {

			// forming function Col = a * Row + b
			int a = (secondPoint->col - currentPlacedPoint.col)/
							(secondPoint->row - currentPlacedPoint.row);
			int b = secondPoint->col - a * secondPoint->row;

			// testing if the point is safe
			if (point.col == a * point.row + b) {
				delete (secondPoint);
				return false;
			}

			delete(secondPoint);
		}
		
		// ---------------------- downward diagonal line -------------------- //

		// downward diagonal line 
		// get second point
		secondPoint = NULL;
		
		if (currentPlacedPoint.row + 1 < N_queen_board_size && currentPlacedPoint.col - 1 >= 0) {
			
			secondPoint = new boardPoint(currentPlacedPoint.row + 1, currentPlacedPoint.col - 1);

		} else if (currentPlacedPoint.row - 1 >= 0 && currentPlacedPoint.col + 1 < N_queen_board_size) {
			
			secondPoint = new boardPoint(currentPlacedPoint.row - 1, currentPlacedPoint.col + 1);
		
		}
		
		if (secondPoint != NULL) {

			// forming function Col = a * Row + b
			int a = (secondPoint->col - currentPlacedPoint.col)/
							(secondPoint->row - currentPlacedPoint.row);
			int b = secondPoint->col - a * secondPoint->row;

			// testing if the point is safe
			if (point.col == a * point.row + b) {
				delete (secondPoint);
				return false;
			}

			delete(secondPoint);
		}

	}
	
	return true;
};

// --------------- fibonacci dynamic programming -------
void Fibonacci::execute() {
	Timer m_timer;

	const int _input = 40;
	const int memoSize = _input + 1;

	int memo[memoSize]; // memoize
	for (int i = 0; i < memoSize; i++) memo[i] = -1;
	
	cout << cal_fibonacci(40, memo) << endl;	
}

int Fibonacci::cal_fibonacci(int idx, int memo[]) {
	if (idx == 0 || idx == 1) return 1;
	if (memo[idx] != -1) return memo[idx];

	int result = cal_fibonacci(idx - 1, memo) + cal_fibonacci(idx - 2, memo); 
	memo[idx] = result;

	return result;
}

// --------------- TSP ----------------

void TSP::execute() {
	print2DVector(this->dist);   // print distances
	Node* tmpNode = new Node();
	do_tsp(1, 0, tmpNode); // at postition A (or 0) and MASK = 1 ~ bit = 0001
 
	this->startNode = tmpNode;	
	this->printNodes(startNode);	
}

void TSP::printNodes(Node* startNode) {
	Node* m_node = startNode;
	cout << m_node->currentPoint;
	while (m_node != NULL) {
		cout << " -> " << m_node->bestNextPoint;
		m_node = m_node->nextNode;
	}

	cout << endl;
	cout << "total distances: "<< startNode->bestDistance << endl;
}

bool TSP::isVisited(int mask, int point_pos) {
	return (mask & (1 << point_pos)) != 0;  // 0011 & (1 << 0 = 0001) = 0001 (A is visited!)
}

int TSP::visitedAll() {
	return (1 << this->N) - 1; // -1 ~ BIT = 11111... all node is visited
}

int TSP::do_tsp (int mask, int pos, Node* tmpNode_out) {
	if (mask == this->visitedAll()) {
		tmpNode_out->setInfo(pos, 0, dist[pos][0], NULL); 

		return this->dist[pos][0]; // distance from D/B/C -> start postition (A)
	}

	int distance = INT_MAX;

	for (int i_point = 0; i_point < this->N; i_point ++) {
		if (!this->isVisited(mask, i_point)) {	
			Node* tmpNode = new Node();
			int new_distance = this->dist[pos][i_point] + do_tsp(mask | (1 << i_point), i_point, tmpNode); 

			if(new_distance < distance) {
				distance = new_distance;

				tmpNode_out->setInfo(pos, i_point, distance, tmpNode);
			}
		}

	}	

	return distance;
}
