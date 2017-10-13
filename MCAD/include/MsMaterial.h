////////////////////////////////////////////////////////////////////////////
//	  MsMaterial.h
////////////////////////////////////////////////////////////////////////////
#pragma once

#include	<GL/gl.h>

namespace MC
{

inline	void GLSet3f( GLfloat* fDst, GLfloat f1, GLfloat f2, GLfloat f3)
					{ fDst[0] = f1; fDst[1] = f2; fDst[2] = f3;}
inline	void GLSet4f( GLfloat* fDst, GLfloat f1, GLfloat f2, GLfloat f3, GLfloat f4)
					{ fDst[0] = f1; fDst[1] = f2; fDst[2] = f3; fDst[3] = f4;}

// Material
typedef struct {
	GLfloat		ambient[4];
	GLfloat		diffuse[4];
	GLfloat		specular[4];
	GLfloat		shininess[1];
	GLfloat		emission[4];
} MATERIAL_LIST;

// LIGHT MODEL
typedef struct {
	GLfloat		ambient[4];
	GLfloat		local_view;
	GLfloat		two_size;
} LIGHT_MODEL;

// LIGHT
typedef struct {
	GLint	ON;
	GLfloat		ambient[4];
	GLfloat		diffuse[4];
	GLfloat		specular[4];
	GLfloat		position[4];
	GLfloat		spot_direction[3];
	GLfloat		spot_exponet;
	GLfloat		spot_cutoff;
	GLfloat		KC;
	GLfloat		KL;
	GLfloat		KQ;
} LIGHT;

// Argument Opt
typedef struct
{
	char lightname[128];
	char modelname[128];
	char materialname[128];
	char reverce;
} ARG_OPT;

} // namespace MC