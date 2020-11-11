/**
@file			JCPersistSemantics.h
@brief			因为RenderObject和3DScene和camera都在C++这边，这个类里面的字符串定义都是C++里面保留的，这些语义是有意义的
@author			James
@version		1.0
@date			2016_12_17
*/

#ifndef __JCPersistSemantics_H__
#define __JCPersistSemantics_H__

#include <stdio.h>
#include <string>

namespace laya
{
    struct JCPersistSemantics
    {
    public:
        static std::string WORLD_MATRIX;
        static std::string MVP_MATRIX;
        static std::string PROJECT_MATRIX;
        static std::string VIEW_MATRIX;
        static std::string VP_MATRIX;
        static std::string VP_MATRIX_NO_TRANSLATE;
        static std::string CAMERA_POS;
        static std::string DEBUG_BOX_COLOR;
    };
};


//------------------------------------------------------------------------------


#endif //__JCPersistSemantics_H__

//-----------------------------END FILE--------------------------------