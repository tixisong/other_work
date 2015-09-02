#include "dataitem.h"


Dataitem::Dataitem()
{
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
		if (line.find("Ô¤²â") != std::string::npos)
		{
			analyze_prediction_id(line);
		}
		else if (line.find("1Î»") != std::string::npos)
			continue;
		else
			analyze_value_bit(line);
	}
	combin_to_string_num();
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
			_prediction_id.push_back(std::stoi(sub));
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
	ValueBit valuebit;

	while ((current_pos = (line.find(',', current_pos))) != std::string::npos)
	{
		if (++prediction_id_index % 6 == 0)
		{
			current_pos++;
			pre_pos = current_pos;
			continue;
		}
		sub = line.substr(pre_pos, current_pos - pre_pos);
		int value;
		if (sub.empty())
			value = 0;
		else
			value = std::stoi(sub);
		switch (bit_value_index)
		{
		case 0:
			valuebit.bit1 = value;
			break;
		case 1:
			valuebit.bit2 = value;
			break;
		case 2:
			valuebit.bit3 = value;
			break;
		case 3:
			valuebit.bit4 = value;
			break;
		case 4:
			valuebit.bit5 = value;

			if (valuebit.bit1 != 0 || valuebit.bit2 != 0 || valuebit.bit3 != 0 || valuebit.bit4 != 0 || valuebit.bit5 != 0)
				_colldata.insert(std::make_pair(_prediction_id[prediction_id_index / 6], valuebit));
			bit_value_index = -1;
			memset(&valuebit, 0, sizeof(valuebit));
			break;
		}
		current_pos++;
		pre_pos = current_pos;
		bit_value_index = std::min(++bit_value_index, 4);
	}

	if ((current_pos = line.rfind(',')) != std::string::npos)
	{
		if (current_pos == line.size() - 1)
			valuebit.bit5 = 0;
		else
		{
			sub = line.substr(current_pos + 1);
			valuebit.bit5 = std::stoi(sub);
		}

		if (valuebit.bit1 != 0 || valuebit.bit2 != 0 || valuebit.bit3 != 0 || valuebit.bit4 != 0 || valuebit.bit5 != 0)
			_colldata.insert(std::make_pair(_prediction_id[prediction_id_index / 6], valuebit));
	}

}


void Dataitem::combin_to_string_num()
{
	auto iter = _colldata.begin();
	for (; iter != _colldata.end(); ++iter)
	{
		std::string num1,num2,num3,num4,num5;
		std::string totalnum;

		totalnum += std::to_string(iter->second.bit1);
		totalnum += "*" + std::to_string(iter->second.bit2);
		totalnum += "*" + std::to_string(iter->second.bit3);
		totalnum += "*" + std::to_string(iter->second.bit4);
		totalnum += "*" + std::to_string(iter->second.bit5);

		/*if (iter->second.bit1 != 0)
		{
		totalnum += std::to_string(iter->second.bit1);
		}
		if (iter->second.bit2 != 0)
		{
		totalnum += "*" + std::to_string(iter->second.bit2);
		}
		if (iter->second.bit3 != 0)
		{
		totalnum += "*" + std::to_string(iter->second.bit3);
		}
		if (iter->second.bit4 != 0)
		{
		totalnum += "*" + std::to_string(iter->second.bit4);
		}
		if (iter->second.bit5 != 0)
		{
		totalnum += "*" + std::to_string(iter->second.bit5);
		}*/
		_combine.insert(make_pair(totalnum, iter->first));
	}

	auto combiniter = _combine.begin();
	for (; combiniter != _combine.end(); ++combiniter)
	{
		int counter = _combine.count(combiniter->first);
		if (counter < 3)
			continue;
		_collcounter.insert(std::make_pair(counter, make_pair(combiniter->first,combiniter->second)));
	}

	
}

void Dataitem::prinf_csv()
{
	auto showiter = _collcounter.begin();
	for (; showiter != _collcounter.end(); ++showiter)
	{
		std::cout << showiter->first << "   " << showiter->second.first << " "
			<< showiter->second.second << std::endl;
	}
}
