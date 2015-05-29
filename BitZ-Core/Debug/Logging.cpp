#include "../Common.h"
#include "Logging.h"

namespace Bitz
{
	namespace Debug
	{
		std::queue<Logging::LogEvent> * Logging::_LogsToProcess = nullptr;
		Bitz::Content::IO::FileHandle * Logging::_OutputFile = nullptr;
		const std::string Logging::ErrorTypeAsString[] = { "Notice", "Warning", "Error", "Critcal", "Debug" };

		bool Logging::_LogToConsole = false;
		bool Logging::_LogToFile = false;
		bool Logging::_LogToBlackHole = false;
		bool Logging::_Active = false;
		bool Logging::_LogDebug = false;

		std::thread Logging::_LogProcessingThread;
		std::mutex Logging::_LogsToProcessMutex;
		std::mutex Logging::_SettingsMutex;

		void Logging::Log(const ErrorType type, const std::string message)
		{
			if (type == ErrorType::Debug && !_LogDebug) return;
			assert(_Active && "Log cannot be called on the logger untill it is made active");
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

			assert(!_Active);
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
				_OutputFile->Close();
				_OutputFile = nullptr;
			}
		}

		void Logging::Update(bool active)
		{
			std::queue<LogEvent> * _LogsInProcessing = nullptr;

			assert(!_Active);
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
#ifdef WIN32
							printf("%s:\t%s\n", Logging::ErrorTypeAsString[(int)event.type].c_str(), event.message.c_str());
#elif __ANDROID__
							switch (event.type)
							{
							case Logging::ErrorType::Critcal:((void)__android_log_print(ANDROID_LOG_FATAL, "Bitz", event.message.c_str()));break;
							case Logging::ErrorType::Error:((void)__android_log_print(ANDROID_LOG_ERROR, "Bitz", event.message.c_str()));break;
							case Logging::ErrorType::Warning:((void)__android_log_print(ANDROID_LOG_WARN, "Bitz", event.message.c_str()));break;
							case Logging::ErrorType::Notice:((void)__android_log_print(ANDROID_LOG_INFO, "Bitz", event.message.c_str()));break;
							case Logging::ErrorType::Debug:((void)__android_log_print(ANDROID_LOG_DEBUG, "Bitz", event.message.c_str()));break;
							}
#endif
						}
						if (_LogToFile)
						{
							fprintf(_OutputFile->Handle, "%s:\t%s\n", Logging::ErrorTypeAsString[(int)event.type].c_str(), event.message.c_str());
							fflush(_OutputFile->Handle);
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

			assert(!_LogToConsole);
			_LogToConsole = true;
			return true;
		}
		bool Logging::EnableLogToFile(std::string fileName)
		{
			if (!_Active)Init();
			std::lock_guard<std::mutex> Lock(_SettingsMutex);

			assert(!_LogToFile);
			assert(_OutputFile == nullptr);
			_OutputFile = Bitz::Content::IO::OpenFile(fileName, Bitz::Content::IO::FileMode::Write);
			_LogToFile = !_OutputFile->IsDisposed();
			return _LogToFile;
		}
		bool Logging::EnableLogToBlackHole()
		{
			if (!_Active)Init();
			std::lock_guard<std::mutex> Lock(_SettingsMutex);

			assert(!_LogToBlackHole);
			_LogToBlackHole = true;
			return true;
		}

		void Logging::DisableLogToConsole()
		{
			std::lock_guard<std::mutex> Lock(_SettingsMutex);

			assert(_Active &&"DisableLogToConsole cannot be called on the logger untill it is made active");
			if (!_Active)return
				assert(_LogToConsole);
			_LogToConsole = false;
		}

		void Logging::DisableLogToFile()
		{
			std::lock_guard<std::mutex> Lock(_SettingsMutex);

			assert(_Active && "DisableLogToFile cannot be called on the logger untill it is made active");
			if (!_Active)return
				assert(_LogToFile);
			assert(_OutputFile != nullptr);
			_OutputFile->Close();
			_OutputFile = nullptr;
			_LogToFile = false;
		}

		void Logging::DisableLogToBlackHole()
		{
			std::lock_guard<std::mutex> Lock(_SettingsMutex);

			assert(_Active && "DisableLogToBlackHole cannot be called on the logger untill it is made active");
			if (!_Active)return
				assert(_LogToBlackHole);
			_LogToBlackHole = false;
		}
	}
}