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

#include <string>

namespace fymil::elements
{
	class element
	{
	public:
		enum class element_type
		{
			command,
			parser
		};

		[[nodiscard]] virtual element_type get_type() const noexcept = 0;
		[[nodiscard]] virtual const std::string& get_name() const noexcept = 0;
		virtual ~element() = default;
	};
}
