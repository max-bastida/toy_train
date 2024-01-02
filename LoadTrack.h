//  ========================================================================
//  COSC363: Computer Graphics (2022);  University of Canterbury.
//
//  LoadTrack.h
//  Functions for loading a track csv file
//  ========================================================================


#if !defined(LOAD_TRACK)
#define LOAD_TRACK

#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>
#include <GL/freeglut.h>
using namespace std;

const int NPNTS = 545;
float track_x[NPNTS];
float track_z[NPNTS];

void loadTrack(const char* filename) {
	ifstream file(filename, ios::in | ios::binary);

	if (!file)
	{
		cout << "*** Error opening track file: " << filename << endl;
		exit(1);
	}
	file.ignore(200, file.widen('\n'));
	for (int i = 0; i < NPNTS; i++) {
		char string_x[265];
		char string_z[265];
		file.getline(string_x, 265, ',');
		file.getline(string_z, 265, '\n');
		stringstream tmp_x(string_x);
		tmp_x >> track_x[i];
		stringstream tmp_z(string_z);
		tmp_z >> track_z[i];
	}
}

#endif