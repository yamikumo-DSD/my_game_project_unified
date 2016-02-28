//ranged_uniform_random.cpp

#include "ranged_uniform_random.h"

boost::minstd_rand gp::RangedUniformRandom::gen(static_cast<unsigned int>(time(0)));

gp::RangedUniformRandom::RangedUniformRandom(double _min, double _max) :min(_min), max(_max),dst(_min,_max),random(gen,dst){}

double gp::RangedUniformRandom::rand(void){ return random(); }
