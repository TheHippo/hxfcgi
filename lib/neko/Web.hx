/*
 * Copyright (c) 2005, The haXe Project Contributors
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   - Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   - Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE HAXE PROJECT CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE HAXE PROJECT CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 */
package neko;

/**
	This class is used for accessing the local Web server and the current
	client request and informations.
**/

using StringTools;
using Lambda;

class Web {
	
	static var hxfcgi_createRequest = Web.load("hxfcgi_create_request",0);
	static var hxfcgi_addHeader = Web.load("hxfcgi_add_header",3);
	public static var hxfcgi_print = Web.load("hxfcgi_print",2);
	static var hxfcgi_cacheModule = Web.load("hxfcgi_cache_module",1);
	static var hxfcgi_getClientIP = Web.load("hxfcgi_get_client_ip",1);
	static var hxfcgi_getURI = Web.load("hxfcgi_get_uri",1);
	static var hxfcgi_getAllHeaders = Web.load("hxfcgi_get_all_headers",1);
	static var hxfcgi_getHeader = Web.load("hxfcgi_get_header",2);
	static var hxfcgi_getMethod = Web.load("hxfcgi_get_method",1);
	static var hxfcgi_setReturnCode = Web.load("hxfcgi_set_return_code",2);
	static var hxfcgi_getPostData = Web.load ("hxfcgi_get_post_data",1);
	static var hxfcgi_getParamsString = Web.load("hxfcgi_get_params_string",1);
	static var hxfcgi_getParams = Web.load("hxfcgi_get_params",1);
	static var hxfcgi_log = Web.load("hxfcgi_log",2);
	static var hxfcgi_flush = Web.load("hxfcgi_flush",1);
	static var hxfcgi_getCookies = Web.load("hxfcgi_get_cookies",1);
	static var hxfcgi_setCookie = Web.load("hxfcgi_set_cookie",3);
	static var hxfcgi_parseMultipart = Web.load("hxfcgi_parse_multipart_neko",3);
	static var _base_decode = Lib.load("std","base_decode",2);


	public static var request:Dynamic = init();
	
	public static function init() {
		haxe.Log.trace = function(v:Dynamic,?info:haxe.PosInfos) {
			Lib.print(info.fileName+":"+info.lineNumber+": "+Std.string(v)+"\n");
		}
		return Web.hxfcgi_createRequest();
	}
	
	/**
		Returns the GET and POST parameters.
	**/
	public static function getParams() {
		var ret = new Hash<String>();
		var a:Array<String> = Lib.nekoToHaxe(Web.hxfcgi_getParams(Web.request));
		for (x in 0...(a.length >> 1))
			if(a[2*x].length > 0) ret.set(a[2*x],(a[2*x+1].length > 0 ) ? StringTools.urlDecode(a[2*x+1]) : null);
		return ret;
	}

	/**
		Returns an Array of Strings built using GET / POST values.
		If you have in your URL the parameters [a[]=foo;a[]=hello;a[5]=bar;a[3]=baz] then
		[neko.Web.getParamValues("a")] will return [["foo","hello",null,"baz",null,"bar"]]
	**/
	public static function getParamValues( param : String ) : Array<String> {
		var reg = new EReg("^"+param+"(\\[|%5B)([0-9]*?)(\\]|%5D)=(.*?)$", "");
		var res = new Array<String>();
		var explore = function(data:String){
			if (data == null || data.length == 0)
				return;
			for (part in data.split("&")){
				if (reg.match(part)){
					var idx = reg.matched(2);
					var val = StringTools.urlDecode(reg.matched(4));
					if (idx == "")
						res.push(val);
					else
						res[Std.parseInt(idx)] = val;
				}
			}
		}
		explore(StringTools.replace(getParamsString(), ";", "&"));
		explore(getPostData());
		if (res.length == 0)
			return null;
		return res;
	}

	/**
		Returns the local server host name
	**/
	public static function getHostName() {
		return  Sys.getEnv('SERVER_NAME');
	}

	/**
		Surprisingly returns the client IP address.
	**/
	public static function getClientIP():String {
		return Lib.nekoToHaxe(Web.hxfcgi_getClientIP(Web.request));
	}

	/**
		Returns the original request URL (before any server internal redirections)
	**/
	public static function getURI():String {
		return Lib.nekoToHaxe(Web.hxfcgi_getURI(Web.request));
	}

	/**
		Tell the client to redirect to the given url ("Location" header)
	**/
	public static function redirect( url : String ) {
		Web.setHeader("Location",url);
	}

	/**
		Set an output header value. If some data have been printed, the headers have
		already been sent so this will raise an exception.
	**/
	public static function setHeader( h : String, v : String ) {
		Web.hxfcgi_addHeader(Web.request,Lib.haxeToNeko(h),Lib.haxeToNeko(v));
	}

	/**
		Set the HTTP return code. Same remark as setHeader.
	**/
	public static function setReturnCode( r : Int ) {
		Web.hxfcgi_setReturnCode(Web.request,r);
	}

	/**
		Retrieve a client header value sent with the request.
	**/
	public static function getClientHeader( k : String ) {
		return Lib.nekoToHaxe(Web.hxfcgi_getHeader(Web.request,Lib.haxeToNeko(k)));
	}

	/**
		Retrieve all the client headers.
	**/
	public static function getClientHeaders(): List<{ value : String, header : String }> {
		var ret = new List<{value:String,header:String}>();
		var header:Array<String> = Lib.nekoToHaxe(Web.hxfcgi_getAllHeaders(Web.request));
		for (h in header) {
			var reg = ~/([^=]*)=(.*)/;
			if(reg.match(h)) {
				ret.add({
					header:reg.matched(1),
					value:reg.matched(2)
				});
			}
		}		
		return ret;
	}

	/**
		Returns all the GET parameters String
	**/
	public static function getParamsString():String {
		return Lib.nekoToHaxe(Web.hxfcgi_getParamsString(Web.request));
	}

	/**
		Returns all the POST data. POST Data is always parsed as
		being application/x-www-form-urlencoded and is stored into
		the getParams hashtable. POST Data is maximimized to 256K
		unless the content type is multipart/form-data. In that
		case, you will have to use [getMultipart] or [parseMultipart]
		methods.
	**/
	public static function getPostData():Null<String> {
		var ret:String = Lib.nekoToHaxe(Web.hxfcgi_getPostData(Web.request));
		if(ret.length == 0) return null;
		return ret;
	}

	/**
		Returns an hashtable of all Cookies sent by the client.
		Modifying the hashtable will not modify the cookie, use setCookie instead.
	**/
	public static function getCookies() {
		var p:Array<Dynamic> = Lib.nekoToHaxe(Web.hxfcgi_getCookies(Web.request));
                var h = new Hash<String>();
                while( p != null ) {
                        h.set(p[0],p[1]);
                        p = p[2];
                }
                return h;
	}


	/**
		Set a Cookie value in the HTTP headers. Same remark as setHeader.
	**/
	public static function setCookie( key : String, value : String, ?expire: Date, ?domain: String, ?path: String, ?secure: Bool ) {
		var buf = new StringBuf();
                buf.add(Lib.haxeToNeko(value));
                if( expire != null ) addPair(buf, "expires=", DateTools.format(expire, "%a, %d-%b-%Y %H:%M:%S GMT"));
                addPair(buf, "domain=", Lib.haxeToNeko(domain));
                addPair(buf, "path=", Lib.haxeToNeko(path));
                if( secure ) addPair(buf, "secure", "");
                var v = buf.toString();
		Web.hxfcgi_setCookie(Web.request,Lib.haxeToNeko(key),v);
	}

	static function addPair( buf : StringBuf, name, value ) {
		if( value == null ) return;
		buf.add("; ");
		buf.add(name);
		buf.add(value);
	}

	/**
		Returns an object with the authorization sent by the client (Basic scheme only).
	**/
	public static function getAuthorization() : { user : String, pass : String } {
		var h = getClientHeader("Authorization");
		var reg = ~/^Basic ([^=]+)=*$/;
		if( h != null && reg.match(h) ){
			var val = reg.matched(1);
			untyped val = new String(_base_decode(Lib.haxeToNeko(val),Lib.haxeToNeko("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/")));
			var a = val.split(":");
			if( a.length != 2 ){
				throw "Unable to decode authorization.";
			}
			return {user: a[0],pass: a[1]};
		}
		return null;
	}

	/**
		Get the current script directory in the local filesystem.
	**/
	public static function getCwd():String {
		var f =  Sys.getEnv('SCRIPT_FILENAME');
		if(f == null || f.length == 0) return null; 
		return f.substr(0,f.lastIndexOf('/')+1);
	}

	/**
		Set the main entry point function used to handle requests.
		Setting it back to null will disable code caching.
	**/
	public static function cacheModule( f : Void -> Void ) {
		f();
		var r = function(request:Dynamic) {
			Web.request = request;
			f();
		};
		Web.hxfcgi_cacheModule(r);
	}

	/**
		Get the multipart parameters as an hashtable. The data
		cannot exceed the maximum size specified.
	**/
	public static function getMultipart( maxSize : Int ) : Hash<String> {
		var h = new Hash();
		var buf : haxe.io.BytesBuffer = null;
		var curname = null;
		parseMultipart(function(p,_) {
			if( curname != null )
				h.set(curname,neko.Lib.stringReference(buf.getBytes()));
			curname = p;
			buf = new haxe.io.BytesBuffer();
			maxSize -= p.length;
			if( maxSize < 0 )
				throw "Maximum size reached";
		},function(str,pos,len) {
			maxSize -= len;
			if( maxSize < 0 )
				throw "Maximum size reached";
			buf.addBytes(str,pos,len);
		});
		if( curname != null )
			h.set(curname,neko.Lib.stringReference(buf.getBytes()));
		return h;
	}

	/**
		Parse the multipart data. Call [onPart] when a new part is found
		with the part name and the filename if present
		and [onData] when some part data is readed. You can this way
		directly save the data on hard drive in the case of a file upload.
	**/
	public static function parseMultipart( onPart : String -> String -> Void, onData : haxe.io.Bytes -> Int -> Int -> Void ) : Void {
		Web.hxfcgi_parseMultipart(Web.request,
			function(p,f) { onPart(new String(p),if( f == null ) null else new String(f)); },
			function(buf,pos,len) { onData(untyped new haxe.io.Bytes(__dollar__ssize(buf),buf),pos,len); }
		);
	}

	/**
		Flush the data sent to the client. 
	**/
	public static function flush() : Void {
		Web.hxfcgi_flush(Web.request);
	}

	/**
		Get the HTTP method used by the client. This api requires Neko 1.7.1+
	**/
	public static function getMethod() : String {
		return Lib.nekoToHaxe(Web.hxfcgi_getMethod(Web.request));
	}

	/**
		Write a message into the web server log file. This api requires Neko 1.7.1+
	**/
	public static function logMessage( msg : String ) {
		Web.hxfcgi_log(Web.request,Lib.haxeToNeko(msg));
	}
	
	static function load(name,narg):Dynamic {
		try {
			return Lib.load("hxfcgi",name,narg);
		}
		catch (e:Dynamic) {
			return Lib.load(Sys.getCwd()+"hxfcgi",name,narg);
		}
		return null;
	}

	public static var isModNeko = false;
	public static var isTora = false;


}
