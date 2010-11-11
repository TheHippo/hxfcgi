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

	public static var request:Dynamic;
	
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

	
	public static function init() {
		Web.request = Web.hxfcgi_createRequest();
		haxe.Log.trace = function(v:Dynamic,?info:haxe.PosInfos) {
			Lib.print(info.fileName+":"+info.lineNumber+": "+Std.string(v)+"\n");
		}
	}
	
	/**
		Returns the GET and POST parameters.
	**/
	public static function getParams() {
		throw "not implemented";
		return null;
	}

	/**
		Returns an Array of Strings built using GET / POST values.
		If you have in your URL the parameters [a[]=foo;a[]=hello;a[5]=bar;a[3]=baz] then
		[neko.Web.getParamValues("a")] will return [["foo","hello",null,"baz",null,"bar"]]
	**/
	public static function getParamValues( param : String ) : Array<String> {
		throw "not implemented";
		return null;
	}

	/**
		Returns the local server host name
	**/
	public static function getHostName() {
		throw "not implemented";
		return null;
	}

	/**
		Surprisingly returns the client IP address.
	**/
	public static function getClientIP():String {
		return Web.hxfcgi_getClientIP(Web.request);
	}

	/**
		Returns the original request URL (before any server internal redirections)
	**/
	public static function getURI():String {
		return Web.hxfcgi_getURI(Web.request);
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
	public static function getPostData():String {
		return Lib.nekoToHaxe(Web.hxfcgi_getPostData(Web.request));
	}

	/**
		Returns an hashtable of all Cookies sent by the client.
		Modifying the hashtable will not modify the cookie, use setCookie instead.
	**/
	public static function getCookies() {
		throw "not implemented";
		return null;
	}


	/**
		Set a Cookie value in the HTTP headers. Same remark as setHeader.
	**/
	public static function setCookie( key : String, value : String, ?expire: Date, ?domain: String, ?path: String, ?secure: Bool ) {
		throw "not implemented";
		return null;
	}

	/**
		Returns an object with the authorization sent by the client (Basic scheme only).
	**/
	public static function getAuthorization() : { user : String, pass : String } {
		throw "not implemented";
		return null;
	}

	/**
		Get the current script directory in the local filesystem.
	**/
	public static function getCwd() {
		throw "not implemented";
		return null;
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
		throw "not implemented";
		return null;
	}

	/**
		Parse the multipart data. Call [onPart] when a new part is found
		with the part name and the filename if present
		and [onData] when some part data is readed. You can this way
		directly save the data on hard drive in the case of a file upload.
	**/
	public static function parseMultipart( onPart : String -> String -> Void, onData : haxe.io.Bytes -> Int -> Int -> Void ) : Void {
		throw "not implemented";
		return null;
	}

	/**
		Flush the data sent to the client. By default on Apache, outgoing data is buffered so
		this can be useful for displaying some long operation progress.
	**/
	public static function flush() : Void {
		throw "not implemented";
		return null;
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
		throw "not implemented";
		return null;
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
