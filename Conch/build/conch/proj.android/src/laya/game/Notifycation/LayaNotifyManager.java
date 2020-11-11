package laya.game.Notifycation;

import java.util.Calendar;
import java.util.Date;

import android.util.Log;
import laya.game.browser.LayaWrapper;
import laya.game.browser.MainActivity;
  
public class LayaNotifyManager
{
	private static LayaNotifyManager	g_spLayaNotifyManager = null;
	//private static MainActivity 				m_pMainView = MainActivity.GetInstance();
	
	public static final int ALARM_REPEAT_TYPE_YEAR = 0;		//每年
	public static final int ALARM_REPEAT_TYPE_MONTH = 1;		//每月
	public static final int ALARM_REPEAT_TYPE_DAY = 2;		//每日
	public static final int ALARM_REPEAT_TYPE_HOUR = 3;		//每小时
	public static final int ALARM_REPEAT_TYPE_MINUTE = 4;		//每分
	public static final int ALARM_REPEAT_TYPE_SECOND = 5;		//每秒
	
	//-------------------------------------------------------------------
	public static LayaNotifyManager GetInstance()
	{
		if( g_spLayaNotifyManager == null )
		{
			g_spLayaNotifyManager = new LayaNotifyManager();
		}
		return g_spLayaNotifyManager;
	}
	//-------------------------------------------------------------------
	public LayaNotifyManager()
	{
		
	}
	//-------------------------------------------------------------------
	/*
	 * 设置重复的消息
	 * 参数一：ID
	 * 参数二：第一次闹钟的延后时间 单位毫秒
	 * 参数三：闹钟重复的间隔类型 年 月 日 时 分 秒
	 * 参数四：消息提示的
	 * 参数五：消息提示的抬头
	 * 参数六：消息提示的内容
	 */
	//-------------------------------------------------------------------
	public static void setRepeatingNotify( int p_nID,long p_nStartTime,int p_nRepeatType,String p_sTickertext,String p_sTitletext,String p_sDesc )
	{
		Log.i("0", "==============setRepeatingNotify" );
		long nIntervalTime = 0;
		switch (p_nRepeatType) {
		case ALARM_REPEAT_TYPE_SECOND:
			nIntervalTime = 1000;
			break;
		case ALARM_REPEAT_TYPE_MINUTE:
			nIntervalTime = 60 * 1000;
			break;
		case ALARM_REPEAT_TYPE_HOUR:
			nIntervalTime = 60 * 60 * 1000;
			break;
		case ALARM_REPEAT_TYPE_DAY:
			nIntervalTime = 24 * 60 * 60 * 1000;
			break;
		case ALARM_REPEAT_TYPE_MONTH:
			nIntervalTime = 30 * 24 * 60 * 60 * 1000;
			break;
		case ALARM_REPEAT_TYPE_YEAR:
			nIntervalTime = 365 * 30 * 24 * 60 * 60 * 1000;
			break;
		default:
			Log.i( "0","setRepeatingNotify repeattype 错误" );
			break;
		}
		
		LayaAlarm.setRepeatingAlarm( LayaWrapper.getLayaApplicationContext(), p_nID, p_nStartTime, nIntervalTime, p_sTickertext, p_sTitletext, p_sDesc );
	}
	
	//-------------------------------------------------------------------
		/*
		 * 设置重复的消息
		 * 参数一：ID
		 * 参数二：第一次闹钟响铃时间的小时数 	0~23
		 * 参数三：第一次闹钟响铃时间的分钟数 	0~59
		 * 参数四：闹钟重复的间隔类型 年 月 日 时 分 秒
		 * 参数五：消息提示的
		 * 参数六：消息提示的抬头
		 * 参数七：消息提示的内容
		 */
		//-------------------------------------------------------------------
	public static void setRepeatWithHourMinute( int p_nID,int p_nStartHour,int p_nStartMinute,int p_nRepeatType,String p_sTickertext,String p_sTitletext,String p_sDesc )
	{
		Log.i("0", "==============setRepeatWithHourMinute" );
		long nIntervalTime = 0;
		long p_nStartTime = 0;
		switch (p_nRepeatType) {
		case ALARM_REPEAT_TYPE_SECOND:
			nIntervalTime = 1000;
			break;
		case ALARM_REPEAT_TYPE_MINUTE:
			nIntervalTime = 60 * 1000;
			break;
		case ALARM_REPEAT_TYPE_HOUR:
			nIntervalTime = 60 * 60 * 1000;
			break;
		case ALARM_REPEAT_TYPE_DAY:
			nIntervalTime = 24 * 60 * 60 * 1000;
			break;
		case ALARM_REPEAT_TYPE_MONTH:
			nIntervalTime = 30 * 24 * 60 * 60 * 1000;
			break;
		case ALARM_REPEAT_TYPE_YEAR:
			nIntervalTime = 365 * 30 * 24 * 60 * 60 * 1000;
			break;
		default:
			Log.i( "0","setRepeatingNotify repeattype 错误" );
			break;
		}
		Calendar calendar = Calendar.getInstance();
		int nCurrentHour = calendar.get(Calendar.HOUR_OF_DAY); 
		int mCurrentMinute = calendar.get(Calendar.MINUTE);
		if(nCurrentHour < p_nStartHour)
		{
			
			Calendar cal = Calendar.getInstance();
			cal.set(Calendar.HOUR_OF_DAY, p_nStartHour);
			cal.set(Calendar.MINUTE, p_nStartMinute);
			cal.set(Calendar.SECOND, 0);
			cal.set(Calendar.MILLISECOND, 0);
			p_nStartTime = cal.getTimeInMillis()-calendar.getTimeInMillis(); 
		}
		else if(nCurrentHour > p_nStartHour)
		{
			Calendar cal = Calendar.getInstance();
			cal.set(Calendar.HOUR_OF_DAY, p_nStartHour);
			cal.set(Calendar.MINUTE, p_nStartMinute);
			cal.set(Calendar.SECOND, 0);
			cal.set(Calendar.MILLISECOND, 0);
			p_nStartTime =  24*60*60*1000 - (calendar.getTimeInMillis()-cal.getTimeInMillis());
		}
		else
		{
			if( mCurrentMinute < p_nStartMinute )
			{
				Calendar cal = Calendar.getInstance();
				cal.set(Calendar.HOUR_OF_DAY, p_nStartHour);
				cal.set(Calendar.MINUTE, p_nStartMinute);
				cal.set(Calendar.SECOND, 0);
				cal.set(Calendar.MILLISECOND, 0);
				p_nStartTime = cal.getTimeInMillis()-calendar.getTimeInMillis(); 
			}
			else
			{
				Calendar cal = Calendar.getInstance();
				cal.set(Calendar.HOUR_OF_DAY, p_nStartHour);
				cal.set(Calendar.MINUTE, p_nStartMinute);
				cal.set(Calendar.SECOND, 0);
				cal.set(Calendar.MILLISECOND, 0);
				p_nStartTime =  24*60*60*1000 - (calendar.getTimeInMillis()-cal.getTimeInMillis());
			}
		}
		LayaAlarm.setRepeatingAlarm( LayaWrapper.getLayaApplicationContext(), p_nID, p_nStartTime, nIntervalTime, p_sTickertext, p_sTitletext, p_sDesc );
	}
	
	/*
	 * 设置重复的消息
	 * 参数一：ID
	 * 参数二：闹钟的延迟时间 单位毫秒  
	 * 参数三：消息提示的
	 * 参数四：消息提示的抬头
	 * 参数五：消息提示的内容
	 */
	//-------------------------------------------------------------------
	public static void setOnceNotify( int p_nID,long p_nStartTime,String p_sTickertext,String p_sTitletext,String p_sDesc )
	{
		Log.i("0", "==============setOnceNotify id=" + p_nID + ",p_nStartTime=" + p_nStartTime  );
		LayaAlarm.setOnceAlarm( LayaWrapper.getLayaApplicationContext(), p_nID, p_nStartTime, p_sTickertext, p_sTitletext, p_sDesc );
	}
	/*
	 * 只删除消息，但是定时器还保留
	 */
	//-------------------------------------------------------------------
	public static void removeNotify( int p_nID )
	{
		Log.i("0", "==============removeNotify" );
		LayaLocalNotify.removeNotfication( LayaWrapper.getLayaApplicationContext(),p_nID );
	}
	/*
	 * 只删除全部消息，但是定时器还保留
	 */
	//-------------------------------------------------------------------
	public static void removeAllNotify()
	{
		Log.i("0", "==============removeAllNotify" );
		LayaLocalNotify.removeAllNotification( LayaWrapper.getLayaApplicationContext() );
	}
	/*
	 * 删除消息
	 * 参数：id
	 */
	//-------------------------------------------------------------------
	public static void deleteAlarmAndNotify( int p_nID )
	{
		//删除定时器
		LayaAlarm.deleteAlarm( LayaWrapper.getLayaApplicationContext(),p_nID );
		//删除推送信息
		removeNotify( p_nID );
	}
	//-------------------------------------------------------------------
}