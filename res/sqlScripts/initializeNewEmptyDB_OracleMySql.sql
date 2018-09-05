
create database ArmyAntDB;
use ArmyAntDB;
create table app_list (id int, appid bigint, name varchar(512));

CREATE USER 'aa_server_dbproxy'@'%' IDENTIFIED BY 'L05rg05rgP8K';
GRANT ALL ON ArmyAntDB.* TO 'aa_server_dbproxy'@'%';
CREATE USER 'aa_db_guest'@'%' IDENTIFIED BY '0624zjcy0225';
GRANT select ON ArmyAntDB.* TO 'aa_db_guest'@'%';

use mysql;
update user set host = '%'  where user ='root';
FLUSH PRIVILEGES;
ALTER USER 'root'@'%' IDENTIFIED WITH mysql_native_password BY '05rgP8KL05rg';
