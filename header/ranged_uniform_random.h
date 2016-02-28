//ranged_uniform_rqndom.h

#ifndef __RANGED_UNIFORM_RANDOM_H
#define __RANGED_UNIFORM_RANDOM_H

#include <boost/random.hpp>
#include <random>
#include <ctime>

namespace gp
{
	class RangedUniformRandom
	{
		private:
			static boost::minstd_rand gen;
			double min, max;
			boost::uniform_real<> dst;
			boost::variate_generator < boost::minstd_rand&, boost::uniform_real<> > random;
		public:
			RangedUniformRandom(double _min,double _max);
			double rand(void);
	};

	template<class T> T rand(T _min, T _max)
	{
		static std::minstd_rand engine(static_cast<unsigned>(time(0)));
		std::uniform_real_distribution<> dist(_min, _max);
		return static_cast<T>(dist(engine));
	}

	template<class ArgType,class ResultType = ArgType> 
	inline ResultType safe_rand(ArgType _lim1, ArgType _lim2)
	{
		if (_lim1 == _lim2){ return static_cast<ResultType>(_lim1); }
		return static_cast<ResultType>(_lim1 < _lim2 ? rand(_lim1, _lim2) : rand(_lim2,_lim1));
	}
}

#endif
