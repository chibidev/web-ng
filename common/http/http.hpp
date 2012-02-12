#ifndef __HTTP_HPP__
#define __HTTP_HPP__

#include <limits>

namespace http {

// NOTE
// This only works in C++11, must provide an alternative way for older standards
// __cplusplus macro is broken in gcc<4.7
//

enum header : uint64_t {
	accept						= 0x00000000000001, // request
	accept_charset				= 0x00000000000002, // request
	accept_encoding				= 0x00000000000004, // request
	accept_language 			= 0x00000000000008, // request
	accept_ranges 				= 0x00000000000010, // response
	allow 						= 0x00000000000020, // response
	authorization 				= 0x00000000000040, // request
	cache_control 				= 0x00000000000080, // request response
	connection 					= 0x00000000000100, // request response
	cookie 						= 0x00000000000200, // request
	content_disposition			= 0x00000000000400, // response
	content_encoding 			= 0x00000000000800, // response
	content_language 			= 0x00000000001000, // response
	content_length 				= 0x00000000002000, // request response
	content_location 			= 0x00000000004000, // response
	content_md5 				= 0x00000000008000, // request response
	content_range 				= 0x00000000010000, // response
	content_type 				= 0x00000000020000, // request response
	date 						= 0x00000000040000, // request response
	etag 						= 0x00000000080000, // response
	expect 						= 0x00000000100000, // request
	expires 					= 0x00000000200000, // response
	from 						= 0x00000000400000, // request
	host 						= 0x00000000800000, // request
	if_match 					= 0x00000001000000, // request
	if_modified_since 			= 0x00000002000000, // request
	if_none_match 				= 0x00000004000000, // request
	if_range 					= 0x00000008000000, // request 
	if_unmodified_since 		= 0x00000010000000, // request
	last_modified 				= 0x00000020000000, // response
	link 						= 0x00000040000000, // response
	location 					= 0x00000080000000, // response
	max_forwards 				= 0x00000100000000, // request 
	p3p 						= 0x00000200000000, // response
	pragma 						= 0x00000400000000, // request response
	proxy_authenticate 			= 0x00000800000000, // response
	proxy_authorization 		= 0x00001000000000, // request
	range 						= 0x00002000000000, // request
	referer 					= 0x00004000000000, // request 
	refresh 					= 0x00008000000000, // response
	retry_after 				= 0x00010000000000, // response
	server 						= 0x00020000000000, // response
	set_cookie 					= 0x00040000000000, // response
	strict_transport_security 	= 0x00080000000000, // response
	te 							= 0x00100000000000, // request 
	trailer						= 0x00200000000000, // response
	transfer_encoding 			= 0x00400000000000, // response
	upgrade 					= 0x00800000000000, // request
	user_agent 					= 0x01000000000000, // request
	vary 						= 0x02000000000000, // response
	via 						= 0x04000000000000, // request response
	warning 					= 0x08000000000000, // request reponse
	www_authenticate 			= 0x10000000000000, // response
	all							= 0xFFFFFFFFFFFFFF
};

class request {
};

class response {
};

class parser {
	public:	
		parser(std::istream& __http_stream, header __flags = header::all) : _http_stream(__http_stream), _flags(__flags) {
		}

		
		//request
		void parse_request() {
			int c = _http_stream.get();
			switch (c) {
				case 'A':
					c = _http_stream.get();
					switch (c) {
						case 'c':
							_http_stream.ignore(4);
							c = _http_stream.get();
							switch(c) {
								case ':': // Accept
								case '-':
									c = _http_stream.get();
									switch (c) {
										case 'C': // Accept-Charset
										case 'E': // Accept-Encoding
										case 'L': // Accept-Language
										default:
											drop();
									}
								default:
									drop();
							}
							break;
						case 'u': // Authorization
						default:
							drop();
					}
					break;
				case 'C':
					c = _http_stream.get();
					switch (c) {
						case 'a': // Cache-Control
							drop();
							break;
						case 'o':
							c = _http_stream.get();
							switch (c) {
								case 'n':
									c = _http_stream.get();
									switch (c) {
										case 'n': // Connection
										case 't':
											_http_stream.ignore(4);
											c = _http_stream.get();
											switch (c) {
												case 'L': // Content-Length
												case 'M': // Content-MD5
												case 'T': // Content-Type
												default:
													drop();
											}
										default:
											drop();
									}
								case 'o': // Cookie
								default:
									drop();
							}
						default:
							drop();
					}
					break;
				case 'D': // Date
				case 'E': // Expect
				case 'F': // From
				case 'H': // Host
					drop();
					break;
				case 'I':
					_http_stream.ignore(2);
					c = _http_stream.get();
					switch (c) {
						case 'M':
							c = _http_stream.get();
							switch (c) {
								case 'a': // If-Match
								case 'o': // If-Modified-Since
								default:
									drop();
							}
							break;
						case 'N': // If-None-Match
						case 'R': // If-Range
						case 'U': // If-Unmodified-Since
						default:
							drop();
					}
					break;
				case 'M': // Max-Forwards
					drop();
					break;
				case 'P':
					_http_stream.ignore(1);
					c = _http_stream.get();
					switch (c) {
						case 'a': // Pragma
						case 'o': // Proxy-Authorization
						default:
							drop();
					}
					break;
				case 'R':
					c = _http_stream.get();
					switch (c) {
						case 'a': // Range
						case 'e': // Referer
						default:
							drop();
					}
					break;
				case 'T': // TE
					drop();
					break;
				case 'U':
					c = _http_stream.get();
					switch (c) {
						case 'p': // Upgrade
						case 's': // User-Agent
						default:
							drop();
					}
					break;
				case 'V': // Via
				case 'W': // Warning
				default:
					drop();
			}
		}
		
		//response
		void parse_response() {
			// TODO
			// 
			// check RFC specification whether to provide case insensitive syntax or not
			//


			response r;

			std::string h; // reponse header
			getline(_http_stream, h, '\r');
			_http_stream.ignore(1);

			std::cout << h << std::endl;

			//_http_stream.ignore(std::numeric_limits<int>::max(), '\n'); // response header

			int c;

			do {
				c = _http_stream.get();
					
				switch (c) {
					case 'A':
						c = _http_stream.get();
						switch (c) {
							case 'c': // Accept-Ranges
							case 'g': // Age
							case 'l': // Allow
							default:
								drop();
						}
						break;
					case 'C':
						c = _http_stream.get();
						switch (c) {
							case 'a': // Cache-Control
								drop();
								break;
							case 'o':
								_http_stream.ignore(1);
								c = _http_stream.get();
								switch (c) {
									case 'n': // Connection
										drop();
										break;
									case 't':
										_http_stream.ignore(4);
										c = _http_stream.get();
										switch (c) {
											case 'D': // Content-Disposition
											case 'E': // Content-Encoding
												drop();
												break;
											case 'L':
												c = _http_stream.get();
												switch (c) {
													case 'e': // Content-Length
													case 'o': // Content-Location
													default:
														drop();
												}
												break;
											case 'M': // Content-MD5
											case 'R': // Content-Range
											case 'T': // Content-Type
											default:
												drop();
										}
										break;
									default:
										drop();
								}
								break;
							default:
								drop();
						}
						break;
					case 'D': // Date
						drop();
						break;
					case 'E':
						c = _http_stream.get();
						switch (c) {
							case 'T': // ETag
							case 'x': // Expires
							default:
								drop();
						}
						break;
					case 'L':
						c = _http_stream.get();
						switch (c) {
							case 'a': // Last-Modified
							case 'i': // Link
							case 'o': // Location
							default:
								drop();
						}
						break;
					case 'P':
						c = _http_stream.get();
						switch (c) {
							case '3': // P3P
								drop();
								break;
							case 'r':
								c = _http_stream.get();
								switch (c) {
									case 'a': // Pragma
									case 'o': // Proxy-Authenticate
									default:
										drop();
								}
								break;
							default:
								drop();
						}
						break;
					case 'R':
						_http_stream.ignore(1);
						c = _http_stream.get();
						switch (c) {
							case 'f': // Refresh
							case 't': // Retry-After
							default:
								drop();
						}
						break;
					case 'S':
						c = _http_stream.get();
						switch (c) {
							case 'e':
								c = _http_stream.get();
								switch (c) {
									case 'r': // Server
									case 't': // Set-Cookie
									default:
										drop();
								}
								break;
							case 't': // Strict-Transport-Security
							default:
								drop();
						}
						break;
					case 'T':
						_http_stream.ignore(2);
						c = _http_stream.get();
						switch (c) {
							case 'i': // Trailer
								drop();
								break;
							case 'n': // Transfer-Encoding
								if (_flags & header::transfer_encoding) {
									std::cout << get_value() << std::endl;
									break;
								}
							default:
								drop();
						}
						break;
					case 'V':
						c = _http_stream.get();
						switch (c) {
							case 'a': // Vary
							case 'i': // Via
							default:
								drop();
						}
						break;
					case 'W':
						c = _http_stream.get();
						switch (c) {
							case 'a': // Warning
							case 'W': // WWW-Authenticate
							default:
								drop();
						}
						break;
					default:
						drop();
				}

				// TODO
				// X-Frame-Options, X-XSS-Protection, X-Content-Type-Options, X-Forwarded-Proto, X-Powered-By, X-UA-Compatible

			} while (c != '\r');
		}

	protected:
		std::string get_value() {
			_http_stream.ignore(std::numeric_limits<int>::max(), ' ');

			std::string ret;
			getline(_http_stream, ret, '\r');

			_http_stream.ignore(1); // remove \n
			return ret;
		}

		void drop() {
			_http_stream.ignore(std::numeric_limits<int>::max(), '\n');
		}


		std::istream& _http_stream;
		header _flags;

};

}

#endif
