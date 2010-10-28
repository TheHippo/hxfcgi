#define IMPLEMENT_API
#include <hx/CFFI.h>
#include "request.h"

DEFINE_KIND(hxRequest);

value create_request() {
	hxfcgi::Request *req = new hxfcgi::Request();
	value ret = alloc_abstract(hxRequest,req);
	return ret;
	
}

DEFINE_PRIM(create_request,0);
