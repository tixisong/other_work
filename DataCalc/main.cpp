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
	cout << "����ʱ�䣺" << stime << endl;
	cout << "����ʱ��Ϊ���죡��" << endl;

	if (day + 2 >= 11 || year != 2015 || month != 9)
	{
		cout << "����ʱ�䵽��������ʹ��" << endl;
		return 0;
	}







	Dataitem dataitem;
	bool bRet = dataitem.openfile("198--��3.csv");
	if (!bRet)
		cout << "open file error!";
	bRet = dataitem.createtable();
	dataitem.readfile();

}