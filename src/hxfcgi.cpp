#define IMPLEMENT_API
#include <hx/CFFI.h>
#include "request.h"

DEFINE_KIND(hxRequest);


inline hxfcgi::Request* get_request(value hreq) {
	val_check_kind(hreq,hxRequest);
	return (hxfcgi::Request*)val_data(hreq);
}

value hxfcgi_create_request() {
	try {
		hxfcgi::Request *req = new hxfcgi::Request();
		value ret = alloc_abstract(hxRequest,req);
		return ret;
	}
	catch (string error) {
		hx_failure(error.c_str());
	}
	return val_null;	
}

value hxfcgi_add_header(value hreq,value type,value value) {
	val_check(type,string);
	val_check(value,string);
	hxfcgi::Request *req = get_request(hreq);
	req->addHeader(val_string(type),val_string(value));
	return val_null;	
}

value hxfcgi_print(value hreq,value msg) {
	val_check(msg,string);
	hxfcgi::Request *req = get_request(hreq);
	req->print(val_string(msg));
	return val_null;
}

value hxfcgi_cache_module(value func) {
	val_check_function(func,1);
	hxfcgi::Request *req;
	while (true) {
		try {
			req = new hxfcgi::Request();
			val_call1(func,alloc_abstract(hxRequest,req));
			delete req;
		}
		catch (string error) {
			hx_failure(error.c_str());
			break;
		}
	}
	return val_null;
	
}

DEFINE_PRIM(hxfcgi_create_request,0);
DEFINE_PRIM(hxfcgi_add_header,3);
DEFINE_PRIM(hxfcgi_print,2);
DEFINE_PRIM(hxfcgi_cache_module,1);
