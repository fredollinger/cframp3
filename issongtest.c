// test issong()

#include "cframp4.h"

int main(int argc, char *argv[])
{
		char *asong = "song.mp3";
		int ans;
		int i;
		for (i=0;i < 20;i++)
		{
		ans=isogg(asong);
		printf("%s %i\n", asong, ans);
		}
		if (FRAM_NULL == ans)
		{
				printf("not a song\n");
		}
		else
		{
				printf("a song\n");
		}
		return 0;
}
