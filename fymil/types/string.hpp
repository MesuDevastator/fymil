#pragma once

#include <stdexcept>

#include "type.hpp"

namespace fymil::types
{
	class string final : public type
	{
	private:
		std::string str;
	public:
		explicit string(std::string str) noexcept
			: str{ std::move(str) }
		{
		}

		[[nodiscard]] data_type get_type() const noexcept override
		{
			return data_type::string;
		}

		[[nodiscard]] const std::string& as_string() const override
		{
			return str;
		}

		[[nodiscard]] bool as_boolean() const override
		{
			throw std::bad_cast{};
		}

		~string() override = default;
	};
}
