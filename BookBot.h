#pragma once
#include <fstream>
#include <vector>
#include <string>
#include <map>

#include <iostream>

#include "sanitize.h"

using namespace std;

class BookBot {
private:
	std::map<std::string, std::vector<std::string>> markov_chain;
public:
	BookBot(const unsigned int seed);
	void readIn(const std::string& filename);
	bool isEndPunctuation(const char character);
	std::vector<std::string> getValues(const std::string& key);
	std::string generateSentence();
};

BookBot::BookBot(const unsigned int seed) {
	srand(seed);										// saving seed to srand
}

// checking if we are at the end of the sentence by testing if the char is: . , ? , or !
bool BookBot::isEndPunctuation(const char character) {
	if (character == '.' || character == '?' || character == '!') { 			// checking if char is . , ? , or !
		return true;
	}
	else {
		return false;
	}
}

void BookBot::readIn(const std::string& filename) {

	ifstream inFile(filename);								 // opening the file

	//Check For Error
	if (inFile.fail()) {
		cerr << "error opening file" << endl;
		exit(1);

	}
	string word;
	string word2 = "^"; 									// saving previous word as beginning of sentence

	while (inFile >> word) { 								// reading in one word at a time

		sanitize(word);

		char punc = word[word.size() - 1]; 						// saving last letter of string as char
		if (isEndPunctuation(punc)) { 							// checking if the punc char is punctuation
			string punctuation(1, word[word.size() - 1]);				// capturing punctuation from last letter of string
			word = word.substr(0, word.size() - 1); 				// resaving word as the string without the last letter
			markov_chain[word2].push_back(word);					// saving word in vector of previous word's key
			markov_chain[word].push_back(punctuation); 				// saving the punctuation in vector of word's key
			markov_chain[punctuation].push_back("$"); 				// inserting $ in vector corresponding to the punctuation
			word2 = "^";								// starting new sentence
		}
		else {
			markov_chain[word2].push_back(word); 					// saving the word in the vector belonging to previous word's key
			word2 = word; 								// updating old word
		}
	}
	inFile.close(); 									// close text file					
}
std::vector<std::string> BookBot::getValues(const std::string& key) {
	return markov_chain[key];								// return vector of words associated with the given key
}

std::string BookBot::generateSentence() {
	string key = "^"; // begin the sentence with ^
	string sentence; // sentence string
	string new_word; // word following key

	while (key != "$") { 									// check to see if at the end of the sentence
		string new_word = markov_chain[key].at(rand() % (markov_chain[key].size()));	// randomly select a word from key's vector and save as new word
		if (new_word == "." || new_word == "!" || new_word == "?" || key == "^") { 	// check to see if we are at the begininng or end of sentence
			sentence = sentence + new_word; 					// insert the punctuation at the end of the sentence without a space
			key = new_word; 							// update the key with the new word
		}
		else if (new_word == "$"){
            key = new_word;
		}

		else {
			sentence = sentence + " " + new_word; 					// insert the new word at the end of the sentence with a space
			key = new_word; 							// update the key with the new word
		}
	}
	return sentence;// return full sentence
}

