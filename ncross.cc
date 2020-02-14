
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern char *optarg;

extern int optind, opterr, optopt;

#define RECLEN 80

FILE *pattern_file, *target_file;

char inrow[RECLEN],outrow[RECLEN],in_tmprow[RECLEN],out_tmprow[RECLEN];

int inrow_cnt=0,outrow_cnt=0;

int main(int argc,char **argv,char **envp);
void closeall(void);
void read_pattern(void);
void read_target(void);
int right_trim(char *);
int eof_all_handler(void);

int main(int argc,char **argv,char **envp)
	{
	int cmp_val,sub_val,src_len,tgt_len;

	int option=0,flag=0;

	opterr=0;

	atexit(closeall);

	option=getopt(argc,argv,"abpd");

	if(option==-1)
		{
		flag|=7;
		}

	while(option!=-1)
		{
		switch((char)option)
			{
			case 'a':
				{
				flag|=7;

				break;
				}

			case 'b':
				{
				flag|=1;

				break;
				}

			case 'p':
				{
				flag|=2;

				break;
				}

			case 'd':
				{
				flag|=4;

				break;
				}

			case '?':
				{
				printf("-%c : unrecognized option\n\n",(char)optopt);

				return(2);
				}

			default:
				{
				break;
				}
			}

		option=getopt(argc,argv,"abpd");
		}

	if((argc-optind)!=2)
		{
		printf("Usage: %s -a|b|p|d <pattern file> <target file>\n\n",argv[0]);

		printf("Switch -a (or omitted): print substring, superstring and duplicate strings\n");

		printf("       -b : print substring\n");

		printf("       -p : print superstring\n");

		printf("       -d : print duplicate strings\n");

		return(1);
		}

	pattern_file=fopen(argv[optind],"r");

	if(pattern_file==NULL)
		{
		printf("Invalid pattern file\n\n");

		return(-1);
		}

	target_file=fopen(argv[optind+1],"r");

	if(target_file==NULL)
		{
		printf("Invalid target file\n\n");

		return(-1);
		}

	rewind(pattern_file);

	clearerr(pattern_file);

	read_pattern();

	rewind(target_file);

	clearerr(target_file);

	read_target();

	while(!eof_all_handler())
		{
		cmp_val=strcmp(inrow,outrow);

		if(cmp_val==0)
			{
			if(flag&4) printf("Dup record found:\tpattern (%d) %s\ttarget (%d) %s\n",
				inrow_cnt,in_tmprow,outrow_cnt,out_tmprow);

			read_target();

			continue;
			}

		src_len=strlen(inrow);

		tgt_len=strlen(outrow);

		if(src_len<tgt_len)
			{
			sub_val=strncmp(inrow,outrow,src_len);

			if(sub_val==0)
				{
				if(flag&1) printf("Substring found:\tpattern (%d) %s\ttarget (%d) %s\n",
					inrow_cnt,in_tmprow,outrow_cnt,out_tmprow);

				read_target();

				continue;
				}
			}

		if(src_len>tgt_len)
			{
			sub_val=strncmp(outrow,inrow,tgt_len);

			if(sub_val==0)
				{
				if(flag&2) printf("Superstring found:\tpattern (%d) %s\ttarget (%d) %s\n",
					inrow_cnt,in_tmprow,outrow_cnt,out_tmprow);

				read_pattern();

				continue;
				}
			}

		if(cmp_val<0)
			{
			read_pattern();
			}

		if(cmp_val>0)
			{
			read_target();
			}
		}

	return(0);
	}

void read_pattern()
	{
	fgets(in_tmprow,RECLEN,pattern_file);

	if(feof(pattern_file))
		{
		fgets(out_tmprow,RECLEN,target_file);

		sscanf(out_tmprow," %s",outrow);

		right_trim(out_tmprow);

		outrow_cnt++;

		return;
		}

	sscanf(in_tmprow," %s",inrow);

	right_trim(in_tmprow);

	inrow_cnt++;

	return;
	}

void read_target()
	{
	fgets(out_tmprow,RECLEN,target_file);

	if(feof(target_file))
		{
		fgets(in_tmprow,RECLEN,pattern_file);

		sscanf(in_tmprow," %s",inrow);

		right_trim(in_tmprow);

		inrow_cnt++;

		return;
		}

	sscanf(out_tmprow," %s",outrow);

	right_trim(out_tmprow);

	outrow_cnt++;

	return;
	}

int right_trim(char *row)
	{
	int len;

	len=strlen(row)-1;

	switch(row[len])
		{
		case 10: /* Line Feed */
		case 13: /* Carriage Return */
		case 32: /* Blank */
		case 9:  /* Tab */
			{
			row[len]=0;

			right_trim(row);

			break;
			}

		default:
			{
			break;
			}
		}

	return(0);
	}

void closeall()
	{
	if(pattern_file!=NULL)
		{
		fclose(pattern_file);
		}

	if(target_file!=NULL)
		{
		fclose(target_file);
		}

	/*
	printf("Program termination\n");
	*/

	return;
	}

int eof_all_handler()
	{
	if(feof(pattern_file)&&feof(target_file))
		{
		return(1);
		}
	else
		{
		return(0);
		}
	}
