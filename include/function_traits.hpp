#pragma once
namespace xutil
{
	template<typename>
	struct function_traits;

	template<typename Ret, typename ...Args>
	struct function_traits<Ret(Args...)>
	{
		typedef std::function<Ret(Args...)> stl_function_type;
	};

	template<typename Ret, typename ...Args>
	struct function_traits<Ret(*)(Args...)> :function_traits<Ret(Args...)> { };

	template<typename Ret, typename Class, typename ...Args>
	struct function_traits<Ret(Class::*)(Args...) const> :function_traits<Ret(Args...)> { };

	template<typename Ret, typename Class, typename ...Args>
	struct function_traits<Ret(Class::*)(Args...)> :function_traits<Ret(Args...)> { };

	template<typename Callable>
	struct function_traits :function_traits<decltype(&Callable::operator())> { };

	template<typename Ret, typename ...Args>
	struct function_traits<std::function<Ret(Args...)>> : function_traits<Ret(Args...)> { };

	template<typename FuncType>
	static inline
		typename xutil::function_traits<FuncType>::stl_function_type
		to_function(FuncType func)
	{
		return  typename xutil::function_traits<FuncType>::stl_function_type{ func };
	}
}