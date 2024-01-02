//  ========================================================================
//  COSC363: Computer Graphics (2022);  University of Canterbury.
//  Max Bastida
//
//  Motion.cpp
//  Functions for positioning animated objects in the scene.
//  ========================================================================

#include <cmath>
#include <GL/freeglut.h>
#include <iostream>
using namespace std;

void moveTrain(int icurr, float* track_x, float* track_z, int NPNTS) {

	int iback;
	int iforward;
	if (icurr == 0) {
		iback = NPNTS - 1;
		iforward = 1;
	}
	else if (icurr == NPNTS - 1) {
		iback = icurr - 1;
		iforward = 0;
	}
	else {
		iback = icurr - 1;
		iforward = icurr + 1;
	}

	float v[3] = { track_x[iforward] - track_x[iback], 0.0, track_z[iforward] - track_z[iback] };
	float dot = v[0] / sqrt(pow(v[0], 2) + pow(v[2], 2));
	float theta = acos(dot) / 3.14 * 180.0 + 180;
	if (v[0] == 0) {
		if (v[2] < 0) {
			theta = -1*theta;
		}
	}
	else if (v[2] >= 0) {
		theta = theta * -1;
	}

	glTranslatef(track_x[icurr], 1, track_z[icurr]);
	glRotatef(theta, 0.0, 1., 0.0);
}


void trainCamera(int icurr, float* track_x, float* track_z, int NPNTS) {

	int iback;
	int iforward;
	if (icurr == 0) {
		iback = NPNTS - 1;
		iforward = 1;
	}
	else if (icurr == NPNTS - 1) {
		iback = icurr - 1;
		iforward = 0;
	}
	else {
		iback = icurr - 1;
		iforward = icurr + 1;
	}

	float v[3] = { track_x[iforward] - track_x[iback], 0.0, track_z[iforward] - track_z[iback] };

	gluLookAt(track_x[icurr] + 2* v[0], 15, track_z[icurr] + 2*v[2],
		(float)track_x[icurr] + 3*v[0], 15.0+v[1], (float)track_z[icurr]+ 3*v[2], 0, 1, 0);
}