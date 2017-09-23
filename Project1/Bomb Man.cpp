#include "graphics.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#define KEY_DOWN(vk_c) (GetAsyncKeyState(vk_c) & 0x8000 ? 1 : 0)  //按键判断
#define LEFT 1
#define RIGHT 2
#define DOWN 3
#define UP 4
#define MONSPEED 6
#define Door 4
#define BOOST 5
#define CROSS_BOMB 6
#define CROSS_WALL 7
#define BOMB_IGNORE 8
#define ADJUST      3

#pragma comment(lib,"Winmm.lib")

void Menu_show();
void init_map_show();
void map_show();
void pause_menu();
void pause_back();
void prop_show();
void bomb_show();
void init_monster_show();
void monster_show_left(int i);
void monster_show_right(int i);
void monster_death_show();
void Character_show();
void Character_show_down();
void Character_show_up();
void Character_show_left();
void Character_show_right();
void character_death_show();                           //用来显示游戏中的每一种画面


void startup_music();
void startup_map_show();
void startup_character_show();
void startup_monster_show();
void startup_bomb_show();
void startup_menu_show();
void startup_prop_show();            //加载游戏所需的所有图片及音频素材

void ismonsterdeath();
void ismandeath();
void isprop();                 //用来判定人物与怪物是否死亡，及人物是否吃到道具


void operate();
void TimerEventProcess(int timerID);
void KeyboardEventProcess(int key, int event);                 //键盘监听及计时器函数


typedef struct Monster
{
	POINT Monster_position;
	int direction;
	int Mcount;
	int Deathcount;
	int death;
} Monster;       //记录每个怪物信息
typedef struct PROP {
	int isCross_Bomb;
	int isCross_Wall;
	int isBomb_ignore;
}PROP;            //记录每种道具信息

int Man_Count = 1;                     //人物行走动态图片显示计数
int Man_death_count;                  //人物死亡动态图片显示计数
int max_monster;                        //记录怪物数，用于循环遍历
int sum_monster;                        //记录场上现存怪物数，用于胜利判定
int Bomb_Count;                           //炸弹动态图片显示计数
int pause;                                //判断是否处于菜单界面
int turn;                                //调整菜单显示选项
int victory;                             //判断游戏是否胜利
int Direction = DOWN;                        //存储人物当前方向
int press = 0;                           //记录暂停界面下是否按下执行键
int randturn = 0;                         //怪物行走的随机时机调控
int music_switch = 0;                     //判断是否按下静音键
int isdeath;                              //判断人物是否死亡
int SPEED = 6;                              //人物行走速度/步长

int map[931][661];       //标记地图上的每一个点
int Brinkput;           //地图随机生成完成的标志
int Brink_Broke_up, Brink_Broke_left, Brink_Broke_down, Brink_Broke_right, Brink_Broke;              //是否摧毁砖块标志
int Start;         //开始，进行和结束界面的调控
POINT Character, Bomb, pausemenu_position;    //记录人物、炸弹及暂停菜单的坐标
Monster monster[23];
PROP prop;

ACL_Image Man_dawn_1, Man_dawn_2, Man_dawn_3, Man_up_1, Man_up_2, Man_up_3,
Man_Left_1, Man_Left_2, Man_Left_3, Man_Right_1, Man_Right_2, Man_Right_3;

ACL_Image Man_Death_1, Man_Death_2, Man_Death_3, Man_Death_4, Man_Death_5, Man_Death_6;

ACL_Image Monster_left_1, Monster_left_2, Monster_left_3, Monster_right_1, Monster_right_2, Monster_right_3,
Monster_death_1, Monster_death_2, Monster_death_3, Monster_death_4, Monster_death_5;

ACL_Image Map_sign, door, Information, background, brick, stone, Brink_Broke_1, Brink_Broke_2, Brink_Broke_3, Brink_Broke_4, Brink_Broke_5, Brink_Broke_6;

ACL_Image Bomb_1, Bomb_2, Bomb_3, bomb_m_1, bomb_n_1, bomb_s_1, bomb_w_1, bomb_e_1;

ACL_Sound Main_sound, Main2_sound, begin_music, Start_Button, pushBomb, explosion, pause_button, vic, man_death, defeat, propSound;

ACL_Image Start_Menu, Transition, Pause_home, Pause_resume, Pause_music, Pause_musicoff, Victory, Defeat;

ACL_Image Boost, Cross_Bomb, Cross_Wall, Bomb_ignore;                               //定义所有图片和音频指针

int Main()
{
	initWindow("Bomb Man", DEFAULT, DEFAULT, 930, 660);
	srand((unsigned)time(NULL));
	startup_menu_show();
	startup_map_show();
	startup_character_show();
	startup_monster_show();
	startup_bomb_show();
	startup_prop_show();
	startup_music();
	playSound(begin_music, 1);
	registerTimerEvent(TimerEventProcess);
	registerKeyboardEvent(KeyboardEventProcess);/*注册键盘消息回调函数*/
	startTimer(0, 50);
	return 0;
}

void startup_character_show()
{
	Character.x = 30;
	Character.y = 60;          //初始化人物坐标

	loadImage("resource\\Man_dawn_1.bmp", &Man_dawn_1);
	loadImage("resource\\Man_dawn_2.bmp", &Man_dawn_2);
	loadImage("resource\\Man_dawn_3.bmp", &Man_dawn_3);
	loadImage("resource\\Man_up_1.bmp", &Man_up_1);
	loadImage("resource\\Man_up_2.bmp", &Man_up_2);
	loadImage("resource\\Man_up_3.bmp", &Man_up_3);
	loadImage("resource\\Man_Left_1.bmp", &Man_Left_1);
	loadImage("resource\\Man_Left_2.bmp", &Man_Left_2);
	loadImage("resource\\Man_Left_3.bmp", &Man_Left_3);
	loadImage("resource\\Man_Right_1.bmp", &Man_Right_1);
	loadImage("resource\\Man_Right_2.bmp", &Man_Right_2);
	loadImage("resource\\Man_Right_3.bmp", &Man_Right_3);

	loadImage("resource\\Man_Death_1.bmp", &Man_Death_1);
	loadImage("resource\\Man_Death_2.bmp", &Man_Death_2);
	loadImage("resource\\Man_Death_3.bmp", &Man_Death_3);
	loadImage("resource\\Man_Death_4.bmp", &Man_Death_4);
	loadImage("resource\\Man_Death_5.bmp", &Man_Death_5);
	loadImage("resource\\Man_Death_6.bmp", &Man_Death_6);
}

void startup_monster_show()
{
	loadImage("resource\\Monster_left_1.bmp", &Monster_left_1);
	loadImage("resource\\Monster_left_2.bmp", &Monster_left_2);
	loadImage("resource\\Monster_left_3.bmp", &Monster_left_3);
	loadImage("resource\\Monster_right_1.bmp", &Monster_right_1);
	loadImage("resource\\Monster_right_2.bmp", &Monster_right_2);
	loadImage("resource\\Monster_right_3.bmp", &Monster_right_3);
	loadImage("resource\\Monster_death_1.bmp", &Monster_death_1);
	loadImage("resource\\Monster_death_2.bmp", &Monster_death_2);
	loadImage("resource\\Monster_death_3.bmp", &Monster_death_3);
	loadImage("resource\\Monster_death_4.bmp", &Monster_death_4);
	loadImage("resource\\Monster_death_5.bmp", &Monster_death_5);
}

void startup_map_show()
{
	loadImage("resource\\Map_sign.bmp", &Map_sign);
	loadImage("resource\\Information.bmp", &Information);

	loadImage("resource\\background.bmp", &background);
	loadImage("resource\\brick.bmp", &brick);
	loadImage("resource\\stone.bmp", &stone);

	loadImage("resource\\Brink_Broke_1.bmp", &Brink_Broke_1);
	loadImage("resource\\Brink_Broke_2.bmp", &Brink_Broke_2);
	loadImage("resource\\Brink_Broke_3.bmp", &Brink_Broke_3);
	loadImage("resource\\Brink_Broke_4.bmp", &Brink_Broke_4);
	loadImage("resource\\Brink_Broke_5.bmp", &Brink_Broke_5);
	loadImage("resource\\Brink_Broke_6.bmp", &Brink_Broke_6);       // //地图素材加载

	loadImage("resource\\victory.bmp", &Victory);
	loadImage("resource\\Defeat.bmp", &Defeat);
}

void startup_prop_show()
{
	loadImage("resource\\Boost.bmp", &Boost);
	loadImage("resource\\Cross_Bomb.bmp", &Cross_Bomb);
	loadImage("resource\\Cross_Wall.bmp", &Cross_Wall);
	loadImage("resource\\Bomb_ignore.bmp", &Bomb_ignore);
	loadImage("resource\\Door.bmp", &door);
}

void startup_menu_show()
{
	pausemenu_position.x = 191;
	pausemenu_position.y = -360;

	loadImage("resource\\Start_Menu.bmp", &Start_Menu);
	loadImage("resource\\Transition.bmp", &Transition);

	loadImage("resource\\pause_menu_home.bmp", &Pause_home);
	loadImage("resource\\pause_menu_resume.bmp", &Pause_resume);
	loadImage("resource\\pause_menu_music.bmp", &Pause_music);
	loadImage("resource\\pause_menu_music_off.bmp", &Pause_musicoff);
}

void startup_bomb_show()
{
	loadImage("resource\\Bomb_1.bmp", &Bomb_1);
	loadImage("resource\\Bomb_2.bmp", &Bomb_2);
	loadImage("resource\\Bomb_3.bmp", &Bomb_3);

	loadImage("resource\\bomb_m_1.bmp", &bomb_m_1);
	loadImage("resource\\bomb_n_1.bmp", &bomb_n_1);
	loadImage("resource\\bomb_s_1.bmp", &bomb_s_1);
	loadImage("resource\\bomb_w_1.bmp", &bomb_w_1);
	loadImage("resource\\bomb_e_1.bmp", &bomb_e_1);       //炸弹素材加载
}

void startup_music()
{
	loadSound("resource\\begin_music.wav", &begin_music);
	loadSound("resource\\Start_Button.wav", &Start_Button);
	loadSound("resource\\main_music.wav", &Main_sound);
	loadSound("resource\\Main2_sound.wav", &Main2_sound);
	loadSound("resource\\pushBomb.wav", &pushBomb);
	loadSound("resource\\pause_button.wav", &pause_button);
	loadSound("resource\\explosion.wav", &explosion);
	loadSound("resource\\vic.wav", &vic);
	loadSound("resource\\man_death.wav", &man_death);
	loadSound("resource\\defeat.wav", &defeat);
	loadSound("resource\\propSound.wav", &propSound);
}

void Menu_show()
{	
	playSound(begin_music, 1);

	while (1)
	{
		if (KEY_DOWN(13))
			break;
	}

	stopSound(begin_music);
}

void init_map_show()
{
	
	int i, j, p, brink_x, q, sum_brink, door, boost, cross_bomb, cross_wall, bomb_ignore;

	sum_brink = 0;           //用于统计砖块个数

	putImage(&Map_sign, 0, 0);
	putImage(&Information, 55, 0);

	for (i = 1; i < 30; i++)
	{
		for (j = 2; j < 21; j++)
		{
			putImage(&background, i * 30, j * 30);
		}
	}     //生成地板
	
	for (j = 1; j < 22; j++)
	{
		putImage(&stone, 0, 30 * j);
		putImage(&stone, 900, 30 * j);
		
		for (p = 0; p <= 30; p++)
			for (q = 0; q <= 30; q++)
			{
				map[p][30 * j + q] = 1;
				map[900 + p][30 * j + q] = 1;
			} //石墙标定
	}

	for (i = 1; i < 30; i++)
	{
		putImage(&stone, 30 * i, 30);
		putImage(&stone, 30 * i, 630);
		for (p = 0; p <= 30; p++)
			for (q = 0; q <= 30; q++)
			{
				map[30 * i + p][30 + q] = 1;
				map[30 * i + p][630 + q] = 1;
			} //石墙标定
	}
	
	for (j = 0; j < 9; j++)
	{
		for (i = 1; i < 15; i++)
		{
			putImage(&stone, 60 * i, 90 + 60 * j);
			for (p = 0; p <= 30; p++)
				for (q = 0; q <= 30; q++)
				{
					map[60 * i + p][90 + 60 * j + q] = 1;
				} //石墙标定
		}
	} //生成石墙
	
		for (j = 0; j < 19; j++)
		{
			brink_x = rand() % 7;
			if (j % 2 == 0) {
				for (; brink_x >= 0; brink_x--)
				{
					i = 30 * (rand() % 28 + 1);
					if ((i == 30 || i == 60) && j == 0);
					else
					{
						putImage(&brick, i, 60 + 30 * j);
						map[i][60 + 30 * j] = 2;
						for (p = 1; p < 30; p++)
							for (q = 1; q < 30; q++)
							{
								map[i + p][60 + 30 * j + q] = 2;
							} //砖块标定
					}
				}
			}
			else
			{
				for (; brink_x >= 0; brink_x--)
				{
					i = 30 + 60 * (rand() % 15);
					if (i == 30 && j == 1);
					else
					{
						putImage(&brick, i, 60 + 30 * j);
						map[i][60 + 30 * j] = 2;
						for (p = 1; p < 30; p++)
							for (q = 1; q < 30; q++)
							{
								map[i + p][60 + 30 * j + q] = 2;
							}   //砖块标定
					}
				}
			}
		} //随机生成砖块
		Brinkput = 1;      //标记砖块生成完毕
		
		for (i = 1; i < 30; i++)
		{
			for (j = 2; j < 21; j++)
			{
				if (map[i * 30][j * 30] == 2)
				{
					sum_brink++;
				}
			}
		}        //统计砖块个数
		
		door = rand() % sum_brink + 1;
		for (boost = rand() % sum_brink + 1; boost == door; boost = rand() % sum_brink + 1);
		for (cross_bomb = rand() % sum_brink + 1; cross_bomb == door || cross_bomb == boost; cross_bomb = rand() % sum_brink + 1);
		for (cross_wall = rand() % sum_brink + 1; cross_wall == door || cross_wall == boost || cross_wall == cross_bomb; cross_wall = rand() % sum_brink + 1);
		for (bomb_ignore = rand() % sum_brink + 1; bomb_ignore == door || bomb_ignore == boost || bomb_ignore == cross_bomb || bomb_ignore == cross_wall; bomb_ignore = rand() % sum_brink + 1);
		for (i = 1; i < 30; i++)
		{
			for (j = 2; j < 21; j++)
			{
				if (map[i * 30][j * 30] == 2)
				{
					door--;
					boost--;
					cross_bomb--;
					cross_wall--;
					bomb_ignore--;
					if (door == 0)
					{
						map[i * 30 + 15][j * 30 + 15] = Door;
					}
					if (boost == 0)
					{
						map[i * 30 + 15][j * 30 + 15] = BOOST;
					}
					if (cross_bomb == 0)
					{
						map[i * 30 + 15][j * 30 + 15] = CROSS_BOMB;
					}
					if (cross_wall == 0)
					{
						map[i * 30 + 15][j * 30 + 15] = CROSS_WALL;
					}
					if (bomb_ignore == 0)
					{
						map[i * 30 + 15][j * 30 + 15] = BOMB_IGNORE;
					}
				}
			}
		}        //随机生成道具的位置
}

void init_monster_show()
{
	int i, j, moneter_count;

	//srand((unsigned)time(NULL));

	for (j = 0; j < 19; j++)
	{
		moneter_count = rand() % 3;     //随机每行怪物的个数为0~2
		if (j % 2 == 0) {
			for (; moneter_count > 0; moneter_count--)
			{
				i = 30 * (rand() % 28 + 1);
				if (((i == 30 || i == 60) && j == 0) || (map[i][60 + 30 * j] == 2));  //判断是否在人物的初始位置或砖块的位置
				else
				{
					max_monster++;   //怪物初始序号为1
					putImage(&Monster_left_1, i, 60 + 30 * j);
					monster[max_monster].Monster_position.x = i;
					monster[max_monster].Monster_position.y = 60 + 30 * j;
				}
			}
		}
	} //随机生成怪物
	sum_monster = max_monster;
}

void monster_show_left(int i)
{
	if (monster[i].Mcount == 18)
	{
		monster[i].Mcount = 0;
	}
	
	if (monster[i].Mcount >= 0 && monster[i].Mcount <= 5)
	{
		putImageTransparent(&Monster_left_1, monster[i].Monster_position.x, monster[i].Monster_position.y, 30, 30, RGB(0, 170, 1));
		monster[i].Mcount++;
	}
	if (monster[i].Mcount >= 6 && monster[i].Mcount <= 11)
	{
		putImageTransparent(&Monster_left_2, monster[i].Monster_position.x, monster[i].Monster_position.y, 30, 30, RGB(0, 170, 1)); 
		monster[i].Mcount++;
	}
	if (monster[i].Mcount >= 12 && monster[i].Mcount <= 17)
	{
		putImageTransparent(&Monster_left_3, monster[i].Monster_position.x, monster[i].Monster_position.y, 30, 30, RGB(0, 170, 1));
		monster[i].Mcount++;
	}
}

void monster_show_right(int i)
{
	if (monster[i].Mcount == 18)
	{
		monster[i].Mcount = 0;
	}
	if (monster[i].Mcount >= 0 && monster[i].Mcount <= 5)
	{
		putImageTransparent(&Monster_right_1, monster[i].Monster_position.x, monster[i].Monster_position.y, 30, 30, RGB(0, 170, 1));
		monster[i].Mcount++;
	}
	if (monster[i].Mcount >= 6 && monster[i].Mcount <= 11)
	{
		putImageTransparent(&Monster_right_2, monster[i].Monster_position.x, monster[i].Monster_position.y, 30, 30, RGB(0, 170, 1));
		monster[i].Mcount++;
	}
	if (monster[i].Mcount >= 12 && monster[i].Mcount <= 17)
	{
		putImageTransparent(&Monster_right_3, monster[i].Monster_position.x, monster[i].Monster_position.y, 30, 30, RGB(0, 170, 1));
		monster[i].Mcount++;
	}
}

void monster_death_show()
{
	int i;

	for (i = 1; i <= max_monster; i++)
	{
		if (monster[i].death == 1)
		{
			switch (monster[i].Deathcount)
			{
			case 0: putImageTransparent(&Monster_death_1, monster[i].Monster_position.x, monster[i].Monster_position.y, 30, 30, RGB(0, 170, 1)); monster[i].Deathcount++; break;
			case 1: putImageTransparent(&Monster_death_2, monster[i].Monster_position.x, monster[i].Monster_position.y, 30, 30, RGB(0, 170, 1)); monster[i].Deathcount++; break;
			case 2: putImageTransparent(&Monster_death_3, monster[i].Monster_position.x, monster[i].Monster_position.y, 30, 30, RGB(0, 170, 1)); monster[i].Deathcount++; break;
			case 3: putImageTransparent(&Monster_death_4, monster[i].Monster_position.x, monster[i].Monster_position.y, 30, 30, RGB(0, 170, 1)); monster[i].Deathcount++; break;
			case 4: putImageTransparent(&Monster_death_5, monster[i].Monster_position.x, monster[i].Monster_position.y, 30, 30, RGB(0, 170, 1)); monster[i].Deathcount++; sum_monster--; break;
			}
		}
	}
}

void map_show()
{
	int i, j;

	putImage(&Map_sign, 0, 0);
	putImage(&Information, 55, 0);    //生成上方计分菜单

	for (i = 1; i < 30; i++)
	{
		for (j = 2; j < 21; j++)
		{
			putImage(&background, i * 30, j * 30);
		}
	}     //生成地板

	for (j = 1; j < 22; j++)
	{
		putImage(&stone, 0, 30 * j);
		putImage(&stone, 900, 30 * j);
	}

	for (i = 1; i < 30; i++)
	{
		putImage(&stone, 30 * i, 30);
		putImage(&stone, 30 * i, 630);
	}

	for (j = 0; j < 9; j++)
	{
		for (i = 1; i < 15; i++)
		{
			putImage(&stone, 60 * i, 90 + 60 * j);
		}
	} //生成石墙
	
	prop_show();

	for (i = 1; i < 30; i++)
	{
		for (j = 2; j < 21; j++)
		{
			if (map[i * 30][j * 30] == 2)
			{
				putImage(&brick, i * 30, j * 30);
			}
		}
	}        //持续贴出已经生成的砖块
}

void prop_show()
{
	int i, j;
	
	for (i = 1; i < 30; i++)
	{
		for (j = 2; j < 21; j++)
		{
			if (map[i * 30 + 15][j * 30 + 15] == Door)
			{
				putImage(&door, i * 30, j * 30);
			}
			if (map[i * 30 + 15][j * 30 + 15] == BOOST)
			{
				putImage(&Boost, i * 30, j * 30);
			}
			if (map[i * 30 + 15][j * 30 + 15] == CROSS_BOMB)
			{
				putImage(&Cross_Bomb, i * 30, j * 30);
			}
			if (map[i * 30 + 15][j * 30 + 15] == CROSS_WALL)
			{
				putImage(&Cross_Wall, i * 30, j * 30);
			}
			if (map[i * 30 + 15][j * 30 + 15] == BOMB_IGNORE)
			{
				putImage(&Bomb_ignore, i * 30, j * 30);
			}
		}
	}       
}

void Character_show_down()
{
	if (Man_Count == 4)
	{
		Man_Count = 1;
	}
	switch (Man_Count)
	{
	case 1: putImageTransparent(&Man_dawn_1, Character.x, Character.y, 30, 30, RGB(0, 170, 1)); Man_Count++; break;
	case 2: putImageTransparent(&Man_dawn_2, Character.x, Character.y, 30, 30, RGB(0, 170, 1)); Man_Count++; break;
	case 3: putImageTransparent(&Man_dawn_3, Character.x, Character.y, 30, 30, RGB(0, 170, 1)); Man_Count++; break;
	}

}

void Character_show_up()
{
	if (Man_Count == 4)
	{
		Man_Count = 1;
	}
	switch (Man_Count)
	{
	case 1: putImageTransparent(&Man_up_1, Character.x, Character.y, 30, 30, RGB(0, 170, 1)); Man_Count++; break;
	case 2: putImageTransparent(&Man_up_2, Character.x, Character.y, 30, 30, RGB(0, 170, 1)); Man_Count++; break;
	case 3: putImageTransparent(&Man_up_3, Character.x, Character.y, 30, 30, RGB(0, 170, 1)); Man_Count++; break;
	}

}

void Character_show_left()
{
	if (Man_Count == 4)
	{
		Man_Count = 1;
	}
	switch (Man_Count)
	{
	case 1: putImageTransparent(&Man_Left_1, Character.x, Character.y, 30, 30, RGB(0, 170, 1)); Man_Count++; break;
	case 2: putImageTransparent(&Man_Left_2, Character.x, Character.y, 30, 30, RGB(0, 170, 1)); Man_Count++; break;
	case 3: putImageTransparent(&Man_Left_3, Character.x, Character.y, 30, 30, RGB(0, 170, 1)); Man_Count++; break;
	}
}

void Character_show_right()
{
	if (Man_Count == 4)
	{
		Man_Count = 1;
	}
	switch (Man_Count)
	{
	case 1: putImageTransparent(&Man_Right_1, Character.x, Character.y, 30, 30, RGB(0, 170, 1)); Man_Count++; break;
	case 2: putImageTransparent(&Man_Right_2, Character.x, Character.y, 30, 30, RGB(0, 170, 1)); Man_Count++; break;
	case 3: putImageTransparent(&Man_Right_3, Character.x, Character.y, 30, 30, RGB(0, 170, 1)); Man_Count++; break;
	}
}

void Character_show()
{
	switch (Direction)
	{
	case 1: putImageTransparent(&Man_Left_1, Character.x, Character.y, 30, 30, RGB(0, 170, 1)); break;
	case 2: putImageTransparent(&Man_Right_1, Character.x, Character.y, 30, 30, RGB(0, 170, 1)); break;
	case 3: putImageTransparent(&Man_dawn_1, Character.x, Character.y, 30, 30, RGB(0, 170, 1)); break;
	case 4: putImageTransparent(&Man_up_1, Character.x, Character.y, 30, 30, RGB(0, 170, 1)); break;
	}
}

void character_death_show()
{
	if (isdeath == 1)
	{
		switch (Man_death_count)
		{
		case 0: putImageTransparent(&Man_Death_1, Character.x, Character.y, 30, 30, RGB(0, 170, 1)); Man_death_count++; stopSound(Main_sound); playSound(man_death, 0); break;
		case 1: putImageTransparent(&Man_Death_2, Character.x, Character.y, 30, 30, RGB(0, 170, 1)); Man_death_count++; break;
		case 2: putImageTransparent(&Man_Death_3, Character.x, Character.y, 30, 30, RGB(0, 170, 1)); Man_death_count++; break;
		case 3: putImageTransparent(&Man_Death_4, Character.x, Character.y, 30, 30, RGB(0, 170, 1)); Man_death_count++; break;
		case 4: putImageTransparent(&Man_Death_5, Character.x, Character.y, 30, 30, RGB(0, 170, 1)); Man_death_count++; break;
		case 5: putImageTransparent(&Man_Death_6, Character.x, Character.y, 30, 30, RGB(0, 170, 1)); Man_death_count++; isdeath = 2; break;
		}
	}
}

void bomb_show()
{
	int i, j, p, q;

	if (Bomb_Count == 44)
	{
		Bomb_Count = 0;
		for (p = 1; p < 30; p++)
			for (q = 1; q < 30; q++)
			{
				map[Bomb.x + p][Bomb.y + q] = 0;
			} //炸弹标定解除
	}

	if (Bomb_Count >= 1 && Bomb_Count <= 12)
	{
		putImageTransparent(&Bomb_1, Bomb.x, Bomb.y, 30, 30, RGB(0, 170, 1)); 
		Bomb_Count++;
	}

	if (Bomb_Count >= 13 && Bomb_Count <= 24)
	{
		putImageTransparent(&Bomb_2, Bomb.x, Bomb.y, 30, 30, RGB(0, 170, 1));
		Bomb_Count++;
	}

	if (Bomb_Count >= 25 && Bomb_Count <= 36)
	{
		putImageTransparent(&Bomb_3, Bomb.x, Bomb.y, 30, 30, RGB(0, 170, 1));
		Bomb_Count++;
	}   //动态炸弹生成

	if (Bomb_Count >= 37 &&  Bomb_Count <= 43)
	{
		if(Bomb_Count == 37)
			playSound(explosion, 0);

		if (map[Bomb.x + 1][Bomb.y - 5 * (Bomb_Count - 37)] != 1)    //判定火焰是否遇到石墙，下同
		{
			if (map[Bomb.x][Bomb.y - 30] == 2)
			{
				for (i = Bomb.x + 1; i < Bomb.x + 30; i++)
					for (j = Bomb.y - 29; j < Bomb.y; j++)
					{
						if (map[i][j] == 2)   
							map[i][j] = 0;
					}
				map[Bomb.x][Bomb.y - 30] = 0;
				Brink_Broke_up = 1;
			}
			putImageTransparent(&bomb_n_1, Bomb.x, Bomb.y - 5 * (Bomb_Count - 37), 30, 30, RGB(0, 170, 1));
		}   //砖块标记坐标初始化

		if (map[Bomb.x + 1][Bomb.y + 5 * (Bomb_Count - 37) + 30] != 1)
		{
			if (map[Bomb.x][Bomb.y + 30] == 2)
			{
				for (i = Bomb.x + 1; i < Bomb.x + 30; i++)
					for (j = Bomb.y + 31; j < Bomb.y + 60; j++)
					{
						if (map[i][j] == 2)
							map[i][j] = 0;
					}
				
				map[Bomb.x][Bomb.y + 30] = 0;
				
				Brink_Broke_down = 1;
			}
			putImageTransparent(&bomb_s_1, Bomb.x, Bomb.y + 5 * (Bomb_Count - 37), 30, 30, RGB(0, 170, 1));
		}    //砖块标记坐标初始化

		
		if (map[Bomb.x - 5 * (Bomb_Count - 37)][Bomb.y + 1] != 1)
		{
			if (map[Bomb.x - 30][Bomb.y] == 2)
			{
				for (i = Bomb.x - 29; i < Bomb.x; i++)
					for (j = Bomb.y + 1; j < Bomb.y + 30; j++)
					{
						if (map[i][j] == 2)
							map[i][j] = 0;
					}

				map[Bomb.x - 30][Bomb.y] = 0;

				Brink_Broke_left = 1;
			}   

			putImageTransparent(&bomb_w_1, Bomb.x - 5 * (Bomb_Count - 37), Bomb.y, 30, 30, RGB(0, 170, 1));

		}     //砖块标记坐标初始化

		if (map[Bomb.x + 5 * (Bomb_Count - 37) + 30][Bomb.y + 1] != 1)
		{
			if (map[Bomb.x + 30][Bomb.y] == 2)
			{
				for (i = Bomb.x + 31; i < Bomb.x + 60; i++)
					for (j = Bomb.y + 1; j < Bomb.y + 30; j++)
					{
						if (map[i][j] == 2)
							map[i][j] = 0;
					}

				map[Bomb.x + 30][Bomb.y] = 0;

				Brink_Broke_right = 1;
			}

			putImageTransparent(&bomb_e_1, Bomb.x + 5 * (Bomb_Count - 37), Bomb.y, 30, 30, RGB(0, 170, 1));
		}

		putImageTransparent(&bomb_m_1, Bomb.x, Bomb.y, 30, 30, RGB(0, 170, 1));
		if (map[Bomb.x][Bomb.y] == 2)
		{
			for (i = Bomb.x + 1; i < Bomb.x + 30; i++)
				for (j = Bomb.y + 1; j < Bomb.y + 30; j++)
				{
					if (map[i][j] == 2)
						map[i][j] = 0;   //防止擦除掉道具
				}

			map[Bomb.x][Bomb.y] = 0;

			Brink_Broke = 1;
		}               //砖块标记坐标初始化 

		if (Brink_Broke_up)
		{
			switch (Bomb_Count)
			{
			case 38: putImageTransparent(&Brink_Broke_1, Bomb.x, Bomb.y - 30, 30, 30, RGB(0, 170, 1)); break;
			case 39: putImageTransparent(&Brink_Broke_2, Bomb.x, Bomb.y - 30, 30, 30, RGB(0, 170, 1)); break;
			case 40: putImageTransparent(&Brink_Broke_4, Bomb.x, Bomb.y - 30, 30, 30, RGB(0, 170, 1)); break;
			case 41: putImageTransparent(&Brink_Broke_5, Bomb.x, Bomb.y - 30, 30, 30, RGB(0, 170, 1)); break;
			case 42: putImageTransparent(&Brink_Broke_4, Bomb.x, Bomb.y - 30, 30, 30, RGB(0, 170, 1)); break;
			case 43: putImageTransparent(&Brink_Broke_6, Bomb.x, Bomb.y - 30, 30, 30, RGB(0, 170, 1)); Brink_Broke_up = 0; break;
			}
		}

		if (Brink_Broke_down)
		{
			switch (Bomb_Count)
			{
			case 38: putImageTransparent(&Brink_Broke_1, Bomb.x, Bomb.y + 30, 30, 30, RGB(0, 170, 1)); break;
			case 39: putImageTransparent(&Brink_Broke_2, Bomb.x, Bomb.y + 30, 30, 30, RGB(0, 170, 1)); break;
			case 40: putImageTransparent(&Brink_Broke_4, Bomb.x, Bomb.y + 30, 30, 30, RGB(0, 170, 1)); break;
			case 41: putImageTransparent(&Brink_Broke_5, Bomb.x, Bomb.y + 30, 30, 30, RGB(0, 170, 1)); break;
			case 42: putImageTransparent(&Brink_Broke_4, Bomb.x, Bomb.y + 30, 30, 30, RGB(0, 170, 1)); break;
			case 43: putImageTransparent(&Brink_Broke_6, Bomb.x, Bomb.y + 30, 30, 30, RGB(0, 170, 1)); Brink_Broke_down = 0; break;
			}
		}

		if (Brink_Broke_left)
		{
			switch (Bomb_Count)
			{
			case 38: putImageTransparent(&Brink_Broke_1, Bomb.x - 30, Bomb.y, 30, 30, RGB(0, 170, 1)); break;
			case 39: putImageTransparent(&Brink_Broke_2, Bomb.x - 30, Bomb.y, 30, 30, RGB(0, 170, 1)); break;
			case 40: putImageTransparent(&Brink_Broke_4, Bomb.x - 30, Bomb.y, 30, 30, RGB(0, 170, 1)); break;
			case 41: putImageTransparent(&Brink_Broke_5, Bomb.x - 30, Bomb.y, 30, 30, RGB(0, 170, 1)); break;
			case 42: putImageTransparent(&Brink_Broke_4, Bomb.x - 30, Bomb.y, 30, 30, RGB(0, 170, 1)); break;
			case 43: putImageTransparent(&Brink_Broke_6, Bomb.x - 30, Bomb.y, 30, 30, RGB(0, 170, 1)); Brink_Broke_left = 0; break;
			}
		}

		if (Brink_Broke_right)
		{
			switch (Bomb_Count)
			{
			case 38: putImageTransparent(&Brink_Broke_1, Bomb.x + 30, Bomb.y, 30, 30, RGB(0, 170, 1)); break;
			case 39: putImageTransparent(&Brink_Broke_2, Bomb.x + 30, Bomb.y, 30, 30, RGB(0, 170, 1)); break;
			case 40: putImageTransparent(&Brink_Broke_4, Bomb.x + 30, Bomb.y, 30, 30, RGB(0, 170, 1)); break;
			case 41: putImageTransparent(&Brink_Broke_5, Bomb.x + 30, Bomb.y, 30, 30, RGB(0, 170, 1)); break;
			case 42: putImageTransparent(&Brink_Broke_4, Bomb.x + 30, Bomb.y, 30, 30, RGB(0, 170, 1)); break;
			case 43: putImageTransparent(&Brink_Broke_6, Bomb.x + 30, Bomb.y, 30, 30, RGB(0, 170, 1)); Brink_Broke_right = 0; break;
			}
		}     

		if (Brink_Broke)
		{
			switch (Bomb_Count)
			{
			case 38: putImageTransparent(&Brink_Broke_1, Bomb.x, Bomb.y, 30, 30, RGB(0, 170, 1)); break;
			case 39: putImageTransparent(&Brink_Broke_2, Bomb.x, Bomb.y, 30, 30, RGB(0, 170, 1)); break;
			case 40: putImageTransparent(&Brink_Broke_4, Bomb.x, Bomb.y, 30, 30, RGB(0, 170, 1)); break;
			case 41: putImageTransparent(&Brink_Broke_5, Bomb.x, Bomb.y, 30, 30, RGB(0, 170, 1)); break;
			case 42: putImageTransparent(&Brink_Broke_4, Bomb.x, Bomb.y, 30, 30, RGB(0, 170, 1)); break;
			case 43: putImageTransparent(&Brink_Broke_6, Bomb.x, Bomb.y, 30, 30, RGB(0, 170, 1)); Brink_Broke = 0; break;
			}
		}                     //生成炸弹被炸毁的动态效果

		Bomb_Count++;

	}

}

void TimerEventProcess(int timerID)
{
	if (timerID == 0)
	{
		beginPaint();
		if (Start == -1)
		{
			Sleep(1000);
			Start = 1;
		}
		
		if (isdeath == 2)
		{
			Sleep(2000);
			playSound(defeat, 0);
			isdeath = 3;
			Start = -2;
		}
		
		if (victory == 0)
			clearDevice();
		else
			Start = -2;

		if (Start == 0)
		{   
			putImage(&Start_Menu, 0, 0);

			if (KEY_DOWN(90) || KEY_DOWN(122))
			{
				Start = -1;
				stopSound(begin_music);
				playSound(Start_Button, 0);
				Sleep(1000);
				putImage(&Transition, 0, 0);
				playSound(Main_sound, 1);
			}
		}
		if(Start == 1)
		{
			if (Brinkput == 0)
			{
				init_map_show();
				init_monster_show();
			}
			map_show();
			if (pause == 1)
			{
				Character_show();
				pause_menu();
			}
			if (pause == 0) {
				pause_back();
				operate();
				if (Bomb_Count != 0)
					bomb_show();
				ismonsterdeath();
				ismandeath();
				monster_death_show();
				character_death_show();
			}
			isprop();
		}

		if (isdeath == 3)
		{
			putImage(&Defeat, 0, 0);
		}
		endPaint();
	}
}

void operate()
{
	int i, p, q;      //矫正炸弹的坐标

	if (KEY_DOWN(VK_LEFT) && isdeath == 0)
	{
		Character_show_left();
		if (prop.isCross_Wall == 0)
		{
			if (map[Character.x - 1][Character.y + 1] != 1 && map[Character.x-1][Character.y + 1] != 2 && map[Character.x - 1][Character.y + 29] != 1 && map[Character.x - 1][Character.y + 29] != 2)
			{
				Character.x -= SPEED;
			}
			else
			{
				if (map[Character.x - 1][Character.y + 1] != 1 && map[Character.x-1][Character.y + 1] != 2 && (map[Character.x - 1][Character.y + 29] == 1 || map[Character.x - 1][Character.y + 29] == 2))
				{
					Character.y -= ADJUST;
				}
				else
				{
					if ((map[Character.x - 1][Character.y + 1] == 1 || map[Character.x-1][Character.y + 1] == 2) && map[Character.x - 1][Character.y + 29] != 1 && map[Character.x - 1][Character.y + 29] != 2)
					{
						Character.y += ADJUST;
					}
				}
			}
		}
		else
		{
			if (map[Character.x - 1][Character.y + 1] != 1 && map[Character.x - 1][Character.y + 29] != 1)
			{
				Character.x -= SPEED;
			}
			else
			{
				if (map[Character.x - 1][Character.y + 1] != 1 && map[Character.x - 1][Character.y + 29] == 1)
				{
					Character.y -= ADJUST;
				}
				else
				{
					if (map[Character.x - 1][Character.y + 1] == 1 && map[Character.x - 1][Character.y + 29] != 1)
					{
						Character.y += ADJUST;
					}
				}
			}
		}
		Direction = LEFT;
	}

	if (KEY_DOWN(VK_RIGHT) && isdeath == 0)
	{
		Character_show_right();
		if (prop.isCross_Wall == 0)
		{
			if (map[Character.x + 31][Character.y + 1] != 1 && map[Character.x + 31][Character.y + 1] != 2 && map[Character.x + 31][Character.y + 29] != 1 && map[Character.x + 31][Character.y + 29] != 2)
			{
				Character.x += SPEED;
			}
			else
			{
				if (map[Character.x + 31][Character.y + 1] != 1 && map[Character.x + 31][Character.y + 1] != 2 && (map[Character.x + 31][Character.y + 29] == 1 || map[Character.x + 31][Character.y + 29] == 2))
				{
					Character.y -= ADJUST;
				}
				else
				{
					if ((map[Character.x + 31][Character.y + 1] == 1 || map[Character.x + 31][Character.y + 1] == 2) && map[Character.x + 31][Character.y + 29] == 1 != 1 && map[Character.x + 31][Character.y + 29] != 2)
					{
						Character.y += ADJUST;
					}
				}
			}
		}
		else
		{
			if (map[Character.x + 31][Character.y + 1] != 1 && map[Character.x + 31][Character.y + 29] != 1)
			{
				Character.x += SPEED;
			}
			else
			{
				if (map[Character.x + 31][Character.y + 1] != 1 && map[Character.x + 31][Character.y + 29] == 1)
				{
					Character.y -= ADJUST;
				}
				else
				{
					if (map[Character.x + 31][Character.y + 1] == 1 && map[Character.x + 31][Character.y + 29] != 1)
					{
						Character.y += ADJUST;
					}
				}
			}
		}
		Direction = RIGHT;
	}

	if (KEY_DOWN(VK_DOWN) && isdeath == 0)
	{
		Character_show_down();
		if (prop.isCross_Wall == 0)
		{
			if (map[Character.x + 1][Character.y + 31] != 1 && map[Character.x + 1][Character.y + 31] != 2 && map[Character.x + 29][Character.y + 31] != 1 && map[Character.x + 29][Character.y + 31] != 2)
			{
				Character.y += SPEED;
			}
			else
			{
				if (map[Character.x + 1][Character.y + 31] != 1 && map[Character.x + 1][Character.y + 31] != 2 && (map[Character.x + 29][Character.y + 31] == 1 || map[Character.x + 29][Character.y + 31] == 2))
				{
					Character.x -= ADJUST;
				}
				else
				{
					if ((map[Character.x + 1][Character.y + 31] == 1 || map[Character.x + 1][Character.y + 31] == 2) && map[Character.x + 29][Character.y + 31] != 1 && map[Character.x + 29][Character.y + 31] != 2)
					{
						Character.x += ADJUST;
					}
				}
			}
		}
		else
		{
			if (map[Character.x + 1][Character.y + 31] != 1 && map[Character.x + 29][Character.y + 31] != 1)
			{
				Character.y += SPEED;
			}
			else
			{
				if (map[Character.x + 1][Character.y + 31] != 1 && map[Character.x + 29][Character.y + 31] == 1)
				{
					Character.x -= ADJUST;
				}
				else
				{
					if (map[Character.x + 1][Character.y + 31] == 1 && map[Character.x + 29][Character.y + 31] != 1)
					{
						Character.x += ADJUST;
					}
				}
			}
		}
		Direction = DOWN;
	}

	if (KEY_DOWN(VK_UP) && isdeath == 0)
	{
		Character_show_up();
		if (prop.isCross_Wall == 0)
		{
			if (map[Character.x + 1][Character.y - 1] != 1 && map[Character.x + 1][Character.y - 1] != 2 && map[Character.x + 29][Character.y - 1] != 1 && map[Character.x + 29][Character.y - 1] != 2)
			{
				Character.y -= SPEED;
			}
			else
			{
				if (map[Character.x + 1][Character.y - 1] != 1 && map[Character.x + 1][Character.y - 1] != 2 && (map[Character.x + 29][Character.y - 1] == 1 || map[Character.x + 29][Character.y - 1] == 2))
				{
					Character.x -= ADJUST;
				}
				else
				{
					if ((map[Character.x + 1][Character.y - 1] == 1 || map[Character.x + 1][Character.y - 1] == 2) && map[Character.x + 29][Character.y - 1] != 1 && map[Character.x + 29][Character.y - 1] != 2)
					{
						Character.x += ADJUST;
					}
				}
			}
		}
		else
		{
			if (map[Character.x + 1][Character.y - 1] != 1 && map[Character.x + 29][Character.y - 1] != 1)
			{
				Character.y -= SPEED;
			}
			else
			{
				if (map[Character.x + 1][Character.y - 1] != 1 && map[Character.x + 29][Character.y - 1] == 1)
				{
					Character.x -= ADJUST;
				}
				else
				{
					if (map[Character.x + 1][Character.y - 1] == 1 && map[Character.x + 29][Character.y - 1] != 1)
					{
						Character.x += ADJUST;
					}
				}
			}
		}
		Direction = UP;
	}

	if (!(KEY_DOWN(VK_LEFT) || KEY_DOWN(VK_RIGHT) || KEY_DOWN(VK_DOWN) || KEY_DOWN(VK_UP)) && isdeath == 0)
	{
		Character_show();
	}

	if ((KEY_DOWN(90) || KEY_DOWN(122)) && Bomb_Count == 0)
	{
		playSound(pushBomb, 0);

		Bomb_Count = 1;

		for (i = 0; (Character.y + 15 - i) % 30 != 0; i++);
		Bomb.y = Character.y + 15 - i;
		for (i = 0; (Character.x + 15 - i) % 30 != 0; i++);
		Bomb.x = Character.x + 15 - i;

		if (prop.isCross_Bomb == 1)
		{
			for (p = 2; p < 29; p++)
				for (q = 2; q < 29; q++)
				{
					map[Bomb.x + p][Bomb.y + q] = 1;
				} //炸弹标定
		}
		else
		{
			for (p = 1; p < 30; p++)
				for (q = 1; q < 30; q++)
				{
					map[Bomb.x + p][Bomb.y + q] = 1;
				} //炸弹标定
		}

	}

	//随机生成怪物的前进方向
	for (i = 1; i <= max_monster; i++)
	{
		if (monster[i].death == 0)
		{
			if (randturn == 0)
			{
				monster[i].direction = rand() % 4 + 1;
			}

			if (monster[i].direction == LEFT)
			{
				monster_show_left(i);
				if (map[monster[i].Monster_position.x - 5][monster[i].Monster_position.y + 15] != 1 && map[monster[i].Monster_position.x - 5][monster[i].Monster_position.y + 15] != 2)
				{
					monster[i].Monster_position.x -= MONSPEED;
				}
			}

			if (monster[i].direction == RIGHT)
			{
				monster_show_right(i);
				if (map[monster[i].Monster_position.x + 35][monster[i].Monster_position.y + 15] != 1 && map[monster[i].Monster_position.x + 35][monster[i].Monster_position.y + 15] != 2)
				{

					monster[i].Monster_position.x += MONSPEED;
				}
			}

			if (monster[i].direction == DOWN)
			{
				monster_show_right(i);
				if (map[monster[i].Monster_position.x + 15][monster[i].Monster_position.y + 35] != 1 && map[monster[i].Monster_position.x + 15][monster[i].Monster_position.y + 35] != 2)
				{
					monster[i].Monster_position.y += MONSPEED;
				}
			}

			if (monster[i].direction == UP)
			{
				monster_show_left(i);
				if (map[monster[i].Monster_position.x + 15][monster[i].Monster_position.y - 5] != 1 && map[monster[i].Monster_position.x + 15][monster[i].Monster_position.y - 5] != 2)
				{
					monster[i].Monster_position.y -= MONSPEED;
				}
			}
		}
	}
	randturn++;
	if (randturn == 10)
		randturn = 0;
}

void pause_menu() {

	if (pausemenu_position.y < 180)
	{
		putImage(&Pause_resume,pausemenu_position.x, pausemenu_position.y);
		pausemenu_position.y += 90;
	}          //暂停界面动态生成
	if (pausemenu_position.y == 180)
	{
		switch (turn)
		{
		case 1:
			putImage(&Pause_home, 191, 180);
			if (press == 1) {
				pause = 0;
				press = 0;
				pausemenu_position.y = -360;
				turn = 2;
				stopSound(Main_sound);
				playSound(begin_music, 1);
				Start = 0;
			}                      //回复到主菜单界面
			break;
		case 2:
			putImage(&Pause_resume, 191, 180);
			if (press == 2) {
				pause = 0;
				press = 0;
				Bomb_Count = 44;
			}                        //退回暂停界面，游戏继续
			break;
		case 3:
			putImage(&Pause_music, 191, 180);
			if (press == 3) {
				if (music_switch == 1) {
					putImage(&Pause_musicoff, 191, 180);
				}
				else {
					putImage(&Pause_music, 191, 180);
				}
			}                     //关闭或开启音乐
			break;
		}
	}

}

void pause_back() {

	if (pausemenu_position.y > -360) {
		putImage(&Pause_resume, pausemenu_position.x, pausemenu_position.y);
		pausemenu_position.y -= 90;
	}
}

void KeyboardEventProcess(int key, int event)/*每当产生键盘消息，都要执行*/
{
	if (event == KEY_DOWN) {
		if (pause == 1 && (key == 27 || key == VK_LEFT || key == VK_RIGHT || key == 13)) {
			playSound(pause_button, 0);
		} //按键音效*/

		if (pause == 0 && key == 27) {
			playSound(pause_button, 0);
			pause = 1;
		}
		else if (pause == 1 && key == 27) {
			pause = 0;
		}

		if (pause == 1) {
			if (key == VK_LEFT) {
				turn--;
			}

			if (key == VK_RIGHT) {
				turn++;
			}

			if (turn == 0) {
				turn = 3;
			}

			if (turn == 4) {
				turn = 1;
			}

			if ((key == 13) && turn == 3) {
				press = 3;
				if (music_switch == 0) {
					music_switch = 1;
					stopSound(Main_sound);
				}
				else {
					music_switch = 0;
					playSound(Main_sound, 1);
				}
			}

			if ((key == 13) && turn == 2) {
				press = 2;
			}

			if ((key == 13) && turn == 1) {
				press = 1;
			}
		}
	}
}

void ismonsterdeath()
{
	int i;

	for (i = 1; i <= max_monster; i++)
	{
		if (Bomb_Count >= 37 && Bomb_Count <= 43)
		{
			if ((abs(monster[i].Monster_position.y - Bomb.y) < 30 && abs(monster[i].Monster_position.x - Bomb.x) < 60) || (abs(monster[i].Monster_position.x - Bomb.x) < 30 && abs(monster[i].Monster_position.y - Bomb.y) < 60))
			{
				monster[i].death = 1;
			}
		}
	}
}

void ismandeath()
{
	int i;
	
	if (prop.isBomb_ignore == 0)
	{
		if (Bomb_Count >= 37 && Bomb_Count <= 43)
		{
			if ((abs(Character.y - Bomb.y) < 30 && abs(Character.x - Bomb.x) < 60) || (abs(Character.x - Bomb.x) < 30 && abs(Character.y - Bomb.y) < 60))
			{
				isdeath = 1;
			}
		}
	}

	for (i = 1; i  <= max_monster; i++)
	{
		if (isdeath == 0 && monster[i].death == 0 && abs(monster[i].Monster_position.y - Character.y) < 20 && abs(monster[i].Monster_position.x - Character.x) < 20)
		{
			isdeath = 1;
		}
	}
	
}

void isprop()
{
	if (sum_monster == 0 && map[Character.x + 15][Character.y + 15] == Door && map[Character.x + 15][Character.y + 14] == 0)
	{
		victory = 1;
		stopSound(Main_sound);
		putImage(&Victory, 0, 0);
		playSound(vic, 0);
	}

	if (map[Character.x + 15][Character.y + 15] == BOOST && map[Character.x + 15][Character.y + 14] == 0)
	{
		map[Character.x + 15][Character.y + 15] = 0;
		playSound(propSound, 0);
		SPEED = 10;
	}

	if (map[Character.x + 15][Character.y + 15] == CROSS_BOMB && map[Character.x + 15][Character.y + 14] == 0)
	{
		map[Character.x + 15][Character.y + 15] = 0;
		playSound(propSound, 0);
		prop.isCross_Bomb = 1;
	}

	if (map[Character.x + 15][Character.y + 15] == CROSS_WALL && map[Character.x + 15][Character.y + 14] == 0)
	{
		map[Character.x + 15][Character.y + 15] = 0;
		playSound(propSound, 0);
		prop.isCross_Wall = 1;
	}

	if (map[Character.x + 15][Character.y + 15] == BOMB_IGNORE && map[Character.x + 15][Character.y + 14] == 0)
	{
		map[Character.x + 15][Character.y + 15] = 0;
		playSound(propSound, 0);
		prop.isBomb_ignore = 1;
	}
}