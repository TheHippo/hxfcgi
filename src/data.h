#include <map>
#include <string>
#include "request.h"

using namespace std;

#ifndef HXFCGI_DATA
#define HXFCGI_DATA

#define BUFSIZE 1024

namespace hxfcgi {

	class Data {
		public:
			string getPostData();
			string getParamsString();
			map<string,string> getParams(Request req);
			int getStdinData(char* buf, int len);
		private:
			int getPostLength();
			string getCompleteQueryString(string post, string get);
			string parseKey(string data);
			string parseValue(string data);
		
	};	
	
}

#endif
