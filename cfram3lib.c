/* $Id: cfram3lib.c,v 1.53 2006/12/28 01:11:49 bl0rg Exp $
 * cframp3 - command line mp3 front-end in C++
 * rewritten from the perl version framp3
 * both by Fred Ollinger (follinge@diadig.com)
 */ 

#include "cframp4.h"
#include <glib.h>
#include <errno.h>

#ifdef FRAM_GROOVE
#include "fram_groove.h"
#endif

GSList *cframp3_file_list=NULL;

// copies the path to the configfile to char conf
void cframp3_getconfigfile(char *conf)
{
  char *home = getenv("HOME");
	char *fn = ".cframp3";
  size_t size = strlen(home) + strlen(fn) + 2;;
  char *path = (char *) malloc (size);
	sprintf(path, "%s/%s", home, fn);
	strcpy(conf, path);
}

// initializes the cframp3lib
// CALLERS:
// 1. framplayer.c: fram_sound_init()
int cframp3lib_init()
{
	printf("cfram3lib_init(): initializing \n");
}

void cframp3lib_quit(void)
{
	//cframp3lib_writecwd();
}

int cframp3lib_getowd(char *dir)
{
  char *path = (char *) malloc (1000);
	cframp3_getconfigfile(path);
	FILE *fi;
  	fi = fopen (path, "r");
	if (fi)
	{
		int i;
		i = fread (dir, 1,  1000, fi);
		fclose(fi);
		return;
	}
	else
	{
		printf("cframp3lib_getowd(): could not open configfile!\n");
		return -1;
	}
}

int cframp3lib_writecwd(void)
{
	printf("cframp3lib_writecwd(): called \n");
  char *path = (char *) malloc (1000);
	cframp3_getconfigfile(path);

	printf("cframp3lib_writecwd(): got path \n");
	char *pwd = get_current_dir_name();
	int songnum = frameng_getsongnum();
	printf("cframp3lib_writecwd(): song num = %i \n", songnum);
       	size_t size = strlen(pwd) + 255;
        char *str = (char *) malloc (size);
	printf("cframp3lib_writecwd(): malloc worked \n");
	//sprintf(path, "%s/%s", home, fn);
	sprintf(str, "%s\n%i", pwd, songnum);
	printf("cframp3lib_writecwd(): save str is: %s \n", str);
	
	FILE *fi;
  	fi = fopen (path, "w");
	if (fi)
	{
		fwrite (pwd, strlen(pwd), 1, fi);
	}
	fclose(fi);
	return;
}

// dummy function
// CALLERS:
// 1. cfram3lib.c: fram_ls_grep()
// 2. cfram3lib.c: get_song_name()
// 3. cfram3lib.c: fram_ls()
static int
*fram_one (const struct dirent *unused)
{
       return;
}

// CALLERS:
// 1. cfram3lib.c: issong()
int isogg(char *s)
{
	FILE *fi;
	int c;
	fi = fopen(s, "r");
	c = fgetc (fi);
	if (79 != c)
	{
		fclose(fi);
		return FRAM_ERR;
	}
	c = fgetc (fi);
	if (103 != c)
	{
		fclose(fi);
		return FRAM_ERR;
	}
	c = fgetc (fi);
	if (103 != c)
	{
		fclose(fi);
		return FRAM_ERR;
	}
	c = fgetc (fi);
	if (83 != c)
	{
		fclose(fi);
		return FRAM_ERR;
	}
	fclose(fi);
	return FRAM_OGG;
}

// CALLERS:
// 1. cfram3lib.c: issong()
int ismpeg(char *s)
{
	FILE *fi;
	int c;
	fi = fopen(s, "r");
	c = fgetc (fi);
	if (255 != c)
	{
		fclose(fi);
		return FRAM_ERR;
	}
	c = fgetc (fi);
	if ((251 != c) && (243 != c) && (250 != c))
	{
		fclose(fi);
		printf("ismeg(): %s failed on 2nd byte: %i \n", s, c);
		return FRAM_ERR;
	}
	c = fgetc (fi);
	// While the code below is more precise, it fails more. Let's be less
	// precise until things start to break.
	/*
	if ((144 != c) && (146 != c) && (192 != c) && (152 != c))
	{
		fclose(fi);
		printf("ismeg(): %s failed on 3nd byte: %i \n", s, c);
		return FRAM_ERR;
	}
	*/
	fclose(fi);
	return FRAM_MPEG;
}

// CALLERS:
// 1. cfram3lib.c: issong()
int ismp3(char *s)
{
	FILE *fi;
	int c;
	fi = fopen(s, "r");
    
    if ( 0 != errno) return FRAM_ERR;

	c = fgetc (fi);
	if (73 != c)
	{
		fclose(fi);
		return FRAM_ERR;
	}
	c = fgetc (fi);
	if (68 != c)
	{
		fclose(fi);
		return FRAM_ERR;
	}
	c = fgetc (fi);
	if (51 != c)
	{
		fclose(fi);
		return FRAM_ERR;
	}
	fclose(fi);
	return FRAM_MP3;
}

// CALLERS:
// 1. fram_ls_grep()
// 2. fram_ls()
// 3. fram_ls_orig()
// 4. get_song_name()
int issong(char *s){
	int i;
	struct stat buff;

	i = lstat(s,&buff);

	if (S_ISDIR (buff.st_mode))
		return FRAM_DIR;

	if (!S_ISREG (buff.st_mode))
        return FRAM_NULL;

	i = ismp3(s);
	if (FRAM_MP3 == i)
		return FRAM_MP3;

	i = isogg(s);

	if (FRAM_OGG == i)
		return FRAM_OGG;
	i = ismpeg(s);

	if (FRAM_MPEG == i)
		return FRAM_MPEG;

	return FRAM_NULL;
}

// actually return the pwd
int fram_getcwd(char* p){
    	// *p is the working directory 
       size_t size = 100;
	   int errno;
	   int range = 512000 ;
     
       while (1){
           char *buffer = (char *) malloc (size);
           if (getcwd (buffer, size) == buffer){
          	   free (buffer);
			   return 0;
		   }
           if (errno != range)
                return 0;
           size *= 2;
         }
     	
}

void fram_pwd(void)
{
     
       size_t size = 100;
	   int errno;
	   int range = 512000 ;
     
       while (1)
         {
           char *buffer = (char *) malloc (size);
           if (getcwd (buffer, size) == buffer)
		   {
			printf("%s \n", buffer);
           		free (buffer);
			return;
		   }
           if (errno != range)
             return ;
           size *= 2;
         }
     	
}

// list only certain songs that match
int fram_ls_grep(char* needle)
{

	// boilerplate pcre_compile:
	const char *error;
	int erroroffset = 0;
	unsigned const char *tables = NULL;

	// boilerplate pcre_exec:
	const pcre_extra *extra;
	int startoffset = 0;
	int options=0;
	int ovecsize = 99;
	int ovector[99];
	int result = 0;

	// mp3 matches
	pcre *re_song = NULL;

	re_song = pcre_compile((char *)needle, options, &error, &erroroffset, tables);

	struct dirent **eps;
	int n;

	//n = scandir ("./", &eps, fram_one, alphasort);
	n = scandir ("./", &eps, 0, alphasort);

/*
	int scandir(const char *dir, struct dirent ***namelist,
          int(*select)(const struct dirent *), int(*compar)(const struct
          dirent **, const struct dirent **));
*/

	if (n < 0)
	{
			printf("No songs available.\n");
			printf("freeing eps! \n");
			free(eps);
			return 0;
	}


    	int counter = 1;
    	int ans=0;
	int cnt=0;

  	int length;

    while (cnt < n)
    {
        char *s;
        char *copys;
	char *cmatch;
        int i=0;
	
	copys=eps[cnt]->d_name;
	//printf("COPYS IS: %s \n", copys);

  	length = (int)strlen(copys);
	result = pcre_exec(re_song, NULL, copys, length, startoffset, options, ovector, ovecsize);

	ans = issong(copys);

        if (ans > 0)
	{
		if  (-1 != result)
		{
            		printf("%i: %s\n", counter, copys);
		}
            		counter ++;
	}
        
	else if (ans == FRAM_DIR)
	{
		if  (-1 != result)
		{
           		printf("dir: %s\n", copys);
		}
	}

	cnt++;	
    	}
	printf("freeing eps! \n");
	free(eps);
	return 0;
}

int fram_ls_orig()
{
    DIR *dir;
    struct dirent *dp;
    dir = opendir(".");

	FILE *pager;
	char *cpager="less";
	pager = popen (cpager, "w");

	if (!pager)
    {
           fprintf (stderr,
                    "incorrect parameters or too many files.\n");
           return EXIT_FAILURE;
    }

    if (dir == NULL)
    {
        printf ("\nopendir error\n");
        return -1;
    }

    dp = readdir (dir);

    int counter = 1;

    while (dp != NULL)
    {
        char *s;
        char *copys;
        int i=0;
        int ans=0;

        copys=dp->d_name;
	ans = issong(copys);

        if (ans > 0)
        {
            //printf("%i: %s\n", counter, copys);
            fprintf(pager, "%i: %s\n", counter, copys);
            printf("%i: %s\n", counter, copys);
            counter ++;
        }

        if (ans == FRAM_DIR)
	{
            fprintf(pager, "dir: %s\n", copys);
            printf("dir: %s\n", copys);
	}

        dp = readdir(dir);
    }
	closedir (dir);
	pclose (pager);
	return 0;
}

// return the song of the number given
int get_song_name(int songnum, char *song)
{
	int songtype;
	int counter = 0;
	int cnt = 0;
	struct dirent **eps;
	int n;

	//n = scandir ("./", &eps, fram_one, alphasort);
	n = scandir ("./", &eps, 0, alphasort);

	if (n < 0)
	{
		printf("No songs available.\n");
		//printf("freeing eps! \n");
		free(eps);
		return 0;
	}

	while(cnt < n)
    	{
		char *s;
        	static char *copys;
        	int i=0;

		copys=eps[cnt]->d_name;
		songtype = issong(copys);

        	if (songtype > 0)
        	{
            		counter ++;
        	}
		else
		{
        		//printf ("get_song_name(): is NOT song %s\n", copys);
		}

		if (counter == songnum)
		{
			strcpy(song, copys);
			//printf("freeing eps! \n");
			free(eps);
			//printf("freeing copys! \n");
			//free(copys);
			return 0;
		}

		cnt++;
	}

	//printf("freeing eps! \n");
	free(eps);
	return -1;
}

int fram_ls()
{
    int pagesize = 13; // the number of lines to show at a time
    printf("cfram3lib.c: fram_ls() called .\n");
    fram_pwd();

    struct dirent **eps;
    int n;

    n = scandir ("./", &eps, 0, alphasort);

    if (n < 0)
    {
        printf("No songs available.\n");

    	printf("freeing eps! \n");
        free(eps);
        return 0;
    }

    int ans=0; // result of the song check
    int cnt=0; // counter for the file number
    int counter = 1;
    int counter2 = pagesize; 
    int lcounter = 1;  //loop counter
    int mychar=0;

    while(cnt < n){
        while(cnt < n && lcounter < counter2){
            char *copys;
            copys=eps[cnt]->d_name;
            ans = issong(copys);
			
            if (ans > 0){
                printf("%i: %s\n", counter, copys);
		        counter++;
		        lcounter++;
            }

            if (ans == FRAM_DIR){
	            printf("dir: %s\n", copys);
		        lcounter++;
            }
	    cnt++;
	  } // END while (cnt < n && lcounter < counter2)

     counter2 = lcounter + pagesize;
	 mychar = getc(stdin);
	 if (113 == mychar)
	     cnt = n + 1;
    } // END while(cnt < n)

    printf("freeing eps! \n");
    free(eps);
    return 0;
}


int fram_count_songs()
{
    fram_pwd();

    struct dirent **eps;
    int n;

    //n = scandir ("./", &eps, fram_one, alphasort);
    n = scandir ("./", &eps, 0, alphasort);

    if (n < 0)
    {
        //printf("No songs available.\n");

    	//printf("freeing eps! \n");
        free(eps);
        return 0;
    }

    int ans=0; // result of the song check
    int cnt=0; // counter for the file number
    int counter = 1;

    while(cnt < n)
    {
        char *copys;
        copys=eps[cnt]->d_name;
        ans = issong(copys);
			
        if (ans > 0)
        {
		counter++;
        }

	cnt++;

    	//printf("freeing eps! %i, %i\n", cnt, n);
    }
		
    //printf("freeing eps! \n");
    free(eps);
    printf("cfram3lib.c: fram_count_songs() called %i .\n", counter-1);
    return counter-1;
}

void cframp3_load_playlist (char *playlist)
{

	gchar *list;
	g_stpcpy(list, playlist);
	g_print("getting playlist: %s \n", list);
	gchar *line=NULL;
	gsize len=1;
	gsize term;

	GIOChannel *fd;
	GError *ger=NULL;

	// clean up the list in case there are old files in here

	fd = g_io_channel_new_file (list, "r", &ger);

	while(len > 0)	
	{
		g_io_channel_read_line (fd, &line, &len, &term, &ger);

		if (len > 0)
		{
			printf("song: %s len: %i \n", line, (int)len);
			cframp3_file_list = g_slist_append(cframp3_file_list, line);
		}

	}

	return;
}

void cframp3_show_playlist ()
{
	g_print("playlist called:  \n");

	guint n;
	n=1;
    char *line;

	g_print("crashing!! \n");
	n = g_slist_length(cframp3_file_list);

	line=g_slist_nth_data(cframp3_file_list, 0);
	g_print("list: %s len:  \n", line);

	return;

}


// get the nth element from playlist
int cframp3_playlist_get_nth (int n, char *c)
{
	char *s;
	int len;
	n=n-1;
	if (n > g_slist_length(cframp3_file_list))
	{
		printf("exceeded playlist length!! \n");
		return -1;
	}

	s=g_slist_nth_data(cframp3_file_list, n);
	len=strlen(s);
	g_print("cframp3_playlist_get_nth: len: %i s: ***%s***  \n", len, s);
	strncpy(c, s, len-2);
	g_print("cframp3_playlist_get_nth: returning: ***%s*** \n", c);
	return 0;
}

// $Id: cfram3lib.c,v 1.53 2006/12/28 01:11:49 bl0rg Exp $
