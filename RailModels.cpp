//  ========================================================================
//  COSC363: Computer Graphics (2022);  University of Canterbury.
//
//  RailModels.cpp
//  A collection of functions for generating the models for a railway scene
//  ========================================================================

#include <cmath>
#include <GL/freeglut.h>
#include "RailModels.h"
using namespace std;

const int tunnel_array_len = 182;

void normal(float x1, float x2, float x3,
	float y1, float y2, float y3,
	float z1, float z2, float z3)
{
	float nx = y1 * (z2 - z3) + y2 * (z3 - z1) + y3 * (z1 - z2);
	float ny = z1 * (x2 - x3) + z2 * (x3 - x1) + z3 * (x1 - x2);
	float nz = x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2);
	glNormal3f(nx, ny, nz);
}

void draw_textured_quad(GLuint texId, float x1, float x2, float x3, float x4,
                        float y1, float y2, float y3, float y4,
                        float z1, float z2, float z3, float z4,
                        float texture_x, float texture_y) {
	glBindTexture(GL_TEXTURE_2D, texId);
	glBegin(GL_QUADS);
		normal(x1, x2, x3, y1, y2, y3, z1, z2, z3);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(x1, y1, z1);
		glTexCoord2f(texture_x, 0.0);
		glVertex3f(x2, y2, z2);
		glTexCoord2f(texture_x, texture_y);
		glVertex3f(x3, y3, z3);
		glTexCoord2f(0.0, texture_y);
		glVertex3f(x4, y4, z4);
	glEnd();
}


//--------------- GROUND PLANE ------------------------------------
// This is a square shaped region on the xz-plane of size 400x400 units
// centered at the origin.  This region is constructed using small quads
// of unit size, to facilitate rendering of spotlights
//-----------------------------------------------------------------
void floor()
{
	float white[4] = {1., 1., 1., 1.};
	float black[4] = {0};
	glColor4f(0.665, 0.7, 0.5, 1.0);  //The floor is gray in colour
	glNormal3f(0.0, 1.0, 0.0);

    //suppress specular reflections
    glMaterialfv(GL_FRONT, GL_SPECULAR, black);

	//The floor is made up of several tiny squares on a 400 x 400 grid. Each square has a unit size.
	glBegin(GL_QUADS);
		glVertex3f(-2000, 0, -2000);
		glVertex3f(-2000, 0, 2000);
		glVertex3f(2000, 0, 2000);
		glVertex3f(2000, 0, -2000);
	glEnd();

    glMaterialfv(GL_FRONT, GL_SPECULAR, white);

}

//--------------- RAILWAY TRACK ------------------------------------
// A circular railway track of specified median radius and width
// The tracks are modelled using small quadrilateral segments each covering 5 deg arc
// The tracks have a height of 1 unit (refer to lab notes)
//-----------------------------------------------------------------
void tracks(float inner_width, float outer_width, float sleeper_width, float sleeper_length,
		int NPNTS, float* track_x, float* track_z)  
{
	float angle1,angle2, ca1,sa1, ca2,sa2;
	float p1[2], p2[2], p3[2]; //three points on the track line
	float u1[2], u2[2], v1[2], v2[2]; //orthogonal unit vectors
	float x1,z1, x2,z2, x3,z3, x4,z4;  //four points of a quad
	float sx1, sz1, sx2, sz2, sx3, sz3, sx4, sz4; //four points of rail sleeper
    float toRad = 3.14159265/180.0;  //Conversion from degrees to radians

	p1[0] = track_x[NPNTS - 2]; p1[1] = track_z[NPNTS - 2];
	p2[0] = track_x[NPNTS - 1]; p2[1] = track_z[NPNTS - 1];
	
    glBegin(GL_QUADS);
	for (int j = -1; j < 2; j+=2)   //Two parallel tracks (radius = inRad, outRad)
	{
		for (int i = 0; i < NPNTS; i ++)    //iterate through points
		{
			p3[0] = track_x[i]; p3[1] = track_z[i];

			u1[0] = p2[0] - p1[0]; u1[1] = p2[1] - p1[1];
			float len1 = sqrt(pow(u1[0], 2) + pow(u1[1], 2));
			v1[0] = u1[1] / len1; v1[1] = -u1[0] / len1;
			u2[0] = p3[0] - p2[0]; u2[1] = p3[1] - p2[1];
			float len2 = sqrt(pow(u2[0], 2) + pow(u2[1], 2));
			v2[0] = u2[1] / len2; v2[1] = -u2[0] / len2;
			x1 = p1[0] + v1[0] * inner_width * j; z1 = p1[1] + v1[1] * inner_width * j;
			x2 = p1[0] + v1[0] * outer_width * j; z2 = p1[1] + v1[1] * outer_width * j;
			x3 = p2[0] + v2[0] * outer_width * j; z3 = p2[1] + v2[1] * outer_width * j;
			x4 = p2[0] + v2[0] * inner_width * j; z4 = p2[1] + v2[1] * inner_width * j;
			
			glColor4f(0.15, 0.1, 0.12, 1.0);

			glNormal3f(0., 1., 0.);       //Quad 1 facing up
			glVertex3f(x1, 1.0, z1);
			glVertex3f(x2, 1.0, z2);
			glVertex3f(x3, 1.0, z3);
			glVertex3f(x4, 1.0, z4);

			glNormal3f(v1[0], 0.0, v1[1]);   //Quad 2 facing inward
			glVertex3f(x1, 0.0, z1);
			glVertex3f(x1, 1.0, z1);
			glNormal3f(v2[0], 0.0, v2[1]);
			glVertex3f(x4, 1.0, z4);
			glVertex3f(x4, 0.0, z4);

			glNormal3f(-v1[0], 0.0, -v1[1]);  //Quad 3 facing outward
			glVertex3f(x2, 1.0, z2);
			glVertex3f(x2, 0.0, z2);
			glNormal3f(-v2[0], 0.0, -v2[1]);
			glVertex3f(x3, 0.0, z3);
			glVertex3f(x3, 1.0, z3);
			
			if (j == 1) {       // rail sleeper
				sx1 = p1[0] + v1[0] * sleeper_length;
				sz1 = p1[1] + v1[1] * sleeper_length;

				sx2 = p1[0] + (v1[0] * sleeper_length) + (v1[1] * sleeper_width);
				sz2 = p1[1] + (v1[1] * sleeper_length) - (v1[0] * sleeper_width);

				sx3 = p1[0] - (v1[0] * sleeper_length) + (v1[1] * sleeper_width);
				sz3 = p1[1] - (v1[1] * sleeper_length) - (v1[0] * sleeper_width);

				sx4 = p1[0] - v1[0] * sleeper_length;
				sz4 = p1[1] - v1[1] * sleeper_length;

				glColor4f(0.3, 0.1, 0.15, 1.0);
				glNormal3f(0., 1., 0.);
				glVertex3f(sx1, 0.1, sz1);
				glVertex3f(sx2, 0.1, sz2);
				glVertex3f(sx3, 0.1, sz3);
				glVertex3f(sx4, 0.1, sz4);
			}

			p1[0] = p2[0]; p1[1] = p2[1];
			p2[0] = p3[0]; p2[1] = p3[1];
		}
	}
	glEnd();
}


//--------------- MODEL BASE --------------------------------------
// This is a common base for the locomotive and wagons
// The base is of rectangular shape (20 length x 10 width x 2 height)
// and has wheels and connectors attached.
//-----------------------------------------------------------------
void base()
{
    glColor4f(0.2, 0.2, 0.2, 1.0);   //Base color
    glPushMatrix();
      glTranslatef(0.0, 4.0, 0.0);
      glScalef(30.0, 2.0, 12.0);     //Size 30x12 units, thickness 2 units.
      glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();					//Connector between wagons
      glTranslatef(20.0, 4.0, 0.0);
	  glScalef(8.0, 1.0, 1.0);
      glutSolidCube(2.0);
    glPopMatrix();

    //4 Wheels (radius = 2 units)
	//x, z positions of wheels:
	float wx[4] = {  -8,   8,   -8,    8 }; 
	float wz[4] = { 6.1, 6.1, -6.1, -6.1 };
    glColor4f(0.07, 0.07, 0.08, 1.0);    //Wheel color
	GLUquadric *q = gluNewQuadric();   //Disc

	for (int i = 0; i < 4; i++)
	{
		glPushMatrix();
		glTranslatef(wx[i], 2.0, wz[i]);
		gluDisk(q, 0.0, 2.0, 20, 2);
		glPopMatrix();
	}
}

//--------------- LOCOMOTIVE --------------------------------
// This simple model of a locomotive consists of the base,
// cabin and boiler
//-----------------------------------------------------------
void engine()
{
    base();

    //Cab
    glColor4f(0.01, 0.02, 0.0, 1.0);
    glPushMatrix();
      glTranslatef(10.0, 9, 0.0);
      glScalef(10.0, 8.0, 10.0);
      glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
      glTranslatef(9.0, 14.0, 0.0);
      glScalef(8.0, 4.0, 8.0);
      glutSolidCube(1.0);
    glPopMatrix();

    //Boiler
	GLUquadric *q = gluNewQuadric();   //Cylinder
    glPushMatrix();
      glColor4f(0.1, 0.3, 0.15, 1.0);
      glTranslatef(8.0, 10.0, 0.0);
      glRotatef(-90.0, 0., 1., 0.);
      gluCylinder(q, 5.0, 5.0, 23.0, 20, 5);
      glTranslatef(0.0, 0.0, 23.0);
      glColor4f(0.01, 0.02, 0.0, 1.0);
      gluDisk(q, 0.0, 5.0, 20, 3);
      glColor4f(0.5, 1.0, 0.0, 1.0);
      glTranslatef(0.0, 4.0, 0.2);
      gluDisk(q, 0.0, 1.0, 20,2);  //headlight!
    glPopMatrix();

    glPushMatrix();
    glColor4f(0.01, 0.02, 0.0, 1.0);
    glTranslatef(8.0, 10.0, 4.5);
    glRotatef(-90.0, 0., 1., 0.);
    gluCylinder(q, 1, 1, 23, 20, 5);
    glTranslatef(0.0, 0.0, 23.0);
    gluDisk(q, 0.0, 1.0, 20, 3);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(8.0, 10.0, -4.5);
    glRotatef(-90.0, 0., 1., 0.);
    gluCylinder(q, 1, 1, 23, 20, 5);
    glTranslatef(0.0, 0.0, 23.0);
    gluDisk(q, 0.0, 1.0, 20, 3);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-8.0, 15.0, 0);
    glRotatef(-90.0, 1., 0., 0.);
    gluCylinder(q, 2, 2, 3, 20, 5);
    glTranslatef(0.0, 0.0, 3.0);
    gluDisk(q, 1.0, 2.0, 20, 3);
    gluCylinder(q, 3, 3, 1, 20, 5);
    glTranslatef(0.0, 0.0, 1.0);
    gluDisk(q, 0.0, 3.0, 20, 3);
    glPopMatrix();
    

}

//--------------- WAGON ----------------------------------
// This simple model of a rail wagon consists of the base,
// and a cube(!)
//--------------------------------------------------------
void wagon(GLuint texId1, GLuint texId2, GLuint texId3, GLuint texId4, GLuint texId5)
{
    base();
	glEnable(GL_TEXTURE_2D);

    glColor4f(1.0, 1.0, 1.0, 1.0);
    glPushMatrix();
      glTranslatef(0.0, 12.5, 0.0);
      glScalef(28.0, 15.0, 11.0);
    draw_textured_quad(texId1, -0.5, 0.5, 0.5, -0.5, -0.5, -0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 1.0, 1.0);
    draw_textured_quad(texId2, 0.5, 0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, 1.0);
    draw_textured_quad(texId3, 0.5, -0.5, -0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, -0.5, -0.5, -0.5, 1.0, 1.0);
    draw_textured_quad(texId4, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5, 0.5, -0.5, 0.5, 1.0);
    draw_textured_quad(texId5, -0.5, 0.5, 0.5, -0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, -0.5, -0.5, 1.0, 1.0);
    glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}


void station(GLuint texId) {
    //platform
	glColor4f(0.4, 0.3, 0.25, 1.0);
    glPushMatrix();
        glTranslatef(66.0, 2.5, -165.0);
        glScalef(200.0, 5.0, 40.0);
        glutSolidCube(1.0);
	glPopMatrix();

    glColor4f(0.4, 0.3, 0.25, 1.0);
        glPushMatrix();
        glTranslatef(66.0, 26.5, -171.0);
        glScalef(200.0, 2, 28.0);
        glutSolidCube(1.0);
    glPopMatrix();

    glColor4f(0.3, 0.2, 0.15, 1.0);
        glPushMatrix();
        glTranslatef(66.0+85, 0.0, -161.0);
        glRotatef(90, -1, 0, 0);
        glutSolidCylinder(2, 25, 50, 4);
    glPopMatrix();

    glColor4f(0.3, 0.2, 0.15, 1.0);
    glPushMatrix();
    glTranslatef(66.0 - 85, 0.0, -161.0);
    glRotatef(90, -1, 0, 0);
    glutSolidCylinder(2, 25, 50, 4);
    glPopMatrix();

    glColor4f(0.3, 0.2, 0.15, 1.0);
    glPushMatrix();
    glTranslatef(66.0 -30, 0.0, -161.0);
    glRotatef(90, -1, 0, 0);
    glutSolidCylinder(2, 25, 50, 4);
    glPopMatrix();

    glColor4f(0.3, 0.2, 0.15, 1.0);
    glPushMatrix();
    glTranslatef(66.0 +30, 0.0, -161.0);
    glRotatef(90, -1, 0, 0);
    glutSolidCylinder(2, 25, 50, 4);
    glPopMatrix();

    //building
    glEnable(GL_TEXTURE_2D);
    glColor4f(1, 1, 1, 1.0);
    glPushMatrix();
        glTranslatef(66.0, 2.5, -210.0);
        glScalef(200.0, 50.0, 50.0);
        glutSolidCube(1.0);
        draw_textured_quad(texId, -0.5, -0.5, -0.5, -0.5, 0.5, 0.5, 1.0, 1.0, -0.5, 0.5, 0.2, -0.2, 0.0, 0.0);
        draw_textured_quad(texId, -0.5, 0.5, 0.5, -0.5, 0.5, 0.5, 1.0, 1.0, 0.5, 0.5, 0.2, 0.2, 3.0, 1.0);
        draw_textured_quad(texId, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 1.0, 1.0, 0.5, -0.5, -0.2, 0.2, 0.0, 0.0);
        draw_textured_quad(texId, 0.5, -0.5, -0.5, 0.5, 0.5, 0.5, 1.0, 1.0,-0.5, -0.5, -0.2, -0.2,3.0, 1.0);
        draw_textured_quad(texId, -0.5, -0.5, 0.5, 0.5, 1.0, 1.0, 1.0, 1.0, -0.2, 0.2, 0.2, -0.2,  1.0, 1.0);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

}

void tunnel_pattern(float* x, float* y, float* nx, float* ny, float radius) {
    float toRads = 3.1415 / 180;
    x[0] = radius; y[0] = 0.01;
    nx[0] = 1; ny[0] = 0;
    x[tunnel_array_len - 1] = -radius; y[tunnel_array_len - 1] = 0.01;
    nx[tunnel_array_len - 1] = -1; ny[tunnel_array_len - 1] = 0;

    for(int i = 0; i < tunnel_array_len - 2; i++){
        x[i+1] = (cos((float)i * toRads) * radius);
        y[i+1] = sin((float)i * toRads) * radius + radius;
        nx[i+1] = cos((float)i*toRads);
        ny[i+1] = sin((float)i*toRads);
    }
}

void position_tunnel(float* x, float* z, float* midpoint, float* direction, float* pattern) {
    for(int i = 0; i < tunnel_array_len; i++) {
        x[i] = midpoint[0] + pattern[i] * direction[0];
        z[i] = midpoint[1] + pattern[i] * direction[1];
    }
}

void tunnel_wall(int start_position, int tunnel_len, float* track_x, float* track_z, int normal_direction,
                float* pattern_x, float* y, float* nx, float* ny)
{
    float p1[2];
    float p2[2] = { track_x[start_position - 1], track_z[start_position - 1] };
    float v[2];
    float v_len;
    float v_unit[2];

    float vx[tunnel_array_len]; float vz[tunnel_array_len];
    float wx[tunnel_array_len]; float wz[tunnel_array_len];

    float tx = 0.;
    float ty = 0.;

    glColor3f(1, 1, 1);
    for (int j = start_position; j < start_position + tunnel_len; j++){
        p1[0] = track_x[j]; p1[1] = track_z[j];
        v[0] = p1[0]- p2[0]; v[1] = p1[1] - p2[1];
        v_len = sqrt(pow(v[0], 2) + pow(v[1], 2));
        v_unit[0] = v[1] / v_len; v_unit[1] = -v[0] / v_len;
        glBegin(GL_QUAD_STRIP);
        ty = 0;
        for(int i = 0; i< tunnel_array_len; i++){
            wx[i] = p1[0] + pattern_x[i] * v_unit[0];
            wz[i] = p1[1] + pattern_x[i] * v_unit[1];
            if(j > start_position){
                glNormal3f(nx[i] * v_unit[0] *(float)normal_direction, ny[i] * (float)normal_direction, nx[i] * v_unit[1] * (float)normal_direction);
                if (i == 1) {
                    ty = 0.8;
                }
                else if (i == tunnel_array_len - 1) {
                    ty += 0.8;
                }
                glTexCoord2f(tx, ty);
                glVertex3f(vx[i], y[i], vz[i]);
                glTexCoord2f(tx + 0.3, ty);
                glVertex3f(wx[i], y[i], wz[i]);
            }
            vx[i] = wx[i];
            vz[i] = wz[i];
            ty += 0.03;
        }
        tx += 0.3;
        glEnd();
        p2[0] = p1[0]; p2[1] = p1[1];
    }
}

void tunnel(int start_position, int tunnel_len, float* track_x, float* track_z, GLuint texture) {
    float pattern_x1[tunnel_array_len];
    float y1[tunnel_array_len];
    float pattern_x2[tunnel_array_len];
    float y2[tunnel_array_len];
    float nx[tunnel_array_len];
    float ny[tunnel_array_len];
    glBindTexture(GL_TEXTURE_2D, texture);
    glEnable(GL_TEXTURE_2D);
    tunnel_pattern(pattern_x1, y1, nx, ny, 15.0);
    tunnel_wall(start_position, tunnel_len, track_x, track_z, -1, pattern_x1, y1, nx, ny);
    for (int i = 0; i < tunnel_array_len; i++) {
        pattern_x2[i] = pattern_x1[i] + 9 * nx[i];
        y2[i] = y1[i] + 9 * ny[i];
    }
    tunnel_wall(start_position, tunnel_len, track_x, track_z, 1, pattern_x2, y2, nx, ny);

    float p1[2] = { track_x[start_position], track_z[start_position] };
    float p2[2] = { track_x[start_position - 1], track_z[start_position - 1] };
    float v[2] = { p2[0] - p1[0], p2[1] - p1[1] };
    float v_len = sqrt(pow(v[0], 2) + pow(v[1], 2));
    float v_unit[2] = { v[1] / v_len, -v[0] / v_len };

    glDisable(GL_TEXTURE_2D);
    glColor3f(0.3, 0.1, 0.1);
    glBegin(GL_QUAD_STRIP);
    glNormal3f(v_unit[1], 0, -v_unit[0]);
    for (int i = 0; i < tunnel_array_len; i++) {
        glVertex3f(p1[0] + pattern_x1[i] * v_unit[0], y1[i], p1[1] + pattern_x1[i] * v_unit[1]);
        glVertex3f(p1[0] + pattern_x2[i] * v_unit[0], y2[i], p1[1] + pattern_x2[i] * v_unit[1]);
    }
    glEnd();

    p1[0] = track_x[start_position + tunnel_len - 1]; p1[1] = track_z[start_position + tunnel_len - 1];
    p2[0] = track_x[start_position + tunnel_len - 2]; p2[1] = track_z[start_position + tunnel_len - 2];
    v[0] = p2[0] - p1[0]; v[1] = p2[1] - p1[1];
    v_len = sqrt(pow(v[0], 2) + pow(v[1], 2));
    v_unit[0] = v[1] / v_len; v_unit[1] = -v[0] / v_len;

    glBegin(GL_QUAD_STRIP);
    glNormal3f(v_unit[1], 0, -v_unit[0]);
    for (int i = 0; i < tunnel_array_len; i++) {
        glVertex3f(p1[0] + pattern_x1[i] * v_unit[0], y1[i], p1[1] + pattern_x1[i] * v_unit[1]);
        glVertex3f(p1[0] + pattern_x2[i] * v_unit[0], y2[i], p1[1] + pattern_x2[i] * v_unit[1]);
    }
    glEnd();

}