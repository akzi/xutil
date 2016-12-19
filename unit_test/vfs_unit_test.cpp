#include "../../../xtest/include/xtest.hpp"
#include "../include/vfs.hpp"
xtest_run;

using namespace xutil;

XTEST_SUITE(vfs)
{
	XUNIT_TEST(file_type)
	{
		auto type = vfs::file_type()("F:\\vpn.txt");
	}
	XUNIT_TEST(ls_files)
	{
		auto current_dir = vfs::getcwd()();
		auto files = xutil::vfs::ls_files()(current_dir ,3);

		for (auto &itr : files)
			std::cout << itr << std::endl;
	}
}