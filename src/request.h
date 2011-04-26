#include <map>
#include <string>
#include <hx/CFFI.h>

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
			void putchar(char c);
			void log(string msg);
			void flush();
			void setReturnCode(int code);
			string getPostData();
			void bufferFill(buffer buf,int *len);
		
		private:
			map<string,string> header;
			bool header_sent;
			bool post_fetched;
			string postData;
			string codeToHeader(int r);	
	};
	
}

#endif
