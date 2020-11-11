package laya.game.Notifycation;

import java.util.List;
import java.util.Set;

import android.R.string;
import android.app.ActivityManager;
import android.app.ActivityManager.RunningAppProcessInfo;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;

public class LayaAlarmReceiver extends BroadcastReceiver 
{
	/*
	 * 判断 com.dawawa.miao是否在运行
	 */
	public static boolean isApplicationShowing( String p_sPackageName,Context p_pContext ) 
	{
		boolean result = false;
		ActivityManager pActivityManager = (ActivityManager) p_pContext.getSystemService(Context.ACTIVITY_SERVICE);
		List<RunningAppProcessInfo> vAppProcesses = pActivityManager.getRunningAppProcesses();
		if ( vAppProcesses != null ) 
		{
			for ( RunningAppProcessInfo pRunningAppProcessInfo : vAppProcesses ) 
			{
				if ( pRunningAppProcessInfo.processName.equals( p_sPackageName ) ) 
				{
					int nStatus = pRunningAppProcessInfo.importance;
					if ( nStatus == RunningAppProcessInfo.IMPORTANCE_VISIBLE || nStatus == RunningAppProcessInfo.IMPORTANCE_FOREGROUND  )
					{
						result = true;
					}
				}
			}
		}
		return result;
	}
	public static boolean isApplicationBackGround( String p_sPackageName,Context p_pContext ) 
	{
		boolean result = false;
		ActivityManager pActivityManager = (ActivityManager) p_pContext.getSystemService(Context.ACTIVITY_SERVICE);
		List<RunningAppProcessInfo> vAppProcesses = pActivityManager.getRunningAppProcesses();
		if ( vAppProcesses != null ) 
		{
			for ( RunningAppProcessInfo pRunningAppProcessInfo : vAppProcesses ) 
			{
				if ( pRunningAppProcessInfo.processName.equals( p_sPackageName ) ) 
				{
					int nStatus = pRunningAppProcessInfo.importance;
					if (  nStatus == RunningAppProcessInfo.IMPORTANCE_BACKGROUND )
					{
						result = true;
					}
				}
			}
		}
		return result;
	}
	public void onReceive( Context context, Intent intent ) 
	{
		Log.i("0", "========================LayaAlarmReceiver.onReceive pakeName=" + context.getPackageName() );
		Bundle pBundle= intent.getExtras();  
	    if( pBundle != null )
	    {
	    	/*
	        Set<String> vSet= pBundle.keySet();  
	        for( String iter:vSet )
	        {
	        	Log.i("0", "======key=" + iter + ",value=" + pBundle.get( iter ) );
	        }
	        */
	    	if( isApplicationShowing( context.getPackageName(),context ) == false )
	    	{
		        int nID = -1;
		        nID = pBundle.getInt( "id" );
		        String sTitle = pBundle.getString( "title" );
		        String sTicker = pBundle.getString( "ticker" );
		        String sDesc = pBundle.getString( "desc" );
		        if( nID != -1 )
		        {
		        	LayaLocalNotify.sendNotification( context, nID, sTicker, sTitle,sDesc );
		        }
	    	}
	    }
	}

}