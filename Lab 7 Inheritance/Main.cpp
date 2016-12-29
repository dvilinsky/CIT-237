#include <iostream>
#include <string>
#include "Employee.h"
#include "ProductionWorker.h"
using namespace std;

ProductionWorker* createWorker();
void printWorker(ProductionWorker*);
void printHelpText();

int main() {
	string command;
	ProductionWorker *worker = nullptr;
	while (true) {
		cout << "Enter a command: ";
		cin >> command;
		if (command == "c") {
			 worker = createWorker();
		}
		else if (command == "h") {
			//print help text
			printHelpText();
		}
		else if (command == "p") {
			//print production worker information
			printWorker(worker);
		}
		else if (command == "q") {
			return 0; //end the programg
		}
		else {
			cout << "Error invalid command\n";
			printHelpText();
		}
	}
	system("pause");
}

ProductionWorker* createWorker() {
	bool exceptionWasThrown = false;
	ProductionWorker *w = nullptr;
	do
	{
		int shift;
		double payRate;
		string name, hireDate;
		cout << "Enter shift for employee- \"1\" for day, \"2\" for night: ";
		cin >> shift;
		cout << "Enter employee's payrate, typing only numbers in decimal form: ";
		cin >> payRate;
		cin.ignore(80, '\n');
		cout << "Enter the employee's name: ";
		getline(cin, name);
		cout << "Enter the employee's hire date: ";
		getline(cin, hireDate);
		try
		{
			//need to test shift, hire date, and pay rate before creating employee
			//how?????
			w = new ProductionWorker(shift, payRate, name, hireDate);
			exceptionWasThrown = false;
		}
		catch (Employee::InvalidHireDateEmployeeNumber e) {
			cout << "Error: " << e.getDate() << " is an invalid date."
				<< " Please enter a date of the form MM/DD/YYYY \n";
			exceptionWasThrown = true;
		}
		catch (ProductionWorker::InvalidShift e) {
			cout << "Error: " << e.getShift() << " is an invalid shift. Shift number must be eithe 1 or 2"
				<< " Please try again.\n";
			exceptionWasThrown = true;
		}
		catch (ProductionWorker::InvalidPayRate e) {
			cout << "Error: " << e.getPayRate() << " is an invalid pay rate. "
				<< "Please enter a non-negative number of the form XX.XX\n";
			exceptionWasThrown = true;
		}
	} while (exceptionWasThrown);
	return w;
}

void printWorker(ProductionWorker *worker) {
	if (worker != nullptr) {
		cout << "Name: " << worker->getEmployeeName() << "\n";
		cout << "Hire Date: " << worker->getHireDate() << "\n";
		cout << "Shift: " << worker->getShift() << "\n";
		cout << "Employee Number: " << worker->getLastEmployeeNumberIssued() << "\n";
		cout << "Pay Rate: " << worker->getPayRate() << "\n\n";
	}
	else {
		cout << "No production worker to print.\n";
	}
}

void printHelpText() {
	cout << "Supported Commands: \n"
		<< "         c        create a new ProdcutionWorker object\n"
		<< "         h        print this help text\n"
		<< "         p        print ProductionWorker information\n"
		<< "         q        quit the program\n";
}