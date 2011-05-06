CGI/FastCGI Wrapper for nekoVM and the haxe cpp target
======================================================

Documentation
--------------
For more documentation visit the [hxfcgi](http://haxe.org/com/libs/hxfcgi) wiki page.


Differences to haxe Web API:
----------------------------
* If you using FastCGI you should use `Web.cacheModule()`, otherwise the module will be restarted after every request, which is quite slow.
* If you using CGI if does not matter if you use `Web.cacheModule()`.
* You need to print some data, when sending headers only, call `Lib.print('');` after setting all headers.
