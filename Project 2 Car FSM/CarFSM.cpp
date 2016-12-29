#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

const int DELTA = 5; //acceleration rate for the "car";
const int TIME_INTERVAL = 1; //1 second time interval that we use to calculate the position of the car

const int STATE_STOPPED = 0;
const int STATE_CRUISE = 1;
const int STATE_ACCEL = 2;
const int STATE_BRAKE = 3;

const int EVENT_BRAKE = 0;
const int EVENT_ACCEL = 1;
const int EVENT_CRUISE = 2;
const int EVENT_NONE = 3;

void printCommands();
void showDemo();
void updateCar(int &, int &, double &, double &, double &);
double updatePosition(double, double, double &);
void printStatus(int, int, double, double, double);
string eventToString(int);
string stateToString(int);
void printHeader();

int main() {
	int state = STATE_STOPPED; //default state to start. 
	int event = EVENT_NONE; //default event
	double previousSpeed = 0, currentSpeed = 0, distanceTraveled = 0;
	string command;
	cout << "Hello! Welcome to the car simulation program. The supported commands are listed in the menu below\n";
	printCommands();
	printHeader();

	while (true) {
		event = EVENT_NONE; //event needs to be reset to default at every iteration- if we didn't do this, if the 
		//user hit "enter", program would run as if the user had typed their previous command
		cout << "\n Command: ";
		getline(cin, command);
		if (command == "h") {
			printCommands();
			continue;
		}
		else if (command == "d") {
			showDemo();
			continue;
		}
		else if (command == "a") {
			event = EVENT_ACCEL;
		}
		else if (command == "b") {
			event = EVENT_BRAKE;
		}
		else if (command == "c") {
			event = EVENT_CRUISE;
		}
		else if (command == "q") {
			return 0;
		}
		else {
			cout << "Invalid command. Supported commands listed below: ";
			printCommands();
			continue;
		}
		updateCar(event, state, previousSpeed, distanceTraveled, currentSpeed);
	}
	system("pause");
}

/*Gives a demo to the user of the program's functionality.*/
void showDemo() {
	int event = EVENT_CRUISE, state = STATE_STOPPED;
	double distanceTraveled = 0, previousSpeed = 0, currentSpeed = 0;
	for (int i = 0; i <= 2; i++) {
		updateCar(event, state, previousSpeed, distanceTraveled, currentSpeed);
	}
	event = EVENT_ACCEL;
	for (int i = 0; i <= 4; i++) {
		updateCar(event, state, previousSpeed, distanceTraveled, currentSpeed);
	}
	event = EVENT_CRUISE;
	for (int i = 0; i <= 4; i++) {
		updateCar(event, state, previousSpeed, distanceTraveled, currentSpeed);
	}
	event = EVENT_BRAKE;
	for (int i = 0; i <= 7; i++) {
		updateCar(event, state, previousSpeed, distanceTraveled, currentSpeed);
	}
}

/* Prints the "column" headers of the data that the program reports. Note: whether or not this looks good 
*  unfortunately depends on the size of the console window.
*/
void printHeader() {
	cout << right <<setw(20) << "Function " << right <<setw(20) << "Current State " << right <<setw(20) << "Current Speed" 
		<< right <<setw(20) << "Interval Distance " << right << setw(20) << " Total feet traveled";
}

/* This function both moves the car forward, and returns the distance traveled during that time interval
*  I think that's kind of bad, and that it should only do one of those things, but that's the best I've got
** right now
*  Parameters: Speed before the the 1s interval, speed after the interval, and the total distance traveled so far
*/
double updatePosition(double previousSpeed, double currentSpeed, double & distanceTraveled) {
	double averageSpeedFPS = ((previousSpeed + currentSpeed) / 2) * (5280.0/3600.0);
	double intervalDistance = averageSpeedFPS * TIME_INTERVAL;
	distanceTraveled += intervalDistance;
	return intervalDistance;
}

/*Using a nested switch/case block, this function finds the (event, state) ordered pair and then takes action
* depending on what the program requirements were. 
* Parameters: event, state, previous speed of the car, total distance traveled, speed after the interval
*/
void updateCar(int & event, int & state, double & previousSpeed, double & distanceTraveled, double & currentSpeed) {
	double intervalDistance = 0;
	switch (event) {
	case EVENT_NONE: 
		switch (state) {
		case STATE_STOPPED:
			break;
		case STATE_ACCEL:
			//move the car forward, increase speed by delta
			previousSpeed = currentSpeed;
			currentSpeed += DELTA;
			intervalDistance = updatePosition(previousSpeed, currentSpeed, distanceTraveled);
			break;
		case STATE_BRAKE:
			//car has to keep moving forward, while slowing down by delta, we'll deal with the nuances later
			//keep state the same UNLESS you get to the stopped state. 
			previousSpeed = currentSpeed;
			if (currentSpeed <= DELTA) {
				currentSpeed = 0;
				state = STATE_STOPPED;
			}
			else {
				currentSpeed -= DELTA;
			}
			intervalDistance = updatePosition(previousSpeed, currentSpeed, distanceTraveled);
			break;
		case STATE_CRUISE:
			//continue to move car forward at a constant rate
			intervalDistance = updatePosition(previousSpeed, currentSpeed, distanceTraveled);
			break;
		}
		break;
	case EVENT_CRUISE:
		switch (state) {
		case STATE_STOPPED:
			//do nothing- cruise is essentially a "continue as you were state." So if you're stopped, you stay stopped
			break;
		case STATE_ACCEL: case STATE_BRAKE:
			//stop accelerating/brake, so keep speed as is
			//update position of the car
			//change state to the cruising state
			previousSpeed = currentSpeed;
			intervalDistance = updatePosition(previousSpeed, currentSpeed, distanceTraveled);
			state = STATE_CRUISE;
			break;
		case STATE_CRUISE:
			//move car forward, update position, that's it
			previousSpeed = currentSpeed;
			intervalDistance = updatePosition(previousSpeed, currentSpeed, distanceTraveled);			
			break;
		}
		break;
	case EVENT_ACCEL:
		switch (state) {
		case STATE_STOPPED:
			//increase speed by delta
			//start moving the car
			//enter the accelerating state
			previousSpeed = 0;
			currentSpeed += DELTA;
			state = STATE_ACCEL;
			break;
		case STATE_ACCEL:
			//increase speed by delta
			//move car forward
			previousSpeed = currentSpeed;
			currentSpeed += DELTA;
			break;
		case STATE_BRAKE: case STATE_CRUISE:
			//stop braking/crusing- as it turns out, what to do in the braking/cruising state is the same 
			//increase speed by delta, move car forward
			//enter the accelerating state
			previousSpeed = currentSpeed;
			currentSpeed += DELTA;
			state = STATE_ACCEL;
			break;
		}
		intervalDistance = updatePosition(previousSpeed, currentSpeed, distanceTraveled);
		break;
	case EVENT_BRAKE:
		switch (state) {
		case STATE_STOPPED:
			//do nothing;
			break;
		case STATE_ACCEL: case STATE_BRAKE: case STATE_CRUISE:
			//decresase speed by 5, enter the braking state
			previousSpeed = currentSpeed;
			if (currentSpeed <= DELTA) {
				currentSpeed = 0;
				state = STATE_STOPPED;
			}
			else {
				currentSpeed -= DELTA;
				state = STATE_BRAKE;
			}
			intervalDistance = updatePosition(previousSpeed, currentSpeed, distanceTraveled);
			break;
		}
		break;
	}
	printStatus(event, state, currentSpeed, intervalDistance, distanceTraveled);
}

void printStatus(int event, int state, double currentSpeed, double intervalDistance, double distanceTraveled) {
	cout << setw(20) << right << eventToString(event) << setw(20) << right << stateToString(state) << setw(20)
		<< right << currentSpeed << right << setw(20) << intervalDistance << right << setw(20) << distanceTraveled;
	
}

string stateToString(int state) {
	if (state == STATE_ACCEL) {
		return "Accelerating";
	}
	else if (state == STATE_BRAKE) {
		return "Braking";
	}
	else if (state == STATE_CRUISE) {
		return "Cruising";
	}
	else {
		return "Stopped";
	}
}

string eventToString(int event) {
	if (event == EVENT_ACCEL) {
		return "Accelerate";
	}
	else if (event == EVENT_BRAKE) {
		return "Brake";
	}
	else if (event == EVENT_CRUISE) {
		return "Cruise";
	}
	else {
		return "Stop";
	}
}

void printCommands() {
	cout << "Supported Commands:\n"
		<< "                 a          Accelerate\n"
		<< "                 b          Brake\n"
		<< "                 c          Cruise\n"
		<< "                 d          Show the demo\n"
		<< "                 h          Print this help text\n"
		<< "                 q          Quit and end the program\n";
} 