#include <map>
#include <string>

using namespace std;

#ifndef HXFCGI_REQUEST
#define HXFCGI_REQUEST

namespace hxfcgi {
	
	class Request {
		public:
			Request();
			bool headerSent();
			void printHeaders();
			void addHeader(string type,string value);
			void print(string msg);
			void setReturnCode(int code);
			string getPostData();
			
		
		private:
			map<string,string> header;
			bool header_sent;
			bool post_fetched;
			string postData;
			string codeToHeader(int r);	
	};
	
}

#endif
