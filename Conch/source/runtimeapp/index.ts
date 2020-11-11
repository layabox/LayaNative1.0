
window._conchInfo = { version: '2.1.3.1' };
var _inline =!conchConfig.localizable; //是否是单机版
console.log('======================================================  ');
console.log('             LAYA CONCH            ');
console.log('     runtimeversion:' + conchConfig.getRuntimeVersion());
console.log('          jsversion:' + window._conchInfo.version);
console.log('             isplug:' + conchConfig.getIsPlug());
console.log('======================================================');

function log(m) {
    console.log(m);
}
if (conchConfig.getOS() == "Conch-ios"){
    require('promise');
}
import asyncs = require('async');

async function initFreeType() {
    var sOS = conchConfig.getOS();
    var bRet = false;

    //优先使用包中的字体
    var sTempFontPath = conch.getCachePath() + "/runtimeFont/";
    if (!fs_exists(sTempFontPath)) {
        fs_mkdir(sTempFontPath);
    }
    sTempFontPath += "layabox.ttf";
    bRet = conch.initFreeTypeDefaultFontFromFile(sTempFontPath);
    if(bRet == false){
        var assetFontData = conch.readFileFromAsset('font/layabox.ttf','raw');
        if( assetFontData ){
            fs_writeFileSync(sTempFontPath, assetFontData);
            bRet = conch.initFreeTypeDefaultFontFromFile(sTempFontPath);
        }
    }
    
    if(!bRet){
        if (sOS == "Conch-window") {
            bRet = conch.initFreeTypeDefaultFontFromFile("C:/Windows/Fonts/simhei.ttf");
        }
        else if (sOS == "Conch-android") {
            var fSystemVersion = navigator.sv;
            if (fSystemVersion >= 2.0 && fSystemVersion < 5.0) {
                //这个步骤为因为HTC部分手机，作出了一个奇葩的决定：将DroidSansFallback架空，引入DFHEIA5A和DFHEIA7A两个字体文件。
                bRet = conch.initFreeTypeDefaultFontFromFile("/system/fonts/DFHEIA5A.ttf");
                if (bRet == false) {
                    bRet = conch.initFreeTypeDefaultFontFromFile("/system/fonts/DroidSansFallback.ttf");
                }
            }
            else if (fSystemVersion >= 5.0 && fSystemVersion < 6.0) {
                var vDefaultStrings = [];
                vDefaultStrings.push("/system/fonts/NotoSansHans-Regular.otf");
                vDefaultStrings.push("/system/fonts/Roboto-Regular.ttf");
                bRet = conch.initFreeTypeDefaultFontFromFile(vDefaultStrings.join('|'));
            }
            else if (fSystemVersion >= 6.0 && fSystemVersion < 7.0) {
                var vDefaultStrings = [];
                vDefaultStrings.push("/system/fonts/NotoSansSC-Regular.otf");
                vDefaultStrings.push("/system/fonts/Roboto-Regular.ttf");
                bRet = conch.initFreeTypeDefaultFontFromFile(vDefaultStrings.join('|'));
            }
            else if (fSystemVersion >= 7.0 && fSystemVersion < 8.0) {
                bRet = false;
            }
        }
        else if(sOS=="Conch-ios")
        {
            bRet = conch.initFreeTypeDefaultFontFromFile("");
        }
    }
    if (bRet == false) {
        log('字体初始化失败，从网络下载字体...');
        var data = <ArrayBuffer>(await asyncs.downloadSync(location.fullpath+'/font/simhei.ttf', true, null));
        if(!data){
            data=<ArrayBuffer>(await asyncs.downloadSync('http://runtime.layabox.com/font/simhei.ttf', true, null));
        }
        if (!data) {
            alert('下载字体失败。 ');
            return;
        }
        fs_writeFileSync(sTempFontPath, data);
        bRet = conch.initFreeTypeDefaultFontFromFile(sTempFontPath);
    }
    if(!bRet){
        log('字体初始化失败。');
    }
}
//initFreeType();

function setOrientation(s: string) {
    var nameToVal = {
        landscape: 0, portrait: 1, user: 2, behind: 3, sensor: 4, nosensor: 5, sensor_landscape: 6,
        sensor_portrait: 7, reverse_landscape: 8, reverse_portrait: 9, full_sensor: 10,
    }
    var nOri = (function (name: string) {
        try {
            var n = nameToVal[name];
            return n || 0;
        } catch (e) {
            return 0;
        }
    })(s);

    //if (conchConfig.getScreenOrientation() != nOri) {
        conchConfig.setScreenOrientation(nOri);;
    //}
}

function initHtml(data: string) {
    var tag = new RegExp("<(/|)([\\w.]+)(?: +|)?([^>]*?)(/|)>", "g");
    var res, attrs;
    var scripts = [];
    var all = [];
    //var includetags:Array = ["script", "meta"];
    var lastTag: any = {};
    var lastIndex = 0;
    var _comments = new RegExp("<!--[\\s\\S]*?-->", "g");
    //var _scripts:RegExp = new RegExp("(/\\*([^*]|[\\r\\\n]|(\\*+([^*/]|[\\r\\n])))*\\*+/)|(//.*)", "g");
    var _blank = new RegExp("^[^\\S]*|[^\\S]*$", "g");//空白字符 前后
    var _attributes = new RegExp("(\\w+)\\s*=\\s*(\"[^\"]*\"|'[^']*')", "g")
    data = data.replace(_comments, "");
    var layaMeta = null;
    var supportedTag = ['html','title','head','body','script','meta'];
    while (res = tag.exec(data)) {
        var temp: any = {};
        temp.tagName = res[2];
        if (res[3]) {
            temp.attributes = {};
            while ((attrs = _attributes.exec(res[3])) != null) {
                temp.attributes[attrs[1]] = attrs[2].substring(1, attrs[2].length - 1);
            }
        }

        if(temp.tagName==='meta' && temp.attributes.name==='laya')
            layaMeta = temp.attributes;
        if( supportedTag.indexOf(temp.tagName)<0){
            alert('LayaPlayer不支持的标签：\n'+res[0]+'\n\n(具体请参考LayaBox网站文档)');
        }
        
        if (!res[1] && !res[4])//<script>
            temp.type = -1;
        else if (!res[1] && res[4])//<script/>
            temp.type = 0;
        else if (res[1] && !res[4])//</script>
            temp.type = 1;
        else {
            alert("解析标签出错</" + temp.tagName + "/>");
            return;
        }
        temp.index = res.index;
        all.push(temp);
        if (temp.tagName == "script" || temp.tagName == "meta") {
            if (temp.type == -1) {//<script>
                if (lastTag.tagName == temp.tagName && lastTag.type == -1) {
                    if (temp.tagName == "meta") {
                        scripts.push(temp);
                    }
                    else {
                        alert("解析标签" + temp.tagName + "出错,连续两个script标签可能是 脚本中包含<script>");
                        return;
                    }
                }
                //else if (temp.attributes) {
                    scripts.push(temp);
                //}
            }
            else if (temp.type == 1) {//</script>
                if (lastTag.tagName == temp.tagName && lastTag.type == -1) {
                    var text: String = data.substring(lastIndex, temp.index);
                    text = text.replace(_blank, "");
                    if (text) {
                        temp=scripts.pop();
                        temp.type=1;
                        temp.attributes = temp.attributes || {};
                        temp.attributes.text = text;
                        scripts.push(temp);
                    }
                }
            }
            else {//
                if (temp.attributes)
                    scripts.push(temp);
            }
        }
        lastIndex = res.index + res[0].length;
        lastTag = temp;
    }
    var chkjs = layaMeta && layaMeta.layajsprotect && layaMeta.layajsprotect=='true';
    for (var d in scripts) {
        var one = scripts[d];
        if(chkjs){ //如果要检查js的话，必须加了loader='laya'的才允许执行
            if(!one.attributes) continue;
            if( one.tagName==='script' && one.attributes.loader!='laya') continue;
        }
        var t = document.createElement(one.tagName);
        //失败了重刷下页面试试
        t.onerror=function(){
            if(window["onLayaInitError"])
            {
                window["onLayaInitError"]("Load script error");
            }
           
        }
        for (var attr in one.attributes) {
            t[attr] = one.attributes[attr];
        }
        document.head.appendChild(t);
    }
    if (layaMeta) setOrientation(layaMeta.screenorientation);
    var onloadScipt = document.createElement("script");
    onloadScipt.text = "window.onload&&window.onload()";
}

async function updateDcc(cache:AppCache,urlpath:string) {
    var isDccOk=true;
    cache.setResourceID('appurl', urlpath);
    var curassets = cache.getResourceID('netassetsid');
    var assetsidStr =<string>( await asyncs.downloadSync(urlpath + 'update/assetsid.txt?rand=' + Math.random() * Date.now(), false, null));
    console.log("assetsid old:"+curassets+"  new:"+assetsidStr);
    if (!assetsidStr) {
        //对于发布版，这个功能还是不要了把。一旦下载错误，就会导致都重新下载，在网络不好的情况下，恶化了实际表现。
        //如果没有assetsid，则去掉校验功能。
        //cache.saveFileTable('');
        //cache.setResourceID('netassetsid', '');	//去掉，保证下次能正确更新
        //cache = new AppCache(urlpath);
        if(curassets && curassets!="")//如果之前就有dcc，现在下载不到，肯定是网络问题，不能直接进游戏，不然会有问题
        {
            if(window["onLayaInitError"])
            {
                isDccOk=false;
                window["onLayaInitError"]("Update DCC get assetsid error");
            }
        }
    } else {
        if (curassets != assetsidStr) {
            log('need update;');
            var txtdcc = '';
            var bindcc = await asyncs.downloadSync(urlpath + 'update/filetable.bin?' + assetsidStr, true, null);
            if (!bindcc || !(bindcc instanceof ArrayBuffer)) {
                txtdcc = <string>(await asyncs.downloadSync(urlpath + 'update/filetable.txt?' + assetsidStr, false, null));
            } else {
                if (bindcc.byteLength % 8 != 0) {
                    log('下载的的filetable.bin的长度不对。是不是错了。');
                } else {
                    var v = new Uint32Array(bindcc);
                    if (v[0] != 0xffeeddcc || v[1] != 1) {//需要校验
                        //onloadfterr();
                        log('dcc.bin file err!');
                    } else {
                        if (v[2] == 0x00ffffff) {
                            //从v[4]开始是32个字节的md5
                            var stp = (4 + 8) / 2;
                            //var md5int = v.slice(4, 12);//支持低版本JSC
                            var md5int = new Uint8Array(v.buffer, 16, 32);
                            //var md5char = new Uint8Array(md5int.buffer);//支持低版本JSC
                            //var so = String.fromCharCode.apply(null, md5char);//支持低版本JSC
                            var so = String.fromCharCode.apply(null, md5int);
                            console.log('--------------------------------------------')
                            console.log('so=' + so);
                            console.log('netid=' + assetsidStr);
                            if (so == assetsidStr) {
                                //一致了
                                for (var ii = stp, isz = v.length / 2; ii < isz; ii++)
                                    txtdcc += v[ii * 2].toString(16) + ' ' + v[ii * 2 + 1].toString(16) + '\n';
                            }
                        } else {
                            console.log('----------------old format');
                            for (var ii = 1, isz = v.length / 2; ii < isz; ii++)
                                txtdcc += v[ii * 2].toString(16) + ' ' + v[ii * 2 + 1].toString(16) + '\n';
                        }
                    }
                }
            }
            if (txtdcc&&txtdcc.length > 0) {
                cache.saveFileTable(txtdcc);
                window.appcache = cache = new AppCache(urlpath);
                //设置新的id。只有写完filetable之后才能写这个
                cache.setResourceID('netassetsid', assetsidStr);
            } else {
                //没有得到任何dcc TODO
                if(window["onLayaInitError"])
                {
                     isDccOk=false;
                    window["onLayaInitError"]("Update DCC get filetable error");
                }
            }
        }
    }
    return isDccOk;
}


/**
 * 
 * @param data json字符串
 */
function startApp(data: string) {
    var jsonobj:{scripts:string[],screenorientation:string,screenOrientation:string,}=null;
    try{
        jsonobj = JSON.parse(data);
    }catch(e){
        console.log("Error:start page parse error! \n "+data);
        //TODO 通知全局错误
        return;
    }

    jsonobj.scripts.forEach((v)=>{
        var t = document.createElement("script");
        t["src"]=v;
        t.onerror=function(){
            if(window["onLayaInitError"])
            {
                window["onLayaInitError"]("Load script error");
            }
           
        }
        document.head.appendChild(t);
    });
    if (jsonobj.screenOrientation) setOrientation(jsonobj.screenOrientation);
    else if (jsonobj.screenorientation) setOrientation(jsonobj.screenorientation);
    else setOrientation("sensor_landscape");
    document.createElement("script").text="window.onload&&window.onload()";
}


async function loadUrl(url: string) {
    if(url==='http://stand.alone.version/index.html')
        _inline = false;

    if( !_inline ){
        url = 'http://stand.alone.version/index.html';
    }
    console.log("loadURL:" + url);
    if (history.length <= 0) {
        history._push(url);
    }
    if (url.length < 2) return;
    location.setHref(url);
    var urlpath = location.fullpath + '/';
    var cache = window.appcache = new AppCache(urlpath);//这时候会加载资源索引，因此如果更新了索引，必须重新创建。
    //为了让启动html也使用缓存，需要在loadUrl刚开始的时候就创建appcache对象。
    document.loadCookie();
    await initFreeType();   //TODO 如果下载这里相当于会卡住。
    try{
        require("config");
    }
    catch(e)
    {
        
    }
    var isDccOk=true;

    //dcc
    if(_inline){
        isDccOk = await updateDcc(cache,urlpath);
        if(!isDccOk){
            console.log("init dcc fail");
            return;
        }
    }

    var data: string = await asyncs.loadText(url);
    for(var n=0; n<3 && !data; n++){
        data = await asyncs.loadText(url);
    }
    if(!data){       
        if (window["loadingView"]) {
            window["loadingView"].setFontColor("#FF0000");
            window["loadingView"].setTips(['网络异常，请检查您的网络或与开发商联系。']);
        }
        data = cache.loadCachedURL(url);
        if(!data || data.length <= 0)
                 if(window["onLayaInitError"])
                {
                    window["onLayaInitError"]("Load start url error");
                }
            return ;
    }    

    console.log("");//此操作在ios下 能起很大的作用  如果注释掉就会非法 不信你试试 但是最后还是得看看是鸡巴毛原因 TODO
    //var extName = url.substr(url.lastIndexOf('.')).toLowerCase();
    var extName = location.pathname.substr(location.pathname.lastIndexOf('.')).toLowerCase();
    if (extName === '.js') {
        window.eval(data + `
        window.onload&&window.onload();
        //@ sourceURL=${url}
        `);
    }else if(extName==='.json'){
        startApp(data);
    }
    else {
        initHtml(data);
    }
    
    if (window["loadingView"] && window["loadingView"].loadingAutoClose) {
        window["loadingView"].hideLoadingView();
    }
}

window.document.addEventListener('keydown', function (e: KeyboardEvent) {
    switch (e.keyCode) {
        case 115://F4
            //urlBack();
            simAndroidDeviceLosted();
            break;
        case 116://F5
            reloadJS(true);
            break;
        case 117://F6
            history.back();
            //simAndroidDeviceLosted();
            break;
        case 118://F7
            //conch.printAllMemorySurvey("","http://10.10.20.114:8889/upload","layabox");
            break;
        case 119://F8
            //conch.printTextureManagerInfo();
            break;
        case 120:
            gc();
            break;
    }
});
window.loadConchUrl = loadUrl;


/**
 * 更新。
 * @param {string} url  zip下载地址
 * @param {(event:string,downloadPercent:number,curfile:string)=>boolean} onEvent  事件的回调。
 *      事件名称：
 *          'downloading' 下载中，这时候downloadPercent有值
 *          'downloadError' 下载错误
 *          'downloadOK' 下载成功。
 *          'updating'  更新中，这时候 curfile有值，表示正在更新的文件
 *          'updateError' curfile更新错误。因为curfile不在dcc列表，或者文件内容与dcc内容不一致。少量更新错误可以忽略，因为在实际使用的时候还是会下载
 *          'unknownError' 
 * @param {function(localfile:string):void} onEnd 更新完成的回调
 */
window['updateByZip'] =function(url, onEvent, onEnd){
    var cachePath = conch.getCachePath();
    var localfile =  cachePath+url.substr(url.lastIndexOf('/'));
    /**
     * 下载文件，保存到localfile中。
     */
    downloadBigFile(url,localfile, 
        //进度回调
        (total:number,now:number,speed:number)=>{
            onEvent('downloading',Math.floor((now/total)*100),null);
            return false;
        },
        //完成回调
        (curlret:number,httpret:number)=>{
            if(curlret!=0 || httpret<200||httpret>=300){
                onEvent('downloadError');
                //throw 'download error';
            }else{
                onEvent('downloadOK');
                //let md5 = calcmd5(fs_readFileSync(localfile));
                //console.log('md5='+md5);
                var zip = new ZipFile();
                if(zip.setSrc(localfile)){
                    zip.forEach((id,name,dir,sz)=>{
                        if(!dir){
                            var buf = zip.readFile(id);
                            //console.log("update "+name);
                            var fid = window.appcache.hashstr('/'+name);    //TODO 测试
                            if(window.appcache.updateFile(fid,0,buf,false)){
                                //更新成功
                                onEvent('updating',null,name);
                            }else{
                                //更新失败。
                                /**
                                 * 更新失败是指没有对应的dcc信息，或者文件内容与dcc中的不一致。
                                 * 这个可以看做不是致命错误。
                                 */
                                onEvent('updateError',null,name);
                                //console.log("update error :"+name);
                            }
                        }
                    });
                    zip.close();
                    if(onEnd) onEnd(localfile);
                }else{
                    console.log( 'set zip src error!');
                    onEvent('unknownError');
                }
            }
        },10,100000000);
}


loadUrl(conch.presetUrl || "http://runtime.layabox.com/layaplayer/index.html");