//
//  glPlatform.h
//  MonkVG-iOS
//
//  Created by Micah Pearlman on 8/6/11.
//  Copyright 2011 Zero Vision. All rights reserved.
//

#ifndef MonkVG_iOS_glPlatform_h
#define MonkVG_iOS_glPlatform_h

#define PLATFORM_IS_GLES 1
#define GL_GLEXT_PROTOTYPES 1

#if __APPLE__
#include <OpenGLES/ES2/gl.h>
#else
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif
#include "glu.h"
typedef float GLdouble;
#ifndef APIENTRY
    #define APIENTRY
#endif // APIENTRY

#endif
