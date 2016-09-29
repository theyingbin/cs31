// Code for Project 1
// Report poll results

#include <iostream>
using namespace std;   // pp. 37-38 in Savitch book explain this line

int main()
{
	int numberSurveyed;
	int forJerry;
	int forNeel;

	cout << "How many registered voters were surveyed? ";
	cin >> numberSurveyed;
	cout << "How many of them say they will vote for Jerry? ";
	cin >> forJerry;
	cout << "How many of them say they will vote for Neel? ";
	cin >> forNeel;

	double pctJerry = forJerry / numberSurveyed * 100.0;
	double pctNeel = forNeel / numberSurveyed * 100.0;

	cout.setf(ios::fixed);   // see pp. 30-31 in Savitch book
	cout.precision(1);

	cout << endl;
	cout << pctJerry << "% say they will vote for Jerry." << endl;
	cout << pctNeel << "% say they will vote for Neel." << endl;

	if (forJerry > forNeel)
		cout << "Jerry is predicted to win the election." << endl;
	else
		cout << "Neel is predicted to win the election." << endl;
}