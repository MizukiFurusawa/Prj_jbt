#include <windows.h>
#include "my_debug.h"

class Chigh_timer{
private:
	BOOL high_timer_enable;
    LARGE_INTEGER start;
    LARGE_INTEGER now;
	LARGE_INTEGER get_bit;
public:
	Chigh_timer::Chigh_timer();
	void Chigh_timer::reset_time();
	double get_high_timer(void);
};