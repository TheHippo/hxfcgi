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
		printf("post: %s\n",data.c_str());
		return data;
	}
	
	string Data::getParamsString() {
		string ret(getenv("QUERY_STRING"));
		printf("get: %s\n",ret.c_str());
		return ret;
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
	
	map<string,string> Data::getParams() {
		map<string,string> params;
		string base = "";
		string post = getPostData();
		string get = getParamsString();
		printf("combined: %s\n",base.c_str());
		return params;
	}
	
}
