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
	typedef std::map<int, ValueBit,std::greater<int> > COLLDATE;
	COLLDATE _colldata;
	typedef COLLDATE::const_iterator ITER_COLL_DATA;
	std::vector<int> _coll_prediction_id;

	//转成字符 统计 出现的序号 + 出现的次数 
	//std::map<std::string, std::vector<std::pair<int, int>>> _collcounter;
	std::hash_map<std::string, int> _collcounter;
	//std::hash_map<_int64, int> _collcounter;

private:
	void analyze_prediction_id(const std::string & line);
	void analyze_value_bit(const std::string & line);
	void counter_show();
	void prinf_csv();




public:
	bool openfile(const std::string & filename);
	bool readfile();


};

