#pragma once

#include <array>
#include <algorithm>
#include <string>
#include <xercesc/dom/DOM.hpp>

#include "../elements/element.hpp"

namespace fymil::utilities
{
	[[nodiscard]] const char* to_string(xercesc::DOMNode::NodeType node_type)
	{
		static std::array<const char*, 12> table{
			{
				"ELEMENT",
				"ATTRIBUTE",
				"TEXT",
				"CDATA SECTION",
				"ENTITY REFERENCE",
				"ENTITY",
				"PROCESSING INSTRUCTION",
				"COMMENT",
				"DOCUMENT",
				"DOCUMENT TYPE",
				"DOCUMENT FRAGMENT",
				"NOTATION NODE"
			}
		};
		return table.at(node_type - 1);
	}

	[[nodiscard]] const char* to_string(fymil::elements::element::element_type element_type)
	{
		static std::array<const char*, 2> table{
			{
				"COMMAND",
				"PARSER"
			}
		};
		return table.at(static_cast<int>(element_type));
	}

	void trim(std::string& str)
	{
		if (str.empty())
		{
			return;
		}
		str.erase(0, str.find_first_not_of(" \r\n\v\f\r"));
		str.erase(str.find_last_not_of(" \r\n\v\f\r") + 1);
	}
}
