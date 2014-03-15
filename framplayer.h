/* $Id: framplayer.h,v 1.9 2005/02/07 13:29:45 follinge Exp $
 * cframp3 - command line mp3 front-end in C++
 * rewritten from the perl version framp3
 * both by Fred Ollinger (follinge@diadig.com)
 */ 

#ifndef FRAMPLAYER_H
#define FRAMPLAYER_H
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_mixer.h>

int framplay_song(char*);
int fram_volume_control();
#endif
