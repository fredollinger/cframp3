/* $Id: engtest.c,v 1.5 2005/02/12 23:10:04 follinge Exp $
 * cframp3 - command line mp3 front-end in C++
 * rewritten from the perl version framp3
 * both by Fred Ollinger (follinge@diadig.com)
 */ 

#include "cframp4.h"

int main(int argc, char *argv[])
{
	fram_sound_init();
	frameng_init();
	frameng_play();
	sleep(1);
	frameng_stop();
	sleep(5);
	frameng_play();
	sleep(10);
	return 0;
}

/* $Id: engtest.c,v 1.5 2005/02/12 23:10:04 follinge Exp $ */
