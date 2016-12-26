#pragma once
#include "cocos2d.h"
#include "sqlite3.h"

class Database {
	public:
		static sqlite3* getDatabase();
		static bool open();
		static bool create(std::string table_name);
		static void close();
		static bool execute(std::string sql_stmt);
		static bool createDataTables();
		static std::vector<std::vector<std::string>> getQueryResults(const char* query);
		static bool createSaveTables();
		static bool createLevelTables();
		static bool createLevel(int num_enemies, int num_balls, int num_hits_per_enemy);

	private:
		Database();
		static Database* singleton;
		sqlite3* database;
		static Database* getInstance();
};
