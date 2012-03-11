#include "rss.hpp"

//////////////////////// RSSITEM

string RssItem::toString(){
	return ("<item>\n\t<title>"+title+"</title>\n\t<link>"+link+"</link>\n\t<descrtiption>"+description+"</description>\n\t<pubDate>"+pubDate+"</pubDate>\n</item>\n");
}

//////////////////////// RSS

void Rss::addRssItem(RssItem item){
	items.push_back(item);
}

string Rss::toString(){
	string tmp = "\n\n<rss>\n<channel>\n<title>"+title+"</title>\n<link>"+link+"</link>\n<description>"+description+"</description>\n";
	for(int i=0;i<items.size();i++){
		tmp += items[i].toString();
	}
	tmp += "</channel>\n</rss>\n\n";
	return tmp;
}

//////////////////////////// RSSREADER

RssReader::RssReader(string filename){
	download(filename);
}

RssReader::~RssReader(){
	for(int i=0;i<urls.size();i++){
		delete[] urls[i];
	}
}

Rss RssReader::parseurl(string url[]){
	inet4_address ia(url[0].c_str(),80);
	std::socket<inet4_address> soc(ia);
	std::inet4stream ns(soc);
	ns << "GET /"+url[1]+" HTTP/1.1\r\n" << "Host:"+url[0]+"\r\n" << "\r\n";
	xml::dom_parser<xml::document> dp;
	http::parser<0> p(ns);
	p.parse_response();
	dp.parse(ns);
	xml::document* doc = dp.document();
	Rss channel(XPATH_GET_FIRST_TEXT("/rss/channel/title"),XPATH_GET_FIRST_TEXT("/rss/channel/link"),XPATH_GET_FIRST_TEXT("/rss/channel/description"));
	list<basic_node*> title = doc->xpath("/rss/channel/item/title");
	list<basic_node*> link = doc->xpath("/rss/channel/item/link");
	list<basic_node*> desc = doc->xpath("/rss/channel/item/description");
	list<basic_node*> pd = doc->xpath("/rss/channel/item/pubDate");
	auto itl = link.begin();
	auto itd = desc.begin();
	auto itp = pd.begin();
	for(auto it = title.begin(); it != title.end();++it){
		channel.addRssItem(RssItem(ITERATOR_GTF(it),ITERATOR_GTF(itl),ITERATOR_GTF(itd),ITERATOR_GTF(itp)));
	}
	return channel;
}

void RssReader::refresh(){
	vector<Rss> tmp;
	for(unsigned int i=0; i<urls.size();i++){
		tmp.push_back(parseurl(urls[i]));
	}
	rssvector=tmp;
}

//vector<Rss> Rssreader::download(string filename){
void RssReader::download(string filename){
	
	std::ifstream f(filename.c_str());
	string host[2];
	getline(f,host[0],'/');
	getline(f,host[1]);
		while(!f.fail()){
		urls.push_back(host);
		getline(f,host[0],'/');
		getline(f,host[1]);
	}
	refresh();
}

string RssReader::toString(){
	string ret = "";
	for(int i=0;i<rssvector.size();i++){
		ret += rssvector[i].toString();
	}
	return ret;
}


