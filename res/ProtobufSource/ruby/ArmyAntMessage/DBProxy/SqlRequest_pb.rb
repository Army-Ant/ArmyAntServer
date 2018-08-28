# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: ArmyAntMessage/DBProxy/SqlRequest.proto

require 'google/protobuf'

Google::Protobuf::DescriptorPool.generated_pool.build do
  add_message "ArmyAntMessage.DBProxy.SqlHeadOne" do
    optional :length, :int32, 1
    optional :catalogName, :string, 2
    optional :columnName, :string, 3
    optional :type, :int32, 4
    optional :allowNumm, :bool, 5
    optional :autoIncrease, :bool, 6
  end
  add_message "ArmyAntMessage.DBProxy.SqlRow" do
    repeated :fields, :string, 1
  end
  add_message "ArmyAntMessage.DBProxy.SqlDatabaseInfo" do
    optional :name, :string, 1
    optional :server, :string, 2
    optional :charset, :string, 3
    optional :sortRule, :string, 4
  end
  add_message "ArmyAntMessage.DBProxy.SqlTableInfo" do
    optional :tableName, :string, 5
    optional :engine, :string, 6
    optional :comment, :string, 7
    optional :database, :message, 8, "ArmyAntMessage.DBProxy.SqlDatabaseInfo"
  end
  add_message "ArmyAntMessage.DBProxy.SqlResult" do
    optional :table, :message, 1, "ArmyAntMessage.DBProxy.SqlTableInfo"
    repeated :heads, :message, 2, "ArmyAntMessage.DBProxy.SqlHeadOne"
    repeated :rows, :message, 3, "ArmyAntMessage.DBProxy.SqlRow"
  end
  add_message "ArmyAntMessage.DBProxy.Any2Dbp_SqlRequest" do
    optional :type, :int32, 1
  end
end

module ArmyAntMessage
  module DBProxy
    SqlHeadOne = Google::Protobuf::DescriptorPool.generated_pool.lookup("ArmyAntMessage.DBProxy.SqlHeadOne").msgclass
    SqlRow = Google::Protobuf::DescriptorPool.generated_pool.lookup("ArmyAntMessage.DBProxy.SqlRow").msgclass
    SqlDatabaseInfo = Google::Protobuf::DescriptorPool.generated_pool.lookup("ArmyAntMessage.DBProxy.SqlDatabaseInfo").msgclass
    SqlTableInfo = Google::Protobuf::DescriptorPool.generated_pool.lookup("ArmyAntMessage.DBProxy.SqlTableInfo").msgclass
    SqlResult = Google::Protobuf::DescriptorPool.generated_pool.lookup("ArmyAntMessage.DBProxy.SqlResult").msgclass
    Any2Dbp_SqlRequest = Google::Protobuf::DescriptorPool.generated_pool.lookup("ArmyAntMessage.DBProxy.Any2Dbp_SqlRequest").msgclass
  end
end
