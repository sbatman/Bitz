#include "../Common.h"
#include "IO.h"

namespace Bitz
{
	namespace Content
	{
		std::string IO::_CachedWorkingDirectory;
		bool IO::_CachedWorkingDirectoryStored;
		std::vector<IO::FileHandle *> IO::_OpenFiles;

		std::string IO::GetWorkingDirectory(bool force)
		{
			if (_CachedWorkingDirectoryStored && !force)return _CachedWorkingDirectory;

		//	WCHAR buffer[MAX_PATH] = { 0 };
		//	LPWSTR exePath = buffer;

		//	HMODULE hModule = GetModuleHandle(nullptr);
		//	_ASSERT(hModule != NULL);
		//	GetModuleFileName(hModule, exePath, MAX_PATH * sizeof(WCHAR));

	//		std::string folder = std::string(exePath);
		//	size_t lastSlash = folder.find_last_of('\\');
			//folder = folder.substr(0, lastSlash);

		//	_CachedWorkingDirectory = folder;
		//	_CachedWorkingDirectoryStored = true;

	//		return folder;

			return "/";
		}

		IO::FileHandle * IO::OpenFile(std::string fileName, FileMode mode)
		{
			assert(fileName.length() != 0);
			FILE * fileHandle;
			char modeChar;
			switch (mode)
			{
			case Read:
				modeChar = 'r';
				break;
			case Write:
				modeChar = 'a';
				break;
			case Create:
				modeChar = 'w';
				break;
			default:
				throw std::invalid_argument("Invalid FileMode");
			}

#ifdef __ANDROID__
			fileHandle = fopen(fileName.c_str(), &modeChar);
#elif WIN32			
			fopen_s(&fileHandle, fileName.c_str(), &modeChar);
#endif	

			FileHandle * returnHandle = new FileHandle( fileName, fileHandle, mode );
			_OpenFiles.push_back(returnHandle);
			return returnHandle;
		}

		void IO::CloseAllOpen()
		{
			for (FileHandle * file : _OpenFiles)
			{
				fclose(file->Handle);
				file->Handle = nullptr;
				delete file;
			}
			_OpenFiles.clear();
		}

		void IO::CloseFile(FileHandle file)
		{
			assert(!file.IsDisposed());
			if (!file.IsDisposed())
			{
				fclose(file.Handle);
				file.Handle = nullptr;
			}
		}

		IO::FileHandle::FileHandle(std::string fileName, FILE * handle, FileMode mode)
		{
			FileName = fileName;
			Handle = handle;
			Mode = mode;
		}

		bool IO::FileHandle::IsDisposed()
		{
			return Handle == nullptr;
		}


		void IO::FileHandle::Close()
		{
			IO::CloseFile(*this);
		}
	}
}