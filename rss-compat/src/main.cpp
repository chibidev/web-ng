#include <iostream>

#include <netstream>
#include <http/http.hpp>
#include <streams/filters.hpp>
#include <rss/rss.hpp>

#ifdef AS_MODULE
extern char* cache;
#else
char* cache;
#endif

int main() {
	/*ifstream feed("test.xml");

	rss_parser<0> parser;
	parser.parse(feed);


	unique_ptr<rss::feed> feedptr(parser.get_feed());

	std::cout << feedptr->get_first_item().get_title() << std::endl;*/


	std::inet4_address addr("feeds.feedburner.com", 80);
	std::socket<std::inet4_address> sock(addr);
	std::inet4stream ns(sock);

	ns << "GET /HUP HTTP/1.1\r\n" << "Host: feeds.feedburner.com\r\nConnection: close\r\n\r\n";
	ns.flush();

	std::streamstack st(ns);
	http::clientstream/*<http::header::all>*/ ci;

	st.push(ci);

	rss::parser parser;
	parser.parse(st);

	std::unique_ptr<rss::feed> fp(parser.get_feed());
	
	std::list<rss::item> items = fp->get_items();
	for (auto& item : items) {
		std::cout << item.get_title() << std::endl;
	}

	return 0;
}
