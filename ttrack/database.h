#ifndef _DATABASE_H_
#define _DATABASE_H_

BOOL DatabaseOpen(void);
void DatabaseInit(void);
void DatabasePush(LPWSTR modulePath, LPWSTR windowTitle);
void DatabaseClose(void);

#endif