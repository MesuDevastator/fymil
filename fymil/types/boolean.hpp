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
