#include <fastcgi.h>
#include <fcgi_stdio.h>
#include <stdlib.h>
#include "basic.h"


namespace hxfcgi {
	
	char* BasicData::getClientIP() {
		return getenv("REMOTE_ADDR");
	}
	
	char* BasicData::getURI() {
		return getenv("REQUEST_URI");
	}
	
	
}
