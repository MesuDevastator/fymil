#pragma once

#include <map>
#include <memory>
#include <functional>
#include <string>

#include <xercesc/dom/DOM.hpp>
#include <log4cxx/logmanager.h>

#include "context.hpp"
#include "elements/element.hpp"
#include "utilities/transcode.hpp"

namespace fymil
{
	class element_manager final
	{
	private:
		std::map<std::string, std::function<elements::element*(xercesc::DOMElement*)>> elements;
		log4cxx::LoggerPtr lg;
	public:
		element_manager() noexcept
			: elements{}, lg{ log4cxx::LogManager::getLogger("fymil.element_manager") }
		{
		}

		void register_element(const char* tag, const std::function<elements::element*(xercesc::DOMElement*)>& ctor)
		{
			LOG4CXX_TRACE(lg, "Registering element " << tag);
			elements[std::string{ tag }] = ctor;
		}

		auto unregister_element(const char* tag)
		{
			LOG4CXX_TRACE(lg, "Unregistering element " << tag);
			return elements.erase(std::string{ tag });
		}

		std::unique_ptr<elements::element> parse(xercesc::DOMElement* dom_element) const
		{
			if (dom_element == nullptr)
				throw std::invalid_argument{ "DOM element can't be nullptr" };
			auto tag{ utilities::transcode(dom_element->getNodeName()) };
			elements::element* element{ elements.at(std::string{ tag.get() })(dom_element) };
			LOG4CXX_TRACE(lg, "Parsed element " << tag.get());
			return std::unique_ptr<elements::element>{ element };
		}

		~element_manager()
		{
			LOG4CXX_TRACE(lg, "Releasing element manager");
		}
	};
}
