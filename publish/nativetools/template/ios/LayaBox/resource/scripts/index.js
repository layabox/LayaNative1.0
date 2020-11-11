"use strict";
var __awaiter = (this && this.__awaiter) || function (thisArg, _arguments, P, generator) {
    function adopt(value) { return value instanceof P ? value : new P(function (resolve) { resolve(value); }); }
    return new (P || (P = Promise))(function (resolve, reject) {
        function fulfilled(value) { try { step(generator.next(value)); } catch (e) { reject(e); } }
        function rejected(value) { try { step(generator["throw"](value)); } catch (e) { reject(e); } }
        function step(result) { result.done ? resolve(result.value) : adopt(result.value).then(fulfilled, rejected); }
        step((generator = generator.apply(thisArg, _arguments || [])).next());
    });
};
var __generator = (this && this.__generator) || function (thisArg, body) {
    var _ = { label: 0, sent: function() { if (t[0] & 1) throw t[1]; return t[1]; }, trys: [], ops: [] }, f, y, t, g;
    return g = { next: verb(0), "throw": verb(1), "return": verb(2) }, typeof Symbol === "function" && (g[Symbol.iterator] = function() { return this; }), g;
    function verb(n) { return function (v) { return step([n, v]); }; }
    function step(op) {
        if (f) throw new TypeError("Generator is already executing.");
        while (_) try {
            if (f = 1, y && (t = op[0] & 2 ? y["return"] : op[0] ? y["throw"] || ((t = y["return"]) && t.call(y), 0) : y.next) && !(t = t.call(y, op[1])).done) return t;
            if (y = 0, t) op = [op[0] & 2, t.value];
            switch (op[0]) {
                case 0: case 1: t = op; break;
                case 4: _.label++; return { value: op[1], done: false };
                case 5: _.label++; y = op[1]; op = [0]; continue;
                case 7: op = _.ops.pop(); _.trys.pop(); continue;
                default:
                    if (!(t = _.trys, t = t.length > 0 && t[t.length - 1]) && (op[0] === 6 || op[0] === 2)) { _ = 0; continue; }
                    if (op[0] === 3 && (!t || (op[1] > t[0] && op[1] < t[3]))) { _.label = op[1]; break; }
                    if (op[0] === 6 && _.label < t[1]) { _.label = t[1]; t = op; break; }
                    if (t && _.label < t[2]) { _.label = t[2]; _.ops.push(op); break; }
                    if (t[2]) _.ops.pop();
                    _.trys.pop(); continue;
            }
            op = body.call(thisArg, _);
        } catch (e) { op = [6, e]; y = 0; } finally { f = t = 0; }
        if (op[0] & 5) throw op[1]; return { value: op[0] ? op[1] : void 0, done: true };
    }
};
Object.defineProperty(exports, "__esModule", { value: true });
window._conchInfo = { version: '2.1.3.1' };
var _inline = !conchConfig.localizable;
console.log('======================================================  ');
console.log('             LAYA CONCH            ');
console.log('     runtimeversion:' + conchConfig.getRuntimeVersion());
console.log('          jsversion:' + window._conchInfo.version);
console.log('             isplug:' + conchConfig.getIsPlug());
console.log('======================================================');
function log(m) {
    console.log(m);
}
if (conchConfig.getOS() == "Conch-ios") {
    require('promise');
}
var asyncs = require("async");
function initFreeType() {
    return __awaiter(this, void 0, void 0, function () {
        var sOS, bRet, sTempFontPath, assetFontData, fSystemVersion, vDefaultStrings, vDefaultStrings, data;
        return __generator(this, function (_a) {
            switch (_a.label) {
                case 0:
                    sOS = conchConfig.getOS();
                    bRet = false;
                    sTempFontPath = conch.getCachePath() + "/runtimeFont/";
                    if (!fs_exists(sTempFontPath)) {
                        fs_mkdir(sTempFontPath);
                    }
                    sTempFontPath += "layabox.ttf";
                    bRet = conch.initFreeTypeDefaultFontFromFile(sTempFontPath);
                    if (bRet == false) {
                        assetFontData = conch.readFileFromAsset('font/layabox.ttf', 'raw');
                        if (assetFontData) {
                            fs_writeFileSync(sTempFontPath, assetFontData);
                            bRet = conch.initFreeTypeDefaultFontFromFile(sTempFontPath);
                        }
                    }
                    if (!bRet) {
                        if (sOS == "Conch-window") {
                            bRet = conch.initFreeTypeDefaultFontFromFile("C:/Windows/Fonts/simhei.ttf");
                        }
                        else if (sOS == "Conch-android") {
                            fSystemVersion = navigator.sv;
                            if (fSystemVersion >= 2.0 && fSystemVersion < 5.0) {
                                bRet = conch.initFreeTypeDefaultFontFromFile("/system/fonts/DFHEIA5A.ttf");
                                if (bRet == false) {
                                    bRet = conch.initFreeTypeDefaultFontFromFile("/system/fonts/DroidSansFallback.ttf");
                                }
                            }
                            else if (fSystemVersion >= 5.0 && fSystemVersion < 6.0) {
                                vDefaultStrings = [];
                                vDefaultStrings.push("/system/fonts/NotoSansHans-Regular.otf");
                                vDefaultStrings.push("/system/fonts/Roboto-Regular.ttf");
                                bRet = conch.initFreeTypeDefaultFontFromFile(vDefaultStrings.join('|'));
                            }
                            else if (fSystemVersion >= 6.0 && fSystemVersion < 7.0) {
                                vDefaultStrings = [];
                                vDefaultStrings.push("/system/fonts/NotoSansSC-Regular.otf");
                                vDefaultStrings.push("/system/fonts/Roboto-Regular.ttf");
                                bRet = conch.initFreeTypeDefaultFontFromFile(vDefaultStrings.join('|'));
                            }
                            else if (fSystemVersion >= 7.0 && fSystemVersion < 8.0) {
                                bRet = false;
                            }
                        }
                        else if (sOS == "Conch-ios") {
                            bRet = conch.initFreeTypeDefaultFontFromFile("");
                        }
                    }
                    if (!(bRet == false)) return [3, 4];
                    log('字体初始化失败，从网络下载字体...');
                    return [4, asyncs.downloadSync(location.fullpath + '/font/simhei.ttf', true, null)];
                case 1:
                    data = (_a.sent());
                    if (!!data) return [3, 3];
                    return [4, asyncs.downloadSync('http://runtime.layabox.com/font/simhei.ttf', true, null)];
                case 2:
                    data = (_a.sent());
                    _a.label = 3;
                case 3:
                    if (!data) {
                        alert('下载字体失败。 ');
                        return [2];
                    }
                    fs_writeFileSync(sTempFontPath, data);
                    bRet = conch.initFreeTypeDefaultFontFromFile(sTempFontPath);
                    _a.label = 4;
                case 4:
                    if (!bRet) {
                        log('字体初始化失败。');
                    }
                    return [2];
            }
        });
    });
}
function setOrientation(s) {
    var nameToVal = {
        landscape: 0, portrait: 1, user: 2, behind: 3, sensor: 4, nosensor: 5, sensor_landscape: 6,
        sensor_portrait: 7, reverse_landscape: 8, reverse_portrait: 9, full_sensor: 10,
    };
    var nOri = (function (name) {
        try {
            var n = nameToVal[name];
            return n || 0;
        }
        catch (e) {
            return 0;
        }
    })(s);
    conchConfig.setScreenOrientation(nOri);
    ;
}
function initHtml(data) {
    var tag = new RegExp("<(/|)([\\w.]+)(?: +|)?([^>]*?)(/|)>", "g");
    var res, attrs;
    var scripts = [];
    var all = [];
    var lastTag = {};
    var lastIndex = 0;
    var _comments = new RegExp("<!--[\\s\\S]*?-->", "g");
    var _blank = new RegExp("^[^\\S]*|[^\\S]*$", "g");
    var _attributes = new RegExp("(\\w+)\\s*=\\s*(\"[^\"]*\"|'[^']*')", "g");
    data = data.replace(_comments, "");
    var layaMeta = null;
    var supportedTag = ['html', 'title', 'head', 'body', 'script', 'meta'];
    while (res = tag.exec(data)) {
        var temp = {};
        temp.tagName = res[2];
        if (res[3]) {
            temp.attributes = {};
            while ((attrs = _attributes.exec(res[3])) != null) {
                temp.attributes[attrs[1]] = attrs[2].substring(1, attrs[2].length - 1);
            }
        }
        if (temp.tagName === 'meta' && temp.attributes.name === 'laya')
            layaMeta = temp.attributes;
        if (supportedTag.indexOf(temp.tagName) < 0) {
            alert('LayaPlayer不支持的标签：\n' + res[0] + '\n\n(具体请参考LayaBox网站文档)');
        }
        if (!res[1] && !res[4])
            temp.type = -1;
        else if (!res[1] && res[4])
            temp.type = 0;
        else if (res[1] && !res[4])
            temp.type = 1;
        else {
            alert("解析标签出错</" + temp.tagName + "/>");
            return;
        }
        temp.index = res.index;
        all.push(temp);
        if (temp.tagName == "script" || temp.tagName == "meta") {
            if (temp.type == -1) {
                if (lastTag.tagName == temp.tagName && lastTag.type == -1) {
                    if (temp.tagName == "meta") {
                        scripts.push(temp);
                    }
                    else {
                        alert("解析标签" + temp.tagName + "出错,连续两个script标签可能是 脚本中包含<script>");
                        return;
                    }
                }
                scripts.push(temp);
            }
            else if (temp.type == 1) {
                if (lastTag.tagName == temp.tagName && lastTag.type == -1) {
                    var text = data.substring(lastIndex, temp.index);
                    text = text.replace(_blank, "");
                    if (text) {
                        temp = scripts.pop();
                        temp.type = 1;
                        temp.attributes = temp.attributes || {};
                        temp.attributes.text = text;
                        scripts.push(temp);
                    }
                }
            }
            else {
                if (temp.attributes)
                    scripts.push(temp);
            }
        }
        lastIndex = res.index + res[0].length;
        lastTag = temp;
    }
    var chkjs = layaMeta && layaMeta.layajsprotect && layaMeta.layajsprotect == 'true';
    for (var d in scripts) {
        var one = scripts[d];
        if (chkjs) {
            if (!one.attributes)
                continue;
            if (one.tagName === 'script' && one.attributes.loader != 'laya')
                continue;
        }
        var t = document.createElement(one.tagName);
        t.onerror = function () {
            if (window["onLayaInitError"]) {
                window["onLayaInitError"]("Load script error");
            }
        };
        for (var attr in one.attributes) {
            t[attr] = one.attributes[attr];
        }
        document.head.appendChild(t);
    }
    if (layaMeta)
        setOrientation(layaMeta.screenorientation);
    var onloadScipt = document.createElement("script");
    onloadScipt.text = "window.onload&&window.onload()";
}
function updateDcc(cache, urlpath) {
    return __awaiter(this, void 0, void 0, function () {
        var isDccOk, curassets, assetsidStr, txtdcc, bindcc, v, stp, md5int, so, ii, isz, ii, isz;
        return __generator(this, function (_a) {
            switch (_a.label) {
                case 0:
                    isDccOk = true;
                    cache.setResourceID('appurl', urlpath);
                    curassets = cache.getResourceID('netassetsid');
                    return [4, asyncs.downloadSync(urlpath + 'update/assetsid.txt?rand=' + Math.random() * Date.now(), false, null)];
                case 1:
                    assetsidStr = (_a.sent());
                    console.log("assetsid old:" + curassets + "  new:" + assetsidStr);
                    if (!!assetsidStr) return [3, 2];
                    if (curassets && curassets != "") {
                        if (window["onLayaInitError"]) {
                            isDccOk = false;
                            window["onLayaInitError"]("Update DCC get assetsid error");
                        }
                    }
                    return [3, 7];
                case 2:
                    if (!(curassets != assetsidStr)) return [3, 7];
                    log('need update;');
                    txtdcc = '';
                    return [4, asyncs.downloadSync(urlpath + 'update/filetable.bin?' + assetsidStr, true, null)];
                case 3:
                    bindcc = _a.sent();
                    if (!(!bindcc || !(bindcc instanceof ArrayBuffer))) return [3, 5];
                    return [4, asyncs.downloadSync(urlpath + 'update/filetable.txt?' + assetsidStr, false, null)];
                case 4:
                    txtdcc = (_a.sent());
                    return [3, 6];
                case 5:
                    if (bindcc.byteLength % 8 != 0) {
                        log('下载的的filetable.bin的长度不对。是不是错了。');
                    }
                    else {
                        v = new Uint32Array(bindcc);
                        if (v[0] != 0xffeeddcc || v[1] != 1) {
                            log('dcc.bin file err!');
                        }
                        else {
                            if (v[2] == 0x00ffffff) {
                                stp = (4 + 8) / 2;
                                md5int = new Uint8Array(v.buffer, 16, 32);
                                so = String.fromCharCode.apply(null, md5int);
                                console.log('--------------------------------------------');
                                console.log('so=' + so);
                                console.log('netid=' + assetsidStr);
                                if (so == assetsidStr) {
                                    for (ii = stp, isz = v.length / 2; ii < isz; ii++)
                                        txtdcc += v[ii * 2].toString(16) + ' ' + v[ii * 2 + 1].toString(16) + '\n';
                                }
                            }
                            else {
                                console.log('----------------old format');
                                for (ii = 1, isz = v.length / 2; ii < isz; ii++)
                                    txtdcc += v[ii * 2].toString(16) + ' ' + v[ii * 2 + 1].toString(16) + '\n';
                            }
                        }
                    }
                    _a.label = 6;
                case 6:
                    if (txtdcc && txtdcc.length > 0) {
                        cache.saveFileTable(txtdcc);
                        window.appcache = cache = new AppCache(urlpath);
                        cache.setResourceID('netassetsid', assetsidStr);
                    }
                    else {
                        if (window["onLayaInitError"]) {
                            isDccOk = false;
                            window["onLayaInitError"]("Update DCC get filetable error");
                        }
                    }
                    _a.label = 7;
                case 7: return [2, isDccOk];
            }
        });
    });
}
function startApp(data) {
    var jsonobj = null;
    try {
        jsonobj = JSON.parse(data);
    }
    catch (e) {
        console.log("Error:start page parse error! \n " + data);
        return;
    }
    jsonobj.scripts.forEach(function (v) {
        var t = document.createElement("script");
        t["src"] = v;
        t.onerror = function () {
            if (window["onLayaInitError"]) {
                window["onLayaInitError"]("Load script error");
            }
        };
        document.head.appendChild(t);
    });
    if (jsonobj.screenOrientation)
        setOrientation(jsonobj.screenOrientation);
    else if (jsonobj.screenorientation)
        setOrientation(jsonobj.screenorientation);
    else
        setOrientation("sensor_landscape");
    document.createElement("script").text = "window.onload&&window.onload()";
}
function loadUrl(url) {
    return __awaiter(this, void 0, void 0, function () {
        var urlpath, cache, isDccOk, data, n, extName;
        return __generator(this, function (_a) {
            switch (_a.label) {
                case 0:
                    if (url === 'http://stand.alone.version/index.html')
                        _inline = false;
                    if (!_inline) {
                        url = 'http://stand.alone.version/index.html';
                    }
                    console.log("loadURL:" + url);
                    if (history.length <= 0) {
                        history._push(url);
                    }
                    if (url.length < 2)
                        return [2];
                    location.setHref(url);
                    urlpath = location.fullpath + '/';
                    cache = window.appcache = new AppCache(urlpath);
                    document.loadCookie();
                    return [4, initFreeType()];
                case 1:
                    _a.sent();
                    try {
                        require("config");
                    }
                    catch (e) {
                    }
                    isDccOk = true;
                    if (!_inline) return [3, 3];
                    return [4, updateDcc(cache, urlpath)];
                case 2:
                    isDccOk = _a.sent();
                    if (!isDccOk) {
                        console.log("init dcc fail");
                        return [2];
                    }
                    _a.label = 3;
                case 3: return [4, asyncs.loadText(url)];
                case 4:
                    data = _a.sent();
                    n = 0;
                    _a.label = 5;
                case 5:
                    if (!(n < 3 && !data)) return [3, 8];
                    return [4, asyncs.loadText(url)];
                case 6:
                    data = _a.sent();
                    _a.label = 7;
                case 7:
                    n++;
                    return [3, 5];
                case 8:
                    if (!data) {
                        if (window["loadingView"]) {
                            window["loadingView"].setFontColor("#FF0000");
                            window["loadingView"].setTips(['网络异常，请检查您的网络或与开发商联系。']);
                        }
                        data = cache.loadCachedURL(url);
                        if (!data || data.length <= 0)
                            if (window["onLayaInitError"]) {
                                window["onLayaInitError"]("Load start url error");
                            }
                        return [2];
                    }
                    console.log("");
                    extName = location.pathname.substr(location.pathname.lastIndexOf('.')).toLowerCase();
                    if (extName === '.js') {
                        window.eval(data + ("\n        window.onload&&window.onload();\n        //@ sourceURL=" + url + "\n        "));
                    }
                    else if (extName === '.json') {
                        startApp(data);
                    }
                    else {
                        initHtml(data);
                    }
                    if (window["loadingView"] && window["loadingView"].loadingAutoClose) {
                        window["loadingView"].hideLoadingView();
                    }
                    return [2];
            }
        });
    });
}
window.document.addEventListener('keydown', function (e) {
    switch (e.keyCode) {
        case 115:
            simAndroidDeviceLosted();
            break;
        case 116:
            reloadJS(true);
            break;
        case 117:
            history.back();
            break;
        case 118:
            break;
        case 119:
            break;
        case 120:
            gc();
            break;
    }
});
window.loadConchUrl = loadUrl;
window['updateByZip'] = function (url, onEvent, onEnd) {
    var cachePath = conch.getCachePath();
    var localfile = cachePath + url.substr(url.lastIndexOf('/'));
    downloadBigFile(url, localfile, function (total, now, speed) {
        onEvent('downloading', Math.floor((now / total) * 100), null);
        return false;
    }, function (curlret, httpret) {
        if (curlret != 0 || httpret < 200 || httpret >= 300) {
            onEvent('downloadError');
        }
        else {
            onEvent('downloadOK');
            var zip = new ZipFile();
            if (zip.setSrc(localfile)) {
                zip.forEach(function (id, name, dir, sz) {
                    if (!dir) {
                        var buf = zip.readFile(id);
                        var fid = window.appcache.hashstr('/' + name);
                        if (window.appcache.updateFile(fid, 0, buf, false)) {
                            onEvent('updating', null, name);
                        }
                        else {
                            onEvent('updateError', null, name);
                        }
                    }
                });
                zip.close();
                if (onEnd)
                    onEnd(localfile);
            }
            else {
                console.log('set zip src error!');
                onEvent('unknownError');
            }
        }
    }, 10, 100000000);
};
loadUrl(conch.presetUrl || "http://runtime.layabox.com/layaplayer/index.html");
