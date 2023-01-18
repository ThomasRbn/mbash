#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>
#include "stdio.h"

typedef struct string string;
char lecture[2048];
char *token;
char historique[100][20]; // tableau de char pour stocker les commandes
int cmpHistorique = 0; // compteur pour le tableau historique
char path[2048];

void mbash(char executable[], char *arguments[]) {
    execvp(executable, arguments);
}


int main(int argc, char **argv) {
    while (1) {

//        for (int j = 0; j < i; j++) {
//            printf("%s\n", args[j]);
//            fflush(stdout);
//        }


        int pid = fork();
        if (pid == 0) {
            char *args[2048];
            char back = 0;

            printf("%s > ", getcwd(path, sizeof(path)));
            fflush(stdout);
            fgets(lecture, 2048, stdin);

            strcpy(historique[cmpHistorique], lecture);
            cmpHistorique++;

            token = strtok(lecture, " \n");
            int i = 0;
            while (token != NULL) {
                args[i] = token;
                if (strcmp(token, "&") == 0) {
                    back = 1;
                }
                token = strtok(NULL, " \n");
                i++;
            }

            char executable[] = "/bin/";
            strcat(executable, args[0]);

            if (strcmp(args[0], "cd") == 0) {
                if (args[1] == NULL || strcmp(args[1], "") == 0 || strcmp(args[1], "~") == 0 ||
                    strcmp(args[1], " ") == 0) {
                    chdir(getenv("HOME"));
                } else {
                    chdir(args[1]);
                }
            } else if (strcmp(args[0], "ls") == 0) {
                if (args[1] == NULL || strcmp(args[1], "") == 0) {
                    args[1] = ".";
                }
                mbash(executable, args);
            } else if (strcmp(args[0], "echo") == 0) {
                if (strncmp(args[1], "$", 1) == 0) {
                    if (strcmp(args[1] + 1, "$") == 0) {
                        printf("%d\n", getpid());
                    } else {
                        printf("%s\n", getenv(args[1] + 1));
                    }
                } else {
                    mbash(executable, args);
                }
            } else if (strcmp(args[0], "pwd") == 0) {
                args[1] = NULL;
                mbash(executable, args);
            } else if (strcmp(args[0], "history") == 0) {
                printf("Historique des commandes exécutées\n");
                fflush(stdout);
                for (int j = 0; j < cmpHistorique; j++) {
                    printf("%d : %s", j, historique[j]);
                    fflush(stdout);
                }
            } else {
                mbash(executable, args);
            }
        } else {
            waitpid(-1, NULL, 0);
        }
    }

}

