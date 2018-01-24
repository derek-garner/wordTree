#include <string>
#include <vector>
#include <iostream>
using namespace std;
#ifndef GAME_H
#define GAME_H

//Used for entries into dictionary
class wordStore {
public:
	wordStore();
	wordStore(string insertWord);
	string word;
	bool used;
	friend bool compareByWordValue(const wordStore &a, const wordStore &b); 
};

class Node {
public:
	vector<string> wordladder;
	Node* next;
	Node* prev;
	Node(string word, Node* newNext = NULL, Node* newPrev = NULL);
};

class DoublyLinkedList {
public:
	int size;
	Node* head;
	Node* tail;

	DoublyLinkedList() {
		head = NULL;
		size = 0;
		tail = NULL;
	}
	void addToTail(string x);
	void remove();
	void clear();
	bool isEmpty() { return head == NULL; }
	void printEachItem();
};


class Game {
public:
	// Constructor loads dictionary into vector
	Game(string file);

	//Method to list first ten words as a test
	void listWords();
	void play(string firstWord);
	void play(string firstWord, string secondWord);
	bool findWord(int dictionaryLength,string firstWord,vector<wordStore> &dictionary);
	bool findSolution(string firstWord, string secondWord = NULL);
	void printAllLadders();

	vector<wordStore> fullDictionary;
	vector<wordStore> shortDictionary;
	void buildLadder(string firstWord, string secondWord);

	int fullDictionaryLength;
	int shortDictionaryLength;
	DoublyLinkedList ladderList;
};


#endif