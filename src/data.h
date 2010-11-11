#include <map>
#include <string>
#include "request.h"

using namespace std;

#ifndef HXFCGI_DATA
#define HXFCGI_DATA

namespace hxfcgi {
	
	class Data {
		public:
			string getPostData();
			string getParamsString();
			map<string,string> getParams(Request req);
		private:
			int getPostLength();
			string getCompleteQueryString(string post, string get);
			string parseKey(string data);
			string parseValue(string data);
		
	};	
	
}

#endif
