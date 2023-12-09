/*
 * Copyright 2023 Mesu Devastator
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
