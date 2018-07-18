// Robot.cpp
// Author: Anbumani Manikantan
// Creation Date: 4-Feb-2016
// Modification History
// -----------------------------------------------
// Version  | Date       | Changes
// 1.0      | 04Feb2016  | Initial version
// -----------------------------------------------


#include <iostream>
#include "Robot.h"

using namespace std;

//-----------------------------------------------------------------------------
// method to implement the movement of the Robot
//-----------------------------------------------------------------------------
void Robot::move() {
	// Declare local variables
	WayPossibility front,side;
	WayPossibility& fnt = front;
	WayPossibility& sde = side;

	// variable to indicate if deadend has reached
	bool isDeadEnd = false;

	startPath = new LabyPath();
	currentPath = startPath;

	// continue the loop for 100 steps
	while(stepCnt < 500) {
		// Check the current possibility by reading laby simulation
		labySimu.getPsblWay(pos->getPoint(),ori,fnt,sde);

		cout << "Current step is " << stepCnt << endl;

		// Update position info
		updatePointPossibility(fnt,sde);

		// Check if exit is present in front or side direction
		if(fnt == EXIT && (stepCnt != 0)) {
			cout << "~~~ Exit found in the front direction ~~~"<<endl;
			break;
		}

		if(sde == EXIT && (stepCnt != 0)) {
			cout << "~~~ Exit found in the side direction ~~~"<<endl;
			break;
		}

		// Check if movement is possible
		// Req1. Check if front direction is possible and unexplored, then take the move
		if((pos->getWayPsbl(ori) == PSBL) && !pos->getWayExplored(ori)) {
			// Set the explored state on the current direction
			pos->setWayExplored(ori);

			// Add this point to the current path
			pos->print();
			currentPath->addPoint(pos);

			// move one step in the front direction
			stepForward();

			// continue
			continue;
		}

		// Req2. Check if camera side direction is possible and unexplored, then take the move
		if((pos->getWayPsbl(sideDir()) == PSBL) && !pos->getWayExplored(sideDir())) {
			// Change the orientation of the Robot to side
			ori = sideDir();

			cout << endl << " <<< towards " << OrientStr(ori) << "  >>>"<< endl;
			cin.get();

			// Get the camera output
			labySimu.getPsblWay(pos->getPoint(),ori,fnt,sde);

			// Update position info
			updatePointPossibility(fnt,sde);

			// Set the explored state on the current direction
			pos->setWayExplored(ori);

			// Add this point to the current path
			pos->print();
			currentPath->addPoint(pos);

			// move one step in the front direction
			stepForward();

			// continue
			continue;
		}

		// Get the possibility of opposite direction by turning the Robot
		ori = oppSideDir();
		cout << endl << " <<< towards " << OrientStr(ori) << "  >>>"<< endl;
		cin.get();

		// Get the camera output
		labySimu.getPsblWay(pos->getPoint(),ori,fnt,sde);

		// Update position info
		updatePointPossibility(fnt,sde);

		pos->print();

		// Req3. Check if opposite to camera side direction is possible and unexplored, then take the move
		if((pos->getWayPsbl(ori) == PSBL) && !pos->getWayExplored(ori)) {
			// Set the explored state on the current direction
			pos->setWayExplored(ori);

			// Add this point to the current path
			pos->print();
			currentPath->addPoint(pos);

			// move one step in the front direction
			stepForward();

			// continue
			continue;
		}

		// Dead end has reached
		cout << endl << "!!! Dead End reached !!! " <<endl;
		currentPath->addPoint(pos);
		//currentPath->print();
		isDeadEnd = true;

		// Set the orientation to backward direction
		ori = pos->getLastDir();

		// Move back in the current path
		moveBack();


	}

}


//-----------------------------------------------------------------------------
// method to implement the movement of the Robot in backward direction from a deadend
// 1. Go to previous point
// 2. Get other two directions which are not explored
// 3. Read possibility and explored information on those directions
// 4. if both the directions are possible and not explored, TBD
// 5. if no direction is possible, goto step 1
// 6. if one direction is possible
// 7. make the current point as new junction
//    set the previous path to the cuurent path
// 8. create a LabyPath called termPath, starts from next point till end of the dead end
//    set the type of the path to TERMINATED
//    set the end point from the current path
//    set the previous junction to the new juncion
//    set P1 of the new junction to this path
// 9. set the end point of the current path to the previous point of the current point
//    set the next pointer of the end point as NULL
//    set the next junction of the current path to the newly created junction
//10. Create a new current path with start point as newly identified point
//    set the previous junction of this path to newly created junction
//-----------------------------------------------------------------------------
void Robot::moveBack() {
	// Local variable to store the possible values in the front and side direction
	WayPossibility front,side;

/*	// the two directions that are already explored
	Orientation dir1, dir2;

	// Temporary point
	LabyPoint *tmpPos;

	// New directions to be explored
	Orientation nDir1, nDir2;
*/
	cout << "<<<< Moving in backward direction !!!" << endl;

	// Move in backward direction until a new junction is found
	while(true) {
		// the two directions that are already explored
		Orientation dir1, dir2;
	
		// Temporary point
		LabyPoint *tmpPos;
	
		// New directions to be explored
		Orientation nDir1, nDir2;

		// Orient the Robot to last direction
		ori = pos->getLastDir();

		// Opposite direction of the current point is the direction not to be explored in the previous point
		dir1 = oppDir(pos->getLastDir());

		// Move to previous point
		tmpPos = pos;
		cout << "->temp position";
		tmpPos->print();
		pos = currentPath->getPrevPoint();
		cout << "->prev position";
		pos->print();
		
		// if it reached the begining of the path, then go to previous path and start finding new path to explore
		if (tmpPos == pos) {
			cout << "!!! Going back to previous path in search of new path !!!" << endl;
			currentPath = currentPath->getPrevPath();
			currentPath->print();
			pos = currentPath->getPrevPoint();
			pos->getPrevPoint()->print();
			continue;
		}

		// Orient the Robot to last direction
		ori = pos->getLastDir();

		// Antoher direction not to be explored is last direction of the previous point
		dir2 = ori;

		// Determine the other two directions to be explored
		getRemainingDir(dir1,dir2,nDir1,nDir2);

		// Read the possibility info on both directions
		if(pos->getWayPsbl(nDir1) == UNKNOWN) {
			// Orient the robot to this direction to read the possibility value
			ori = nDir1;
			labySimu.getPsblWay(pos->getPoint(),ori,front,side);
			updatePointPossibility(front,side);
		}

		if(pos->getWayPsbl(nDir2) == UNKNOWN) {
			// Orient the robot to this direction to read the possibility value
			ori = nDir2;
			labySimu.getPsblWay(pos->getPoint(),ori,front,side);
			updatePointPossibility(front,side);
		}

		pos->print();

		// If both directions are possible and not explored, TBD
		if((pos->getWayPsbl(nDir1) == PSBL) && !pos->getWayExplored(nDir1)) {
			// If both directions are possible and not explored, TBD
			if((pos->getWayPsbl(nDir2) == PSBL) && !pos->getWayExplored(nDir2)) {
				cout << "Both the directions are possible for this point " << endl ;
				pos->print();
			}
			// Create new junction in this point
			createNewJunction(nDir1);
			break;
		} else if((pos->getWayPsbl(nDir2) == PSBL) && !pos->getWayExplored(nDir2)) {
			// Create new junction in this point
			createNewJunction(nDir2);
			break;
		}
	}
}


//-----------------------------------------------------------------------------
// A private method to get remaining two directions
//-----------------------------------------------------------------------------
void Robot::createNewJunction(Orientation orien) {
	// Create local variable to store the previous path
	LabyPath *prePath = currentPath;

	cout <<endl;
	pos->printCoord();
	cout << "XXXXX New Junction is created !!!" << endl;

	// Make the current point as a new Junction
	Junction *newJun = new Junction(pos);

	// Create the terminated path
	LabyPath *termPath = new LabyPath(pos->getNextPoint());

	// Set the type of this path to TERMINATED
	termPath->setType(TERMINATED);

	// Set the previous junction of this path (by default next junction is set to NULL)
	termPath->setPrevJunction(newJun);

	// Set the previous path
	termPath->setPrevPath(prePath);

	// Connect the terminated path to the next junction
	newJun->setP1(termPath);

	// Set the next junction of the current path to new junction
	currentPath->setNextJunction(newJun);

	// Set the end point of the current path to previous point of the current position
	currentPath->setEndPoint(pos->getPrevPoint());

	// Move forward in the new unexplored direction
	ori = orien;
	stepForward();

	cout << "The current path before creating junction:" <<endl;
	currentPath->print();

	// Set the previous path of the junction to the current path
	newJun->setPreviousPath(currentPath);

	// Create a new current path to start the journey again
	currentPath = new LabyPath(pos);

	// Set the previous path for this new current path
	currentPath->setPrevPath(prePath);

	cout << "\n Term and current paths after creating the new junction:\n";
	
	cout << "==> Terminated Path"<<endl;
	termPath->print();
	cout << "==> Current Path"<<endl;
	currentPath->print();

}

//-----------------------------------------------------------------------------
// A private method to get remaining two directions
// Input : dir1 and dir2
// Output: nD1 and nD2
//-----------------------------------------------------------------------------
void Robot::getRemainingDir(Orientation dir1, Orientation dir2, Orientation& nD1, Orientation& nD2) {

	switch(dir1) {
		case EAST:
			switch(dir2) {
				case WEST:
					nD1 = SOUTH;
					nD2 = NORTH;
					break;
				case NORTH:
					nD1 = SOUTH;
					nD2 = WEST;
					break;
				case SOUTH:
					nD1 = WEST;
					nD2 = NORTH;
					break;
			}
			break;
		case WEST:
			switch(dir2) {
				case EAST:
					nD1 = SOUTH;
					nD2 = NORTH;
					break;
				case NORTH:
					nD1 = SOUTH;
					nD2 = EAST;
					break;
				case SOUTH:
					nD1 = EAST;
					nD2 = NORTH;
					break;
			}
			break;
		case NORTH:
			switch(dir2) {
				case WEST:
					nD1 = SOUTH;
					nD2 = EAST;
					break;
				case EAST:
					nD1 = SOUTH;
					nD2 = WEST;
					break;
				case SOUTH:
					nD1 = WEST;
					nD2 = EAST;
					break;
			}
			break;
		case SOUTH:
			switch(dir2) {
				case WEST:
					nD1 = EAST;
					nD2 = NORTH;
					break;
				case NORTH:
					nD1 = EAST;
					nD2 = WEST;
					break;
				case EAST:
					nD1 = WEST;
					nD2 = NORTH;
					break;
			}
			break;
	}
	//cout << "getRemainingDir (" << dir1 << "," << dir2 << "," << nD1 << "," << nD2 << ",)" << endl;
}

//-----------------------------------------------------------------------------
// A method to implement the one step movement of the Robot in forward direction
//-----------------------------------------------------------------------------
void Robot::stepForward() {

	// Get the current Point
	Point curP = pos->getPoint();

	// Determine the new coordinate based on current orientation and position
	switch(ori) {
		case NORTH:
			curP.y++;
			break;
		case SOUTH:
			curP.y--;
			break;
		case EAST:
			curP.x++;
			break;
		case WEST:
			curP.x--;
			break;
	}

	// update pos data member based on the current orientation
	LabyPoint *newPos = new LabyPoint(curP,oppDir(ori));

	pos = newPos;

	// Set the possibility and explored state of last direction
	pos->setWayPsbl(oppDir(ori),PSBL);
	pos->setWayExplored(oppDir(ori));

	// Increment the step count and set it to the point
	stepCnt++;
	pos->setStepId(stepCnt);
}

//-----------------------------------------------------------------------------
// A method to find short path from the robot travel
//-----------------------------------------------------------------------------
void Robot::shortPath() {
	cout <<"\n\n==> Method to find short path!!!"<<endl;

	// print the start path
	startPath->print();

	// Get the next junction of this path and connected paths
	Junction* curJx = startPath->getNextJunction();
	//curJx->display();
	LabyPath* curP1 = curJx->getP1();
	LabyPath* curP2 = curJx->getP2();
	LabyPath* curP3 = curJx->getP3();

	// Loop until end of the maze
	while(1) {
		// Check the type of the path P1
		if((curP1 != NULL) && (curP1->getType() == CONNECTED)) {
			cout << "Continue in another path P1!!!"<<endl;
			curP1->print();

			// if P1 is connected, then travel to next junction at the end of this path
			curJx = curP1->getNextJunction();
			curP1 = curJx->getP1();
			curP2 = curJx->getP2();
			curP3 = curJx->getP3();


			// Continue the search
			continue;
		}

		// Check the type of the path P2
		if((curP2 != NULL) && (curP2->getType() == CONNECTED)) {
			cout << "Continue in another path P2 !!!"<<endl;
			curP2->print();

			// if P2 is connected, then travel to next junction at the end of this path
			curJx = curP2->getNextJunction();
			curP1 = curJx->getP1();
			curP2 = curJx->getP2();
			curP3 = curJx->getP3();

			// Continue the search
			continue;
		}
		// Check the type of the path P3
		if((curP3 != NULL) && (curP3->getType() == CONNECTED)) {
			cout << "Continue in another path P3 !!!"<<endl;
			curP3->print();

			// if P3 is connected, then travel to next junction at the end of this path
			curJx = curP3->getNextJunction();
			curP1 = curJx->getP1();
			curP2 = curJx->getP2();
			curP3 = curJx->getP3();

			// Continue the search
			continue;
		}

		// break the loop as no connected path is present
		cout <<"\n!!!breaking the loop as no connected path is present "<<endl;
		break;
	}


}



/* TBD
1. Set the camera side of the Robot
2. Robot(int xa=0, int ya=0, T_CamDir camS)
50	3	F:\AER_TLS_CloudBot_ParcoursCollaboratif\WIP\Robot.cpp	[Error] default argument missing for parameter 3 of 'Robot::Robot(int, int, T_CamDir)'
3. Finding 2 unexplored directions in easier way
*/
