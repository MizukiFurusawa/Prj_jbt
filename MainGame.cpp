#include "DxLib.h"
#include "MainGame.h"
#include "ComDef.h"
#include <math.h>
extern bool touch_btn_flag[16];
extern int cnt_touch;

CMainGame::CMainGame(){
	font_handle = CreateFontToHandle( "meiryo",15,2,DX_FONTTYPE_ANTIALIASING_8X8);
	extend_handle = MakeScreen(DRAW_GRAPG_SIZE,DRAW_GRAPG_SIZE,false);
	img_touch = LoadGraph("rc/system/img_touch.png");

	
		img_wave_line = LoadGraph("rc/system/wave_line.png");
		img_wave_dot = LoadGraph("rc/system/wave_dot.png");
		for(int i=0;i<4;i++){
			before_data[i] = 0;
			sin_data[i].cnt =0;
			sin_data[i].y = 0;
		}


	cnt_music = 0;
	speed_pattern = 0;
	play_speed = 100;
	bg_color = 0;
	img_back = LoadGraph("jbt_rc/img_back.png");
	color_name[0]="blue";
	color_name[1]="black";
	color_name[2]="green";
	color_name[3]="pink";
	color_name[4]="red";
	color_name[5]="yellow";
	//曲数を数える

	HANDLE hFind;
	WIN32_FIND_DATA win32fd;
	hFind = FindFirstFile("data//*.txt", &win32fd);
	if (hFind != INVALID_HANDLE_VALUE){
		do {
			if ( !(win32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ) cnt_music++;
		} while (FindNextFile(hFind, &win32fd));
	}
	FindClose(hFind);

	//dmsg("cnt_music = %d",cnt_music);

	if(cnt_music<=0){
		MessageBeep(MB_ICONHAND);
		dmsg("ファイルロードエラー!!");
		FindClose(hFind);
		exit(EXIT_FAILURE);
	}

	file_name = new char *[cnt_music];
	for(int i=0;i<cnt_music;i++)file_name[i]=new char [512];

	
	int pos = 0;
	//win32fd.cFileName
	//楽曲名を取得する
	hFind = FindFirstFile("data//*.txt", &win32fd);
		if (hFind == INVALID_HANDLE_VALUE){
			MessageBeep(MB_ICONHAND);
			dmsg("ファイルロードエラー!!");
			FindClose(hFind);
			exit(EXIT_FAILURE);
		}
		do {
			if ( !(win32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ){
				strcpy( file_name[pos] , win32fd.cFileName );
				strtok( file_name[pos] , "." );
				pos++;
			}
		} while (FindNextFile(hFind, &win32fd));
		
	FindClose(hFind);
	top_img = LoadGraph("jbt_rc/top_img.png");
	pattern = 0;
	pos_y = 0;
	now_pos = 0;
	wait_timer = GetNowCount();
	fade_flag= false;
}

CMainGame::~CMainGame(){
	if(m_LoadFumen!=NULL)delete m_LoadFumen;
	for(int i=0;i<cnt_music;i++)delete[] file_name[i];
	delete [] file_name;
	DeleteGraph(extend_handle);
	DeleteGraph(top_img);
	DeleteGraph(img_back);
	
}

void CMainGame:: Calc(){

}

#define BRIGHT	510
#define BRIGHT_ 2
void CMainGame:: Draw(){



	if(fade_flag==false){
			int tmp = GetNowCount() - wait_timer;
			if(tmp>BRIGHT)tmp=BRIGHT;
			SetDrawBright(tmp/BRIGHT_,tmp/BRIGHT_,tmp/BRIGHT_);
	}
	
	if(fade_flag==true){
			int tmp = GetNowCount() - wait_timer;
			if(tmp>BRIGHT)tmp=BRIGHT;
			SetDrawBright(255-tmp/BRIGHT_,255-tmp/BRIGHT_,255-tmp/BRIGHT_);
	}








	switch( pattern ){
	case 0:
		for(int i=0;i<20;i++){
			DrawStringToHandle(10,80+30*i,file_name[i+pos_y],(now_pos==i)?GetColor(255,0,0):GetColor(50,50,50),font_handle);
		}
		switch(  speed_pattern  ){
			case 0:
				now_pos += -GetMouseWheelRotVol();
				if(now_pos<0){
					now_pos=0;
					if(pos_y>0)pos_y--;
				}
				if(now_pos>19){
					now_pos=19;
					if(pos_y<cnt_music-20)pos_y++;
				}
				if( (GetMouseInput()&MOUSE_INPUT_LEFT)!=0  )speed_pattern=1;
				if( (GetMouseInput()&MOUSE_INPUT_RIGHT)!=0  )speed_pattern=2;
				break;

			case 1:
				if( (GetMouseInput()&MOUSE_INPUT_LEFT)==0  )speed_pattern=0;
				play_speed += GetMouseWheelRotVol()*10;
				if(play_speed<10)play_speed=10;
				if(play_speed>200)play_speed=200;
				DrawFormatString(  300  ,  570  ,  GetColor(255,0,0)  , "play_speed=%d",play_speed);
				DrawFormatString(  300  ,  600  ,  GetColor(255,0,0)  , "color = %s",color_name[bg_color]);
				break;

			case 2:
				if( (GetMouseInput()&MOUSE_INPUT_RIGHT)==0  )speed_pattern=0;
				bg_color += GetMouseWheelRotVol();
				if(bg_color<0)bg_color=0;
				if(bg_color>5)bg_color=5;
				DrawFormatString(  300  ,  570  ,  GetColor(255,0,0)  , "play_speed=%d",play_speed);
				DrawFormatString(  300  ,  600  ,  GetColor(255,0,0)  , "color = %s",color_name[bg_color]);
				break;
			}

		if(CheckHitKey(KEY_INPUT_SPACE)==1){
			pattern++;
			wait_timer = GetNowCount();
		}
		break;

	case 1:
		if(GetNowCount() - wait_timer>=0)pattern++;
		for(int i=0;i<20;i++){
			DrawStringToHandle(10,80+30*i,file_name[i+pos_y],(now_pos==i)?GetColor(255,0,0):GetColor(50,50,50),font_handle);
		}
		break;

	case 2:
		m_LoadFumen= new CLoadFumen(bg_color,play_speed);
		if(m_LoadFumen->load_fumen(file_name[now_pos+pos_y])==-1){
			pattern=0;
			fade_flag=false;
			break;
		}
		pattern++;
		wait_timer = GetNowCount();
		break;

	case 3:
		m_LoadFumen -> play();
		if(CheckHitKey(KEY_INPUT_SPACE)==1)pattern = 4;
		break;



	case 4:
		m_LoadFumen -> play();
		if(CheckHitKey(KEY_INPUT_SPACE)!=1){
			wait_timer = GetNowCount();
			fade_flag = true;
			pattern++;
		}
		break;

	case 5:
		m_LoadFumen -> play();
		if(GetNowCount() - wait_timer>=1020)pattern++;
		break;

	case 6:
		if(m_LoadFumen!=NULL)delete m_LoadFumen;
		m_LoadFumen =NULL;
		pattern = 0;
		wait_timer = GetNowCount();
		fade_flag = false;
		break;
	}


	for(int i = 0 ; i < 16 ; i++ ){
		if( touch_btn_flag[i]==true )DrawExtendGraph( getButtonPosX(i+1),getButtonPosY(i+1),getButtonPosX(i+1)+(int)(211*DRAW_EXTEND_RATE),getButtonPosY(i+1)+(int)(211*DRAW_EXTEND_RATE),img_touch,true);
	}
	//DrawFormatString(50,75,GetColor(255,0,0),"cnt_touch = %d",cnt_touch);

}
void CMainGame::back_animetion(){
		for(int i=0;i<540;i++){
			DrawExtendGraph( i*2 , max_y[i]+1 , i*2+2 , 570 , img_wave_line , false );
			sin_data[0].y = (int)(530 + (float)(40 * sin(PI/120.0 * sin_data[0].cnt + 2*PI/1050 * i)));
			sin_data[1].y = (int)(530 + (float)(40 * sin(PI/150.0 * sin_data[1].cnt + 2*PI/650 * i)));
			sin_data[2].y = (int)(530 - (float)(40 * sin(PI/120.0 * sin_data[2].cnt + 2*PI/1050 * i)));

			DrawGraph(  i*2  ,  sin_data[0].y , img_wave_dot , false);
			DrawGraph(  i*2  ,  sin_data[1].y , img_wave_dot , false);
			DrawGraph(  i*2  ,  sin_data[2].y , img_wave_dot , false);
			max_y[i]=sin_data[0].y;
			for(int j=1;j<2;j++){
				if(max_y[i]<sin_data[j].y)max_y[i]=sin_data[j].y;
			}
		}
		now_line = 1 -  now_line;
		sin_data[0].cnt+=3;
		sin_data[1].cnt+=3;
		 sin_data[2].cnt+=2;
	}


void CMainGame:: Process(){

	SetDrawScreen(extend_handle);
	DrawBox(0,0,1920,1080,GetColor(255,255,255),true);

	SetDrawMode( DX_DRAWMODE_BILINEAR );
	Draw();
	SetDrawScreen( DX_SCREEN_BACK ) ;

	SetDrawMode( DX_DRAWMODE_BILINEAR );
	DrawRotaGraph(DRAW_WINDOW_SIZE/2,DRAW_WINDOW_SIZE/2,(float)DRAW_WINDOW_SIZE/DRAW_GRAPG_SIZE,0.0,extend_handle,false);
	SetDrawMode( DX_DRAWMODE_NEAREST );


	//SetDrawMode( DX_DRAWMODE_BILINEAR );
	//back_animetion();
	//DrawExtendGraph(1080+40,0+40,1920-40,1080-40,img_back,false);
	//SetDrawMode( DX_DRAWMODE_NEAREST );


	DrawExtendGraph(0,0,1920,1080,top_img,true); 
	SetDrawScreen(  DX_SCREEN_BACK  );
}