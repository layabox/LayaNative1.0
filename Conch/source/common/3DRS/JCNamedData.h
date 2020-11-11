/**
@file           JCNamedData.h
@brief          
@author         guo
@version        1.0
@date           2016_5_27
*/

#ifndef __JCNamedData_H__
#define __JCNamedData_H__
#include <string>
#include <vector>
#include "../util/Log.h"

namespace laya{
    class JCNamedData;
    /** 
     * 根据 JCNamedData 创建的一个实例对象。内存结构符合JCNamedData的描述。可以根据名字设置对应的变量的值。
     */
    class JCNamedDataInst {
    public:
        char* m_pData = nullptr;
        JCNamedData* m_pNamedData = nullptr;
        ~JCNamedDataInst() {
            if (m_pData)
                delete[] m_pData;
        }
        void create(JCNamedData* pNamedData);
        bool setData(const char* pName, char* pValue, int len);
        void setData(int idx, char* pValue, int len);
    };

    class JCNamedData
    {
    public:
        enum type{
            tp_unknown=0,
            tp_int8=1,
            tp_uint8=2,
            tp_int16=3,
            tp_uint16=4,
            tp_int32=5,
            tp_uint32=6,
            tp_f32=7,
            tp_f64=8,
            tp_fvec2=9,
            tp_fvec3=10,
            tp_fvec4=11,
            tp_mat2=12,
            tp_mat3=13,
            tp_mat4=14,
            tp_ptr=15,      //TODO 如果把这个做上的话，可以不要求内存是连续的。
            tp_user=16,    //自定义的都要大于这个
        };
        struct dataDesc
        {
            dataDesc(unsigned short nName,unsigned short nOffset,unsigned short nType,unsigned short nNum, unsigned int nLen = 0)
            {
                m_nName = nName;
                m_nOffset = nOffset;
                m_nType = nType;
                m_nNum = nNum;
                m_nLen = nLen;
            }
            unsigned short m_nName;        //名字所在位置
            unsigned short m_nOffset;
            unsigned short m_nType;        //如果需要考虑自定义的话，则需要很多
            unsigned short m_nNum;
            unsigned int  m_nLen;        //如果没有指定类型的话，就要指定长度
        };

        JCNamedData()
        {

        }

        virtual ~JCNamedData()
        {
            for (int i = 0; i < m_AllData.size(); i++)
            {
                if (m_AllData[i])
                {
                    delete m_AllData[i];
                    m_AllData[i] = NULL;
                }
            }
            m_AllData.clear();
        }

        JCNamedData& add(const char* p_pszName, short p_nOffset, short p_nType, short p_nNum, unsigned int len = 0 ) {
            int fpos = get(p_pszName);
            dataDesc* pCData = nullptr;
            if ( fpos>= 0) {
                printf("已经存在这个变量了[%s]! 下面要修改他\n", p_pszName);
                pCData = m_AllData[fpos];
                pCData->m_nNum = p_nNum;
                pCData->m_nOffset = p_nOffset;
                pCData->m_nType = p_nType;
            }
            else {
                int len = strlen(p_pszName) + 1;
                int curlen = m_pNameBuffer.size();
                m_pNameBuffer.resize(curlen + len);
                memcpy(&m_pNameBuffer[curlen], p_pszName, len);
                pCData = new dataDesc(curlen, p_nOffset, p_nType, p_nNum,len );
                m_AllData.push_back(pCData);
            }
            if (p_nType==tp_unknown) {
                pCData->m_nLen = len;
            }
            else {
                pCData->m_nLen = getSize((type)p_nType, p_nNum);
            }
            return *this;
        }
        JCNamedData& add(int nIndex, short p_nOffset, short p_nType, short p_nNum, unsigned int len = 0) 
        {
            dataDesc* pCData = nullptr;
            if (nIndex == m_AllData.size())
            {
                pCData = new dataDesc(-1, p_nOffset, p_nType, p_nNum,len);
                m_AllData.push_back(pCData);
            }
            else if (nIndex > m_AllData.size())
            {
                m_AllData.resize(nIndex + 1);
                pCData = new dataDesc(-1, p_nOffset, p_nType, p_nNum, len);
                m_AllData[nIndex] = pCData;
            }
            else
            {
                pCData = new dataDesc(-1, p_nOffset, p_nType, p_nNum, len);
                m_AllData[nIndex] = pCData;
            }
            if (p_nType == tp_unknown) {
                pCData->m_nLen = len;
            }
            else {
                pCData->m_nLen = getSize((type)p_nType, p_nNum);
            }
            return *this;
        }
        //返回-1则没有
        int get(const char* p_pszName ){
            for (int i = 0, sz = m_AllData.size(); i < sz; i++) {
                if (m_AllData[i])
                {
                    if ( m_AllData[i]->m_nName != (unsigned short)(-1) && strcmp(p_pszName, &m_pNameBuffer[m_AllData[i]->m_nName]) == 0) {
                        return i;
                    }
                }
            }
            return -1;
        }
        char* createBuffer() {
            return new char[getNamedDataSize()];
        }
        int getNamedDataSize() {
            int sz = m_AllData.size();
            if (sz <= 0)return 0;
            dataDesc* pLast = m_AllData[ sz- 1];
            return pLast->m_nOffset + pLast->m_nLen;
        }
        JCNamedDataInst* createInst() {
            JCNamedDataInst* pInst = new JCNamedDataInst();
            pInst->create(this);
            return pInst;
        }
        void setData(int idx, char* pData, char* pValue, int len) {
            dataDesc* pDesc = m_AllData[idx];
#ifdef _DEBUG
            if (pDesc->m_nLen < len) {
                *(int*)0 = 1;
            }
#endif
            int* pDst = (int*)(pData+ pDesc->m_nOffset);
            int* pSrc = (int*)pValue;
            switch(len){
            case 16:*pDst++ = *pSrc++;
            case 12:*pDst++ = *pSrc++;
            case 8:*pDst++ = *pSrc++;
            case 4:*pDst++ = *pSrc++;
                break;
            default:
                memcpy(pData + pDesc->m_nOffset, pValue, len);
                break;
            }
        }
        bool setData(const char* pName, char* pData, char* pValue, int len) {
            int idx = get(pName);
            if (idx < 0)
                return false;
            setData(idx, pData, pValue, len);
            return true;
        }

        int getSize(type tp, int num) {
            switch (tp)
            {
            case laya::JCNamedData::tp_unknown:
                return 0;
            case laya::JCNamedData::tp_int8:
            case laya::JCNamedData::tp_uint8:
                return num;
            case laya::JCNamedData::tp_int16:
            case laya::JCNamedData::tp_uint16:
                return num * 2;
            case laya::JCNamedData::tp_int32:
            case laya::JCNamedData::tp_uint32:
            case laya::JCNamedData::tp_f32:
                return num * 4;
            case laya::JCNamedData::tp_f64:
            case laya::JCNamedData::tp_fvec2:
                return num * 8;
            case laya::JCNamedData::tp_fvec3:
                return num * 12;
            case laya::JCNamedData::tp_fvec4:
            case laya::JCNamedData::tp_mat2:
                return num * 16;
            case laya::JCNamedData::tp_mat3:
                return num * 36;
            case laya::JCNamedData::tp_mat4:
                return num * 64;
            case laya::JCNamedData::tp_ptr:
                return sizeof(intptr_t);
            case laya::JCNamedData::tp_user:
                break;
            default:
                break;
            }
            return 0;
        }
        std::string              m_strClassName;            //相同类的。//20
        std::vector<dataDesc*>   m_AllData;                //4
        std::vector<char>        m_pNameBuffer;            //1
    };
};
#endif //__JCNamedData_H__

