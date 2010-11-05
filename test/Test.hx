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
		#if HXFGCI
		Web.init();
		#end
		Web.cacheModule(run);
		run();
	}
	
	static function run() {
		Web.setHeader("X-Power-By","hxFCGI");
		Web.setHeader("Content-type","text/plain");
		Lib.println(id++);
		trace(Web.getClientIP());
		trace(Web.getURI());
		trace(Web.getClientHeaders());

	}
	
}
