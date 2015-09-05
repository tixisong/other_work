#include "dataitem.h"

#include "MyLog.h"

Dataitem::Dataitem()
{
	_coll_prediction_id.reserve(1010);
}


Dataitem::~Dataitem()
{
}


bool Dataitem::openfile(const std::string & filename)
{
	_file.open(filename);
	if (!_file.good())
		return false;
	return true;
}

bool Dataitem::readfile()
{
	std::string line;
	while (getline(_file, line))
	{
		if (line.find("预测") != std::string::npos)
		{
			analyze_prediction_id(line);
		}
		else if (line.find("1位") != std::string::npos)
			continue;
		else
			analyze_value_bit(line);
	}
	counter_show();
	return true;
}

void Dataitem::analyze_prediction_id(const std::string & line)
{
	std::string sub;
	size_t current_pos = 0;
	size_t pre_pos = 0;
	while ((current_pos = (line.find(',', current_pos))) != std::string::npos)
	{
		sub = line.substr(pre_pos, current_pos - pre_pos);
		if (std::string::npos != sub.find_first_of("0123456789"))
		{
			_coll_prediction_id.push_back(std::stoi(sub));
			ValueBit emptyvaluebit;
			_colldata.insert(std::make_pair(std::stoi(sub), emptyvaluebit));
		}
		current_pos++;
		pre_pos = current_pos;
	}

}

void Dataitem::analyze_value_bit(const std::string & line)
{
	std::string sub;
	size_t current_pos = 0;
	size_t pre_pos = 0;

	int bit_value_index = 0;
	int prediction_id_index = 0;
	
	while ((current_pos = (line.find(',', current_pos))) != std::string::npos)
	{
		if (++prediction_id_index % 6 == 0)
		{
			current_pos++;
			pre_pos = current_pos;
			continue;
		}
		sub = line.substr(pre_pos, current_pos - pre_pos);
		if (!sub.empty())
		{
			switch (bit_value_index)
			{
			case 0:
				_colldata[_coll_prediction_id[prediction_id_index / 6]].bit1.push_back(std::stoi(sub));
				break;
			case 1:
				_colldata[_coll_prediction_id[prediction_id_index / 6]].bit2.push_back(std::stoi(sub));
				break;
			case 2:
				_colldata[_coll_prediction_id[prediction_id_index / 6]].bit3.push_back(std::stoi(sub));
				break;
			case 3:
				_colldata[_coll_prediction_id[prediction_id_index / 6]].bit4.push_back(std::stoi(sub));
				break;
			case 4:
				_colldata[_coll_prediction_id[prediction_id_index / 6]].bit5.push_back(std::stoi(sub));
				break;
			default:
				break;
			}
		}
		current_pos++;
		pre_pos = current_pos;
		if (++bit_value_index >= 5)
			bit_value_index = 0;
	}

	if ((current_pos = line.rfind(',')) != std::string::npos)
	{
		sub = line.substr(pre_pos, current_pos - pre_pos);
		if (!sub.empty())
		{
			_colldata[_coll_prediction_id[prediction_id_index / 6]].bit5.push_back(std::stoi(sub));
		}
	}
}


void Dataitem::counter_show()
{
	clock_t t = clock();

	ITER_COLL_DATA iter = _colldata.begin();
	for (; iter != _colldata.end(); ++iter)
	{
		for (size_t num1 = 0; num1 < iter->second.bit1.size(); ++num1)
		{
			for (size_t num2 = 0; num2 < iter->second.bit2.size(); ++num2)
			{
				for (size_t num3 = 0; num3 < iter->second.bit3.size(); ++num3)
				{
					for (size_t num4 = 0; num4 < iter->second.bit4.size(); ++num4)
					{
						for (size_t num5 = 0; num5 < iter->second.bit5.size(); ++num5)
						{
							char cnum1[5], cnum2[5], cnum3[5], cnum4[5], cnum5[5];
							_itoa_s(iter->second.bit1[num1], cnum1, 10);
							_itoa_s(iter->second.bit2[num2], cnum2, 10);
							_itoa_s(iter->second.bit3[num3], cnum3, 10);
							_itoa_s(iter->second.bit4[num4], cnum4, 10);
							_itoa_s(iter->second.bit5[num5], cnum5, 10);
							
							strcat_s(cnum1 + strlen(cnum1), 5,"*");
							strcat_s(cnum2 + strlen(cnum2), 5, "*");
							strcat_s(cnum3 + strlen(cnum3), 5, "*");
							strcat_s(cnum4 + strlen(cnum4), 5, "*");
							//strcat_s(cnum5 + strlen(cnum5), 5, "*");


							char cnum[40] = { 0 };
							strcat_s(cnum + strlen(cnum), 40, cnum1);
							strcat_s(cnum + strlen(cnum), 40, cnum2);
							strcat_s(cnum + strlen(cnum), 40, cnum3);
							strcat_s(cnum + strlen(cnum), 40, cnum4);
							strcat_s(cnum + strlen(cnum), 40, cnum5);





						//	string snum = string(cnum1) + string(cnum2) + string(cnum3) + string(cnum4) + string(cnum5);





							/*std::string snum = std::to_string(iter->second.bit1[num1]) +
								"*" + std::to_string(iter->second.bit2[num2]) +
								"*" + std::to_string(iter->second.bit3[num3]) +
								"*" + std::to_string(iter->second.bit4[num4]) +
								"*" + std::to_string(iter->second.bit5[num5]);
								*/
							

						/*	auto iter_find = _collcounter.find(cnum);
							if (iter_find != _collcounter.end())
							{
								iter_find->second++;
							}
							else*/
							//	_collcounter.insert(std::make_pair(cnum, 1));
						}
					}
				}
			}
		}
	}

	cout << "counter time: " << clock() - t << endl;

	auto itercount = _collcounter.begin();
	for (; itercount != _collcounter.end(); ++itercount)
	{
	LOG(itercount->first << "   " << itercount->second);
	}

	cout << "Log time: " << clock() - t << endl;
}



//void Dataitem::prinf_csv()
//{
//	std::ofstream ofile;
//	ofile.open("c:\\b.csv");
//	if (ofile.bad())
//	{
//		std::cout << "aa";
//	}
//
//
//	ofile << "次数,预测,位1,位2,位3,位4,位5" << std::endl;
//
//	
//	auto showiter = _collcounter.begin();
//	for (; showiter != _collcounter.end(); ++showiter)
//	{
//		ofile << showiter->first << ",";
//		ofile << showiter->second.second << ",";
//		ofile << showiter->second.first << std::endl;
//	/*	ofile << showiter->first << "," << showiter->second.first << ","
//			<< showiter->second.second << std::endl;*/
//	}
//
//
//	ofile.close();
//}
