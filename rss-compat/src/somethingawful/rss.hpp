#ifndef rss_hpp_include
#define rss_hpp_include

//#include<iostream>
//#include<vector>
#include <netstream> //kell elérési útvonala a commonnak fordításkor!!!
//#include <socket>
#include <http/http.hpp>
#include <xml/xml.hpp>
#include <fstream>
#include <istream>


#define XPATH_GET_FIRST_TEXT(xPath) doc->xpath(xPath).front()->get_first_child()->get_value()
#define ITERATOR_GTF(it) (*it)->get_first_child()->get_value()

using std::string;
using std::list;
using std::getline;
using std::inet4_address;
//using std::socket;
using std::vector;
using xml::basic_node;

class RssItem{
	string title,link,description, pubDate;
	public:
	RssItem(string Title,string Link,string Description,string Pubdate = ""):title(Title),link(Link),description(Description),pubDate(Pubdate) {}
	//TODO ... amire szukseg lesz ...
	string toString();
};


class Rss{
	string title,link,description;
	vector<RssItem> items;
	public:
	Rss(string Title,string Link,string Description = ""):title(Title),link(Link),description(Description) {}
	void addRssItem(RssItem item);
//	RssItem removeRssItem(int index); //?
	string toString();
};


class RssReader{
	Rss parseurl(string url[]);
	vector<Rss> rssvector;
	vector<string*> urls;

public:
	RssReader(){}
	RssReader(string filename);
//	vector<Rss> download(string filename);
	void refresh();
	void download(string filename);
	~RssReader();
	string toString();
};




#endif
