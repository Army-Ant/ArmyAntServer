#include <Logger.h>

#include <ctime>

#include <AAClassPrivateHandle.hpp>
#include <AAString.h>
#include <AAIStream_File.h>

namespace ArmyAntServer {

	class Logger_Private {
	public:
		Logger_Private() {}
		~Logger_Private() {}

		static ArmyAnt::String getTimeStamp(__time64_t time);
		static ArmyAnt::String getWholeContent(const char * content, Logger::AlertLevel level, const char * tag);

		Logger::AlertLevel consoleLevel = Logger::AlertLevel::Import;
		ArmyAnt::File logFile;
		Logger::AlertLevel fileLevel = Logger::AlertLevel::Verbose;
		std::ostream* userStream = nullptr;
		Logger::AlertLevel userStreamLevel = Logger::AlertLevel::Debug;
	};

	static ArmyAnt::ClassPrivateHandleManager<Logger, Logger_Private> s_handleManager;

	ArmyAnt::String Logger_Private::getTimeStamp(__time64_t time) {
		return _ctime64(&time);
	}

	ArmyAnt::String Logger_Private::getWholeContent(const char * content, Logger::AlertLevel level, const char * tag) {
		ArmyAnt::String timeString = "[ " + Logger_Private::getTimeStamp(_time64(0)) + " ] ";
		ArmyAnt::String tagString = "[ " + ArmyAnt::String(tag) + " ] ";
		ArmyAnt::String wholeContent = timeString + tagString + "[ " + Logger::convertLevelToString(level) + " ] " + content;
		return wholeContent;
	}

	const char * Logger::convertLevelToString(AlertLevel level)
	{
		switch (level) {
		case AlertLevel::Verbose:
			return "Verbose";
		case AlertLevel::Debug:
			return "Debug";
		case AlertLevel::Info:
			return "Info";
		case AlertLevel::Import:
			return "Import";
		case AlertLevel::Warning:
			return "Warning";
		case AlertLevel::Error:
			return "Error";
		case AlertLevel::Exception:
			return "Exception";
		}
	}

	Logger::Logger(const char* logFilePath) {
		s_handleManager.GetHandle(this);
		setLogFile(logFilePath);
	}

	Logger::~Logger(){
		s_handleManager.ReleaseHandle(this);
	}

	void Logger::setConsoleLevel(Logger::AlertLevel level) {
		s_handleManager[this]->consoleLevel = level;
	}

	Logger::AlertLevel Logger::getConsoleLevel()const {
		return s_handleManager[this]->consoleLevel;
	}

	bool Logger::setLogFile(const char* path) {
		s_handleManager[this]->logFile.Close();
		if (path == nullptr)
			return s_handleManager[this]->logFile.GetSourceName() == path;
		return s_handleManager[this]->logFile.Open(path);
	}
	const char*Logger::getLogFilePath()const {
		return s_handleManager[this]->logFile.GetSourceName();
	}

	void Logger::setFileLevel(Logger::AlertLevel level) {
		s_handleManager[this]->fileLevel = level;
	}

	Logger::AlertLevel Logger::getFileLevel()const {
		return s_handleManager[this]->fileLevel;
	}

	void Logger::setUserStream(std::ostream* stream) {
		s_handleManager[this]->userStream = stream;
	}

	std::ostream*Logger::getUserStream() {
		return s_handleManager[this]->userStream;
	}

	void Logger::setUserStreamLevel(Logger::AlertLevel level) {
		s_handleManager[this]->userStreamLevel = level;
	}

	Logger::AlertLevel Logger::getUserStreamLevel()const {
		return s_handleManager[this]->userStreamLevel;
	}

	bool Logger::pushLog(const char * content, Logger::AlertLevel level, const char * tag)
	{
		ArmyAnt::String wholeContent = Logger_Private::getWholeContent(content, level, tag);
		bool ret = true;
		if (level >= s_handleManager[this]->consoleLevel) {
			ret = pushLogToConsole(wholeContent.c_str());
		}
		if (level >= s_handleManager[this]->fileLevel) {
			ret = ret && pushLogToFile(wholeContent.c_str());
		}
		if (level >= s_handleManager[this]->userStreamLevel) {
			ret = ret && pushLogToUserStream(wholeContent.c_str());
		}
		return ret;
	}

	bool Logger::pushLogOnlyInConsole(const char * content, AlertLevel level, const char * tag)
	{
		ArmyAnt::String wholeContent = Logger_Private::getWholeContent(content, level, tag);
		bool ret = true;
		if (level >= s_handleManager[this]->consoleLevel) {
			ret = pushLogToConsole(wholeContent.c_str());
		}
		return ret;
	}

	bool Logger::pushLogOnlyInFile(const char * content, AlertLevel level, const char * tag)
	{
		ArmyAnt::String wholeContent = Logger_Private::getWholeContent(content, level, tag);
		bool ret = true;
		if (level >= s_handleManager[this]->fileLevel) {
			ret = ret && pushLogToFile(wholeContent.c_str());
		}
		return ret;
	}

	bool Logger::pushLogToConsole(const char * wholeContent)
	{
		std::cout << wholeContent << std::endl;
		return true;
	}

	bool Logger::pushLogToFile(const char * wholeContent)
	{
		if (!s_handleManager[this]->logFile.IsOpened())
			return true;
		bool ret = s_handleManager[this]->logFile.Write(wholeContent);
		if (ret == 0) {
			return false;
		}
		ret = s_handleManager[this]->logFile.Write("\n");
		if (ret == 0) {
			return false;
		}
		return true;
	}

	bool Logger::pushLogToUserStream(const char * wholeContent)
	{
		if (s_handleManager[this]->userStream == nullptr)
			return true;
		if (s_handleManager[this]->userStream->bad())
			return false;
		*(s_handleManager[this]->userStream) << wholeContent << "\n";
		return true;
	}

} // namespace ArmyAntServer 