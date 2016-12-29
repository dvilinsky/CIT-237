#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm> //for std::find
#include "InventoryItem.h"
using namespace std;

const int SIZE = 4; //size of our data-field holding array- there are 4 fields, so array is size 4

void printHelpText();
void readInput(vector<InventoryItem> &);
void getFile(fstream &, bool in = true);
void fillArray(string *, string, int);
void printItems(vector<InventoryItem> &);
void incrementQuantity(vector<InventoryItem> &);
void createNewItem(vector<InventoryItem> &);
void outputItems(vector<InventoryItem> &);
void removeItem(vector<InventoryItem> &);
int getValidNumber(vector<InventoryItem> &);
void dir();

int main() {
	string command;
	vector<InventoryItem> items; //this will be the main container that holds all the inventory item objects
	//main command loop of program
	while (true) {
		cout << "Enter a command: ";
		cin >> command;
		if (command == "a") {
			incrementQuantity(items);
		}
		else if (command == "h") {
			printHelpText();
		}
		else if (command == "i") {
			//presumably, this is where the user has to begin using this program
			readInput(items);
		}
		else if (command == "p") {
			printItems(items);
		}
		else if (command == "n") {
			createNewItem(items);
		}
		else if (command == "o") {
			outputItems(items);
		}
		else if (command == "r") {
			removeItem(items);
		}
		else if (command == "dir") {
			dir();
		}
		else if (command == "q") {
			//quit the program 
			return 0;
		}
		else {
			cout << "Invalid command. Supported commands are: \n";
			printHelpText();
		}
	}
	system("pause");
	return 0;
}

/* Function readInput() prompts a user for an input file that we assume to be in pipe-
*  delimited format. Then, it reads that file into an array of data fields, which then
*  lets us make an InventoryItem object, which we push into our main InventoryItem vector
*  If an item is already in the vector, that item's quantity gets increased
*  Post: A vector containing some InventoryItem objects
*  Parameter: A reference to the vector of InventoryItem objects
*/
void readInput(vector<InventoryItem> &items) {
	fstream inputFile;
	getFile(inputFile);
	string line;
	while (getline(inputFile, line)) {
		string data[SIZE];
		string s = line;
		string *ptr = data;
		fillArray(ptr, s, SIZE); 
		InventoryItem item(ptr[1], stod(ptr[2]), stod(ptr[3]));
		int position = find(items.begin(), items.end(), item) - items.begin();
		if (position < items.size()) {
			items[position] += stod(ptr[3]);
			continue;
		}
		items.push_back(item);
	}
	inputFile.close();
}

/* Function fillArray takes a pipe-delimited string of data fields, splits the string along those
*  pipes and places each data field into one slot in an array. This array has a very specific
*  format. A[1] is the description, A[2] is the cost, and A[3] is the item's quantity
*  Much of this code has been shamelessly copied from Project 3. I hope this doesn't count as plagiarism
*  Parameters: Pointer to an array of strings, string to be split, size of the array
*/
void fillArray(string *ptr, string s, int size) {
	int pipePos = s.find("|"); //finds the position of the first pipe
	int strIndex = 0, arrIndex = 0;
	while (strIndex < s.length()) {
		if (pipePos == string::npos) {
			pipePos = s.length(); //this ends the loop because below we will set strIndex to pipePos +1, making it greater then s.length()
		}
		string sub = s.substr(strIndex, (pipePos - strIndex)); //get substring start at strIndex until (pipePos-strIndex) chars later
		ptr[arrIndex] = sub; //add to array
		strIndex = pipePos + 1; //increase strIndex so we start looking for pipe at first char after the last pipe we found 
		pipePos = s.find("|", strIndex);
		arrIndex++;
	}
}

/* This function is a simple modularization of some boilerplate file opening techniques
*  Parameters: A reference to the file object I want to open, flag for if we're using 
*  file for input or output
*/
void getFile(fstream &file, bool in) {
	string file_name;
	bool fileOpen = false;
	do {
		cout << "Enter the file name: ";
		cin >> file_name;
		if (in) {
			file.open(file_name.c_str());
		}
		else {
			file.open(file_name.c_str(), ios::out);
		}
		if (file.fail()) {
			cout << "Error opening the input file. Please try again. \n";
		}
		else {
			fileOpen = true;
		}
	} while (!fileOpen);
}

/* Increases the number of units for a particular item. 
*  Parameter: A reference to a vector of InventoryItems
*/
void incrementQuantity(vector<InventoryItem> &items) {
	int itemNumber = getValidNumber(items);
	int partsToAdd;
	cout << "How many parts to add? ";
	cin >> partsToAdd;
	items[itemNumber] += partsToAdd;
}

/* Allows the user to create a new InventoryItem object in our "database", as long as
*  an item with that description does not already exist in the vector. 
*  Parameter: Reference to a vector of inventory items.
*/
void createNewItem(vector<InventoryItem> &items) {
	string description;
	double cost;
	int quantity;
	cout << "Enter description for the new item: ";
	cin.ignore();
	getline(cin, description);
	cout << "Enter the item's cost: "; 
	cin >> cost;
	cout << "Enter an initial quantity for the new item: ";
	cin >> quantity;
	InventoryItem item(description, cost, quantity);
	if (find(items.begin(), items.end(), item) != items.end()) {
		cout << "Error: Item already exists. Please try again.\n";
		//potential problem: An item called "pump" will be considered different from 
		//an item called "pump    ". But trimming whitespace in my operator== is a bit 
		//too much for this project I think. 
	}
	else {
		items.push_back(item);
		cout << "Announcing a new inventory item: " << items.at(items.size() - 1).getDescription() << "\n";
		cout << "We now have " << items.size() << " different inventory items in stock!\n";
	}
}

/* Decrease the quantity of an item. Negative item quantities not allowed. That wouldn't even 
*  make sense. 
*  Paramter: A vector of InventoryItems 
*/
void removeItem(vector<InventoryItem> &items) {
	int itemNumber = getValidNumber(items);
	int numToRemove;
	bool tooMany = false;
	do {
		tooMany = false; //this is awful 
		cout << "Enter number of parts you wish to remove: ";
		cin >> numToRemove;
		if (numToRemove > items.at(itemNumber).getUnits()) {
			cout << "Error: You are attempting to remove more parts than the item currently holds\n";
			tooMany = true;
		} 
	} while (tooMany);
	items[itemNumber] -= numToRemove;
}

/* This function writes our vector of InventoryItems to a file in pipe-delimited format,
*  following this convention: item number|description|cost|units \n. 
*  Post: A pipe-delimited file that this program can then read in the same as if it were
*  one of the files provided.
*  Parameter: A vector of InventoryItems.
*/
void outputItems(vector<InventoryItem> &items) {
	fstream outputFile;
	bool in = false;
	getFile(outputFile, in);
	for (int i = 0; i < items.size(); i++) {
		outputFile << i << "|" << items.at(i).getDescription() << "|" << items.at(i).getCost()
			<< "|" << items.at(i).getUnits() << "\n";
	}
	outputFile.close();
}

/* A basic index validator. Prevents user from choosing a number not within size of 
*  items. 
*  Parameter: A vector of inventory items. 
*/
int getValidNumber(vector<InventoryItem> &items) {
	int itemNumber = 0;
	bool inRange = false;
	//validate user input so they choose an item number within the vector bounds
	do {
		cout << "Choose an item number: ";
		cin >> itemNumber;
		if (itemNumber < items.size() && !(itemNumber < 0)) {
			inRange = true;
		}
		else {
			cout << "Error: Item number not in range. Please choose a number "
				<< "between 0 and " << items.size() - 1 << "\n";
		}
	} while (!inRange);
	return itemNumber;
}

void printItems(vector<InventoryItem> &items) {
	cout << left << setw(14) << "Item Num" << setw(46) << "Description"
		<< setw(10) << "Cost" << setw(8) << "Quantity\n";
	cout << left << setw(14) << "________" << setw(46) << "___________"
		<< setw(10) << "____" << setw(8) << "________\n";
	for (int i = 0; i < items.size(); i++) {
		cout << left << "     " << setw(10) << i << setw(46) << items[i].getDescription()
			<< right << setw(6) << items[i].getCost() << right << setw(10) << items[i].getUnits();
		cout << "\n";
	}
	cout << items.size() << " records.\n";
}

void printHelpText() {
	cout << "Supported commands: \n"
		<< "           a        Add parts.\n"
		<< "           h        Print this help text.\n"
		<< "           i        Input inventory data from a file.\n"
		<< "           p        Print inventory list.\n"
		<< "           n        Add new inventory item.\n"
		<< "           o        Output inventory data to a file.\n"
		<< "           r        Remove parts.\n"
		<< "           dir      list files available to read in.\n"
		<< "           q        Quit the program.\n";
}

void dir() {
	system( "dir | findstr "".txt"" ");
}