//image_pool.cpp

#include "image_pool.h"
#include <dxlib.h>
#include <boost/lexical_cast.hpp>

void MyGameProject::ImagePool::add(const std::string& _name)
{
	data().insert({ _name, LoadGraph(_name.c_str()) });
}

void MyGameProject::ImagePool::add_multi(const std::string& _name, int _all_num, int _x_num, int _y_num, int _x_size, int _y_size)
{
	std::vector<int> v; v.resize(_all_num);
	LoadDivGraph(_name.c_str(),_all_num,_x_num,_y_num,_x_size,_y_size,v.data());
	for (int i = 0; i < _all_num;++i)
	{
		//First number is 0, and last number is (_all_num - 1)
		data().insert({_name + "@" + boost::lexical_cast<std::string>(i),v[i]});
	}
}

int MyGameProject::ImagePool::get(const std::string& _name)
{
	return data()[_name];
}

void MyGameProject::ImagePool::clear(void)
{
	data().clear();
	InitGraph();
}