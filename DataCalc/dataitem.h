#pragma once

#include <string>
#include <map>
#include <set>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm> 
#include <functional>

class Dataitem
{
public:
	struct ValueBit {
		short bit1;
		short bit2;
		short bit3;
		short bit4;
		short bit5;
		ValueBit() :bit1(0), bit2(0), bit3(0), bit4(0), bit5(0){}
	};

public:
	Dataitem();
	~Dataitem();

private:
	std::fstream _file;
	typedef std::multimap<int, ValueBit> COLLDATE;
	COLLDATE _colldata;
	std::vector<int> _prediction_id;

	typedef std::multimap<std::string, int> COMBINE;
	COMBINE _combine;

	typedef std::multimap<int,std::pair<std::string,int>,std::greater<int> > COLLCOUNTRE;
	COLLCOUNTRE _collcounter;

private:
	void analyze_prediction_id(const std::string & line);
	void analyze_value_bit(const std::string & line);
	void combin_to_string_num();
	
	void prinf_csv();




public:
	bool openfile(const std::string & filename);
	bool readfile();


};

