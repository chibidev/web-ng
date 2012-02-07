#include <iostream>

#include "netstream"

int main() {
	using namespace std;

	std::socket<inet4_address> sock(inet4_address("127.0.0.1", 80));
	inet4stream ns(sock);
	ns << "GET / HTTP/1.1\r\n" << "Host: localhost\r\n" << "\r\n";
	ns.flush();
	while (ns.good()) {
		std::string s;
		getline(ns, s);
		std::cout << s << std::endl;
	}

	return 0;
}
