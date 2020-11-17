#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(){

	//char *filename = "/proc/procrank";
	char line[256];
	char *pids[1000];
	char *uss[1000];
	char *vss[1000];
	char *rss[1000];
	char *pss[1000];
	//char c;
	FILE *file;
	file = fopen("/proc/procrank","r");
	if (file == NULL){
		perror("Error while opening the file!\n");
		exit(EXIT_FAILURE);
	}
	printf("The content of /proc/procrank file are:\n");

	// while((c = fgetc(file)) != EOF){
	// 	printf("%c", c);
	// }
	// 	fclose(file);
	// 	return 0;
	
	// fscanf(file, "%[^\n]", line);
	// printf("Data from file:\n%s", line);
	// fclose(file);
	// printf("bro %d\n", line[0] );
	int i = 0;
	while(fgets(line, sizeof(line), file) && i<15){
		char  *token = strtok(line, "|");
		/*
		strtok permet de décomposer une chaine de caractère suivant un délimiteur. La boucle while en dessous 
		permet de parcourir les différentes valeurs obtenues lors de la décomposition. une ligne ressemble à:
		ligne : " 279 |   systemd-journal |     366284 |     174448 |      64568 |     116482"
		de la gauche vers la droite nous avons respectivement le PID, le nom du process, le VSS, le RSS, l'USS et
		le PSS. Ce sont les colonnes 1 et 3 qui m'interessent donc, d'ou les if que je fais dans le while
		*/
		int j = 0;
		while(token != NULL){
			//printf("Token: %s\n",token );
			//pids[i] = token;
			//printf("pid de token%s\n", pids[i]);
			printf("token:%s\n", token);
			 if (j==0){
			 	pids[i] = token;
			 	printf("pid[%d] vaut %s\n",i, pids[i]);
			 	if (i>1){
			 		printf("pid[%d] vallait %s\n",i-1, pids[i-1]);
			 	}
			 }
			 else if(j==3){
			 	uss[i] = token;
			 }
			 else{
			 	token = strtok(NULL, "|");
			 }
			
			j++;
		}
		i += 1;

	}
	fclose(file);
	//printf("%s\n", pids);
	for (int i = 0; i < sizeof(*pids); ++i)
		{
			printf("pids[%d] = %s\n",i, pids[i]);
		}

}