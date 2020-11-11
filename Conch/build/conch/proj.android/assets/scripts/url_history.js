//url对象
(function(exports){
	function URL(url){
		this._href=url.trim();
		this.isAbs=function(){
			return this._href.indexOf('://')>0;
		}
		/*
		get href:function(){}
		get host=function(){}
		get pathname=function(){}
		get search = function(){}
		get path = function(){}
		get query=function(){}
		*/
		this.resolve=function(from,to){
			//from是路径么
			var tfrom=from.trim();

			var qst=tfrom.indexOf('?');
			if(qst>0)
				tfrom=tfrom.substr(0,qst);

			if(tfrom[tfrom.length-1]=='/'||tfrom[tfrom.length-1]=='\\'){
				return tfrom+to;
			}else{
				var p = tfrom.lastIndexOf('/');
				var p1 = tfrom.lastIndexOf('\\');
				p=p<p1?p1:p;
				return tfrom.substr(0,p+1)+to;
			}
		}
	}
	exports.URL=URL;
})(window);

//history对象
(function(exports){
	var history=function(){
		this.back=function(){
			urlBack();
		}
		this.forward=function(){
			urlForward();
		}
	}
	exports.history=new history();
})(window)

