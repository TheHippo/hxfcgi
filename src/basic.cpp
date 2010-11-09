#include <fastcgi.h>
#include <fcgi_stdio.h>
#include <stdlib.h>
#include "basic.h"
#include <list>
#include <string>
#include <algorithm>

using namespace std;

namespace hxfcgi {
	
	char* BasicData::getClientIP() {
		return getenv("REMOTE_ADDR");
	}
	
	char* BasicData::getURI() {
		return getenv("REQUEST_URI");
	}
	
	list<string> BasicData::getAllHeaders() {
		list<string> header;
		char **evn = environ;
		for (; *evn != NULL; evn++) {
			string *s = new string(*evn);
			if (s->find("HTTP")==0)
				header.push_back(s->substr(5));
		}
		return header;
	}
	
	string BasicData::getHeader(string key) {
		string rkey = "HTTP_";
		transform(key.begin(),key.end(),key.begin(),::toupper);
		rkey.append(key);
		char *cret = getenv(rkey.c_str());
		if (cret != NULL)
			return string(cret);
		else
			return string("");
	}
	
	char* BasicData::getMethod() {
		return getenv("REQUEST_METHOD");
	}
	
	
}
