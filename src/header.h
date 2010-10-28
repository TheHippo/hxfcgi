#include <map>
#include <string>


using namespace std;

namespace hxfcgi {
	
	class Header {
		public:
			Header();
			
		private:
			map<string,string> header;
		
		
	};
	
}
