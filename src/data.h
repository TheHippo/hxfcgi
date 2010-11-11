#include <string>

using namespace std;

namespace hxfcgi {
	
	class Data {
		public:
			string getPostData();
		private:
			int getPostLength();
		
	};	
	
}
