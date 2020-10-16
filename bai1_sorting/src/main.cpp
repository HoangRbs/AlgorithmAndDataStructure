#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <vector>
#include "../includes/Timer.cpp"

using namespace std;

class BubbleSort {
	public:
		vector<int> initialArray;

		BubbleSort(vector<int>& initialArray) {
			this->initialArray = initialArray;
		}

		void execute();
};

class QuickSort {
	public: 
		vector<int> initialArray;

		QuickSort (vector<int>& initialArray) {
			this->initialArray = initialArray;
		}

		void execute();
		void do_quickSort(int* L, int* R);
		void partition(int* L, int* R); // Left, Right pointer
};

class MergeSort {
	public: 
		vector<int> initialArray;

		MergeSort (vector<int>& initialArray) {
			this->initialArray = initialArray;
		}
		
		void execute();
		vector<int> do_mergeSort(int arraySize, int startIndex, int lastIndex);
		vector<int> merge(vector<int>& mergedLeftArray, vector<int>& mergedRightArray);
};

class HeapSort {
	public: 
		vector<int> initialArray;

		HeapSort (vector<int>& initialArray) {
			this->initialArray = initialArray;
		}

		void execute();
		void max_Heapify(vector<int>& m_arr, int arrSizeForHeap ,int parentIndex);
};

void printArray(vector<int>& arr) {
	for (int i = 0; i < arr.size(); i++){
		cout << arr[i] << " ";
	}
};

void swapNumber(int& a, int& b) {
	int temp = a;
	a = b;
	b = temp;
};

int main() {
	srand(time(NULL));

	vector<int> randomArray;
	int randomArraySize = 8;

	// generate random array
	for (int i = 0; i < randomArraySize; i++) { 
		int randomNumber = rand()%20 + 1;
		randomArray.push_back(randomNumber);
	}

	// BubbleSort m_sort = BubbleSort(randomArray);  // intialize bubble sort  
	// QuickSort m_sort = QuickSort(randomArray);			 // initialize quick sort 
	// MergeSort m_sort = MergeSort(randomArray);
	HeapSort m_sort = HeapSort(randomArray);

	if (randomArraySize <= 20) {
		// display array with small size for testing

  	cout << "-------initial Array--------" << endl;
		printArray(m_sort.initialArray);

		m_sort.execute();     // start executing

		cout << endl << "-------sorted Array----------"<<endl;
		printArray(m_sort.initialArray);
		cout << endl << endl;
	} else {
		// with bigger array size such as 10000, need accurate time measure
		m_sort.execute();
	}
};

// ------------- bubble sort --------------
void BubbleSort::execute() {
	Timer m_timer;
	
	for(int i = this->initialArray.size() - 1; i > 0; i--) {
		for(int j = 0; j < i; j++) {
			if (this->initialArray[j] > this->initialArray[j + 1]) {
				swapNumber(this->initialArray[j], this->initialArray[j + 1]);
			}
		}
	}
}

// ------------- quick sort ---------------
void QuickSort::execute() {
	Timer m_timer;
	
	vector<int>& m_arr = this->initialArray;

	do_quickSort(&m_arr[0], &m_arr[m_arr.size() - 1]);	
}

void QuickSort::do_quickSort(int* L, int* R) {
	if (R <= L) { // if R == L (only one number in array) 
								// or L > R (when the new array next to the pivot is empty)
		return;
	}

	int* lastElement = R; // get the current last element in smaller array
	int* firstElement = L; // get the current first element in smaller array

	partition(L, R); // after partition, L = R = pivot position
	int* m_pivot = L;
	
	do_quickSort(firstElement, m_pivot - 1); // quick sort left side of pivot
	do_quickSort(m_pivot + 1, lastElement); // quick sort right side of pivot
}

void QuickSort::partition(int* L, int* R) {

	int pivot = *R;						// my default pivot is the outer right side of the array
	bool L_turn = true; 			// -> Left turn first

	while (L != R) {
		if (L_turn) {
			if (*L > pivot) { 		// left > pivot -> move to right -> R_turn
				*R = *L;
				R--;								
				L_turn = false; 		
			} else { 							// left <= pivot, move Left
				L++;
			}
		} else { 								// R_turn
			if (*R < pivot) {			// if right < pivot -> move to left -> L_turn 
				*L = *R;
				L++;
				L_turn = true;
			}	else {							// right >= pivot, move right
				R--;
			}
		}
	}

	*L = pivot; // place pivot value where  L == R;
}

// --------------- merge sort -----------------------
void MergeSort::execute() {
	Timer m_timer;
	
	this->initialArray = do_mergeSort(this->initialArray.size(), 0, this->initialArray.size() - 1);
};

vector<int> MergeSort::do_mergeSort(int arraySize, int startIndex, int lastIndex) {

	if (arraySize == 1) { // ~~ startIndex == lastIndex, reach the leaf node
		vector<int> tmp;
		tmp.push_back(this->initialArray[startIndex]);
		return tmp;
	}

	int dividerIndex = (arraySize - 1) / 2 + startIndex;
	
	// left array
	int leftArray_startIndex = startIndex;
	int leftArray_lastIndex = dividerIndex;
	vector<int> mergedLeftArray = do_mergeSort((dividerIndex - startIndex) + 1, 
																						 leftArray_startIndex, 
																						 leftArray_lastIndex);

	// right array
	int rightArray_startIndex = dividerIndex + 1;
	int rightArray_lastIndex = lastIndex;
	vector<int> mergedRightArray = do_mergeSort(lastIndex - dividerIndex, 
																							rightArray_startIndex, 
																							rightArray_lastIndex);

	// merge left array and right array
	return merge(mergedLeftArray, mergedRightArray);
};

vector<int> MergeSort::merge(vector<int>& mergedLeftArray, vector<int>& mergedRightArray) 
{
	vector<int> tmp;
	
	int leftArrIndex = 0;
	int rightArrIndex = 0;
	
	while (leftArrIndex <= mergedLeftArray.size() - 1 
					&& rightArrIndex <= mergedRightArray.size() - 1) {

		if (mergedLeftArray[leftArrIndex] > mergedRightArray[rightArrIndex]) {
			tmp.push_back(mergedRightArray[rightArrIndex]);
			rightArrIndex++;
		} else {
			tmp.push_back(mergedLeftArray[leftArrIndex]);
			leftArrIndex++;
		}
	};
	
	// pushing the left over
	while (leftArrIndex <= mergedLeftArray.size() - 1) {
		tmp.push_back(mergedLeftArray[leftArrIndex]);
		leftArrIndex++;
	};

	while (rightArrIndex <= mergedRightArray.size() - 1) {
		tmp.push_back(mergedRightArray[rightArrIndex]);
		rightArrIndex++;
	};

	return tmp;
};


// --------------- Heap Sort ---------------------
void HeapSort::execute() {
	Timer m_timer;
	vector<int>& m_arr = this->initialArray;

	// build max heap at first time 
	// start from that last parent of the array (size/2 - 1)
	for (int i = m_arr.size()/2 - 1; i >= 0; i--) {
		max_Heapify(m_arr, m_arr.size(), i);
	}

	int arrSizeForHeap = m_arr.size();

	// after we archive a max heap. SWAP the last element with the top parent node (the biggest number after builing max heap)
	// this will push the biggest number to the last position on the array	
	for (int j = m_arr.size() - 1; j >= 0; j--) {
		swapNumber(m_arr[0], m_arr[j]);

		// access heap is based on array size
		// ex: arr size decrease by one 
		// ~~ access to leaf node will also decrease	
		arrSizeForHeap--; 

		// after initial building max heap
		// always heapify from the top (index = 0) 
		max_Heapify(m_arr, arrSizeForHeap, 0);		
	}	
};

void HeapSort::max_Heapify(vector<int>& m_arr, int arrSizeForHeap, int parentIndex) {
	
	int leftIndex = parentIndex * 2 + 1;
	int rightIndex = parentIndex * 2 + 2;

	// leaf node
	if (leftIndex > arrSizeForHeap - 1 && rightIndex > arrSizeForHeap - 1) return; 

	// determine whether left or right value is bigger
	bool isLeftBigger = false, isRightBigger = false;

	// parent has both left and right node
	if (leftIndex <= arrSizeForHeap - 1 && rightIndex <= arrSizeForHeap - 1) {
		
		if(m_arr[leftIndex] > m_arr[rightIndex] &&
			 m_arr[leftIndex] > m_arr[parentIndex]) {
			
			isLeftBigger = true;
		} else if (m_arr[rightIndex] >= m_arr[leftIndex] &&
							 m_arr[rightIndex] > m_arr[parentIndex]) {
			
			isRightBigger = true;
		}
	} else if (leftIndex <= arrSizeForHeap - 1 && 
						 rightIndex > arrSizeForHeap - 1 && 
						 m_arr[parentIndex] < m_arr[leftIndex]) {
		
		// parent has only left node and apply one condition
		isLeftBigger = true;
	} else if (rightIndex <= arrSizeForHeap - 1 && 
						 leftIndex > arrSizeForHeap - 1 && 
						 m_arr[parentIndex] < m_arr[rightIndex]) {
		
		// parent has only right node and apply one condition
		isRightBigger = true;
	}

	if (isLeftBigger) {
		swapNumber(m_arr[parentIndex], m_arr[leftIndex]);
		max_Heapify(m_arr, arrSizeForHeap, leftIndex);
	} else if (isRightBigger) {
		swapNumber(m_arr[parentIndex], m_arr[rightIndex]);
		max_Heapify(m_arr, arrSizeForHeap, rightIndex);
	}

	// -> return
}
