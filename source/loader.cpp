//loader.cpp

#include "play_game.h"
#include "player.h"
#include "shot.h"
#include "enemy.h"
#include "bullet.h"
#include "item.h"
#include "player_health_gage.h"
#include "destroy_enemy_rate.h"
#include "score_drawer.h"
#include "stage0.h"
#include "stage3.h"
#include "se_manager.h"
#include "concrete_back_ground.h"

namespace MyGameProject
{
	void PlayGame::load(void)
	{
		Player::preperation();
		Shot::preperation();
		Pseud3D::preperation();
		ThroughCloud::preperation();
		Enemy::preperation();
		Bullet::preperation();
		Item::preperation();
		Stage0BG::preparation();
		Stage3BG::preparation();
		PlayerHealthGage::preparation();
		DestroyEnemyRate::load_digit_images();
		BaseRateDrawer::preparation();
		ScoreDrawer::preparation();

		if (se_manager)
		{
			se_manager->add("../../data/sound/lock_option.wav");
			se_manager->add("../../data/sound/shot_down.wav");
			se_manager->add("../../data/sound/key-in1.mp3");
			se_manager->add("../../data/sound/meka_ge_kaseto_deki_botan03.mp3");
			se_manager->add("../../data/sound/shot.mp3");
			se_manager->add("../../data/sound/shot2.wav");
			se_manager->add("../../data/sound/enemy_shot.wav");
			se_manager->add("../../data/sound/enemy_shot_01.wav");
			se_manager->add("../../data/sound/enemy_shot_02.wav");
			se_manager->add("../../data/sound/enemy_shot_03.wav");
			se_manager->add("../../data/sound/magic_wave3.mp3");
			se_manager->add("../../data/sound/ata_a27.mp3");
			se_manager->add("../../data/sound/se-033a.mp3");
			se_manager->add("../../data/sound/Clock-Second_Hand01-1L.mp3");
			se_manager->add("../../data/sound/sei_ge_garasu_hibi02.mp3");
			se_manager->add("../../data/sound/button04b.mp3");
			se_manager->add("../../data/sound/button04a.mp3");
			se_manager->add("../../data/sound/click.mp3");
			se_manager->add("../../data/sound/glass-break3.mp3");
			se_manager->add("../../data/sound/attack2.mp3");
			se_manager->add("../../data/sound/laser_released.wav");
			se_manager->add("../../data/sound/item_get.wav");
			se_manager->add("../../data/sound/crow.wav");
			se_manager->add("../../data/sound/rocket_going.wav");
			se_manager->add("../../data/sound/laser.wav");
			se_manager->add("../../data/sound/laser2.wav");
			se_manager->add("../../data/sound/stroke.wav");
			se_manager->add("../../data/sound/launcher1.mp3");
			se_manager->add("../../data/sound/dive.wav");
			se_manager->add("../../data/sound/dive2.wav");
		}
	}
}