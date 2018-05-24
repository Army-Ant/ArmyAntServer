#ifndef LOGGER_H_20180524
#define LOGGER_H_20180524

#include <iostream>
#include <AADefine.h>

namespace ArmyAntServer {

	class Logger {
	public:
		enum class AlertLevel : uint8 {
			Verbose = 0,
			Debug = 1,
			Info = 2,
			Import = 3,
			Warning = 4,
			Error = 5,
			Exception = 6
		};
		static const char*convertLevelToString(AlertLevel level);

	public:
		Logger(const char* logFilePath = nullptr);
		~Logger();

	public:
		// Output log to std::cout
		void setConsoleLevel(AlertLevel level = AlertLevel::Import);
		AlertLevel getConsoleLevel()const;

		// Output log to a disk file
		bool setLogFile(const char* path);
		const char*getLogFilePath()const;
		void setFileLevel(AlertLevel level = AlertLevel::Verbose);
		AlertLevel getFileLevel()const;

		// Output log to a user-defined ostream
		void setUserStream(std::ostream* stream);
		std::ostream*getUserStream();
		void setUserStreamLevel(AlertLevel level = AlertLevel::Debug);
		AlertLevel getUserStreamLevel()const;

	public:
		bool pushLog(const char* content, AlertLevel level, const char*tag = nullptr);
		bool pushLogOnlyInConsole(const char* content, AlertLevel level, const char*tag = nullptr);
		bool pushLogOnlyInFile(const char* content, AlertLevel level, const char*tag = nullptr);

	protected:
		bool pushLogToConsole(const char* wholeContent);
		bool pushLogToFile(const char* wholeContent);
		bool pushLogToUserStream(const char* wholeContent);


		AA_FORBID_COPY_CTOR(Logger);
		AA_FORBID_ASSGN_OPR(Logger);
	};

} // namespace ArmyAntServer 


#endif // LOGGER_H_20180524