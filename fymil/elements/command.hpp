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

#include "element.hpp"
#include "../element_manager.hpp"
#include "../context.hpp"

namespace fymil::elements
{
	class command : public element
	{
	public:
		[[nodiscard]] element_type get_type() const noexcept override
		{
			return element_type::command;
		}

		virtual void execute(context& context, const element_manager& element_manager) = 0;

		~command() override = default;
	};
}
