package laya.game.conch;


public abstract interface ILayaJavaJSBridge {
	/**
	 * 注册js实例
	 * @param jsName   导出到js的名字
	 * @param instance java实例
	 */
	public abstract void registerJSInstance(String jsName,Object instance);
	
	/**
	 * 注销js实例
	 * @param jsName  注册时的jsName
	 */
	public abstract void unregisterJSInstance(String jsName);
	
	/**
	 * 执行js方法
	 * @param methodName   	js方法名
	 * @param args			参数
	 */
	public abstract void executeJSMethod(String methodName,Object... args);
}
