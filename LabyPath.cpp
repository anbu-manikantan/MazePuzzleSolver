// LabyPath.cpp
// Author: Anbumani Manikantan
// Creation Date: 3-Feb-2016
// Modification History
// -----------------------------------------------
// Version  | Date       | Changes
// 1.0      | 03Feb2016  | Initial version
// -----------------------------------------------

#include "LabyPath.h"

using namespace std;

string LabyPoint::getPsblStr(WayPossibility psb) const {
	switch(psb) {
		case UNKNOWN :
			return "UNKNOWN";
		case PSBL    :
			return "POSSIBLE";
		case NOT_PSBL:
			return "NOT POSSIBLE";
		case EXIT:
			return "EXIT";
	}
}

LabyPoint::LabyPoint(Point pnt, Orientation ldir):next(NULL),
												  previous(NULL),
												  stepId(0) {
	// set data of the class
	setPoint(pnt,ldir);
}

void LabyPoint::setStepId(int id) {
	if (stepId != 0) {
		cout << "!!! ERROR: StepId for this point (" <<p.x <<","<<p.y<<") is already set !!!"<< endl;
	} else
		stepId = id;
}

// method to set data members of the class
void LabyPoint::setPoint(Point newP, Orientation ldir) {
	// set the cartesian coordinates with the value passed to the constructor
	p = newP;

	// set the orientation of the previous point
	LastDir = ldir;

	// set previous and next point to null
	next = NULL;
	previous = NULL;
}

// method to get the possiblity of the way
WayPossibility LabyPoint::getWayPsbl(Orientation ori) const {
	switch(ori) {
		case EAST:
			return East.getPossibility();
		case WEST:
			return West.getPossibility();
		case NORTH:
			return North.getPossibility();
		case SOUTH:
			return South.getPossibility();
	}
}

// method to get the explored state of the way
bool LabyPoint::getWayExplored(Orientation ori) const {
	switch(ori) {
		case EAST:
			return East.getExplored();
		case WEST:
			return West.getExplored();
		case NORTH:
			return North.getExplored();
		case SOUTH:
			return South.getExplored();
	}
}

// method to set the way value
void LabyPoint::setWayPsbl(Orientation ori, WayPossibility psb) {
	switch(ori) {
		case EAST:
			East.setPossibility(psb);
			break;
		case WEST:
			West.setPossibility(psb);
			break;
		case NORTH:
			North.setPossibility(psb);
			break;
		case SOUTH:
			South.setPossibility(psb);
			break;

	}
}

// method to set the explored state
void LabyPoint::setWayExplored(Orientation ori) {
	switch(ori) {
		case EAST:
			East.setExplored();
			break;
		case WEST:
			West.setExplored();
			break;
		case NORTH:
			North.setExplored();
			break;
		case SOUTH:
			South.setExplored();
			break;

	}
}

void LabyPoint::print() const {
	cout << "[" << p.x << "," << p.y << "]"
	     << " E:"
	     <<  getPsblStr(East.getPossibility())[0]
	     <<  getExplrStr(East.getExplored())
	     << " W:"
	     <<  getPsblStr(West.getPossibility())[0]
	     <<  getExplrStr(West.getExplored())
	     << " N:"
	     <<  getPsblStr(North.getPossibility())[0]
	     <<  getExplrStr(North.getExplored())
	     << " S:"
	     <<  getPsblStr(South.getPossibility())[0]
	     <<  getExplrStr(South.getExplored()) << endl;
}

// Method to display the junction details
void Junction::display() const {
	LabyPoint *tmp;

	cout << "Jx:";
	pnt->print();
	if(P1 == NULL) {
		cout << "P1=[NULL]\t";
	} else {
		cout << "P1=";
		//P1->getStartPoint()->printCoord();
		//tmp = P1->getStartPoint();
		//tmp = P1->getCurrentPoint();
		//P1->print();
		cout << "\t";
	}

	if(P2 == NULL) {
		cout << "P2=[NULL]\t";
	} else {
		cout << "P2=";
		//P2->getStartPoint()->printCoord();
		cout << "\t";
	}

	if(P3 == NULL) {
		cout << "P3=[NULL]\t";
	} else {
		cout << "P3=";
		//P3->getStartPoint()->printCoord();
		cout << "\t";
	}
}

// Add a point at the end
void LabyPath::addPoint(LabyPoint *lp) {
	//cout << "Adding new point " << endl;
	// if the path is empty, then add this point at the begining
	if (start == NULL) {
		start = lp;
		current = lp;
		//cout << "New point in the start" <<endl;
	} else {	// else add the new point after the current point and update the previous and current pointers
		current->next = lp;
		lp->previous = current;
		current = lp;
	}
}

// Move to the previous point and returns the current point
LabyPoint* LabyPath::getPrevPoint() {
	// if current point is at the starting of the path
	if (current == start) {
		cout << "!!! Reached at the starting of the path !!!" <<endl;
		cin.get();
		return start;
	}

	// return the previous point
	current = current->getPrevPoint();
	return current;
}

// method to print all points in the path
void LabyPath::print() const {
	// local pointer
	LabyPoint *lp = start;

	// print the type of the path
	cout << "\nDisplaying the path details of " << pathTypeStr(type) << " path"<<endl;

	while(lp!=NULL) {
		lp->print();
		lp = lp->next;
	}

	// print the next junction
	if(nextJunction != NULL) {
		cout << "The next junction is at " <<endl;
		nextJunction->display();
	}

}

