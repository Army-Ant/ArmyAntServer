#ifndef ECHO_APP_H_20180913
#define ECHO_APP_H_20180913

#include <subApplication.h>

namespace ArmyAntServer{

class EchoApp : public SubApplication{
public:
	EchoApp(int64 appid, ServerMain&server);
	virtual ~EchoApp();

public:
	virtual bool onStart() override;
	virtual bool onStop() override;

private:
	AA_FORBID_ASSGN_OPR(EchoApp);
	AA_FORBID_COPY_CTOR(EchoApp);
};

}

#endif // ECHO_APP_H_20180913
