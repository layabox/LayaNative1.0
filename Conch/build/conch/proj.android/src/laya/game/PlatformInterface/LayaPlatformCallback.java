package laya.game.PlatformInterface;

import android.content.Context;
import android.util.Log;
import laya.game.browser.GL2JNILib;
import laya.game.browser.LayaWrapper;
import laya.game.conch.LayaConch3;


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
		LayaConch3.GetInstance().PlatformInitOK(nFlag);
	}
	//平台登录的回调 调用js脚本继续逻辑    p_sUserName,const char* p_sToken,const char* p_sMarketName,bool p_bResult
	public void LP_LoginCallback(String jsonParam )
	{
		GL2JNILib.loginCallback(jsonParam );
	}
	public void LP_onRechargeCallback( String jsonParam )
	{
		GL2JNILib.rechargeEvent(jsonParam);
	}
	public void LP_ShareAndFeedCallback( String jsonParam )
	{
		GL2JNILib.onShareAndFeed( jsonParam );
	}
	public void LP_onAuthorizeCallback( String jsonParam )
	{
		GL2JNILib.authorizeCallback(jsonParam);
	}
	public void LP_onRefreshTokenCallback( String jsonParam )
	{
		GL2JNILib.refreshTokenCallback( jsonParam );
	}
	public void LP_onGetGameFriendsCallback( String jsonParam )
	{
		GL2JNILib.onGetGameFriends( jsonParam );
	}
	public void LP_onSendToDesktopCallback( String jsonParam )
	{
		GL2JNILib.onSendToDesktop( jsonParam );
	}
	public void LP_onLogoutCallback( String jsonParam )
	{
		GL2JNILib.onLogout( jsonParam );
	}
	public void LP_onMarketInitCallback( String p_sTokenDir )
	{
		GL2JNILib.onMarketInit( p_sTokenDir );
	}

	public void LP_onTopicCircleCallback( String jsonParam )
	{
		GL2JNILib.onTopicCircle(jsonParam);
	}

	
	public void LP_InviteCallback(String jsonParam)
	{
		GL2JNILib.onInviteCallback( jsonParam );
	}

	public void LP_SwitchUserCallback(String jsonParam)
	{
		GL2JNILib.onSwitchUserCallback( jsonParam );
	}

	public void LP_EnterPlatformCallback(String jsonParam)
	{
		GL2JNILib.onEnterPlatformCallback( jsonParam );
	}

	public void LP_EnterBBSCallback(String jsonParam)
	{
		GL2JNILib.onEnterBBSCallback( jsonParam );
	}

	public void LP_EnterFeedbackCallback(String jsonParam)
	{
		GL2JNILib.onEnterFeedbackCallback( jsonParam );
	}

	public void LP_EnterAccountMgrCallback(String jsonParam)
	{
		GL2JNILib.onEnterAccountMgrCallback( jsonParam );
	}

	public void LP_BuyPropsCallback(String jsonParam)
	{
		GL2JNILib.onBuyPropsCallback( jsonParam );
	}

	public void LP_SetRechargeInfoCallback(String jsonParam)
	{
		GL2JNILib.onSetRechargeInfoCallback( jsonParam );
	}
	
	public void LP_SendMessageToPlatformCallback(String jsonParam)
	{
		GL2JNILib.onSendMessageToPlatformCallback( jsonParam );
	}
	
	public void LP_GetUserInfoCallback(String jsonParam)
	{
		GL2JNILib.onGetUserInfoCallback(jsonParam);
	}
	
	public void LP_GetAvailableLoginTypeCallback(String jsonParam)
	{
		GL2JNILib.onGetAvailableLoginTypeCallback(jsonParam);
	}
}
