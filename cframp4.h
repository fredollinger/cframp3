/* $Id: cframp4.h,v 1.23 2006/05/21 17:41:16 bl0rg Exp $
 * cframp3 - command line mp3 front-end in C++
 * rewritten from the perl version framp3
 * both by Fred Ollinger (follinge@diadig.com)
 */ 

/*
#ifndef MAGIC
#define MAGIC "/usr/share/misc/file/magic" 
#endif
*/

#ifndef CFRAMP4_H
#define CFRAMP4_H
//#include <magic.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <pthread.h>
#include <wchar.h>
#include <pcre.h>
#include <cdaudio.h>
//#include "SDL_thread.h"

#define TRUE 1
#define FALSE -1

// define file types
#define FRAM_ERR -1
#define FRAM_DIR 0
#define FRAM_NULL -2
#define FRAM_MP3 1
#define FRAM_OGG 2
#define FRAM_WAVE 3
#define FRAM_MPEG 4

// cfram3lib.c
int fram_ls();
int fram_ls2();
int fram_ls_glob(char*);
int get_song_name(int, char*);
int fram_strlen(char*);
int isogg(char*);

// cframp3engine.c:
int frameng_songdone();
//void *fram_engineon(void *ptr)
#endif
// $Id: cframp4.h,v 1.23 2006/05/21 17:41:16 bl0rg Exp $
