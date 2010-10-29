#include <fastcgi.h>
#include <fcgi_stdio.h>
#include "request.h"

namespace hxfcgi {
	
	Request::Request() {
		header_sent = false;
		header["Content-type"]="text/html";
		if (!FCGI_Accept()==0) {
			string error = "Could not generate Request";
			throw error;
		}
	}
	
	bool Request::headerSent() {
		return header_sent;
	}
	
	void Request::printHeaders() {
		map<string,string>::iterator iter;
		for (iter = header.begin(); iter != header.end(); iter++) {
			printf("%s: %s\r\n",iter->first.c_str(),iter->second.c_str());
		}
		printf("\r\n");
		header_sent = true;
	}
	
	void Request::addHeader(string type,string value) {
		header[type]=value;
	}	
	
	void Request::print(string msg) {
		if (header_sent==false)
			printHeaders();
		printf("%s",msg.c_str());
	}
}
