#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>  
#include <unistd.h>     
#include <sys/types.h>  
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define PORT 80
#define REC_LEVEL 5
#define MAXLEN 50000 

const char *eroare_redirect300="Multiple Choices Available";
const char *eroare_redirect301="Link Moved Permanently";
const char *eroare_redirect302="Found";
const char *eroare_redirect305="Proxy Required";
const char *eroare_redirect308="Permanent Redirect";
const char *eroare_client400="Bad Request";
const char *eroare_client401="Authorization Required";
const char *eroare_client402="Payment Required";
const char *eroare_client403="Response refused by the server";
const char *eroare_client404="Link not found";
const char *eroare_client405="Method not Allowed";
const char *eroare_client406="Not Acceptable";
const char *eroare_client407="Proxy Authentication Required";

/*Functie care intoarce numarul de octeti citit dintr-un sir*/
ssize_t Readline(int sockd, void *vptr, size_t maxlen) {
	    ssize_t n, rc;
	    char c, *buffer;

	    buffer = vptr;

	    for ( n = 1; n < maxlen; n++ ) {	
		if ( (rc = read(sockd, &c, 1)) == 1 ) {
		    *buffer++ = c;
		    if ( c == '\n' )
		    	break;
		}
		else if ( rc == 0 ) {
		    if ( n == 1 )
		        return 0;
		    else
			break;
		}
		else {
			if ( errno == EINTR ) {
		
			continue;
		}
	    
	    }
	}
	*buffer = '\0';
	return n;
	
}

/*Functie pentru realizarea download-ului implicit (fara parametrii -e si -r
in linia de comanda) a fisierului specificat*/
void download_implicit(int sockfd, char *sendbuf, FILE *f, FILE *erori) {
	char recvbuf[MAXLEN];
	char details[MAXLEN];
	int nbytes, sep=0, code;
	char CRLF[3];
	CRLF[0] = 13; CRLF[1] = 10; CRLF[2] = 0;
	strcat(sendbuf, CRLF);

	/*Trimit comanda catre server*/
	write(sockfd, sendbuf, strlen(sendbuf));

	/*Pe masura ce citesc rezultatul linie cu linie*/
	while ((nbytes=Readline(sockfd, recvbuf, MAXLEN - 1))>0){
		     recvbuf[nbytes] = '\0';
			//printf("%d %s\n",nbytes,recvbuf);
		     /*Verific daca linia contine rezultatul cererii GET*/
			/*Daca da, atunci obtin codul corespunzator si afisez eroare (daca este cazul)*/
		     if (strncmp (recvbuf, "HTTP/1.1",8) == 0) {
			sscanf(recvbuf,"HTTP/1.1 %d %s\n",&code,details);		     
			switch (code) {
				case 300:
					fprintf(erori,"Error 300: %s\n",eroare_redirect300);
					break;
				case 301:
					fprintf(erori,"Error 301: %s\n",eroare_redirect301);
					break;
				case 302:
					fprintf(erori,"Error 302: %s\n",eroare_redirect302);
					break;
				case 305:
					fprintf(erori,"Error 305: %s\n",eroare_redirect305);
					break;
				case 308:
					fprintf(erori,"Error 308: %s\n",eroare_redirect308);
					break;
				case 400:
					fprintf(erori,"Error 400: %s\n",eroare_client400);
					break;
				case 401:
					fprintf(erori,"Error 401: %s\n",eroare_client401);
					break;
				case 402:
					fprintf(erori,"Error 402: %s\n",eroare_client402);
					break;
				case 403:
					fprintf(erori,"Error 403: %s\n",eroare_client403);
					break;
				case 404:
					fprintf(erori,"Error 404: %s\n",eroare_client404);
					break;
				case 405:
					fprintf(erori,"Error 405: %s\n",eroare_client405);
					break;
				case 406:
					fprintf(erori,"Error 406: %s\n",eroare_client406);
					break;
				case 407:
					fprintf(erori,"Error 407: %s\n",eroare_client407);
					break;
				default :
					;
			}
			}
			if (strncmp (recvbuf, "HTTP/1.0",8) == 0) {
			sscanf(recvbuf,"HTTP/1.0 %d %s\n",&code,details);		     
			switch (code) {
				case 300:
					fprintf(erori,"Error 300: %s\n",eroare_redirect300);
					break;
				case 301:
					fprintf(erori,"Error 301: %s\n",eroare_redirect301);
					break;
				case 302:
					fprintf(erori,"Error 302: %s\n",eroare_redirect302);
					break;
				case 305:
					fprintf(erori,"Error 305: %s\n",eroare_redirect305);
					break;
				case 308:
					fprintf(erori,"Error 308: %s\n",eroare_redirect308);
					break;
				case 400:
					fprintf(erori,"Error 400: %s\n",eroare_client400);
					break;
				case 401:
					fprintf(erori,"Error 401: %s\n",eroare_client401);
					break;
				case 402:
					fprintf(erori,"Error 402: %s\n",eroare_client402);
					break;
				case 403:
					fprintf(erori,"Error 403: %s\n",eroare_client403);
					break;
				case 404:
					fprintf(erori,"Error 404: %s\n",eroare_client404);
					break;
				case 405:
					fprintf(erori,"Error 405: %s\n",eroare_client405);
					break;
				case 406:
					fprintf(erori,"Error 406: %s\n",eroare_client406);
					break;
				case 407:
					fprintf(erori,"Error 407: %s\n",eroare_client407);
					break;
				default :
					;
			}
			}
		     /*Prima linie goala gasita in raspunsul de la server 
			marcheaza sfarsitul headerelor*/
			if (nbytes ==1 && sep==0)
				sep=1;
		     /*Liniile de dupa aceasta linie goala vor fi redirectate 
			in fisier, in directorul corespunzator*/
			if (sep==1)
				fprintf(f,"%s",recvbuf);		
			
		  }
}

/*Functie pentru extragerea dintr-un sir de caractere a numelui
unui fisier cu extensie specificata (daca acesta exista)*/
char *extrageFisierExtensie (char *linie, char *extensie) {
	char *aux = (char *)malloc(40*sizeof(char));
	if (linie == NULL)
		return "";
	char *token = strstr(linie,extensie);
	if (token == NULL)
		return "";
	int poz = token - linie + strlen(extensie);
	strncpy(aux,linie,poz);
	char *token1 , *token2;
	if (linie[poz]=='\"' || linie[poz]=='\'' && linie[poz]==' ') {
		if ( strstr (aux,"\'/") != NULL ) {
			 token1 = strstr (aux,"/\'") + 1 ;
			return token1;
		}
		if (strstr (aux,"\"/") != NULL) {
			token2 = strstr (aux,"\"/") + 1 ;
			return token2;
		}
		return "";
	}
	return "";
}


char **listaFisiereLink (int sockfd, char *sendbuf, char **extensii, FILE *erori) {
	char recvbuf[MAXLEN];
	char details[MAXLEN];
	char beforeEndTag[MAXLEN];
	char bufaux[MAXLEN];
	char *mediaFile = (char *)malloc(50*sizeof(char));
	char **mediaFileList = (char **)malloc(20*sizeof(char *));
	int nrOfFiles = 0, nbytes, code, i;

	for (i=0;i<20;i++)
		mediaFileList[i] = (char *)malloc(50*sizeof(char));

	char CRLF[3];
	CRLF[0] = 13; CRLF[1] = 10; CRLF[2] = 0;
	strcat(sendbuf, CRLF);

	int in_tag=-1;

	/*Trimit comanda catre server*/
	write(sockfd, sendbuf, strlen(sendbuf));

	while ((nbytes=Readline(sockfd, recvbuf, MAXLEN - 1))>0){
		     recvbuf[nbytes] = '\0';
			if (strncmp (recvbuf, "HTTP/1.1",8) == 0) {
			  sscanf(recvbuf,"HTTP/1.1 %d %s\n",&code,details);		     
			  switch (code) {
				case 300:
					fprintf(erori,"Error 300: %s\n",eroare_redirect300);
					break;
				case 301:
					fprintf(erori,"Error 301: %s\n",eroare_redirect301);
					break;
				case 302:
					fprintf(erori,"Error 302: %s\n",eroare_redirect302);
					break;
				case 305:
					fprintf(erori,"Error 305: %s\n",eroare_redirect305);
					break;
				case 308:
					fprintf(erori,"Error 308: %s\n",eroare_redirect308);
					break;
				case 400:
					fprintf(erori,"Error 400: %s\n",eroare_client400);
					break;
				case 401:
					fprintf(erori,"Error 401: %s\n",eroare_client401);
					break;
				case 402:
					fprintf(erori,"Error 402: %s\n",eroare_client402);
					break;
				case 403:
					fprintf(erori,"Error 403: %s\n",eroare_client403);
					break;
				case 404:
					fprintf(erori,"Error 404: %s\n",eroare_client404);
					break;
				case 405:
					fprintf(erori,"Error 405: %s\n",eroare_client405);
					break;
				case 406:
					fprintf(erori,"Error 406: %s\n",eroare_client406);
					break;
				case 407:
					fprintf(erori,"Error 407: %s\n",eroare_client407);
					break;
				default :
					;
			    }
			}
			else if (strncmp (recvbuf, "HTTP/1.0",8) == 0) {
				sscanf(recvbuf,"HTTP/1.0 %d %s\n",&code,details);		     
				switch (code) {
					case 300:
					fprintf(erori,"Error 300: %s\n",eroare_redirect300);
					break;
				case 301:
					fprintf(erori,"Error 301: %s\n",eroare_redirect301);
					break;
				case 302:
					fprintf(erori,"Error 302: %s\n",eroare_redirect302);
					break;
				case 305:
					fprintf(erori,"Error 305: %s\n",eroare_redirect305);
					break;
				case 308:
					fprintf(erori,"Error 308: %s\n",eroare_redirect308);
					break;
				case 400:
					fprintf(erori,"Error 400: %s\n",eroare_client400);
					break;
				case 401:
					fprintf(erori,"Error 401: %s\n",eroare_client401);
					break;
				case 402:
					fprintf(erori,"Error 402: %s\n",eroare_client402);
					break;
				case 403:
					fprintf(erori,"Error 403: %s\n",eroare_client403);
					break;
				case 404:
					fprintf(erori,"Error 404: %s\n",eroare_client404);
					break;
				case 405:
					fprintf(erori,"Error 405: %s\n",eroare_client405);
					break;
				case 406:
					fprintf(erori,"Error 406: %s\n",eroare_client406);
					break;
				case 407:
					fprintf(erori,"Error 407: %s\n",eroare_client407);
					break;
				default :
					;
				}
		
		     }

			else if (strstr(recvbuf, "<a href=") != NULL && strstr(recvbuf,"</a>") != NULL) {
				strcpy(bufaux, strstr(recvbuf,"<a href=")+strlen("<a href="));
				if (strncmp(bufaux,"\"http://",8) == 0) {
					in_tag = 0;
					continue;
				}
				for (i=0;i<6;i++) {
					strcpy(mediaFile, extrageFisierExtensie(bufaux, extensii[i]));
					if ( strcmp (mediaFile,"") != 0 ) {
						strcpy(mediaFileList[nrOfFiles],mediaFile);
						nrOfFiles ++ ;					
					}
				}
				in_tag=0;
			}
			
		     else if (strstr(recvbuf,"<a href=") !=NULL) {
			strcpy (bufaux, strstr(recvbuf,"<a href=")+strlen("<a href="));
			//Neglijam caile ale caror nume incep cu http://
			if (strncmp(bufaux,"\"http://",8) == 0) {
				in_tag=0;
				continue;
			}
			for (i=0;i<6;i++) {
				//Daca exista fisier cu una din extensiile dorite in linie de raspuns, il descarcam
	 			strcpy(mediaFile, extrageFisierExtensie(bufaux,extensii[i]));
				if ( strcmp (mediaFile,"") != 0 ) {
					strcpy(mediaFileList[nrOfFiles],mediaFile);
					nrOfFiles ++ ;					
				}
				
			}
			in_tag=1;
		    }
		     else if (in_tag == 1)
		     	for (i=0;i<6;i++) {
				//Daca exista fisier cu una din extensiile dorite in linie de raspuns, il descarcam
				strcpy(mediaFile, extrageFisierExtensie(recvbuf,extensii[i]));
				if ( strcmp (mediaFile,"") != 0 ) {
					strcpy(mediaFileList[nrOfFiles],mediaFile);
					nrOfFiles ++ ;					
				}
			}
			
		     else if (strstr(recvbuf,"</a>")!=NULL && in_tag==1) {
				int nrCh = strstr(recvbuf,"</a>")-recvbuf + 1;
				strncpy (beforeEndTag, recvbuf, nrCh);
				beforeEndTag[nrCh]='\0';
				for (i=0;i<6;i++) {
					//Daca exista fisier cu una din extensiile dorite in linie de raspuns, il descarcam
					strcpy(mediaFile, extrageFisierExtensie(beforeEndTag,extensii[i]));
					if ( strcmp (mediaFile,"") != 0 ) {
						strcpy(mediaFileList[nrOfFiles],mediaFile);
						nrOfFiles ++ ;					
					}
			
				}
				in_tag=0;
		    }
		    else ;
			
	}
	if (nrOfFiles == 0)
		return NULL;
	mediaFileList = (char **)realloc(mediaFileList,nrOfFiles*sizeof(char *));
	return mediaFileList;
}

/*Functie pentru extragerea dintr-un URL a numelui
directorului parinte pentru pagina de descarcat*/
char *directorParinte (char *linie) {
	char *token = strrchr(linie,'/');
	if (token == NULL)
		return "";
	int poz = token-linie;
	char *result = (char *)malloc(40*sizeof(char));
	strncpy(result,linie,poz+1);
	return result;
}


/*Functie pentru extragerea numelui fisierului de descarcat dintr-un URL*/
char *numeFisier (char *linie) {
	char *token = strrchr (linie, '/');
	if (token == NULL)
		return "";
	int poz = token-linie;
	return linie+(poz+1);
}

/*Functie pentru determinarea pozitiei unui argument intr-o lista de argumente.
Daca argumentul dorit nu exista, se returneaza -1.*/
int pozitie_arg (char **args, char *arg) {
	int i = 0 ;
	while (args[i]!=NULL) {
		if (strcmp(args[i],arg)==0)
			return i;
		i++;
	}
	return -1;
}

int main(int argc, char **argv) {
	int sockfd, port = PORT, poz_argFis = argc -1;
	int flag_e, flag_r, flag_o;

 	if (argc < 2) {
		printf("Numar insuficient de argumente!\n");
		return -1;		
	}

	FILE *erori;
	struct sockaddr_in servaddr;
	struct hostent *host;
	char sendbuf[MAXLEN];
	char nume_server[30], cale_catre_pagina[30];
	char ip_server[20];
	char *URL = strdup(argv[poz_argFis]);
	/*Determin numele serverului*/
	sscanf(URL,"http://%[^/]", nume_server);
	/*Determin calea catre pagina de descarcat*/
	strcpy(cale_catre_pagina,strstr(URL,nume_server)+strlen(nume_server));

	host = gethostbyname(nume_server);
	/*Determin adresa IP a serverului*/
	strcpy(ip_server,inet_ntoa(*(struct in_addr *)host->h_addr_list[0]));

	//Arborele de directoare pentru download implicit
	char nume_director[30];
	strcpy(nume_director,nume_server);
	strcat(nume_director,directorParinte(cale_catre_pagina));
	char comanda[40];
	sprintf(comanda,"mkdir -p %s",nume_director);
	/*Creez arborele de directoare*/
	system ( comanda );
	
	flag_o = pozitie_arg(argv,"-o");
	flag_r = pozitie_arg(argv,"-r");
	flag_e = pozitie_arg(argv,"-e");

	/*Daca optiunea -o este activata, fisierul pentru erori dat ca parametru
	va fi folosit pentru stocarea acestora*/
	if (flag_o!=-1) {
		erori= fopen(argv[flag_o+1],"w");
		if (erori==NULL) {
			fprintf(stderr,"Problem creating error file!\n");	
		}
	}
	/*Altfel erorile sunt afisate la stderr*/
	else 
		erori = stderr;

	struct in_addr ** list = (struct in_addr **)host->h_addr_list;  

	sockfd = socket(PF_INET,SOCK_STREAM,0);
	if (sockfd < 0) {
		fprintf(erori,"Error creating socket!");
		return -1;
	}
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	memcpy( (char *) &servaddr.sin_addr,list[0],host->h_length);
	if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0 ) {
	    fprintf(erori,"Error in connect!\n");
	    return -1;
	}
	
	FILE *fw ;

	//Download implicit
	if (flag_e == -1 && flag_r == -1) { 
		int com = chdir(nume_director);
		
		fw = fopen(numeFisier(cale_catre_pagina),"w");

		/*Trimit serverului cererea de pagina*/
		sprintf(sendbuf, "GET %s HTTP/1.0\n",cale_catre_pagina);
  		download_implicit(sockfd, sendbuf,fw,erori);
		
		fclose(fw);
		fclose(erori);
	
		close(sockfd);

	}
	else if (flag_e != -1) {
		char **extensii = (char **)malloc(6*sizeof(char *));
		extensii[0]=strdup(".gif");
		extensii[1]=strdup(".jpg");
		extensii[2]=strdup(".zip");
		extensii[3]=strdup(".png");
		extensii[4]=strdup(".jpeg");
		extensii[5]=strdup(".pdf");

		int com = chdir(nume_director);
		
		fw = fopen(numeFisier(cale_catre_pagina),"w");

		sprintf(sendbuf, "GET %s HTTP/1.0\n",cale_catre_pagina);
		download_implicit(sockfd,sendbuf,fw,erori);

		fclose(fw);
		close(sockfd);

		sockfd = socket(PF_INET,SOCK_STREAM,0);
		if (sockfd < 0) {
			fprintf(erori,"Error creating socket!");
			return -1;
		}
		memset(&servaddr, 0, sizeof(servaddr));
		servaddr.sin_family = AF_INET;
		servaddr.sin_port = htons(port);
		memcpy( (char *) &servaddr.sin_addr,list[0],host->h_length);
		if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0 ) {
		    fprintf(erori,"Error in connect!\n");
		    return -1;
		}
		char **listaFisiere = listaFisiereLink(sockfd, sendbuf, extensii, erori);
		if (listaFisiere == NULL)
			return 0;
		
		int nrFisiere = sizeof(listaFisiere)/sizeof(char *);
		int i ;
		for (i = 0 ; i < nrFisiere ; i++) {
			sockfd = socket(PF_INET,SOCK_STREAM,0);
			if (sockfd < 0) {
				fprintf(erori,"Error creating socket!");
				return -1;
			}
			memset(&servaddr, 0, sizeof(servaddr));
			servaddr.sin_family = AF_INET;
			servaddr.sin_port = htons(port);
			memcpy( (char *) &servaddr.sin_addr,list[0],host->h_length);
			if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0 ) {
			    fprintf(erori,"Error in connect!\n");
			    return -1;
			}
			sprintf(sendbuf, "GET %s HTTP/1.0\n",listaFisiere[i]); 
			fw = fopen (numeFisier (listaFisiere[i]), "w");
			download_implicit (sockfd, sendbuf, fw, erori);
			fclose(fw);
			close(sockfd);
			
		}		
	}
	return 0;
}


