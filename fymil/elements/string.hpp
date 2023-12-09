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

#include <sstream>
#include <log4cxx/logmanager.h>
#include <xercesc/dom/DOM.hpp>

#include "parser.hpp"
#include "../types/undefined.hpp"
#include "../types/string.hpp"
#include "../utilities/string_conversions.hpp"
#include "../utilities/transcode.hpp"

namespace fymil::elements
{
	class string final : public parser
	{
	private:
		xercesc::DOMElement* element;
		log4cxx::LoggerPtr lg;
	public:
		explicit string(xercesc::DOMElement* element)
			: element{ element }, lg{ log4cxx::LogManager::getLogger("fymil.elements.parser.string") }
		{
		}

		[[nodiscard]] const std::string& get_name() const noexcept override
		{
			static const std::string string_name{ "string" };
			return string_name;
		}

		std::unique_ptr<types::type> parse(context& context, const element_manager& element_manager) override
		{
			std::stringstream ss{};
			xercesc::DOMNodeList* element_children{ element->getChildNodes() };
			LOG4CXX_TRACE(lg, "Parsing to string, " << element_children->getLength() << " nodes");;
			for (XMLSize_t i{}; i < element_children->getLength(); ++i)
			{
				xercesc::DOMNode* cur{ element_children->item(i) };
				auto tag{ utilities::transcode(cur->getNodeName()) };
				LOG4CXX_TRACE(lg, "Iterating node " << i + 1
													<< ", node type " << utilities::to_string(cur->getNodeType())
													<< ", tag " << tag.get());
				if (cur->getNodeType() == xercesc::DOMNode::ELEMENT_NODE)
				{
					std::unique_ptr<elements::element>
						element_{ element_manager.parse(dynamic_cast<xercesc::DOMElement*>(cur)) };
					LOG4CXX_TRACE(lg, "Parsed as element, element type " << utilities::to_string(element_->get_type())
																		 << ", name " << element_->get_name());
					if (element_->get_type() != element::element_type::parser)
					{
						LOG4CXX_ERROR(lg,
							"Expecting a parser, but got a " << utilities::to_string(element_->get_type()) << ": " << element_
								->get_name());
						throw std::exception{};
					}
					std::string result;
					try
					{
						result =
							dynamic_cast<parser*>(element_.get())->parse(context, element_manager)->as_string();
					}
					catch (...)
					{
						LOG4CXX_WARN(lg, "Failed to get parsed result");
						result = types::undefined{}.as_string();
					}
					utilities::trim(result);
					LOG4CXX_TRACE(lg, "Parser result: " << result);
					ss << result;
				}
				if (cur->getNodeType() == xercesc::DOMNode::TEXT_NODE)
				{
					auto text{ utilities::transcode(dynamic_cast<xercesc::DOMText*>(cur)->getTextContent()) };
					std::string trimmed_text{ text.get() };
					utilities::trim(trimmed_text);
					if (!trimmed_text.empty())
					{
						LOG4CXX_TRACE(lg, "Parsed as text: " << trimmed_text);
						ss << trimmed_text;
					}
				}
				if (cur->getNodeType() == xercesc::DOMNode::CDATA_SECTION_NODE)
				{
					auto text{ utilities::transcode(dynamic_cast<xercesc::DOMCDATASection*>(cur)->getTextContent()) };
					LOG4CXX_TRACE(lg, "Parsed as text: " << text.get());
					ss << text.get();
				}
				if (cur->getNodeType() == xercesc::DOMNode::ENTITY_REFERENCE_NODE)
				{
					auto
						text{ utilities::transcode(dynamic_cast<xercesc::DOMEntityReference*>(cur)->getTextContent()) };
					LOG4CXX_TRACE(lg, "Parsed as text: " << text.get());
					ss << text.get();
				}
			}
			std::string result{ ss.str() };
			LOG4CXX_TRACE(lg, "Final string: " << result);
			return std::make_unique<types::string>(result);
		}
	};
}
