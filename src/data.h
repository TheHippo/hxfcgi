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

#ifndef HXFCGI_DATA
#define HXFCGI_DATA

#include <map>
#include <string>
#include "request.h"

using namespace std;

#define BUFSIZE 1024

namespace hxfcgi {

	class Data {
		public:
			string getPostData();
			string getParamsString();
			map<string,string> getParams(Request req);
			int getStdinData(char* buf, int len);
		private:
			int getPostLength();
			string getCompleteQueryString(string post, string get);
			string parseKey(string data);
			string parseValue(string data);
		
	};	
	
}

#endif
