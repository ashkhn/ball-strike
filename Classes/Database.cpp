#include "Database.h"
USING_NS_CC;

Database* Database::singleton = NULL;

Database::Database(){
	database = NULL;
}

/* Initialize a singleton instance of the databse
 *
 */
Database* Database::getInstance(){
	if(singleton == NULL){
		singleton = new Database();
	}
	return singleton;
}

sqlite3* Database::getDatabase(){
	return Database::getInstance()->database;
}

/* Open the database connection */
/* @return true if successful and false otherwise */
bool Database::open(){
	std::string file_path = FileUtils::getInstance()->getWritablePath() + "game_data.sqlite3";
	if(sqlite3_open(file_path.c_str(), &Database::getInstance()->database) != SQLITE_OK){
		sqlite3_close(Database::getInstance()->database);
		log("Failed to open database");
		return false;
	}
	return true;
}

/* Close the database connection. */ 
void Database::close(){
	sqlite3_close(Database::getInstance()->database);
}

/* Execute given sql statement. Use it for queries which do not return any data. */
/* For other queries use getQueryResults() instead */
/* @param the sql query to be executed. */ 
/* sqlite3_exec internally calls sqlite3_prepare_v2(), sqlite3_step() and sqlite3_finalize() */
/* @return true if the query was executed successfully and false otherwise */
bool Database::execute(std::string sql){
	char* error_message;
	if(sqlite3_exec(Database::getInstance()->database, sql.c_str(), NULL, NULL, &error_message) != SQLITE_OK){
		log("%s", error_message);
		return false;
	}

	return true;
}

/* Creates the tables for storing game level parameters */
/* @return true if tables were created successfully and false otherwise */
bool Database::createDataTables(){
	std::string create_stmt = "create table if not exists game_data(num_enemies integer, num_balls integer, scale float)";
	if(execute(create_stmt)){
		std::string insert_stmt = "insert into game_data(num_enemies, num_balls, scale) values(4, 2, 1.0)";
		auto status = execute(insert_stmt) ? "Success" : "Failure";
		log("The operation status was %s", status);
		return true;
	}

	return false;
}
/* Creates the tables for storing game save data */
/* @return true if tables were created successfully and false otherwise */
bool Database::createSaveTables(){
	std::string create_ball_stmt = "create table if not exists ball_save_data(id integer primary key autoincrement, ball_class integer, scale float, posn_x float, posn_y float)";
	std::string create_enemy_stmt = "create table if not exists enemy_save_data(id integer primary key autoincrement, enemy_class integer, posn_x float, posn_y float)";
	auto ball_status = execute(create_ball_stmt);
	auto enemy_status = execute(create_enemy_stmt);
	return ball_status && enemy_status;
}

/* Executes a query and returns the results. */ 
/* Use it only for queries which return a result. For other queries use execute() instead. */
/* @param query : SQL query to be executed */
/* @return 2d vector containing results. Each row of the vector corresponds to a result row.*/
/* Casts all column values to string */
/* First row contains the column names so ignore those while processing the result data */
std::vector<std::vector<std::string>> Database::getQueryResults(const char* query){
	sqlite3_stmt* query_stmt;
	std::vector<std::vector<std::string>> results;
	//Prepare the statement
	if(sqlite3_prepare_v2(Database::getInstance()->database, query, -1, &query_stmt, 0) == SQLITE_OK){
		// If prepare was successful step through the results
		int cols = sqlite3_column_count(query_stmt);
		int result_code = 0;
		while(1){
			result_code = sqlite3_step(query_stmt);
			if(result_code == SQLITE_ROW){
				//For each row get the column data
				std::vector<std::string> values;
				for(int i = 0; i < cols; i++){
					std::string value;
					char* ptr = (char*) sqlite3_column_text(query_stmt, i);
					// Check for null values
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
	//Log the error
	const char* error_message = sqlite3_errmsg(Database::getInstance()->database);
	log("Error message is %s", error_message);
	return results;
}
