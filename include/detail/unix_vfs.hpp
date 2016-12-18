#pragma once
#include <string>
#include <sys/types.h>
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/uio.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/file.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>
#include <utime.h>
#include <stdio.h>
#include <stdlib.h>
namespace xutil
{
	struct chdir
	{
		bool operator()(const std::string &path)
		{
			return !::chdir(path.c_str());
		}
	};
	struct getcwd 
	{
		std::string operator()()
		{
			char buffer[4096];
			char *result;
			result = ::getcwd(buffer, sizeof(buffer));
			if (result == 0)
				return{};
			return std::string(result);
		}
	};
	struct mkdir
	{
		bool operator()(const std::string &path, int mode = 0777)
		{
			return !::mkdir(path.c_str(), mode);
		}
	};
	struct rmdir
	{
		bool operator()(const std::string &path)
		{
			return !::rmdir(path.c_str());
		}
	};
	struct isdir 
	{
		bool operator()(const std::string &path)
		{
			struct stat st;
			if (::stat(path.c_str(), &st)!= 0)
				return false;
			return !!S_ISDIR(st.st_mode);
		}
	};
	struct unlink
	{
		bool operator()(const std::string &path)
		{
			return !::unlink(path.c_str());
		}
	};
	struct file_exists
	{
		bool operator()(const std::string &path)
		{
			return !::access(path.c_str(), F_OK);
		}
	};
	struct file_size
	{
		int64_t operator()(const std::string &path)
		{
			struct stat st;
			if (::stat(path.c_str(), &st) != 0)
				return -1;
			return (int64_t)st.st_size;
		}
	};
	struct realpath 
	{
		std::string operator()(const std::string &path)
		{
			char *real = ::realpath(path.c_str(), 0);
			if (real == nullptr)
				return{};
			std::string result(real);
			free(real);
			return result;
		}
	};
	struct rename 
	{
		bool operator()(const std::string &old_name, const std::string &new_name)
		{
			return !::rename(old_name.c_str(), new_name.c_str());
		}
	};
	struct chmod 
	{
		bool operator()(const std::string &path, int mode)
		{
			return !::chmod(path.c_str(), mode);
		}
	};

	struct chown 
	{
		bool operator()(const std::string &path, const std::string &user)
		{
			struct passwd *pwd = getpwnam(user.c_str());
			if (pwd == 0)
				return false;
			return !::chown(path.c_str(), pwd->pw_uid, -1);
		}
	};

	struct is_file 
	{
		bool operator()(const std::string &path)
		{
			struct stat st;
			if (::stat(path.c_str(), &st) != 0)
				return false;
			return !!S_ISREG(st.st_mode);
		}
	};

	struct inlink 
	{
		bool operator()(const std::string &path)
		{
			struct stat st;
			if (stat(path.c_str(), &st) != 0)
				return false;
			return !!S_ISLNK(st.st_mode);
		}
	};
}