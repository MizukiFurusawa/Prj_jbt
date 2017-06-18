#include "LoadFumen.h"
#include "ComDef.h"
#include <math.h>
extern bool touch_btn_flag[16];
char		keyBuff[256];
void getKey(void){
	char tempKey[256];
	GetHitKeyStateAll(tempKey);
	for (int i=0;i<256;i++){
		if(tempKey[i]!=0){
			keyBuff[i]++;
			if(keyBuff[i]<=0)keyBuff[i]=2;
		}else{
			keyBuff[i]=0;
		}
	}
}

CLoadFumen::CLoadFumen(int back_clr,int get_play_speed){


	start_flag = true;
	score = 0;
	ef_go = LoadSoundMem("jbt_rc/go.mp3");
	get_bpm_now_playing = play_pattern = 0;
	calc_sin_flag=false;
	shutter_open=0;
	shutter_sin=0;
	cnt_note = 0;
	cnt_all_note = 0;
	exc_pos = 475;
	start_shutter_move =0;
	start_shutter_cnt=0;
	now_play_hcp = 0;
	playing_note = 0;
	now_combo = 0;
	combo_anime_cnt  = 0;
	cnt_per_note = 0;
	exc_flag = false;
	play_speed = get_play_speed;
	img_combo = LoadGraph("jbt_rc/img_combo.png");
	img_bg_top = LoadGraph("jbt_rc/bg_top.png");
	img_bg_buttom = LoadGraph("jbt_rc/bg_buttom.png");
	LoadDivGraph("jbt_rc/img_combo_font.png",10,10,1,205,300,img_combo_font);
	combo_anime_flag=false;
	cmbo_anime_move = 300;
	switch(back_clr){
	case 0:
		img_back_nor = LoadGraph("jbt_rc/bg_nor/ble.png");
		img_back_max = LoadGraph("jbt_rc/bg_max/ble.png");
		break;
	case 1:
		img_back_nor = LoadGraph("jbt_rc/bg_nor/blk.png");
		img_back_max = LoadGraph("jbt_rc/bg_max/blk.png");
		break;
	case 2:
		img_back_nor = LoadGraph("jbt_rc/bg_nor/grn.png");
		img_back_max = LoadGraph("jbt_rc/bg_max/grn.png");
		break;
	case 3:
		img_back_nor = LoadGraph("jbt_rc/bg_nor/pnk.png");
		img_back_max = LoadGraph("jbt_rc/bg_max/pnk.png");
		break;
	case 4:
		img_back_nor = LoadGraph("jbt_rc/bg_nor/red.png");
		img_back_max = LoadGraph("jbt_rc/bg_max/red.png");
		break;
	case 5:
		img_back_nor = LoadGraph("jbt_rc/bg_nor/ylw.png");
		img_back_max = LoadGraph("jbt_rc/bg_max/ylw.png");
		break;
	}
	
				
	for(int i=0;i<HNDCLP;i++){
		ef_hcp[i]=LoadSoundMem("jbt_rc/ef_hcp.wav");
		ChangeVolumeSoundMem( 255 , ef_hcp[  (now_play_hcp<HNDCLP-1)?now_play_hcp++:now_play_hcp=0 ] );
	}
	for(int i=0;i<16;i++){
		mkr_anime_pattern[i]=0;
		request_start_time[i]=0;
	}
	exc_left = LoadGraph("jbt_rc/exc_left.png");
	exc_right = LoadGraph("jbt_rc/exc_right.png");
	now_select_mkr = 0 ;
	const char *mkr_name[11]={"blur","copious","cyber","flower","hanabi","knit","mirror","ripples","sand","syoku","yubiko"};
	char make_path[128];
	for(int i=0;i<11;i++){
		sprintf( make_path , "jbt_rc/marker/bad/%s.png" , mkr_name[i] );
		LoadDivGraph(make_path,25,5,5,160,160,img_mkr[i].mkr_bad);
		p_mkr_anime[1][i] = img_mkr[i].mkr_bad;

		sprintf( make_path , "jbt_rc/marker/good/%s.png" , mkr_name[i] );
		LoadDivGraph(make_path,25,5,5,160,160,img_mkr[i].mkr_good);
		p_mkr_anime[2][i] = img_mkr[i].mkr_good;

		sprintf( make_path , "jbt_rc/marker/great/%s.png" , mkr_name[i] );
		LoadDivGraph(make_path,25,5,5,160,160,img_mkr[i].mkr_great);
		p_mkr_anime[3][i] = img_mkr[i].mkr_great;

		sprintf( make_path , "jbt_rc/marker/normal/%s.png" , mkr_name[i] );
		LoadDivGraph(make_path,25,5,5,160,160,img_mkr[i].mkr_nor);
		p_mkr_anime[0][i] = img_mkr[i].mkr_nor;

		sprintf( make_path , "jbt_rc/marker/perfect/%s.png" , mkr_name[i] );
		LoadDivGraph(make_path,25,5,5,160,160,img_mkr[i].mkr_per);
		p_mkr_anime[4][i] = img_mkr[i].mkr_per;

	}


	//debug
	//img_start_pos

	key_code[0] = KEY_INPUT_1;
	key_code[1] = KEY_INPUT_2;
	key_code[2] = KEY_INPUT_3;
	key_code[3] = KEY_INPUT_4;
	key_code[4] = KEY_INPUT_Q;
	key_code[5] = KEY_INPUT_W;
	key_code[6] = KEY_INPUT_E;
	key_code[7] = KEY_INPUT_R;
	key_code[8] = KEY_INPUT_A;
	key_code[9] = KEY_INPUT_S;
	key_code[10] = KEY_INPUT_D;
	key_code[11] = KEY_INPUT_F;
	key_code[12] = KEY_INPUT_Z;
	key_code[13] = KEY_INPUT_X;
	key_code[14] = KEY_INPUT_C;
	key_code[15] = KEY_INPUT_V;
	img_mkr[0].mkr_frame=32;
	img_mkr[1].mkr_frame=35;
	img_mkr[2].mkr_frame=35;
	img_mkr[3].mkr_frame=60;
	img_mkr[4].mkr_frame=60;
	img_mkr[5].mkr_frame=32;
	img_mkr[6].mkr_frame=32;
	img_mkr[7].mkr_frame=35;
	img_mkr[8].mkr_frame=50;
	img_mkr[9].mkr_frame=35;
	img_mkr[10].mkr_frame=35;
	before_note = -1;
	wait_timer = GetNowCount();
}

//1行の中でbpmを探す 
int CLoadFumen::check_bpm_line(void){
	long now_pointer = ftell( file_p );
	char l_get_str[128];
	int l_get_bpm = 0;
	fgets  (  l_get_str  ,  128  ,  file_p  );
	sscanf  (  l_get_str  ,  "bpm=%d\n"  ,  &l_get_bpm  );
	if( l_get_bpm != 0 ){
		now_bpm = l_get_bpm;
		//dmsg("cbl = %d",l_get_bpm);
		return 1;
	}else{
		fseek( file_p , -(ftell( file_p ) - now_pointer) , SEEK_CUR );
	}
	return 0;
}

//カッコ内にあるbpmを探す
int CLoadFumen::check_bpm_char(char *get_s , int *now_s_pos){
	int cbc_str_pos=0;
	int l_get_bpm = 0;
	char make_str[32];
	if( get_s[ (*now_s_pos) ] == '(' ){
		(*now_s_pos)++;
		while( get_s[ (*now_s_pos) ] != ')' ){
			if(  get_s[ (*now_s_pos) ] == '\n'  ||  get_s[ (*now_s_pos) ] == '\0'  ||  get_s[ (*now_s_pos) ] == '('  ){
				dmsg("%d小節目\n\"(\" に対応する \")\"が見つかりません。",now_shosetu);
				return -1;
			}
			make_str[cbc_str_pos] = get_s[ *now_s_pos ];
			cbc_str_pos++;
			(*now_s_pos)++;
		}
		(*now_s_pos)++;
		make_str[cbc_str_pos] = '\0';
		if(sscanf  (  make_str  ,  "bpm=%d"  ,  &l_get_bpm  )!=0)now_bpm =  (int)(l_get_bpm*((double)play_speed/100));
	}else if(get_s[ (*now_s_pos) ] == ')')dmsg("%d小節目\n\"(\" に対応する \")\"が見つかりません。",now_shosetu);
	return 0;
}


int CLoadFumen::load_next_line( void ){
	while( 1 ){
		int l_get_shosetu = 0;
		int l_get_bpm = 0;
		int mns_str_pos = 0;
		int str_cnt = 0;
		if(fgets  (  get_str  ,  128  ,  file_p  )==NULL)break;
	
		//bpm表記の行だったらもう1回読み込む
		if(  strstr  (  get_str  ,  "("  )  ==  NULL && strstr  (  get_str  ,  ")"  )  ==  NULL && strstr  (  get_str  ,  "bpm="  )  !=  NULL  ){
			sscanf  (  get_str  ,  "bpm=%d\n"  ,  &l_get_bpm  );
			if( l_get_bpm != 0 )now_bpm = (int)(l_get_bpm*((double)play_speed/100));
			continue;
		}
		//譜面データの行だったら終了
		if(  strstr  (  get_str  ,  "|"  )  !=  NULL  ||  strstr  (  get_str  ,  "x"  )  !=  NULL  ||  strstr  (  get_str  ,  "-"  )  !=  NULL  ){
			return 0;
		}
		if(  strstr  (  get_str  ,  "s_pos="  )  !=  NULL  )continue;
		if(  strstr  (  get_str  ,  "high_score="  )  !=  NULL  )continue;
		if(  strstr  (  get_str  ,  "offset="  )  !=  NULL  )continue;
		if(  strstr  (  get_str  ,  "level="  )  !=  NULL  )continue;

		//小節の先頭でも終了
		sscanf  (  get_str  ,  "%d\n"  ,  &l_get_shosetu  );
		if( l_get_shosetu == now_shosetu + 1 ){
			if( l_get_shosetu != 0 )change_shosetu=true;
			now_shosetu++;
			continue;
		}

		//譜面データの行で分割されている場合でも終了
		while( get_str[mns_str_pos]!= '\n' && get_str[mns_str_pos]!= '\0' ){
			if( check_str(get_str[mns_str_pos]) >=0)str_cnt++;
			mns_str_pos++;
		}
		if( str_cnt>=4 ){
			return 0;
		}
	}
	return -1;
}

//ノーツ数をカウントする
int CLoadFumen::f_cnt_note(void){

	//まずは初期位置にセット
	fseek(  file_p  ,  0  , SEEK_SET  );
	now_line_pos = 0;

	//小節の先頭にジャンプ
	while ( load_next_line() == 0 ){

		//BPM変更チェック
		check_bpm_line();
		
		//get_str中の最初の" | "を見つけるまでスキップ
		int fcn_str_pos = 0;
		while ( get_str[ fcn_str_pos ] != '|' ){
			if( get_str[ fcn_str_pos ] == '\0' ||  get_str[ fcn_str_pos ] == '\n' )break;
			fcn_str_pos++;
		}
			
		//get_str中の最後の" | "を見つけるまでノーツ数をカウント
		if( get_str[ fcn_str_pos ] != '\0' &&  get_str[ fcn_str_pos ] != '\n' ){
			fcn_str_pos++;
			while ( get_str[ fcn_str_pos ] != '|' ){

				//今回はノーツ数だけなのでBPMはカウントしない
				if( check_bpm_char( get_str , &fcn_str_pos )==-1 )return -1;

				if( get_str[ fcn_str_pos ] == '\0' ||  get_str[ fcn_str_pos ] == '\n' )break;
				if( check_str( get_str[ fcn_str_pos ] )  >=0 )cnt_note++;
				fcn_str_pos++;
			}
		}
	}
	return 0;
}


int CLoadFumen::f_load_time(void){

	//まずは初期位置にセット
	fseek(  file_p  ,  0  , SEEK_SET  );
	now_line_pos = 0;

	//小節の先頭にジャンプ
	while ( load_next_line() == 0 ){

		//BPM変更チェック
		check_bpm_line();

		//get_str中の最初の" | "を見つけるまでスキップ
		int flt_str_pos = 0;
		while ( get_str[ flt_str_pos ] != '|' ){
			if( get_str[ flt_str_pos ] == '\0' ||  get_str[ flt_str_pos ] == '\n' )break;
			flt_str_pos++;
		}
			
		//get_str中の最後の" | "を見つけるまで表示時間を計算
		if( get_str[ flt_str_pos ] != '\0' &&  get_str[ flt_str_pos ] != '\n' ){
			flt_str_pos++;

			//計算前に何分割か数える
			int cnt = 0;
			int tmp_str_pos = flt_str_pos;
			while( get_str[ tmp_str_pos ] != '\0' &&  get_str[ tmp_str_pos ] != '\n' && get_str[ tmp_str_pos ]!='|' ){
				check_bpm_char( get_str , &tmp_str_pos );
				if( get_str[ tmp_str_pos ] == '-' ||  check_str( get_str[ tmp_str_pos ] )  >=0 )cnt++;
				tmp_str_pos++;
			}
			//マーカーの出現時間を算出
			while( get_str[ flt_str_pos ] != '\0' &&  get_str[ flt_str_pos ] != '\n' && get_str[ flt_str_pos ]!='|' ){
				check_bpm_char( get_str , &flt_str_pos );
				if( check_str( get_str[ flt_str_pos ] ) >= 0 ){
					m_obj_data[ now_note ].time = total_time;
					m_obj_data[ now_note ].now_bpm = now_bpm;
					now_note++;
				}
				total_time += get_note_time( cnt );
				flt_str_pos++;
			}
		}
	}
	return 0;
}



int CLoadFumen::f_load_pos(void){

	//まずは初期位置にセット
	int flp_now_note = 0;
	int flp_before_note = 0;
	fseek(  file_p  ,  0  , SEEK_SET  );
	now_line_pos = 0;
	change_shosetu = false;

	//小節の先頭にジャンプ
	while ( load_next_line() == 0 ){
		if(now_line_pos>3){
			now_line_pos=0;
		}
		if( change_shosetu == true ){
			for(int i=0;i<32;i++)check_mkr_redundancy[i]=false;
			flp_now_note+=flp_before_note;
			flp_before_note=0;
			change_shosetu = false;
		}
		//BPM変更チェック
		check_bpm_line();

		//get_str中の最初の" | "を見つけるまで
		int flp_str_pos = 0;
		int now_char_pos = 0;
		while ( get_str[ flp_str_pos ] != '|' ){
			if(now_char_pos>=4 ||  get_str[ flp_str_pos ] == '\0' ||  get_str[ flp_str_pos ] == '\n' )break;
			int get_pos = check_str( get_str[ flp_str_pos ] );
			if( get_pos>=0 || get_str[ flp_str_pos ]=='x'){
				if( get_pos>=0 ){
					if( flp_now_note + get_pos >= cnt_note )return -1;
					m_obj_data[ flp_now_note + get_pos ].pos[ now_line_pos*4+now_char_pos ] = true;
					cnt_all_note++;
					if(check_mkr_redundancy[get_pos]==false){
						flp_before_note++;
						check_mkr_redundancy[get_pos]=true;
					}
				}
				now_char_pos++;
			}
			flp_str_pos++;
		}
		now_line_pos++;
	}
	return 0;
}


//ノーツ数をカウント
int CLoadFumen::load_fumen(char *path){
	get_bpm = 0;
	get_c=0;
	part_line=0;
	part_flag=false;
	exc_cnt = 0;
	sprintf( file_path , "data/%s.txt" , path);
	sprintf( bgm_path , "music/%s.mp3" , path);
	ef_select_bgm = LoadSoundMem(bgm_path);
	bgm_sanple = (int)(GetFrequencySoundMem(ef_select_bgm) * ((double)play_speed/100));


	file_p = fopen(file_path,"r");
	if(file_p==NULL){
		dmsg("%s は見つかりませんでした。",file_path);
		return -1;
	}

	//BPM探す
	fseek(  file_p  ,  0  , SEEK_SET  );
	while (1){
		if(  fgets  (  get_str  ,  128  , file_p  )==NULL  ){
			dmsg("Syntax error!!  \n  bpmは見つかりませんでした。");
			return -1;
		}
		if(  strstr  (  get_str  ,  "bpm="  )  !=  NULL  )sscanf(  get_str  ,  "bpm=%lf"  ,  &get_bpm  );
		if(get_bpm>0 &&  get_bpm<=300)break;
	}
	//back_bpm

	bgm_offset=mkr_offset=500;
	//オフセット探す
	fseek(  file_p  ,  0  , SEEK_SET  );
	while (1){
		if(  fgets  (  get_str  ,  128  , file_p  )==NULL  )break;
		if(  strstr  (  get_str  ,  "bgm_offset="  )  !=  NULL  ){
			sscanf(  get_str  ,  "bgm_offset=%d"  ,  &bgm_offset  );
		}
		if(  strstr  (  get_str  ,  "mkr_offset="  )  !=  NULL  ){
			sscanf(  get_str  ,  "mkr_offset=%d"  ,  &mkr_offset  );
		}
	}
	init_value();
	if( f_cnt_note() == -1 ){
		fclose(file_p);
		return -1;
	}
	
	//ノーツ分構造体データを確保
	//dmsg( "note = %d", cnt_note );
	m_obj_data = (struct obj_data *)malloc( sizeof(struct obj_data) * cnt_note );
	if( m_obj_data != NULL ){
		for(int i=0;i<cnt_note;i++){
			m_obj_data[i].time = 0;
			m_obj_data[i].now_bpm = 0;
			for(int j=0;j<16;j++){
				m_obj_data[i].pos[j]=false;
				m_obj_data[i].anime_pattern[j]=0;
				m_obj_data[i].anime_start_time[j]=0;
			}
		}
	}else {
		dmsg("メモリの確保に失敗しました。(%d byte)",sizeof(struct obj_data) * cnt_note);
		fclose(file_p);
		return -1;
	}
	
	init_value();
	if( f_load_time() == -1 ){
		fclose( file_p );
		return -1;
	}

	init_value();
	if( f_load_pos() == -1 ){
		fclose( file_p );
		return -1;
	}
	
	fclose(file_p);
	bgm_offset_flag = true;
	mkr_offset_flag=true;
	revision_pattern = 0;
	return 0;
}

void CLoadFumen::init_value(void){
	for(int i=0;i<32;i++)check_mkr_redundancy[i]=false;
	now_note=0;
	total_time=500;
	now_bpm=get_bpm;
	now_shosetu = 0;
	part_flag = false;
	part_line=0;
	now_line_pos=0;
}


double CLoadFumen::get_note_time(int p_max){
	double tmp = 60/(now_bpm/4);
	return (tmp/(double)p_max)*250;









	      
}             

//ボタンのX座標を取得
int getButtonPosX(char buttonPos){
	return (int)((48+(258*((buttonPos-1)%4)))*DRAW_EXTEND_RATE);
}

//ボタンのY座標を取得
int getButtonPosY(char buttonPos){
	return (int)((48+(258*((buttonPos-1)/4)))*DRAW_EXTEND_RATE);
}

void CLoadFumen::draw_mkr(int pos,int frame,int *p_img){
	int x = getButtonPosX(pos);
	int y = getButtonPosY(pos);
	DrawExtendGraph(  x , y , x+(int)(211*DRAW_EXTEND_RATE) , y+(int)(211*DRAW_EXTEND_RATE) ,  p_img[frame] , true );
}




int CLoadFumen::get_mkr_anime_pattern(int mkr_frame,int scope){
	switch(scope){
	case 0:
		switch(mkr_frame){
		//mkr_per
		case 15:
			return 4;
			break;

		//mkr_great
		case 16:
		case 14:
			return 3;
			break;

		//mkr_good
		case 13:
		case 17:
			return 2;
			break;

		//mkr_good
		case 12:
		case 18:
			return 2;
			break;
	
		//mkr_bad
		default:
			return 1;
			break;
		}
		break;


	case 1:
		switch(mkr_frame){
		//mkr_per
		case 16:
		case 15:
		case 14:
			return 4;
			break;

		//mkr_great
		case 17:
		case 13:
		case 12:
			return 3;
			break;

		//mkr_good
		case 19:
		case 18:
		case 11:
		case 10:
			return 2;
			break;

		//mkr_good
		case 9:
		case 20:
			return 2;
			break;
	
		//mkr_bad
		default:
			return 1;
			break;
		}
		break;

	
	case 2:
		switch(mkr_frame){
		//mkr_per
		case 16:
		case 15:
		case 14:
		case 13:
			return 4;
			break;

		//mkr_great
		case 18:
		case 17:
		case 12:
		case 11:
			return 3;
			break;

		//mkr_good
		case 20:
		case 19:
		case 10:
		case 9:
			return 2;
			break;

		//mkr_good
		case 8:
		case 21:
			return 2;
			break;
	
		//mkr_bad
		default:
			return 1;
			break;
		}
		break;
	}
	return 1;
}
bool CLoadFumen::check_end( void ){
	if(CheckSoundMem(ef_select_bgm)==0 && before_note>=cnt_note-1)return true;
	return false;
}
#define OFF_SET	1000
void CLoadFumen::show_mkr( void ){
	double p_tmp;
	for(int note=0 ; note<cnt_note ; note++ ){
		for(int i=0;i<16;i++){
			if(m_obj_data[ note ].pos[ i ]==true ){
				
				if(m_obj_data[ note ].anime_pattern[i]==0){
					p_tmp = m_obj_data[ note ].time - m_ht_mkr.get_high_timer() + MKR_MS*10 ;
					if( p_tmp > -MKR_MS*10 && p_tmp <=MKR_MS*14 ){
						get_bpm_now_playing = note;
						int mkr_frame = (p_tmp>=0)?14 - (int)floor(p_tmp/MKR_MS):15 + (int)floor(-p_tmp/MKR_MS);
						if(  before_note != note &&  (int)floor(p_tmp/MKR_MS) == 0){
							//play_ef_hand();
							before_note = note;
						}
						draw_mkr( i+1 , mkr_frame , p_mkr_anime[0][now_select_mkr] );
						if(mkr_frame == 15 || CheckHitKey(key_code[i])==1 || touch_btn_flag[i]==true){
							m_obj_data[ note ].anime_pattern[i]=get_mkr_anime_pattern(mkr_frame,1);
							if(m_obj_data[ note ].anime_pattern[i]==4)cnt_per_note++;
							if(cnt_per_note==cnt_all_note){
								exc_flag=true;
								exc_cnt = 0;
							}
							switch(m_obj_data[ note ].anime_pattern[i]){
							case 4:
								shutter_open +=	(double)2048/cnt_all_note;
								combo_anime_flag=true;
								score += 900000/cnt_all_note;
								now_combo++;
								break;
							case 3:
								shutter_open +=	(double)2048/cnt_all_note;
								combo_anime_flag=true;
								score += 900000/(cnt_all_note*70);
								now_combo++;
								break;
							case 2:
								shutter_open +=		(double)1024/cnt_all_note;
								combo_anime_flag=true;
								score += 900000/(cnt_all_note*40);
								now_combo++;
								break;
							case 1:
								shutter_open -= 	(double)8192/cnt_all_note;
								now_combo=0;
								break;
							}
							m_obj_data[ note ].anime_start_time[i]=m_ht_mkr.get_high_timer();
						}else if( mkr_frame ==24 )now_combo=0;
					}
				}else{
					p_tmp = m_ht_mkr.get_high_timer() - m_obj_data[ note ].anime_start_time[i];
					if( p_tmp>=0 && p_tmp <= img_mkr[now_select_mkr].mkr_frame*24 ){
						int mkr_frame = (int)floor(p_tmp/img_mkr[now_select_mkr].mkr_frame);
						//SetDrawBlendMode( DX_BLENDMODE_ADD  , 255 );
						draw_mkr( i+1 , mkr_frame , p_mkr_anime[m_obj_data[ note ].anime_pattern[i]][now_select_mkr] );
						//SetDrawBlendMode( DX_BLENDMODE_NOBLEND , 255 );

					}
				}
			}
		}
	}
}
int  CLoadFumen::play(void){
	getKey();



	if(keyBuff[KEY_INPUT_UP]==1)if(now_select_mkr<10)now_select_mkr++;
	if(keyBuff[KEY_INPUT_DOWN]==1)if(now_select_mkr>0)now_select_mkr--;
	

	SetDrawArea(0,0,DRAW_GRAPG_SIZE,DRAW_GRAPG_SIZE);
	switch(play_pattern){ 
	case 0:
		start_shutter_move = 540-sin(PI/2/50*start_shutter_cnt)*540;
		DrawRotaGraph( DRAW_GRAPG_SIZE/2 , DRAW_GRAPG_SIZE/2 , 1.0 , 0.0 ,img_back_nor , false );		
		DrawExtendGraphF(  0  , (float) start_shutter_move  ,  (float)DRAW_GRAPG_SIZE  ,  (float)(DRAW_GRAPG_SIZE+(float) start_shutter_move)  ,  img_bg_buttom  ,  true  );
		DrawExtendGraphF(  0  ,  -(float) start_shutter_move  ,  (float)DRAW_GRAPG_SIZE  ,  (float)(DRAW_GRAPG_SIZE-(float) start_shutter_move)  ,  img_bg_top  ,  true  );
		if(start_shutter_move>0)start_shutter_cnt++;
		if((int)start_shutter_move<=0){
			play_pattern++;
			m_ht_bgm.reset_time();
			m_ht_mkr.reset_time();
			img_start_pos_frame = 0;
			img_start_pos_alpha = 0;
		}
		break;


	case 1:
		//アニメーション計算
		calc_tmp_shatter = shutter_open*((double)550/1024);
		tmp = 60/(m_obj_data[ get_bpm_now_playing ].now_bpm/4);
		shutter_sin = cos(PI*2/((tmp/(double)4)*1000)*m_ht_bgm.get_high_timer())*4.1;


		
		calc_sin = cos(PI*2/((tmp/(double)4)*1000)*m_ht_bgm.get_high_timer())*0.02;
		DrawRotaGraph( DRAW_GRAPG_SIZE/2 , DRAW_GRAPG_SIZE/2 , (0.95+calc_sin)*((double)1080/DRAW_GRAPG_SIZE) , 0.0 ,img_back_nor, false );
		//背景
		//シャッター
		DrawExtendGraphF(  -(float)shutter_sin  ,  (float)(calc_tmp_shatter+shutter_sin)  ,  (float)DRAW_GRAPG_SIZE+(float)shutter_sin  ,  (float)(DRAW_GRAPG_SIZE+calc_tmp_shatter+shutter_sin)  ,  img_bg_buttom  ,  true  );
		DrawExtendGraphF(  -(float)shutter_sin  ,  -(float)(calc_tmp_shatter+shutter_sin)  ,  (float)DRAW_GRAPG_SIZE+(float)shutter_sin  ,  (float)(DRAW_GRAPG_SIZE-calc_tmp_shatter-shutter_sin)  ,  img_bg_top  ,  true  );

		//コンボ
		calcNum=now_combo;
		numCount=0;
		combo_anime = (int)(cmbo_anime_move-10);
		if(now_combo>=15){
			for(numCount=1;(calcNum/=10)!=0;)numCount++;
			if(numCount==2){
				DrawGraph(260,combo_anime,*(img_combo_font+(now_combo/10)%10),true);
				DrawGraph(490,combo_anime,*(img_combo_font+(now_combo/1)%10),true);
				DrawGraph(520,570,img_combo,true);
			}
			if(numCount==3){
				DrawGraph(190,combo_anime,*(img_combo_font+(now_combo/100)%10),true);
				DrawGraph(380,combo_anime,*(img_combo_font+(now_combo/10)%10),true);
				DrawGraph(580,combo_anime,*(img_combo_font+(now_combo/1)%10),true);
				DrawGraph(520,570,img_combo,true);
			}
			if(numCount==4){
				DrawGraph(30,combo_anime,*(img_combo_font+(now_combo/1000)%10),true);
				DrawGraph(260,combo_anime,*(img_combo_font+(now_combo/100)%10),true);
				DrawGraph(490,combo_anime,*(img_combo_font+(now_combo/10)%10),true);
				DrawGraph(710,combo_anime,*(img_combo_font+(now_combo/1)%10),true);
				DrawGraph(740,570,img_combo,true);
			}
		}
		if(combo_anime_flag==true){
			if(cmbo_anime_move<=294)combo_anime_flag=false;
			cmbo_anime_move-=3;
		}else{
			if(cmbo_anime_move<300)cmbo_anime_move+=3;
		}

		//マーカー、BGMのオフセット待ち
		if( bgm_offset_flag == true ){
			if(  bgm_offset+OFF_SET  <=  m_ht_bgm.get_high_timer()  ){
				PlaySoundMem(  ef_select_bgm , DX_PLAYTYPE_BACK  );
				start_flag = false;
				ChangeVolumeSoundMem( 255 , ef_select_bgm );
				SetFrequencySoundMem( bgm_sanple , ef_select_bgm);
				bgm_offset_flag = false;
				m_ht_bgm.reset_time();
			}
		}
		if( mkr_offset_flag == true ){
			if(  mkr_offset+(OFF_SET-500)  <=  m_ht_mkr.get_high_timer()  ){
				mkr_offset_flag = false;
				m_ht_mkr.reset_time();
			}
		}else{
			show_mkr();
			if(shutter_open<0)shutter_open=0;
			if(shutter_open>1024)shutter_open=1024;
		}

		//オフセット調整
		switch(  revision_pattern  ){
		case 0:
			if( keyBuff[KEY_INPUT_RETURN] == 0 )break;
			if( (GetMouseInput()&MOUSE_INPUT_LEFT)!=0  )revision_pattern=1;
			if( (GetMouseInput()&MOUSE_INPUT_RIGHT)!=0  )revision_pattern=2;
			break;
		case 1:
			if( (GetMouseInput()&MOUSE_INPUT_LEFT)==0  )revision_pattern=3;
			bgm_offset += GetMouseWheelRotVol()*10;
			if(bgm_offset<0)bgm_offset=0;
			DrawFormatString(  300  ,  570  ,  GetColor(255,0,0)  , "bgm_offset=%d",bgm_offset);
			DrawFormatString(  300  ,  600  ,  GetColor(255,0,0)  , "mkr_offset=%d",mkr_offset);
			break;


		case 2:
			if( (GetMouseInput()&MOUSE_INPUT_RIGHT)==0  )revision_pattern=3;
			mkr_offset += GetMouseWheelRotVol()*10;
			if(mkr_offset<0)mkr_offset=0;
			DrawFormatString(  300  ,  570  ,  GetColor(255,0,0)  , "bgm_offset=%d",bgm_offset);
			DrawFormatString(  300  ,  600  ,  GetColor(255,0,0)  , "mkr_offset=%d",mkr_offset);
			break;

		case 3:
			for(int i=0;i<cnt_note;i++){
				for(int j=0;j<16;j++){
					m_obj_data[i].anime_pattern[j]=0;
					m_obj_data[i].anime_start_time[j]=0;
				}
			}
			m_ht_bgm.reset_time();

			m_ht_mkr.reset_time();
			revision_pattern = 0;
			shutter_open = 0;
			
			play_pattern = 0;
			//PlaySoundMem(ef_go , DX_PLAYTYPE_BACK);

			exc_flag = false;
			exc_pos = 475;
			exc_cnt = 0;

			cnt_per_note = 0;
			score = 0;

			mkr_offset_flag = bgm_offset_flag = true;
			if(CheckSoundMem(ef_select_bgm)==1)StopSoundMem(ef_select_bgm);
			start_flag = true;
			break;
		}


		if(start_flag == false && CheckSoundMem(ef_select_bgm)==0){
			score += (int)(100000 * 1024 / shutter_open) ;
			start_shutter_cnt = 0;
			play_pattern++;
		}
		break;


		case 2:
		start_shutter_move = 540-sin(PI/2/50*start_shutter_cnt)*540;
		DrawRotaGraph( DRAW_GRAPG_SIZE/2 , DRAW_GRAPG_SIZE/2 , 1.0 , 0.0 ,img_back_nor , false );		
		DrawExtendGraphF(  0  , (float) start_shutter_move  ,  (float)DRAW_GRAPG_SIZE  ,  (float)(DRAW_GRAPG_SIZE+(float) start_shutter_move)  ,  img_bg_buttom  ,  true  );
		DrawExtendGraphF(  0  ,  -(float) start_shutter_move  ,  (float)DRAW_GRAPG_SIZE  ,  (float)(DRAW_GRAPG_SIZE-(float) start_shutter_move)  ,  img_bg_top  ,  true  );
		if(start_shutter_move>0)start_shutter_cnt++;
		if(start_shutter_move<=0){
			play_pattern++;
			m_ht_bgm.reset_time();
			m_ht_mkr.reset_time();
			img_start_pos_frame = 0;
			img_start_pos_alpha = 0;
		}
		if( keyBuff[KEY_INPUT_RETURN] == 0 )break;
		if( (GetMouseInput()&MOUSE_INPUT_LEFT)!=0  )play_pattern=0;
		if( (GetMouseInput()&MOUSE_INPUT_RIGHT)!=0  )play_pattern=0;
		start_flag = true;
		break;
	}

	
	DrawFormatString(50,50,GetColor(255,0,0),"bpm = %d",(int)m_obj_data[ get_bpm_now_playing ].now_bpm);
	DrawFormatString(50,70,GetColor(255,0,0),"score = %d",score);
	if(exc_flag==true){
		exc_pos = (int)(sin( PI*2 / 420 * exc_cnt ) * 475);
		if(exc_pos>=475){
			exc_cnt = 0;
			exc_flag=false;
		}
		else exc_cnt++;
	}
	SetDrawBlendMode(DX_BLENDMODE_ADD,255-(int)(((double)255/475)*exc_pos));
	DrawExtendGraph(0-exc_pos,0,950-exc_pos,950,exc_left,true);
	DrawExtendGraph(0+exc_pos,0,950+exc_pos,950,exc_right,true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,255);
	SetDrawAreaFull();
	return 0;
}
CLoadFumen::~CLoadFumen(void){

	char get_tmp[512];
	char make_bof_str[512];
	char make_mof_str[512];
	FILE *old_fp;
	FILE *new_fp;
	old_fp = fopen(file_path,"r");
	new_fp = fopen("data/tmp","w");
	sprintf( make_bof_str , "bgm_offset=%d\n",bgm_offset);
	sprintf( make_mof_str , "mkr_offset=%d\n",mkr_offset);
	DeleteGraph(exc_left);
	DeleteGraph(exc_right);
	fputs(make_bof_str,new_fp);
	fputs(make_mof_str,new_fp);

	while (fgets(get_tmp, 512, old_fp) != NULL){
		if(strstr(get_tmp,"bgm_offset=")==NULL && strstr(get_tmp,"mkr_offset=")==NULL)fputs(get_tmp,new_fp);
	}

	fclose(old_fp);
	fclose(new_fp);
	remove(file_path);
	rename("data/tmp",file_path);
	if(m_obj_data!=NULL)free(m_obj_data);
	

	for(int i=0;i<HNDCLP;i++)DeleteSoundMem(ef_hcp[i]);
	DeleteSoundMem(ef_select_bgm);
	DeleteGraph(img_bg_buttom);
	DeleteGraph(img_bg_top);
	DeleteGraph(img_combo);
	DeleteGraph(img_back_nor);
	DeleteGraph(img_back_max);
	DeleteGraph(img_combo_font[0]);
	

	for(int i=0;i<11;i++){
		DeleteGraph(img_mkr[i].mkr_bad[0]);
		DeleteGraph(img_mkr[i].mkr_good[0]);
		DeleteGraph(img_mkr[i].mkr_great[0]);
		DeleteGraph(img_mkr[i].mkr_nor[0]);
		DeleteGraph(img_mkr[i].mkr_per[0]);
	}
}


int CLoadFumen::check_str(char get_c){
		const char  pos_name[32]={'1','2' ,'3' ,'4' ,'5' ,'6' ,'7' ,'8' ,'9' ,'a' ,'b' ,'c' ,'d' ,'e' ,'f' ,'g' ,'h','i' ,'j' ,'k' ,'l' ,'m' ,'n' ,'o' ,'p' ,'q' ,'r' ,'s' ,'t' ,'u' ,'v' ,'w'};
		for(int i=0;i<32;i++)if(  pos_name[i]  ==  get_c  )return i;
		return -1;
}


void CLoadFumen::play_ef_hand(void){
		PlaySoundMem( ef_hcp[  (now_play_hcp<HNDCLP-1)?now_play_hcp++:now_play_hcp=0  ],DX_PLAYTYPE_BACK);
}