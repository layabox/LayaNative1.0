package laya.game.Notifycation;

import android.app.AlarmManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

public class LayaAlarm
{
	//-------------------------------------------------------------------
	public static void setRepeatingAlarm( Context p_pContext,int p_nID,long p_nStartTime,long p_nIntervalTime,String p_sTickertext,String p_sTitletext,String p_sDesc )
	{
		Log.i("0","========setRepeatingAlarm");
		if(p_pContext==null){
			return;
		}
		AlarmManager pAlarmManager = (AlarmManager) p_pContext.getSystemService( Context.ALARM_SERVICE );
		//保存数据
		Intent pIntent = new Intent( p_pContext,LayaAlarmReceiver.class );
		pIntent.putExtra( "id",  p_nID );
		pIntent.putExtra( "ticker", p_sTickertext );
		pIntent.putExtra( "title", p_sTitletext );
		pIntent.putExtra( "desc", p_sDesc );
		PendingIntent pPendingIntent = PendingIntent.getBroadcast( p_pContext, p_nID, pIntent,PendingIntent.FLAG_UPDATE_CURRENT );
		pAlarmManager.setRepeating( AlarmManager.RTC_WAKEUP, System.currentTimeMillis() + p_nStartTime,p_nIntervalTime, pPendingIntent );
	}
	//-------------------------------------------------------------------
	public static void setOnceAlarm( Context p_pContext,int p_nID,long p_nStartTime,String p_sTickertext,String p_sTitletext,String p_sDesc )
	{
		if(p_pContext==null){
			return;
		}

		Log.i("0","========setOnceAlarm id=" + p_nID + ",p_nStartTime=" + p_nStartTime );
		
		AlarmManager pAlarmManager = (AlarmManager) p_pContext.getSystemService( Context.ALARM_SERVICE );
		Intent pIntent = new Intent( p_pContext,LayaAlarmReceiver.class );
		pIntent.putExtra( "id",  p_nID );
		pIntent.putExtra( "ticker", p_sTickertext );
		pIntent.putExtra( "title", p_sTitletext );
		pIntent.putExtra( "desc", p_sDesc );
		PendingIntent pPendingIntent = PendingIntent.getBroadcast( p_pContext, p_nID, pIntent,PendingIntent.FLAG_UPDATE_CURRENT );
		pAlarmManager.set( AlarmManager.RTC_WAKEUP, System.currentTimeMillis() + p_nStartTime, pPendingIntent );
		
	}
	//-------------------------------------------------------------------
	public static void deleteAlarm( Context p_pContext,int p_nID )
	{
		if(p_pContext==null){
			return;
		}

		Log.i("0","========deleteAlarm");
		AlarmManager pAlarmManager = (AlarmManager) p_pContext.getSystemService( Context.ALARM_SERVICE );
		Intent pIntent = new Intent( p_pContext,LayaAlarmReceiver.class );
		PendingIntent pPendingIntent = PendingIntent.getBroadcast( p_pContext, p_nID, pIntent,PendingIntent.FLAG_UPDATE_CURRENT );
		pAlarmManager.cancel( pPendingIntent );
	}
}