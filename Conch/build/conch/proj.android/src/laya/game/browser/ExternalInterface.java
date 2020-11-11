package laya.game.browser;

import java.util.HashMap;

import laya.game.browser.IPluginListener;
import laya.game.browser.PluginNotify;

import org.json.JSONException;
import org.json.JSONObject;
import org.json.JSONTokener;

import android.util.Log;

public class ExternalInterface implements IPluginListener{
	private static final String TAG = "ExternalInterface";
	private HashMap mCallbackList = new HashMap();
	static private ExternalInterface _instance = null;	
	
	static public interface IExternalInterface{
		public abstract void call(String param);
		public abstract String call(String[] param);
	}
	
	static public ExternalInterface getInstance(){
		if(_instance == null)
			_instance = new ExternalInterface();
		return _instance;
	}
	
	//	增加浏览器java回调函数
	public void addCallBack(String method,IExternalInterface _interface){
		mCallbackList.put(method, _interface);
	}
	
	//	调用游戏js函数
	public void call(String method,String[] params){
		Log.e(TAG,"plugin-----------------game_engine_get_externalInterface()");
	    PluginNotify.callJSMethod(method,params);
		return;
	}
	@Override
	public void recivedInfo(String paramString){
	    Log.d(TAG, "plugin----------------recivedInfo info = " + paramString);
	    try{
	    	JSONObject localJSONObject = new JSONObject(new JSONTokener(paramString));
	        String str = localJSONObject.getString("functionName");
	        IExternalInterface localIExternalInterface = (IExternalInterface)this.mCallbackList.get(str);
	        if (localIExternalInterface != null){
	        	localIExternalInterface.call(localJSONObject.getString("value"));
	        	return;
	        }
	        Log.d(TAG, "plugin----------------调用了java没有注册的方法:" + str);
	        return;
	    }catch(JSONException localJSONException){
	    	localJSONException.printStackTrace();
	    }
	}

	@Override
	public String recivedInfoEx(String method, Object[] params) {
		String[] sParams = (String[]) params;
		IExternalInterface localIExternalInterface = (IExternalInterface)this.mCallbackList.get(method);
        if (localIExternalInterface != null){
        	return localIExternalInterface.call(sParams);
        }
		// TODO Auto-generated method stub
		return "";
	}
	
	public void run(){
		PluginNotify.addPluginLlistener(this);
	}
}
