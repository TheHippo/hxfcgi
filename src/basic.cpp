#include <fastcgi.h>
#include <fcgi_stdio.h>
#include <stdlib.h>
#include "basic.h"
#include <list>
#include <string>

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
	
	/*void BasicData::clearList(list<string> list) {
		list<string> iter;
		for (iter = list.begin(); iter<list.end(); iter++) {
			delete (*iter);
		}
		delete list;	
	} */
	
	
}
