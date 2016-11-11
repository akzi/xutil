#pragma once
namespace xutil
{
	template<typename T>
	struct function_traits;
	
	template<typename Ret, typename ...Args>
	struct function_traits<Ret(Args...)>
	{
		typedef Ret return_type;
	};

	template<typename Ret, typename ...Args>
	struct function_traits<Ret(*)(Args...)> 
	{
		typedef Ret return_type;
	};
}