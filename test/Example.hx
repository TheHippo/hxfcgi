#if neko
import neko.Web;
import neko.Lib;
#else
import cpp.Web;
import cpp.Lib;
#end

class Example {

	static function main() {
		Web.cacheModule(run);
		//run();
	}
	
	static function run() {
		Web.setHeader("Content-type","text/plain");
		Lib.println(Web.getParams());
	}
}
