#include <fastcgi.h>
#include <fcgi_stdio.h>
#include <sstream>
#include <istream>
#include <iostream>
#include <stdlib.h>
#include "data.h"

using namespace std;

namespace hxfcgi {
	
	string Data::getPostData() {
		int length = getPostLength();
		char c[length];
		fgets(c,length+1,stdin);
		string data(c);
		return data;
	}
	
	int Data::getPostLength() {
		int ret = 0;
		char *clen = getenv("CONTENT_LENGTH");
		if (clen == NULL) {
			return ret;
		}
		else {
			string slen(clen);
			stringstream stream(slen);
			stream >> ret;
			return ret;
		}
	}
	
}
