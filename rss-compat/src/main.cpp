#include <iostream>
#include <sstream>

#include "netstream"

#include "http/http.hpp"

#include <memory>

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
