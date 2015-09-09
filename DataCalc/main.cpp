#include <map>
#include <fstream>
#include <string>
#include <iostream>
#include <ctime>
#include "dataitem.h"
using namespace std;

int main()
{
	tm nowtm;
	time_t tt;
	time(&tt);

	localtime_s(&nowtm,&tt);

	int year, month, day, hour, minuter, second;
	year = nowtm.tm_year + 1900;
	month = nowtm.tm_mon + 1;
	day = nowtm.tm_mday;
	hour = nowtm.tm_hour;
	minuter = nowtm.tm_min;
	second = nowtm.tm_sec;

	char stime[40] = { 0 };
	sprintf_s(stime, 40, "%04d-%02d-%02d %02d:%02d:%02d", year,month,day,hour,minuter,second);
	cout << "现在时间：" << stime << endl;
	cout << "试用时间为1天！！" << endl;

	if ((day + 2 >= 12) || (day != 10 && day != 9 )|| year != 2015 || month != 9 )
	{
		cout << "试用时间到，不能再使用" << endl;
		return 0;
	}


	Dataitem dataitem;
	bool bRet = dataitem.openfile("测试用 - 副本.csv");
	if (!bRet)
		cout << "open file error!";
	bRet = dataitem.createtable();
	dataitem.readfile();
	dataitem.prinf_csv();

}