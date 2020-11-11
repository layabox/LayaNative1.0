package laya.game.Notifycation;

import laya.game.browser.MainActivity;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

public class LayaLocalNotify 
{
	private static NotificationManager 		m_pNotificationManager = null;
	private static PendingIntent 			m_pPendingIntent = null;
	private static Intent 					m_pIntent = null;
	private static Notification 			m_pNotification = null;
	
	public static void sendNotification( Context p_pContext,int p_nID,String p_sTickertext,String p_sTitle,String p_sContent )
	{
		m_pNotificationManager =  (NotificationManager)p_pContext.getSystemService( p_pContext.NOTIFICATION_SERVICE );
		int nDrawableId = p_pContext.getResources().getIdentifier("ic_launcher","drawable", p_pContext.getPackageName());
		m_pNotification = new Notification( nDrawableId,p_sTickertext,System.currentTimeMillis() );
		m_pNotification.defaults=Notification.DEFAULT_ALL;
		m_pIntent = new Intent( p_pContext, MainActivity.class );
		m_pPendingIntent=PendingIntent.getActivity( p_pContext, 0, m_pIntent, 0 );
		m_pNotification.setLatestEventInfo( p_pContext,p_sTitle,p_sContent,m_pPendingIntent );
		m_pNotificationManager.notify( p_nID, m_pNotification );
		
	}
	public static void removeNotfication( Context p_pContext,int p_nID )
	{
		//Log.i("0","========removeNotfication");
		m_pNotificationManager =  (NotificationManager)p_pContext.getSystemService( p_pContext.NOTIFICATION_SERVICE );
		m_pNotificationManager.cancel( p_nID );
	}
	public static void removeAllNotification( Context p_pContext )
	{
		m_pNotificationManager =  (NotificationManager)p_pContext.getSystemService( p_pContext.NOTIFICATION_SERVICE );
		m_pNotificationManager.cancelAll();
	}
}