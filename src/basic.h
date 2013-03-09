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

#ifndef HXFCGI_BASICDATA
#define HXFCGI_BASICDATA

#include <string>
#include <list>

using namespace std;

namespace hxfcgi {
	
	class BasicData {
		public:
			char* getClientIP();
			string getURI();
			list<string> getAllHeaders();
			string getHeader(string key);
			char* getMethod();
		private:
			string formatHeader(string h);
	};
	
}

#endif
