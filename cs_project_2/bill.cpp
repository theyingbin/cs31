#include <iostream>
#include <string>
using namespace std;

int main(){

	//gets the number of minutes used and stores in a variable
	int minutesUsed;
	cout << "Minutes used: ";
	cin >> minutesUsed;
	//checks if minutesUsed is a valid number
	if (minutesUsed < 0){
		cout << "The number of minutes used must be nonnegative.";
		return 1;
	}

	//gets the number of text messages and stores in a variable
	int textMessages;
	cout << "Text messages: ";
	cin >> textMessages;
	cin.ignore(10000, '\n');
	//checks if textMessages is a valid number
	if (textMessages < 0){
		cout << "The number of text messages must be nonnegative.";
		return 1;
	}

	//gets customer's name and stores in a variable
	string customerName;
	cout << "Customer name: ";
	getline(cin, customerName);
	//makes sure customerName isn't an empty string
	if (customerName.length() == 0){
		cout << "You must enter a customer name.";
		return 1;
	}

	//gets month and stores in a variable
	int monthNumber;
	cout << "Month number (1=Jan, 2=Feb, etc.): ";
	cin >> monthNumber;
	//checks if monthNumber is a valid number
	if ( ( monthNumber + 11 ) / 12 != 1 ){
		cout << "The month number must be in the range 1 through 12.";
		return 1;
	}

	cout << "---" << endl;

	double bill = 40.00;
	cout.setf(ios::fixed);
	cout.precision(2);

	//adds to the bill if minutesUsed exceeds 500
	if (minutesUsed > 500){
		bill += (minutesUsed - 500) * 0.45;
	}

	if (textMessages > 200){
		double textRate;
		if (monthNumber == 6 || monthNumber == 7 || monthNumber == 8 || monthNumber == 9){
			textRate = 0.02;
		}
		else{
			textRate = 0.03;
		}
		if (textMessages > 400){
			bill += (textMessages - 400) * 0.11;
			bill += (200 * textRate);
		}
		else{
			bill += (textMessages - 200) * textRate;
		}
	}

	cout << "The bill for " << customerName << " is $" << bill;
}