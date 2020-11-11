/**
@file			JCParticleTemplate2DCmdDispath.h
@brief			
@author			hugao
@version		1.0
@date			2016_11_8
*/

#ifndef __JCParticleTemplate2DCmdDispath_H__
#define __JCParticleTemplate2DCmdDispath_H__


//------------------------------------------------------------------------------

#include <JCICmdDispatch.h>
#include "JCParticleTemplate2D.h"

namespace laya
{
	class JCParticleTemplate2DCmdDispath : public JCICmdDispatch
	{
	public:

		typedef void (JCParticleTemplate2DCmdDispath::*ProcFunction)(JCMemClass& cmdBuffer);

	public:

		/** @brief构造函数
		*/
		JCParticleTemplate2DCmdDispath(int nID);

		/** @brief析构函数
		*/
		~JCParticleTemplate2DCmdDispath();

		bool restartJSThread();

		bool dispatchRenderCmd(JCMemClass& pRenderCmd, int nFrameCount);

		bool dispatchRenderCmdWhenLostedDevice(JCMemClass& pRenderCmd, int nFrameCount);

	public:

		void _rendercmd_setSetting(JCMemClass& cmdBuffer);
		
		void _rendercmd_setImage(JCMemClass& cmdBuffer);

		void _rendercmd_play(JCMemClass& cmdBuffer);

		void _rendercmd_stop(JCMemClass& cmdBuffer);

	public:
		JCParticleTemplate2D*                           m_pParticleTemplate2D;

	};
}
//------------------------------------------------------------------------------


#endif //__JCParticleTemplate2DCmdDispath_H__

//-----------------------------END FILE--------------------------------