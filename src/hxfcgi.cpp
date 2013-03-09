/*	
 *  hxfcgi - CGI/FastCGI Wrapper for nekoVM and the haxe cpp target
 *  Copyright (C) 2011 Philipp "TheHippo" Klose
 *  Copyright (C) 2011 "KaalH!"
 *
 *  This file is part of hxfcgi.
 *
 *  hxfcgi is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as 
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  hxfcgi is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public 
 *  License along with hxfcgi. If not, see <http://www.gnu.org/licenses/>.
 */

#define IMPLEMENT_API
#include <hx/CFFI.h>
#include "request.h"
#include "basic.h"
#include "data.h"

#include <fcgi_stdio.h>

DEFINE_KIND(hxRequest);

#define PARSE_HEADER(start,cursor) \
cursor = start; \
if( *cursor == '"' ) { \
start++; \
cursor++; \
while( *cursor != '"' && *cursor != 0 ) \
cursor++; \
} else { \
while( *cursor != 0 && *cursor != '\r' && *cursor != '\n' && *cursor != '\t' ) \
cursor++; \
}

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
		req->pchar(val_string(msg)[i]);
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
			if(FCGX_IsCGI()) break;
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


static char *memfind( char *mem, int mlen, const char *v ) {
	char *found;
	int len = (int)strlen(v);
	if( len == 0 )
		return mem;
	while( (found = (char*) memchr(mem,*v,mlen)) != NULL ) {
		if( (int)(found - mem) + len > mlen )
			break;
		if( memcmp(found,v,len) == 0 )
			return found;
		mlen -= (int)(found - mem + 1);
		mem = found + 1;
	}
	return NULL;
}

value hxfcgi_parse_multipart(value hreq, value onpart, value ondata ) {
	val_check_kind(hreq,hxRequest);
	val_check_function(onpart,2);
	val_check_function(ondata,3);
	hxfcgi::Request *req = get_request(hreq);
	buffer buf;
	int len = 0;
	buffer boundstr;
	buf = alloc_buffer_len(BUFSIZE);
	hxfcgi::BasicData b;
	string ctype = b.getHeader("CONTENT_TYPE");
	if(ctype.find("multipart/form-data") != 0)
		return val_null;
	// extract boundary value
	{
		const char *boundary, *bend;
		if( (boundary = strstr(ctype.c_str(),"boundary=")) == NULL )
			neko_error();
		boundary += 9;
		PARSE_HEADER(boundary,bend);
		len = (int)(bend - boundary);
		boundstr = alloc_buffer_len(len+3);
		if( strlen(buffer_data(boundstr)) > BUFSIZE / 2 )
			neko_error();
		
		buffer_data(boundstr)[0] = '-';
		buffer_data(boundstr)[1] = '-';
		memcpy(buffer_data(boundstr)+2,boundary,len);
		buffer_data(boundstr)[len+2] = 0;
	}
	len = 0;
	
	while( true ) {
		char *name, *end_name, *filename, *end_file_name, *data;
		int pos;
		// refill buffer
		// we assume here that the the whole multipart header can fit in the buffer
		req->bufferFill(buf,&len);
		// is boundary at the beginning of buffer ?
		if( len < (int) strlen(buffer_data(boundstr)) || memcmp(buffer_data(buf),buffer_data(boundstr),strlen(buffer_data(boundstr))) != 0 )
			return val_null;
		name = memfind(buffer_data(buf),len,"Content-Disposition:");
		if( name == NULL )
			break;
		name = memfind(name,len - (int)(name - buffer_data(buf)),"name=");
		if( name == NULL )
			return val_null;
		name += 5;
		PARSE_HEADER(name,end_name);
		data = memfind(end_name,len - (int)(end_name - buffer_data(buf)),"\r\n\r\n");
		if( data == NULL )
			return val_null;
		filename = memfind(name,(int)(data - name),"filename=");
		if( filename != NULL ) {
			filename += 9;
			PARSE_HEADER(filename,end_file_name);
			// send part name
			val_call2(onpart,copy_string(name,(int)(end_name - name)),copy_string(filename,(int)(end_file_name - filename)));
		} else {
			// send part name
			val_call2(onpart,copy_string(name,(int)(end_name - name)),val_null);
		}
		data += 4;
		pos = (int)(data - buffer_data(buf));
	 
	 
		// read data
		while( true ) {
			const char *boundary;
			// recall buffer
			memcpy(buffer_data(buf),buffer_data(buf)+pos,len - pos);
			len -= pos;
			pos = 0;
			req->bufferFill(buf,&len);
			// lookup bounds
			boundary = memfind(buffer_data(buf),len,buffer_data(boundstr));
			if( boundary == NULL ) {
				if( len == 0 )
					return val_null;
				// send as much buffer as possible to client
				if( len < BUFSIZE )
					pos = len;
				else
					pos = len - strlen(buffer_data(boundstr)) + 1;
				val_call3(ondata,buffer_val(buf),alloc_int(0),alloc_int(pos));
			} else {
				// send remaining data
				pos = (int)(boundary - buffer_data(buf));
				val_call3(ondata,buffer_val(buf),alloc_int(0),alloc_int(pos-2));
				// recall
				memcpy(buffer_data(buf),buffer_data(buf)+pos,len - pos);
				len -= pos;
				break;
			}
		}
	}	
	return val_null;
}

value hxfcgi_parse_multipart_neko(value hreq, value onpart, value ondata ) {
	val_check_kind(hreq,hxRequest);
	val_check_function(onpart,2);
	val_check_function(ondata,3);
	hxfcgi::Request *req = get_request(hreq);
	char *buf;
	int len = 0;
	char *boundstr;
	hxfcgi::BasicData b;
	string ctype = b.getHeader("CONTENT_TYPE");
	if(ctype.find("multipart/form-data") != 0)
		return val_null;
	// extract boundary value
	{
		const char *boundary, *bend;
		if( (boundary = strstr(ctype.c_str(),"boundary=")) == NULL )
			neko_error();
		boundary += 9;
		PARSE_HEADER(boundary,bend);
		len = (int)(bend - boundary);
		boundstr = (char *) malloc(sizeof(char) * (len+3));
		if( strlen(boundstr) > BUFSIZE / 2 )
			neko_error();
		
		boundstr[0] = '-';
		boundstr[1] = '-';
		memcpy(boundstr+2,boundary,len);
		boundstr[len+2] = 0;
	}
	
	len = 0;
	buf = (char *) malloc(sizeof(char) * (BUFSIZE));
	while( true ) {
		char *name, *end_name, *filename, *end_file_name, *data;
		int pos;
		// refill buffer
		// we assume here that the the whole multipart header can fit in the buffer
		req->bufferFill(buf,&len);
		// is boundary at the beginning of buffer ?
		if( len < (int) strlen(boundstr) || memcmp(buf,boundstr,strlen(boundstr)) != 0 ) {
			free(boundstr);
			free(buf);
			return val_null;
		}		
		name = memfind(buf,len,"Content-Disposition:");
		if( name == NULL )
			break;
		name = memfind(name,len - (int)(name - buf),"name=");
		if( name == NULL ) {
			free(boundstr);
			free(buf);
			return val_null;
		}
		name += 5;
		PARSE_HEADER(name,end_name);
		data = memfind(end_name,len - (int)(end_name - buf),"\r\n\r\n");
		if( data == NULL ) {
			free(boundstr);
			free(buf);
			return val_null;
		}
		filename = memfind(name,(int)(data - name),"filename=");
		if( filename != NULL ) {
			filename += 9;
			PARSE_HEADER(filename,end_file_name);
		}
		data += 4;
		pos = (int)(data - buf);
		// send part name
		val_call2(onpart,copy_string(name,(int)(end_name - name)),filename?copy_string(filename,(int)(end_file_name - filename)):val_null);
		
		
		// read data
		while( true ) {
			const char *boundary;
			// recall buffer
			memcpy(buf,buf+pos,len - pos);
			len -= pos;
			pos = 0;
			req->bufferFill(buf,&len);
			// lookup bounds
			boundary = memfind(buf,len,boundstr);
			if( boundary == NULL ) {
				if( len == 0 ) {
					free(boundstr);
					free(buf);
					return val_null;
				}
				// send as much buffer as possible to client
				if( len < BUFSIZE )
					pos = len;
				else
					pos = len - strlen(boundstr) + 1;
				val_call3(ondata,copy_string(buf,pos),alloc_int(0),alloc_int(pos));
			} else {
				// send remaining data
				pos = (int)(boundary - buf);
				val_call3(ondata,copy_string(buf,pos-2),alloc_int(0),alloc_int(pos-2));
				// recall
				memcpy(buf,buf+pos,len - pos);
				len -= pos;
				break;
			}
		}
	}	
	free(boundstr);
	free(buf);
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
DEFINE_PRIM(hxfcgi_parse_multipart_neko,3);
