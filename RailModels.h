//  ========================================================================
//  COSC363: Computer Graphics (2022);  University of Canterbury.
//
//  RailModels.h
//  Header file for a collection of models for a railway scene
//  ========================================================================

void floor();

void tracks(float, float, float, float, int, float*, float*) ;

void engine();

void wagon(GLuint, GLuint, GLuint, GLuint, GLuint);

void base();

void station(GLuint);

void tunnel(int start_position, int tunnel_len, float* track_x, float* track_z, GLuint texture);