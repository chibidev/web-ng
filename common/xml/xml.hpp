#pragma once
#ifndef __XML_XML_HPP__
#define __XML_XML_HPP__

#include <stack>
#include <string>
#include <sstream>
#include <limits>
#include <memory>
#include <list>
#include <map>


#include <algorithm>
#include <functional>
#include <locale>

namespace xml {

std::stringstream& trim(std::stringstream& string) {
	string >> std::ws;
	return string;
}


class parser {
	public:
		static const uint32_t strict = 0x01;
};


// trim from start
static inline std::string &ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}


static inline std::string &trim(std::string &s) {
	return ltrim(rtrim(s));
}



#define CLR_SSTREAM(stream) \
stream.str(string()); \
stream.clear();


template<uint32_t flags = 0>
class basic_parser {
	public:
		virtual ~basic_parser() {
		}

		void parse(std::istream& stream) {
			using namespace std;

			char c;
			stringstream tag_name, node_value, attribute_name, attribute_value, text;

			stream.ignore(numeric_limits<int>::max(), '<');
			_state = state::tag;
			_next = state::tag;

			on_start();

			do {
				// TODO
				// <tagname/> handling


				c = (char) stream.get();

				_state = _next;

				switch (_state) {
					case state::tag:
						switch (c) {
							case '/':
								_next = state::tag_close;
								break;
							case '?':
								_next = state::dtd;
								break;
							case '!':
								_next = state::c;
								break;
							default:
								tag_name << c;
								_next = state::tag_open;
								break;
						}
						break;

					case state::tag_close:
						switch (c) {
							case '>':
								_next = state::text;

								// state transition
								//
								
								on_tag_close(tag_name.str());
								CLR_SSTREAM(tag_name);

								break;
							default:
								tag_name << c;
						}
						break;

					case state::dtd:
						{
							std::string tmp;
							getline(stream, tmp, '>');
							//std::cout << c << tmp << std::endl;
						}
						//stream.ignore(numeric_limits<int>::max(), '>');
						_next = state::text;
						break;

					case state::c:
						switch (c) {
							case '-':
								// if strict check next, expect '-'
								// else ignore the next character, assume '-'
								stream.ignore(1);
								_next = state::comment;
								break;
							case '[':
								// if strict check next 6 chars, expect "CDATA["
								// else ignore the next 6 chars, assume "CDATA["
								stream.ignore(6);
								_next = state::cdata;
								break;
							default:
								// error
								std::cout << "error " << __LINE__ << std::endl;
						}
						break;
					
					case state::tag_open: 
						switch (c) {
							case ' ':
								_next = state::attr_name;
								break;
							case '>':
								_next = state::text;
								break;
							// case illegal character:
							// case illegal char2:
							// ...
							// 		error
							default:
								tag_name << c;
								goto end;
						}
						on_new_node(tag_name.str());

						CLR_SSTREAM(tag_name);

						end:
						break;

					case state::comment:
						switch (c) {
							case '-':
								// expect "->"
								stream.ignore(2);
								_next = state::text;
								break;
							default:
								// ignore comment for the time being
								(void) _next;
						}
						break;

					case state::cdata:
						switch (c) {
							case ']':
								// expect and check "]>"
								{
								char l1 = (char) stream.get();
								char l2 = (char) stream.get();
								if (l1 == ']' && l2 == '>') {
									_next = state::text;
									on_new_text(node_value.str());
									CLR_SSTREAM(node_value);
								} else {
									node_value << ']';
									stream.unget();
									stream.unget();
								}
								}

								break;
							default:
								node_value << c;
						}
						break;

					case state::attr_name:
						switch (c) {
							case '=':
								// if strict, check next char, expect '"'
								// else ignore next character, assume '"'
								stream.ignore(1);
								_next = state::attr_val;
								break;
							case '>':
								//std::cout << attribute_value.str() << '>';
								//attribute_value.str(string());
								//attribute_value.clear();
								_next = state::text;
								break;
							default:
								if (! isspace(c)) attribute_name << c;
						}
						break;

					case state::attr_val:
						switch (c) {
							case '"':
								_next = state::attr_name;

								on_new_attribute(attribute_name.str(), attribute_value.str());

								CLR_SSTREAM(attribute_name);
								CLR_SSTREAM(attribute_value);
								break;

							default:
								attribute_value << c;
						}
						break;

					case state::text:
					switch (c) {
							case '<':
								_next = state::tag;

								{
									string t = text.str();
									if (trim(t).size() > 0) on_new_text(t);
								}

								CLR_SSTREAM(text);
								break;
							default:
								text << c;
						}
						break;

					default:
						std::cout << "no state" << std::endl;
				}
			} while (c != EOF);


			//document* doc = new document();
			//

			on_end();
		}

	protected:
		virtual void on_start() {
		}

		virtual void on_end() {
		}

		virtual void on_new_text(const std::string&) {
		}

		virtual void on_new_attribute(const std::string&, const std::string&) {
		}

		virtual void on_new_node(const std::string&) {
		}

		virtual void on_comment(const std::string&) {
		}

		virtual void on_tag_close(const std::string&) {
		}

		virtual void on_error() {
		}

		enum class state {
			text, tag, tag_open, tag_close, dtd, c, comment, cdata, attr_name, attr_val
		};

		state _state;
		state _next;
};

template<typename _TD, typename _TN = typename _TD::node_type, uint32_t flags = 0>
class dom_parser : public basic_parser<flags> {
	public:
		typedef _TD document_type;
		typedef _TN node_type;

		document_type* document() {
			return _document.release();
		}

	protected:
		virtual void on_start() {
		}

		virtual void on_end() {
			_document.reset(new _TD(_node_stack.top()));
		}

		virtual void on_new_text(const std::string& text) {
			node_type* tnode = new node_type(std::string(), text);
			_node_stack.top()->add_child(tnode);
		}

		virtual void on_new_attribute(const std::string& name, const std::string& value) {
			if (_node_stack.empty()) return; //error

			_node_stack.top()->add_attribute(name, value);
		}

		virtual void on_new_node(const std::string& name) {
			node_type* node = new node_type(name);
			if (! _node_stack.empty()) _node_stack.top()->add_child(node);
			_node_stack.push(node);
		}

		virtual void on_tag_close(const std::string&) {
			if (_node_stack.empty()) return; //error
			if (_node_stack.size() > 1) _node_stack.pop();
		}

		virtual void on_error() {
		}


		std::stack<node_type*> _node_stack;
		std::unique_ptr<document_type> _document;
};


class basic_node {
	public:
		basic_node(const std::string& tagname, const std::string& value = std::string()) : _tagname(tagname), _value(value) {
		}

		~basic_node() {
			// in the future, usage of unique_ptr is preferred. But for the time being, a copy operator is not implemented
			for (std::list<basic_node*>::iterator it = _children.begin(); it != _children.end(); ++it) {
				delete *it;
			}
		}

		void add_attribute(const std::string& name, const std::string& value) {
			_attr[name] = value;
		}

		void add_child(basic_node* const node) {
			_children.push_back(node);
		}

		const std::string& get_value() const {
			return _value;
		}

		const std::string& get_name() const {
			return _tagname;
		}

		void xpath(std::string path, std::list<basic_node*>& node_list) {
			std::string tagname = path.substr(0, path.find_first_of('/'));
			path.erase(0, tagname.length() + 1);

			for (std::list<basic_node*>::iterator it = _children.begin(); it != _children.end(); ++it) {
				if ((*it)->get_name() == tagname) {
					if (path != std::string()) {
						(*it)->xpath(path, node_list);
					} else {
						node_list.push_back(*it);
					}
				}
			}
		}

		basic_node* get_first_child() {
			return _children.front();
		}
		
	protected:
		std::string _tagname;
		std::string _value;

		basic_node* _parent;
		std::list<basic_node*> _children;
		std::map<std::string, std::string> _attr;
};


template<typename _TN>
class basic_document {
	public:
		typedef _TN node_type;

		basic_document(node_type* const root) : _root(root) {
		}

		~basic_document() {
		}

		std::list<node_type*> xpath(std::string path) {
			path.erase(0, 1);
			std::cout << path << std::endl;

			path.erase(0, path.find_first_of('/') + 1);
			std::cout << path << std::endl;
	
			std::list<node_type*> ret;

			_root->xpath(path, ret);

			return ret;
		}
	
	protected:
		std::unique_ptr<node_type> _root;
};

typedef basic_document<basic_node> document;



template<uint32_t flags = 0>
class xml_compiler : public basic_parser<flags> {
	protected:
		virtual void on_start() {
		}

		virtual void on_end() {
		}

		virtual void on_new_text(const std::string& text) {
			std::cout << "mkt " << text << std::endl;
		}

		virtual void on_new_attribute(const std::string& name, const std::string& value) {
			std::cout << "mka " << name << std::endl;
			std::cout << "sav " << value << std::endl;
		}

		virtual void on_new_node(const std::string& name) {
			std::cout << "push " << name << std::endl;
		}

		virtual void on_tag_close(const std::string& name) {
			std::cout << "pop " << name << std::endl;
		}

		virtual void on_error() {
		}
};

} // namespace xml

#endif
