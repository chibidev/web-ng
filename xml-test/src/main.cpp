#include <iostream>
#include <fstream>
#include "xml/xml.hpp"

int main() {
	using namespace std;
	using namespace xml;

	ifstream f("test.xml");

	dom_parser<document> p;

	p.parse(f);

	document* doc = p.document();

	list<basic_node*> nl = doc->xpath("/rss/channel/item/title");
	for (auto it = nl.begin(); it != nl.end(); ++it) {
		std::cout << (*it)->get_first_child()->get_value() << std::endl;
	}

	delete doc;

	return 0;
}
