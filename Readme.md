CGI/FastCGI Wrapper for nekoVM and the haxe cpp target
======================================================

Current status
--------------
Basic requests work. You can output data and set headers.

Differences to haxe Web API:
----------------------------
* You need to call Web.init() before you set the first headers or sent any other output. Web.init() waits until the first request will arive and then continues.
* If you using FastCGI you should use Web.cacheModule(), otherwise the module will be restarted after every request, which is quite slow
* If you using CGI if does not matter if you use Web.cacheModule().

Status
------

*Not Implemented*:
* getParams
* getParamValues
* getHostName
* getClientIP
* getURI
* setReturnCode
* getClientHeader
* getClientHeaders
* getParamsString
* getPostData
* getCookies
* setCookie
* getAuthorization
* getCwd
* getMultipart
* parseMultipart
* flush
* getMethod
* logMessage

*Implemented*:
* redirect
* setHeader
* cacheModule
* isModNeko is always `false`
* isTora is always `false`

