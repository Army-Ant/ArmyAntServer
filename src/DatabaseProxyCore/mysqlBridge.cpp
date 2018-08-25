#include <mysqlBridge.h>
#include <jdbc/mysql_connection.h>
#include <jdbc/mysql_driver.h>
#include <jdbc/cppconn/statement.h>

namespace ArmyAntDBProxy{

MySqlBridge::MySqlBridge() :connection(nullptr), driver(nullptr){}

MySqlBridge::~MySqlBridge(){
	disconnect();
}

bool MySqlBridge::setConnectionUserInfo(const ArmyAnt::String& username, const ArmyAnt::String& password){
	this->username = username;
	this->password = password;
	return true;
}

bool MySqlBridge::connect(const ArmyAnt::String&serverHost){
	try{
		driver = sql::mysql::get_mysql_driver_instance();
		connection = driver->connect(serverHost.c_str(), username.c_str(), password.c_str());
	} catch(sql::SQLException& e){
		return false;
	}
	return !connection->isClosed();
}

bool MySqlBridge::connect(const ArmyAnt::String & serverAddress, const ArmyAnt::String & port){
	return connect(serverAddress + ":" + port);
}

void MySqlBridge::disconnect(){
	if(connection)
		connection->close();
	ArmyAnt::Fragment::AA_SAFE_DEL(connection);
}

bool MySqlBridge::isConnection(){
	return connection != nullptr && !connection->isClosed();
}

int64 MySqlBridge::getDatabaseCount(){
	return query("show databases").height;
}

bool MySqlBridge::getDatabaseList(ArmyAnt::SqlDatabaseInfo *& dbs, uint32 maxCount){
	auto ret = query("show databases");

	return false;
}

ArmyAnt::SqlTable MySqlBridge::query(const ArmyAnt::String & sql){
	auto st = connection->createStatement();
	auto res = st->executeQuery(sql.c_str());
	return parseResultSetToAATable(res);
}

int64 MySqlBridge::update(const ArmyAnt::String & sql){
	auto st = connection->createStatement();
	return st->executeUpdate(sql.c_str());
}

int MySqlBridge::execute(const ArmyAnt::String & sql, ArmyAnt::SqlTable*result, int32 maxResultCount){
	auto st = connection->createStatement();
	try{
		auto ret = st->execute(sql.c_str());
	} catch(sql::SQLException& e){
		return -1;
	}
	int32 index = 0;
	auto res = st->getResultSet();
	do{
		if(res == nullptr){
			auto updateRes = st->getUpdateCount();
			if(updateRes < 0){
				return 0;
			}
			if(result != nullptr && maxResultCount > index){
				result[index++] = parseResultSetToAATable(updateRes);
			}
		} else{
			if(result != nullptr && maxResultCount > index){
				result[index++] = parseResultSetToAATable(res);
			}
		}
		++index;
	} while(st->getMoreResults());

	return index;
}

ArmyAnt::SqlTable MySqlBridge::parseResultSetToAATable(void* set){
	auto jdbcSet = static_cast<sql::ResultSet*>(set);
	auto metadata = jdbcSet->getMetaData();
	auto width = metadata->getColumnCount();
	auto height = jdbcSet->rowsCount();
	auto head = new ArmyAnt::SqlFieldHead[width];
	for(mac_uint i = 0; i < width; ++i){
		head[i].allowNull = metadata->isNullable(i);
		head[i].autoIncrease = metadata->isAutoIncrement(i);
		head[i].catalogName = metadata->getCatalogName(i).c_str();
		head[i].columnName = metadata->getColumnName(i).c_str();
		head[i].length = metadata->getColumnDisplaySize(i);
		switch(metadata->getColumnType(i)){
			case sql::DataType::UNKNOWN:
				head[i].type = ArmyAnt::SqlFieldType::Unknown;
				break;
			case sql::DataType::BIT:
				head[i].type = ArmyAnt::SqlFieldType::MySql_BIT;
				break;
			case sql::DataType::TINYINT:
				head[i].type = ArmyAnt::SqlFieldType::MySql_TINYINT;
				break;
			case sql::DataType::SMALLINT:
				head[i].type = ArmyAnt::SqlFieldType::MySql_SMALLINT;
				break;
			case sql::DataType::MEDIUMINT:
				head[i].type = ArmyAnt::SqlFieldType::MySql_MEDIUMINT;
				break;
			case sql::DataType::INTEGER:
				head[i].type = ArmyAnt::SqlFieldType::MySql_INT;
				break;
			case sql::DataType::BIGINT:
				head[i].type = ArmyAnt::SqlFieldType::MySql_BIGINT;
				break;
			case sql::DataType::REAL:
				head[i].type = ArmyAnt::SqlFieldType::MySql_FLOAT;
				break;
			case sql::DataType::DOUBLE:
				head[i].type = ArmyAnt::SqlFieldType::MySql_DOUBLE;
				break;
			case sql::DataType::DECIMAL:
				head[i].type = ArmyAnt::SqlFieldType::MySql_DEMICAL;
				break;
			case sql::DataType::NUMERIC:
				head[i].type = ArmyAnt::SqlFieldType::MySql_NUMERIC;
				break;
			case sql::DataType::CHAR:
				head[i].type = ArmyAnt::SqlFieldType::MySql_CHAR;
				break;
			case sql::DataType::BINARY:
				head[i].type = ArmyAnt::SqlFieldType::MySql_BINARY;
				break;
			case sql::DataType::VARCHAR:
				head[i].type = ArmyAnt::SqlFieldType::MySql_VARCHAR;
				break;
			case sql::DataType::VARBINARY:
				head[i].type = ArmyAnt::SqlFieldType::MySql_VARBINARY;
				break;
			case sql::DataType::LONGVARCHAR:
				head[i].type = ArmyAnt::SqlFieldType::MySql_LONGVARCHAR;
				break;
			case sql::DataType::LONGVARBINARY:
				head[i].type = ArmyAnt::SqlFieldType::MySql_LONGVARBINARY;
				break;
			case sql::DataType::TIMESTAMP:
				head[i].type = ArmyAnt::SqlFieldType::MySql_TIMESTAMP;
				break;
			case sql::DataType::DATE:
				head[i].type = ArmyAnt::SqlFieldType::MySql_DATE;
				break;
			case sql::DataType::TIME:
				head[i].type = ArmyAnt::SqlFieldType::MySql_TIME;
				break;
			case sql::DataType::YEAR:
				head[i].type = ArmyAnt::SqlFieldType::MySql_YEAR;
				break;
			case sql::DataType::GEOMETRY:
				head[i].type = ArmyAnt::SqlFieldType::MySql_GEOMETRY;
				break;
			case sql::DataType::ENUM:
				head[i].type = ArmyAnt::SqlFieldType::MySql_ENUM;
				break;
			case sql::DataType::SET:
				head[i].type = ArmyAnt::SqlFieldType::MySql_SET;
				break;
			case sql::DataType::SQLNULL:
				head[i].type = ArmyAnt::SqlFieldType::Null;
				break;
			case sql::DataType::JSON:
				head[i].type = ArmyAnt::SqlFieldType::MySql_JSON;
				break;
			default:
				head[i].type = ArmyAnt::SqlFieldType::Unknown;
		}
	}
	auto ret = createSqlTable(head, width, height);
	for(uint32 j = 0; j < width; ++j){
		for(uint32 i = 0; i < height; ++i){
			ret(i, j).setValue(jdbcSet->getString(j).c_str());
		}
		jdbcSet->next();
	}
	ArmyAnt::Fragment::AA_SAFE_DELALL(head);
	return ret;
}

ArmyAnt::SqlTable MySqlBridge::parseResultSetToAATable(int res){
	ArmyAnt::SqlFieldHead head;
	head.allowNull = false;
	head.autoIncrease = false;
	head.catalogName = "sqlResults";
	head.columnName = "sqlResults";
	head.length = sizeof(int64);
	head.type = ArmyAnt::SqlFieldType::UpdateResult;
	auto ret = createSqlTable(&head, 1, 1);
	ret(0, 0).setValue(ArmyAnt::String(0));
	return ret;
}

}
