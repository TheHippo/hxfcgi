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
//		Web.setHeader("Content-type","text/plain");
		Web.setReturnCode(301);
		Lib.print("<html><head></head><body><pre>");
		Lib.println(id++);
		trace(Web.getClientIP());
		trace(Web.getURI());
		var header = Web.getClientHeaders();
		for (x in header) {
			trace(x.header+": "+x.value);
		}
		trace(Web.getClientHeader("user_agent"));
		trace(Web.getClientHeader("user_agents"));
		trace(Web.getMethod());
		if (Web.getMethod()=="POST") {
			var post = Web.getPostData();
			trace(post.length+":"+post);
		}
		trace(Web.getParamsString());
		var params = Web.getParams();
		for (k in params.keys()) {
			trace(k+": "+params.get(k));
		}
		Lib.print("</pre>");
		Lib.print('<form action="?foo=bar&test=test" method="POST"><input type="text" name="text"/><input type="password" name="pass" /><input type="submit" /></form>');
		Lib.print("</body></html>");

	}
	
}
