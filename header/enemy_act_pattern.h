//enemy_act_pattern.h

#ifndef __ENEMY_ACT_PATTERN_H
#define __ENEMY_ACT_PATTERN_H

#include "mob_enemy.h"

namespace MyGameProject
{
	extern MobEnemy::ActPattern default_pattern;
	extern MobEnemy::ActPattern pattern0;
	extern MobEnemy::ActPattern pattern1;
	extern MobEnemy::ActPattern pattern2;
	extern MobEnemy::ActPattern pattern3;
	extern MobEnemy::ActPattern pattern4;
	extern MobEnemy::ActPattern pattern5;
	extern MobEnemy::ActPattern pattern6;
	extern MobEnemy::ActPattern pattern7;
	extern MobEnemy::ActPattern pattern8;

	struct Pattern9Vars;
	struct Pattern10Vars;
	struct Pattern11Vars;
	struct Pattern12Vars;
	struct Pattern13Vars;
	struct Pattern14Vars;
	struct Pattern15Vars;
	struct Pattern16Vars;
	struct Pattern17Vars;
	struct Pattern18Vars;
	struct Pattern19Vars;
	struct Pattern20Vars;
	struct Pattern21Vars;

	//Patterns using member variables.
	struct Pattern { virtual void operator()(MobEnemy& _enemy) = 0; };
	struct Pattern9 : public Pattern { std::shared_ptr<Pattern9Vars>  vars; Pattern9(void); virtual void operator()(MobEnemy& _enemy) override final; }; extern Pattern9   pattern9;
	struct Pattern10 : public Pattern { std::shared_ptr<Pattern10Vars> vars; Pattern10(void); virtual void operator()(MobEnemy& _enemy)override final; }; extern Pattern10 pattern10;
	struct Pattern11 : public Pattern { std::shared_ptr<Pattern11Vars> vars; Pattern11(void); virtual void operator()(MobEnemy& _enemy)override final;}; extern Pattern11 pattern11;
	struct Pattern12 : public Pattern { std::shared_ptr<Pattern12Vars> vars; Pattern12(void); virtual void operator()(MobEnemy& _enemy)override final;}; extern Pattern12 pattern12;
	struct Pattern13 : public Pattern { std::shared_ptr<Pattern13Vars> vars; Pattern13(void); virtual void operator()(MobEnemy& _enemy)override final;}; extern Pattern13 pattern13;
	struct Pattern14 : public Pattern { std::shared_ptr<Pattern14Vars> vars; Pattern14(void); virtual void operator()(MobEnemy& _enemy)override final;}; extern Pattern14 pattern14;
	struct Pattern15 : public Pattern { std::shared_ptr<Pattern15Vars> vars; Pattern15(void); virtual void operator()(MobEnemy& _enemy)override final;}; extern Pattern15 pattern15;
	struct Pattern16 : public Pattern { std::shared_ptr<Pattern16Vars> vars; Pattern16(void); virtual void operator()(MobEnemy& _enemy)override final;}; extern Pattern16 pattern16;
	struct Pattern17 : public Pattern { std::shared_ptr<Pattern17Vars> vars; Pattern17(void); virtual void operator()(MobEnemy& _enemy)override final;}; extern Pattern17 pattern17;
	struct Pattern18 : public Pattern { std::shared_ptr<Pattern18Vars> vars; Pattern18(void); virtual void operator()(MobEnemy& _enemy)override final;}; extern Pattern18 pattern18;
	struct Pattern19 : public Pattern { std::shared_ptr<Pattern19Vars> vars; Pattern19(void); virtual void operator()(MobEnemy& _enemy)override final;}; extern Pattern19 pattern19;
	struct Pattern20 : public Pattern { std::shared_ptr<Pattern20Vars> vars; Pattern20(void); virtual void operator()(MobEnemy& _enemy)override final;}; extern Pattern20 pattern20;
	struct Pattern21 : public Pattern { std::shared_ptr<Pattern21Vars> vars; Pattern21(void); virtual void operator()(MobEnemy& _enemy)override final;}; extern Pattern21 pattern21;

	MobEnemy::ActPattern pattern100(const std::vector<Point2D>& _args);
	MobEnemy::ActPattern pattern101(const std::vector<Point2D>& _args);
	MobEnemy::ActPattern pattern102(const std::vector<Point2D>& _args);
	MobEnemy::ActPattern pattern103(const std::vector<Point2D>& _args);
	MobEnemy::ActPattern pattern104(const std::vector<Point2D>& _args);
	MobEnemy::ActPattern pattern105(const std::vector<Point2D>& _args);
	MobEnemy::ActPattern pattern106(const std::vector<Point2D>& _args);
	MobEnemy::ActPattern pattern107(const std::vector<Point2D>& _args);
}

#endif
