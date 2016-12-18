#pragma once
#include <windows.h>
#include <string>
namespace xutil
{
	namespace vfs
	{
		struct username
		{
			std::string operator()()
			{
				char zUser[1024];
				DWORD nByte;
				BOOL rc;
				nByte = sizeof(zUser);
				if (!GetUserNameA(zUser, &nByte))
					return{};
				return std::string(zUser, nByte);
			}
		};
		struct process_id
		{
			std::size_t operator()()
			{
				return GetProcessId(GetCurrentProcess());
			}
		};

		struct temp_dir
		{
			std::string operator()()
			{
				char buffer[1024];
				DWORD n = GetTempPathA(sizeof(buffer), buffer);
				if (n < 1)
					return "C:\\Windows\\Temp";
				return std::string(buffer, n);
			}
		};
		struct open_read_only
		{
			HANDLE operator ()(const std::string &path)
			{
				DWORD dwType = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS;
				DWORD dwShare = FILE_SHARE_READ | FILE_SHARE_WRITE;
				DWORD dwAccess = GENERIC_READ;
				DWORD dwCreate = OPEN_EXISTING;
				HANDLE pHandle;
				pHandle = CreateFileA(path.c_str(), dwAccess, dwShare, 0, dwCreate, dwType, 0);
				if (pHandle == INVALID_HANDLE_VALUE)
					return nullptr;
				return pHandle;
			}

		};
		struct chdir
		{
			bool operator()(const std::string &path)
			{
				return !!SetCurrentDirectoryA(path.c_str());
			}
		};
		struct getcwd
		{
			std::string operator()()
			{
				char buffer[2048] = { 0 };
				GetCurrentDirectoryA(sizeof(buffer), buffer);
				return std::string(buffer);
			}
		};
		struct mkdir
		{
			bool operator()(const std::string &path)
			{
				return CreateDirectoryA(path.c_str(), 0);
			}
		};
		struct rmdir
		{
			bool operator()(const std::string &path)
			{
				return !!RemoveDirectoryA(path.c_str());
			}
		};
		struct isdir
		{
			bool operator()(const std::string &path)
			{
				DWORD dwAttr;
				dwAttr = GetFileAttributesA(path.c_str());
				if (dwAttr == INVALID_FILE_ATTRIBUTES)
					return false;
				return (dwAttr & FILE_ATTRIBUTE_DIRECTORY);
			}
		};
		struct rename
		{
			bool operator()(const std::string &_old, const std::string &_new)
			{
				return !!MoveFileA(_old.c_str(), _new.c_str());
			}
		};
		struct real_path
		{
			std::string operator()(const std::string &path)
			{
				DWORD len;
				std::string buffer;
				len = GetFullPathNameA(path.c_str(), 0, 0, 0);
				if (len > 0)
				{
					buffer.resize(len);
					GetFullPathNameA(path.c_str(), len, (char*)buffer.data(), 0);
				}
				return std::move(buffer);
			}
		};
		struct  unlink
		{
			bool operator()(const std::string &path)
			{
				return !!DeleteFileA(path.c_str());
			}
		};
		struct disk_free_space
		{
			int64_t operator()(const std::string &path)
			{
				DWORD dwSectPerClust,
					dwBytesPerSect,
					dwFreeClusters,
					dwTotalClusters;
				BOOL rc;
				std::string buffer;
				auto pos = path.find_first_of('\\');
				if (pos == std::string::npos)
					pos = path.find_first_of('/');
				if (pos == std::string::npos)
					buffer = path;
				else
					buffer = path.substr(0, pos);
				rc = GetDiskFreeSpaceA(buffer.c_str(),
					&dwSectPerClust,
					&dwBytesPerSect,
					&dwFreeClusters,
					&dwTotalClusters);
				if (!rc)
					return 0;
				return
					(int64_t)dwFreeClusters *
					dwSectPerClust *
					dwBytesPerSect;
			}
		};
		struct disk_total_space
		{
			int64_t operator()(const std::string &path)
			{
				DWORD dwSectPerClust,
					dwBytesPerSect,
					dwFreeClusters,
					dwTotalClusters;
				BOOL rc;
				std::string buffer;
				auto pos = path.find_first_of('\\');
				if (pos == std::string::npos)
					pos = path.find_first_of('/');
				if (pos == std::string::npos)
					buffer = path;
				else
					buffer = path.substr(0, pos);
				if (!GetDiskFreeSpaceA(buffer.c_str(),
					&dwSectPerClust,
					&dwBytesPerSect,
					&dwFreeClusters,
					&dwTotalClusters))
					return 0;
				return
					(int64_t)dwTotalClusters *
					dwSectPerClust *
					dwBytesPerSect;
			}

		};
		struct file_exists
		{
			bool operator()(const std::string &path)
			{
				DWORD dwAttr;
				dwAttr = GetFileAttributesA(path.c_str());
				if (dwAttr == INVALID_FILE_ATTRIBUTES)
					return false;
				return true;
			}

		};
		struct mmap
		{
			void *operator ()(const std::string &path, int64_t &pSize)
			{
				DWORD size_low, size_high;
				HANDLE file_handle, map_handle;
				void *map;

				file_handle = open_read_only()(path);
				if (!file_handle)
					return nullptr;
				size_low = GetFileSize(file_handle, &size_high);
				map_handle = CreateFileMappingA(file_handle,
					0, PAGE_READONLY, size_high, size_low, 0);
				if (map_handle == 0)
				{
					CloseHandle(file_handle);
					return nullptr;
				}
				pSize = ((int64_t)size_high << 32) | size_low;
				map = MapViewOfFile(map_handle, 
					FILE_MAP_READ, 0, 0, (SIZE_T)(pSize));
				CloseHandle(map_handle);
				CloseHandle(file_handle);
				return map;
			}

		};
		struct unmap
		{
			void operator()(void *handle, int64_t size = 0)
			{
				UnmapViewOfFile(handle);
			}
		};
	}
}