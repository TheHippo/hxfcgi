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
				header.push_back(formatHeader(s->substr(5)));
			else if (s->find("CONTENT_TYPE")==0)
				header.push_back(formatHeader(s->c_str()));
		
		}
		return header;
	}
	
	string BasicData::formatHeader(string h) {
		std::string::size_type pos = h.find("_");
		if(pos != string::npos) 
			h.replace(pos, 1, "-"); 
		for (std::string::size_type j=1; j<h.length(); ++j)	{
			if(j != pos && j != pos+1) 
				h[j]=tolower(h[j]);
		} 
		return h;
	}
	
	string BasicData::getHeader(string key) {
		std::string::size_type pos = key.find("-");
		if(pos != string::npos) 
			key.replace(pos, 1, "_"); 
		transform(key.begin(),key.end(),key.begin(),::toupper);
		char *cret = getenv(key.c_str());
		if (cret != NULL) {
			return string(cret);
		}
		else {
			string rkey = "HTTP_";
			rkey.append(key);
			cret = getenv(rkey.c_str());
			if (cret != NULL)
				return string(cret);
		}
		return string("");
	}
	
	char* BasicData::getMethod() {
		return getenv("REQUEST_METHOD");
	}
	
	
}
