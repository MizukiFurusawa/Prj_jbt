#include "my_debug.h"
void dmsg( LPCSTR pszFormat, ...){
	va_list	argp;
	char pszBuf[ 256];
	va_start(argp, pszFormat);
	vsprintf( pszBuf, pszFormat, argp);
	va_end(argp);
	MessageBox( NULL, pszBuf, "", MB_OK);
}
