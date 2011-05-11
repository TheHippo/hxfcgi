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
		Web.cacheModule(run);
		//run();
	}
	
	static function run() {
		Web.setHeader("X-Power-By","hxFCGI");
		//Web.setHeader("Content-type","text/plain");
		//Web.setReturnCode(503);
		Web.setCookie("test","hxfcgi"+id,null,null,"/",false);
		Lib.print("<html><head></head><body><pre>");
		Lib.println(id++);
		trace(Web.getHostName());
		trace(Web.getCookies());
		trace(Web.getClientIP());
		trace(Web.getURI());
		trace(Web.getCwd());
		trace(Web.getAuthorization());
		var header = Web.getClientHeaders();
		for (x in header) {
			trace(x.header+": "+x.value);
		}
		trace(Web.getClientHeader("user_agent"));
		trace(Web.getClientHeader("user_agents"));
		trace(Web.getClientHeader("User-Agent"));
		trace(Web.getMethod());
		if (Web.getMethod()=="POST") {
			trace( Web.getMultipart(65635000));
			var post = Web.getPostData();
			trace(post);
		}
		Web.flush();
		trace(Web.getParamsString());
		var params = Web.getParams();
		trace(params);
		for (k in params.keys()) {
			trace(k+": "+params.get(k));
		}
		
		
		
		Lib.print("</pre>");
		Lib.print('<form action="?foo=bar&test=test" method="POST"><input type="text" name="text"/><input type="password" name="pass" /><input type="submit" /></form><br/>');
		Lib.print('<form action="?foo=bar&test=test" method="POST" enctype="multipart/form-data"><input type="text" name="text"/><input type="password" name="pass" /><input type="file" name="file" /><input type="submit" /></form>');
		Lib.print("</body></html>");

	}
	
}
