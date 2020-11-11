/**
@file			JCMatrix44.cpp
@brief			
@author			James
@version		1.0
@date			2016_10_31
*/

#include "JCMatrix44.h"
#include<cstring>

namespace laya
{
    static void multiply(const JCMatrix44 &a, const JCMatrix44& b, JCMatrix44 &c)
    {
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                c[i][j] = a[i][0] * b[0][j] + a[i][1] * b[1][j] +
                    a[i][2] * b[2][j] + a[i][3] * b[3][j];
            }
        }
    }
    JCMatrix44 JCMatrix44::operator * (const JCMatrix44& v) const
    {
        JCMatrix44 tmp;
        multiply(*this, v, tmp);

        return tmp;
    }
    bool JCMatrix44::operator==(JCMatrix44 const & m1)
    {
        return memcmp(&this->value[0][0], &m1.value[0][0], 16 * sizeof(float)) == 0;
    }
    JCMatrix44& JCMatrix44::operator=(JCMatrix44 const & m)
    {
        memcpy(&this->value, &m.value, 16 * sizeof(float));
        return *this;
    }
    
}

//-----------------------------END FILE--------------------------------