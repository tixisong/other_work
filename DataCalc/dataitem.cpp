#include "dataitem.h"

#include "MyLog.h"
#include <thread>

#define SPLIT_SIZE	300

Dataitem::Dataitem()
{
	_coll_prediction_id.reserve(1010);
}


Dataitem::~Dataitem()
{
}

bool  Dataitem::createtable()
{
	int rc = sqlite3_enable_shared_cache(1);
	rc = sqlite3_open("data.db", &db);
	rc = sqlite3_exec(db, "PRAGMA synchronous = OFF", nullptr, nullptr, 0);
	rc = sqlite3_exec(db, "PRAGMA journal_mode = MEMORY", nullptr, nullptr, 0);
	rc = sqlite3_exec(db, "PRAGMA cache_size = 8000", nullptr, nullptr, 0);
	rc = sqlite3_exec(db, "PRAGMA page_size = 8192", nullptr, nullptr, 0);

	if (rc)
	{
		std::cout << "create db error" << std::endl;
		return false;
	}
	/*char * sql;
	char * errMsg;
	sql = "create table if not exists t_data (combine_num text,prediction_id int);";
	rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);
	if (rc)
	{
		std::cout << errMsg << endl;
		sqlite3_free(errMsg);
		return false;
	}*/

	/*sql = "create index if not exists COMBINE_NUM on t_data(combine_num);";
	rc = sqlite3_exec(db, sql, NULL, NULL, &errMsg);
	if (rc)
	{
		std::cout << errMsg << endl;
		sqlite3_free(errMsg);
		return false;
	}*/

	return true;


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

	split_colldata();

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


void Dataitem::split_colldata()
{
	if (_colldata.size() >= 100)
	{
		int counter = 0;
		auto iter_current = _colldata.begin();
		auto iter_pre_begin = iter_current;
		for (; iter_current != _colldata.end(); ++iter_current)
		{
			if (++counter > SPLIT_SIZE)
			{
				_thread_coll_data.push_back(COLLDATA(iter_pre_begin, iter_current));
				iter_pre_begin = iter_current;
				/*if (++iter_pre_begin == _colldata.end())
					break;*/
				counter = 0;
			}
		}
		if (iter_pre_begin != _colldata.end())
			_thread_coll_data.push_back(COLLDATA(iter_pre_begin, _colldata.end()));
	}
}


void Dataitem::insert_db(int index)
{
	std::cout <<"index: " << index << "  " << this_thread::get_id() << endl;

	sqlite3_stmt * stmt;
	char *errMsg;
	char * tail = 0;
	char sql[256] = { 0 };
	
	sprintf_s(sql,"create table if not exists t_data_%d (combine_num text,prediction_id int);",0);
	int rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);
	if (rc)
	{
		std::cout << errMsg << endl;
		sqlite3_free(errMsg);
	}
	sprintf_s(sql, 256, "insert into t_data_%d values (@RT, @BR)",0);
	rc = sqlite3_prepare_v2(db, sql, 256, &stmt, nullptr);
	if (rc)
	{
		std::cout << "insert into t_data values error" << endl;
	}
	/*rc = sqlite3_exec(db, "begin transaction", 0, 0, &errMsg);
	if (rc)
	{
		std::cout << "begin transaction error" << endl;
	}*/

	ITER_COLL_DATA iter = _thread_coll_data[index].begin();
	for (; iter != _thread_coll_data[index].end(); ++iter)
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
							strcat_s(cnum1, 5, "*");
							strcat_s(cnum2, 5, "*");
							strcat_s(cnum3, 5, "*");
							strcat_s(cnum4, 5, "*");
							char cnum[40] = { 0 };
							strcat_s(cnum, 40, cnum1);
							strcat_s(cnum, 40, cnum2);
							strcat_s(cnum, 40, cnum3);
							strcat_s(cnum, 40, cnum4);
							strcat_s(cnum, 40, cnum5);

							rc = sqlite3_bind_text(stmt, 1, cnum, -1, SQLITE_TRANSIENT);
							rc = sqlite3_bind_int(stmt, 2, iter->first);
							sqlite3_step(stmt);     /* Execute the SQL Statement */
							sqlite3_clear_bindings(stmt);   /* Clear bindings */
							sqlite3_reset(stmt);        /* Reset VDBE */

						}
					}
				}
			}
		}
	}
	//sqlite3_exec(db, "commit transaction", 0, 0, &errMsg);
	sqlite3_finalize(stmt);


	/*sprintf_s(sql,"create index if not exists COMBINE_NUM_%d on t_data_%d(combine_num);",index,index);
	rc = sqlite3_exec(db, sql, NULL, NULL, &errMsg);
	if (rc)
	{
		std::cout << errMsg << endl;
		sqlite3_free(errMsg);
	}*/

}



void Dataitem::counter_show()
{
	clock_t t = clock();

	int rc;
	char *errMsg;
	rc = sqlite3_exec(db, "begin transaction", 0, 0, &errMsg);
	if (rc)
	{
		std::cout << "begin transaction error" << endl;
	}

	std::thread t1(&Dataitem::insert_db, this, 0);
	std::thread t2(&Dataitem::insert_db, this, 1);
	std::thread t3(&Dataitem::insert_db, this, 2);
	std::thread t4(&Dataitem::insert_db, this, 3);
	t1.join();
	t2.join();
	t3.join();
	t4.join();

	sqlite3_exec(db, "commit transaction", 0, 0, &errMsg);

	char sql[200] = { 0 };
	sprintf_s(sql, "create index if not exists COMBINE_NUM on t_data_%d(combine_num);", 0);
	rc = sqlite3_exec(db, sql, NULL, NULL, &errMsg);
	if (rc)
	{
		std::cout << errMsg << endl;
		sqlite3_free(errMsg);
	}


	LOG("counter time:" << clock() - t );





	//sqlite3_stmt * stmt;
	//char *errMsg;
	//char * tail = 0;
	//char sql[256] = { 0 };
	//sprintf_s(sql, 256, "insert into t_data values (@RT, @BR)");
	//int rc = sqlite3_prepare_v2(db, sql, 256, &stmt, nullptr);
	//if (rc)
	//{
	//	std::cout << "insert into t_data values error"  << endl;
	//}
	//
	//rc = sqlite3_exec(db, "begin transaction", 0, 0, &errMsg);
	//if (rc)
	//{
	//	std::cout << "begin transaction error" << endl;
	//}

	//ITER_COLL_DATA iter = _colldata.begin();
	//for (; iter != _colldata.end(); ++iter)
	//{
	//	for (size_t num1 = 0; num1 < iter->second.bit1.size(); ++num1)
	//	{
	//		for (size_t num2 = 0; num2 < iter->second.bit2.size(); ++num2)
	//		{
	//			for (size_t num3 = 0; num3 < iter->second.bit3.size(); ++num3)
	//			{
	//				for (size_t num4 = 0; num4 < iter->second.bit4.size(); ++num4)
	//				{
	//					for (size_t num5 = 0; num5 < iter->second.bit5.size(); ++num5)
	//					{
	//						char cnum1[5], cnum2[5], cnum3[5], cnum4[5], cnum5[5];
	//						_itoa_s(iter->second.bit1[num1], cnum1, 10);
	//						_itoa_s(iter->second.bit2[num2], cnum2, 10);
	//						_itoa_s(iter->second.bit3[num3], cnum3, 10);
	//						_itoa_s(iter->second.bit4[num4], cnum4, 10);
	//						_itoa_s(iter->second.bit5[num5], cnum5, 10);
	//						strcat_s(cnum1, 5,"*");
	//						strcat_s(cnum2, 5, "*");
	//						strcat_s(cnum3, 5, "*");
	//						strcat_s(cnum4, 5, "*");
	//						char cnum[40] = { 0 };
	//						strcat_s(cnum, 40, cnum1);
	//						strcat_s(cnum, 40, cnum2);
	//						strcat_s(cnum, 40, cnum3);
	//						strcat_s(cnum, 40, cnum4);
	//						strcat_s(cnum, 40, cnum5);

	//						rc = sqlite3_bind_text(stmt, 1, cnum, -1, SQLITE_TRANSIENT);
	//						rc = sqlite3_bind_int(stmt, 2, iter->first);
	//						sqlite3_step(stmt);     /* Execute the SQL Statement */
	//						sqlite3_clear_bindings(stmt);   /* Clear bindings */
	//						sqlite3_reset(stmt);        /* Reset VDBE */

	//					}
	//				}
	//			}
	//		}
	//	}
	//}
	//sqlite3_exec(db, "commit transaction", 0, 0, &errMsg);
	//sqlite3_finalize(stmt);

	/*auto itercount = _collcounter.begin();
	for (; itercount != _collcounter.end(); ++itercount)
	{
	LOG(itercount->first << "   " << itercount->second);
	}
*/
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
