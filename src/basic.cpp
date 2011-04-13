#include <fastcgi.h>
#include <fcgi_stdio.h>
#include <stdlib.h>
#include <list>
#include <string>
#include <algorithm>
#include "basic.h"

#if defined(__APPLE__)
#include <crt_externs.h>
#define environ (*_NSGetEnviron())
#endif

using namespace std;

namespace hxfcgi {
	
	char* BasicData::getClientIP() {
		return getenv("REMOTE_ADDR");
	}
	
	string BasicData::getURI() {
		string uri(getenv("REQUEST_URI"));
		std::string::size_type pos = uri.find("?");
		if (pos == string::npos)
			return uri;
		else {
			return uri.substr(0,pos);
		}
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
