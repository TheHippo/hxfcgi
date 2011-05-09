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

#ifndef HXFCGI_REQUEST
#define HXFCGI_REQUEST

#include <map>
#include <string>
#include <hx/CFFI.h>

using namespace std;

namespace hxfcgi {
	
	class Request {
		public:
			Request();
			bool headerSent();
			void printHeaders();
			void addHeader(string type,string value);
			void pchar(char c);
			void log(string msg);
			void flush();
			void setReturnCode(int code);
			string getPostData();
			void bufferFill(buffer buf,int *len);
			void bufferFill(char *buf,int *len);
		
		private:
			map<string,string> header;
			bool header_sent;
			bool post_fetched;
			string postData;
			string codeToHeader(int r);	
	};
	
}

#endif
