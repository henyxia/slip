#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <libthrd.h>
#include "http.h"
#include "teams.h"


#include <errno.h>

#define WEB_DIR  "./www"
#define PAGE_NOTFOUND "error.html"
#define MAX_BUFFER 1024

#define CODE_OK  200
#define CODE_NOTFOUND 404

bool acceptableVarChar(unsigned char buf)
{
	return ((buf>64 && buf <91) || (buf>47 && buf<58) || (buf == '_'));
}

void interpreter(FILE* webpage, FILE* client)
{
	unsigned char	buf = fgetc(webpage);
	char			buffer[MAX_BUFFER];
	int				cmdLen = 0;
	int				team;
	char			query;

	while(cmdLen < MAX_BUFFER && !feof(webpage) && acceptableVarChar(buf))
	{
		buffer[cmdLen] = buf;
		cmdLen++;
		buf = fgetc(webpage);
	}

	if(cmdLen == 0)
	{
		fprintf(client, "<br/>This symbol is reserved<br/>\r\n");
		return;
	}
	else if(cmdLen == MAX_BUFFER)
	{
		fprintf(client, "<br/>Your command is way too long<br/>\r\n");
		return;
	}
	else
	{
		if(sscanf(buffer, "TEAMS_%d_%c", &team, &query) != 2)
		{
			fprintf(client, "<br/>Command recieved %s, %d char long, but not recognized<br/>", buffer, cmdLen);
			return;
		}
		if(team<0 || team>=MAX_TEAMS)
		{
			fprintf(client, "<br/>The team %d does not exists\r\n<br/>", team);
			return;
		}
		if(query == 'M')
			fprintf(client, "%s", getTeamMembers(team));
		else if(query == 'X' || query == 'Y' || query == 'Z' || query == 'T')
			fprintf(client, "%d", getTeamValue(team, query));
		else
			fprintf(client, "<br/>Command not recognized\n");
	}

	// Writing last char
	fputc(buf, client);
}

void processHTTPClient(void* arg)
{
	int *tmp = arg;
	int sock = *tmp;
	printf("New HTTP Client (sock %d)\n", sock);

	char	buffer[MAX_BUFFER];
	char	cmd[MAX_BUFFER];
	char	page[MAX_BUFFER];
	char	proto[MAX_BUFFER];
	char	path[MAX_BUFFER];
	char	type[MAX_BUFFER];
	FILE*	webpage = NULL;
	FILE*	client = NULL;

	printf("Creating file descriptor\n");

	client = fdopen(sock, "r+");
	if(client == NULL)
		printf("Useless client\n");

	if(fgets(buffer,MAX_BUFFER,client)==NULL)
	{
		printf("Client did not speak\n");
		return;
	}
	if(sscanf(buffer,"%s %s %s",cmd,page,proto)!=3)
	{
		printf("Unexpected format\n");
		return;
	}
	while(fgets(buffer,MAX_BUFFER,client)!=NULL)
		if(strcmp(buffer,"\r\n")==0)
			break;

	if(strcasecmp(cmd,"GET")==0)
	{
		printf("The client want to GET the webpage %s\n", page);
		if(strcmp(page, "/")==0)
		{
			printf("Let's help the client with the home page\n");
			sprintf(page, "/index.html");
		}
		int code=CODE_OK;
		struct stat fstat;
		sprintf(path,"%s%s",WEB_DIR,page);
		printf("And this page is located at %s\n", path);
		if(stat(path,&fstat)!=0 || !S_ISREG(fstat.st_mode))
		{
			printf("But this page does not exist\n");
		    sprintf(path,"%s/%s",WEB_DIR,PAGE_NOTFOUND);
		    code=CODE_NOTFOUND;
	    }
		strcpy(type,"text/html");
		char *end=page+strlen(page);
		if(strcmp(end-4,".png")==0) strcpy(type,"image/png");
		if(strcmp(end-4,".jpg")==0) strcpy(type,"image/jpg");
		if(strcmp(end-4,".gif")==0) strcpy(type,"image/gif");
		fprintf(client,"HTTP/1.0 %d\r\n",code);
		fprintf(client,"Server: CWeb\r\n");
		fprintf(client,"Content-type: %s\r\n",type);
		fprintf(client,"Content-length: %d\r\n", (int)fstat.st_size);
		fprintf(client,"\r\n");
		fflush(client);
		printf("Data send successfully\n");
		webpage=fopen(path, "r");
		if(webpage != NULL)
		{
			unsigned char buf;
			while(!feof(webpage))
			{
				buf = fgetc(webpage);
				if(buf == '$')
					interpreter(webpage, client);
				else
					if(!feof(webpage))
						fputc(buf, client);
			}
		}
	}
	else
		printf("This command is not supported yet\n");

	fclose(client);
}

void newHTTPClient(int sock)
{
	newThread(processHTTPClient, &sock, sizeof(int * ));
}

