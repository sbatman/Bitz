#include "../Common.h"
#include "Logging.h"

namespace Bitz
{
	namespace Debug
	{
		std::queue<Logging::LogEvent> * Logging::_LogsToProcess = nullptr;
		FILE * Logging::_OutputFile = nullptr;
		const std::string Logging::ErrorTypeAsString[] = { "Notice", "Warning", "Error", "Critcal", "Debug" };

		bool Logging::_LogToConsole = false;
		bool Logging::_LogToFile = false;
		bool Logging::_LogToBlackHole = false;
		bool Logging::_Active = false;
		bool Logging::_LogDebug = false;

		std::thread Logging::_LogProcessingThread;
		std::mutex Logging::_LogsToProcessMutex;
		std::mutex Logging::_SettingsMutex;

		static void Update();

		void Logging::Log(const ErrorType type, const std::wstring message)
		{
			if (type == ErrorType::Debug && !_LogDebug) return;
			_ASSERT_EXPR(_Active, L"Log cannot be called on the logger untill it is made active");
			if (!_Active)return;
			LogEvent logEvent;
			logEvent.type = type;
			logEvent.message = message;

			_LogsToProcessMutex.lock();

			_LogsToProcess->push(logEvent);

			_LogsToProcessMutex.unlock();
		}

		void Logging::SetDebugLoggingActive(bool state)
		{
			_LogDebug = state;
		}

		bool Logging::HasInit()
		{
			return _Active;
		}

		void Logging::Init()
		{
			std::lock_guard<std::mutex> Lock(_SettingsMutex);

			_ASSERT(!_Active);
			_OutputFile = nullptr;
			_LogToConsole = false;
			_LogToFile = false;
			_LogToBlackHole = false;
			_Active = false;
			if (_LogsToProcess != nullptr)
			{
				delete _LogsToProcess;
				_LogsToProcess = nullptr;
			}
			_LogsToProcess = new std::queue<LogEvent>();

			_LogProcessingThread = std::thread(&Logging::Update, true);

		}

		void Logging::StaticDispose()
		{
			_Active = false;
			_LogProcessingThread.join();
			if (_LogsToProcess != nullptr)
			{
				delete _LogsToProcess;
				_LogsToProcess = nullptr;
			}
			if (_LogToFile)
			{
				fclose(_OutputFile);
				_OutputFile = nullptr;
			}
		}

		void Logging::Update(bool active)
		{
			std::queue<LogEvent> * _LogsInProcessing = nullptr;

			_ASSERT(!_Active);
			_Active = active;
			while (_Active)
			{
				if (!_LogToConsole && !_LogToFile && !_LogToBlackHole)
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(3));
					continue;
				}
				_LogsToProcessMutex.lock();
				if (_LogsToProcess->size() > 0)
				{
					_LogsInProcessing = _LogsToProcess;
					_LogsToProcess = new std::queue<LogEvent>();
				}
				_LogsToProcessMutex.unlock();

				if (_LogsInProcessing != nullptr)
				{
					while (!_LogsInProcessing->empty())
					{
						LogEvent event = _LogsInProcessing->front();
						_LogsInProcessing->pop();

						if (_LogToConsole)
						{
							printf("%s:\t%S\n", Logging::ErrorTypeAsString[(int)event.type].c_str(), event.message.c_str());
						}
						if (_LogToFile)
						{
							fprintf(_OutputFile, "%s:\t%S\n", Logging::ErrorTypeAsString[(int)event.type].c_str(), event.message.c_str());
							fflush(_OutputFile);
						}
						if (_LogToBlackHole)
						{

						}
					}
					delete _LogsInProcessing;
					_LogsInProcessing = nullptr;
					std::this_thread::sleep_for(std::chrono::milliseconds(0));
				}
				else {
					std::this_thread::sleep_for(std::chrono::milliseconds(5));
				}

			}
		}

		bool Logging::EnableLogToConsole()
		{
			if (!_Active)Init();
			std::lock_guard<std::mutex> Lock(_SettingsMutex);

			_ASSERT(!_LogToConsole);
			_LogToConsole = true;
			return true;
		}
		bool Logging::EnableLogToFile(std::string fileName)
		{
			if (!_Active)Init();
			std::lock_guard<std::mutex> Lock(_SettingsMutex);

			_ASSERT(!_LogToFile);
			_ASSERT(_OutputFile == nullptr);
			errno_t error = fopen_s(&_OutputFile, fileName.c_str(), "w");
			_LogToFile = error == 0;
			return _LogToFile;
		}
		bool Logging::EnableLogToBlackHole()
		{
			if (!_Active)Init();
			std::lock_guard<std::mutex> Lock(_SettingsMutex);

			_ASSERT(!_LogToBlackHole);
			_LogToBlackHole = true;
			return true;
		}

		void Logging::DisableLogToConsole()
		{
			std::lock_guard<std::mutex> Lock(_SettingsMutex);

			_ASSERT_EXPR(_Active, L"DisableLogToConsole cannot be called on the logger untill it is made active");
			if (!_Active)return
				_ASSERT(_LogToConsole);
			_LogToConsole = false;
		}

		void Logging::DisableLogToFile()
		{
			std::lock_guard<std::mutex> Lock(_SettingsMutex);

			_ASSERT_EXPR(_Active, L"DisableLogToFile cannot be called on the logger untill it is made active");
			if (!_Active)return
				_ASSERT(_LogToFile);
			_ASSERT(_OutputFile != nullptr);
			fclose(_OutputFile);
			_LogToFile = false;
		}

		void Logging::DisableLogToBlackHole()
		{
			std::lock_guard<std::mutex> Lock(_SettingsMutex);

			_ASSERT_EXPR(_Active, L"DisableLogToBlackHole cannot be called on the logger untill it is made active");
			if (!_Active)return
				_ASSERT(_LogToBlackHole);
			_LogToBlackHole = false;
		}
	}
}