#include "../../xtest/include/xtest.hpp"
#include "../include/vfs.hpp"
xtest_run;


XTEST_SUITE(vfs)
{
	XUNIT_TEST(file_type)
	{
		auto type = xutil::vfs::file_type()("F:\\vpn.txt");
	}
	XUNIT_TEST(ls_files)
	{
		auto current_dir = xutil::vfs::getcwd()();
		auto files = xutil::vfs::ls_files()("/home/skyinno/akzi/" ,3);

		for (auto &itr : files)
			std::cout << itr << std::endl;
	}
}
