#if cpp
import cpp.Web;
import cpp.Lib;
#else
import neko.Web;
import neko.Lib;
#end
class Test {

	static var id = 0;
	
	static function main() {
		Web.init();
		Web.cacheModule(run);
		run();
	}
	
	static function run() {
		Web.setHeader("X-Power-By","hxFCGI");
		Web.setHeader("Content-type","text/plain");
		Lib.println("Foo:");
		Lib.println(id++);
		trace("foO");

	}
	
}
