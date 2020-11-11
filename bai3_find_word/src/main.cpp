#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <sstream>
#include "../includes/Timer.cpp"

using namespace std;

class WordInfoNode {
public:
	int line = 0; // line in paragraph that contains the word
	WordInfoNode* nextInfo = NULL;
};

class WordHashTableNode {
public:
	string word = "";
	WordHashTableNode* nextWord = NULL;
	WordInfoNode* nextInfo = NULL;
};

class WordHashTableNode_List {
	public:
		WordHashTableNode* pHead = NULL;
		WordHashTableNode* pTail = NULL;

		void insertWord(string word, int line);
		void findWord(string word);
};

class WordHashTable {
	public: 
		static const int MAX_ELEMENTS = 40;
		WordHashTableNode_List m_hashTable[MAX_ELEMENTS];

		void insertWord(string word, int line);
		void findWord(string word);
		static int getHashIndex(string word);
};

class WordBinaryNode {
public:
	WordBinaryNode (string word) {
		this->word = word;
	}

	string word;
	WordInfoNode* pNextInfoHead = NULL;
	WordInfoNode* pNextInfoTail = NULL;

	WordBinaryNode* left = NULL;
	WordBinaryNode* right = NULL;
};

class WordBinaryTree {
	public:
		WordBinaryNode* pRoot = NULL; // tree to save words in paragraph
		void insertWord(string word, int line);
		void findWord(string word);
};

/*
void loadParagraph (WordHashTable& m_wordsTable) {
	ifstream mFile ("paragraphs.txt");

	string line;
	int lineNumber = 1;

	while (getline(mFile, line)) {
		// read each word in a line
		string word;
		stringstream iss(line);

		while (iss >> word) {
			m_wordsTable.insertWord(word, lineNumber);
		}

		lineNumber++;
	}

	mFile.close();
}
*/

void loadParagraph (WordBinaryTree& m_wordsTree) {
	ifstream mFile ("paragraphs.txt");

	string line;
	int lineNumber = 1;

	while (getline(mFile, line)) {
		// read each word in a line
		string word;
		stringstream iss(line);

		while (iss >> word) {
			// cout << "\n inserting word: " << word;
			m_wordsTree.insertWord(word, lineNumber);
			// cout << " -> insert succeed! "<< endl;
		}

		lineNumber++;
	}

	mFile.close();
}


// for testing : saved by hash index order
string words_order[WordHashTable::MAX_ELEMENTS] = {""};

/*
void loadAndFindKeywords (WordHashTable& m_wordsTable) {
	ifstream mFile("keywords.txt");
	string line;
	
	while(getline(mFile, line)) {
		string word = line; 
		// m_wordsTable.findWord(word);
		words_order[WordHashTable::getHashIndex(word)] = word;
	}

	for (int i = 0; i < WordHashTable::MAX_ELEMENTS; i ++) {
		if (words_order[i] != "") m_wordsTable.findWord(words_order[i]);
	}

	mFile.close();
}
*/

void loadAndFindKeywords (WordBinaryTree& m_wordsTree) {
	ifstream mFile("keywords.txt");
	string line;
	
	while(getline(mFile, line)) {
		string word = line; 
		words_order[WordHashTable::getHashIndex(word)] = word;
	}

	for (int i = 0; i < WordHashTable::MAX_ELEMENTS; i ++) {
		if (words_order[i] != "") m_wordsTree.findWord(words_order[i]);
	}

	mFile.close();
}

int main () {
	// WordHashTable m_wordHashTable;
	WordBinaryTree m_wordTree;

	loadParagraph(m_wordTree);
	loadAndFindKeywords(m_wordTree);
	
}

void WordHashTableNode_List::insertWord(string word, int line) {
	if (pHead == NULL) {
		pHead = new WordHashTableNode();
		pHead->word = word;
		
		pHead->nextInfo = new WordInfoNode();
		pHead->nextInfo->line = line;

		pTail = pHead;
	
	} else {
		WordHashTableNode* pWord = this->pHead;
		while (pWord != NULL) {
			if (pWord->word == word) {
				break;
			}

			pWord = pWord->nextWord;
		} 

		if (pWord != NULL) {
			// an existed word -> add new info (line) at the end of the word info list
			WordInfoNode* pWordInfo = pWord->nextInfo;

			while (pWordInfo->nextInfo != NULL) 
				pWordInfo = pWordInfo->nextInfo;
			
			pWordInfo->nextInfo = new WordInfoNode();
			pWordInfo->nextInfo->line = line;

		} else {
			// pWord == NULL : end of the list -> add new word
			pTail->nextWord = new WordHashTableNode();
			pTail = pTail->nextWord;
		
			// add next info
			pTail->word = word;
			pTail->nextInfo = new WordInfoNode();
			pTail->nextInfo->line = line;
		} 
	}
}

void WordHashTableNode_List::findWord(string word) {
	if (this->pHead == NULL) {
		// cout << "Error pHead is empty !"<< endl; 
	
		cout << endl << "\"" << word << "\"" << ": cannot find word !" << endl;
		return;
	}

	WordHashTableNode* pWord = this->pHead;
	while (pWord != NULL) {
		if (pWord->word == word) break; 
		pWord = pWord->nextWord;
	}

	if (pWord == NULL) {
		cout << endl << "\"" << word << "\"" << ": cannot find word !" << endl;
		return;
	}

	// print output
	int count = 0;
	int previousLineOutput = -1;
	WordInfoNode* pWordInfo = pWord->nextInfo;

	cout << endl << "\"" << pWord->word << "\"" << ": " << "(line): ";
	
	while (pWordInfo != NULL) {
		// prevent print out repeated line
		if (previousLineOutput != pWordInfo->line) {
			cout << pWordInfo->line << " ";
			previousLineOutput = pWordInfo->line; 
		}
		count++;
		pWordInfo = pWordInfo->nextInfo;
	}

	cout << " (count): " << count;
	cout << " (hash index): " << WordHashTable::getHashIndex(word);
}

void WordHashTable::insertWord(string word, int line) {
	int wordHashIndex = this->getHashIndex(word);
	this->m_hashTable[wordHashIndex].insertWord(word, line);	
}

void WordHashTable::findWord(string word) {
	this->m_hashTable[this->getHashIndex(word)].findWord(word);
}

int WordHashTable::getHashIndex(string word) {
	int sum = 0;
	for (int i = 0; i < word.size(); i++) {
		sum = sum + (int) word[i]; // sum of ascii code of each character 
	}

	// divide by max elements and get the remainders
	// the remainders will always less then max elements -> fill up the array 
	int finalIndex = sum % MAX_ELEMENTS; 
	return finalIndex;
}


void WordBinaryTree::insertWord(string word, int line) {
	if (this->pRoot == NULL) {
		this->pRoot = new WordBinaryNode(word);
		this->pRoot->pNextInfoHead = new WordInfoNode();
		this->pRoot->pNextInfoHead->line = line;
		this->pRoot->pNextInfoTail = this->pRoot->pNextInfoHead;
	
	} else {
		WordBinaryNode* pWord = this->pRoot;
		bool addOnLeftSide = false;

		while (true) {
			if (pWord->word == word) {
				pWord->pNextInfoTail->nextInfo = new WordInfoNode();
				pWord->pNextInfoTail = pWord->pNextInfoTail->nextInfo;
				pWord->pNextInfoTail->line = line;

				return;
			} else {
				if (WordHashTable::getHashIndex(word) <= WordHashTable::getHashIndex(pWord->word)) {
					if (pWord->left == NULL) {
						addOnLeftSide = true;
						break;
					} else {
						pWord = pWord->left;
					}
				} else {
					if (pWord->right == NULL) {
						addOnLeftSide = false;
						break;
					} else {
						pWord = pWord->right;
					}
				}
			}
		}

		if (addOnLeftSide) {	
			pWord->left = new WordBinaryNode(word);
			pWord = pWord->left;
			pWord->pNextInfoHead = new WordInfoNode();
			pWord->pNextInfoHead->line = line;
			pWord->pNextInfoTail = pWord->pNextInfoHead;
		
		} else {
			pWord->right = new WordBinaryNode(word);
			pWord = pWord->right;
			pWord->pNextInfoHead = new WordInfoNode();
			pWord->pNextInfoHead->line = line;
			pWord->pNextInfoTail = pWord->pNextInfoHead;
		}
	}
}

void WordBinaryTree::findWord(string word) {
	WordBinaryNode* pWord = this->pRoot;

	while (pWord != NULL) {
		if (pWord->word == word) {
			// print output
			int count = 0;
			int previousLineOutput = -1;
			WordInfoNode* pWordInfo = pWord->pNextInfoHead;
		
			cout << endl << "\"" << pWord->word << "\"" << ": " << "(line): ";
				
			while (pWordInfo != NULL) {
				// prevent print out repeated line
				if (previousLineOutput != pWordInfo->line) {
					cout << pWordInfo->line << " ";
					previousLineOutput = pWordInfo->line; 
				}
				count++;
				pWordInfo = pWordInfo->nextInfo;
			}
		
			cout << " (count): " << count;
			cout << " (hash index): " << WordHashTable::getHashIndex(word);
			return;
		
		} else {
			if (WordHashTable::getHashIndex(word) <= WordHashTable::getHashIndex(pWord->word)) {
				pWord = pWord->left;
			} else {
				pWord = pWord->right;
			}
		}
	}

	cout << endl << "\"" << word << "\"" << ": cannot find word !" << endl;
}
