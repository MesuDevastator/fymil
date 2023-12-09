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
