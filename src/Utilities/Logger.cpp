#include <Logger.h>

#include <ctime>
#include <queue>
#ifdef OS_LINUX
#include <sys/time.h>
#endif

#include <AAClassPrivateHandle.hpp>
#include <AAString.h>
#include <AAIStream_File.h>


#define AA_HANDLE_MANAGER ArmyAnt::ClassPrivateHandleManager<Logger, Logger_Private>::getInstance()

namespace ArmyAntServer {

	class Logger_Private {
	public:
		Logger_Private() :mutex(), logFileWriteQueue(), threadEnd(false), logFileWriteThread(&Logger_Private::update, this){}
		~Logger_Private() {
			threadEnd = true;
			logFileWriteThread.join();
		}

		void update();

		static ArmyAnt::String getTimeStamp();
		static ArmyAnt::String getWholeContent(const char * content, Logger::AlertLevel level, const char * tag);

		Logger::AlertLevel consoleLevel = Logger::AlertLevel::Import;
		ArmyAnt::File logFile;
		Logger::AlertLevel fileLevel = Logger::AlertLevel::Verbose;
		std::ostream* userStream = nullptr;
		Logger::AlertLevel userStreamLevel = Logger::AlertLevel::Debug;

		ArmyAnt::String logFileName;
		std::mutex mutex;
		std::queue<ArmyAnt::String> logFileWriteQueue;
		bool threadEnd = false;
		std::thread logFileWriteThread;
	};

	ArmyAnt::String Logger_Private::getTimeStamp() {
#if defined OS_WINDOWS
        auto tm = _time64(0);
		char ret[64] = "";
		_ctime64_s(ret,&tm);
		auto retStr = ArmyAnt::String(ret);
		retStr.replace('\n', "");
#else
        auto tm = time(0);
		auto retStr =  ArmyAnt::String(ctime(&tm));
#endif
		return retStr;
	}

	ArmyAnt::String Logger_Private::getWholeContent(const char * content, Logger::AlertLevel level, const char * tag) {
		ArmyAnt::String timeString = "[ " + Logger_Private::getTimeStamp() + " ] ";
		ArmyAnt::String tagString = "[ " + ArmyAnt::String(tag) + " ] ";
		ArmyAnt::String wholeContent = timeString + tagString + "[ " + Logger::convertLevelToString(level) + " ] " + content;
		return wholeContent;
	}

	void Logger_Private::update(){
		bool isEmpty = false;
		while(true){
			mutex.lock();
			bool isEmpty = logFileWriteQueue.empty();
			if(!isEmpty){
				if(logFile.IsOpened())
					logFile.Close();
				if(!logFile.Open(logFileName)){
					std::this_thread::sleep_for(std::chrono::microseconds(1));
					continue;
				}
				while(!logFileWriteQueue.empty()){
					auto msg = logFileWriteQueue.front();

					logFile.MoveTo(-1);
					bool ret = logFile.Write(msg);
					if(ret != 0){
						ret = logFile.Write("\n");
					} else{

					}

					logFileWriteQueue.pop();
				}
				logFile.Close();
				mutex.unlock();
			} else{
				mutex.unlock();
				if(threadEnd){
					break;
				} else{
					std::this_thread::sleep_for(std::chrono::microseconds(1));
				}
			}
		}
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
		if (path == nullptr)
			return false;
		auto hd = AA_HANDLE_MANAGER[this];
		hd->logFileName = path;
		hd->mutex.lock();
		hd->logFile.SetStreamMode(false, false);
		auto ret = hd->logFile.Open(path);
		hd->logFile.Close();
		hd->mutex.unlock();
		return ret;
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
		auto hd = AA_HANDLE_MANAGER[this];
		hd->mutex.lock();
		hd->logFileWriteQueue.push(wholeContent);
		hd->mutex.unlock();
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