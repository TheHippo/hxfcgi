#include <string>
#include <list>

using namespace std;

namespace hxfcgi {
	
	class BasicData {
		public:
			char* getClientIP();
			char* getURI();
			list<string> getAllHeaders();
			//void clearList(list<string> list);
		
	};
	
}
