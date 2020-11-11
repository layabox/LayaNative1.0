/**
@file			JCConchBridge.cpp
@brief
@author			James
@version		1.0
@date			2016_5_27
*/

#include "JCConchBridge.h"
#include "../../conch/JCScrpitRuntime.h"
#include "../JSWrapper/LayaWrap/JSLayaContext.h"

namespace laya
{
    void JCConchBridge::getPixelsRenderToJS(unsigned char * pPixels, int nSize, int nID, int w, int h)
    {
        if (JCScriptRuntime::s_JSRT) {
            JCScriptRuntime::s_JSRT->m_ScriptThread.post([pPixels, nSize, nID, w, h]() {
                std::map<int, JSLayaContext*>::iterator it = JSLayaContext::m_sMapContexts.find(nID);
                if (it != JSLayaContext::m_sMapContexts.end()) {
                    it->second->onGetPixelsCallbackCallJS(pPixels, nSize, w , h);
                }
            });
        }
    }
    void JCConchBridge::toBaseRenderToJS(const char* base64, int nID)
    {
        if (JCScriptRuntime::s_JSRT) {
            JCScriptRuntime::s_JSRT->m_ScriptThread.post([base64, nID]() {
                std::map<int, JSLayaContext*>::iterator it = JSLayaContext::m_sMapContexts.find(nID);
                if (it != JSLayaContext::m_sMapContexts.end()) {
                    it->second->onToBase64CallbackCallJS(base64);
                }
            });
        }
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
