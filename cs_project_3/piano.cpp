#include <iostream>
#include <string>
#include <cctype>

using namespace std;

bool isTuneWellFormed(string tune);
int translateTune(string tune, string& instructions, int& badBeat);
bool isNoteLetter(char tune);
int translateOctave(char c);
char translateNote(int octave, char noteLetter, char accidentalSign);

int main()
{
	
}

/*
 * Checks to see that the string given to isTuneWellFormed is a valid tune
 * Takes in a string as input representing the tune we are to test
 * Returns a boolean representing whether or not the string is a valid tune
 */
bool isTuneWellFormed(string tune)
{
	// Checks for an empty string. Also allows so that the rest of the code does not have an out of bounds error
	if (tune.size() == 0)
	{
		return true;
	}
	// Checks to see that the first character is a valid cahracter
	if ( !isNoteLetter(tune[0]) && tune[0] != '/' || tune[0] == 'b')
	{
		return false;
	}

	// Checks the body of the string to see that each individual character is valid
	for (int i = 1; i < tune.size(); i++)
	{
		// Checks to see that the character before '#' and 'b' characters are Alphabet characters
		if (tune[i] == '#' || tune[i] == 'b')
		{
			if (!isNoteLetter(tune[i-1]))
			{
				return false;
			}
		}
		// Checks that the character before a digit is either an accidental sign or a note letter
		else if (isdigit(tune[i]))
		{
			if (tune[i-1] != '#' && tune[i-1] != 'b' && !isNoteLetter(tune[i-1]))
			{
				return false;
			}
		}
		// Checks to see that any character other than accidental signs and digits are either note letters or slashes
		else if (!isNoteLetter(tune[i]) && tune[i] != '/')
		{
			return false;
		}
	}

	// Checks to see that the last character in the string tune is a slash
	if (tune[tune.size() - 1] != '/'){
		return false;
	}
	// If all the above conditions are true, we return true; 
	return true;
}

/*
 * Checks that the character given to the function is a valid note character
 * Takes in a character as input
 * Returns a boolean as to whether or not the character is a valid note character
 */
bool isNoteLetter(char noteLetter)
{
	// Returns true if the character is between A-G and false otherwise
	if (noteLetter >= 'A' && noteLetter <= 'G')
	{
		return true;
	}
	else
	{
		return false;
	}
}
/*
 * If the tune is not well formed, we will only return 1
 * If tune is playable, we set instructions to be the translated tune
 * If the tune is unplayable, we will change badBeat to the beat which we found the bad beat and return 2
 */
int translateTune(string tune, string& instructions, int& badBeat)
{
	// Checks to see that the tune is well formed
	if (!isTuneWellFormed(tune))
	{
		return 1;
	}
	else
	{
		int beatCounter = 1;
		string beat = "";
		string total = "";
		int lastIndex = 0;
		bool allSlash = false;
		// Deals with the empty string case
		if (tune.size() == 0){
			instructions = "";
			return 0;
		}
		// Checks that the first character is a slash. This is primarily for the case in which two slashes translates into two spaces
		if (tune[0] == '/'){
			allSlash = true;
		}
		// Loops through tune to check each individual character
		for (int i = 1; i < tune.size(); i++){
			// If any character is not a slash, we set allSlash to false;
			if (tune[i] != '/'){
				allSlash = false;
			}
			// Checks that the character starts a beat
			if (tune[i] == '/' || isNoteLetter(tune[i])){
				// Checks that the previous character is a slash so we know if we can add beat to the cumulating beats, total
				if (tune[i-1] == '/'){
					beatCounter++;
					if (beat.size()>1){
						total += ("[" + beat + "]");
						beat = "";
					}
					else{
						total += beat;
						beat = "";
					}
				}
				// Gets the size of the current beat
				int beatSize = i - lastIndex;

				if (beatSize == 1){
					// Checks for double slashes. if no double slashes are found, we move onto the next beat
					if (tune[i - 1] == '/')
					{
						if (tune[i] == '/'){
							total += ' ';
						}
						lastIndex = i;
					}
					// Checks to make sure that the beat is playable
					else if (translateNote(4, tune[i - 1], ' ') == ' '){
						badBeat = beatCounter;
						return 2;
					}
					// Translates the beat
					else{
						beat += translateNote(4, tune[i - 1], ' ');
						lastIndex = i;
					}
				}
				else if (beatSize == 2){
					// Checks to make sure the beat is playable
					if (tune[i - 1] != '#' && tune[i - 1] != 'b'){
						if (translateNote(translateOctave(tune[i - 1]), tune[i - 2], ' ') == ' '){
							badBeat = beatCounter;
							return 2;
						}
						// Translates the beat
						else{
							beat += translateNote(translateOctave(tune[i - 1]), tune[i - 2], ' ');
							lastIndex = i;
						}
					}
					else{
						if (translateNote(4, tune[i - 2], tune[i-1]) == ' '){
							badBeat = beatCounter;
							return 2;
						}
						// Translates the beat
						else{
							beat += translateNote(4, tune[i - 2], tune[i-1]);
							lastIndex = i;
						}
					}
				}
				else{
					// Checks to make sure the beat is playable
					if (translateNote(translateOctave(tune[i - 1]), tune[i - 3], tune[i - 2]) == ' '){
						badBeat = beatCounter;
						return 2;
					}
					// Translates the beat
					else{
						beat += translateNote(translateOctave(tune[i - 1]), tune[i - 3], tune[i-2]);
						lastIndex = i;
					}
				}
			}
		}
		// Checks the last character of the string to make sure it ends a beat and adds beat to total
		if (tune[tune.size()-1] == '/'){
			if (beat.size()>1){
				total += ("[" + beat + "]");
				beat = "";
			}
			else{
				total += beat;
				beat = "";
			}
		}
		// If tune is all slashes, we will add an additional space to it
		if (allSlash){
			total += ' ';
		}
		// Changes total since we now know that tune is playable
		instructions = total;
		return 0;
	}
}

/*
 * Translates number characters into the actual number (i.e. takes '6' as input and returns 6)
 * Takes in a character representing a number
 * Returns the number corresponding to that character
 */
int translateOctave(char c)
{
	if (c>'0' && c <= '9'){
		return c - '0';
	}
	else{
		return -1;
	}
}
// Given an octave number, a note letter, and an accidental sign, return
// the character that the indicated note translates to if it is playable.
// Return a space character if it is not playable.
//
// First parameter:   the octave number (the integer 4 is the number of the
//                    octave that starts with middle C, for example).
// Second parameter:  an upper case note letter, 'A' through 'G'
// Third parameter:   '#', 'b', or ' ' (meaning no accidental sign)
//
// Examples:  translateNote(4, 'A', ' ') returns 'Q'
//            translateNote(4, 'A', '#') returns '%'
//            translateNote(4, 'H', ' ') returns ' '

char translateNote(int octave, char noteLetter, char accidentalSign)
{
	// This check is here solely to report a common CS 31 student error.
	if (octave > 9)
	{
		cerr << "********** translateNote was called with first argument = "
			<< octave << endl;
	}

	// Convert Cb, C, C#/Db, D, D#/Eb, ..., B, B#
	//      to -1, 0,   1,   2,   3, ...,  11, 12

	int note;
	switch (noteLetter)
	{
	case 'C':  note = 0; break;
	case 'D':  note = 2; break;
	case 'E':  note = 4; break;
	case 'F':  note = 5; break;
	case 'G':  note = 7; break;
	case 'A':  note = 9; break;
	case 'B':  note = 11; break;
	default:   return ' ';
	}
	switch (accidentalSign)
	{
	case '#':  note++; break;
	case 'b':  note--; break;
	case ' ':  break;
	default:   return ' ';
	}

	// Convert ..., A#1, B1, C2, C#2, D2, ... to
	//         ..., -2,  -1, 0,   1,  2, ...

	int sequenceNumber = 12 * (octave - 2) + note;

	string keymap = "Z1X2CV3B4N5M,6.7/A8S9D0FG!H@JK#L$Q%WE^R&TY*U(I)OP";
	if (sequenceNumber < 0 || sequenceNumber >= keymap.size())
		return ' ';
	return keymap[sequenceNumber];
}