/*
 * Copyright (C) 1997-2001 Id Software, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.
 *
 * =======================================================================
 *
 * This file implements the operating system binding of GL to QGL function
 * pointers. When doing a port of Quake2 you must implement the following
 * two functions:
 *
 * QGL_Init() - loads libraries, assigns function pointers, etc.
 * QGL_Shutdown() - unloads libraries, NULLs function pointers
 *
 * =======================================================================
 */

#if !defined(QGL_DIRECT_LINK)

#include <dlfcn.h>

#include "../refresh/header/local.h"
#include "header/glwindow.h"

void ( APIENTRY *qglAlphaFunc )( GLenum func, GLclampf ref );
void ( APIENTRY *qglBindTexture )( GLenum target, GLuint texture );
void ( APIENTRY *qglBlendFunc )( GLenum sfactor, GLenum dfactor );
void ( APIENTRY *qglClear )( GLbitfield mask );
void ( APIENTRY *qglClearColor )( GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha );
void ( APIENTRY *qglClearStencil )( GLint s );
void ( APIENTRY *qglColor4f )( GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha );
void ( APIENTRY *qglColorPointer )( GLint size, GLenum type, GLsizei stride, const GLvoid *pointer );
void ( APIENTRY *qglCullFace )( GLenum mode );
void ( APIENTRY *qglDeleteTextures )( GLsizei n, const GLuint *textures );
void ( APIENTRY *qglDepthFunc )( GLenum func );
void ( APIENTRY *qglDepthMask )( GLboolean flag );
void ( APIENTRY *qglDepthRange )( GLclampd zNear, GLclampd zFar );
void ( APIENTRY *qglDisable )( GLenum cap );
void ( APIENTRY *qglDisableClientState )( GLenum array );
void ( APIENTRY *qglDrawArrays )( GLenum mode, GLint first, GLsizei count );
void ( APIENTRY *qglEnable )( GLenum cap );
void ( APIENTRY *qglEnableClientState )( GLenum array );
void ( APIENTRY *qglFinish )( void );
void ( APIENTRY *qglFrustum )( GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar );
GLenum ( APIENTRY *qglGetError )( void );
void ( APIENTRY *qglGetFloatv )( GLenum pname, GLfloat *params );
const GLubyte * ( APIENTRY * qglGetString )(GLenum name);
void ( APIENTRY *qglLoadIdentity )( void );
void ( APIENTRY *qglLoadMatrixf )( const GLfloat *m );
void ( APIENTRY *qglMatrixMode )( GLenum mode );
void ( APIENTRY *qglOrtho )( GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar );
void ( APIENTRY *qglPointSize )( GLfloat size );
void ( APIENTRY *qglPopMatrix )( void );
void ( APIENTRY *qglPushMatrix )( void );
void ( APIENTRY *qglReadPixels )( GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels );
void ( APIENTRY *qglRotatef )( GLfloat angle, GLfloat x, GLfloat y, GLfloat z );
void ( APIENTRY *qglScalef )( GLfloat x, GLfloat y, GLfloat z );
void ( APIENTRY *qglScissor )( GLint x, GLint y, GLsizei width, GLsizei height );
void ( APIENTRY *qglShadeModel )( GLenum mode );
void ( APIENTRY *qglStencilFunc )( GLenum func, GLint ref, GLuint mask );
void ( APIENTRY *qglStencilOp )( GLenum fail, GLenum zfail, GLenum zpass );
void ( APIENTRY *qglTexCoordPointer )( GLint size, GLenum type, GLsizei stride, const GLvoid *pointer );
void ( APIENTRY *qglTexEnvf )( GLenum target, GLenum pname, GLfloat param );
void ( APIENTRY *qglTexEnvi )( GLenum target, GLenum pname, GLint param );
void ( APIENTRY *qglTexImage2D )( GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border,
		GLenum format, GLenum type, const GLvoid *pixels );
void ( APIENTRY *qglTexParameterf )( GLenum target, GLenum pname, GLfloat param );
void ( APIENTRY *qglTexSubImage2D )( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height,
		GLenum format, GLenum type, const GLvoid *pixels );
void ( APIENTRY *qglTranslatef )( GLfloat x, GLfloat y, GLfloat z );
void ( APIENTRY *qglVertexPointer )( GLint size, GLenum type, GLsizei stride, const GLvoid *pointer );
void ( APIENTRY *qglViewport )( GLint x, GLint y, GLsizei width, GLsizei height );
void ( APIENTRY *qglPointParameterfEXT )( GLenum param, GLfloat value );
void ( APIENTRY *qglPointParameterfvEXT )( GLenum param, const GLfloat *value );
void ( APIENTRY *qglColorTableEXT )( GLenum, GLenum, GLsizei, GLenum, GLenum, const GLvoid * );
void ( APIENTRY *qglSelectTextureSGIS )( GLenum );
void ( APIENTRY *qglMTexCoord2fSGIS )( GLenum, GLfloat, GLfloat );
void ( APIENTRY *qglActiveTextureARB )( GLenum texture );
void ( APIENTRY *qglClientActiveTextureARB )( GLenum texture );
static void ( APIENTRY *dllAlphaFunc )( GLenum func, GLclampf ref );
static void ( APIENTRY *dllBindTexture )( GLenum target, GLuint texture );
static void ( APIENTRY *dllBlendFunc )( GLenum sfactor, GLenum dfactor );
static void ( APIENTRY *dllClear )( GLbitfield mask );
static void ( APIENTRY *dllClearColor )( GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha );
static void ( APIENTRY *dllClearStencil )( GLint s );
static void ( APIENTRY *dllColor4f )( GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha );
static void ( APIENTRY *dllColorPointer )( GLint size, GLenum type, GLsizei stride, const GLvoid *pointer );
static void ( APIENTRY *dllCullFace )( GLenum mode );
static void ( APIENTRY *dllDeleteTextures )( GLsizei n, const GLuint *textures );
static void ( APIENTRY *dllDepthFunc )( GLenum func );
static void ( APIENTRY *dllDepthMask )( GLboolean flag );
static void ( APIENTRY *dllDepthRange )( GLclampd zNear, GLclampd zFar );
static void ( APIENTRY *dllDisable )( GLenum cap );
static void ( APIENTRY *dllDisableClientState )( GLenum array );
static void ( APIENTRY *dllDrawArrays )( GLenum mode, GLint first, GLsizei count );
static void ( APIENTRY *dllEnable )( GLenum cap );
static void ( APIENTRY *dllEnableClientState )( GLenum array );
static void ( APIENTRY *dllFinish )( void );
static void ( APIENTRY *dllFrustum )( GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar );
GLenum ( APIENTRY *dllGetError )( void );
static void ( APIENTRY *dllGetFloatv )( GLenum pname, GLfloat *params );
const GLubyte * ( APIENTRY * dllGetString )(GLenum name);
static void ( APIENTRY *dllLoadIdentity )( void );
static void ( APIENTRY *dllLoadMatrixf )( const GLfloat *m );
static void ( APIENTRY *dllMatrixMode )( GLenum mode );
static void ( APIENTRY *dllOrtho )( GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar );
static void ( APIENTRY *dllPointSize )( GLfloat size );
static void ( APIENTRY *dllPopMatrix )( void );
static void ( APIENTRY *dllPushMatrix )( void );
static void ( APIENTRY *dllReadPixels )( GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type,
		GLvoid *pixels );
static void ( APIENTRY *dllRotatef )( GLfloat angle, GLfloat x, GLfloat y, GLfloat z );
static void ( APIENTRY *dllScalef )( GLfloat x, GLfloat y, GLfloat z );
static void ( APIENTRY *dllScissor )( GLint x, GLint y, GLsizei width, GLsizei height );
static void ( APIENTRY *dllShadeModel )( GLenum mode );
static void ( APIENTRY *dllStencilFunc )( GLenum func, GLint ref, GLuint mask );
static void ( APIENTRY *dllStencilOp )( GLenum fail, GLenum zfail, GLenum zpass );
static void ( APIENTRY *dllTexCoordPointer )( GLint size, GLenum type, GLsizei stride, const GLvoid *pointer );
static void ( APIENTRY *dllTexEnvf )( GLenum target, GLenum pname, GLfloat param );
static void ( APIENTRY *dllTexEnvi )( GLenum target, GLenum pname, GLint param );
static void ( APIENTRY *dllTexImage2D )( GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height,
		GLint border, GLenum format, GLenum type, const GLvoid *pixels );
static void ( APIENTRY *dllTexParameterf )( GLenum target, GLenum pname, GLfloat param );
static void ( APIENTRY *dllTexSubImage2D )( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width,
		GLsizei height, GLenum format, GLenum type, const GLvoid *pixels );
static void ( APIENTRY *dllTranslatef )( GLfloat x, GLfloat y, GLfloat z );
static void ( APIENTRY *dllVertexPointer )( GLint size, GLenum type, GLsizei stride, const GLvoid *pointer );
static void ( APIENTRY *dllViewport )( GLint x, GLint y, GLsizei width, GLsizei height );

static void ( APIENTRY *dllActiveTextureARB )( GLenum texture );
static void ( APIENTRY *dllClientActiveTextureARB )( GLenum texture );
static void ( APIENTRY *dllPointParameterfEXT )( GLenum param, GLfloat value );
static void ( APIENTRY *dllPointParameterfvEXT )( GLenum param, const GLfloat *value );

static void APIENTRY
logAlphaFunc ( GLenum func, GLclampf ref )
{
	fprintf( glw_state.log_fp, "glAlphaFunc( 0x%x, %f )\n", func, ref );
	dllAlphaFunc( func, ref );
}

static void APIENTRY
logBindTexture ( GLenum target, GLuint texture )
{
	fprintf( glw_state.log_fp, "glBindTexture( 0x%x, %u )\n", target, texture );
	dllBindTexture( target, texture );
}

static void APIENTRY
logBlendFunc ( GLenum sfactor, GLenum dfactor )
{
	fprintf( glw_state.log_fp, "glBlendFunc( 0x%x, 0x%x )\n", sfactor, dfactor );
	dllBlendFunc( sfactor, dfactor );
}

static void APIENTRY
logClear ( GLbitfield mask )
{
	fprintf( glw_state.log_fp, "glClear\n" );
	dllClear( mask );
}

static void APIENTRY
logClearColor ( GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha )
{
	fprintf( glw_state.log_fp, "glClearColor\n" );
	dllClearColor( red, green, blue, alpha );
}

static void APIENTRY
logClearStencil ( GLint s )
{
	fprintf( glw_state.log_fp, "glClearStencil\n" );
	dllClearStencil( s );
}

#define SIG( x ) fprintf( glw_state.log_fp, x "\n" )

static void APIENTRY
logColor4f ( GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha )
{
	fprintf( glw_state.log_fp, "glColor4f( %f,%f,%f,%f )\n", red, green, blue, alpha );
	dllColor4f( red, green, blue, alpha );
}

static void APIENTRY
logColorPointer ( GLint size, GLenum type, GLsizei stride, const void *pointer )
{
	SIG( "glColorPointer" );
	dllColorPointer( size, type, stride, pointer );
}

static void APIENTRY
logCullFace ( GLenum mode )
{
	SIG( "glCullFace" );
	dllCullFace( mode );
}

static void APIENTRY
logDeleteTextures ( GLsizei n, const GLuint *textures )
{
	SIG( "glDeleteTextures" );
	dllDeleteTextures( n, textures );
}
static void APIENTRY

logDepthFunc ( GLenum func )
{
	SIG( "glDepthFunc" );
	dllDepthFunc( func );
}

static void APIENTRY
logDepthMask ( GLboolean flag )
{
	SIG( "glDepthMask" );
	dllDepthMask( flag );
}

static void APIENTRY
logDepthRange ( GLclampd zNear, GLclampd zFar )
{
	SIG( "glDepthRange" );
	dllDepthRange( zNear, zFar );
}

static void APIENTRY
logDisable ( GLenum cap )
{
	fprintf( glw_state.log_fp, "glDisable( 0x%x )\n", cap );
	dllDisable( cap );
}

static void APIENTRY
logDisableClientState ( GLenum array )
{
	SIG( "glDisableClientState" );
	dllDisableClientState( array );
}

static void APIENTRY
logDrawArrays ( GLenum mode, GLint first, GLsizei count )
{
	SIG( "glDrawArrays" );
	dllDrawArrays( mode, first, count );
}

static void APIENTRY
logEnable ( GLenum cap )
{
	fprintf( glw_state.log_fp, "glEnable( 0x%x )\n", cap );
	dllEnable( cap );
}

static void APIENTRY
logEnableClientState ( GLenum array )
{
	SIG( "glEnableClientState" );
	dllEnableClientState( array );
}

static void APIENTRY
logFinish ( void )
{
	SIG( "glFinish" );
	dllFinish();
}

static void APIENTRY
logFrustum ( GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar )
{
	SIG( "glFrustum" );
	dllFrustum( left, right, bottom, top, zNear, zFar );
}

static GLenum APIENTRY
logGetError ( void )
{
	SIG( "glGetError" );
	return ( dllGetError() );
}

static void APIENTRY
logGetFloatv ( GLenum pname, GLfloat *params )
{
	SIG( "glGetFloatv" );
	dllGetFloatv( pname, params );
}

static const GLubyte *APIENTRY
logGetString ( GLenum name )
{
	SIG( "glGetString" );
	return ( dllGetString( name ) );
}

static void APIENTRY
logLoadIdentity ( void )
{
	SIG( "glLoadIdentity" );
	dllLoadIdentity();
}

static void APIENTRY
logLoadMatrixf ( const GLfloat *m )
{
	SIG( "glLoadMatrixf" );
	dllLoadMatrixf( m );
}

static void APIENTRY
logMatrixMode ( GLenum mode )
{
	SIG( "glMatrixMode" );
	dllMatrixMode( mode );
}

static void APIENTRY
logOrtho ( GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar )
{
	SIG( "glOrtho" );
	dllOrtho( left, right, bottom, top, zNear, zFar );
}

static void APIENTRY
logPointSize ( GLfloat size )
{
	SIG( "glPointSize" );
	dllPointSize( size );
}

static void APIENTRY
logPopMatrix ( void )
{
	SIG( "glPopMatrix" );
	dllPopMatrix();
}

static void APIENTRY
logPushMatrix ( void )
{
	SIG( "glPushMatrix" );
	dllPushMatrix();
}

static void APIENTRY
logReadPixels ( GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels )
{
	SIG( "glReadPixels" );
	dllReadPixels( x, y, width, height, format, type, pixels );
}

static void APIENTRY
logRotatef ( GLfloat angle, GLfloat x, GLfloat y, GLfloat z )
{
	SIG( "glRotatef" );
	dllRotatef( angle, x, y, z );
}

static void APIENTRY
logScalef ( GLfloat x, GLfloat y, GLfloat z )
{
	SIG( "glScalef" );
	dllScalef( x, y, z );
}

static void APIENTRY
logScissor ( GLint x, GLint y, GLsizei width, GLsizei height )
{
	SIG( "glScissor" );
	dllScissor( x, y, width, height );
}

static void APIENTRY
logShadeModel ( GLenum mode )
{
	SIG( "glShadeModel" );
	dllShadeModel( mode );
}

static void APIENTRY
logStencilFunc ( GLenum func, GLint ref, GLuint mask )
{
	SIG( "glStencilFunc" );
	dllStencilFunc( func, ref, mask );
}

static void APIENTRY
logStencilOp ( GLenum fail, GLenum zfail, GLenum zpass )
{
	SIG( "glStencilOp" );
	dllStencilOp( fail, zfail, zpass );
}

static void APIENTRY
logTexCoordPointer ( GLint size, GLenum type, GLsizei stride, const void *pointer )
{
	SIG( "glTexCoordPointer" );
	dllTexCoordPointer( size, type, stride, pointer );
}

static void APIENTRY
logTexEnvf ( GLenum target, GLenum pname, GLfloat param )
{
	fprintf( glw_state.log_fp, "glTexEnvf( 0x%x, 0x%x, %f )\n", target, pname, param );
	dllTexEnvf( target, pname, param );
}

static void APIENTRY
logTexEnvi ( GLenum target, GLenum pname, GLint param )
{
	fprintf( glw_state.log_fp, "glTexEnvi( 0x%x, 0x%x, 0x%x )\n", target, pname, param );
	dllTexEnvi( target, pname, param );
}

static void APIENTRY
logTexImage2D ( GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format,
		GLenum type,
		const void *pixels )
{
	SIG( "glTexImage2D" );
	dllTexImage2D( target, level, internalformat, width, height, border, format, type, pixels );
}

static void APIENTRY
logTexParameterf ( GLenum target, GLenum pname, GLfloat param )
{
	fprintf( glw_state.log_fp, "glTexParameterf( 0x%x, 0x%x, %f )\n", target, pname, param );
	dllTexParameterf( target, pname, param );
}

static void APIENTRY
logTexSubImage2D ( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format,
		GLenum type,
		const void *pixels )
{
	SIG( "glTexSubImage2D" );
	dllTexSubImage2D( target, level, xoffset, yoffset, width, height, format, type, pixels );
}

static void APIENTRY
logTranslatef ( GLfloat x, GLfloat y, GLfloat z )
{
	SIG( "glTranslatef" );
	dllTranslatef( x, y, z );
}

static void APIENTRY
logVertexPointer ( GLint size, GLenum type, GLsizei stride, const void *pointer )
{
	SIG( "glVertexPointer" );
	dllVertexPointer( size, type, stride, pointer );
}

static void APIENTRY
logViewport ( GLint x, GLint y, GLsizei width, GLsizei height )
{
	SIG( "glViewport" );
	dllViewport( x, y, width, height );
}

static void APIENTRY
logActiveTextureARB ( GLenum texture )
{
	SIG( "glActiveTexture" );
	dllActiveTextureARB( texture );
}

static void APIENTRY
logClientActiveTextureARB ( GLenum texture )
{
	SIG( "glClientActiveTexture" );
	dllClientActiveTextureARB( texture );
}

static void APIENTRY
logPointParameterfEXT ( GLenum param, GLfloat value )
{
	SIG( "glPointParameterf" );
	dllPointParameterfEXT( param, value );
}

static void APIENTRY
logPointParameterfvEXT ( GLenum param, const GLfloat *value )
{
	SIG( "glPointParameterfv" );
	dllPointParameterfvEXT( param, value );
}

/*
 * Unloads the specified DLL then nulls out all the proc pointers.
 */
void
QGL_Shutdown ( void )
{
	if ( glw_state.OpenGLLib )
	{
		dlclose( glw_state.OpenGLLib );
		glw_state.OpenGLLib = NULL;
	}

	glw_state.OpenGLLib = NULL;

	qglAlphaFunc                 = NULL;
	qglBindTexture               = NULL;
	qglBlendFunc                 = NULL;
	qglClear                     = NULL;
	qglClearColor                = NULL;
	qglClearStencil              = NULL;
	qglColor4f                   = NULL;
	qglColorPointer              = NULL;
	qglCullFace                  = NULL;
	qglDeleteTextures            = NULL;
	qglDepthFunc                 = NULL;
	qglDepthMask                 = NULL;
	qglDepthRange                = NULL;
	qglDisable                   = NULL;
	qglDisableClientState        = NULL;
	qglDrawArrays                = NULL;
	qglEnable                    = NULL;
	qglEnableClientState         = NULL;
	qglFinish                    = NULL;
	qglFrustum                   = NULL;
	qglGetError                  = NULL;
	qglGetFloatv                 = NULL;
	qglGetString                 = NULL;
	qglLoadIdentity              = NULL;
	qglLoadMatrixf               = NULL;
	qglMatrixMode                = NULL;
	qglOrtho                     = NULL;
	qglPointSize                 = NULL;
	qglPopMatrix                 = NULL;
	qglPushMatrix                = NULL;
	qglReadPixels                = NULL;
	qglRotatef                   = NULL;
	qglScalef                    = NULL;
	qglScissor                   = NULL;
	qglShadeModel                = NULL;
	qglStencilFunc               = NULL;
	qglStencilOp                 = NULL;
	qglTexCoordPointer           = NULL;
	qglTexEnvf                   = NULL;
	qglTexEnvi                   = NULL;
	qglTexImage2D                = NULL;
	qglTexParameterf             = NULL;
	qglTexSubImage2D             = NULL;
	qglTranslatef                = NULL;
	qglVertexPointer             = NULL;
	qglViewport                  = NULL;
	qglActiveTextureARB          = NULL;
	qglClientActiveTextureARB    = NULL;
    qglPointParameterfEXT        = NULL;
    qglPointParameterfvEXT       = NULL;
}

#define GPA( a ) dlsym( glw_state.OpenGLLib, a )

void *
qwglGetProcAddress ( char *symbol )
{
	if ( glw_state.OpenGLLib )
	{
		return ( GPA( symbol ) );
	}

	return ( NULL );
}

/*
 * This is responsible for binding our qgl function pointers to
 * the appropriate GL stuff.  In Windows this means doing a
 * LoadLibrary and a bunch of calls to GetProcAddress.  On other
 * operating systems we need to do the right thing, whatever that
 * might be.
 */
qboolean
QGL_Init ( const char *dllname )
{
	if ( glw_state.OpenGLLib )
	{
		QGL_Shutdown();
	}

	if ( ( glw_state.OpenGLLib = dlopen( dllname, RTLD_LAZY ) ) == 0 )
	{
		char fn [ MAX_OSPATH ];
		char    *path;

		/* try basedir next */
		path = ri.Cvar_Get( "basedir", ".", CVAR_NOSET )->string;

		snprintf( fn, MAX_OSPATH, "%s/%s", path, dllname );

		if ( ( glw_state.OpenGLLib = dlopen( fn, RTLD_LAZY ) ) == 0 )
		{
			ri.Con_Printf( PRINT_ALL, "%s\n", dlerror() );
			return ( false );
		}

		Com_Printf( "Using %s for OpenGL.\n", fn );
	}
	else
	{
		Com_Printf( "Using %s for OpenGL.\n", dllname );
	}

	qglAlphaFunc                 = dllAlphaFunc = GPA( "glAlphaFunc" );
	qglBindTexture               = dllBindTexture = GPA( "glBindTexture" );
	qglBlendFunc                 = dllBlendFunc = GPA( "glBlendFunc" );
	qglClear                     = dllClear = GPA( "glClear" );
	qglClearColor                = dllClearColor = GPA( "glClearColor" );
	qglClearStencil              = dllClearStencil = GPA( "glClearStencil" );
	qglColor4f                   = dllColor4f = GPA( "glColor4f" );
	qglColorPointer              = dllColorPointer = GPA( "glColorPointer" );
	qglCullFace                  = dllCullFace = GPA( "glCullFace" );
	qglDeleteTextures            = dllDeleteTextures = GPA( "glDeleteTextures" );
	qglDepthFunc                 = dllDepthFunc = GPA( "glDepthFunc" );
	qglDepthMask                 = dllDepthMask = GPA( "glDepthMask" );
#if defined(GLES_ONLY)
	qglDepthRange                = dllDepthRange = GPA( "glDepthRangef" );
#else
	qglDepthRange                = dllDepthRange = GPA( "glDepthRange" );
#endif
	qglDisable                   = dllDisable = GPA( "glDisable" );
	qglDisableClientState        = dllDisableClientState = GPA( "glDisableClientState" );
	qglDrawArrays                = dllDrawArrays = GPA( "glDrawArrays" );
	qglEnable                    =  dllEnable                    = GPA( "glEnable" );
	qglEnableClientState         =  dllEnableClientState         = GPA( "glEnableClientState" );
	qglFinish                    =  dllFinish                    = GPA( "glFinish" );
#if defined(GLES_ONLY)
	qglFrustum                   =  dllFrustum                   = GPA( "glFrustumf" );
#else
	qglFrustum                   =  dllFrustum                   = GPA( "glFrustum" );
#endif
	qglGetError                  =  dllGetError                  = GPA( "glGetError" );
	qglGetFloatv                 =  dllGetFloatv                 = GPA( "glGetFloatv" );
	qglGetString                 =  dllGetString                 = GPA( "glGetString" );
	qglLoadIdentity              =  dllLoadIdentity              = GPA( "glLoadIdentity" );
	qglLoadMatrixf               =  dllLoadMatrixf               = GPA( "glLoadMatrixf" );
	qglMatrixMode                =  dllMatrixMode                = GPA( "glMatrixMode" );
#if defined(GLES_ONLY)
	qglOrtho                     =  dllOrtho                     = GPA( "glOrthof" );
#else
	qglOrtho                     =  dllOrtho                     = GPA( "glOrtho" );
#endif
	qglPointSize                 =  dllPointSize                 = GPA( "glPointSize" );
	qglPopMatrix                 =  dllPopMatrix                 = GPA( "glPopMatrix" );
	qglPushMatrix                =  dllPushMatrix                = GPA( "glPushMatrix" );
	qglReadPixels                =  dllReadPixels                = GPA( "glReadPixels" );
	qglRotatef                   =  dllRotatef                   = GPA( "glRotatef" );
	qglScalef                    =  dllScalef                    = GPA( "glScalef" );
	qglScissor                   =  dllScissor                   = GPA( "glScissor" );
	qglShadeModel                =  dllShadeModel                = GPA( "glShadeModel" );
	qglStencilFunc               =  dllStencilFunc               = GPA( "glStencilFunc" );
	qglStencilOp                 =  dllStencilOp                 = GPA( "glStencilOp" );
	qglTexCoordPointer           =  dllTexCoordPointer           = GPA( "glTexCoordPointer" );
	qglTexEnvf                   =  dllTexEnvf                   = GPA( "glTexEnvf" );
	qglTexEnvi                   =  dllTexEnvi                   = GPA( "glTexEnvi" );
	qglTexImage2D                =  dllTexImage2D                = GPA( "glTexImage2D" );
	qglTexParameterf             =  dllTexParameterf             = GPA( "glTexParameterf" );
	qglTexSubImage2D             =  dllTexSubImage2D             = GPA( "glTexSubImage2D" );
	qglTranslatef                =  dllTranslatef                = GPA( "glTranslatef" );
	qglVertexPointer             =  dllVertexPointer             = GPA( "glVertexPointer" );
	qglViewport                  =  dllViewport                  = GPA( "glViewport" );
	qglActiveTextureARB          =  dllActiveTextureARB          = GPA( "glActiveTexture" );
	qglClientActiveTextureARB    =  dllClientActiveTextureARB    = GPA( "glClientActiveTexture" );
    qglPointParameterfEXT        =  dllPointParameterfEXT        = GPA( "glPointParameterf" );
    qglPointParameterfvEXT       =  dllPointParameterfvEXT       = GPA( "glPointParameterfv" );
	qglColorTableEXT = 0;
	qglSelectTextureSGIS = 0;
	qglMTexCoord2fSGIS = 0;
	return ( true );
}

void
GLimp_EnableLogging ( qboolean enable )
{
	if ( enable )
	{
		if ( !glw_state.log_fp )
		{
			struct tm *newtime;
			time_t aclock;
			char buffer [ 1024 ];

			time( &aclock );
			newtime = localtime( &aclock );

			asctime( newtime );

			Com_sprintf( buffer, sizeof ( buffer ), "%s/gl.log", ri.FS_Gamedir() );
			glw_state.log_fp = fopen( buffer, "wt" );

			fprintf( glw_state.log_fp, "%s\n", asctime( newtime ) );
		}

		qglAlphaFunc                 = logAlphaFunc;
		qglBindTexture               = logBindTexture;
		qglBlendFunc                 = logBlendFunc;
		qglClear                     = logClear;
		qglClearColor                = logClearColor;
		qglClearStencil              = logClearStencil;
		qglColor4f                   = logColor4f;
		qglColorPointer              = logColorPointer;
		qglCullFace                  = logCullFace;
		qglDeleteTextures            = logDeleteTextures;
		qglDepthFunc                 = logDepthFunc;
		qglDepthMask                 = logDepthMask;
		qglDepthRange                = logDepthRange;
		qglDisable                   = logDisable;
		qglDisableClientState        = logDisableClientState;
		qglDrawArrays                = logDrawArrays;
		qglEnable                    =  logEnable;
		qglEnableClientState         =  logEnableClientState;
		qglFinish                    =  logFinish;
		qglFrustum                   =  logFrustum;
		qglGetError                  =  logGetError;
		qglGetFloatv                 =  logGetFloatv;
		qglGetString                 =  logGetString;
		qglLoadIdentity              =  logLoadIdentity;
		qglLoadMatrixf               =  logLoadMatrixf;
		qglMatrixMode                =  logMatrixMode;
		qglOrtho                     =  logOrtho;
		qglPointSize                 =  logPointSize;
		qglPopMatrix                 =  logPopMatrix;
		qglPushMatrix                =  logPushMatrix;
		qglReadPixels                =  logReadPixels;
		qglRotatef                   =  logRotatef;
		qglScalef                    =  logScalef;
		qglScissor                   =  logScissor;
		qglShadeModel                =  logShadeModel;
		qglStencilFunc               =  logStencilFunc;
		qglStencilOp                 =  logStencilOp;
		qglTexCoordPointer           =  logTexCoordPointer;
		qglTexEnvf                   =  logTexEnvf;
		qglTexEnvi                   =  logTexEnvi;
		qglTexImage2D                =  logTexImage2D;
		qglTexParameterf             =  logTexParameterf;
		qglTexSubImage2D             =  logTexSubImage2D;
		qglTranslatef                =  logTranslatef;
		qglVertexPointer             =  logVertexPointer;
		qglViewport                  =  logViewport;
		qglActiveTextureARB          =  logActiveTextureARB;
		qglClientActiveTextureARB    =  logClientActiveTextureARB;
        qglPointParameterfEXT        =  logPointParameterfEXT;
        qglPointParameterfvEXT       =  logPointParameterfvEXT;
	}
	else
	{
		qglAlphaFunc                 = dllAlphaFunc;
		qglBindTexture               = dllBindTexture;
		qglBlendFunc                 = dllBlendFunc;
		qglClear                     = dllClear;
		qglClearColor                = dllClearColor;
		qglClearStencil              = dllClearStencil;
		qglColor4f                   = dllColor4f;
		qglColorPointer              = dllColorPointer;
		qglCullFace                  = dllCullFace;
		qglDeleteTextures            = dllDeleteTextures;
		qglDepthFunc                 = dllDepthFunc;
		qglDepthMask                 = dllDepthMask;
		qglDepthRange                = dllDepthRange;
		qglDisable                   = dllDisable;
		qglDisableClientState        = dllDisableClientState;
		qglDrawArrays                = dllDrawArrays;
		qglEnable                    =  dllEnable;
		qglEnableClientState         =  dllEnableClientState;
		qglFinish                    =  dllFinish;
		qglFrustum                   =  dllFrustum;
		qglGetError                  =  dllGetError;
		qglGetFloatv                 =  dllGetFloatv;
		qglGetString                 =  dllGetString;
		qglLoadIdentity              =  dllLoadIdentity;
		qglLoadMatrixf               =  dllLoadMatrixf;
		qglMatrixMode                =  dllMatrixMode;
		qglOrtho                     =  dllOrtho;
		qglPointSize                 =  dllPointSize;
		qglPopMatrix                 =  dllPopMatrix;
		qglPushMatrix                =  dllPushMatrix;
		qglReadPixels                =  dllReadPixels;
		qglRotatef                   =  dllRotatef;
		qglScalef                    =  dllScalef;
		qglScissor                   =  dllScissor;
		qglShadeModel                =  dllShadeModel;
		qglStencilFunc               =  dllStencilFunc;
		qglStencilOp                 =  dllStencilOp;
		qglTexCoordPointer           =  dllTexCoordPointer;
		qglTexEnvf                   =  dllTexEnvf;
		qglTexEnvi                   =  dllTexEnvi;
		qglTexImage2D                =  dllTexImage2D;
		qglTexParameterf             =  dllTexParameterf;
		qglTexSubImage2D             =  dllTexSubImage2D;
		qglTranslatef                =  dllTranslatef;
		qglVertexPointer             =  dllVertexPointer;
		qglViewport                  =  dllViewport;
		qglActiveTextureARB          =  dllActiveTextureARB;
		qglClientActiveTextureARB    =  dllClientActiveTextureARB;
        qglPointParameterfEXT        =  dllPointParameterfEXT;
        qglPointParameterfvEXT       =  dllPointParameterfvEXT;
	}
}

void
GLimp_LogNewFrame ( void )
{
	fprintf( glw_state.log_fp, "*** R_BeginFrame ***\n" );
}

#endif /* QGL_DIRECT_LINK */

