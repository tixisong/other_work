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
#include <hash_map>
#include "sqlite3.h"

#pragma comment(lib,"sqlite3.lib")

class Dataitem
{
public:
	struct ValueBit {
		std::vector<int> bit1;
		std::vector<int> bit2;
		std::vector<int> bit3;
		std::vector<int> bit4;
		std::vector<int> bit5;
		ValueBit()
		{
			bit1.reserve(15);
			bit2.reserve(15);
			bit3.reserve(15);
			bit4.reserve(15);
			bit5.reserve(15);
		}
	};

public:
	Dataitem();
	~Dataitem();

private:
	std::fstream _file;
	typedef std::map<int, ValueBit,std::greater<int> > COLLDATA;
	COLLDATA _colldata;
	typedef COLLDATA::const_iterator ITER_COLL_DATA;
	std::vector<int> _coll_prediction_id;
	std::vector<COLLDATA> _thread_coll_data;

	//std::map<std::string, int> _collcounter;
	sqlite3 * db;

private:
	void analyze_prediction_id(const std::string & line);
	void analyze_value_bit(const std::string & line);
	void split_colldata();
	void counter_show();
	void prinf_csv();
	void insert_db(int index);

public:
	bool openfile(const std::string & filename);
	bool readfile();
	bool createtable();
	bool search_data(int counter);

};

