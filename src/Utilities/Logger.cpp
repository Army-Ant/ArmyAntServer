#include <Logger.h>

#include <ctime>

#include <AAClassPrivateHandle.hpp>
#include <AAString.h>
#include <AAIStream_File.h>


#define AA_HANDLE_MANAGER ArmyAnt::ClassPrivateHandleManager<Logger, Logger_Private>::getInstance()

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

	ArmyAnt::String Logger_Private::getTimeStamp(__time64_t time) {
#if defined OS_WINDOWS
		char ret[64] = "";
		_ctime64_s(ret,&time);
		return ret;
#else
		return _ctime64(&time);
#endif
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
		case AlertLevel::Fatal:
			return "Fatal";
		}
		return "Unknown";
	}

	Logger::Logger(const char* logFilePath) {
		AA_HANDLE_MANAGER.GetHandle(this);
		setLogFile(logFilePath);
	}

	Logger::~Logger(){
		AA_HANDLE_MANAGER.ReleaseHandle(this);
	}

	void Logger::setConsoleLevel(Logger::AlertLevel level) {
		AA_HANDLE_MANAGER[this]->consoleLevel = level;
	}

	Logger::AlertLevel Logger::getConsoleLevel()const {
		return AA_HANDLE_MANAGER[this]->consoleLevel;
	}

	bool Logger::setLogFile(const char* path) {
		AA_HANDLE_MANAGER[this]->logFile.Close();
		if (path == nullptr)
			return AA_HANDLE_MANAGER[this]->logFile.GetSourceName() == path;
		return AA_HANDLE_MANAGER[this]->logFile.Open(path);
	}
	const char*Logger::getLogFilePath()const {
		return AA_HANDLE_MANAGER[this]->logFile.GetSourceName();
	}

	void Logger::setFileLevel(Logger::AlertLevel level) {
		AA_HANDLE_MANAGER[this]->fileLevel = level;
	}

	Logger::AlertLevel Logger::getFileLevel()const {
		return AA_HANDLE_MANAGER[this]->fileLevel;
	}

	void Logger::setUserStream(std::ostream* stream) {
		AA_HANDLE_MANAGER[this]->userStream = stream;
	}

	std::ostream*Logger::getUserStream() {
		return AA_HANDLE_MANAGER[this]->userStream;
	}

	void Logger::setUserStreamLevel(Logger::AlertLevel level) {
		AA_HANDLE_MANAGER[this]->userStreamLevel = level;
	}

	Logger::AlertLevel Logger::getUserStreamLevel()const {
		return AA_HANDLE_MANAGER[this]->userStreamLevel;
	}

	bool Logger::pushLog(const char * content, Logger::AlertLevel level, const char * tag)
	{
		ArmyAnt::String wholeContent = Logger_Private::getWholeContent(content, level, tag);
		bool ret = true;
		if (level >= AA_HANDLE_MANAGER[this]->consoleLevel) {
			ret = pushLogToConsole(wholeContent.c_str());
		}
		if (level >= AA_HANDLE_MANAGER[this]->fileLevel) {
			ret = ret && pushLogToFile(wholeContent.c_str());
		}
		if (level >= AA_HANDLE_MANAGER[this]->userStreamLevel) {
			ret = ret && pushLogToUserStream(wholeContent.c_str());
		}
		return ret;
	}

	bool Logger::pushLogOnlyInConsole(const char * content, AlertLevel level, const char * tag)
	{
		ArmyAnt::String wholeContent = Logger_Private::getWholeContent(content, level, tag);
		bool ret = true;
		if (level >= AA_HANDLE_MANAGER[this]->consoleLevel) {
			ret = pushLogToConsole(wholeContent.c_str());
		}
		return ret;
	}

	bool Logger::pushLogOnlyInFile(const char * content, AlertLevel level, const char * tag)
	{
		ArmyAnt::String wholeContent = Logger_Private::getWholeContent(content, level, tag);
		bool ret = true;
		if (level >= AA_HANDLE_MANAGER[this]->fileLevel) {
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
		if (!AA_HANDLE_MANAGER[this]->logFile.IsOpened())
			return true;
		bool ret = AA_HANDLE_MANAGER[this]->logFile.Write(wholeContent);
		if (ret == 0) {
			return false;
		}
		ret = AA_HANDLE_MANAGER[this]->logFile.Write("\n");
		if (ret == 0) {
			return false;
		}
		return true;
	}

	bool Logger::pushLogToUserStream(const char * wholeContent)
	{
		if (AA_HANDLE_MANAGER[this]->userStream == nullptr)
			return true;
		if (AA_HANDLE_MANAGER[this]->userStream->bad())
			return false;
		*(AA_HANDLE_MANAGER[this]->userStream) << wholeContent << "\n";
		return true;
	}

} // namespace ArmyAntServer 

#undef AA_HANDLE_MANAGER