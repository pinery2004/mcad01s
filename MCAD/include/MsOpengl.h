////////////////////////////////////////////////////////////////////////////
//	  MsOpengl.h
//
//		座標値の実数精度のDoubleとSingleの切り替え
//		環境数値はSingleのみで切り替えなし
//
////////////////////////////////////////////////////////////////////////////
#pragma once

#include	<GL/gl.h>

inline void glColor3r( GLdouble red, GLdouble green, GLdouble blue) {
			glColor3d( red, green, blue);
}
inline void glColor3rv( const GLdouble *v) {
			glColor3dv ( v);
}
inline void glColor3r( GLfloat red, GLfloat green, GLfloat blue) {
			glColor3f( red, green, blue);
}
inline void glColor3rv( const GLfloat *v) {
			glColor3fv( v);
}
inline void glColor4r( GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha) {
			glColor4d( red, green, blue, alpha);
}
inline void glColor4rv(const GLdouble *v) {
			glColor4dv( v);
}
inline void glColor4r( GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
			glColor4f( red, green, blue, alpha);
}
inline void glColor4rv(const GLfloat *v) {
			glColor4fv( v);
}
inline void glEvalCoord1r( GLdouble u) {
			glEvalCoord1d( u);
}
inline void glEvalCoord1rv(const GLdouble *u) {
			glEvalCoord1dv( u);
}
inline void glEvalCoord1r(GLfloat u) {
			glEvalCoord1f( u);
}
inline void glEvalCoord1rv(const GLfloat *u) {
			glEvalCoord1fv( u);
}
inline void glEvalCoord2r(GLdouble u, GLdouble v) {
			glEvalCoord2d( u, v);
}
inline void glEvalCoord2rv(const GLdouble *u) {
			glEvalCoord2dv( u);
}
inline void glEvalCoord2r(GLfloat u, GLfloat v) {
			glEvalCoord2f( u, v);
}
inline void glEvalCoord2rv(const GLfloat *u) {
			glEvalCoord2fv( u);
}
inline void glGetMaprv( GLenum target, GLenum query, GLdouble *v) {
			glGetMapdv( target, query, v);
}
inline void glGetMaprv( GLenum target, GLenum query, GLfloat *v) {
			glGetMapfv( target, query, v);
}
inline void glGetTexGenrv( GLenum coord, GLenum pname, GLdouble *params) {
			glGetTexGendv( coord, pname, params);
}
inline void glGetTexGenrv( GLenum coord, GLenum pname, GLfloat *params) {
			glGetTexGenfv( coord, pname, params);
}
inline void glIndexr( GLdouble c) {
			glIndexd( c);
}
inline void glIndexrv( const GLdouble *c) {
			glIndexdv( c);
}
inline void glIndexr( GLfloat c) {
			glIndexf( c);
}
inline void glIndexrv( const GLfloat *c) {
			glIndexfv( c);
}
inline void glLoadMatrixr( const GLdouble *m) {
			glLoadMatrixd( m);
}
inline void glLoadMatrixr( const GLfloat *m) {
			glLoadMatrixf( m);
}
inline void glMap1r( GLenum target, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble *points) {
			glMap1d( target, u1, u2, stride, order, points);
}
inline void glMap1r( GLenum target, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat *points) {
			glMap1f( target, u1, u2, stride, order,  points);
}
inline void glMap2r( GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble *points) {
			glMap2d( target, u1, u2, ustride, uorder, v1, v2, vstride, vorder, points);
}
inline void glMap2r( GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat *points) {
			glMap2f( target, u1, u2, ustride, uorder, v1, v2, vstride, vorder, points);
}
inline void glMapGrid1r( GLint un, GLdouble u1, GLdouble u2) {
			glMapGrid1d( un, u1, u2);
}
inline void glMapGrid1r( GLint un, GLfloat u1, GLfloat u2) {
			glMapGrid1f( un, u1, u2);
}
inline void glMapGrid2r( GLint un, GLdouble u1, GLdouble u2, GLint vn, GLdouble v1, GLdouble v2) {
			glMapGrid2d( un, u1, u2, vn, v1, v2);
}
inline void glMapGrid2r( GLint un, GLfloat u1, GLfloat u2, GLint vn, GLfloat v1, GLfloat v2) {
			glMapGrid2f( un, u1, u2, vn, v1, v2);
}
inline void glMultMatrixr( const GLdouble *m) {
			glMultMatrixd( m);
}
inline void glMultMatrixr( const GLfloat *m) {
			glMultMatrixf( m);
}
inline void glNormal3r( GLdouble nx, GLdouble ny, GLdouble nz) {
			glNormal3d( nx, ny, nz);
}
inline void glNormal3rv( const GLdouble *v) {
			glNormal3dv( v);
}
inline void glNormal3r( GLfloat nx, GLfloat ny, GLfloat nz) {
			glNormal3f( nx, ny, nz);
}
inline void glNormal3rv( const GLfloat *v) {
			glNormal3fv( v);
}
inline void glRasterPos2r( GLdouble x, GLdouble y) {
			glRasterPos2d( x, y);
}
inline void glRasterPos2rv( const GLdouble *v) {
			glRasterPos2dv( v);
}
inline void glRasterPos2r( GLfloat x, GLfloat y) {
			glRasterPos2f( x, y);
}
inline void glRasterPos2rv( const GLfloat *v) {
			glRasterPos2fv ( v);
}
inline void glRasterPos3r( GLdouble x, GLdouble y, GLdouble z) {
			glRasterPos3d( x, y, z);
}
inline void glRasterPos3rv( const GLdouble *v) {
			glRasterPos3dv( v);
}
inline void glRasterPos3r( GLfloat x, GLfloat y, GLfloat z) {
			glRasterPos3f( x, y, z);
}
inline void glRasterPos3rv( const GLfloat *v) {
			glRasterPos3fv( v);
}
inline void glRasterPos4r( GLdouble x, GLdouble y, GLdouble z, GLdouble w) {
			glRasterPos4d( x, y, z, w);
}
inline void glRasterPos4rv( const GLdouble *v) {
			glRasterPos4dv( v);
}
inline void glRasterPos4r( GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
			glRasterPos4f( x, y, z, w);
}
inline void glRasterPos4rv( const GLfloat *v) {
			glRasterPos4fv( v);
}
inline void glRectr( GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2) {
			glRectd ( x1, y1, x2, y2);
}
inline void glRectrv( const GLdouble *v1, const GLdouble *v2) {
			glRectdv( v1, v2);
}
inline void glRectr( GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2) {
			glRectf( x1, y1, x2, y2);
}
inline void glRectrv( const GLfloat *v1, const GLfloat *v2) {
			glRectfv( v1, v2);
}
inline void glRotater( GLdouble angle, GLdouble x, GLdouble y, GLdouble z) {
			glRotated( angle, x, y, z);
}
inline void glRotater( GLfloat angle, GLfloat x, GLfloat y, GLfloat z) {
			glRotatef( angle, x, y, z);
}
inline void glScaler( GLdouble x, GLdouble y, GLdouble z) {
			glScaled( x, y, z);
}
inline void glScaler( GLfloat x, GLfloat y, GLfloat z) {
			glScalef( x, y, z);
}
inline void glTexCoord1r( GLdouble s) {
			glTexCoord1d( s);
}
inline void glTexCoord1rv( const GLdouble *v) {
			glTexCoord1dv( v);
}
inline void glTexCoord1r( GLfloat s) {
			glTexCoord1f( s);
}
inline void glTexCoord1rv( const GLfloat *v) {
			glTexCoord1fv( v);
}
inline void glTexCoord2r( GLdouble s, GLdouble t) {
			glTexCoord2d( s, t);
}
inline void glTexCoord2rv( const GLdouble *v) {
			glTexCoord2dv( v);
}
inline void glTexCoord2r( GLfloat s, GLfloat t) {
			glTexCoord2f( s, t);
}
inline void glTexCoord2rv( const GLfloat *v) {
			glTexCoord2fv( v);
}
inline void glTexCoord3r( GLdouble s, GLdouble t, GLdouble r) {
			glTexCoord3d( s, t, r);
}
inline void glTexCoord3rv( const GLdouble *v) {
			glTexCoord3dv( v);
}
inline void glTexCoord3r( GLfloat s, GLfloat t, GLfloat r) {
			glTexCoord3f( s, t, r);
}
inline void glTexCoord3rv( const GLfloat *v) {
			glTexCoord3fv( v);
}
inline void glTexCoord4r( GLdouble s, GLdouble t, GLdouble r, GLdouble q) {
			glTexCoord4d( s, t, r, q);
}
inline void glTexCoord4rv( const GLdouble *v) {
			glTexCoord4dv( v);
}
inline void glTexCoord4r( GLfloat s, GLfloat t, GLfloat r, GLfloat q) {
			glTexCoord4f( s, t, r, q);
}
inline void glTexCoord4rv( const GLfloat *v) {
			glTexCoord4fv( v);
}
inline void glTexGenr( GLenum coord, GLenum pname, GLdouble param) {
			glTexGend( coord, pname, param);
}
inline void glTexGenrv( GLenum coord, GLenum pname, const GLdouble *params) {
			glTexGendv( coord, pname,  params);
}
inline void glTexGenr( GLenum coord, GLenum pname, GLfloat param) {
			glTexGenf( coord, pname, param);
}
inline void glTexGenrv( GLenum coord, GLenum pname, const GLfloat *params) {
			glTexGenfv( coord, pname, params);
}
inline void glTranslater( GLdouble x, GLdouble y, GLdouble z) {
			glTranslated( x, y, z);
}
inline void glTranslater( GLfloat x, GLfloat y, GLfloat z) {
			glTranslatef( x, y, z);
}
inline void glVertex2r( GLdouble x, GLdouble y) {
			glVertex2d( x, y);
}
inline void glVertex2rv( const GLdouble *v) {
			glVertex2dv( v);
}
inline void glVertex2r( GLfloat x, GLfloat y) {
			glVertex2f( x, y);
}
inline void glVertex2rv( const GLfloat *v) {
			glVertex2fv( v);
}
inline void glVertex3r( GLdouble x, GLdouble y, GLdouble z) {
			glVertex3d( x, y, z);
}
inline void glVertex3rv( const GLdouble *v) {
			glVertex3dv( v);
}
inline void glVertex3r( GLfloat x, GLfloat y, GLfloat z) {
			glVertex3f( x, y, z);
}
inline void glVertex3rv( const GLfloat *v) {
			glVertex3fv( v);
}
inline void glVertex4r( GLdouble x, GLdouble y, GLdouble z, GLdouble w) {
			glVertex4d( x, y, z, w);
}
inline void glVertex4rv( const GLdouble *v) {
			glVertex4dv( v);
}
inline void glVertex4r( GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
			glVertex4f( x, y, z, w);
}
inline void glVertex4rv( const GLfloat *v) {
			glVertex4fv( v);
}
