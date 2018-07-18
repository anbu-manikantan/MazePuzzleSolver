#ifndef LABYRINTH_SIMU_H
#define LABYRINTH_SIMU_H

#include "CloudBot_Common.h"
#include "LabyPath.h"

/*-----------------------------------------------------------------------------
 * Labyrinth Simulation Class:
 *	Represent a point in the maze with possible value in all directions
 * Properties:
 * 		1. Possible values in all four directions
 * 		2. Possible values in the current direction
 * Capabilities:
 * 		1. set possible ways of the given position
 * 		2. get the possible ways in the current position
-----------------------------------------------------------------------------*/
class LabyrinthSimu {
	private:
		WayPossibility  North, South, East, West;  //Front, Rear, Left, Right;
		WayPossibility  Current; 	// To indicate the current position is wall or path
		T_CamDir		camSide;	// LEFT or RIGHT ||| TBD: move this data member to LabySimu class

	public:
		// Constructor
		LabyrinthSimu(T_CamDir dir = RIGHT):camSide(dir) {
		}

		// method to set camera direction
		void setCamDir(T_CamDir dir) {
			camSide = dir;
		}

		// method to set the possible way for all direction
		void setPsblWay(WayPossibility Cur, WayPossibility Nrt, WayPossibility Sut, WayPossibility Est, WayPossibility Wst);
		
		bool getPsblWay(Orientation ort, WayPossibility& front, WayPossibility& side) const;
};

/*-----------------------------------------------------------------------------
 * Labyrinth Simulation interface Class:
 *	Read input file and create Maze simulation matrix
 * Properties:
 * 		1. Matrix of simulation point
 * 		2. offset point to the labyrinth entrance
 * Capabilities:
 * 		1. Initialize the labyrinth with the input file
 * 		2. get the possible ways in the current position
-----------------------------------------------------------------------------*/
class LabySimu {
		// Private section
	private:
		// declaring 16 by 16 points of labyrinth cells
		LabyrinthSimu simuPoint[16][16];
		
		// exit points
		Point exitPoints[4];

		// offset position of the labyrinth entrance
		int x_off, y_off;
	public:
		// Public Declarations
		int Initialize(char* filename);
		
		// method to get the possible value of a point
		bool getPsblWay(Point p, Orientation ort, WayPossibility& front, WayPossibility& side) const {
			if (!simuPoint[p.x+x_off][y_off-p.y].getPsblWay(ort,front,side)) {
				cout << " Error:  The given coordinate [" <<p.x << "," << p.y << "] is not a valid path location !!! (" << p.x+x_off <<"," <<y_off-p.y <<")" << endl;
				return false;
			}
			return true;
		}
	protected:
		// Protected Declarations
};


#endif


