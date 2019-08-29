#include <stdarg.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <resolv.h>
#include <errno.h>
#include <netdb.h>
#include <locale.h>

#define MAXBUF 1024
#define PORT 80

int find_substr(char *listPointer, char *itemPointer);

int main()
{
int sockfd, bytes_read, i;
i = 0;

struct sockaddr_in dest;
struct hostent *he;

char buffer[MAXBUF];
char buffer1[MAXBUF];

char *Strings[3];
Strings[0] = "www.delfi.lt";
Strings[1] = "www.15min.lt";
Strings[2] = "www.veidas.lt";
int k,cpid;

for(k=0;k<=3;k++)
{

cpid = fork();
if(cpid==0)
{
if((he=gethostbyname(Strings[k])) == NULL)
{
perror("gethostbyname()");
}

//sukuriamas socket'as transliavimui
if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
{
printf("Socket");
}

//Inicijuojamas serverio adresas ir portas
bzero(&dest, sizeof(dest));
dest.sin_family = AF_INET;
dest.sin_addr = *((struct in_addr *)he->h_addr);
dest.sin_port = htons(PORT);
memset(&(dest.sin_zero), '\0', 8);

//Jungiamasi prie serverio
if ( connect(sockfd, (struct sockaddr *)&dest, sizeof(struct sockaddr)) < 0 )
{
close(sockfd);
continue;
}

//zinute siunciama i HTTP serveri
bzero(buffer, MAXBUF);
sprintf(buffer, "GET / HTTP/1.0\nHost: %s\n\n",Strings[k]);
send(sockfd, buffer, strlen(buffer), 0);

//vykdoma jei pavyko prisijungimas, nuskaitoma ir irasoma
do
{
bzero(buffer1, MAXBUF);
bytes_read = recv(sockfd, buffer1, MAXBUF, 0);
if ( bytes_read > 0 )
if(find_substr(buffer1, "ir") != -1)
i++;
}
while ( bytes_read > 0 );

close(sockfd);

//isspausdinami gauti rezultatai
printf("Zodis 'ir' rastas %s tinklapyje %d kartu. \n",Strings[k],i);

i = 0;

exit(0);

}
 }
 sleep(3);
 return 0;
 }

 int find_substr(char *listPointer, char *itemPointer)
 {
 int t;
 char *p, *p2;

 for(t=0; listPointer[t]; t++)
 {

p = &listPointer[t];

p2 = itemPointer;

while(*p2 && *p2==*p)

{
p++;
p2++;
}
if(!*p2) return t; /* 1st return */
}
 return -1; /* 2nd return */
}
