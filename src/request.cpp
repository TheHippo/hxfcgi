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

#include <fastcgi.h>
#include <fcgi_stdio.h>
#include <sstream>
#include "request.h"
#include "data.h"

namespace hxfcgi {

	Request::Request() {
		header_sent = false;
		post_fetched = false;
		postData = "";
		if (!FCGI_Accept()==0) {
			string error = "Could not generate Request";
			throw error;
		}
	}
	
	string Request::getPostData() {
		if (!post_fetched) {
			Data d;
			postData = d.getPostData();
			post_fetched = true;
		}		
		return postData;
	}
	
	void Request::bufferFill(buffer buf,int *len) {
		Data d;
		int pos = *len;
		while( pos < BUFSIZE ) {
			int k = d.getStdinData(buffer_data(buf)+pos,BUFSIZE-pos);
			if( k == 0 )
				break;
			pos += k;
		}
		*len = pos;
	}

	void Request::bufferFill(char *buf,int *len) {
		Data d;
		int pos = *len;
		while( pos < BUFSIZE ) {
			int k = d.getStdinData(buf+pos,BUFSIZE-pos);
			if( k == 0 )
				break;
			pos += k;
		}
		*len = pos;
	}
	
	bool Request::headerSent() {
		return header_sent;
	}
	
	void Request::printHeaders() {
		if (header_sent==true) return;
		if(header.count("Content-type") == 0 && header.count("Content-Type") == 0)
			header["Content-type"]="text/html";
		map<string,string>::iterator iter;
		for (iter = header.begin(); iter != header.end(); iter++) {
			FCGI_printf("%s: %s\r\n",iter->first.c_str(),iter->second.c_str());
		}
		FCGI_printf("\r\n");
		header_sent = true;
	}
	
	void Request::addHeader(string type,string value) {
		header[type]=value;
	}	

	void Request::pchar(char c) {
		FCGI_putchar(c);
 	}

	void Request::log(string msg) {
		FCGI_fprintf(stderr, "%s",msg.c_str());
 	}

	void Request::flush() {
		FCGI_fflush(FCGI_stdout);
 	}
	
	void Request::setReturnCode(int code) {
		header["Status"]=codeToHeader(code);
	}
	
	string Request::codeToHeader(int r) {
		string code;
		switch(r) {
			case 100: code = "100 Continue";
			case 101: code = "101 Switching Protocols";
			case 200: code = "200 Continue";
			case 201: code = "201 Created";
			case 202: code = "202 Accepted";
			case 203: code = "203 Non-Authoritative Information";
			case 204: code = "204 No Content";
			case 205: code = "205 Reset Content";
			case 206: code = "206 Partial Content";
			case 300: code = "300 Multiple Choices";
			case 301: code = "301 Moved Permanently";
			case 302: code = "302 Found";
			case 303: code = "303 See Other";
			case 304: code = "304 Not Modified";
			case 305: code = "305 Use Proxy";
			case 307: code = "307 Temporary Redirect";
			case 400: code = "400 Bad Request";
			case 401: code = "401 Unauthorized";
			case 402: code = "402 Payment Required";
			case 403: code = "403 Forbidden";
			case 404: code = "404 Not Found";
			case 405: code = "405 Method Not Allowed";
			case 406: code = "406 Not Acceptable";
			case 407: code = "407 Proxy Authentication Required";
			case 408: code = "408 Request Timeout";
			case 409: code = "409 Conflict";
			case 410: code = "410 Gone";
			case 411: code = "411 Length Required";
			case 412: code = "412 Precondition Failed";
			case 413: code = "413 Request Entity Too Large";
			case 414: code = "414 Request-URI Too Long";
			case 415: code = "415 Unsupported Media Type";
			case 416: code = "416 Requested Range Not Satisfiable";
			case 417: code = "417 Expectation Failed";
			case 500: code = "500 Internal Server Error";
			case 501: code = "501 Not Implemented";
			case 502: code = "502 Bad Gateway";
			case 503: code = "503 Service Unavailable";
			case 504: code = "504 Gateway Timeout";
			case 505: code = "505 HTTP Version Not Supported";
			default: {
				stringstream ss;
				ss << r;
				code = ss.str();				
			}
		}
		return code;
	}
}
