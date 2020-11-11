
(function(exports){
	var platEvt={};
	androidBridge.addPlatformMsgListener=function(msg,func){
		if(!platEvt[msg])platEvt[msg]=[func];
		else platEvt[msg].push(func);
	}
	androidBridge.delPlatformMsgListener=function(msg,func){
		var handlers = platEvt[msg];
		var newhd = [];
		if(handlers){
			handlers.forEach(function(h){
				if(h!=func)newhd.push(h);
			});
		}
		platEvt[msg] = newhd;
	}
	androidBridge.onPlatformMsg=function(msg,param){
		var listeners = platEvt['webview_MSG_'+androidBridge.name+'_'+msg];
		if(listeners){
			listeners.forEach(function(func){func(param);});
		}
	}
	androidBridge.setJavaMsgHandler(androidBridge.onPlatformMsg);
	
	function jsExternalInterface(){
		var jsJava={}
		this.addCallBack = function(method,func){
			jsJava[method] = func;
		}
		this.call = function(method,params){
			return androidBridge.callArray('callExternalMethod',method,params);
		}
		this.executeJS = function(param){
			var _func = jsJava[param.functionName]; 
			if( !_func ){
				window[param.functionName].apply(null,param.value);
				return;
			}
			if(param.value.length>1){
				var _pa = JSON.parse(param.value);				
				_func.apply(this,_pa);
			}else{
				_func();
			}
		}
	}
	var _ExternalInterface = new jsExternalInterface();
	window.JavaCallJSDM = function(param){
		//window[param.functionName]&&window[param.functionName]({value:param.value[0]});
		//_ExternalInterface.executeJS(param);
	}
	exports.externalInterface=_ExternalInterface;
})(window||exports);
