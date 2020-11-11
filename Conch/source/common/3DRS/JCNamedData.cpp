
#include "JCNamedData.h"

namespace laya{
    void JCNamedDataInst::create(JCNamedData* pNamedData) {
        m_pNamedData = pNamedData;
        m_pData = m_pNamedData->createBuffer();
    }
    bool JCNamedDataInst::setData(const char* pName, char* pValue, int len) {
        return m_pNamedData->setData(pName, m_pData, pValue, len);
    }

    void JCNamedDataInst::setData(int idx, char* pValue, int len) {
        m_pNamedData->setData(idx, m_pData, pValue, len);
    }

};


