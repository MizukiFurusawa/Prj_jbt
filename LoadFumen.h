#pragma once
#include <DxLib.h>
#include "HighTimer.h"
#define HNDCLP	64
class CLoadFumen{
private:

	//マーカーのデータ用
	struct obj_data{
		double time;
		bool pos[16];
		int anime_pattern[16];
		double anime_start_time[16];
		double now_bpm;
	};

	//ノーツ分確保する
	struct obj_data *m_obj_data;
	bool part_flag;
	bool combo_anime_flag;
	double cmbo_anime_move;
	double start_shutter_move;
	int start_shutter_cnt;
	bool calc_sin_flag;
	int combo_anime_cnt;
	int img_combo;
	int play_pattern;
	int img_bg_buttom;
	double tmp;
	double calc_sin;
	double shutter_open;
	int calcNum;
	int numCount;
	int combo_anime;
	double shutter_sin;
	int img_bg_top;
	int cnt_note;
	int cnt_all_note;
	FILE *file_p;
	int part_line;
	int ef_hcp[HNDCLP];
	int ef_select_bgm;
	int now_play_hcp;
	int playing_note;
	double get_bpm;
	char get_str[128];
	int now_shosetu;
	int get_c;
	int now_note;
	double total_time;
	double now_bpm;
	int now_select_mkr;

	struct {
		int mkr_bad[25];
		int mkr_good[25];
		int mkr_great[25];
		int mkr_nor[25];
		int mkr_per[25];
		int mkr_frame;
	}img_mkr[11];
	int exc_left;
	int exc_right;
	int exc_cnt;
	bool exc_flag;
	int exc_pos;
	int cnt_per_note;
	double calc_tmp_shatter;
	int revision_pattern;
	int get_wheel;
	Chigh_timer m_ht_bgm;
	int bgm_offset;
	bool bgm_offset_flag;
	int now_combo;
	int img_combo_font[10];
	
	//debug
	int img_start_pos[84];
	int img_start_pos_frame;
	int img_start_pos_alpha;
	int ef_go;

	Chigh_timer m_ht_mkr;
	int mkr_offset;
	bool start_flag;
	bool mkr_offset_flag;
	int wait_time;
	int now_line_pos;
	bool check_mkr_redundancy[32];
	double request_start_time[16];
	bool mkr_draw_flag[16];
	int mkr_anime_pattern[16];
	int before_note;
	int key_code[16];
	int *p_mkr_anime[5][11];
	int bgm_sanple;
	int play_speed;
	bool change_shosetu;
	char file_path[512];
	char bgm_path[512];
	int img_back_nor;
	int img_back_max;
	int get_bpm_now_playing;
	int wait_timer;
	int score;
	void CLoadFumen::show_mkr( void );
	bool CLoadFumen::check_end( void );
	void CLoadFumen::draw_mkr(int pos,int frame,int *p_img);
	double CLoadFumen::get_note_time(int p_max);
	int CLoadFumen::get_mkr_anime_pattern(int mkr_frame,int scope);
	int CLoadFumen::load_next_line( void );
	int CLoadFumen::f_cnt_note(void);
	int CLoadFumen::f_load_time(void);
	int CLoadFumen::f_load_pos(void);
	int CLoadFumen::check_str(char get_c);		//文字データを配置データに変換
	void CLoadFumen::play_ef_hand(void);
	void CLoadFumen::init_value(void);
	int CLoadFumen::check_bpm_line(void);
	int CLoadFumen::check_bpm_char(char *get_s , int *now_s_pos);
public:
	CLoadFumen::CLoadFumen(int back_clr,int get_play_speed);
	CLoadFumen::~CLoadFumen();
	int CLoadFumen::load_fumen(char *path);
	int CLoadFumen::play(void);
};


int getButtonPosX(char buttonPos);
int getButtonPosY(char buttonPos);