#include "../Common.h"
#include "IO.h"

namespace Bitz
{
	namespace Content
	{
		std::wstring IO::_CachedWorkingDirectory;
		bool IO::_CachedWorkingDirectoryStored;
		std::vector<IO::FileHandle> IO::_OpenFiles;

		std::wstring IO::GetWorkingDirectory(bool force)
		{
			if (_CachedWorkingDirectoryStored && !force)return _CachedWorkingDirectory;

		//	WCHAR buffer[MAX_PATH] = { 0 };
		//	LPWSTR exePath = buffer;

		//	HMODULE hModule = GetModuleHandle(nullptr);
		//	_ASSERT(hModule != NULL);
		//	GetModuleFileName(hModule, exePath, MAX_PATH * sizeof(WCHAR));

	//		std::wstring folder = std::wstring(exePath);
		//	size_t lastSlash = folder.find_last_of('\\');
			//folder = folder.substr(0, lastSlash);

		//	_CachedWorkingDirectory = folder;
		//	_CachedWorkingDirectoryStored = true;

	//		return folder;

			return L"d";
		}

		IO::FileHandle IO::OpenFile(std::wstring fileName, FileMode mode)
		{
			_ASSERT(fileName.length() != 0);
			FILE * fileHandle;
			switch (mode)
			{
			case Read:
				_wfopen_s(&fileHandle, fileName.c_str(), L"r");
				break;
			case Write:
				_wfopen_s(&fileHandle, fileName.c_str(), L"a");
				break;
			case Create:
				_wfopen_s(&fileHandle, fileName.c_str(), L"w");
				break;
			default:
				throw std::exception("Invalid FileMode");
			}
			FileHandle returnHandle = { fileName, fileHandle, mode };
			_OpenFiles.push_back(returnHandle);
			return returnHandle;
		}

		void IO::CloseAllOpen()
		{
			for (FileHandle file : _OpenFiles)
			{
				fclose(file.Handel);
				file.Handel = nullptr;
			}
			_OpenFiles.clear();
		}

		void IO::CloseFile(FileHandle file)
		{
			_ASSERT(!file.IsDisposed());
			if (!file.IsDisposed())
			{
				fclose(file.Handel);
				file.Handel = nullptr;
			}
		}

		bool IO::FileHandle::IsDisposed()
		{
			return Handel == nullptr;
		}
	}
}