package layaair.game.Market;


import layaair.game.PlatformInterface.LayaPlatformCallback;
import layaair.game.PlatformInterface.LayaPlatformGlue;
import layaair.game.PlatformInterface.LayaPlatformInterface;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.content.Context;
import android.content.Intent;
import android.os.Handler;
import android.util.Log;
import android.webkit.ValueCallback;

public class MarketTest implements LayaPlatformInterface
{

    private final String TAG  = "MarketTest";
	public Handler m_Handler = new Handler();
	public String mUserId = "";
	private Context context;
	
	@Override
	public void LP_EnterPlatform(final String jsonParam) {
		
	}

	@Override
	public void LP_Init(final Context activity) {
		Log.d(TAG,">>>>>>>>>>>>>>>>MarketLaya init ok");
		context = activity;
		LayaPlatformCallback.GetInstance().LP_InitCallback( 0 );
	}

	@Override 
	public void LP_LeavePlatform(final String jsonParam) {
		
	}

	@Override
	public void LP_Login(final String jsonParam) {
		Log.d(TAG,">>>>>>>>>>>>> plugin LP_Login param = "+jsonParam);
		JSONObject _obj = new JSONObject();
		GameEngine.getInstance().getRuntimeProxy().Login(_obj, new ValueCallback<JSONObject>() {
			@Override
			public void onReceiveValue(JSONObject LP_obj) {
				Log.d(TAG,">>>>>>>>>>>>>>> plugin LP_Login onReceiveValue = "+LP_obj.toString());
				JSONObject objCallBack = new JSONObject();	
				
				String nickName = "";
                String userId = "";
                int status = -1;
                String photoUrl = "";
                String token = "";
                String message = "";
                if(LP_obj.has("unionUserID")){
                    try {
                        status = LP_obj.getInt("status");
                        userId = LP_obj.getString("unionUserID");
                        nickName = LP_obj.getString("nickName");
                        photoUrl = LP_obj.getString("photo");
                        token = LP_obj.getString("sptoken");
                        message = LP_obj.getString("msg");
                        String marketName = LayaPlatformGlue.GetInstance().getMarketName();         
                        objCallBack.put("nickName", nickName);
                        objCallBack.put("refreshToken", token);
                        objCallBack.put("unionUserId",userId);
                        objCallBack.put("sessionKey",token);
                        objCallBack.put("avtorUrl", photoUrl);
                        objCallBack.put("marketName", marketName); 
                        objCallBack.put("result",status);
                        objCallBack.put("desc",message );
                        LayaPlatformCallback.GetInstance().LP_LoginCallback( objCallBack.toString() );
                    } catch (JSONException e) {
                        e.printStackTrace();
                    }                    
                }
			}
			
		});
	}
	
	@Override
	public void LP_Logout(final String jsonParam) {
	    try {
            Log.d(TAG, ">>>>>LP_Logout jsonParam= "+jsonParam);
            JSONObject obj = new JSONObject();
            //JSONObject m_obj = new JSONObject(jsonParam);
            obj.put("userId", mUserId);
            Log.d(TAG,">>>>>>>>>LP_Logout obj=" + obj.toString() );
            GameEngine.getInstance().getRuntimeProxy().Logout(obj, new ValueCallback<JSONObject>()
            {
                @Override
                public void onReceiveValue(JSONObject value) 
                {
                    try
                    {
                        String sMsg = value.getString("msg");
                        int nResult = Integer.parseInt( value.getString("status"));
                        JSONObject objCallBack = new JSONObject();
                        objCallBack.put("result",nResult );
                        objCallBack.put("desc",sMsg );
                        Log.d(TAG,">>>>>>>>>LP_Logout callback" + objCallBack.toString() );
                        LayaPlatformCallback.GetInstance().LP_onLogoutCallback( objCallBack.toString() );
                    } 
                    catch (JSONException e) 
                    {
                        Log.d(TAG,">>>>>>LP_Logout callback error" + e );
                        JSONObject objCallBack = new JSONObject();
                        try {
                            objCallBack.put("result",-1 );
                            objCallBack.put("desc","" );
                        } catch (JSONException e1) {
                            e1.printStackTrace();
                        }
                        LayaPlatformCallback.GetInstance().LP_onLogoutCallback( objCallBack.toString() );
                    }
                }
            });
        } catch (JSONException e) {
            Log.d(TAG, ">>>>>+++LP_Logout: "+e.toString());
            JSONObject objCallBack = new JSONObject();
            try {
                objCallBack.put("result",-1 );
                objCallBack.put("desc","" );
            } catch (JSONException e1) {
                e1.printStackTrace();
            }
            LayaPlatformCallback.GetInstance().LP_onLogoutCallback( objCallBack.toString() );
        }
	}

	@Override
	public void LP_OnDestroy() {
		
	}

	@Override
	public void LP_RefreshToken(final String jsonParam) {
		
	}

	@Override
	public void LP_SwitchUser(final String jsonParam) {
		
	}
	/** 参数说明：
	 	Context             	Context
		uid                 	傲游H5游戏中心帐号
		cp_id               	H5游戏厂商代码
		service_id          	H5游戏及区服代码
		pay_fee             	充值金额(人民币,单位是分)
		goods_id            	购买道具或者游戏币代码
		game_name          		游戏的中文名称
		item_name           	购买的内容
		lock                	是否锁定支付金额，布尔类型
		params              	游戏方自定义预留参数
	*/
	@Override
	public void LP_CZ(final String jsonParam) {
	    Log.d(TAG,">>>>>>>>>>>>> plugin LP_Recharge param = "+jsonParam); 
		JSONObject _obj = new JSONObject();
		try {
			JSONObject m_obj = new JSONObject(jsonParam);
			_obj.put("orderId", m_obj.get("orderId"));
			_obj.put("payAmount", ""+m_obj.get("amount"));
            _obj.put("goodsName", m_obj.get("goodName"));
            _obj.put("userId", m_obj.get("customer_name"));
            _obj.put("gameId", m_obj.get("appid"));
            
            String extra = m_obj.getString("other");
            JSONObject objectOther=new JSONObject(extra);
            String goodsId = objectOther.getString("goodsId");
            String callbackUrl = objectOther.getString("notifyUrl");
            
            _obj.put("callbackUrl", callbackUrl);
            _obj.put("goodId", goodsId);
            
		} catch (JSONException e) {			
			e.printStackTrace();
		}
        Log.d(TAG,">>>>>>>>>>>>> plugin LP_Recharge param = "+_obj.toString()); 
		GameEngine.getInstance().getRuntimeProxy().Pay(_obj, new ValueCallback<JSONObject>() {

			@Override
			public void onReceiveValue(JSONObject value) {
				Log.d(TAG,">>>>>>>>>>>>> plugin LP_Recharge onReceiveValue = "+value.toString());
				try {
			        int p_nResult = value.getInt("status");

			        JSONObject _jobj = new JSONObject();
			        _jobj.put("result", p_nResult);
			        _jobj.put("desc", value.getString("msg"));
					LayaPlatformCallback.GetInstance().LP_onCZCallback(_jobj.toString());
				} catch (JSONException e) {
					e.printStackTrace();
				}
			}
		});
	}
	
	@Override
	public void LP_authorize(final String jsonParam) {
		
	}

	@Override
	public void LP_buyProps(final String jsonParam) {
		
	}

	@Override
	public int LP_canSendToDesktop(final String jsonParam) {
		return 0;
	}

	@Override
	public void LP_enterAccountMgr(final String jsonParam) {
		
	}

	@Override
	public void LP_enterBBS(final String jsonParam) {	
	    Log.d(TAG,">>>>>>>LP_enterBBS param =" + jsonParam );
	    try {
            JSONObject jsonObj = new JSONObject(jsonParam);
            GameEngine.getInstance().getRuntimeProxy().OpenBBS(jsonObj, new ValueCallback<JSONObject>() {
                @Override
                public void onReceiveValue(JSONObject value) {
                    Log.d(TAG, ">>>>>>>LP_enterBBS onReceiveValue value=" + value.toString() );
                    try 
                    {
                        int nResult = Integer.parseInt( value.getString("status") );
                        String sMsg = value.getString("msg");
                        JSONObject objCallBack = new JSONObject();
                        objCallBack.put("result", nResult);
                        objCallBack.put("desc", sMsg);
                        LayaPlatformCallback.GetInstance().LP_EnterBBSCallback( objCallBack.toString() );
                        
                    }
                    catch (JSONException e) 
                    {
                        Log.d(TAG,">>>>>>>LP_enterBBS error=" + e );
                        JSONObject objCallBack = new JSONObject();
                        try {
                            objCallBack.put("result", -1);
                            objCallBack.put("desc", e.toString());
                        } catch (JSONException e1) {
                            e1.printStackTrace();
                        }
                        LayaPlatformCallback.GetInstance().LP_EnterBBSCallback( objCallBack.toString() );
                    }
                    
                }
            });
	    }catch (JSONException e) {
            e.printStackTrace();
        }	    
	}

	@Override
	public void LP_enterFeedback(final String jsonParam) {
		
	}

	@Override
	public void LP_enterInvite(final String jsonParam) {
		
	}

	@Override
	public void LP_enterShareAndFeed(final String jsonParam) {
        Log.d(TAG,"LP_enterShareAndFeed jsonParam = "+jsonParam);
        
        try {
            JSONObject outParam = new JSONObject(jsonParam);
            JSONObject desParam = new JSONObject();
            desParam.put("gameUrl", outParam.getString("link"));
            desParam.put("gameName", outParam.getString("title"));
            desParam.put("gameIcon", outParam.getString("imgsrc"));
            desParam.put("isFullScreen", true);
            desParam.put("desc", outParam.getString("desc"));
            GameEngine.getInstance().getRuntimeProxy().Share(desParam, new ValueCallback<JSONObject>() {

                @Override
                public void onReceiveValue(JSONObject value) {
                    Log.d(TAG,"LP_enterShareAndFeed receiveValue = "+value);
                    try 
                    {
                        int nResult = Integer.parseInt( value.getString("status") );
                        String sMsg = value.getString("msg");
                        JSONObject objCallBack = new JSONObject();
                        objCallBack.put("code", nResult);
                        objCallBack.put("desc", sMsg);
                        LayaPlatformCallback.GetInstance().LP_ShareAndFeedCallback( objCallBack.toString() );
                        
                    } catch (JSONException e) 
                    {
                        Log.d(TAG,">>>>>>>LP_enterShareAndFeed error=" + e );
                        JSONObject objCallBack = new JSONObject();
                        try {
                            objCallBack.put("result", -1);
                            objCallBack.put("desc", e.toString());
                        } catch (JSONException e1) {
                            e1.printStackTrace();
                        }
                        LayaPlatformCallback.GetInstance().LP_ShareAndFeedCallback( objCallBack.toString() );
                    }
                }
            });
        } catch (JSONException e) {
            e.printStackTrace();
        }
        
        
	   
	}

	@Override
	public void LP_getGameFriends(final String jsonParam) {
	    Log.d(TAG,">>>>>>>>LP_getGameFriends = "+jsonParam);
        try {
            JSONObject jsonObj = new JSONObject(jsonParam);
            
            JSONObject jsonObjParam = new JSONObject();
            jsonObjParam.put("gameId", jsonObj.getString("appid"));
            jsonObjParam.put("userId", mUserId);
            jsonObjParam.put("accessToken", "");
            GameEngine.getInstance().getRuntimeProxy().GetFriendsList(jsonObjParam, new ValueCallback<JSONObject>() {
                @Override
                public void onReceiveValue(JSONObject value) {
                    try 
                    {
                        int nResult = Integer.parseInt( value.getString("status") );
                        String sMsg = value.getString("msg");
                        JSONArray pFriends = value.getJSONArray("friends");
                        String sFriendsList = pFriends.toString();
                        Log.d(TAG,">>>>>>>>LP_getGameFriends callback msg=" + sMsg + ",result=" + nResult + ",sFriendsList=" + sFriendsList );
                        JSONObject objCallBack = new JSONObject();
                        objCallBack.put("result", nResult);
                        objCallBack.put("desc", sMsg);
                        objCallBack.put("friendslist", sFriendsList);
                        LayaPlatformCallback.GetInstance().LP_onGetGameFriendsCallback( objCallBack.toString() );
                    } 
                    catch (JSONException e) 
                    {
                        Log.d(TAG,">>>>LP_getGameFriends error =" + e );
                        JSONObject objCallBack = new JSONObject();
                        try {
                            objCallBack.put("result", -1);
                            objCallBack.put("desc", e.toString());
                            objCallBack.put("friendslist", "");
                        } catch (JSONException e1) {
                            e1.printStackTrace();
                        }
                        LayaPlatformCallback.GetInstance().LP_onGetGameFriendsCallback( objCallBack.toString() );
                    }
                }
            });
        } catch (JSONException e) {
            e.printStackTrace();
        }
	    
	}

	@Override
	public void LP_onCreate(final Context activity) {
		
	}

	@Override
	public Boolean LP_onExit(final String jsonParam) {
		return null;
	}

	@Override
	public void LP_onGameEvent(final String jsonParam) {
		
	}

	@Override
	public void LP_onPause(final String jsonParam) {
		
	}

	@Override
	public void LP_onResume(final Context activity) {
		
	}

	@Override
	public void LP_openTopicCircle(final String jsonParam) {
	  
	}

	@Override
	public void LP_sendMessageToPlatform(final String jsonParam) {
	    try {
            JSONObject jsonObj = new JSONObject(jsonParam);
            GameEngine.getInstance().getRuntimeProxy().SendMessageToPlatform(jsonObj, new ValueCallback<JSONObject>() {
                @Override
                public void onReceiveValue(JSONObject value) {
                    //TODO
                    LayaPlatformCallback.GetInstance().LP_SendMessageToPlatformCallback(value.toString() );
                }
            });
        } catch (JSONException e) {
            e.printStackTrace();
        }
	}

	@Override
	public void LP_sendToDesktop(final String jsonParam) {
	    Log.d(TAG,"LP_sendToDesktop jsonParam = "+jsonParam);
	    try {
	    	JSONObject outParam = new JSONObject(jsonParam);
            JSONObject desParam = new JSONObject();
            desParam.put("gameUrl", outParam.getString("pageUrl"));
            desParam.put("gameName", outParam.getString("title"));
            desParam.put("gameIcon", outParam.getString("imageUrl"));
            desParam.put("isFullScreen", true);
            String orientation = "landscape";
            if(outParam.has("orientation"))
                orientation = outParam.getString("orientation");
            desParam.put("orientation", orientation);
            GameEngine.getInstance().getRuntimeProxy().PushIcon(desParam, new ValueCallback<JSONObject>() {
                @Override
                public void onReceiveValue(JSONObject value) {
                    Log.d(TAG,"LP_sendToDesktop receiveValue = "+value);
                    try{
                        int nResult = Integer.parseInt( value.getString("status") );
                        String sMsg = value.getString("msg");
                        Log.d(TAG, ">>>>>>>LP_sendToDesktop callback msg" + sMsg + ",result=" + nResult );
                        JSONObject objCallBack = new JSONObject();
                        objCallBack.put("result", nResult);
                        objCallBack.put("desc", sMsg);
                        LayaPlatformCallback.GetInstance().LP_onSendToDesktopCallback( objCallBack.toString() );
                        
                    }
                    catch (JSONException e){
                        Log.d(TAG,">>>>LP_sendToDesktop error =" + e );
                        JSONObject objCallBack = new JSONObject();
                        try {
                            objCallBack.put("result", -1);
                            objCallBack.put("desc", e.toString());
                        } catch (JSONException e1) {
                            e1.printStackTrace();
                        }
                        LayaPlatformCallback.GetInstance().LP_onSendToDesktopCallback( objCallBack.toString() );
                    }
                }
            });
        } catch (JSONException e) {
            e.printStackTrace();
        }	    
	}

	@Override
	public void LP_setCZInfo(final String jsonParam) {
		
	}

    @Override
    public void LP_getAvailableLoginType(String arg0) {
        // TODO Auto-generated method stub        
    }

    @Override
    public String LP_getMarketValue(String arg0) {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public void LP_getUserInfo(String arg0) {
        // TODO Auto-generated method stub        
    }

    @Override
    public void LP_setMarketValue(String arg0, String arg1) {
        // TODO Auto-generated method stub        
    }

    @Override
    public void LP_onStop(Context context) {
        // TODO Auto-generated method stub
    }

    @Override
    public void LP_onRestart(Context context) {
        // TODO Auto-generated method stub
    }

    @Override
    public void onActivityResult(int i, int i1, Intent intent) {
        // TODO Auto-generated method stub
    }

    @Override
    public void onNewIntent(Intent intent) {
        // TODO Auto-generated method stub
    }
}
