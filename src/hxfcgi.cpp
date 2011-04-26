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
	req->printHeaders();
	for (int i = 0; i < val_strlen(msg);i++)
		req->putchar(val_string(msg)[i]);
	return val_null;
}

value hxfcgi_log(value hreq,value msg) {
	val_check(msg,string);
	hxfcgi::Request *req = get_request(hreq);
	req->log(val_string(msg));
	return val_null;
}

value hxfcgi_flush(value hreq) {
	hxfcgi::Request *req = get_request(hreq);
	req->flush();
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
	hxfcgi::Request *req = get_request(hreq);
	hxfcgi::Data d;
	map<string,string> params = d.getParams((*req));
	value ret = alloc_array(params.size()*2);
	unsigned int c = 0;
	for (map<string,string>::iterator iter = params.begin(); iter!=params.end(); iter++, c++) {
		val_array_set_i(ret,2*c,alloc_string(iter->first.c_str()));
		val_array_set_i(ret,2*c+1,alloc_string(iter->second.c_str()));
	}
	return ret;
}

value hxfcgi_get_cookies(value hreq) {
	val_check_kind(hreq,hxRequest);
	hxfcgi::BasicData d;
        string ret = d.getHeader("COOKIE");
        if (ret.compare("")==0)
                return val_null;
	char *k = (char*)ret.c_str();
        char *start, *end;
	value p = val_null, tmp;
        while( (start = strchr(k,'=')) != NULL ) {
                start++;
                end = start;
                while( *end != 0 && *end != '\r' && *end != '\n' && *end != ';' )
                        end++;
                tmp = alloc_array(3);
		val_array_set_i(tmp,0,copy_string(k,(int)(start-k-1)));
                val_array_set_i(tmp,1,copy_string(start,(int)(end-start)));
                val_array_set_i(tmp,2,p);
                p = tmp;
                if( *end != ';' || end[1] != ' ' )
                        break;
                k = end + 2;
        }
        return p;
}

value hxfcgi_set_cookie(value hreq, value name, value v) {
	val_check_kind(hreq,hxRequest);
	val_check(name,string);
        val_check(v,string);
	buffer b;
        value str;
	hxfcgi::Request *req = get_request(hreq);
        b = alloc_buffer(NULL);
        val_buffer(b,name);
        buffer_append(b,"=");
        val_buffer(b,v);
        buffer_append(b,";");
        str = buffer_to_string(b);
        req->addHeader("Set-Cookie",val_string(str));
	return val_true;
}

value hxfcgi_parse_multipart(value hreq, value onpart, value ondata ) {
	val_check_kind(hreq,hxRequest);
	/*
	hxfcgi::BasicData d;
	value buf;
	int len = 0;
	mcontext *c = CONTEXT();
	const char *ctype = d.getHeader("CONTENT_TYPE");
	value boundstr;
	val_check_function(onpart,2);
	val_check_function(ondata,3);
	buf = alloc_empty_string(BUFSIZE);
	if( !ctype || strstr(ctype,"multipart/form-data") == NULL )
		return val_null;
	// extract boundary value
	{
		const char *boundary, *bend;
		if( (boundary = strstr(ctype,"boundary=")) == NULL )
			neko_error();
		boundary += 9;
		PARSE_HEADER(boundary,bend);
		len = (int)(bend - boundary);
		boundstr = alloc_empty_string(len+2);
		if( val_strlen(boundstr) > BUFSIZE / 2 )
			neko_error();
		val_string(boundstr)[0] = '-';
		val_string(boundstr)[1] = '-';
		memcpy(val_string(boundstr)+2,boundary,len);
	}
	len = 0;
    if( !ap_should_client_block(c->r) )
		neko_error();
	while( true ) {
		char *name, *end_name, *filename, *end_file_name, *data;
		int pos;
		// refill buffer
		// we assume here that the the whole multipart header can fit in the buffer
		fill_buffer(c,buf,&len);
		// is boundary at the beginning of buffer ?
		if( len < val_strlen(boundstr) || memcmp(val_string(buf),val_string(boundstr),val_strlen(boundstr)) != 0 )
			return discard_body(c);
		name = memfind(val_string(buf),len,"Content-Disposition:");
		if( name == NULL )
			break;
		name = memfind(name,len - (int)(name - val_string(buf)),"name=");
		if( name == NULL )
			return discard_body(c);
		name += 5;
		PARSE_HEADER(name,end_name);
		data = memfind(end_name,len - (int)(end_name - val_string(buf)),"\r\n\r\n");
		if( data == NULL )
			return discard_body(c);
		filename = memfind(name,(int)(data - name),"filename=");
		if( filename != NULL ) {
			filename += 9;
			PARSE_HEADER(filename,end_file_name);
		}
		data += 4;
		pos = (int)(data - val_string(buf));
		// send part name
		val_call2(onpart,copy_string(name,(int)(end_name - name)),filename?copy_string(filename,(int)(end_file_name - filename)):val_null);
		// read data
		while( true ) {
			const char *boundary;
			// recall buffer
			memcpy(val_string(buf),val_string(buf)+pos,len - pos);
			len -= pos;
			pos = 0;
			fill_buffer(c,buf,&len);
			// lookup bounds
			boundary = memfind(val_string(buf),len,val_string(boundstr));
			if( boundary == NULL ) {
				if( len == 0 )
					return discard_body(c);
				// send as much buffer as possible to client
				if( len < BUFSIZE )
					pos = len;
				else
					pos = len - val_strlen(boundstr) + 1;
				val_call3(ondata,buf,alloc_int(0),alloc_int(pos));
			} else {
				// send remaining data
				pos = (int)(boundary - val_string(buf));
				val_call3(ondata,buf,alloc_int(0),alloc_int(pos-2));
				// recall
				memcpy(val_string(buf),val_string(buf)+pos,len - pos);
				len -= pos;
				break;
			}
		}
	}	
	*/
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
DEFINE_PRIM(hxfcgi_log,2);
DEFINE_PRIM(hxfcgi_flush,1);
DEFINE_PRIM(hxfcgi_cache_module,1);
DEFINE_PRIM(hxfcgi_get_cookies,1);
DEFINE_PRIM(hxfcgi_set_cookie,3);
DEFINE_PRIM(hxfcgi_parse_multipart,3);
