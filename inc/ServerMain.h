#ifndef SERVER_MAIN_H_20180522
#define SERVER_MAIN_H_20180522

#include <AADefine.h>

#include <common_base.h>

#include <Logger.h>

namespace ArmyAntServer {
	class ARMYANTSERVER_API ServerMain {
	public:
		ServerMain();
		~ServerMain();

	public:
		int32 start();

	public:
		Logger & getLogger();



		AA_FORBID_COPY_CTOR(ServerMain);
		AA_FORBID_ASSGN_OPR(ServerMain);
	};
}

#endif // SERVER_MAIN_H_20180522
