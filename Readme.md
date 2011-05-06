CGI/FastCGI Wrapper for nekoVM and the haxe cpp target
======================================================

Current status
--------------
* All methods implemented.

Differences to haxe Web API:
----------------------------
* If you using FastCGI you should use `Web.cacheModule()`, otherwise the module will be restarted after every request, which is quite slow.
* If you using CGI if does not matter if you use `Web.cacheModule()`.
* You need to print some data, when sending headers only, call `Lib.print('');` after setting all headers.

Status
------

*Not Implemented*:

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
* getAuthorization
* getHostName
* flush
* getMultipart
* parseMultipart
* isModNeko is always `false`
* isTora is always `false`

