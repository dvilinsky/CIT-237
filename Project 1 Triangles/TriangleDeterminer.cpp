#include <iostream>
using namespace std;

void promptUser();
bool isTriangle(double, double, double);
void printResults(double, double, double);

int main() {
	cout << "Name:                Daniel Vilinsky\n";
	cout << "Course / Project:    CIT 237  Project: 1\n";
	cout << "Program Description: This program inputs 3 numbers, and determines if those numbers can form the sides of a triangle\n";
	//The four lines of code that follow exist to "prime the pump" of the while loop. I'm not sure if there's a bettter way to do this
	//i.e. to keep everything in the while loop.
	promptUser();
	char keepPrompting;
	cout << "Try again (enter y or n)? ";
	cin >> keepPrompting;
	while (keepPrompting == 'y') {
		promptUser();
		cout << "Try again (enter y or n)?: ";
		cin >> keepPrompting;
	}
	system("pause");
}
/* This method prompts the user for each side of the triangle, one at a time, and then passes those results to the printResults()
*  method. Some input validation might be nice, but unfortunately there's none here.
*/
void promptUser() {
	double sideA, sideB, sideC;
	cout << "Enter side A: ";
	cin >> sideA;
	cout << "Enter side B: ";
	cin >> sideB;
	cout << "Enter side C: ";
	cin >> sideC;
	printResults(sideA, sideB, sideC);
}

/* This method calls the isTriangle() function, description below, and then prints the results to the console
*  Parameters: sideA, sideB, sideC, which are the sides of the triangle
*/
void printResults(double sideA, double sideB, double sideC) {
	if (isTriangle(sideA, sideB, sideC)) {
		cout << "A triangle can be formed with these sides " << sideA << ", " << sideB << ", " << sideC << "\n";
	}
	else {
		cout << "A triangle cannot be formed with these sides " << sideA << ", " << sideB << ", " << sideC << "\n";
	}
}

/*A triangle is determined by its sides. Three sides can compose a triangle if and only if the length of the third side 
* is not greater than the lengths of the other two sides combined. 
* Parameters: sideA, sideB, sideC, the side lengths of the triangle
* Returns: whether the three sides could compose a triangle
*/
bool isTriangle(double sideA, double sideB, double sideC) {
	return ((sideA + sideB) > sideC) && ((sideA + sideC) > sideB) && ((sideB + sideC) > sideA);
}