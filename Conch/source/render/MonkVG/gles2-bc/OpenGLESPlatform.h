//
//  OpenGLESPlatform.h
//  MonkVG-OSX
//
//  Created by Micah Pearlman on 2/3/13.
//
//

#ifndef MonkVG_OSX_OpenGLESPlatform_h
#define MonkVG_OSX_OpenGLESPlatform_h

#define PLATFORM_IS_GLES 1
#define GL_GLEXT_PROTOTYPES 1
#if __APPLE__
#include <OpenGLES/ES2/gl.h>
#else
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

#ifndef APIENTRY
    #define APIENTRY
#endif // APIENTRY



#endif
