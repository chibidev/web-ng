#ifndef __HTTP_HPP__
#define __HTTP_HPP__

#include <limits>
#include "../math"

namespace http {

class header {
	public:
		static const uint64_t accept						= 0x000000000000001; // request
		static const uint64_t accept_charset				= 0x000000000000002; // request
		static const uint64_t accept_encoding				= 0x000000000000004; // request
		static const uint64_t accept_language 				= 0x000000000000008; // request
		static const uint64_t accept_ranges 				= 0x000000000000010; // response
		static const uint64_t allow 						= 0x000000000000020; // response
		static const uint64_t authorization 				= 0x000000000000040; // request
		static const uint64_t cache_control 				= 0x000000000000080; // request response
		static const uint64_t connection 					= 0x000000000000100; // request response
		static const uint64_t cookie 						= 0x000000000000200; // request
		static const uint64_t content_disposition			= 0x000000000000400; // response
		static const uint64_t content_encoding 				= 0x000000000000800; // response
		static const uint64_t content_language 				= 0x000000000001000; // response
		static const uint64_t content_length 				= 0x000000000002000; // request response
		static const uint64_t content_location 				= 0x000000000004000; // response
		static const uint64_t content_md5 					= 0x000000000008000; // request response
		static const uint64_t content_range 				= 0x000000000010000; // response
		static const uint64_t content_type 					= 0x000000000020000; // request response
		static const uint64_t date 							= 0x000000000040000; // request response
		static const uint64_t etag 							= 0x000000000080000; // response
		static const uint64_t expect 						= 0x000000000100000; // request
		static const uint64_t expires 						= 0x000000000200000; // response
		static const uint64_t from 							= 0x000000000400000; // request
		static const uint64_t host 							= 0x000000000800000; // request
		static const uint64_t if_match 						= 0x000000001000000; // request
		static const uint64_t if_modified_since 			= 0x000000002000000; // request
		static const uint64_t if_none_match 				= 0x000000004000000; // request
		static const uint64_t if_range 						= 0x000000008000000; // request 
		static const uint64_t if_unmodified_since 			= 0x000000010000000; // request
		static const uint64_t last_modified 				= 0x000000020000000; // response
		static const uint64_t link 							= 0x000000040000000; // response
		static const uint64_t location 						= 0x000000080000000; // response
		static const uint64_t max_forwards 					= 0x000000100000000; // request 
		static const uint64_t p3p 							= 0x000000200000000; // response
		static const uint64_t pragma 						= 0x000000400000000; // request response
		static const uint64_t proxy_authenticate 			= 0x000000800000000; // response
		static const uint64_t proxy_authorization 			= 0x000001000000000; // request
		static const uint64_t range 						= 0x000002000000000; // request
		static const uint64_t referer 						= 0x000004000000000; // request 
		static const uint64_t refresh 						= 0x000008000000000; // response
		static const uint64_t retry_after 					= 0x000010000000000; // response
		static const uint64_t server 						= 0x000020000000000; // response
		static const uint64_t set_cookie 					= 0x000040000000000; // response
		static const uint64_t strict_transport_security		= 0x000080000000000; // response
		static const uint64_t te 							= 0x000100000000000; // request 
		static const uint64_t trailer						= 0x000200000000000; // response
		static const uint64_t transfer_encoding 			= 0x000400000000000; // response
		static const uint64_t upgrade 						= 0x000800000000000; // request
		static const uint64_t user_agent 					= 0x001000000000000; // request
		static const uint64_t vary 							= 0x002000000000000; // response
		static const uint64_t via 							= 0x004000000000000; // request response
		static const uint64_t warning 						= 0x008000000000000; // request reponse
		static const uint64_t www_authenticate	 			= 0x010000000000000; // response
		static const uint64_t all							= 0x0FFFFFFFFFFFFFF;
};

class options {
	public:
		static const uint64_t strict						= 0x100000000000000u;
};


class request {
};

class response {
	public:
		response() : _values(42) {
		}

		template<uint64_t opt>
		void set(const std::string& value) {
			_values[log(2, opt)] = value;
		}

		template<uint64_t opt>
		const std::string& get() {
			return _values[log(2, opt)];
		}

	protected:
		std::vector<std::string> _values;

};


// If strict parsing is enabled, compare the incoming chars to str
// If parsing is not strict, discard the characters
#define EXPECT(num, str) \
if (flags & options::strict) { \
	if (expect_char<num>(str)) break; \
} else { \
	_http_stream.ignore(num); \
}


// Leaf version of EXPECT. If we are at a leaf in parsing tree, discarding upon
// lazy parsing is not required.
#define LEXPECT(num, str) \
if (flags & options::strict) { \
	if (expect_char<num>(str)) break; \
}

template<uint64_t flags = header::all>
class parser {
	public:
		
		parser(std::istream& __http_stream) : _http_stream(__http_stream) {
		}
				
		//request
		void parse_request() {
			int c = _http_stream.get();
			switch (c) {
				case 'A':
					c = _http_stream.get();
					switch (c) {
						case 'c':
							/*
							if (flags & options::strict) {
								if (expect_char<4>("cept")) break;
							} else {
								_http_stream.ignore(4);
							}
							*/

							EXPECT(4, "cept");

							c = _http_stream.get();
							switch(c) {
								case ':': // Accept
									drop();
									break;
								case '-':
									c = _http_stream.get();
									switch (c) {
										case 'C': // Accept-Charset
										case 'E': // Accept-Encoding
										case 'L': // Accept-Language
										default:
											drop();
									}
									break;
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
											drop();
											break;
										case 't':
											//_http_stream.ignore(4);
											EXPECT(4, "ent-")

											c = _http_stream.get();
											switch (c) {
												case 'L': // Content-Length
												case 'M': // Content-MD5
												case 'T': // Content-Type
												default:
													drop();
											}
											break;
										default:
											drop();
									}
									break;
								case 'o': // Cookie
								default:
									drop();
							}
							break;
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
					//_http_stream.ignore(2);
					EXPECT(2, "f-")
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
					//_http_stream.ignore(1);
					EXPECT(1, "r")
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
		
		response parse_response() {
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
								//_http_stream.ignore(1);
								EXPECT(1, "n")

								c = _http_stream.get();
								switch (c) {
									case 'n': // Connection
										drop();
										break;
									case 't':
										//_http_stream.ignore(4);
										EXPECT(4, "ent-");

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
						//_http_stream.ignore(1);
						EXPECT(1, "e")

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
						//_http_stream.ignore(2);
						EXPECT(2, "ra")

						c = _http_stream.get();
						switch (c) {
							case 'i': // Trailer
								drop();
								break;
							case 'n': // Transfer-Encoding
								if (flags & header::transfer_encoding) {
									r.set<header::transfer_encoding>(get_value());
									break;
								}
								drop();
								break;
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
					case '\r':
						return r;
					default:
						drop();
				}

				// TODO
				// X-Frame-Options, X-XSS-Protection, X-Content-Type-Options, X-Forwarded-Proto, X-Powered-By, X-UA-Compatible

			} while (_http_stream.good());

			return r;
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



		// problem: if parsing is not strict, then an unnecessary ignore is in the function (get_value skips to the appropriate value, no need to use ignore in this function).
		// but, if parsing is not strict, and we are not in a leaf in parsing tree, then the ignore is needed
		template<unsigned int n>
		int expect_char(const char* expected) {
			unsigned int i = 0;
			char c;

			do {
				c = static_cast<char>(_http_stream.get());
				if (c != *expected) {
					drop();
					return -1;
				}
				++expected;
			} while (++i < n);

			return 0;
		}

		// Partial function specialization is not working!
		/*template<>
		int expect_char<1>(const char* expected) {
			char c = _http_stream.get();
			if (c != *expected) {
				drop();
				return -1;
			}

			return 0;

		}*/

		std::istream& _http_stream;
		header _flags;

};


/*
class protocol_interpreter {
	public:
		void send() {
			std::socket<std::inet4_address> sock(std::inet4_address(_options[1], 80));
			std::inet4stream stream(sock);

			http_parser<> parser(stream);

			stream << "GET " << _options[2] << " HTTP/1.1\r\n";
			stream << "Host: " << _options[1] << "\r\n";
			stream << "Accept-Encoding: chunked\r\n"; // Handle only chunked encoding for the time being
			stream << "Connection: close\r\n"; // persistent connection is not available atm
			stream << "\r\n";

			http::response resp = parser.parse_response();

			std::stringstream message_stream;

			if (resp.get<http::header::transfer_encoding>() == "chunked") {
				int chunk;
				do {
					stream >> std::hex >> chunk;
					std::string ch(chunk, '\0');
					stream.read(&ch[0], chunk);

					message_stream << ch;
				} while (chunk > 0 && stream.good());
			}

			return message_stream;
		}
	protected:
		std::vector<std::string> _options;
};
*/
}

#endif
