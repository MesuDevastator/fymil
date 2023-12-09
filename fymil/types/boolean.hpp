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

#include "type.hpp"

namespace fymil::types
{
	class boolean final : public type
	{
	private:
		bool value;
	public:
		explicit boolean(bool value) noexcept
			: value{ value }
		{
		}

		[[nodiscard]] data_type get_type() const noexcept override
		{
			return data_type::boolean;
		}

		[[nodiscard]] const std::string& as_string() const override
		{
			static const std::string false_string{ "false" };
			static const std::string true_string{ "true" };
			return value ?
				true_string :
				false_string;
		}

		[[nodiscard]] bool as_boolean() const override
		{
			return value;
		}

		~boolean() override = default;
	};
}
