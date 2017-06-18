#pragma once
#ifndef DEF_MAINGAME_H
#define DEF_MAINGAME_H
#include "LoadFumen.h"

class CMainGame{
private:
	CLoadFumen *m_LoadFumen;
	char **file_name;
	void Calc();
	void Draw();
	int top_img;
	int pattern;
	int cnt_music;
	int pos_y;
	int now_pos;
	int wait_timer;
	int font_handle;
	bool fade_flag;
	int extend_handle;
	int speed_pattern;
	int play_speed;
	int bg_color;
	char *color_name[6];
	int img_back;
	int img_wave_line;
	int img_wave_dot;
	int now_line;
	int img_touch;
	struct{
		int cnt;
		int y;
	}sin_data[4];

	int before_data[3]; 
	int max_y[540];
public:
	CMainGame();
	~CMainGame();
	void Process();
	void back_animetion();
};
#endif
