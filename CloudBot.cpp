// CloudBot.cpp
// Main program for the cloud bot application
// Author: Anbumani Manikantan
// Creation Date: 3-Feb-2016
// Modification History
// -----------------------------------------------
// Version  | Date       | Changes
// 1.0      | 03Feb2016  | Initial version
// -----------------------------------------------


#include <iostream>
#include "Robot.h"
#include "LabyPath.h"

using namespace std;

int main(){
	// Declare local variables
	WayPossibility front,side=PSBL;
	WayPossibility& fnt = front;
	WayPossibility& sde = side;

	int row,col;
	bool ext = true;

	string line;

	// Create Robot object
	Robot Bot(RIGHT);
	
	cout << "!!! Robot started to MOVE !!!" <<endl;
	// Move the robot
	Bot.move();

	//Bot.getPoint().print();
	
	
	cout << endl<< WayPsblStr2(side) << "  End of cludBot application !!!" <<endl;
	cout << endl<< "  End of cludBot application !!!" <<endl;
	
	// return 
	return 0;
}




/* TBD:
1. Camera side to be initialized for LabySimu object
2. Where to keep the Robot movement logic? inside the Robot Class or outside?
*/
