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
#include <istream>
#include <iostream>
#include <stdlib.h>
#include "data.h"

using namespace std;

namespace hxfcgi {
	
	string Data::getPostData() {
		int length = getPostLength();
		if(length == 0) return "";
		if(length > 262144) length = 262144;
		string ctype(getenv("CONTENT_TYPE"));
		if(ctype.find("multipart/form-data") == 0)
			return "";
		char c[length];
		char ch;
		for(int i=0;i<= length; i++) {
			ch = FCGI_fgetc(FCGI_stdin);
			if( ch == EOF ) break;
			c[i] = ch;
		}
		string data(c,length);
		return data;
	}
	
	int Data::getStdinData(char* buf, int len) {
		return FCGI_fread(buf,1,len,FCGI_stdin);
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
			return data;
	}
	
	string Data::parseValue(string data) {
		std::string::size_type pos = data.find("=");
		if (pos != string::npos)
			return data.substr(pos+1);
		else
			return "";
	}
}
