#include <AASqlClient.h>
#include <AAString.h>

#ifndef MYSQL_BRIDGE_H_20180824
#define MYSQL_BRIDGE_H_20180824

namespace  sql{
class Connection;
namespace mysql{
class MySQL_Connection;
class MySQL_Driver;
}
}

namespace ArmyAntDBProxy{

class MySqlBridge : public ArmyAnt::ISqlClient{
public:
	MySqlBridge();
	~MySqlBridge();

public:
	bool setConnectionUserInfo(const ArmyAnt::String& username, const ArmyAnt::String& password);

public:
	virtual bool connect(const ArmyAnt::String&serverHost) override;
	virtual bool connect(const ArmyAnt::String&serverAddress, const ArmyAnt::String&port) override;
	virtual void disconnect() override;
	virtual bool isConnection() override;
	virtual uint32 getDatabaseCount() override;
	virtual uint32 getDatabaseList(ArmyAnt::String*&dbs, uint32 maxCount = 0) override;

public:
	virtual bool getTablesCount() override;
	virtual bool getViewsCount() override;
	virtual bool getTableNameList(ArmyAnt::SqlTableInfo*&tables, uint32 maxCount = 0) override;
	virtual bool getViewNameList(ArmyAnt::SqlTableInfo*&tables, uint32 maxCount = 0) override;
	virtual ArmyAnt::SqlTable getTableAllFields(ArmyAnt::SqlTableInfo*&tables) override;

public:
	virtual ArmyAnt::SqlTable query(const ArmyAnt::String&sql) override;
	virtual int64 update(const ArmyAnt::String&sql) override;
	virtual int execute(const ArmyAnt::String&sql, ArmyAnt::SqlTable*result, int32 maxResultCount) override;

private:
	static ArmyAnt::SqlTable parseResultSetToAATable(void* set);
	static ArmyAnt::SqlTable parseResultSetToAATable(uint64 res);

private:
	sql::Connection* connection;
	sql::mysql::MySQL_Driver* driver;
	ArmyAnt::String username;
	ArmyAnt::String password;

	AA_FORBID_ASSGN_OPR(MySqlBridge);
	AA_FORBID_COPY_CTOR(MySqlBridge);
};

}

#endif // MYSQL_BRIDGE_H_20180824
