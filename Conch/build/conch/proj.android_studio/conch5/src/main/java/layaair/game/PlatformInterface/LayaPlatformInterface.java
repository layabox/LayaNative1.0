package layaair.game.PlatformInterface;

import android.content.Context;
import android.content.Intent;

public interface LayaPlatformInterface {
	//有的平台需要必须在OnCreate里面初始化
	void LP_onCreate(final Context activity);

	//	平台初始化 在自己的类中把Activity存下来
	void LP_Init(final Context activity);

	//	平台登录
	void LP_Login(final String jsonParam);

	//	平台切换用户
	void LP_SwitchUser(final String jsonParam);

	//	平台登录退出
	void LP_Logout(final String jsonParam);

	//	充值
	void LP_CZ(final String jsonParam);

	//	销毁平台对象
	void LP_OnDestroy();

	//  社区
	void LP_EnterPlatform(final String jsonParam);

	//  退出社区
	void LP_LeavePlatform(final String jsonParam);

	//进入游戏，或者进入大厅
	void LP_onGameEvent(final String jsonParam);

	//进入BBS
	void LP_enterBBS(final String jsonParam);

	//进入游戏反馈
	void LP_enterFeedback(final String jsonParam);

	void LP_onResume(final Context activity);

	//暂停的处理，当从后台转为前台的时候处理
	void LP_onPause(final String jsonParam);

	//退出的处理函数  如果没有处理一定返回false 否则无法退出
	Boolean LP_onExit(final String jsonParam);

	//充值的时候设置的信息
	void LP_setCZInfo(final String jsonParam);

	//账户管理
	void LP_enterAccountMgr(final String jsonParam);

	//购买道具
	void LP_buyProps(final String jsonParam);

	//分享
	void LP_enterShareAndFeed(final String jsonParam);

	//facebook的邀请
	void LP_enterInvite(final String p_sJsonParam);

	//授权
	void LP_authorize(final String p_sJsonParam);

	//刷新token
	void LP_RefreshToken(final String p_sJsonParam);

	//获得好友列表
	void LP_getGameFriends(final String p_sJsonParam);

	//发送桌面图标
	void LP_sendToDesktop(final String p_sJsonParam);

	//	能否发送到桌面
	int LP_canSendToDesktop(final String p_sJsonParam);

	//发送消息给平台
	void LP_sendMessageToPlatform(final String p_sJsonParam);

	//打开话题圈
	void LP_openTopicCircle(final String p_sJsonParam);

	//获取openid的信息
	void LP_getUserInfo(final String p_sJsonParam);

	//获取可选登录信息
	void LP_getAvailableLoginType(final String p_sJsonParam);

	//获取value值
	String LP_getMarketValue(final String key);

	//设置value值
	void LP_setMarketValue(final String key, final String value);

	void LP_onStop(Context ctx);

	void LP_onRestart(Context ctx);

	void onActivityResult(int requestCode, int resultCode, Intent intent);

	void onNewIntent(Intent intent);
}
