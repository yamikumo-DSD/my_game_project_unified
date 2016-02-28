//image_pool.h

#ifndef __IMAGE_POOL_H
#define __IMAGE_POOL_H

#include <unordered_map>
#include <string>

namespace MyGameProject
{
	class ImagePool
	{
	private:
		static std::unordered_map<std::string, int>& data(void)
		{
			static std::unordered_map<std::string, int> d; 
			return d;
		}
	public:
		static void add(const std::string& _name);
		static void add_multi(const std::string& _name,int _all_num,int _x_num,int _y_num,int _x_size,int _y_size);
		static int get(const std::string& _name);
		static void clear(void);
	};
}

#endif