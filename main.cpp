//  DX Library Copyright (C) 2001-2014 Takumi Yamada.

#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif
#include <DxLib.h>
#include "system.h"
#include "MainGame.h"
#include "LoadFumen.h"
#include "Fps.h"
#include <windows.h>    // included for Windows Touch
#include <windowsx.h>   // included for point conversion


//

#define MAXPOINTS 10
int cnt_touch;
bool touch_btn_flag[16];
POINT ptInput;


LRESULT CALLBACK WndProc( HWND WindowHandle, UINT Message, WPARAM wParam, LPARAM lParam ){
	switch( Message ){
	case WM_TOUCH:
		int inputs= LOWORD( wParam );
		TOUCHINPUT  tbuf[ 10 ]; // inputs
		HTOUCHINPUT	hinput= reinterpret_cast<HTOUCHINPUT>( lParam );


		if( GetTouchInputInfo( hinput, inputs, tbuf, sizeof(TOUCHINPUT) ) ){
			TOUCHINPUT*  tp= tbuf;
			for( int i= 0 ; i< inputs && i < 10 ; i++, tp++ ){

				ptInput.x = TOUCH_COORD_TO_PIXEL(tp->x);
				ptInput.y = TOUCH_COORD_TO_PIXEL(tp->y);
				ScreenToClient(GetMainWindowHandle(), &ptInput);

				if(!(tp->dwFlags & TOUCHEVENTF_UP)){
					for( int j = 0 ; j < 16 ; j++ )
						if( ptInput.x >= getButtonPosX(j+1) && ptInput.y >= getButtonPosY(j+1) && 
							ptInput.x < getButtonPosX(j+1)+(int)(211*DRAW_EXTEND_RATE) && ptInput.y < getButtonPosY(j+1)+(int)(211*DRAW_EXTEND_RATE) )touch_btn_flag[j]=true;
				}else{
					for( int j = 0 ; j < 16 ; j++ )
						if( ptInput.x >= getButtonPosX(j+1) && ptInput.y >= getButtonPosY(j+1) && 
							ptInput.x < getButtonPosX(j+1)+(int)(211*DRAW_EXTEND_RATE) && ptInput.y < getButtonPosY(j+1)+(int)(211*DRAW_EXTEND_RATE) )touch_btn_flag[j]=false;
				}
			}
		}
		CloseTouchInputHandle( hinput );
		break;
	}
	return 0;
}






int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow ){

#if _DEBUG
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
#endif
	for(int i = 0 ; i < 16 ; i++ ){
		touch_btn_flag[i]=false;
	}
	set_window();

	// フック用ウインドウプロージャの登録
	SetHookWinProc( WndProc ) ;
	RegisterTouchWindow(GetMainWindowHandle(),0);

	CMainGame m_MainGame;
	CFps m_FpsCtrl;
	while( !ProcessMessage() && !ClearDrawScreen() && !clsDx() ){
		if(  CheckHitKey(  KEY_INPUT_ESCAPE  )  ==  1  )break;
		m_FpsCtrl.Update();
		m_MainGame.Process();
		m_FpsCtrl.Draw();
		ScreenFlip();
		m_FpsCtrl.Wait();
	}
	DxLib_End();
	return 0;
}