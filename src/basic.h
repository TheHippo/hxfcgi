#include <string>
#include <list>

using namespace std;

#ifndef HXFCGI_BASICDATA
#define HXFCGI_BASICDATA

namespace hxfcgi {
	
	class BasicData {
		public:
			char* getClientIP();
			string getURI();
			list<string> getAllHeaders();
			string getHeader(string key);
			char* getMethod();
		private:
			string formatHeader(string h);
	};
	
}

#endif
