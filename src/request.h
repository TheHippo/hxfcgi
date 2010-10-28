#include "header.h"


namespace hxfcgi {
	
	class Request {
		public:
			Request();
			bool headerSent();
		
		private:
			Header *header;
			bool header_sent;
		
		
	};
	
	
}
