package layaair.game.PlatformInterface;

import layaair.game.browser.ConchJNI;
import layaair.game.conch.LayaConch5;


public class LayaPlatformCallback 
{
	static private LayaPlatformCallback sLPCallback= null;
	static public LayaPlatformCallback GetInstance()
	{
		if(sLPCallback == null) sLPCallback = new LayaPlatformCallback();
		return sLPCallback;
	}
	//平台初始化的回调 调用js脚本继续逻辑
	public void LP_InitCallback(int nFlag)
	{
		LayaConch5.GetInstance().PlatformInitOK(nFlag);
	}
	//平台登录的回调 调用js脚本继续逻辑    p_sUserName,const char* p_sToken,const char* p_sMarketName,bool p_bResult
	public void LP_LoginCallback(String jsonParam )
	{
		ConchJNI.loginCallback(jsonParam);
	}
	public void LP_onCZCallback( String jsonParam )
	{
		ConchJNI.rechargeEvent(jsonParam);
	}
	public void LP_ShareAndFeedCallback( String jsonParam )
	{
		ConchJNI.onShareAndFeed(jsonParam);
	}
	public void LP_onAuthorizeCallback( String jsonParam )
	{
		ConchJNI.authorizeCallback(jsonParam);
	}
	public void LP_onRefreshTokenCallback( String jsonParam )
	{
		ConchJNI.refreshTokenCallback(jsonParam);
	}
	public void LP_onGetGameFriendsCallback( String jsonParam )
	{
		ConchJNI.onGetGameFriends(jsonParam);
	}
	public void LP_onSendToDesktopCallback( String jsonParam )
	{
		ConchJNI.onSendToDesktop(jsonParam);
	}
	public void LP_onLogoutCallback( String jsonParam )
	{
		ConchJNI.onLogout(jsonParam);
	}
	public void LP_onMarketInitCallback( String p_sTokenDir )
	{
		ConchJNI.onMarketInit(p_sTokenDir);
	}

	public void LP_onTopicCircleCallback( String jsonParam )
	{
		ConchJNI.onTopicCircle(jsonParam);
	}

	
	public void LP_InviteCallback(String jsonParam)
	{
		ConchJNI.onInviteCallback(jsonParam);
	}

	public void LP_SwitchUserCallback(String jsonParam)
	{
		ConchJNI.onSwitchUserCallback(jsonParam);
	}

	public void LP_EnterPlatformCallback(String jsonParam)
	{
		ConchJNI.onEnterPlatformCallback(jsonParam);
	}

	public void LP_EnterBBSCallback(String jsonParam)
	{
		ConchJNI.onEnterBBSCallback(jsonParam);
	}

	public void LP_EnterFeedbackCallback(String jsonParam)
	{
		ConchJNI.onEnterFeedbackCallback(jsonParam);
	}

	public void LP_EnterAccountMgrCallback(String jsonParam)
	{
		ConchJNI.onEnterAccountMgrCallback(jsonParam);
	}

	public void LP_BuyPropsCallback(String jsonParam)
	{
		ConchJNI.onBuyPropsCallback(jsonParam);
	}

	public void LP_SetCZInfoCallback(String jsonParam)
	{
		ConchJNI.onSetRechargeInfoCallback(jsonParam);
	}
	
	public void LP_SendMessageToPlatformCallback(String jsonParam)
	{
		ConchJNI.onSendMessageToPlatformCallback(jsonParam);
	}
	
	public void LP_GetUserInfoCallback(String jsonParam)
	{
		ConchJNI.onGetUserInfoCallback(jsonParam);
	}
	
	public void LP_GetAvailableLoginTypeCallback(String jsonParam)
	{
		ConchJNI.onGetAvailableLoginTypeCallback(jsonParam);
	}
}
