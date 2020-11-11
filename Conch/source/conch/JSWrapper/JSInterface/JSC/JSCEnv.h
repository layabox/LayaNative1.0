

#ifndef __JSC_ENV_H__
#define __JSC_ENV_H__

#include <JavaScriptCore/JavaScriptCore.h>
#include "JSCProxyTLS.h"
#include <misc/JCWorkerThread.h>

namespace laya
{
	void JSPrint( const char* p_sBuffer );
	void JSAlert( const char* p_sBuffer );
	void JSAlertExit( const char* p_sBuffer );
	void evalJS( const char* p_sSource );

	class JSThread : public JCWorkerThread
	{
	public:
		JSThread();

		~JSThread();

		void initialize( int p_nPort );

		void uninitialize();
        
        void stop();
        
        void post( runObj func );

	public:
        
        virtual void _defRunLoop();

		virtual void _runLoop();
        
	};
		
};
#endif	// __JSC_ENV_H__
