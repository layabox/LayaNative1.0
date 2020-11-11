/**
@file			JCMatrix44.h
@brief
@author			James
@version		1.0
@date			2016_10_31
*/

#ifndef __JCMatrix44_H__
#define __JCMatrix44_H__

namespace laya
{
    /**
    * @brief
    */
    class JCMatrix44
    {
    public:
        JCMatrix44() {}
        JCMatrix44 operator * (const JCMatrix44& v) const;
        const float* operator [] (int i) const { return value[i]; }
        float* operator [] (int i) { return value[i]; }
        JCMatrix44& operator=(JCMatrix44 const & m);
        bool operator==(JCMatrix44 const & m1);
    public:
        float value[4][4] = { { 1.0f,0.0f,0.0f,0.0f },{ 0.0f,1.0f,0.0f,0.0f },{ 0.0f,0.0f,1.0f,0.0f },{ 0.0f,0.0f,0.0f,1 } };
    };
}

#endif //__JCMatrix44_H__

//-----------------------------END FILE--------------------------------
