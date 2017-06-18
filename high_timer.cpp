#include "HighTimer.h"
	//bool check_high_timer_enable;

Chigh_timer::Chigh_timer(){
	high_timer_enable = QueryPerformanceFrequency( &get_bit );
	if( high_timer_enable==false )dmsg( "高解像度タイマーは使えません。");
    QueryPerformanceCounter( &start );
}

double Chigh_timer::get_high_timer(void){
	QueryPerformanceCounter( &now );
	return ((double)( now.QuadPart - start.QuadPart ) / (double)get_bit.QuadPart)*1000;
};


void Chigh_timer::reset_time(void){
    QueryPerformanceCounter( &start );
}