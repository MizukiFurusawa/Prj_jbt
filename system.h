#pragma once
#include <DxLib.h>
#include "ComDef.h"
void set_window(void){
	SetOutApplicationLogValidFlag(false);
	SetGraphMode				(1080,1080,WINDOW_BITS);

	//SetWindowPosition(300,-20);
	SetWindowSize(720,720);
	SetWindowStyleMode(0);
	SetBackgroundColor		(0,0,0);
	SetMainWindowText		(  "JubeatPlayer"  );
	ChangeWindowMode	(  true  );
	DxLib_Init						(  );
	SetDrawScreen				(  DX_SCREEN_BACK  );
	SetOutApplicationLogValidFlag(  FALSE  );
	SetAlwaysRunFlag			(  TRUE  );
}