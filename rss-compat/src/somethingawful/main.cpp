#include "rss.cpp"

int main()
{
	RssReader rs("proba.txt");
	std::cout<<rs.toString();
	return 0;
}

