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
