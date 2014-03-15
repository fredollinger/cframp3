#include <stdio.h>
#include <string.h>
#include "cframp4.h"

//char *glob_song; // contains the song name

int fram_sed(char *s, char ch)
{
	printf("framsed \n");
	int len;
	int rep;
	char *to;
	len = strlen(s);
	printf("malloc %i \n", len);
	to = (char *) malloc (1000);
	printf("stchr %s\n",s);
	to = strstr(*s, "b");
	return 0;
	rep = strlen(to);
	printf("char: %s found at pos: %i\n", to, rep);
}

int get_song_name(int n, char *song)
{
        //static char *copys="milkspoughts - breast mihlk.mp3";
        static char *copys="breast mihlk.mp3";
	//printf ("song lyrics: %i \n", song);
	strcpy(song, copys);
	return 0;
}

int frameng_getsongnum()
{
	return 5;
}

/*
// find a string backwards and return and int
int strbfind(char *s, char *f)
{
	char *c;
	int i,n;
	i = strlen(s);
	c = (char *) malloc (2);
	for(n = 0;n < i;n++)
	{
		strncpy(c,s+i-n,1);
		if (0 == strcmp (c, f))
		{
			printf("YES!!\n");
			return i-n;
		}
	}
	return -1;
}
*/

// find a string backwards and return and int
int strbfind(char *s, char *f)
{
	char *c;
	int i,n, ind;
	i = strlen(s);
	c = (char *) malloc (2);

	for(n = 0;n < i;n++)
	{
		ind = s+i-n;
		//strncpy(c,ind,1);
		strncpy(c,ind,1);
		printf("strbfind: n: %i: s: %s c: %s ind: %s len: %i \n", n, s, c, ind, i);
		if (0 == strcmp (c, f))
		{
			printf("YES!!\n");
			return i-n;
		}
	}
	return -1;
}

// returns the prefix minus the extension
void fram_prefix()
{
	return;
}

void fram_show_lyrics()
{
	int n;
	char *songn;
	char *s;
	songn = (char *) malloc (1000);

	n = frameng_getsongnum();
	get_song_name(n, songn);
	/*n = strbfind(songn, ".");
	n = strbfind(songn, ".");
	s = (char*) malloc(n+1);
	stpncpy (s, songn, n);
	printf ("dot is the %s %i character \n", s, n);
	*/

	fram_sed(songn, "b");
	//strtok(songn, ".");
//	printf ("basename: %s \n", songn);

	
	return;
}

int main(int argc, char *argv[])
{
	fram_show_lyrics();
}
