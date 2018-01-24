#include "Game.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <time.h>
using namespace std;

//Constructor for wordStore object
wordStore::wordStore()
{
	word = "";
	used = false;
}

wordStore::wordStore(string insertWord)
{
	word = insertWord;
	used = false;
}

Node::Node(string wordInsert, Node * anext, Node * aprev){
	next = NULL;
	prev = NULL;
	wordladder.push_back(wordInsert);
}

//Method to add an item to end of list
void DoublyLinkedList::addToTail(string insertWord){
	Node* n = new Node(insertWord,head,tail);
	size++;
	if (isEmpty()) {
		
		head = n;
		tail = n;
		return;
	}
	else {
		tail->next = n;
		tail = n;
	}
}

//Method to remove from tail
void DoublyLinkedList::remove(){
	if (isEmpty()) return ;

	if (head ->next == NULL) {
		delete head;
		head = NULL;
		tail = NULL;
		return;
	}
	Node* curr = head;
	while (curr->next != tail) {
		curr = curr->next;
	}
	delete tail;
	curr->next = NULL;
	tail = curr;
	return;
}

//Method to clear the list
void DoublyLinkedList::clear(){
	while (!isEmpty()) {
		remove();
	}
}

//Prints ladders to text file if needed
void Game::printAllLadders()
{
	fstream fout("output.txt");
	Node* curr = ladderList.head;
	while (curr != NULL) {
		for (int i = 0; i < curr->wordladder.size(); i++) {
			fout << curr->wordladder[i] << endl;
		}
		fout << endl;
		curr = curr->next;
	}
	
	return;

}

//Print the top of each Ladder, or the 
void DoublyLinkedList::printEachItem() {
	Node* curr = head;
	cout << "Displaying all one letter away possibilities: \n";
	while (curr!=NULL){
		cout << curr->wordladder[0] << endl;
	
	curr = curr->next;
	}
	//cout << endl;
	return;
}



//Method to check for solution in ladders
bool Game::findSolution(string firstWord,string secondWord){
	Node* curr = ladderList.head;

	//Temporary while loop fix to stop out of bounds
	while (curr != NULL) {
		for (int i = 0; i < curr->wordladder.size(); i++) {
			if (curr->wordladder[i] == secondWord) {
				cout << "Solution Found" << endl;
				cout << firstWord<<endl;
				for (int j = 0; j < curr->wordladder.size();j++) {
					cout << curr->wordladder[j] << endl;
				}
				cout << endl;
				return true;
			}
		}
		curr = curr->next;
	}
	return false;
}



//Method to compare the words via their integer value
//Used with binary search
bool compareByWordValue(const wordStore & a, const wordStore & b){
	return  b.word > a.word;
}

//Game constructor
Game::Game(string file) {
	fullDictionaryLength=0;
	ifstream fin(file);
	fin.clear();

	while (!fin.eof()) {
		wordStore temp;
		getline(fin, temp.word);
		fullDictionary.push_back(temp);
		fullDictionaryLength++;
	}
	//Sort used to ensure words are searchable by their numerical value
	sort(fullDictionary.begin(), fullDictionary.end(),compareByWordValue);
	cout <<"Dictionary loaded "<< fullDictionaryLength<<" words."<<endl;
};

//Method to search dictionary for word
bool Game::findWord(int dictionaryLength, string firstWord,vector<wordStore> &dictionary) {
	
	int low = 0;
	int high = dictionaryLength -1;
	int mid; 
	while (low <= high) {
		
		mid = low + (high - low) / 2;
		if (dictionary[mid].word == firstWord) {
			if (dictionary[mid].used == false) {
				dictionary[mid].used = true;
				return true;
			}
			return false;
		}
			else if (dictionary[mid].word < firstWord) {
				low = mid + 1;
			}
			else if(dictionary[mid].word>firstWord)  {
				high = mid - 1;
			}
		}
	return false;
}

//Method to list first ten words in dictionary as a test
void Game::listWords() {
	cout << "Printing first 10 words of dictionary as test: " << endl;
	for (int i = 0; i < 10; i++) {
		cout << fullDictionary[i].word<<endl;
	}
}

void Game::play(string firstWord) {
	srand(time(NULL));
	int firstWordLength;
	string secondWord;
	shortDictionaryLength = 0;
	firstWordLength = firstWord.length();
	for (int i = 0; i < fullDictionaryLength; i++) {
		fullDictionary[i].used = false;
		if (fullDictionary[i].word.length() == firstWord.length()) {
			shortDictionary.push_back(fullDictionary[i]);
			shortDictionaryLength++;
		}
	}
	firstWord= shortDictionary[rand() % shortDictionaryLength - 1].word;
	secondWord = shortDictionary[rand() % shortDictionaryLength - 1].word;

	play(firstWord, secondWord);

}

//Produces word ladder when taking in two strings as parameters
//Checks for valid words
void Game::play(string firstWord, string secondWord) {
	bool soulution = false;
	int lettersInAlpha = 26;
	shortDictionaryLength = 0;
	shortDictionary.clear();
	ladderList.clear();

	cout << "==============Testing words: " << firstWord << " " << secondWord <<"=================="<< endl;
	if (firstWord.length() != secondWord.length()) {
		cout << "Words are not the same length!\n" << endl;
		return;
	}
	if (!findWord(fullDictionaryLength, firstWord,fullDictionary)) {
		cout << "First word not found in dictionary" << endl;
		if (!findWord(fullDictionaryLength, secondWord, fullDictionary))
			cout << "and second word not found in dictionary\n" << endl;
		return;
	}
	if (!findWord(fullDictionaryLength, secondWord, fullDictionary)) {
		cout << "Second word not found in dictionary\n" << endl;
		return;
	}
	for (int i = 0; i < fullDictionaryLength; i++) {
		fullDictionary[i].used = false;
		if (fullDictionary[i].word.length() == firstWord.length()) {
			shortDictionary.push_back(fullDictionary[i]);
			shortDictionaryLength++;
			shortDictionary[shortDictionaryLength - 1].used = false;
		}
	}

	buildLadder(firstWord, secondWord);
	findSolution(firstWord,secondWord);
}

//Method to build all ladders in attempt to find solution
void Game::buildLadder(string firstWord,string secondWord) {
	string tempFirstWord = firstWord;
	int lettersInAlpha = 26;
	bool solutionFoundFirst = false;
	//Create initial ladders check for all 1 way words for head of each ladder
	for (int i = 0; i < firstWord.length(); i++) {
		for (int j = 97; j < lettersInAlpha + 97; j++) {
			tempFirstWord[i] = j;

			if (findWord(shortDictionaryLength, tempFirstWord,shortDictionary)) {
				ladderList.addToTail(tempFirstWord);
				if (tempFirstWord == secondWord) solutionFoundFirst=true;
			}
		}
		tempFirstWord = firstWord;
	}
	if (solutionFoundFirst) {
		ladderList.printEachItem();
		return;
	}

	//Build subsequent steps for each ladder until solution found
	//or no new steps for any ladder can be created
	//Below only executes if the ladder isn't solved in one step

	int stepInLadder = 0;
	int newSteps = 0;
	bool currFound = false;
	bool noLadder = false;
	ladderList.printEachItem();

	Node* curr = ladderList.head;
	string tempWord;
	while (!noLadder) {
		curr = ladderList.head;
		currFound = false;
		newSteps = 0;
		while (curr != NULL) {
			stepInLadder = curr->wordladder.size();
			stepInLadder--;
			tempWord = curr->wordladder[stepInLadder];

			for (int i = 0; i < firstWord.length(); i++) {
				for (int j = 97; j < lettersInAlpha + 97; j++) {
					tempWord[i] = j;

					if (currFound == false) {
						if (findWord(shortDictionaryLength, tempWord, shortDictionary)) {
							curr->wordladder.push_back(tempWord);
							currFound = true;
							newSteps++;
							if (tempWord == secondWord)return;
						}
					}
					else if (findWord(shortDictionaryLength, tempWord, shortDictionary)) {
						ladderList.addToTail(curr->wordladder[0]);
						for (int i = 1; i < curr->wordladder.size(); i++) {
							ladderList.tail->wordladder.push_back(curr->wordladder[i]);
						}
						
						ladderList.tail->wordladder.push_back(tempWord);
						if (tempWord == secondWord)return;
						currFound = true;
						newSteps++;
					}	
				}
				tempWord = curr->wordladder[stepInLadder];
			}
			curr = curr->next;
		}
		if (newSteps == 0) noLadder = true;
	}
	cout << "No Ladder Found"<<endl;
}


