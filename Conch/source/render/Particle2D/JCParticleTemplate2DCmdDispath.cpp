/**
@file			JCParticleTemplate2DCmdDispath.cpp
@brief			
@author			hugao
@version		1.0
@date			2016_11_8
*/

//------------------------------------------------------------------------------
#include "JCParticleTemplate2DCmdDispath.h"
#include <util/Log.h>
#include <util/JCCmdStruct.h>


//------------------------------------------------------------------------------
namespace laya
{
    JCParticleTemplate2DCmdDispath::JCParticleTemplate2DCmdDispath(int nID)
    {
		m_pParticleTemplate2D = new JCParticleTemplate2D(nID);

    }
	JCParticleTemplate2DCmdDispath::~JCParticleTemplate2DCmdDispath()
    {
		if (m_pParticleTemplate2D != NULL)
		{
			delete m_pParticleTemplate2D;
			m_pParticleTemplate2D = NULL;
		}
    }

	bool JCParticleTemplate2DCmdDispath::dispatchRenderCmd(JCMemClass& pRenderCmd, int nFrameCount)
	{
		static const ProcFunction g_svProcFunctions[] =
		{
			&JCParticleTemplate2DCmdDispath::_rendercmd_setSetting,
			&JCParticleTemplate2DCmdDispath::_rendercmd_setImage,
			&JCParticleTemplate2DCmdDispath::_rendercmd_play,
			&JCParticleTemplate2DCmdDispath::_rendercmd_stop,
		};
		static const int nFuncs = sizeof(g_svProcFunctions) / sizeof(g_svProcFunctions[0]);
		char* pData = pRenderCmd.getReadPtr();
		if (pData)
		{
			int nFunctionID = *(int*)pData;
			if (nFunctionID >= 0 && nFunctionID < nFuncs)
			{
				(this->*g_svProcFunctions[nFunctionID])(pRenderCmd);
			}
			else
			{
				LOGE("JCParticleTemplate2DCmdDispath::dispatchRenderCmd Wrong command number!,cmd=%d", nFunctionID);
			}
		}
		return true;
	}
	
	bool JCParticleTemplate2DCmdDispath::dispatchRenderCmdWhenLostedDevice(JCMemClass& pRenderCmd, int nFrameCount)
	{
		return dispatchRenderCmd(pRenderCmd, nFrameCount);
	}

	void JCParticleTemplate2DCmdDispath::_rendercmd_setSetting(JCMemClass&cmdBuffer)
	{
		CMDInt* cmdInt= cmdBuffer.popp<CMDInt>();
		char * pbuffer = cmdBuffer.readBuffer(cmdInt->i);
		m_pParticleTemplate2D->initSetting(pbuffer);
	}

	void JCParticleTemplate2DCmdDispath::_rendercmd_setImage(JCMemClass& cmdBuffer)
	{
		CMDInt * cmdInt = cmdBuffer.popp<CMDInt>();
		m_pParticleTemplate2D->m_nImageID= cmdInt->i;
	}

	void JCParticleTemplate2DCmdDispath::_rendercmd_play(JCMemClass& cmdBuffer)
	{
		CMDNoParam * cmdNoParam = cmdBuffer.popp<CMDNoParam>();
		m_pParticleTemplate2D->play();
	}

	void JCParticleTemplate2DCmdDispath::_rendercmd_stop(JCMemClass& cmdBuffer)
	{
		CMDNoParam * cmdNoParam = cmdBuffer.popp<CMDNoParam>();
		m_pParticleTemplate2D->stop();
	}

	bool JCParticleTemplate2DCmdDispath::restartJSThread()
	{
		return true;
	}


}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------