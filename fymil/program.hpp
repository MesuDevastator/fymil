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

#include <memory>
#include <functional>
#include <vector>
#include <xercesc/dom/DOM.hpp>
#include <log4cxx/logmanager.h>

#include "element_manager.hpp"
#include "elements/element.hpp"
#include "elements/print.hpp"
#include "utilities/string_conversions.hpp"
#include "utilities/transcode.hpp"

namespace fymil
{
	class program
	{
	private:
		std::unique_ptr<xercesc::DOMLSParser, std::function<void(xercesc::DOMLSParser*)>> parser;
		xercesc::DOMDocument* document;
		std::vector<std::unique_ptr<elements::command>> commands;
		log4cxx::LoggerPtr lg;
		const char* file;
	public:
		explicit program(const char* file, const element_manager& element_manager)
			: file{ file }, lg{ log4cxx::LogManager::getLogger("fymil.program") }, commands{}
		{
			constexpr const char* impl_name{ "LS" };
			xercesc::DOMImplementationLS* impl{
				reinterpret_cast<xercesc::DOMImplementationLS*>(xercesc::DOMImplementationRegistry::getDOMImplementation(
					utilities::transcode(impl_name).get()))
			};
			if (impl == nullptr)
			{
				LOG4CXX_ERROR(lg, "(" << file << ") Failed to get DOM implementation " << impl_name);
				throw std::exception{};
			}
			LOG4CXX_TRACE(lg, "(" << file << ") Got DOM implementation " << impl_name);

			parser = std::unique_ptr<xercesc::DOMLSParser, std::function<void(xercesc::DOMLSParser*)>>{
				(impl->createLSParser(xercesc::DOMImplementationLS::MODE_SYNCHRONOUS, nullptr)),
				[this, file](xercesc::DOMLSParser* parser_to_delete)
				{
					parser_to_delete->release();
					LOG4CXX_TRACE(lg, "(" << file << ") Released parser");
				}
			};
			if (parser == nullptr)
			{
				LOG4CXX_ERROR(lg, "(" << file << ") Failed to create parser");
				throw std::exception{};
			}
			LOG4CXX_TRACE(lg, "(" << file << ") Created parser");

			parser->getDomConfig()->setParameter(xercesc::XMLUni::fgDOMElementContentWhitespace, false);
			parser->getDomConfig()->setParameter(xercesc::XMLUni::fgDOMComments, false);

			try
			{
				document = parser->parseURI(file);
				document->normalize();
				document->normalizeDocument();
			}
			catch (const xercesc::XMLException& e)
			{
				LOG4CXX_ERROR(lg,
					"(" << file << ") Failed to parse document: " << utilities::transcode(e.getMessage()).get());
				throw;
			}
			catch (const xercesc::DOMException& e)
			{
				LOG4CXX_ERROR(lg, "(" << file << ") Failed to parse document: " << utilities::transcode(e.msg).get());
				throw;
			}
			LOG4CXX_TRACE(lg, "(" << file << ") Parsed document");

			LOG4CXX_TRACE(lg,
				"(" << file << ") XML Version: " << utilities::transcode(document->getXmlVersion()).get());
			LOG4CXX_TRACE(lg,
				"(" << file << ") XML Encoding: " << utilities::transcode(document->getXmlEncoding()).get());
			LOG4CXX_TRACE(lg,
				"(" << file << ") XML Standalone: "
					<< (document->getXmlStandalone() ?
						"true" :
						"false"));

			xercesc::DOMNodeList
				* program_nodes{ document->getElementsByTagName(utilities::transcode("program").get()) };
			if (program_nodes->getLength() != 1 ||
				program_nodes->item(0)->getNodeType() != xercesc::DOMNode::ELEMENT_NODE)
			{
				LOG4CXX_ERROR(lg, "(" << file << ") Failed to find program root node");
				throw std::exception{};
			}
			xercesc::DOMElement* program{ dynamic_cast<xercesc::DOMElement*>(program_nodes->item(0)) };
			LOG4CXX_TRACE(lg, "(" << file << ") Found program node, " << program_nodes->getLength() << " child nodes");

			// MAIN parsing
			xercesc::DOMNodeList* main_nodes{ program->getElementsByTagName(utilities::transcode("main").get()) };
			if (main_nodes->getLength() != 1 ||
				main_nodes->item(0)->getNodeType() != xercesc::DOMNode::ELEMENT_NODE)
			{
				LOG4CXX_ERROR(lg, "(" << file << ") Failed to find main node");
				throw std::exception{};
			}
			xercesc::DOMElement* main{ dynamic_cast<xercesc::DOMElement*>(main_nodes->item(0)) };
			xercesc::DOMNodeList* main_children{ main->getChildNodes() };
			LOG4CXX_TRACE(lg, "(" << file << ") Found main node, " << main_children->getLength() << " child nodes");
			for (XMLSize_t i{}; i < main_children->getLength(); ++i)
			{
				xercesc::DOMNode* cur{ main_children->item(i) };
				LOG4CXX_TRACE(lg, "(" << file << ") Iterating node " << i + 1
									  << ", node type " << utilities::to_string(cur->getNodeType())
									  << ", tag " << utilities::transcode(cur->getNodeName()).get());
				if (cur->getNodeType() == xercesc::DOMNode::ELEMENT_NODE)
				{
					std::unique_ptr<elements::element>
						element{ element_manager.parse(dynamic_cast<xercesc::DOMElement*>(cur)) };
					LOG4CXX_TRACE(lg,
						"(" << file << ") Parsed as element, element type " << utilities::to_string(element->get_type())
							<< ", name " << element->get_name());
					if (element->get_type() != elements::element::element_type::command)
					{
						LOG4CXX_ERROR(lg,
							"Expecting a command, but got a " << utilities::to_string(element->get_type()) << ": " << element
								->get_name());
						throw std::exception{};
					}
					std::unique_ptr<elements::command> command{ dynamic_cast<elements::command*>(element.release()) };
					commands.push_back(std::move(command));
				}
			}
			LOG4CXX_TRACE(lg, "(" << file << ") Parsed " << commands.size() << " command(s)");
		}

		void execute(context& context, const element_manager& element_manager)
		{
			LOG4CXX_TRACE(lg, "(" << file << ") Executing " << commands.size() << " command(s)");
			for (std::unique_ptr<elements::command>& command : commands)
			{
				LOG4CXX_TRACE(lg, "(" << file << ") Executing command: " << command->get_name());
				command->execute(context, element_manager);
			}
			LOG4CXX_TRACE(lg, "(" << file << ") Executed command(s)");
		}

		~program()
		{
			LOG4CXX_TRACE(lg, "(" << file << ") Releasing program");
		}
	};
}
