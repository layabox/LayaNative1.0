//"use strict";  这样子的话，第一次会无法进行不下去（好像弄好之后第二次能行），在updatedcc那里卡住了。
conchConfig.PerfUpdateDt = PerfUpdateDt;
conchConfig.PerfAddData = PerfAddData;
conchConfig.PerfDelData = PerfDelData;
conchConfig.PerfShow = PerfShow;
conch.config = conchConfig;
conch.market = conchMarket;
conch.notify = conchNotify;
window._conchInfo={version:'2.2.0.0'};

console.log('======================================================  \n' );
console.log('             LAYA CONCH            \n' );
console.log('     runtimeversion:' + conchConfig.getRuntimeVersion() +  '\n' );
console.log('          jsversion:' +  window._conchInfo.version + '\n' );
console.log('             isplug:' + conchConfig.getIsPlug() + '\n' );
console.log('======================================================\n' );

function log(m){
    console.log(m);
}

require('debugger/nodefullfill.js');
window.layaHtml5={File:File};//初始化html5内置的类 防止项目层给重置
window.downloadfile = function(url, force,onok,onerr){
	if(force){
		url=(function(url){
			var ret=url;
			if(url.indexOf('?')<0){
				ret = url+'?rnd='+Math.random();
			}else{
				ret = url+'&downloadrnd'+Math.random().toString().substr(2)+'=1';
			}
			return ret;
		})(url);
	}
	var file = new layaHtml5.File(url);
	var filereader = new FileReader();
	filereader.onload=function(){ onok && onok(filereader.result);}
	filereader.onerror=function(){onerr && onerr();}
	filereader.readAsText(file);
}


if(0)
	require('E:/conch4/trunk/Conch4/source/script/out/main');
else{
require('url_history.js');
require('androidBridge.js');
require('webview.js');
IsStreamMode()&&require('stringStream.js');
setOnActive(function()
{
});

setOnDeactive(function()
{
});
//主流程
var runSeqs= new function(){
	this.seqs=[];			//等待执行的函数
	this.run=function(p){		//依次执行，可以递归调用。注意没有做插入等操作，因为在递归的时候处理起来比较麻烦
		var f = this.seqs.shift();
		if(f)f(p);
	}
};
function loadUrl(url){
	log('_loadUrl('+url+')');
	if(url.length<2)return;
	location.setHref(url);	//只是设置，不希望触发跳转。所以用函数。
	var layainfo=null;
    var urlpath = location.fullpath + '/';
	window.localStorage = loadLocalStorage( urlpath );
	var cache = window.appcache = new AppCache(urlpath);//这时候会加载资源索引，因此如果更新了索引，必须重新创建。
										//为了让启动html也使用缓存，需要在loadUrl刚开始的时候就创建appcache对象。
    if (location && location.queries && location.queries['layaStoreAppMarkID']) {
        cache.setResourceID('appid', location.queries['layaStoreAppMarkID']);
    }
    else {
        log('没有设置appid!');
    }
										
	var cookiepath=(function(url){
		var filename='';
		var filepos=url.indexOf('file:///');
		if(url.indexOf('?')>0){
			url = url.substr(0,url.indexOf('?'));	//去掉?
		}
		if(filepos>=0){
			filename = url.substr(filepos+8,url.lastIndexOf('/')-(filepos+8));
			filename = filename.replace(/:/g,'_');
			filename = filename.replace(/[\\\/]/g,'__');
		}else{
			var vv = url.split('/');
			vv.shift();vv.shift();vv.pop();
			vv[0]=vv[0].replace(':','_');
			filename=vv.join('_');
		}
		return filename;
	})(url);
	document.loadCookie( cookiepath );

	var preloads = [];	//需要预先下载的文件
	var scripts=[];		//需要执行的文件
	
	var runNext = function(p){runSeqs.run.call(runSeqs,p);}
	
	function downloadfile(url,onok,onerr,ignoree){
		var file = new layaHtml5.File(url);
		var filereader = new FileReader();
		filereader.setIgnoreError(ignoree?true:false);
		filereader.onload=function(){ onok(filereader.result);}
		filereader.onerror=function(){log('下载文件错误：'+url);onerr();}
		filereader.readAsText(file);
	}
	
	function getPageInfo(){
		//字符串可以是'也可以是"。现在先临时写一个不太严格的。
		var reg = new RegExp(/([a-zA-Z0-9]+)\s?=['"]([^'^"]*)['"]/g);
		function parseKVPair(str,obj){
			var ret = null;
			while(ret=reg.exec(str)){
				var keyv=ret[1];
				var valv=ret[2];
				obj[keyv]=valv;
			}
		}
		
		function onStartPageData(data){
			//加载流程和标准见文档的 <页面加载流程.txt>
			//meta里面的laya信息
			layainfo=(function(data){
				var layametas = data.match(/<\s*meta\s*name=['"]laya['"].*/g);//原来的写法遇到路径分隔符/就会出错
				if(layametas){
					//判断是不是laya页面
					var retobj={/*version:'1.0',*/screenorientation:'landscape',cacheid:'CDCDCDCD'};
					layametas.forEach(function(layameta){
						parseKVPair(layameta,retobj);
					});
					return retobj;
				}
				return null;
			})(data);
			//先加入一个执行的js。这个应该都是laya.js
			if(layainfo ){
				var layaurl=(layainfo.layaurl);//||(runcontext.layarepo+layainfo.version+'/laya.js');
				var urlobj = new window.URL(url);
				if(layaurl && layaurl.indexOf('://')<0){//相对路径
					layaurl = urlobj.resolve(url,layaurl);
					scripts.push(layaurl);
				}
			}
			//由微端加载的script标签。只能有一个
			var layascriptSrc = (function(data){
				var urlobj = new window.URL(url);
				var lscript = data.match(/<script\s+.*loader=['"]laya['"].*/g);
				if(lscript){
					lscript.forEach(function(script){
						var ret = script.match(/src=([^\s]+)/);
						if(ret){
							var scriptsrc=ret[1].substr(1,ret[1].length-2);
							if(scriptsrc.indexOf('://')<0){//相对路径
								scriptsrc = urlobj.resolve(url,scriptsrc);
							}
							scripts.push(scriptsrc);
						}
					});
				}
				return scripts;
			})(data); 
			//需要预先加载的文件
			preloads = (function(data){
				var files=[];
				var preloadmetas = data.match(/<\s*meta\s*name=['"]preload['"].*/g);
				if(preloadmetas){
					preloadmetas.forEach(function(meta){
						var ret={};
						parseKVPair(meta,ret);
						if(ret.files){
							files=files.concat(ret.files.split(';'));
						}
					});
				}
				return files;
			})(data);
			
			if(scripts.length){
				runNext();
			}else{
				//不是laya页面
				alert('Sorry, this is a normal html page. i cant render it yet!\n'+data);
			}
		}
		downloadfile(url+'?rand='+Math.random()*(new Date().valueOf()),	//优先尝试强制下载。
			onStartPageData//on download OK
			, function(e){//on download error
				//如果失败了再尝试有没有打包版的页面数据。
				downloadfile(url,onStartPageData//on download OK
					,function(e){//on download error
						if(androidBridge){
							androidBridge.call('onLoadingEvent','{result:false}');
						}else
							alert('load start page error!url='+url);
					});
			});
	}
	
	function setOrientation(){
		var nameToVal={
			landscape:0,portrait:1,user:2,behind:3,sensor:4,nosensor:5,sensor_landscape:6,
			sensor_portrait:7,reverse_landscape:8,reverse_portrait:9,full_sensor:10,}
		var nOri = (function(name){
				try{
					var n = nameToVal[name];
					return n||0;
				}catch(e){
					return 0;
				}
			})(layainfo.screenorientation);
			
		if(conchConfig.getScreenOrientation()!=nOri){
			conchConfig.setScreenOrientation(nOri);
		}
		runNext();
	}
	function rotDelay(){
		setTimeout(function(){runNext();},200);
	}
	function runConfig()
	{
		if( layainfo )
		{
			if( layainfo.configjs )
			{
				var configJsUrl = layainfo.configjs;
				var urlobj = new window.URL(url);
				if( configJsUrl && configJsUrl.indexOf('://')<0 ){//相对路径
					configJsUrl = urlobj.resolve(url,configJsUrl);
				}
				downloadfile( configJsUrl,function(data){window.eval(data);runNext();},
										function(e){log( ">>>download configjs error e=" + e);runNext();});
			}
			else
			{
				runNext();
			}
		}
		else
		{
			runNext();
		}
	}
	function showLogo()
	{
		if( layainfo )
		{
			if( layainfo.logojs )
			{
				layainfo.bShowLogoJS = true;
				var logojsurl = layainfo.logojs;
				var urlobj = new window.URL(url);
				if( logojsurl && logojsurl.indexOf('://')<0 ){//相对路径
					logojsurl = urlobj.resolve(url,logojsurl);
				}
				downloadfile( logojsurl,function(data){window.eval(data);runNext();},
										function(e){log( ">>>download logjs error e=" + e);layainfo.bShowLogoJS=false;runNext();});
			}
			else
			{
				runNext();
			}
		}
		else
		{
			runNext();
		}
	}
	
	function updateDcc(){
		appcache.setResourceID('appurl',urlpath);
		var curassets = appcache.getResourceID('netassetsid');
		downloadfile(urlpath+'update/assetsid.txt?rand='+Math.random()*(new Date().valueOf()),//加随机是为了防止不小心把它加入了缓存
			function(data){
				if( data!=curassets ){
				    //下载新的包
                    downloadfile(urlpath + 'update/filetable.txt?rand=' + Math.random() * (new Date().valueOf()),
                        function (idxdata) {
                            appcache.saveFileTable(idxdata);
                            appcache = new AppCache(urlpath);
                            //设置新的id。只有写完filetable之后才能写这个
                            appcache.setResourceID('netassetsid', data);
                            runNext();
                        },
                        function (e) {
                            runNext();
                        }, true);
				}else
					runNext();
			},
			function (e) {
                //如果没有assetsid，则去掉校验功能。
			    appcache.saveFileTable('');
			    appcache = new AppCache(urlpath);
				runNext();
			},true);	
	}
	
	//下载必须文件
	function predownload(){
		var num = preloads.length;
		if(num<=0)
			runNext();
		function preloadret(b){
			num--;
			if(num<=0){
				runNext();
			}
		}
		preloads.forEach(function(f){
			var absurl=f;
			var urlobj = new window.URL(url);
			if(absurl.indexOf('://')<0){//相对路径
				absurl = urlobj.resolve(url,absurl);
			}
			//读取二进制文件
			var file = new layaHtml5.File(absurl);
			var filereader = new FileReader();
			filereader.onload=function(){ preloadret(true);}
			filereader.onerror=function(){preloadret(false);}
			filereader.readAsArrayBuffer(file);
		});
	}
	
	function startApp(){
		function getIsEval()
		{
			if( layainfo.bShowLogoJS == true )
			{
				var nOldTime = new Date().valueOf();
				if( conch.getLogoImageState() )
				{
					setTimeout(downloadscript,1);
				}
				else if( new Date().valueOf() - nOldTime > 15000 )
				{
					downloadscript();
				}
			}
			else
			{
				downloadscript();
			}
		}
		//依次执行所有的js
		function downloadscript(){
			clearInterval( conch.scriptTimeInterval );
			if( scripts.length <= 0 )
				return;
			var ssrc=scripts[0];
			scripts.shift();
			log('downloadjs:'+ssrc);			
			downloadfile(ssrc,scriptret,
				function(e){
					if(androidBridge){
						androidBridge.call('onLoadingEvent','{result:false}');
					}else				
						alert('download js error:url='+ssrc);
				});
		}
		function scriptret(data){
			log('downloadjs ok');			
			try{
				window.eval(data);
				if( !layainfo.bShowLogoJS )
				{
					conch.showLoadingView( false );
				}
			}catch(e){
				alert(e+' '+e.stack);
				return;
			}
			//scripts.shift();
			if(scripts.length<=0){
				if( window.onload ) window.onload();
			}
			else{
				setTimeout(downloadscript,1);
			}
		}
		conch.scriptTimeInterval=setInterval(getIsEval,17);
        conchMarket.setIAPCallBack(function(param){
               var xhr = new XMLHttpRequest();
               var _json = JSON.parse(param);
                console.log("pruchase backurl = "+_json.backurl);
               xhr.open('POST',_json.backurl,true);
               var sendParam = 'plateform='+_json.plateform+
                   '&order-id='+_json.order_id+
                   '&game-id='+_json.game_id+
                   '&production-id='+_json.production_id+
                   '&transaction-id='+_json.transaction_id+
                   '&receipt-data='+_json.receipt_data;

               var amount = _json.production_amount;
               console.log("iap purchase param = "+sendParam);
               xhr.onload = function(){
                    console.log("iap web result = "+this.responseText);
                    var _rescode = 0;
                    var _resultJson = JSON.parse(this.responseText);
                    if( _resultJson['error-code'] == 0 ){
                        console.log("成功！！");
                    }else{
                        _rescode = _resultJson['error-code'];
                        console.log("verify err code = "+_rescode+" info = "+_resultJson['error-info']);
                    }
                                   
                    var _res = {'code':''+_rescode,'quantity':''+amount,'orderID':_json.order_id};
                    conchMarket.setIAPPurchaseResult(JSON.stringify(_res));
               }
               xhr.send(sendParam);
            });
	}
	//需要依次执行的函数
	runSeqs.seqs=[getPageInfo,setOrientation,rotDelay,updateDcc,runConfig,showLogo,predownload,startApp];
	runSeqs.run();//这个函数不会退出，直到程序结束		
}


window.layaDoc.addEventListener('keydown',function(e)
{
	switch( e.keyCode )
	{
	case 115://F4
		urlBack();
		break;
	case 116://F5
		reloadCurProcess();
		break;
	case 117://F6
		simAndroidDeviceLosted();
		break;
	case 118://F7
		conch.printAllMemorySurvey("","http://10.10.20.114:8889/upload","layabox");
		break;
	case 119://F8
		conch.printTextureManagerInfo();
		break;
	case 120:
		gc();
		break;
	}
});
function getStartDefaultHtmlPath()
{
	var sExePath = getExePath();
	if( sExePath != null )
	{
		var sStartHtml = "";
		var vString = sExePath.split( '\\' );
		vString.pop();vString.pop();
		sStartHtml = vString.join( '/' );
		sStartHtml += '/starthtml/default.html'
		return sStartHtml
	}
	return null;
}

//当前运行状态的提示信息
var RunInfo=function(){
	/*
	var scrw=window.innerWidth;
	var scrh=window.innerHeight;
	
	var txtinfo = conch.createNode();
	txtinfo.size(scrw,60);
	txtinfo.pos(0,scrh-60);
	txtinfo.bgcolor('#777777');
	txtinfo.alpha(0.2);
	txtinfo.color('#ffffffff');
	txtinfo.font("normal 100 16px Arial");
	//txtinfo.textStr('xxxxxxxxxxxxxxxxxx',0,0);
	//root.insert(txtinfo,0,1);
	conch.setRunInfoNode(txtinfo);	
	this.txt = txtinfo;
	*/
	this.setTxt=function(txt){
		//txtinfo.textStr(txt,0,0);
	}
}
//loadUrl(location.href||'http://10.10.20.200:8899/default.html');
//loadUrl(location.href||'http://10.10.20.200:8888/default.html');
//loadUrl(location.href||'http://10.10.20.200:8889/layaairdebug.max.html');
//loadUrl(location.href||'http://10.10.20.200:8888/default1.html');
//loadUrl(location.href||'http://10.10.20.200:8899/webgl/simple-cube.html' );
loadUrl(location.href||'http://10.10.20.200:7788/conch-main.html' );
//loadUrl(location.href||'http://10.10.20.19:7788/main2.html' );
//loadUrl(location.href||'http://lr.apple.dawawa.com/default.html?&jointypeid=50031');
//loadUrl(location.href||'http://mm.apple.dawawa.com/default.html?&url=mobilegame/index.html&driver=html5&time=null&jointypeid=50031&lang=' );
//loadUrl(location.href||'http://10.10.30.29:8888/default.html');
//loadUrl(location.href||'http://10.10.20.176/cardmin.max.html' );
//loadUrl(location.href||'http://jieyou.jieyou.com/h5/startapp.max.html' );
//loadUrl(location.href||'http://10.10.20.24:8888/hxh.max.html' );
//loadUrl(location.href||'http://10.10.20.173:8899/layaairdebug.max.html');
}