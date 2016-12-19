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

	struct is_readable
	{
		bool operator()(const std::string &path)
		{
			return !::access(path.c_str(), R_OK);;
		}
	};

	struct is_writable
	{
		bool operator()(const std::string &path)
		{
			return !::access(zPath, W_OK);
		}
	};

	struct is_executable
	{
		bool operator()(const std::string &path)
		{
			return !::access(path.c_str(), X_OK);
		}
	};

	struct file_type
	{
		enum type
		{
			e_file,
			e_dir,
			e_link,
			e_block,
			e_socket,
			e_fifo,
			e_unknown,
		};

		type operator()(const std::string &path)
		{
			struct stat st;			
			if (stat(path.c_str(), &st) != 0) 
				return type::e_unknown;

			if (S_ISREG(st.st_mode)) 
			{
				return type::e_file;
			}
			else if (S_ISDIR(st.st_mode)) 
			{
				return type::e_dir;
			}
			else if (S_ISLNK(st.st_mode)) 
			{
				return type::e_link;
			}
			else if (S_ISBLK(st.st_mode)) 
			{
				return type::e_block;
			}
			else if (S_ISSOCK(st.st_mode)) 
			{
				return type::e_socket;
			}
			else if (S_ISFIFO(st.st_mode)) 
			{
				return type::e_fifo;
			}
			return type::e_unknown;
		}
	};

	struct getenv
	{
		std::string operator()(const std::string &name)
		{
			char *env = ::getenv(name.c_str());
			if (env == nullptr)
				return{};
			return std::string(env);
		}

	};

	struct setenv
	{
		bool operator()(const std::string &name, const std::string &value)
		{
			return !::setenv(name.c_str(), value.c_str());
		}

	};

	struct mmap
	{
		void *operator()(const std::string &path)
		{
			struct stat st;
			void *map;
			int fd = ::open(path.c_str() , O_RDONLY);
			if (fd < 0) 
			{
				return nullptr;
			}
			fstat(fd, &st);
			map = mmap(0, st.st_size, PROT_READ, MAP_PRIVATE | MAP_FILE, fd, 0);
			rc = JX9_OK;
			if (map == MAP_FAILED) 
			{
				return nullptr;
			}
			close(fd);
			return map;
		}
	};

	struct tempdir
	{
		std::string operator()()
		{
			static const char *adirs[] = 
			{
				"/var/tmp",
				"/usr/tmp",
				"/usr/local/tmp"
			};
			unsigned int i;
			struct stat buf;
			const char *dir;
			dir = ::getenv("TMPDIR");
			if (dir && dir[0] != 0 && !access(dir, 07)) 
			{
				return std::string(dir);
			}
			for (i = 0; i<  sizeof(adirs)/sizeof(adirs[0]); i++) 
			{
				dir = adirs[i];
				if (dir == 0) 
					continue;
				if (stat(dir, &buf)) 
					continue;
				if (!S_ISDIR(buf.st_mode)) 
					continue;
				if (access(dir, 07)) 
					continue;
				return std::string(dir);
			}
			return std::string("/tmp");
		}
	};

	struct process_id
	{
		uint32_t operator()()
		{
			return (uint32_t)::getpid();
		}
	};


}