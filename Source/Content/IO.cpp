#ifdef __ANDROID__
#include "../BitzAndroid/android_native_app_glue.h"
#endif
#include "../Common.h"
#include "IO.h"

namespace Bitz
{
	namespace Content
	{
		std::string IO::_CachedWorkingDirectory;
		bool IO::_CachedWorkingDirectoryStored;
		std::vector<IO::FileHandle_Ptr> IO::_OpenFiles;

		std::string IO::GetWorkingDirectory(bool force)
		{
			if (_CachedWorkingDirectoryStored && !force)return _CachedWorkingDirectory;

#ifdef __ANDROID__
#elif WIN32
#endif
			return "/";
		}

		IO::FileHandle_Ptr IO::OpenFile(std::string fileName, FileMode mode)
		{
			assert(fileName.length() != 0);
			FILE * fileHandle;
			std::string modeChar;
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
			fileHandle = fopen(fileName.c_str(), modeChar.c_str());
#elif WIN32
			fopen_s(&fileHandle, fileName.c_str(), modeChar.c_str());
#endif

			FileHandle_Ptr returnHandle = FileHandle_Ptr(new FileHandle(fileName, std::unique_ptr<FILE>(fileHandle), mode));
			_OpenFiles.push_back(returnHandle);
			return returnHandle;
		}

		void IO::CloseAllOpen()
		{
			for (FileHandle_Ptr file : _OpenFiles)
			{
				fclose(file->Handle.get());
				file->Handle = nullptr;
				file = nullptr;
			}
			_OpenFiles.clear();
		}

		void IO::CloseFile(FileHandle_Ptr file)
		{
			file->Close();
		}

		std::vector<byte> IO::ReadAllBytes(std::string filename)
		{
#ifdef __ANDROID__
			AAssetManager* assetManager = android_app_GetAssetManager();
			AAsset* asset = AAssetManager_open(assetManager, filename.c_str(), AASSET_MODE_BUFFER);
			int32_t length = AAsset_getLength(asset);
			const void * ptr = AAsset_getBuffer(asset);
			std::vector<byte> result(length);
			Memcpy(&result[0], length, ptr, length);
			AAsset_close(asset);
#elif WIN32
			std::ifstream ifs(filename, std::ios::binary | std::ios::ate);
			std::ifstream::pos_type pos = ifs.tellg();

			std::vector<byte>  result(static_cast<uint32_t>(pos));

			ifs.seekg(0, std::ios::beg);
			ifs.read(reinterpret_cast<char *>(&result[0]), pos);
			ifs.close();
#endif
			return result;
		}

		std::vector<char> IO::ReadAllChars(std::string filename)
		{
#ifdef __ANDROID__
			AAssetManager* assetManager = android_app_GetAssetManager();
			AAsset* asset = AAssetManager_open(assetManager, filename.c_str(), AASSET_MODE_BUFFER);
			int32_t length = AAsset_getLength(asset);
			const void * ptr = AAsset_getBuffer(asset);
			std::vector<char> result(length);
			Memcpy(&result[0], length, ptr, length);
			AAsset_close(asset);
#elif WIN32
			std::ifstream ifs(filename, std::ios::binary | std::ios::ate);
			std::ifstream::pos_type pos = ifs.tellg();

			std::vector<char>  result(static_cast<uint32_t>(pos));

			ifs.seekg(0, std::ios::beg);
			ifs.read(reinterpret_cast<char *>(&result[0]), pos);
			ifs.close();
#endif
			return result;
		}

		IO::FileHandle::FileHandle(std::string fileName, std::unique_ptr<FILE> handle, FileMode mode)
		{
			FileName = fileName;
			Handle = std::move(handle);
			Mode = mode;
		}

		bool IO::FileHandle::IsDisposed()
		{
			return Handle == nullptr;
		}

		void IO::FileHandle::Close()
		{
			assert(!IsDisposed());
			if (!IsDisposed())
			{
				fclose(Handle.get());
				Handle = nullptr;
			}
		}
	}
}