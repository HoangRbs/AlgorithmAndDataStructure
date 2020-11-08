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

class WordNode {
public:
	string word = "";
	WordNode* nextWord = NULL;
	WordInfoNode* nextInfo = NULL;
};

class WordNodeList {
	public:
		WordNode* pHead = NULL;
		WordNode* pTail = NULL;

		void insertWord(string word, int line) {
			if (pHead == NULL) {
				pHead = new WordNode();
				pHead->word = word;
				
				pHead->nextInfo = new WordInfoNode();
				pHead->nextInfo->line = line;

				pTail = pHead;
			
			} else {
				WordNode* pWord = this->pHead;
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
					pTail->nextWord = new WordNode();
					pTail = pTail->nextWord;
				
					// add next info
					pTail->word = word;
					pTail->nextInfo = new WordInfoNode();
					pTail->nextInfo->line = line;
				} 
			}
		}

		void findWord(string word) {
			if (this->pHead == NULL) {
				cout << "Error pHead is empty !"<< endl; 
				return;
			}

			WordNode* pWord = this->pHead;
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

			cout << " (count): " << count << endl; 
		}

};

class WordHashTable {
	public: 
		static const int MAX_ELEMENTS = 40;
		WordNodeList m_hashTable[MAX_ELEMENTS];

		void insertWord(string word, int line) {
			int wordHashIndex = this->getHashIndex(word);
			this->m_hashTable[wordHashIndex].insertWord(word, line);	
		}

		void findWord(string word) {
			this->m_hashTable[this->getHashIndex(word)].findWord(word);
		}

		int getHashIndex(string word) {
			int sum = 0;
			for (int i = 0; i < word.size(); i++) {
				sum = sum + (int) word[i]; // sum of ascii code of each character 
			}
		
			// divide by max elements and get the remainders
			// the remainders will always less then max elements -> fill up the array 
			int finalIndex = sum % MAX_ELEMENTS; 
			return finalIndex;
		}
};

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

void loadAndFindKeywords (WordHashTable& m_wordsTable) {
	ifstream mFile("keywords.txt");
	string line;
	
	while(getline(mFile, line)) {
		string word = line; 
		m_wordsTable.findWord(word);
	}

	mFile.close();
}

int main () {
	WordHashTable m_wordHashTable;
	loadParagraph(m_wordHashTable);
	loadAndFindKeywords(m_wordHashTable);
}
