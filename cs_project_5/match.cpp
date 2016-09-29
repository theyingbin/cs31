#define _CRT_SECURE_NO_WARNINGS
#include <cctype>
#include <cstring>
#include <iostream>
#include <cmath>
#include <cassert>

using namespace std;

const int MAX_WORD_LENGTH = 20;

int standardizeRules(int distance[],
					 char word1[][MAX_WORD_LENGTH + 1],
					 char word2[][MAX_WORD_LENGTH + 1],
					 int nRules);
int determineQuality(const int distance[],
	const char word1[][MAX_WORD_LENGTH + 1],
	const char word2[][MAX_WORD_LENGTH + 1],
	int nRules,
	const char document[]);

int main()
{
	int  a1[] = { 5, 3, 1, 6, 2, 7, 7 };
	char b1[][MAX_WORD_LENGTH + 1] = { "alpha", "phi", "leadership", "omega", "PSI", "GaMmA", "Chi" };
	char c1[][MAX_WORD_LENGTH + 1] = { "leadership", "friendship", "alpha", "service", "APOvalues", "chi", "gamma" };
	int d1 = 7;
	int n = standardizeRules(a1, b1, c1, d1);
	cout << "number of standardized rules : " << n << endl;
	for (int i = 0; i<n; i++){
		cout << a1[i] << ", " << b1[i] << ", " << c1[i] << endl;
	}

}

/*
 * Checks and modifies the arrays to make sure they follow standardized rules
 * Input: an array representing the maximum distance allowed between two cstrings
 *		  two arrays representing the words to be checked for
 *		  an integer representing the number of interesting elements to be looked at in the arrays
 * Return: the number of rules that are "standard" 
 */

int standardizeRules(int distance[], char word1[][MAX_WORD_LENGTH + 1], char word2[][MAX_WORD_LENGTH + 1], int nRules){
	for (int x = 0; x < nRules; x++){
		// Makes all characters in word1 lowercase
		for (int y = 0; word1[x][y] != '\0'; y++){
			word1[x][y] = tolower(word1[x][y]);
		}
		// Makes all characters in word2 lowercase
		for (int y = 0; word2[x][y] != '\0'; y++){
			word2[x][y] = tolower(word2[x][y]);
		}
	}
	bool valid; // This boolean represents whether or not the rule is standardized
	for (int i = 0; i < nRules; i++){
		valid = true; // Resets valid to true;
		// If the distance is nonpositive or either of the cstrings are empty, it is not a valid rule
		if (distance[i] <=0  || word1[i][0] == '\0' || word2[i][0] == '\0'){
			valid = false;
		}
		// Checks to see whether there are any non-alphabetic characters in index i of word1
		for (int k = 0; word1[i][k] != '\0'; k++){
			if (!isalpha(word1[i][k])){
				valid = false;
				break;
			}
		}
		// Checks to see whether there are any non-alphabetic characters in index i of word2
		for (int j = 0; word2[i][j] != '\0'; j++){
			if (!isalpha(word2[i][j])){
				valid = false;
				break;
			}
		}
		// Loops through nRules to check for duplicates
		for (int l = 0; l < nRules; l++){
			// Ignores the loop if l is the same index as i
			if (l == i){
				continue;
			}
			// If there is a duplicate
			if ((strcmp(word1[i],word1[l])==0 && strcmp(word2[i],word2[l])==0) || (strcmp(word1[i],word2[l])==0 && strcmp( word2[i],word1[l])==0 )){
				// If the duplicate has a greater distance, then the current rule is not valid
				if (distance[i] <= distance[l]){
					valid = false;
					break;
				}
			}
		}
		// Checks if the rule is not valid
		if (!valid){
			// If it isn't valid, we move every other element up one space
			for (int m = i + 1; m < nRules; m++){
				strcpy(word1[m - 1], word1[m]);
				strcpy(word2[m - 1], word2[m]);
				distance[m - 1] = distance[m];
			}
			// Decrement i and nRules because we moved every element forward one space
			i--;
			nRules--;
		}
	}
	return nRules; // Returns the number of valid rules
}

/*
 * Checks to see how many matches of the rules are in document
 * Input: An array of integers representing the maximum distance allowed between two cstrings
 *		  Two arrays of cstrings representing the words to be checked for
 *		  An integer representing the number of interesting elements in the arrays
 *		  An array of characters representing the document to be searched through
 * Return: The number of rules that matched in the document
 */

int determineQuality(const int distance[], const char word1[][MAX_WORD_LENGTH + 1], const char word2[][MAX_WORD_LENGTH + 1], int nRules, const char document[]){
	const int MAX_DOCUMENT_LENGTH = 200;
	int score = 0; // Stores the number of standardized rules
	int copyLength = 0; // Stores the length of copyDocument
	char documentCopy[MAX_DOCUMENT_LENGTH + 1];

	// Loops through document and copies document into documentCopy with only lowercase letters and no special symbols
	for (int i = 0; document[i] != '\0'; i++){
		if (isalpha(document[i]) || document[i] == ' '){
			documentCopy[copyLength] = tolower(document[i]);
			documentCopy[copyLength + 1] = '\0';
			copyLength++;
		}
	}

	int placement = 0; // Index of documentCopy
	int temp = 0; // Length/Index of temp cstring
	char wordList[MAX_DOCUMENT_LENGTH][MAX_DOCUMENT_LENGTH + 1]; // A list of cstrings in the document 
	int wordListLength = 0; // Length of the array of cstrings
	char tempString[MAX_DOCUMENT_LENGTH + 1] = ""; // A temporary cstring to be put into wordList
	// Loops until the index of documentCopy(or placement) is greater than the length of the document
	while (placement < copyLength){
		strcpy(tempString, ""); // Resets tempString to become an empty cstring 
		temp = 0; // Resets the length of tempString to become 0

		// Loops through document starting at placement
		for (int k = placement; documentCopy[k] != '\0'; k++){
			// Checks for a space character
			if (documentCopy[k] == ' '){
				// Increments placement and breaks from the loop
				placement++;
				break;
			}
			else{
				tempString[temp] = documentCopy[k]; // Copies the current character into the end of tempString
				tempString[temp + 1] = '\0'; // Adds a terminating byte character
				temp++; // Increments the length of tempString
				placement++; // Increments placement
			}
		}
		// Checks if the length of tempString is greater than 0
		if (temp > 0){
			strcpy(wordList[wordListLength], tempString); // Copies tempString into our new array, wordList
			wordListLength++; // Increments the length of the new array
		}
	}
	// Loops through each individual rule 
	for (int m = 0; m < nRules; m++){
		// Loops through wordList
		for (int n = 0; n < wordListLength; n++){
			int previousScore = score; // Creates an int to compare later
			// Checks to see if the first word we are looking for appears in wordList
			if (strcmp(word1[m], wordList[n]) == 0){
				for (int j = 0; j < wordListLength; j++){
					// Checks to see if the second word we are looking for appears in wordList
					if (strcmp(word2[m], wordList[j]) == 0){
						// Checks if the distance between the two words is less than or equal to the distance allowed
						if (abs(j - n) <= distance[m]){
							// Increments score and breaks out of the loop
							score++;
							break;
						}
					}
				}
			}
			// Checks if score has changed
			if (score != previousScore){
				break; 
			}
		}
	}
	return score; // Return the score
}