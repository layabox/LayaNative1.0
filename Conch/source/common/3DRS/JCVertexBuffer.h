/**
@file			JCVertexBuffer.h
@brief			
@author			guo
@version		1.0
@date			2016_5_31
*/

#ifndef __JCVertexBuffer_H__
#define __JCVertexBuffer_H__

#if __APPLE__
    #include <OpenGLES/ES2/gl.h>
    #include <OpenGLES/ES2/glext.h>
#else
    #include <GLES2/gl2.h>
    #include <GLES2/gl2ext.h>
#endif
#include "../buffer/JCBuffer.h"
#include <string>
#include <vector>

namespace laya{

    /** 
     * @brief vertexBuffer用来管理VB或者IB. 支持多流（只对vb有意义）。
     * 多流的情况下不允许动态改变大小。
     */
    class JCVertexBuffer{
    public:

        JCVertexBuffer();

        ~JCVertexBuffer();
        
        /** 
         *  @brief 创建一个buffer
         *  @param[in] size buffer的大小，单位是byte
         *  @return 成功则返回true
         */
        bool create( int size);

        /** 
         *  @brief 多流的buffer的创建，一次性创建所有的流。
         *  @param[in] p_pStreamSize  每个流的大小。
         *  @param[in] p_nStreamNum 流的个数
         *  @return 成功则返回true。
         */
        bool createMultiStream(int* p_pStreamSize, int p_nStreamNum );
        bool createMultiStream(std::initializer_list<int> streamDescs);

        /** @brief getvbo
         *  @return 返回vbo
        */
        inline unsigned int getVBO(){
            return m_vbo;
        }

        /** @brief 获得内存的buffer
         *  @return JCMemClass指针
        */
        JCMemClass* getMemBuffer(int id){
            return m_pMemBuffer[id]; 
        }

        int getVBLength(){ 
            return m_nVBLength; 
        }

        int getStreamNum(){
            return m_nStreamNum; 
        }

        /** 
         *  @brief 把数据从内存上传到显存。这个函数会影响绑定的vbo。
         *  @param[in] p_nTarget 指定buffer目标
         *  @param[in] p_bForceNewObj true则直接抛弃老的对象，重新创建
         *  @return 
         */
        using FuncUploadTo = void (JCVertexBuffer::*)(int p_nTarget,bool p_bForceNewObj);
        inline void uploadTo(int p_nTarget, bool p_bForceNewObj) {
            (this->*m_uploadTo)(p_nTarget, p_bForceNewObj);
        }

        void freeGLResource();

        void delSysMem();
    protected:
        FuncUploadTo    m_uploadTo = &JCVertexBuffer::_uploadTo1;
        void _uploadTo(int p_nTarget, bool p_bForceNewObj);
        void _uploadTo1(int p_nTarget, bool p_bForceNewObj);    //单流
        //void _uploadTo2(int p_nTarget, bool p_bForceNewObj);    //两个流
    protected:
        //bool                m_bChanged = true;  //任意段发生了改变。
        unsigned int        m_vbo=0;            ///<显存的vbo
        int                 m_nVBLength=0;      ///<当前创建的vbo的实际的大小。如果当前内存buffer的大小超过了实际vbo的大小，需要重新创建新的vbo
        JCMemClass**        m_pMemBuffer=nullptr;   ///<内存数据
        std::vector<int>    m_vStreamStart;     ///<每个流开始的位置。现在多流共享一个实际的buffer，所以需要这里,保存每个流的实际位置。
        unsigned char       m_nStreamNum=0;     ///<流的个数
        int                 m_nTotalSize=0;
    };

    class JCVB :public JCVertexBuffer {
    public:
        void upload(bool bForceNew=false) {
            uploadTo(GL_ARRAY_BUFFER, bForceNew);
        }
    };

    class JCIB :public JCVertexBuffer {
    protected:
        int m_nIndexNum = 0;
    public:
        void upload(bool bForceNew = false) {
            uploadTo(GL_ELEMENT_ARRAY_BUFFER, bForceNew);
        }
        bool createFromArray(unsigned short* p_pIdx, int p_nNum) {
            m_nIndexNum = p_nNum;
            create(p_nNum * 2);
            getMemBuffer(0)->clearData();
            getMemBuffer(0)->append(p_pIdx, p_nNum * 2);
            return true;
        }

        //p_nVertOff 表示所有的index要加上这个值
        void appendData(unsigned short* p_pIdx, int p_nNum, int p_nVertOff);
    };
}
//------------------------------------------------------------------------------


#endif //__JCVertexBuffer_H__

//-----------------------------END FILE--------------------------------