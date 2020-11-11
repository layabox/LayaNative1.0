/**
@file			JCMaterialManager.h
@brief			
@author			James
@version		1.0
@date			2016_5_30
*/

#ifndef __JCMaterialManager_H__
#define __JCMaterialManager_H__

#include <vector>
#include "JCMaterial.h"

namespace laya
{
    /** 
     * @brief 
    */
    class JCMaterialManager
    {
    public:

        /** @brief构造函数
        */
        JCMaterialManager();

        /** @brief析构函数
        */
        ~JCMaterialManager();

        /** @brief 返回material指针 
         *  @return 
        */
        JCMaterial* getMaterial();

        void reset();

    protected:

        std::vector<JCMaterial*>    m_vMaterials;
        int                         m_nCurrentPos;
    };
}

#endif //__JCMaterialManager_H__

//-----------------------------END FILE--------------------------------
