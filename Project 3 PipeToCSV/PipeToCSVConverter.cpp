#include <string>
#include <cstring>
#include <fstream>
#include <iostream>
using namespace std;

const int SIZE = 25; //honestly no clue how big to make it. Each entry will be one field of data, so I'm hoping that no string contains more than 25 fields

void printCommands();
void pipeToCSV(ifstream &, ofstream &);
void fillArray(string*, string, int &);
void writeOutput(string*, ofstream&, int);
void formatElements(string*, int);
void formatQuotes(int, int, string &);
void formatCommas(string &);

int main() {
	ifstream inputFile;
	ofstream outputFile;
	string input_filename;
	string output_filename;
	bool inputFileOpen = false, outputFileOpen = false;
	do {
		// Get the input filename from the user.
		cout << "Enter the input filename: ";
		cin >> input_filename;

		// Open the file.
		inputFile.open(input_filename.c_str());
		if (inputFile.fail()) {
			// Display an error message.
			cout << "Error opening the input file " << input_filename << ".\n";
		}
		else {
			inputFileOpen = true;
		}
	} while (inputFileOpen != true);
	do {
		cout << "Enter the output filename: ";
		cin >> output_filename;
		outputFile.open(output_filename.c_str());
		if (outputFile.fail()) {
			cout << "Error opening the output file " << output_filename << "\n";
		}
		else {
			outputFileOpen = true;
		}
	} while (outputFileOpen != true);

	//main command loop of program 
	string command;
	while (true) {
		cout << "Command:";
		getline(cin, command);
		if (command == "p") {
			pipeToCSV(inputFile, outputFile);
		}
		else if (command == "h") {
			printCommands();
		}
		else if (command == "q") {
			return 0; //end the program
		}
	}
	system("pause");
	return 0;
}

/* Pre: A file in pipe-delimited format
*  Post: A new file in CSV format, following the requirements given
*  Parameters: The pipe delimited input file, and the CSV output file
*  Unfortunately, this method is ultimately running in O(n^4) time. I'm not sure how to make it faster
*/

void pipeToCSV(ifstream &inputFile, ofstream &outputFile) {
	string line;
	while (getline(inputFile, line)) {
		int usefulIndex = 0;
		string data[SIZE]; //this will be an array that holds each of the data fields in one line of pipe-delimited input
						   //so each entry will be one field of data
		string *ptr = data;
		string s = line;
		fillArray(ptr, s, usefulIndex);
		formatElements(ptr, usefulIndex);
		writeOutput(ptr, outputFile, usefulIndex);
	}
	outputFile.close();
	inputFile.close();
}

/* Given an array of strings, this function prepares them to be inserted into a CSV file, following
*  2 rules: (A) If you find a quote, replace it with double quotes and enclose whole string in quotes
*  and (B): If you find a comma, enclose whole string in double quotes
*  Parameters: Pointer to the array of strings, size of the array
*/
void formatElements(string * arrPtr, int size) {
	for (int i = 0; i < size; i++) {
		string s = arrPtr[i];
		int quotePos1 = s.find("\""), quotePos2 = s.find_last_of("\""); //a bit of a premature
		//optimization so we don't have to loop over the whole string in all cases
		int commaPos = s.find(",");
		if (quotePos1 != string::npos) {
			formatQuotes(quotePos1, quotePos2, s);
		}
		if (commaPos != string::npos) {
			formatCommas(s);
		}
		arrPtr[i] = s;
	}
}

/* Implements rule (B) above
*  Parameter: Reference to string to be formatted
*/
void formatCommas(string &s) {
	s.append("\"");
	s.insert(0, "\"");
}

/* Implements rule (A) above.
*  Parameters: first and last index of quotes in the string, string to be formatted
*/
void formatQuotes(int quotePos1, int quotePos2, string &s) {
	for (int i = quotePos1; i <= quotePos2; i++) {
		if (s[i] == '\"') {
			s.replace(i, 1, "\"\"");
			i++;			//if we replace one quote with 2, we need to move i up again
			quotePos2++;	 //otherwise, the next char will also be a quote, which will be replaced
							//with 2 quotes, and so on. Similarly, we need to increment quotePos2
							//because if we add another char, string gets longer so we have to make sure we're iterating over original interval
		}
	}
	s.append("\"");
	s.insert(0, "\"");
}


/* Given a pipe-delimited string, this function splits the string along the pipes into 
*  an array of strings, with each element in the array being one "field" that was between the strings
*  in the pipes.
*  Parameters: Pointer to the array to be filled, string to be split, "length" of the array
*/
void fillArray(string *arrPtr, string s, int & usefulIndex) {
	int pipePos = s.find("|"); //get the position of the first pipe
	int strIndex = 0, arrIndex = 0;
	while (strIndex < s.length()) {
		if (pipePos == string::npos) {
			pipePos = s.length(); //this ends the loop- because when we set strIndex below, will be > s.length()
		}
		string sub = s.substr(strIndex, (pipePos - strIndex)); //get substring from index to wherever the pipe is. 
		arrPtr[arrIndex] = sub; //put that string in the array
		strIndex = (pipePos + 1); //increase string index to first position after the last place we found a pipe
		pipePos = s.find("|", strIndex); //re-search for a pipe
		usefulIndex++;
		arrIndex++;
	}
}

/* This function writes the now formatted array of strings into an output file,
*  separating each string by commas
*  Parameters: array of strings, reference to output file, size of the array
*/
void writeOutput(string *arrPtr, ofstream &outputFile, int size) {
	outputFile << arrPtr[0];
	for (int i = 1; i < size; i++) {
		outputFile << ", " << arrPtr[i];
	}
	outputFile << "\n";
}

void printCommands() {
	cout << "Supported commands:\n"
		<< "          p          convert pipe-delimited text to CSV\n"
		<< "          h          print this help text\n"
		<< "          q          quit the program";
}
