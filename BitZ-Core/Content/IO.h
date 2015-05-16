#pragma once
#include "../Common.h"

namespace Bitz
{
	class Core;

	namespace Content
	{
		class IO
		{
			friend class Core;

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
				std::wstring FileName;
				FILE * Handel;
				FileMode Mode;
				bool IsDisposed();
			};

			/// <summary>
			/// Returns the current working directory of the engine (usualy the exe location)
			/// </summary>
			/// <param name='force'>If true the cached location is ignored and its re-computed</param>
			static std::wstring GetWorkingDirectory(bool force = false);

			static FileHandle OpenFile(std::wstring fileName, FileMode mode);
			static void CloseFile(FileHandle file);

		protected:
			static void CloseAllOpen();

		private:
			/// <summary>
			/// A cached copy of the last return by GetWorkingDirectory
			/// </summary>
			static std::wstring _CachedWorkingDirectory;
			/// <summary>
			/// Whether _CachedWorkingDirectory has been populated
			/// </summary>
			static bool _CachedWorkingDirectoryStored;

			static std::vector<FileHandle> _OpenFiles;
		};
	}
}
