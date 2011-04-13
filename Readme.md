CGI/FastCGI Wrapper for nekoVM and the haxe cpp target
======================================================

Current status
--------------
Basic requests work. You can output data and set headers.

Differences to haxe Web API:
----------------------------
* You need to call `Web.init()` before you set the first headers or sent any other output. `Web.init()` waits until the first request will arive and then continues. Every output before `Web.init()` will result in an error.
* If you using FastCGI you should use `Web.cacheModule()`, otherwise the module will be restarted after every request, which is quite slow
* If you using CGI if does not matter if you use `Web.cacheModule()`.
* `HTTP-Header` are slightly different then in the haXe native neko API. *Example:* "User-Agent" in native Web API will become "USER\_AGENT" (case-insensitive)
* haxe native `Web.getClientHeaders()` return non HTTP header like Content-Type and Content-Length
* You could call `Web.getPostData()` even if it is a GET request
* `Web.flush()` do nothing.

Status
------

*Not Implemented*:

* getHostName
* getAuthorization
* getMultipart
* parseMultipart

*Implemented*:

* redirect
* setHeader
* cacheModule
* getClientIP
* getURI
* getMethod
* getClientHeader
* getClientHeaders
* setReturnCode
* getPostData
* getParamsString
* getParams
* getParamValues
* logMessage
* getCookies
* setCookie
* getCwd
* flush do nothing
* isModNeko is always `false`
* isTora is always `false`

