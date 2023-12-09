#pragma once

#include <string>

namespace fymil::types
{
	class type
	{
	public:
		enum class data_type
		{
			number,
			string,
			boolean,
			undefined
		};

		// TODO: Add number type
		[[nodiscard]] virtual data_type get_type() const noexcept = 0;
		[[nodiscard]] virtual const std::string& as_string() const = 0;
		[[nodiscard]] virtual bool as_boolean() const = 0;
		virtual ~type() = default;
	};
}
