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
