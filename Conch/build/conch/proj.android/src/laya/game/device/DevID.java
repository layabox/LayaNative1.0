package laya.game.device;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

import android.content.Context;
import android.net.wifi.WifiManager;
import android.os.Build;
import android.provider.Settings.Secure;
import android.telephony.TelephonyManager;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.WindowManager;

public class DevID
{
	protected Context m_Context;
	public DevID( Context context)
	{
		m_Context = context;
	}
	public String GetAnUniqueID()
	{
		String devsn = GetDevSerial();
		if( devsn!=null && devsn.length()>0)
			return devsn;
		String wifimac = GetWifiMac();
		if( wifimac !=null && wifimac.length()>0 )
		{
			return wifimac.replace(":", "");
		}
		String androidid = GetAndroidID();
		if( androidid!=null && androidid.length()>0 )
			return androidid;
		return GetMyUniqueID();	
	}
	public String GetIMEI()
	{
		String ts = m_Context.TELEPHONY_SERVICE;
		TelephonyManager telephonyMgr = (TelephonyManager) m_Context.getSystemService(ts);
		String temp="UnKnow";
		if( telephonyMgr!=null ) 
		{
			try
			{
				temp=telephonyMgr.getDeviceId();
			}
			catch(Exception e)
			{
			   Log.e("LayaBox", e.toString());
			}
			catch(Throwable ex)
			{
			   Log.e("LayaBox",ex.toString());
			}
		}
		return temp;
	}
	public String GetIMSI()
	{
		String ts = m_Context.TELEPHONY_SERVICE;
		TelephonyManager telephonyMgr = (TelephonyManager) m_Context.getSystemService(ts);
		if( telephonyMgr !=null )
		{
			String temp=null;
			try
			{
			temp=telephonyMgr.getSubscriberId();
			}
			catch(SecurityException e)
			{
			   Log.e("LayaBox",e.toString());
			}
			return temp;
		}
		return null;
	}
	
	public static String getIMSI_1() {
        String imsiParcel = runCommand("service call iphonesubinfo 3");
        String imsi = getNumberFromParcel(imsiParcel);
        Log.d("laya", "IMSI_1:" + imsi);
        return imsi;
    }

    public static String getIMSI_2() {
        String imsiParcel = runCommand("service call iphonesubinfo2 3");
        String imsi = getNumberFromParcel(imsiParcel);
        Log.d("laya", "IMSI_2:" + imsi);
        return imsi;
    }


    public static String runCommand(String src) {
        try {
            Process process = Runtime.getRuntime().exec(src);
            BufferedReader reader = new BufferedReader(new InputStreamReader(process.getInputStream()));
            int read;
            char[] buffer = new char[2048];
            StringBuffer output = new StringBuffer();
            while ((read = reader.read(buffer)) > 0) {
                output.append(buffer, 0, read);
            }
            reader.close();

            // Waits for the command to finish.
            process.waitFor();

            return output.toString();
        } catch (IOException e) {
            Log.e("apipas", "IOException:" + e.getMessage());
            return null;

        } catch (InterruptedException e) {
            Log.e("apipas", "InterruptedException:" + e.getMessage());
            return null;
        }
    }


    public static String getNumberFromParcel(String str) {
        String res = "";
        if (str != null && str.length() > 0) {
            String lines[] = str.split("\n");
            for (String line : lines) {
                if (line == null || line.length() == 0)
                    continue;
                String content[] = line.split("'");
                if (content.length > 1) {
                    res += content[1].replace(".", "");
                }
            }
        } else return "NA";
        return res;
    }
	public String GetWifiMac()
	{
		WifiManager wm = (WifiManager) m_Context.getApplicationContext().getSystemService(Context.WIFI_SERVICE);
		String m_szWLANMAC = wm.getConnectionInfo().getMacAddress();
		return m_szWLANMAC;
	}
	public String GetDevSerial()
	{
		return "";
	}
	public String GetAndroidID()
	{
		return Secure.getString(m_Context.getContentResolver(), Secure.ANDROID_ID);
	}
	public String GetMyUniqueID()
	{
		return "UNKNOWN";
	}
	public String GenARandomID()
	{
		return "";
	}
	public String GetPhoneModelAndSDK()
	{
		return Build.MODEL;//"Model:"+Build.MODEL;//+",SDK:"+Build.VERSION.SDK;
	}	
	public String GetOSVersion(){
		return android.os.Build.VERSION.RELEASE;
	}
	public int[] GetResolutionArray(){
		DisplayMetrics pDm = new DisplayMetrics();
		WindowManager wm = (WindowManager)(m_Context.getSystemService(Context.WINDOW_SERVICE));
		wm.getDefaultDisplay().getMetrics(pDm);    
		int a[]={pDm.widthPixels,pDm.heightPixels,pDm.densityDpi};
        return a; 
	}
}
