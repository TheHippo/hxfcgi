#include <string>

using namespace std;

namespace hxfcgi {
	
	class Data {
		public:
			string getPostData();
			string getParamsString();
		private:
			int getPostLength();
		
	};	
	
}
