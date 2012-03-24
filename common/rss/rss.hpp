#pragma once
#ifndef __RSS_HPP__
#define __RSS_HPP__

#include <list>
#include <memory>
#include <sstream>

#include <xml/xml.hpp>

namespace rss {


template<typename string_type>
class basic_feed_item {
	public:
		basic_feed_item() {
		}

		const string_type& get_title() const {
			return title;
		}

	protected:
		string_type author;
		string_type category;
		string_type comments;
		string_type description;
		string_type enclosure;
		string_type guid;
		string_type link;
		string_type pub_date;
		string_type source;
		string_type title;
};

template<typename itm, typename container_type = std::list<itm> >
class basic_feed {
	public:
		typedef itm item_type;

		basic_feed() {
		}

		void append(const item_type& item) {
			items.push_back(item);
		}

		
		const container_type& get_items() const {
			return items;
		}


	protected:
		container_type items;
};


template<typename feed_type, uint32_t flags>
class basic_parser : public xml::basic_parser<flags> {
	public:

		basic_parser() : parsed_feed(new feed_type()) {
		}

		feed_type* get_feed() {
			return parsed_feed.release();
		}


	protected:
		virtual void on_start() {
			// parsed_feed.reset(new feed());
		}

		virtual void on_end() {
		}

		virtual void on_new_text(const std::string& text) {
			*buffer = text;
		}

		virtual void on_new_attribute(const std::string&, const std::string&) {
		}

		virtual void on_new_node(const std::string& tagname) {
			// TODO
			// image tag subtags

			switch (tagname[0]) {
				case 'a': // author
					buffer = (std::string*)(&item);
					break;
				case 'c':
					switch (tagname[1]) {
						case 'a': // category both
							if (inside_item) {
								buffer = (std::string*)(&item) + 1;
							}
							break;
						case 'h': // channel subroot
							break;
						case 'l': // cloud feed
							break;
						case 'o':
							switch (tagname[2]) {
								case 'm': // comments
									buffer = (std::string*)(&item) + 2;
								case 'p': // copyright feed
									break;
								default:
									break;
							}

							break;

						default:
							break;
					}
					break;
				case 'd':
					switch (tagname[1]) {
						case 'e': // description both
							if (inside_item) {
								buffer = (std::string*)(&item) + 3;
							}
						case 'o': // docs
							break;
						default:
							break;
					}
					break;
				case 'e': // enclosure
					buffer = (std::string*)(&item) + 4;
					break;
				case 'g':
					switch (tagname[1]) {
						case 'u': // guid
							buffer = (std::string*)(&item) + 5;
							break;
						case 'e': // generator feed
							break;
						default:
							break;
					}
					break;
				case 'i':
					switch (tagname[1]) {
						case 'm': // image feed
							break;
						case 't': // item
							inside_item = true;
							break;
						default:
							break;
					}
					break;
				case 'l':
					switch (tagname[1]) {
						case 'a':
							switch (tagname[2]) {
								case 'n': // language feed
								case 's': // lastBuildDate feed
								default:
									break;
							}
							break;

						case 'i': // link both
							if (inside_item) {
								buffer = (std::string*)(&item) + 6;
							}
							break;
						default:
							break;
					}
					break;
				case 'm': // managingEditor feed
					break;
				case 'p': // pubDate both
					if (inside_item) {
						buffer = (std::string*)(&item) + 7;
					}
					break;
				case 'r': // rating feed, rss root
					break;
				case 's':
					switch (tagname[1]) {
						case 'k': // skipDays feed, skipHours feed
							break;
						case 'o': // source
							buffer = (std::string*)(&item) + 8;
							break;
						default:
							break;
					}
					break;
				case 't':
					switch (tagname[1]) {
						case 'e': // textInput feed
							break;
						case 'i': // title
							buffer = (std::string*)(&item) + 9;
							break;
						case 't': // ttl feed
							break;
						default:
							break;
					}
				case 'w': // webMaster feed
					break;
				default:
					break;
			}
		}

		virtual void on_comment(const std::string&) {
		}

		virtual void on_tag_close(const std::string& tagname) {
			if (tagname == "item") {
				parsed_feed->append(item);
				item = typename feed_type::item_type();
				inside_item = false;
			}
		}

		virtual void on_error() {
		}

		
		bool inside_item;
		std::string* buffer;
		typename feed_type::item_type item;
		std::unique_ptr<feed_type> parsed_feed;
};

typedef basic_feed_item<std::string> item;
typedef basic_feed<item> feed;
typedef basic_parser<feed, 0> parser;



} //namespace rss


#endif
