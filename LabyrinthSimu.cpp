//============================================================================
// Name        : LabyrinthSimu.cpp
// Author      : Anbumani Manikantan
// Version     :
// Copyright   : Your copyright notice
// Description : Simulation of Labyrinth
//============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include "LabyrinthSimu.h"

using namespace std;

		// method to set the possible way for all direction
		void LabyrinthSimu::setPsblWay(WayPossibility Cur, WayPossibility Nrt, WayPossibility Sut, WayPossibility Est, WayPossibility Wst) {
			Current = Cur;
			North = Nrt;
			South = Sut;
			East = Est;
			West = Wst;
			//cout << "Current="Current <<" Front="<<Front<<" Rear="<<Rear<<" Left="<<Left<<" Rigth="<<Right<<endl;
		}


bool LabyrinthSimu::getPsblWay(Orientation ort, WayPossibility& front, WayPossibility& side) const {
	// Return the possible way only if the current position is path
	if( Current == PSBL) {
		switch (ort) {
			case NORTH:
				front = North;
				side  = (camSide == RIGHT) ? East : West;
				break;
			case SOUTH:
				front = South;
				side  = (camSide == RIGHT) ? West : East;
				break;
			case EAST:
				front = East;
				side  = (camSide == RIGHT) ? South : North;
				break;
			case WEST:
				front = West;
				side  = (camSide == RIGHT) ? North : South ;
				break;
		}
	} else {
		return false;
		front = side = UNKNOWN;
	}
	return true;
}


int LabySimu::Initialize(char* filename) {
	// local variable declaration
	char Maze [16][16] ;
	char ch;
	int row=0, col=0;
	WayPossibility cur,North,South,East,West;
	
	// Initialize the exit points
	for(int i=0; i<4; i++){
		exitPoints[i].x = -1;
		exitPoints[i].y = -1;
	}

	// Open the file to read the maze simulation matrix
	ifstream reader(filename);
	//ifstream reader("MazeSimu.txt");

	if(! reader) {

		cout << "Error opening file" << endl;
		return -1;

	} else {

		// Read each character from the stream until end of file
		while(! reader.eof()) {

			// Get the next letter and output it
			reader.get(ch);
			// Check if it reached the end of the row
			if (ch == '\n') {
				if (col != 16) {
					if (row != 16) {
						cout << "The row "<<row<< "does not have 16 vals, instead "<<col<<endl;
						return -1;
					} else
						break;
				}
				col=0;
				row++;
			} else {
				Maze[row][col++] = ch;
			}

		}
		reader.close();
	}

	int exitCnt = 0;	// count of exit points in the maze
	
	// Update the labyrinth points with data read from the file
	cout << "Maze in array..." <<endl;
	for (int r = 0; r <16; r++) {
		cout << endl;
		for (int c=0; c<16; c++) {
			cout  <<Maze[r][c];
			cur  = (Maze[r][c] == '@')? NOT_PSBL : PSBL;

			if (c==15) {	// outer wall of the maze
				if(Maze[r][c] == ' '){	// space means exit of maze
					East = EXIT;
					//cout << "!!! ==> Exit Encountered at [" << r << "," << c << "] <== !!!" << endl;
					exitPoints[exitCnt].x = c;
					exitPoints[exitCnt++].y = r;
				} else{
					East = NOT_PSBL;
				}
			} else {
				East = (Maze[r][c+1] == '@')? NOT_PSBL : PSBL;
			}

			if (c==0) {			// Outer wall of the maze
				if(Maze[r][c] == ' '){	// space means exit of maze
					West = EXIT;
					//cout << "!!! ==> Exit Encountered at [" << r << "," << c << "] <== !!!" << endl;
					exitPoints[exitCnt].x = c;
					exitPoints[exitCnt++].y = r;
				} else{
					West = NOT_PSBL;
				}
			} else {
				West = (Maze[r][c-1] == '@')? NOT_PSBL : PSBL;
			}

			if (r==0) {			// outer wall of the maze
				if(Maze[r][c] == ' '){	// space means exit of maze
					North = EXIT;
					//cout << "!!! ==> Exit Encountered at [" << r << "," << c << "] <== !!!" << endl;
					exitPoints[exitCnt].x = c;
					exitPoints[exitCnt++].y = r;
				} else{
					North = NOT_PSBL;
				}
			} else {
				North = (Maze[r-1][c] == '@')? NOT_PSBL : PSBL;
			}

			if (r==15) {		// outer wall of the maze
				if(Maze[r][c] == ' '){	// space means exit of maze
					South = EXIT;
					//cout << "!!! ==> Exit Encountered at [" << r << "," << c << "] <== !!!" << endl;
					exitPoints[exitCnt].x = c;
					exitPoints[exitCnt++].y = r;
				} else{
					South = NOT_PSBL;
				}

			} else {
				South = (Maze[r+1][c] == '@')? NOT_PSBL : PSBL;
			}

			simuPoint[c][r].setPsblWay(cur, North,South,East,West);
		}
	}
	cout<<endl;
	cout<<endl;

	cout << "Number of exit points in this maze is " << exitCnt<<endl;
	for(int i=0; i<exitCnt; i++){
		cout <<i+1<<". ["<<exitPoints[i].x<<","<<exitPoints[i].y<<"]" << endl;
	}
	
	if (exitCnt <2){
		cout << "The number of exit points are not sufficient to come out of the maze. Exiting the application..."<<endl;
		exit(-1);
	}
	
	// Get the user input to set the starting point of the maze
	int startPoint;
	cout << "Please select the enterance point from the above list:";
	cin >> startPoint;
	
	if (startPoint > exitCnt){
		cout << "Invalid input. Exiting the application"<<endl;
		exit(-1);
	}
	
	// set the labyrinth entrance offset
	x_off = exitPoints[startPoint-1].x;
	y_off = exitPoints[startPoint-1].y;	
}


/*   TBD
1. T_CamDir		camSide;	// LEFT or RIGHT ||| TBD: move this data member to LabySimu class
How to initialize LabyrinthSimu simuPoint[16][16]; with Camera side
2. Warning depredated conversion from string to char* => labySimu.Initialize("MazeSimu.txt");




*/
