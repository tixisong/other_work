#include <map>
#include <fstream>
#include <string>
#include <iostream>
#include "dataitem.h"
using namespace std;

int main()
{
	
	Dataitem dataitem;
	bool bRet = dataitem.openfile("≤‚ ‘”√.csv");
	if (!bRet)
		cout << "open csv error!";
	dataitem.readfile();

}