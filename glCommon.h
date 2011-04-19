/*
 *  glCommon.h
 *  VideoGL
 *
 *  Created by Jonathan Saggau on 2/25/11.
 *  Copyright 2011 Sounds Broken inc. All rights reserved.
 *
 */

#ifdef DEBUG 

#define glReportError()\
{\
GLenum error=glGetError();\
if(GL_NO_ERROR!=error)\
{\
NSLog(@"GL error at %s:%d: glError: 0x%04X",__FILE__,__LINE__,error);\
}\
}\

#else
#define glReportError()
#endif