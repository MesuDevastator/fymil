#pragma once

#include <exception>
#include <log4cxx/logmanager.h>
#include <xercesc/dom/DOM.hpp>

#include "command.hpp"
#include "string.hpp"
#include "../context.hpp"
#include "../utilities/string_conversions.hpp"

namespace fymil::elements
{
	class print final : public command
	{
	private:
		xercesc::DOMElement* element;
		log4cxx::LoggerPtr lg;
		log4cxx::LoggerPtr lg_stdout;
	public:
		explicit print(xercesc::DOMElement* element)
			: element{ element }, lg{ log4cxx::LogManager::getLogger("fymil.elements.command.print") },
			  lg_stdout{ log4cxx::LogManager::getLogger("stdout") }
		{
		}

		[[nodiscard]] const std::string& get_name() const noexcept override
		{
			static const std::string print_name{ "print" };
			return print_name;
		}

		void execute(context& context, const element_manager& element_manager) override
		{
			LOG4CXX_TRACE(lg, "Executing print");
			LOG4CXX_INFO(lg_stdout, fymil::elements::string{ element }.parse(context, element_manager)->as_string());
		}
	};
}
