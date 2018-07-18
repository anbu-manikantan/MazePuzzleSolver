#ifndef LABYPATH_H
#define LABYPATH_H

#include "CloudBot_Common.h"
#include <iostream>

using namespace std;

/*-----------------------------------------------------------------------------
 * Point Class:
 * Represent the cartesian coordinates
-----------------------------------------------------------------------------*/
class Point {
	public:
		int x,y;    // x and y axis

		// Constructor
		Point(int xa=0, int ya=0) {
			x = xa;
			y = ya;
		}
};


// ============================================================================
// A class to represent labyrinth way
// This defines the possibility and exploration status of a way in the labyrinth
// ============================================================================
class LabyWay {
	// Private section
	private:
		WayPossibility  isWayPsbl;		// to denote the way is possible or not
		bool			isExplored;		// to denot if the way has been explored or not
	public:
		// Public Declarations
		// constructors to set default values of the class properties
		LabyWay(){
			//cout << "=== Laby Way constructor is called ===" << endl;
			isWayPsbl = UNKNOWN;
			isExplored = false;
		}
		
		// method to set the possibility value
		void setPossibility(WayPossibility psb){
			isWayPsbl = psb;
		}
		
		// method to set the explored value
		void setExplored(){
			isExplored = true;
		}
		
		// method to get the possibility value
		WayPossibility getPossibility() const {
			return isWayPsbl;
		}
		
		// method to get the explored value
		bool getExplored() const {
			return isExplored;
		}
		
		// destructor
		~LabyWay() {
		}
		
	protected:
		// Protected Declarations
};

class LabyPath;

// ============================================================================
// A class to define labyrinth point
// ============================================================================
class LabyPoint {
	// Private section
	private:
		//int x, y;		// Cartesian coordinates of the point
		Point p;		// Cartesian coordinates of the point
		
		LabyWay East;	// stores the state of the way in east direction
		LabyWay West;	// stores the state of the way in west direction
		LabyWay North;  // stores the state of the way in north direction
		LabyWay South;  // stores the state of the way in south direction
		
		Orientation LastDir;	// stores the direction of the last point from which this point has reached
		
		int stepId;		// stores the value of step for this point
		
		LabyPoint * next;		// store the address of next laby point or junction connected to this point
		LabyPoint * previous;	// store the address of previous laby point or junction connected to this point
		
		string getPsblStr(WayPossibility psb) const;
		
		string getExplrStr(bool expl) const {
			if(expl) return "|y ";
			else return "|n ";
		}
		
	public:
		// Public Declarations
		
		// constructor
		LabyPoint(Point pnt=Point(), Orientation ldir=EAST);
		
		// method to set setpId to new value
		void setStepId(int id);
		
		// Method to get the stepId value
		int getStepId() const {
			return stepId;
		}
		
		// method to set data members of the class
		void setPoint(Point newP, Orientation ldir);
		
		// method to get the possiblity of the way
		WayPossibility getWayPsbl(Orientation ori) const;
		
		// method to get the explored state of the way
		bool getWayExplored(Orientation ori) const;
		
		// method to set the way value
		void setWayPsbl(Orientation ori, WayPossibility psb);
			
		// method to set the explored state
		void setWayExplored(Orientation ori);
		
		// method to get the coordinates of the laby point
		Point getPoint() const {
			return p;
		}
		
		// Method to get the last direction travelled
		Orientation getLastDir() const {
			return LastDir;	
		}

		// Method to get the previous point
		LabyPoint *getPrevPoint() const {
			return previous;
		}
		
		// Method to get the next point
		LabyPoint *getNextPoint() const {
			return next;
		}
		
		// Method to set the next point
		void setNextPoint(LabyPoint * lp) {
			next = lp;
		}
				
		// method to print the coordinates
		void printCoord(){
			cout << "[" << p.x << "," << p.y << "]";
		}
		
		// method to print the properties of the laby point
		void print() const;

		friend class LabyPath;
		
	protected:
		// Protected Declarations
};


class LabyPath;

// ============================================================================
// A class to define Laby junction
// ============================================================================
class Junction {
	// Private section
	private:
		LabyPoint *pnt;		// Cartesian coordinates of the point
		
		Orientation LastDir;	// stores the direction of the last point from which this point has reached
		
		LabyPath* P1,*P2,*P3;		// outward paths connected to this junction
		LabyPath* prevPath;			// inward path connected to this junction
		
	public:
		// Public Declarations
		// Constructor
		Junction(LabyPoint *lP):pnt(lP),P1(NULL),P2(NULL),P3(NULL),prevPath(NULL){
		}
		
		// Method to set path P1
		void setP1(LabyPath *path){
			P1 = path;
		}
		
		// Method to set path P2
		void setP2(LabyPath *path){
			P2 = path;
		}
		
		// Method to set path P3
		void setP3(LabyPath *path){
			P3 = path;
		}
		
		// Method to get path P1
		LabyPath *getP1() const {
			return P1;
		}
		
		// Method to get path P2
		LabyPath *getP2() const {
			return P2;
		}
		
		// Method to get path P3
		LabyPath *getP3() const {
			return P3;
		}

		// Method to get previous path
		LabyPath *getPreviousPath() {
			return prevPath;
		}
		
		// Method to set previous path
		void setPreviousPath(LabyPath * prevP) {
			prevPath = prevP;
		}
				
		// Method to display the junction details
		void display() const;
		
	protected:
		// Protected Declarations
};

// ============================================================================
// A class to define Laby path
// ============================================================================
class LabyPath {
	// Private section
	private:
		LabyPoint* start;	// starting point of the labyrinth path
		LabyPoint* current; // current point in the path
		LabyPoint* end;		// end point of the labyrinth path
		LabyPathType type;	// type of the labyrinth path 
		
		Junction* nextJunction;		// Junction connected at the end of the path
		Junction* prevJunction;		// Junction connected at the begin of the path
		
		LabyPath* prevPath;			// pointer to a path which is connected to this path
		
	public:
		// Public Declarations
		// constructor
		LabyPath(LabyPoint* stPoint = NULL):start(stPoint),
											current(stPoint),
											end(NULL),
											type(CONNECTED),
											nextJunction(NULL),
											prevJunction(NULL), 
											prevPath(NULL) {
		}
		
		// Add a point at the end
		void addPoint(LabyPoint *lp);
				
		// Move to the previous point and returns the current point
		LabyPoint* getPrevPoint();
		
		// method to get the start point in the path
		LabyPoint* getStartPoint() const {
			return start;
		}
		
		// method to get the current point in the path
		LabyPoint* getCurrentPoint() const {
			return current;
		}
			
		// method to set previous junction
		void setPrevJunction(Junction* Junc){
			prevJunction = Junc;
		}
		
		// method to set next junction
		void setNextJunction(Junction* Junc){
			nextJunction = Junc;
		}
		
		// method to get previous junction
		Junction* getPrevJunction() const {
			return prevJunction;
		}
		
		// method to get next junction
		Junction* getNextJunction() const {
			return nextJunction;
		}

		// method to set previous path
		void setPrevPath(LabyPath* pvPath){
			prevPath = pvPath;
		}
		
		// method to get previous path
		LabyPath* getPrevPath() const {
			return prevPath;
		}

		// Method to get the end point
		LabyPoint* getEndPoint() const {
			return end;
		}
		
		// Method to set the end point
		void setEndPoint(LabyPoint *eP){
			end = eP;
			end->setNextPoint(NULL);
		}
		
		// method to set the type of the path
		void setType(LabyPathType typ){
			type = typ;
		}
		
		// method to get the type of the path
		LabyPathType getType() const {
			return type;
		}
		
		// method to print all points in the path
		void print() const;
		
		// destructor
		~LabyPath(){
			
		}
		
	protected:
		// Protected Declarations
};




#endif

/*	TBD
1. Let Junction class be derived from LabyPoint
*/
