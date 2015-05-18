#pragma once
#include "../Common.h"
#include "../Content/IO.h"

namespace Bitz
{
	class Core;

	namespace Debug
	{
		class Logging
		{
			friend Bitz::Core;
		public:
			enum ErrorType { Notice, Warning, Error, Critcal, Debug };
			const static std::string ErrorTypeAsString[];
			static void Log(const ErrorType type, const std::string message);

			static bool EnableLogToConsole();
			static bool EnableLogToFile(std::string fileName);
			static bool EnableLogToBlackHole();
			static void DisableLogToConsole();
			static void DisableLogToFile();
			static void DisableLogToBlackHole();
			static void SetDebugLoggingActive(bool state);

		protected:
			static void Init();
			static void StaticDispose();
			static bool HasInit();
		private:

			struct LogEvent
			{
				ErrorType type;
				std::string message;
			};

			static bool _LogToConsole;
			static bool _LogToFile;
			static bool _LogToBlackHole;
			static bool _Active;
			static bool _LogDebug;

			static std::queue<LogEvent> * _LogsToProcess;
			static std::thread _LogProcessingThread;
			static std::mutex _LogsToProcessMutex;
			static std::mutex _SettingsMutex;

			static Bitz::Content::IO::FileHandle * _OutputFile;

			static void Update(bool active);
		};
	}
}