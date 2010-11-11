#include <map>
#include <string>

using namespace std;

namespace hxfcgi {
	
	class Data {
		public:
			string getPostData();
			string getParamsString();
			map<string,string> getParams();
		private:
			int getPostLength();
		
	};	
	
}
