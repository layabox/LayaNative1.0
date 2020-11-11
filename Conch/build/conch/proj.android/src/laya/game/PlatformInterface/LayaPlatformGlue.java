package laya.game.PlatformInterface;

import laya.game.conch.LayaConch3;
import android.util.Log;

//laya与js的胶水函数定义
public class LayaPlatformGlue
{
	private static LayaPlatformGlue m_spLayaPlatformGlue = null;
	private static String sNotSupport="{\"result\":-50,\"desc\":\"not support\"}";
	private LayaPlatformInterface m_pPlatform = null;
	static public LayaPlatformGlue GetInstance()
	{
		if( m_spLayaPlatformGlue == null )m_spLayaPlatformGlue = new LayaPlatformGlue();
		return m_spLayaPlatformGlue;
	}
	static public void DelInstance(){
		m_spLayaPlatformGlue = null;
	}
	public void Init( LayaPlatformInterface p_pPlatfrom )
	{
		m_pPlatform = p_pPlatfrom;
	}
	public String getMarketName()
	{
		Log.i("0",">>>>>getMarketName");
		String sMarketName = LayaConch3.getMarketBundle().getString(LayaConch3.MARKET_MARKETNAME);
		if( sMarketName == null ) sMarketName = "";
		return sMarketName;
	}
	public int getPayType()
	{
		Log.i("0",">>>>>getPayType");
		return LayaConch3.getMarketBundle().getInt(LayaConch3.MARKET_PAYTYPE);
	}
	public int getChargeType()
	{
		Log.i("0",">>>>>getChargeType");
		return LayaConch3.getMarketBundle().getInt(LayaConch3.MARKET_CHARGETYPE);
	}
	public int getLoginType()
	{
		Log.i("0",">>>>>getLoginType");
		return LayaConch3.getMarketBundle().getInt(LayaConch3.MARKET_LOGINTYPE);
	}
	public String getServerName()
	{
		Log.i("0",">>>>>getServerName");
		String sServerName = LayaConch3.getMarketBundle().getString(LayaConch3.MARKET_SERVERNAME);
		if( sServerName == null ) sServerName = "";
		return sServerName;
	}
	public int getEnterPlatformType()
	{
		Log.i("0",">>>>>getEnterPlatformType");
		return LayaConch3.getMarketBundle().getInt(LayaConch3.MARKET_ENTERPLATFORMTYPE);
	}
	public void login(String jsonParam)
	{
		Log.i("0",">>>>>login");
		if( m_pPlatform != null )
		{
			m_pPlatform.LP_Login(jsonParam);
		}
		else
		{
			LayaPlatformCallback.GetInstance().LP_LoginCallback(sNotSupport);
		}
	}
	public void switchUser(String jsonParam)
	{
		Log.i("0",">>>>>switchUser");
		if( m_pPlatform != null )
		{
			m_pPlatform.LP_SwitchUser(jsonParam);
		}
		else
		{
			LayaPlatformCallback.GetInstance().LP_SwitchUserCallback(sNotSupport);
		}
	}
	public void logout(String jsonParam)
	{
		Log.i("0",">>>>>logout");
		if( m_pPlatform != null )
		{
			m_pPlatform.LP_Logout(jsonParam);
		}
		else
		{
			LayaPlatformCallback.GetInstance().LP_onLogoutCallback(sNotSupport);
		}
	}
	public void recharge( String jsonParam )
	{
		Log.i("0",">>>>>uniPayForCoin");
		if( m_pPlatform != null )
		{
			m_pPlatform.LP_Recharge( jsonParam );
		}
		else
		{
			LayaPlatformCallback.GetInstance().LP_onRechargeCallback(sNotSupport);
		}
	}
	public void enterPlatform(String jsonParam)
	{
		Log.i("0",">>>>>enterPlatform");
		if( m_pPlatform != null )
		{
			m_pPlatform.LP_EnterPlatform(jsonParam);
		}
		else
		{
			LayaPlatformCallback.GetInstance().LP_EnterPlatformCallback(sNotSupport);
		}
	}
	public void onGameEvent( String jsonParam )
	{
		Log.i("0",">>>>>onGameEvent");
		if( m_pPlatform != null )
		{
			m_pPlatform.LP_onGameEvent( jsonParam );
		}
	}
	public void enterBBS(String jsonParam)
	{
		Log.i("0",">>>>>enterBBS");
		if( m_pPlatform != null )
		{
			m_pPlatform.LP_enterBBS(jsonParam);
		}
		else
		{
			LayaPlatformCallback.GetInstance().LP_EnterBBSCallback(sNotSupport);
		}
		
	}
	public void enterFeedback(String jsonParam)
	{
		Log.i("0",">>>>>enterFeedback");
		if( m_pPlatform != null )
		{
			m_pPlatform.LP_enterFeedback(jsonParam);
		}
	}
	public void setRechargeInfo( String jsonParam )
	{
		Log.i("0",">>>>>setRechargeInfo param="+jsonParam);
		if( m_pPlatform != null )
		{
			m_pPlatform.LP_setRechargeInfo(jsonParam );
		}
	}
	public void enterAccountMgr(String jsonParam)
	{
		Log.i("0",">>>>>enterAccountMgr");
		if( m_pPlatform != null )
		{
			m_pPlatform.LP_enterAccountMgr(jsonParam);
		}
	}
	public void buyProps( String jsonParam )
	{
		Log.i("0",">>>>>buyProps = "+jsonParam);
		if( m_pPlatform != null )
		{
			m_pPlatform.LP_buyProps(jsonParam);
		}
	}
	public void shareAndFeed( String p_sExtInfo )
	{
		Log.i("0",">>>>>shareAndFeed param = "+p_sExtInfo);
		if( m_pPlatform != null )
		{
			m_pPlatform.LP_enterShareAndFeed( p_sExtInfo );
		}
		else
		{
			LayaPlatformCallback.GetInstance().LP_ShareAndFeedCallback(sNotSupport);
		}
	}
	public void invite( String p_sJsonParam )
	{
		Log.i("0",">>>>>invite = "+p_sJsonParam);
		if( m_pPlatform != null )
		{
			m_pPlatform.LP_enterInvite(p_sJsonParam);
		}
	}
	public void  authorize( String p_sJsonParam )
	{
		Log.i("0",">>>>>authorize = "+p_sJsonParam);
		if( m_pPlatform != null )
		{
			m_pPlatform.LP_authorize( p_sJsonParam );
		}
		else
		{
			LayaPlatformCallback.GetInstance().LP_onAuthorizeCallback(sNotSupport);
		}
	}
	public void  refreshToken( String p_sJsonParam )
	{
		Log.i("0",">>>>>refreshToken = "+p_sJsonParam);
		if( m_pPlatform != null )
		{
			m_pPlatform.LP_RefreshToken( p_sJsonParam );
		}
		else
		{
			LayaPlatformCallback.GetInstance().LP_onRefreshTokenCallback(sNotSupport);
		}
	}
	//------------------------------------------------------------------------------
	public void getGameFriends( String p_sJsonParam )
	{
		Log.i("0",">>>>>getGameFriends = " + p_sJsonParam );
		if( m_pPlatform != null )
		{
			m_pPlatform.LP_getGameFriends( p_sJsonParam );
		}
		else
		{
			LayaPlatformCallback.GetInstance().LP_onGetGameFriendsCallback(sNotSupport);
		}
	}
	
	//------------------------------------------------------------------------------
	public void sendToDesktop( String p_sJsonParam )
	{
		Log.i("0",">>>>>sendToDesktop extinfo=" + p_sJsonParam );
		if( m_pPlatform != null )
		{
			m_pPlatform.LP_sendToDesktop( p_sJsonParam );
		}
		else
		{
			LayaPlatformCallback.GetInstance().LP_onSendToDesktopCallback(sNotSupport);
		}
	}

	//------------------------------------------------------------------------------
	
	public int canSendToDesktop(String p_sJsonParam){
		Log.i("0",">>>>>canSendToDesktop");
		if( m_pPlatform != null )
		{
			return m_pPlatform.LP_canSendToDesktop(p_sJsonParam);
		}
		return 1;
	}
	//------------------------------------------------------------------------------
	public void sendMessageToPlatform( String p_sParam )
	{
		Log.i("0",">>>>>sendMessageToPlatform p_sParam=" + p_sParam );
		if( m_pPlatform != null )
		{
			m_pPlatform.LP_sendMessageToPlatform( p_sParam );
		}
	}
	public void openTopicCircle(String p_sJsonParam) {
		Log.i("0",">>>>>openTopicCircle p_sParam=" + p_sJsonParam );
		if( m_pPlatform != null )
		{
			m_pPlatform.LP_openTopicCircle( p_sJsonParam );
		}
		else
		{
			LayaPlatformCallback.GetInstance().LP_onTopicCircleCallback(sNotSupport);
		}
	}
	public void getUserInfo(String p_sJsonParam)
	{
		Log.i("0",">>>>>getUserInfo p_sParam=" + p_sJsonParam );
		if( m_pPlatform != null )
		{
			m_pPlatform.LP_getUserInfo( p_sJsonParam );
		}
		else
		{
			LayaPlatformCallback.GetInstance().LP_GetUserInfoCallback(sNotSupport);
		}
	}
	public void getAvailableLoginType(String p_sJsonParam)
	{
		Log.i("0",">>>>>getAvailableLoginType p_sParam=" + p_sJsonParam );
		if( m_pPlatform != null )
		{
			m_pPlatform.LP_getAvailableLoginType( p_sJsonParam );
		}
		else
		{
			LayaPlatformCallback.GetInstance().LP_GetAvailableLoginTypeCallback(sNotSupport);
		}
	}
	public String getMarketValue(String key)
	{
		Log.i("0",">>>>>getMarketValue key=" + key );
		if( m_pPlatform != null )
		{
			return m_pPlatform.LP_getMarketValue(key);
		}
		return "";
	}
	public void setMarketValue(String key,String value)
	{
		Log.i("0",">>>>>setMarketValue key=" + key +"value="+value);
		if( m_pPlatform != null )
		{
			m_pPlatform.LP_setMarketValue(key,value);
		}
	}
}
