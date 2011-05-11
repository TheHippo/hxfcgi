CGI/FastCGI Wrapper for nekoVM and the haxe cpp target
======================================================

Documentation
--------------
For more documentation visit the [hxfcgi](http://haxe.org/com/libs/hxfcgi) wiki page.


Differences to haxe Web API:
----------------------------
* If you are using FastCGI you should use `Web.cacheModule()`, otherwise the module will be restarted after every request, which is quite slow.
* If you are using CGI if does not matter if you use `Web.cacheModule()`.
* When setting cache with `Web.cacheModule(run)`, `run` is called, don't call your `run` method after caching or it may run twice.
* You need to print some data, when sending headers only, call `Lib.print('');` after setting all headers.
