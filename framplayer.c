/* $Id: framplayer.c,v 1.27 2006/12/28 01:07:33 bl0rg Exp $
 * framplayer.c - a lib to play songs
 * cframp3 - command line mp3 front-end in C++
 * rewritten from the perl version framp3
 * both by Fred Ollinger (follinge@diadig.com)
 */ 

#include "framplayer.h"
#include "cframp4.h"

Mix_Music *fram_music = NULL;
/* We're going to be requesting certain things from our audio
     device, so we set them up beforehand */
int fram_audio_rate = 22050;
Uint16 fram_audio_format = AUDIO_S16; /* 16-bit stereo */
int fram_audio_channels = 2;
int fram_audio_buffers = 4096;
int fram_paused = 0;

/* This is the function that we told SDL_Mixer to call when the music
 *    was finished. In our case, we're going to simply unload the music
 *       as though the player wanted it stopped.  In other applications, a
 *          different music file might be loaded and played. */
void fram_musicDone() {
	//printf("framplayer.fram_musicDone() called. Cleaning up.\n");
	int err;
	Mix_HaltMusic();
	Mix_FreeMusic(fram_music);
	fram_music = NULL;
	frameng_songdone();
}

// initialize the sound system
int fram_sound_init()
{
  cframp3lib_init();
  // BEGIN SDL INIT
  SDL_Init(SDL_INIT_AUDIO);
    /* This is where we open up our audio device.  Mix_OpenAudio takes
	 *      as its parameters the audio format we'd /like/ to have. */
    if(Mix_OpenAudio(fram_audio_rate, fram_audio_format, fram_audio_channels, fram_audio_buffers)) {
			    printf("Unable to open audio!\n");
				    exit(1);
					  }

	  /* If we actually care about what we got, we can ask here.  In this
	   *      program we don't, but I'm showing the function call here anyway
	   *           in case we'd want to know later. */
	  Mix_QuerySpec(&fram_audio_rate, &fram_audio_format, &fram_audio_channels);
  // END SDL INIT
	 cd_init_device("/dev/cdrom");
}

int fram_sound_quit()
{
		  /* This is the cleaning up part */
		  Mix_CloseAudio();
		  SDL_Quit();
}

int fram_song_rewind()
{
		Mix_RewindMusic();
		return 0;
}

int fram_song_pause()
{
		if (0 == fram_paused)
		{
			printf("pausing \n");
			Mix_PauseMusic();
			fram_paused = 1;
		}
		else
		{
			printf("unpausing \n");
			Mix_ResumeMusic();
			fram_paused = 0;
		}
}

int fram_song_stop()
{
	/* Stop the music from playing */
	Mix_HaltMusic();

	/* Unload the music from memory, since we don't need it
	* 	   anymore */
	Mix_FreeMusic(fram_music);
					
	fram_music = NULL;
	return 0;
}

int framplay_song(char *song)
{
	printf("framplay_song() playing ***%s***.\n", song);
	fram_music = Mix_LoadMUS(song);

	/* This begins playing the music - the first argument is a
	   pointer to Mix_Music structure, and the second is how many
	   times you want it to loop (use -1 for infinite, and 0 to
	   have it just play once) */
	Mix_PlayMusic(fram_music, 0);

	/* We want to know when our music has stopped playing so we
	   can free it up and set 'music' back to NULL.  SDL_Mixer
	   provides us with a callback routine we can use to do
	   exactly that */
	Mix_HookMusicFinished(fram_musicDone);
	return 0;
}

/* $Id: framplayer.c,v 1.27 2006/12/28 01:07:33 bl0rg Exp $ */
