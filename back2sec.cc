
#include <stdio.h>

#include <time.h>

int main(int argc, char **argv)
	{
	struct tm time_data;

	time_t result;

	int tyear,tmon,tday,thour,tmin;

	if(argc < 6)
		{
		printf("Format: %s <year> <month> <day> <hour> <minute>\n",argv[0]);

		return(1);
		}

	sscanf(argv[1],"%d",&tyear);

	sscanf(argv[2],"%d",&tmon);

	sscanf(argv[3],"%d",&tday);

	sscanf(argv[4],"%d",&thour);

	sscanf(argv[5],"%d",&tmin);

	time_data.tm_year=tyear-1900;

	time_data.tm_mon=tmon-1;

	time_data.tm_mday=tday;

	time_data.tm_hour=thour;

	time_data.tm_min=tmin;

	time_data.tm_sec=0;

	time_data.tm_isdst=-1;

	result=mktime(&time_data);

	printf("%ld\n",result);

	return(0);
	}
