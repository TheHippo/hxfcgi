#include <fastcgi.h>
#include "request.h"

namespace hxfcgi {
	
	Request::Request() {
		header_sent = false;
		header = new Header();
	}
	
	bool Request::headerSent() {
		return headerSent();
	}
	
}
