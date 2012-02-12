#include <iostream>
#include <sstream>

#include "netstream"

#include "http/http.hpp"

#ifdef AS_MODULE
extern char* cache;
#else
char* cache;
#endif

namespace rss {

class server {
	public:
		server() {
		}

		void download_feed(const std::string& url) {
			std::stringstream ss(url);
			std::string protocol, srv, path;
			char drop;

			getline(ss, protocol, ':');

			ss >> drop >> drop;

			getline(ss, srv, '/');
			getline(ss, path);

			std::socket<std::inet4_address> sock(std::inet4_address(srv.c_str(), 80));
			std::inet4stream ns(sock);

			ns << "GET /" << path << " HTTP/1.1" << "\r\n";
			ns << "Host: " << srv << "\r\n";
			ns << "Accept-Encoding: identity\r\n";
			ns << "Connection: close\r\n";
			ns << "\r\n" << std::flush;

			//while (ns.good()) {
			//	std::string line;
			//	getline(ns, line);
			//	std::cout << line << std::endl;
			//}
			
			http::parser p(ns);
			p.parse_response();

			//std::socket<std::inet4_address> sock(std::inet4_address("www.batoto.net", 80));
		}
	protected:
};

} //namespace rss

int main() {
	using namespace std;
	using namespace rss;

	rss::server s;

	s.download_feed("http://www.batoto.net/recent_rss");
	//s.download_feed("http://hc.apache.org/httpcomponents-core-ga/tutorial/html/advanced.html");

	return 0;
}
