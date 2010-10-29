#if cpp
import cpp.Web;
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
		Web.print("Foo");
		Web.print(id++);

	}
	
}
