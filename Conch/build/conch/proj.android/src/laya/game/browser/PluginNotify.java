package laya.game.browser;

import java.util.ArrayList;
import java.util.Iterator;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.util.Log;

public class PluginNotify {
	static private ArrayList mListeners = new ArrayList();
	
	public static void addPluginLlistener(IPluginListener _pluginListener){
		mListeners.add(_pluginListener);
	}
	
	//	通过C调用js
	static public void callJSMethod(String method,String[] params){		

		JSONObject localJSONObject = new JSONObject();
        JSONArray jsonArray = new JSONArray();
    	try {
			localJSONObject.put("functionName", method);
			if(params != null){
				int argLen = params.length;
				for(int i=0;i<argLen;++i){
					jsonArray.put(params[i]);
				}
				localJSONObject.put("value", jsonArray);
			}
			else
				localJSONObject.put("value", "");
		} catch (JSONException e) {
			e.printStackTrace();
		}
		String strJs = "JavaCallJSDM("+localJSONObject.toString()+")";
		Log.d("","plugin--------------- callJSMethod jscontent = "+strJs);
		GL2JNILib.RunJS(strJs);
		JSONObject jscalljava = new JSONObject();
		try {
			jscalljava.put("functionName", method);
			jscalljava.put("value", params[0]);
		} catch (JSONException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		String tempjs="JavaCallJS("+jscalljava.toString()+")";
		GL2JNILib.RunJS(tempjs);
	}
	
	public static void reciveInfoFromJS(String param){
		 Iterator localIterator = mListeners.iterator();
		 while(localIterator.hasNext()) {
			 IPluginListener _lis = (IPluginListener) localIterator.next();
			 if( _lis == null )
				 continue;
			 _lis.recivedInfo(param);
		 }
	}
	public static String reciveInfoFromJSEx(String method,Object[] params){
		 Iterator localIterator = mListeners.iterator();
		 while(localIterator.hasNext()) {
			 IPluginListener _lis = (IPluginListener) localIterator.next();
			 if( _lis == null )
				 continue;
			 return _lis.recivedInfoEx(method,params);
		 }
		 return "";
	}
	public static void removeAllPluginLlistener(){
		mListeners.clear();
	}
	
	public static void removePluginLlistener(IPluginListener _pluginListener){
		if (mListeners.contains(_pluginListener)) {
			mListeners.remove(_pluginListener);
		}
	}
}
