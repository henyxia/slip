#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "http.h"


#include <errno.h>

#define WEB_DIR  "./www"
#define PAGE_NOTFOUND "error.html"
#define MAX_BUFFER 1024

#define CODE_OK  200
#define CODE_NOTFOUND 404

void newHTTPClient(int sock)
{
	printf("New HTTP Client (sock %d)\n", sock);

	char	buffer[MAX_BUFFER];
	char	cmd[MAX_BUFFER];
	char	page[MAX_BUFFER];
	char	proto[MAX_BUFFER];
	char	path[MAX_BUFFER];
	char	type[MAX_BUFFER];
	FILE*	client = NULL;

	printf("Creating file descriptor\n");

	client = fdopen(sock, "r+");
	if(client == NULL)
		printf("Useless client\n");

	if(fgets(buffer,MAX_BUFFER,client)==NULL)
	{
		printf("Client did not speak\n");
		exit(-1);
	}
	if(sscanf(buffer,"%s %s %s",cmd,page,proto)!=3)
	{
		printf("Unexpected format\n");
		exit(-1);
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
		int fd=open(path,O_RDONLY);
		if(fd>=0)
		{
			int bytes;
			while((bytes=read(fd,buffer,MAX_BUFFER))>0)
				write(sock,buffer,bytes);
		}
	}
	else
		printf("This command is not supported yet\n");

	fclose(client);
}
