package laya.game.browser;

public abstract interface IPluginListener {
	public abstract void recivedInfo(String param);
	public abstract String recivedInfoEx(String method,Object[] params);	
}
