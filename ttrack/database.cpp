#include "stdafx.h"

/* Private Variables */
static sqlite3 *db = NULL;

/* Private Functions */
static void GetDatabasePath(TCHAR *modulePath, int size)
{
	int len = GetModuleFileName(NULL, modulePath, size);
	modulePath[len - 3] = 'd';
	modulePath[len - 2] = 'b';
	modulePath[len - 1] = NULL;
}

static int GetApplicationId(LPWSTR modulePath, LPWSTR windowTitle)
{
	sqlite3_stmt *stmt = NULL;

	const TCHAR sql[MAX_PATH*3] = _T("SELECT id FROM APPLICATIONS WHERE path = ? AND title = ?;");
	sqlite3_reset(stmt);
	sqlite3_prepare16(db, sql, MAX_PATH*3, &stmt, NULL);
	sqlite3_bind_text16(stmt, 1, modulePath, -1, SQLITE_STATIC);
	sqlite3_bind_text16(stmt, 2, windowTitle, -1, SQLITE_STATIC);
	int ret = -1;
	if (sqlite3_step(stmt) == SQLITE_ROW) {
		ret = sqlite3_column_int(stmt, 0);
	}
	sqlite3_finalize(stmt);
	return ret;
}

static int PushApplication(LPWSTR modulePath, LPWSTR windowTitle)
{
	sqlite3_stmt *stmt = NULL;
	const TCHAR sql[MAX_PATH*3] = _T("INSERT INTO APPLICATIONS (path, title) VALUES (?, ?);");
	sqlite3_reset(stmt);
	sqlite3_prepare16(db, sql, MAX_PATH*3, &stmt, NULL);
	sqlite3_bind_text16(stmt, 1, modulePath, -1, SQLITE_STATIC);
	sqlite3_bind_text16(stmt, 2, windowTitle, -1, SQLITE_STATIC);
	sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	return GetApplicationId(modulePath, windowTitle);
}


/* Public Functions */
BOOL DatabaseOpen(void)
{
	TCHAR modulePath[MAX_PATH] = {0};
	GetDatabasePath(modulePath, MAX_PATH);
	if (!PathFileExists(modulePath)) {
		sqlite3_open16(modulePath, &db);
		return FALSE;
	}
	return (sqlite3_open16(modulePath, &db) == SQLITE_OK) ;
}

void DatabaseInit(void)
{
	char create_sql[] = "CREATE TABLE APPLICATIONS ( \
						    id INTEGER PRIMARY KEY, \
							path TEXT NOT NULL, \
							title TEXT NOT NULL); \
						 CREATE TABLE ACCESSES ( \
						    time DATETIME NOT NULL, \
							application_id INTEGER NOT NULL, \
							FOREIGN KEY (application_id) \
							REFERENCES APPLICATIONS(id));";
	char *zErrMsg = 0;
	sqlite3_exec(db, create_sql, 0, 0, &zErrMsg);
	return;
}

void DatabasePush(LPWSTR modulePath, LPWSTR windowTitle)
{
	sqlite3_stmt *stmt = NULL;

	int appId = GetApplicationId(modulePath, windowTitle);
	if (appId < 0) {
		appId = PushApplication(modulePath, windowTitle);
	}

	const TCHAR sql[MAX_PATH*3] = _T("INSERT INTO ACCESSES (time, application_id) VALUES (datetime(\'now\', \'localtime\'), ?);");
	sqlite3_reset(stmt);
	sqlite3_prepare16(db, sql, MAX_PATH*3, &stmt, NULL);
	sqlite3_bind_int(stmt, 1, appId);
	sqlite3_step(stmt);
	sqlite3_finalize(stmt);	
	return;
}

void DatabaseClose(void)
{
	if (db != NULL) {
		sqlite3_close(db);
	}
	return;
}
