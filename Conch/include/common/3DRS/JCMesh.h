/**
@file          JCMesh.h
@brief          
@author        guo
@version       1.0
@date          2016_5_27
*/

#ifndef __JCMesh_H__
#define __JCMesh_H__

#if __APPLE__
    #include <OpenGLES/ES2/gl.h>
    #include <OpenGLES/ES2/glext.h>
#else
    #include <GLES2/gl2.h>
    #include <GLES2/gl2ext.h>
#endif
#include <string>
#include <vector>
#include <map>
#include <assert.h>
#include <functional>
#include "JCShaderDefine.h"

namespace laya
{
    class JCVB;
    class JCIB;
    class JCRenderGroupData;

    /** 
     * @brief 顶点描述。包括每部分的类型，偏移，名称（用来与shader的attribute对应）,所在流。
     * 实际的mesh的顶点总是紧密的，除非是多流
     */
    class JCVertexDesc{
    public:
        struct Desc{
            unsigned short  m_nType=0;
            unsigned short  m_nOffset=0;                //这个是相对于整个buffer的偏移。不是局部的。例如多流的情况下，可能很大。
            unsigned char   m_nStride = 0;              //为了方便，这个也要提供。这样渲染的时候就能直接使用了。
            unsigned char   m_nStreamID=0;              //
            unsigned char   padd = 0;
            unsigned int    m_nSemanticsEnum = 0;
            unsigned char   m_nStrLen = 0;
            std::string     m_strAttribName;
            
            Desc(const char* p_pszName, int p_nType, short p_nOffset, int p_nStride,int p_nStreamID){
                m_nStrLen = strlen(p_pszName);
                m_strAttribName = p_pszName;
                m_nType     = (unsigned short)p_nType;
                m_nOffset   = (unsigned short)p_nOffset;
                m_nStride   = (unsigned char) p_nStride;
                m_nStreamID = (unsigned char)p_nStreamID;
            }
            Desc(int nSemanticsEnum, int p_nType, short p_nOffset, int p_nStride, int p_nStreamID) {
                m_nSemanticsEnum = nSemanticsEnum;
                m_nType = (unsigned short)p_nType;
                m_nOffset = (unsigned short)p_nOffset;
                m_nStride = (unsigned char)p_nStride;
                m_nStreamID = (unsigned char)p_nStreamID;
            }
        };
        /** 
         * @brief 简化版的描述。只描述名字和类型。单流，紧密排列，所以offset可以自己推导。
         */
        struct DescSimp{
            const char* pName;
            int         nType;
            DescSimp(const char* name, int tp) {
                pName = name;
                nType = tp;
            }
        };
        std::vector<Desc>   m_VertDescs;
        int                 m_nStreanNum;
		JCShaderDefine      m_kShaderDefine;

    public:

        JCVertexDesc(){}

		void addShaderDefine(long long nValue);

		void removeShaderDefine(long long nValue);

        JCVertexDesc(std::initializer_list<DescSimp> descs);
	
        Desc* getVertexDesc( const char* p_pszName );

        Desc* getVertexDesc( int nSementicsEnum );
    };

    /** 
     * @brief mesh 
     */
    class JCMesh{
    public:

        JCMesh();

        virtual ~JCMesh();

        virtual void freeGLResource();

        /** 
         *  @brief 注册一个新的顶点格式。调用者自己管理id，因为程序自动计算太麻烦。
         *  @param[in] 
         *  @return 
         */
        void regVertexDesc(const JCVertexDesc& p_Desc, int id);

        void upload();

        int getGroupNum();

    public:

        inline JCVB* getVB(){
            return m_pVB; 
        }
        inline JCIB* getIB(){
            return m_pIB; 
        }
        /**
        *  @brief
        *  TODO 这个返回一个对象，不太好。
        */
        JCVertexDesc getVertexDesc(int id) {
            return m_vAllVertexDesc[id];
        }
        
    public:
        std::map<int,JCVertexDesc>      m_vAllVertexDesc;       //本mesh用到的所有的顶点格式。
        std::vector<JCRenderGroupData>  m_vRenderGroupData;     //renderGroupData是一个每帧都在变的东西
        JCVB*                           m_pVB = nullptr;
        JCIB*                           m_pIB = nullptr;
        int                             m_nVertNum = 0;
        int                             m_nIdxNum = 0;
    };
}
#endif //__JCMesh_H__
