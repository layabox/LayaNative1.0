package laya.game.conch;

public abstract interface ILayaExternalInterface
{
	public abstract void addCallBack(String name, IGameExternalInterfaceCallBack paramIGameExternalInterfaceCallBack);
  
	public abstract void call(String method, String params[]);
  
	public abstract void run();
  
//	public static abstract interface IGameExternalInterfaceCallBack{
//		public abstract void callBack(String param);
//	}
}
