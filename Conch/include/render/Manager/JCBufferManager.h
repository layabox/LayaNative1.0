/**
@file			JCBufferManager.h
@brief			
@author			hugao
@version		1.0
@date			2016_6_11
*/

#ifndef __JCBufferManager_H__
#define __JCBufferManager_H__


//包含头文件
//------------------------------------------------------------------------------
#include <map>
#include <vector>

namespace laya
{
    class JCBufferManager
    {
    public:
        struct BufferContent
        {
            int nLength=0;
            char * buffer = NULL;
            ~BufferContent()
            {
                if (buffer)
                {
                    delete [] buffer;
                    buffer = NULL;
                }
            }
        };
        typedef std::map<int, BufferContent*>           MapVertexBuffer;

    public:
        JCBufferManager();
        ~JCBufferManager();
        void createOrUpdateBuffer(int id, int length,char *buffer);
        void deleteBuffer(int id);
        BufferContent* getBufferById(int id);
        void clear();
        void update();
    private:
        void _deleteBuffer(int id);
    private:
        std::vector<int>    m_vDeleteIDs;
        int                 m_nDeleteCount;
        MapVertexBuffer     m_VertexBufferMap;
    };
}
//------------------------------------------------------------------------------


#endif //__JCBufferManager_H__

//-----------------------------END FILE--------------------------------