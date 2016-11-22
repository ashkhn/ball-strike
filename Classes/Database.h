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
		static bool createTables();
		static std::vector<std::vector<std::string>> getQueryResults(const char* query);
		static bool createSaveTables();

	private:
		Database();
		static Database* singleton;
		sqlite3* database;
		static Database* getInstance();
};
