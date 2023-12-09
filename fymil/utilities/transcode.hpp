#pragma once

#include <functional>
#include <memory>

#include <xercesc/dom/DOM.hpp>

namespace fymil::utilities
{
	std::unique_ptr<char, std::function<void(char*)>> transcode(const XMLCh* str)
	{
		return std::unique_ptr<char, std::function<void(char*)>>{ xercesc::XMLString::transcode(str),
			[](char* str_to_delete)
			{ xercesc::XMLString::release(&str_to_delete); }};
	}

	std::unique_ptr<XMLCh, std::function<void(XMLCh*)>> transcode(const char* str)
	{
		return std::unique_ptr<XMLCh, std::function<void(XMLCh*)>>{ xercesc::XMLString::transcode(str),
			[](XMLCh* str_to_delete)
			{ xercesc::XMLString::release(&str_to_delete); }};
	}
}
