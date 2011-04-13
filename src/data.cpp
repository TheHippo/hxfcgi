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
		FCGI_fgets(c,length+1,stdin);
		string data(c);
		return data;
	}
	
	string Data::getParamsString() {
		string ret(getenv("QUERY_STRING"));
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
	
	map<string,string> Data::getParams(Request req) {
		map<string,string> params;
		string base = getCompleteQueryString(req.getPostData(),getParamsString());
		string part,key,value;
		std::string::size_type ppos = base.find("&");
		while (ppos != string::npos) {
			part = base.substr(0,ppos);
			params[parseKey(part)]=parseValue(part);
			base = base.substr(ppos+1);
			ppos = base.find("&");
		}
		params[parseKey(base)]=parseValue(base);
		return params;
	}
	
	string Data::getCompleteQueryString(string post, string get) {
		string base = "";
		if (post.length() != 0)
			base.append(post);
		if (get.length() != 0 && post.length() != 0)
			base.append("&");
		if (get.length() != 0)
			base.append(get);
		return base;
	}
	
	string Data::parseKey(string data) {
		std::string::size_type pos = data.find("=");
		if (pos != string::npos)
			return data.substr(0,pos);
		else
			return "";
	}
	
	string Data::parseValue(string data) {
		std::string::size_type pos = data.find("=");
		if (pos != string::npos)
			return data.substr(pos+1);
		else
			return "";
	}
}
