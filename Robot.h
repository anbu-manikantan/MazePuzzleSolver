#ifndef ROBOT_H
#define ROBOT_H

#include "CloudBot_Common.h"
#include "LabyrinthSimu.h"

// ============================================================================
// A class to represent the Robot
// This defines the current postion, step count, orientation and side of the camera of the Roobt
// ============================================================================
class Robot {
		// Private section
	private:
		LabyPoint *pos;		// represent the current position of the Robot

		Orientation ori;	// orientation of the Robot

		T_CamDir camSide;	// in which side camera is fitted

		int stepCnt;		// count of number of steps travelled by the robot
		
		LabySimu labySimu;	// simulation of labyrinth
		
		LabyPath* startPath; // First path to start the journey
		LabyPath* currentPath; // The current path Robot is travelling

		// private method to make the Robot move in backward direction
		void moveBack();

		// private method to create a new Junction from the current path
		void createNewJunction(Orientation orien);
				
		// private method to determine the side direction based on the camSide
		Orientation sideDir() {
			switch(ori) {
				case EAST:
					return ((camSide == LEFT)? NORTH: SOUTH);
				case WEST:
					return ((camSide == LEFT)? SOUTH: NORTH);
				case NORTH:
					return ((camSide == LEFT)? WEST: EAST);
				case SOUTH:
					return ((camSide == LEFT)? EAST: WEST);
			}
		}

		// private method to get the opposite side of camSide
		Orientation oppSideDir() {
			switch(ori) {
				case EAST:
					return ((camSide == LEFT)? SOUTH: NORTH);
				case WEST:
					return ((camSide == LEFT)? NORTH: SOUTH);
				case NORTH:
					return ((camSide == LEFT)? EAST: WEST);
				case SOUTH:
					return ((camSide == LEFT)? WEST: EAST);
			}
		}
		
		// private method to get the opposite direction
		//Orientation oppDir(Orientation orient = ori) {
		Orientation oppDir(Orientation orient) {
			switch(orient) {
				case EAST:
					return WEST;
				case WEST:
					return EAST;
				case NORTH:
					return SOUTH;
				case SOUTH:
					return NORTH;
			}
		}

		// Private method to get remaining two directions
		void getRemainingDir(Orientation dir1, Orientation dir2, Orientation& nD1, Orientation& nD2);
		
	public:
		// Public Declarations
		// Constructor
		Robot(T_CamDir camS,int xa=0, int ya=0) {
			// Initialize the current position of the Robot
			pos = new LabyPoint();
			pos->setPoint(Point(0,0),EAST);

			// Initialize the orientation of the Robot to East
			ori = EAST;

			// Initialize the side of the camera
			camSide = camS;

			// Initialize the step count
			stepCnt = 0;
			
			// Initialize WEST direction is not possible as that the entry point
			pos->setWayPsbl(WEST,NOT_PSBL);
			
			// Initialize the start and current paths
			startPath = currentPath = NULL;
			
			// Initialize the labyrinth 
			//labySimu.Initialize("MazeSimu.txt");
			//labySimu.setOffset(0,8);
			labySimu.Initialize((char*)"MazePattern1.txt");
		}

		// method to get the location of the Robot
		Point getLocation() {
			return pos->getPoint();
		}

		// method to get the orientation of the Robot
		Orientation getOrientation() {
			return ori;
		}

		// method to update the possibility of the current point with front and side camera inputs
		void updatePointPossibility(WayPossibility fnt, WayPossibility side) {
			// update the possibility of the front direction
			pos->setWayPsbl(ori,fnt);

			// update the possibility of side direction
			pos->setWayPsbl(sideDir(),side);
		}

		// method to get the current point
		LabyPoint* getPoint() {
			return pos;
		}
		
		// method to move a step in forward direction
		void stepForward();
		
		// method to implement the movement of the Robot
		void move();
		
		// method to find short path
		void shortPath();
		
	protected:
		// Protected Declarations
};


#endif
