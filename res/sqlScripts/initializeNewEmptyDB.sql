
create database ArmyAntDB;
use ArmyAntDB;
create table app_list (id int, appid bigint, name varchar(512));

CREATE USER 'aa_server_dbproxy'@'%' IDENTIFIED BY 'L05rg05rgP8K';
GRANT ALL PRIVILEGES ON ArmyAntDB.* TO 'aa_server_dbproxy'@'%';
CREATE USER 'aa_db_guest'@'%' IDENTIFIED BY '0624zjcy0225';
GRANT select ON ArmyAntDB.* TO 'aa_db_guest'@'%';

use mysql;
GRANT ALL PRIVILEGES ON *.* TO 'root'@'%' IDENTIFIED BY '05rgP8KL05rg' WITH GRANT OPTION;
FLUSH PRIVILEGES;
