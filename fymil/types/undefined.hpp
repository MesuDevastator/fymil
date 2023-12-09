#pragma once

#include <stdexcept>

#include "type.hpp"

namespace fymil::types
{
	class undefined final : public type
	{
	public:
		explicit undefined() noexcept = default;

		[[nodiscard]] data_type get_type() const noexcept override
		{
			return data_type::undefined;
		}

		[[nodiscard]] const std::string& as_string() const override
		{
			static const std::string undefined_string{ "<undefined>" };
			return undefined_string;
		}

		[[nodiscard]] bool as_boolean() const override
		{
			return false;
		}

		~undefined() override = default;
	};
}
