/* $Id: cframp3engine.c,v 1.16 2006/09/05 00:50:37 bl0rg Exp $
 * cframp3engine.c - threading wrapping library
 * rewritten from the perl version framp3
 * both by Fred Ollinger (follinge@diadig.com)
 */ 

#include "cframp4.h"

// globals
pthread_mutex_t frameng_mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t frameng_mutex2 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t frameng_cond1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t frameng_cond2 = PTHREAD_COND_INITIALIZER;
pthread_t frameng_thread1;

/*
 0 = regular play (next song)
 1 = random play
 2 = next song mode
 3 = use playlist
*/

enum frameng_mode_num {REGULAR_PLAY, RANDOM_PLAY, NEXT_SONG_PLAY, PLAYLIST_PLAY}; 

int frameng_next = 1; // whether we are on continuous play or not
int frameng_num = -1; // number cooresponding to song
int frameng_nsong = 0; // next song to play
int frameng_mode = 0; //  which mode 0 = normal, 1 = next song mode
int iret1;  // for threading
int frameng_err;
int frameng_numsongs;

char *frameng_song; // contains the song name

// functions
void *frameng_engineon(void *ptr); // pointer to the engine

// call this to start the engine!
int frameng_init()
{
	printf("frameng_init() the engine is ON!\n");
	frameng_song = (char *) malloc (1000);
	frameng_next = 0; // whether we are on continuous play or not
	frameng_num = 1; // number cooresponding to song

	iret1 = pthread_create( &frameng_thread1, NULL, frameng_engineon, (void*) frameng_song);
}

int frameng_quit()
{
	pthread_cancel (frameng_thread1); // this is not redudant b/c we need to kill the player, yes, I know that makes no sense...
	//pthread_kill_other_threads_np();
}

// the thread that actually plays the songs
void *frameng_engineon(void *ptr)
{


	printf("frameng_engineon() started\n");
	while(1)
	{
		// if we are non-continuous play then we should wait
		// else just play the next song
		printf("frameng_engineon(): next song is: %i \n", frameng_next);
		if (frameng_next == 0)
		{
			printf("frameng_engineon(): looping waiting on cond1 %s \n", frameng_song);
			pthread_cond_wait (&frameng_cond1, &frameng_mutex1);
		}
			
		// if we have a playlist, use it, otherwise, get the next song
		// from the directory
		if (PLAYLIST_PLAY == frameng_mode)
		{
			//frameng_err = cframp3_playlist_get_nth (frameng_num, frameng_song);
		}
		else
		{
			frameng_err = get_song_name(frameng_num, frameng_song);
		}

		printf("frameng_engineon(): song number for %i is %s \n", frameng_num, frameng_song);
		// if there's no error then we can actually play the song
		if (-1 != frameng_err)
		{
			printf("frameng_engineon(): playing %s \n", frameng_song);
			//printf("frameng_engineon(): error code %i \n", frameng_err);
			frameng_num=frameng_inc_song_num(frameng_num);
			framplay_song(frameng_song);
			printf("engine(): waiting for the man--I mean cond2.\n");
			pthread_cond_wait (&frameng_cond2, &frameng_mutex2);
			printf("ENGINE ON FRAMENG_NUM NOW %i.\n", frameng_num);
		}
		else
		{
			printf("frameng_engineon(): danger! song %i name not found\n", frameng_num);
			frameng_num = 1;
			printf("FRAMENG_NUM NOW %i.\n", frameng_num);
		}
	}
}

// increment the song number
// going to be used for the next song feature
int frameng_inc_song_num(int i)
{

	printf("frameng_inc_song_num frameng_mode = %i \n", frameng_mode);
	// if in random mode then return random song
	if (RANDOM_PLAY == frameng_mode)
	{
		int n;
		n = frameng_pick_random_song();
		return n;
	}

	if (NEXT_SONG_PLAY == frameng_nsong)
	{
		int n;
		n = frameng_nsong;
		frameng_nsong = 0;
		printf("next song mode: %i\n", n);
		return n;
	}

	// if there is no next song then we should just increment the song
	// number and return that:
	return ++i;
}

frameng_pick_random_song()
{
		float j;  // random number 0-1
		int f; // song number
		int n; // number of songs total
		n = fram_count_songs();
		srand (time (0));
		j = rand();
		f = ((j/RAND_MAX)*n)+1;
		printf("random mode: ");
		printf("%i\n",f);
		return f;
}

void frameng_getsongtest()
{
		get_song_name(frameng_num, frameng_song);
		printf("frameng_getsongtest(): song name %s\n", frameng_song);
}

void frameng_setsongnum(int num)
{
	frameng_num = num;
	printf("FRAMENG_SETSONGNUM(): frameng_num = %i \n", frameng_num);
	/*
	if (-1 == num)
	{
		frameng_num=frameng_inc_song_num(1);
		return;
	}
	frameng_num= num;
	*/
	return;

}

// just return the current song number
// nothing more, nothing less
int frameng_getsongnum()
{
	if (frameng_num < 2)
	{
		return 1;
	}
	return frameng_num-1;
}

// force non-continuous play
void frameng_noncontinuous()
{
	printf("frameng_init() non-continous play!\n");
	frameng_next = 0;
}


// force continuous play
void frameng_continuous_on()
{
	printf("frameng_init() continous play!\n");
	frameng_next = 1;
}

// toggle between continuous and non-continuous play
void frameng_continuous()
{
		if (frameng_next == 1)
		{
				printf("frameng_init() non-continous play!\n");
				frameng_next = 0;
		}
		else
		{
				printf("frameng_init() continous play!\n");
				frameng_next = 1;
		}

}


// turn random mode off 
void frameng_random_off()
{
	printf("random off \n");
	frameng_mode = REGULAR_PLAY;
}


// turn playlist mode on
void frameng_playlist_on()
{
	printf("entering playlist mode \n");
	frameng_mode = PLAYLIST_PLAY;
}

// toggle between continuous and non-continuous play
void frameng_random()
{
		if (RANDOM_PLAY == frameng_mode)
		{
				printf("frameng_init() non-random play!\n");
				frameng_mode = REGULAR_PLAY;
		}
		else
		{
				frameng_continuous_on(); 
				frameng_mode = RANDOM_PLAY;
				frameng_nsong = frameng_pick_random_song();
				frameng_num = frameng_nsong;
				printf("frameng_init() random play! %i %i \n", frameng_num, frameng_nsong);
		}

}

int frameng_songdone()
{
	frameng_err = pthread_cond_broadcast(&frameng_cond2);
}

int frameng_play()
{
			printf("frameng_play()!\n");
			frameng_err = pthread_cond_broadcast(&frameng_cond1);
			return 0;
}

void frameng_stop()
{
				fram_song_stop(); // tell the player to stop the song
				frameng_songdone(); // tell the engine that the song is done
}

//! Mention all the settings
void frameng_mention()
{
	printf("Mode is: %i \n", frameng_mode);
	printf("Currently Playing: %i %s \n", frameng_num, frameng_song);
	printf("Next song: %i \n", frameng_nsong);
}

void frameng_setnextsongnum(int num)
{
			frameng_nsong = num; // next song to play
			frameng_next = 1; // set continuous so next song gets played
			printf("framengine(): song is now %i \n", frameng_nsong);
}
