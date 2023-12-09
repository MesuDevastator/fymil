#pragma once

#include <memory>

#include "element.hpp"
#include "../element_manager.hpp"
#include "../context.hpp"
#include "../types/type.hpp"

namespace fymil::elements
{
	class parser : public element
	{
	public:
		[[nodiscard]] element_type get_type() const noexcept override
		{
			return element_type::parser;
		}

		virtual std::unique_ptr<types::type> parse(context& context, const element_manager& element_manager) = 0;

		~parser() override = default;
	};
}
