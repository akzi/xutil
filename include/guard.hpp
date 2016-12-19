#pragma once
#include <functional>
namespace xutil
{
	struct guard
	{
		guard(std::function<void()> &&func)
			:func_(std::move(func))
		{
			
		}
		~guard()
		{
			if(func_)
				func_();
		}
		std::function<void()> func_;
	};
}