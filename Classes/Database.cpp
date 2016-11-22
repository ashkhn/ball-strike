#include "Database.h"
USING_NS_CC;

Database* Database::singleton = NULL;

Database::Database(){
	database = NULL;
}

Database* Database::getInstance(){
	if(singleton == NULL){
		singleton = new Database();
	}
	return singleton;
}

sqlite3* Database::getDatabase(){
	return Database::getInstance()->database;
}

bool Database::open(){
	std::string file_path = FileUtils::getInstance()->getWritablePath() + "game_data.sqlite3";
	if(sqlite3_open(file_path.c_str(), &Database::getInstance()->database) != SQLITE_OK){
		sqlite3_close(Database::getInstance()->database);
		log("Failed to open database");
		return false;
	}
	return true;
}

void Database::close(){
	sqlite3_close(Database::getInstance()->database);
}

bool Database::execute(std::string sql){
	char* error_message;
	if(sqlite3_exec(Database::getInstance()->database, sql.c_str(), NULL, NULL, &error_message) != SQLITE_OK){
		log("%s", error_message);
		return false;
	}

	return true;
}

bool Database::createTables(){
	std::string check_stmt = "select count(type) from sqlite_master where type='table' and name='game_data'";
	std::vector<std::vector<std::string>> check_result = getQueryResults(check_stmt.c_str());
	if(check_result[0][0] == "0"){
		std::string create_stmt = "create table game_data(num_enemies int, num_balls int, scale float)";
		if(execute(create_stmt)){
			std::string insert_stmt = "insert into game_data(num_enemies, num_balls, scale) values(4, 2, 1.0)";
			auto status = execute(insert_stmt) ? "Success" : "Failure";
			log("The operation status was %s", status);
			return true;
		}
	}
	return false;
}

bool Database::createSaveTables(){
	std::string create_ball_stmt = "create table if not exists ball_save_data(id int primary_key autoincrement, ball_class int, scale float, posn_x float, posn_y float)";
	std::string create_enemy_stmt = "create table if not exists enemy_save_data(id int primary_key autoincrement, enemy_class int, posn_x float, posn_y float)";
	auto ball_status = execute(create_ball_stmt);
	auto enemy_status = execute(create_enemy_stmt);
	return ball_status && enemy_status;
}

std::vector<std::vector<std::string>> Database::getQueryResults(const char* query){
	sqlite3_stmt* query_stmt;
	std::vector<std::vector<std::string>> results;
	if(sqlite3_prepare_v2(Database::getInstance()->database, query, -1, &query_stmt, 0) == SQLITE_OK){
		int cols = sqlite3_column_count(query_stmt);
		int result_code = 0;
		while(1){
			result_code = sqlite3_step(query_stmt);
			if(result_code == SQLITE_ROW){
				std::vector<std::string> values;
				for(int i = 0; i < cols; i++){
					std::string value;
					char* ptr = (char*) sqlite3_column_text(query_stmt, i);
					if(ptr){
						value = ptr;
					}
					else{
						value = "";
					}
					values.push_back(value);
				}
				results.push_back(values);
			}else{
				break;
			}
		}
		sqlite3_finalize(query_stmt);
	}
	const char* error_message = sqlite3_errmsg(Database::getInstance()->database);
	log("Error message is %s", error_message);
	return results;
}
