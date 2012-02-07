#include <iostream>

#include "netstream"

int main() {
	using namespace std;

	inet4stream ns("127.0.0.1", 80);
	ns << "GET / HTTP/1.1\r\n" << "Host: localhost\r\n" << "\r\n";
	ns.flush();
	while (ns.good()) {
		std::string s;
		getline(ns, s);
		std::cout << s << std::endl;
	}

	return 0;
}
