#define IMPLEMENT_API
#include <hx/CFFI.h>
#include "request.h"
#include "basic.h"
#include "data.h"

//for debugging:
#include <fastcgi.h>
#include <fcgi_stdio.h>

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

value hxfcgi_get_client_ip(value hreq) {
	val_check_kind(hreq,hxRequest);
	hxfcgi::BasicData d;
	return alloc_string(d.getClientIP());
}


value hxfcgi_get_uri(value hreq) {
	val_check_kind(hreq,hxRequest);	
	hxfcgi::BasicData d;
	return alloc_string(d.getURI().c_str());
}

value hxfcgi_get_all_headers(value hreq) {
	val_check_kind(hreq,hxRequest);	
	hxfcgi::BasicData d;
	list<string> header = d.getAllHeaders();
	list<string>::iterator iter;
	value ret = alloc_array(header.size());
	int c = 0;
	for (iter = header.begin();iter != header.end(); iter++,c++) {
		val_array_set_i(ret,c,alloc_string((*iter).c_str()));
	}
	return ret;
}

value hxfcgi_get_header(value hreq,value hkey) {
	val_check_kind(hreq,hxRequest);
	val_check(hkey,string);
	hxfcgi::BasicData d;
	string ret = d.getHeader(val_string(hkey));
	if (ret.compare("")==0)
		return val_null;
	else
		return alloc_string(ret.c_str());
}

value hxfcgi_get_method(value hreq) {
	val_check_kind(hreq,hxRequest);
	hxfcgi::BasicData d;
	char *ret = d.getMethod();
	if (ret == NULL)
		hx_failure("This seems not to be a HTTP Request");
	return alloc_string(ret);	
}

value hxfcgi_set_return_code(value hreq,value hcode) {
	val_check(hcode,int);
	hxfcgi::Request *req = get_request(hreq);
	req->setReturnCode(val_int(hcode));
	return val_null;
}

value hxfcgi_get_post_data(value hreq) {
	hxfcgi::Request *req = get_request(hreq);
	return alloc_string(req->getPostData().c_str());	
}

value hxfcgi_get_params_string(value hreq) {
	val_check_kind(hreq,hxRequest);
	hxfcgi::Data d;
	return alloc_string(d.getParamsString().c_str());
}

value hxfcgi_get_params(value hreq) {
	val_check_kind(hreq,hxRequest);
	hxfcgi::Data d;
	map<string,string> params = d.getParams();
	return val_null;	
}

DEFINE_PRIM(hxfcgi_get_params,1);
DEFINE_PRIM(hxfcgi_get_params_string,1);
DEFINE_PRIM(hxfcgi_get_post_data,1);
DEFINE_PRIM(hxfcgi_set_return_code,2);
DEFINE_PRIM(hxfcgi_get_method,1);
DEFINE_PRIM(hxfcgi_get_header,2);
DEFINE_PRIM(hxfcgi_get_all_headers,1);
DEFINE_PRIM(hxfcgi_get_client_ip,1);
DEFINE_PRIM(hxfcgi_get_uri,1);
DEFINE_PRIM(hxfcgi_create_request,0);
DEFINE_PRIM(hxfcgi_add_header,3);
DEFINE_PRIM(hxfcgi_print,2);
DEFINE_PRIM(hxfcgi_cache_module,1);
