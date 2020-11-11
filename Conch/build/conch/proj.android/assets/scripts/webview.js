(function(exports){
	exports.allwebview={}
	exports.createWebview=function(name,url,hidden){
		var ret={
			name:name,
			//par是一个对象
			//调用创建的webview中的脚本函数。
			callJS:function(methodName,par){
				androidBridge.call('callWebviewJsMethod',JSON.stringify({name:this.name,method:methodName,params:JSON.stringify(par)}));
			},
			//创建的webview可以回调layaboxplayer的func
			addJSCallback:function(msg,func){
				if( !androidBridge.addPlatformMsgListener ){
					throw '没有 androidBridge.addPlatformMsgListener，应该先包含androidBridge.js！';
				}
				androidBridge.addPlatformMsgListener('webview_MSG_'+name+'_'+msg, func);
			},
			delJSCallback:function(msg,func){
				if( !androidBridge.delPlatformMsgListener ){
					throw '没有 androidBridge.delPlatformMsgListener，应该先包含androidBridge.js！';
				}
				androidBridge.delPlatformMsgListener('webview_MSG_'+name+'_'+msg, func);
			}
		};
		androidBridge.call('createWebview',JSON.stringify({name:name,url:url,hidden:hidden}));
		androidBridge.name = name;
		allwebview[name]=ret;
		return ret;
	}
	
	exports.getWebview=function(name){
		return allwebview[name];
	}
})(window);
