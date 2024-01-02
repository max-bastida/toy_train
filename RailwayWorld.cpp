//  ========================================================================
//  COSC363: Computer Graphics (2022);  University of Canterbury.
//
//  FILE NAME: RailwayWorld.cpp
//  See Lab02.pdf for details
//  ========================================================================

#include <math.h>
#include <GL/freeglut.h>
#include "RailModels.h"
#include "LoadTrack.h"
#include "Motion.h"
#include "loadBMP.h"

float position = 0.;
int icurr = 0;
int counter = 0;
float speed = 1.;
float acceleration = 0.;
float cam_angle = 90;
float cam_pos[3] = { 0.0, 50.0, 300.0 };
int carriageSpacing = 9;
GLuint txId[5];
int curr_camera = 0;


//-------------------------------------------------------------------
void loadTexture()
{
    glGenTextures(5, txId); 				// Create a Texture object
    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, txId[0]);		//Blue car side with logo
    loadBMP("train_car_logo.bmp");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[1]);		//Blue car side with M22
    loadBMP("train_car_bluem.bmp");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, txId[2]);		//Plain red car side
    loadBMP("train_car_red.bmp");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, txId[3]);		//Plain blue car side
    loadBMP("train_car_blue.bmp");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, txId[4]);		//Blue car side with logo
    loadBMP("bricks.bmp");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
}

//---------------------------------------------------------------------
void initialize(void) 
{
    float grey[4] = {0.2, 0.2, 0.2, 1.0};
    float white[4]  = {1.0, 1.0, 1.0, 1.0};
    loadTrack("track2.csv");

    glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

//	Define light's ambient, diffuse, specular properties
    glLightfv(GL_LIGHT0, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white);
    glLightfv(GL_LIGHT1, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT1, GL_SPECULAR, white);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    glMaterialf(GL_FRONT, GL_SHININESS, 50);
 
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glClearColor (0.0, 0.0, 0.0, 0.0);  //Background colour

    loadTexture();

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(60., 1.0, 10.0, 1000.0);   //Perspective projection
}


//-------------------------------------------------------------------
void display(void)
{
    float lgt_pos[] = { 0.0f, 50.0f, 0.0f, 1.0f };  //light0 position (directly above the origin)
    float spot_pos[] = { -10.0f, 14.0f, 0.0f, 1.0f };  //light1 position
    float spot_dir[] = { -1.0f, -1.0f, 0.0f, 0.0f };

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (curr_camera == 0) {
        gluLookAt(cam_pos[0], cam_pos[1], cam_pos[2], 
            cam_pos[0] + (cos(cam_angle * 3.14 / 180) * 250), 0.0, 
            cam_pos[2] - (sin(cam_angle * 3.14 / 180) * 250), 0.0, 1.0, 0.0);
    }
    else if (curr_camera == 1) {
        gluLookAt(166, 15, -185, -100, 0, 0, 0, 1, 0);
    }
    else {
        trainCamera(icurr, track_x, track_z, NPNTS);
    }
   glLightfv(GL_LIGHT0, GL_POSITION, lgt_pos);   //light position
   glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.0);
   glLightf(GL_LIGHT1, GL_SPOT_EXPONENT,0.01);

   floor();
   tunnel(45, 50, track_x, track_z, txId[4]);
   station(txId[2]);
   tracks(4.0, 6.0, 2, 9.0, NPNTS, track_x, track_z);  //mean radius 120 units, width 10 units
   
   glPushMatrix();
   moveTrain(icurr, track_x, track_z, NPNTS);
    glLightfv(GL_LIGHT1, GL_POSITION, spot_pos);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_dir);
    engine();		 //locomotive
   glPopMatrix();

   glPushMatrix();
   moveTrain((icurr - carriageSpacing + NPNTS)%NPNTS, track_x, track_z, NPNTS);
    wagon(txId[1], txId[3], txId[1], txId[3], txId[3]);
   glPopMatrix();

   glPushMatrix();
    moveTrain((icurr - carriageSpacing*2 + NPNTS)%NPNTS, track_x, track_z, NPNTS);
    wagon(txId[2], txId[2], txId[2], txId[2], txId[2]);
   glPopMatrix();

   glPushMatrix();
   moveTrain((icurr- carriageSpacing*3 + NPNTS)%NPNTS, track_x, track_z, NPNTS);
   wagon(txId[0], txId[3], txId[0], txId[3], txId[3]);
   glPopMatrix();

   glPushMatrix();
   moveTrain((icurr- carriageSpacing*4 + NPNTS)%NPNTS, track_x, track_z, NPNTS);
   base();
   glPopMatrix();

   glPushMatrix();
   moveTrain((icurr - carriageSpacing * 5 + NPNTS) % NPNTS, track_x, track_z, NPNTS);
   wagon(txId[2], txId[2], txId[2], txId[2], txId[2]);
   glPopMatrix();

   
   glutSwapBuffers();   //Useful for animation
}

void myTimer(int value)
{
    position += speed;
    speed += acceleration;
    if (icurr == 244) {
        acceleration = -1.0 / 150.0;
    }
    if (speed > 1) {
        speed = 1;
        acceleration = 0;
    }
    else if (speed <= 0 && counter < 30) {
        speed = 0;
        acceleration = 0;
        counter++;
    }
    else if (speed == 0) {
        acceleration = 1.0 / 76.0;
        speed = acceleration;
        counter = 0;
    }
    icurr = int(position);
    if (icurr >= NPNTS) {
        icurr = 0;
        position = 0;
    }
    glutPostRedisplay();
    glutTimerFunc(50, myTimer, 0);
}

//---- Special key event callback ---------------------------------------
// Used for moving the camera
void special(int key, int x, int y)
{
    if (curr_camera == 0) {
        if (key == GLUT_KEY_RIGHT) cam_angle--;     //turn
        else if (key == GLUT_KEY_LEFT) cam_angle++;
        else if (key == GLUT_KEY_UP) {              // move in direction camera is facing
            cam_pos[0] += cos(cam_angle * 3.14 / 180) * 2;
            cam_pos[2] -= sin(cam_angle * 3.14 / 180) * 2;
        }
        else if (key == GLUT_KEY_DOWN) {
            cam_pos[0] -= cos(cam_angle * 3.14 / 180) * 2;
            cam_pos[2] += sin(cam_angle * 3.14 / 180) * 2;
        }
        else if (key == GLUT_KEY_PAGE_UP) cam_pos[1] ++; // move camera up and down
        else if (key == GLUT_KEY_PAGE_DOWN) cam_pos[1] --;
    }
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 'c') {
        curr_camera++;
        if (curr_camera >= 3) {
            curr_camera = 0;
        }
    }
}

//---------------------------------------------------------------------
int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE|GLUT_DEPTH);
   glutInitWindowSize (600, 600); 
   glutInitWindowPosition (50, 50);
   glutCreateWindow ("Toy Train");
   initialize ();

   glutDisplayFunc(display);
   glutSpecialFunc(special);
   glutKeyboardFunc(keyboard);
   glutTimerFunc(50, myTimer, 0);
   glutMainLoop();
   return 0;
}
