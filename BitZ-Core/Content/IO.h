#pragma once
#include "../Common.h"

namespace Bitz
{
	class Core;

	namespace Content
	{
		class IO
		{
			friend class Bitz::Core;

		public:
			enum FileMode
			{
				Read,
				Write,
				Create
			};

			class FileHandle
			{
			public:
				FileHandle(std::string fileName, FILE * handle, FileMode mode);
				std::string FileName;
				FILE * Handle;
				FileMode Mode;
				bool IsDisposed();
				void Close();
			};

			/// <summary>
			/// Returns the current working directory of the engine (usualy the exe location)
			/// </summary>
			/// <param name='force'>If true the cached location is ignored and its re-computed</param>
			static std::string GetWorkingDirectory(bool force = false);

			static FileHandle * OpenFile(std::string fileName, FileMode mode);
			static void CloseFile(FileHandle file);

		protected:
			static void CloseAllOpen();

		private:
			/// <summary>
			/// A cached copy of the last return by GetWorkingDirectory
			/// </summary>
			static std::string _CachedWorkingDirectory;
			/// <summary>
			/// Whether _CachedWorkingDirectory has been populated
			/// </summary>
			static bool _CachedWorkingDirectoryStored;

			static std::vector<FileHandle *> _OpenFiles;
		};
	}
}
