#include <iostream>

#include "netstream"

/*std::ostream& operator<<(std::ostream& __stream, const std::string& __str) {
	std::cout << "writing: " << __str << std::endl;

	std::endl(std::operator<<(std::operator<<(std::cout, "writing: "), __str));

	return std::operator<<(__stream, __str);
}*/

int main() {
	using namespace std;

	server_socket<inet4_address> server;
	server.bind(inet4_address("127.0.0.1", 31337));
	server.listen();

	std::socket<inet4_address>* client = server.accept();
	inet4stream ns(*client);

	while (ns.good()) {
		string s;
		ns >> s;
		ns << s << "\r\n";
		ns.flush();
	}
	
	delete client;

	return 0;
}
