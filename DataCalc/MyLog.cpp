
#include"MyLog.h"



#include <time.h>
#include <sys/timeb.h>
string  GetNowTime()
{
	struct tm now;
	struct _timeb tb;
	_ftime_s(&tb);
	localtime_s(&now, &tb.time);
	char pAddrB[30];
	sprintf_s(pAddrB, 30, "%04d-%02d-%02d %02d:%02d:%02d.%03d", now.tm_year + 1900,
		now.tm_mon + 1, now.tm_mday, now.tm_hour, now.tm_min, now.tm_sec, tb.millitm);
	string stime(pAddrB);
	return stime;

}

ofstream & getfile()
{
	string szPath = "ds_log.txt";
	static ofstream filelog(szPath.c_str(), ios::app);
	filelog << setprecision(15);
	return filelog;
}


