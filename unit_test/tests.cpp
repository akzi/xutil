#include "../include/function_traits.hpp"
#include "../include/xtest.hpp"

xtest_run;

XTEST_SUITE(function_traits)
{
	using namespace xutil;
	template<typename Func>
	void call_func(Func func)
	{
		typename function_traits<Func>::return_type a = 1;
	}
	int func1()
	{
		return 1;
	}
	XUNIT_TEST(1)
	{
		call_func(func1);
	}
}