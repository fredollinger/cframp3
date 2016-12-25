/* $Id: cframp3.c,v 1.68 2006/12/28 01:07:33 bl0rg Exp $
 * cframp3 - command line mp3 front-end in C++
 * rewritten from the perl version framp3
 * both by Fred Ollinger (follinge@gmail.com)
*/

#include "cframp4.h"

#ifdef USE_GROOVE
#include <groove/groove.h>
#endif // USE_GROOVE

// FUNCTION DECLARATIONS
void *fram_engine( void *ptr );

// GLOBALS
char *glob_song; // contains the song name
int glob_run_engine;  //play the song?
int glob_num; // song number
int glob_num = 1; // default so that if we call on this before choosing a song we won't crash
char *volcmd="/usr/bin/alsamixer";

void fram_show_lyrics()
{
    int n, i;
    char *songn;
    char *comm;
    songn = (char *) malloc (1000);
    comm = (char *) malloc (1000);

    n = frameng_getsongnum();
    get_song_name(n, songn);
    //printf("song name: ***%s*** \n ", songn);
    strtok(songn, ".");
    // need to strip out minus sign here
    // first print all the chars in songn
    for(i = 0;i < strlen(songn);i++)
    {
        if (! isalpha(songn[i]))
        {
            //printf("%i char: %c, is alpha: %i\n", i, songn[i], isalpha(songn[i]));
            songn[i] = ' ';
        }
    }
    //printf(comm, "/usr/lib/surfraw/google -browser=/usr/bin/links -q -l lyrics  %s", songn);
    sprintf(comm, "/usr/lib/surfraw/google -browser=/usr/bin/lynx -q -l lyrics  %s", songn);
    system(comm);
    return;
}

// CALLERS
// cframp3.c: main()
int volume_control()
{
    int status; // status of process
    system(volcmd);
    wait (&status);
    return 0;
}

// print a help message
void fram_help()
{
            printf("cframp3 ver: 0.2\n");
            printf("any number: play that song\n");
            printf("a: pAuse a track\n");
            printf("b: Currently playing\n");
            printf("c: Change dir\n");
            printf("e: tEst playlist\n");
            printf("f: Find a track by name\n");
            printf("g: Go play a song now\n");
            printf("i: use playlIst\n");
            printf("l: List the directory\n");
            printf("m: Mention Status\n");
            printf("r: Toggle contuRuous mode \n");
            printf("n: Next song queue\n");
            printf("o: cOunt songs\n");
            printf("o: print current directory\n");
            printf("t: Toggle random mode \n");
            printf("u: Print username \n");
            printf("s: Stop the music\n");
            printf("w: reWind to the beginning of the current song\n");
            printf("x: eXit\n");
            printf("y: show lYrics\n");
}

int main(int argc, char *argv[])
{
  #ifdef USE_GROOVE
  printf("groovy\n");
  groove_init();
  atexit(groove_finish);
  groove_set_logging(GROOVE_LOG_INFO);
  #endif // USE_GROOVE

    glob_run_engine = 0;  // don't start with a song playing
    char *my_string;
    char song[256];
    char *tail;
    char *mysong;
    char *first;
    char *playcmd; // this is the command that we are going to send to framp3_engine
    char *tmp_string; // this is the command that we are going to send to framp3_engine

    int err;
    size_t csize;

    long songnum;
    long ifirst;
    long nfirst;

    FILE *stream;
    FILE *commfile;

    pid_t pud;


    // make sure that the cframp3 playing engine is stopped initiall
    // so we don't start with having a song playing
    glob_run_engine = -1;

    first = (char *) malloc (2);
    playcmd = (char *) malloc (10);
    mysong = (char *) malloc (300);
    tmp_string = (char *) malloc (256);
    glob_song = (char *) malloc (1000);


    // get the old working directory and change it
    char *fram_confile = (char *) malloc (1000);
    //fram_confile = (char *) malloc (1000);
    cframp3lib_getowd(fram_confile);
    err = chdir(fram_confile);

    // initialize the sound library if it needs it
    // if not then then this will just do nothing
    fram_sound_init();


    // BEGIN THREAD initialization
    frameng_init();

    // MAIN LOOP
    // just wait for keyboard input
    while (1)
    {
        // get song or command choice
        if (my_string)
        {
        //printf("freeing my_string \n");
              //free (my_string);
              my_string = (char *)NULL;
        }

        my_string = readline ("\n mp3> ");
        add_history (my_string);

        strncpy (first, my_string, 1);
        first[1]='\0';
        ifirst = strtoul(my_string, &tail, 0);
        //printf ("ifirst: %i  first: %s mystring: %s %i \n", ifirst, first, my_string, sizeof(first));

        if (0 == strcmp (first, "?"))
        {
                fram_help();
        }

        if (0 == strcmp (first, "b"))
        {
            int num;
            num = frameng_getsongnum();
            get_song_name(num, glob_song);
            printf("currently playing %i: %s\n", num, glob_song);
        }

        if (0 == strcmp (first, "c"))
        {
            tmp_string = strtok (my_string, " ");
            tmp_string = strtok (NULL, " ");
            printf("dir: ***%s***", tmp_string);
            frameng_noncontinuous();
            err = chdir(tmp_string);
            printf("dir: %i", err);
        }

        if (0 == strcmp (first, "d"))
        {
                glob_run_engine = 1;
        }

        // find a track by name
        if (0 == strcmp (first, "f"))
        {
            tmp_string = strtok (my_string, " ");
            tmp_string = strtok (NULL, " ");
            // avoid a segfault
            // don't call the fuction if we are going to a NULL!
            if (NULL == tmp_string)
            {
                printf("You must give a track to find!\n");
            }
            else
            {
                printf("searching for track: ***%s***", tmp_string);
                fram_ls_grep(tmp_string);
            }
        }


        if (0 == strcmp (first, "g"))
        {
                printf("main(): !\n");
                // set the song number
                //frameng_setsongnum(-1);
                glob_run_engine = 1;  // stop the engine
                frameng_play();
        }

        if (0 == strcmp (first, "e"))
        {
            printf("tEsting playlist... \n");
            cframp3_show_playlist ();
        }

        if (0 == strcmp (first, "i"))
        {

            printf("loading playlist... \n");
            tmp_string = strtok (my_string, " ");
            tmp_string = strtok (NULL, " ");
            if (NULL == tmp_string)
                        {
                                printf("You must give a playlist to load!\n");
                        }
            else
            {
            printf("list: ***%s***", tmp_string);
            cframp3_load_playlist (tmp_string);
            frameng_playlist_on();
            frameng_setsongnum(1);
            glob_run_engine = 1;  // stop the engine
            frameng_play();
            //err = chdir(tmp_string);
            }
        }

        if (0 == strcmp (first, "l"))
        {
            fram_ls();
        }


        // queue Next song
        if (0 == strcmp (first, "n"))
        {
            tmp_string = strtok (my_string, " ");
            tmp_string = strtok (NULL, " ");
            // avoid a segfault
            // don't call the fuction if we are going to a NULL!
            if (NULL == tmp_string)
            {
                printf("You must give a track to play!\n");
            }
            else
            {
                /*
            my_string = readline ("\n mp3> ");
            add_history (my_string);
            strncpy (first, my_string, 1);
            */
                nfirst = strtoul(tmp_string, &tail, 0);

                //printf("adding track ***%i*** onto the queue\n", nfirst);
                // uncomment when this is fixed
                frameng_setnextsongnum(nfirst);
            }
        }

        if (0 == strcmp (first, "m"))
        {
            frameng_mention();
        }

        if (0 == strcmp (first, "o"))
        {
            fram_count_songs();
        }

        if (0 == strcmp (first, "p"))
        {
                fram_pwd();
        }

        if (0 == strcmp (first, "a"))
        {
                printf("pAuse selected. Press p to un pause\n");
                fram_song_pause();
        }

        // continue playing flag
        if (0 == strcmp (first, "r"))
        {
            frameng_continuous();
        }

        // toggle random mode
        if (0 == strcmp (first, "t"))
        {
            frameng_random();
        }

        if (0 == strcmp (first, "s"))
        {
                printf("main(): stopping the song!\n");
                glob_run_engine = 0;  // stop the engine
                frameng_noncontinuous();
                frameng_stop();
        }


        if (0 == strcmp (first, "u"))
        {
            //char *fram_confile = (char *) malloc (1000);
            fram_confile = (char *) malloc (1000);
            //cframp3_getconfigfile(fram_confile);
            cframp3lib_getowd(fram_confile);
            printf("owd: %s \n", fram_confile);
        }

        if (0 == strcmp (first, "v"))
        {
            volume_control();
            printf("returning from umix!\n");
        }

        if (0 == strcmp (first, "w"))
        {
            printf ("reWinding the song \n");
            fram_song_rewind();
        }

        if (0 == strcmp (first, "x"))
        {
            cframp3lib_writecwd();
            cframp3lib_quit();
            frameng_quit();
            exit(0); // quit - ending now
        }


        if (0 == strcmp (first, "y"))
        {
            int n;
            n = frameng_getsongnum();
            fram_show_lyrics(n);
        }

        // play a track by name
        // FIXME: this is broken
        // I need to do this in a another fashion.
        /*
        if (0 == strcmp (first, "t"))
        {
            tmp_string = strtok (my_string, " ");
            tmp_string = strtok (NULL, " ");
            printf("playing track: ***%s***", tmp_string);
            //cframp3_play(tmp_string);
        }
        */

        if (ifirst > 0)
        {
                // so we stop the song.
                // Yes, this is bad...
                /*
                if (1 == glob_run_engine)
                {
                    frameng_stop();
                }
                */

            songnum = strtoul(my_string, &tail, 0);

            get_song_name(songnum, mysong);
            get_song_name(songnum, glob_song);
            glob_num = songnum;
            frameng_random_off();
            frameng_continuous_on();
            frameng_stop();
            frameng_setsongnum(songnum);
            glob_run_engine = 1;
            frameng_play();
            }

    }
return 0;
}
// Sat May 10 11:43:33 PDT 2014
