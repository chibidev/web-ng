#pragma once
#ifndef __STD__FILTERS__HPP__
#define __STD__FILTERS__HPP__

#include <streambuf>
#include <iostream>

namespace std {

template<typename charT, typename traits = std::char_traits<charT> >
class basic_filter_streambuf : public std::basic_streambuf<charT, traits> {
	public:
		typedef typename traits::int_type int_type;

		basic_filter_streambuf() : _delegate(0) {
		}

		basic_filter_streambuf(std::basic_streambuf<charT, traits>* buf) : _delegate(buf) {
		}

		void delegate(std::basic_streambuf<charT, traits>* buf) {
			_delegate = buf;
		}

	protected:
		virtual int_type overflow(int_type c) {
			return _delegate->sputc(traits::to_char_type(c));
		}

		virtual int_type uflow() {
			return _delegate->sbumpc();
		}

		virtual int_type underflow() {
			return _delegate->sgetc();
		}

		std::basic_streambuf<charT, traits>* _delegate;
};

template<typename charT, typename traits = std::char_traits<charT> >
class basic_stream_stack : public std::basic_iostream<charT, traits> {
	public:
		basic_stream_stack(std::basic_ios<charT, traits>& stream) : std::basic_iostream<charT, traits>(stream.rdbuf()) {
		}

		void push(std::basic_ios<charT, traits>& stream_filter) {
			static_cast<basic_filter_streambuf<charT, traits>*>(stream_filter.rdbuf())->delegate(this->rdbuf());
			this->rdbuf(stream_filter.rdbuf());
		}

	protected:
};

template<typename charT, typename traits = std::char_traits<charT>, typename streambuf_type = std::basic_streambuf<charT, traits> >
class basic_filter_stream : public std::basic_iostream<charT, traits> {
	public:
		basic_filter_stream() : std::basic_iostream<charT, traits>(&streambuf) {
		}

	protected:
		streambuf_type streambuf;
};







typedef basic_stream_stack<char> streamstack;
typedef basic_stream_stack<wchar_t> wstreamstack;


} // namespace std


#endif
