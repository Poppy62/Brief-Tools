
#include <stdio.h>

#include <time.h>

int main(int argc,char **argv)
	{
	time_t timer;

	if(argc<2)
		{
		printf("Usage: uxt2str <seconds>\n");

		return(1);
		}

	sscanf(argv[1],"%ld",&timer);

	printf("%s",ctime(&timer));

	return(0);
	}
