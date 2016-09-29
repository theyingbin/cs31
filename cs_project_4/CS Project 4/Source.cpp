#include <iostream>
#include <string>

using namespace std;

int appendToAll(string a[], int n, string value);
int lookup(const string a[], int n, string target);
int positionOfMax(const string a[], int n);
int rotateLeft(string a[], int n, int pos);
int rotateRight(string a[], int n, int pos);
int flip(string a[], int n);
int differ(const string a1[], int n1, const string a2[], int n2);
int subsequence(const string a1[], int n1, const string a2[], int n2);
int lookupAny(const string a1[], int n1, const string a2[], int n2);
int separate(string a[], int n, string separator);

int main(){
	
}
/*
 * Adds the string value to the end of the first n strings in the array a 
 * Parameters: An array a in which the contents will be modified,
 *			   An integer n representing the number of strings we will look at starting from the beginning of the array,
 *             A string value representing the string we are adding to the end of each array
 * Return: -1 if a bad argument is given and n otherwise
 */
int appendToAll(string a[], int n, string value){
	// Checks for bad argument
	if (n < 0)
		return -1;
	// Loops through the array and adds value to each one
	for (int i = 0; i < n; i++){
		a[i] += value;
	}
	return n;
}
/*
 * Returns the first index in which the string target was found in the array a
 * We keep look up n contents in array a
 * Parameters: An array a that cannot be changed. It represents the array we are looking through
 *			   An integer n that represents the number of elements we are looking through in array a
 *			   A string target that represents the string we are looking for in array a
 * Return: Returns the position in which the target was found
 *		   Returns -1 if the target was not found or if a bad argument was given(thus not going through the loop) 
 */
int lookup(const string a[], int n, string target){
	// We loop though the array and check so see if the content at that particular index is equal to target
	for (int i = 0; i < n; i++){
		// If they are equal, we return the current position we are at
		if (a[i] == target){
			return i;
		}
	}
	// If we get to this point, either a bad argument was given or we couldn't find the string in the array
	return -1;
}
/*
 * Returns the position of the "greastest" string
 * Parameters: An array a not to be changed in wich we will be looking through
 *			   An integer n representing the number of elements to be looked upon
 * Return: The position of the "maximum" string or -1 if we are given bad arguments
 */
int positionOfMax(const string a[], int n){
	// Checks to see that there are elements in the array
	if (n <= 0){
		return -1;
	}
	// Initializes the maximum to be the first element
	string maximum = a[0];
	int maxPosition = 0;
	// Loops through the array and checks if the element at the current index is greater than maximum
	for (int i = 1; i < n; i++){
		if (a[i]>maximum){
			maximum = a[i];
			maxPosition = i;
		}
	}
	return maxPosition;
}
/*
 * Moves the element at index pos all the way to the end of the array and moves everything to the right of pos left one element
 * Parameters: An array of strings to be modified
 *			   An integer representing the number of elements from the beginning of array a to be modified
 *			   An integer representing the index where the change will be made
 * Return: Returns pos if everything went accordingly and -1 if a bad argument was given
 */
int rotateLeft(string a[], int n, int pos){
	// Checks for bad arguments
	if (n < 0 || pos < 0){
		return -1;
	}
	// Stores the value at index pos
	string temp = a[pos];
	// Moves every element over one position unless we are at the end of the array, in which we set it to equal temp
	for (int i = pos; i < n; i++){
		if (i == n - 1){
			a[i] = temp;
		}
		else{
			a[i] = a[i + 1];
		}
	}
	return pos;
}
/*
 * Moves the element at index pos all the way to the beginning of the array and moves everything to the left of pos right one element
 * Parameters: An array of strings to be modified
 *			   An integer representing the number of elements from the beginning of array a to be modified
 *			   An integer representing the index where the change will be made
 * Return: Returns pos if everything went accordingly and -1 if a bad argument was given
 */
int rotateRight(string a[], int n, int pos){
	// Checks for bad arguments
	if (n < 0 || pos < 0){
		return -1;
	}
	// Stores the element at index pos
	string temp = a[pos];
	// Moves every element over one position unless we are at the beginning of the array, in which we set it to equal temp	
	for (int i = pos; i >= 0; i--){
		if (i == 0){
			a[i] = temp;
		}
		else{
			a[i] = a[i - 1];
		}
	}
	return pos;
}
/*
 * Flips the array. This means we set the first element to equal the last and the last to equal the first
 * Does this with all corresponding elements
 * For example, if I call flip on this array: { "Alpha", "Phi", "Omega", "LFS" }
 * After the call, it would become { "LFS", "Omega", "Phi", "Alpha" }
 * Parameters: An array to be modified
 *			    An integer representing the number of elements from the beginning of array a to be modified
 * Return: The number of elements we looked at or in other words, n. Returns -1 if given bad argument
 */
int flip(string a[], int n){
	// Checks for bad argument
	if (n < 0){
		return -1;
	}
	// Loops through half of the number of elements we want to look at. 
	// We loop through half because this way, we only flip the elements once rather than twice
	for (int i = 0; i < n/2; i++){
		// Stores the element at index i
		string temp = a[i];
		// Flips the values  
		a[i] = a[n - 1 - i];
		a[n - 1 - i] = temp;
	}
	return n;
}
/*
 * Returns the index at which the the two arrays first differ
 * If one array runs out, we return the length of the smaller array
 * Parameters: Two arrays to be compared
 *			   Two integers representing the number of elements inspected in each array
 * Return: The index in which the two arrays first differ
 *		   Or if they are both similar up until one runs out, return the size of the smaller array
 *		   Or -1 if a bad argument is given
 */
int differ(const string a1[], int n1, const string a2[], int n2){
	// Checks for bad argument
	if (n1 < 0 || n2 < 0){
		return -1;
	}
	// Initializes smaller to be the smaller value between n1 and n2
	int smaller;
	if (n1 < n2){
		smaller = n1;
	}
	else{
		smaller = n2;
	}
	// Loops until the end of the smaller array and if an element differs from the other array, we return that index
	for (int i = 0; i < smaller; i++){
		if (a1[i] != a2[i]){
			return i;
		}
	}
	// Returns smaller if get through the loop
	return smaller;
}
/*
 * Checks to see if a2 is in a1 in the same consecutive order
 * Parameters: One array representing the array we are searching for
 *			   Another array representing the array we are looking in
 *			   Two integers representing the size of their corresponding arrays
 * Return: The index in which we a2 begins in a1
 *		   If a2 is not in a1 or bad arguments are given, -1 is returned
 */
int subsequence(const string a1[], int n1, const string a2[], int n2){
	// Checks for bad arguments
	if (n1 < 0 || n2 < 0){
		return -1;
	}
	// Checks that a2 is not an empty array
	// If so, we return the appropriate value because out program below cannot handle a2 being an empty array
	if (n2 == 0){
		return 0;
	}
	// Loops through a1 to check if a2 is contained within a1
	for (int i = 0; i < n1; i++){
		// Checks to see if the element at index i of a1 is equal to the first element of a2
		if (a1[i] == a2[0]){
			// Loops through the second array to check that all the elements are in a1
			for (int k = 0; k < n2; k++){
				// Checks that we are not out of bounds and also if the elements in the corresponding indicies are equal
				if ((i + k > n1) || a1[i + k] != a2[k]){
					break;
				}
				// If we reach the end of the array, we return index i
				if (k == n2 - 1){
					return i;
				}
			}
		}
	}
	// If this point is reached, a2 is not a subsequence of a1 so we return -1
	return -1;
}
/*
 * Returns the smallest index of a1 where we found an element equal to any of those in a2
 * Parameters: One array representing the array we are looking through
 *			   One array representing the array of words we are looking for 
 *			   Two integers representing the size of their corresponding arrays
 * Return: The smallest index of a1 that contains any words in a2
 *		   If given bad arguments or no words in a2 were found in a1, return -1
 */
int lookupAny(const string a1[], int n1, const string a2[], int n2){
	// Checks for bad arguments
	if (n1 < 0 || n2 < 0){
		return -1;
	}
	// Loops through the first array
	for (int i = 0; i < n1; i++){
		// Loops through the second array
		for (int k = 0; k < n2; k++){
			// Checks to see if the element at index k of the second array is equal to the element at index i of the first array
			if (a1[i] == a2[k]){
				return i;
			}
		}
	}
	// If we get to this point, no elements in a2 were in a1
	return -1;
}
/* 
 * Seperates the array to make sure that the words less than a given word come before the words that come after the given word
 * Parameters: An array representing the array to be modified
 *			   An integer representing the number of elements starting from the beginning of a which will be inspected
 *			   A string representing the word that acts as the border
 * Return: The index in which strings in a change from being less than sperator to greater than seperator
 */
int separate(string a[], int n, string separator){
	// Checks for bad arguments
	if (n < 0){
		return -1;
	}
	// In the following code, I decided to assort the array in ascending order 
	
	bool swapped = true;
	int counter = 0;
	// We continue looping through until we see that no elements were swapped in the current iteration
	for (;;){
		// Changes swapped to false;
		swapped = false;
		// Loops through the array and swaps adajacent elements if the element on the right is less than the one on the left
		for (int j = 1; j < n; j++){
			if (a[j] < a[j - 1]){
				// Swaps the adjacent elements 
				string temp = a[j];
				a[j] = a[j - 1];
				a[j - 1] = temp;
				swapped = true;
			}
		}
		// Break out of the loop if we did not swap this round
		if (!swapped){
			break;
		}
	}
	// Counts the number of elements in which seperator is greater than
	for (int i = 0; i < n; i++){
		if (separator>a[i])
			counter++;
	}
	return counter;
}