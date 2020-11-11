package laya.game.conch;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.HashMap;

import laya.game.browser.GL2JNILib;

import android.util.Log;

public class JavaJSBridge implements ILayaJavaJSBridge {
	final String TAG = "JavaJSBridge";

	private HashMap<String,Object> mJSInstance = new HashMap<String, Object>();
	@Override
	public void registerJSInstance(String jsName, Object instance) {
		if(mJSInstance.containsKey(jsName)){
			Log.e(TAG,"exportJSInstance, err : jsName:["+jsName+"] have already exsist");
			return;
		}
		mJSInstance.put(jsName, instance);
	}

	@Override
	public void unregisterJSInstance(String jsName) {
		if(!mJSInstance.containsKey(jsName)){
			Log.e(TAG,"removeJSInstance, err : don't hava jsName:["+jsName+"]");
			return;
		}
		mJSInstance.remove(jsName);
	}

	@Override
	public void executeJSMethod(String methodName, Object... args) {
		int argLen = args.length;
		StringBuilder _sb = new StringBuilder();
		for(int i=0;i<argLen;++i){
			if( args[i].getClass() == String.class )
				_sb.append("\"").append(args[i].toString()).append("\",");
			else
				_sb.append(args[i].toString()).append(",");
		}
		_sb.deleteCharAt(_sb.length()-1);
		Log.e(TAG,"executeJSMethod, methodName:["+methodName+"("+_sb.toString()+")"+"]");
		GL2JNILib.RunJS(methodName+"("+_sb.toString()+")");
	}
	
	public String executeJavaMethod(String jsName,String methodName,String jsonParam){
		if(!mJSInstance.containsKey(jsName)){
			Log.e(TAG,"executeJavaMethod, err : don't hava jsName:["+jsName+"]");
			return "";
		}
		String[] paramArr = null;
		if(jsonParam != null)
			paramArr = jsonParam.split(",");
		Object instance = mJSInstance.get(jsName);
		Method[] _methodArr = instance.getClass().getMethods();
		Method _method = null;
		Object[] obj = null;
		for(int i=0;i<_methodArr.length;++i){
			_method = _methodArr[i];
			if( _method.getName().equals(methodName)){
				Class[] paramTypes = _method.getParameterTypes();
				if( paramArr != null && paramArr.length != paramTypes.length ){
					Log.e(TAG,"excuteMethod, err : param length err jsname:["+jsName+"],methodName:["+methodName+"],needLength:["+paramTypes.length+"],curLength:["+paramArr.length+"]");
					return "";
				}
				if( paramTypes.length > 0 ){
					obj = new Object[paramTypes.length];
					for(int j=0;j<paramTypes.length;++j){
						if(paramTypes[j] == int.class)
							obj[j] = Integer.parseInt(paramArr[j]);
						else if(paramTypes[j] == float.class)
							obj[j] = Float.parseFloat(paramArr[j]);
						else if(paramTypes[j] == double.class)
							obj[j] = Double.parseDouble(paramArr[j]);
						else if(paramTypes[j] == boolean.class)
							obj[j] = Boolean.parseBoolean(paramArr[j]);
						else if(paramTypes[j] == long.class)
							obj[j] = Long.parseLong(paramArr[j]);
						else if(paramTypes[j] == short.class)
							obj[j] = Short.parseShort(paramArr[j]);
						else
							obj[j] = (Object)paramArr[j];
					}
				}
				break;
			}
		}

		if( _method != null ){								//	find method in instance by 'methodName'
			try {
				Object _return = null;
				if( obj == null ){
					_return = _method.invoke(instance);
				}else
					_return = _method.invoke(instance,obj);
				return _return != null ? _return.toString():"";
			} catch (IllegalArgumentException e) {
				e.printStackTrace();
			} catch (IllegalAccessException e) {
				e.printStackTrace();
			} catch (InvocationTargetException e) {
				e.printStackTrace();
			}
		}
		return "";
	}

}
