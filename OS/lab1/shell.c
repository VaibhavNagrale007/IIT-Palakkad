#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>

#define BUFF_SIZE 200       //total length of input 
#define ARG_SIZE 20         //number of characters in each argument string.
#define ARG_COUNT 10        //number of arguments to command including the given command.
#define INPUT 0
#define OUTPUT 1
#define ERROR 2

void sigintHandler(int sig_num){
    signal(SIGINT, sigintHandler);
    printf("PROCESS INTERRUPTED!\n");
}

void sigtstpHandler(int sig_num){
    signal(SIGTSTP, sigtstpHandler);
    printf("PROCESS STOPPED!\n");
}

void sigcontHandler(int sig_num){
    signal(SIGCONT, sigcontHandler);
    printf("PROCESS RESUMED!\n");
}


int ParseBuff(char *argv[], char *buff){
    int i=0, arg_ind = 0;
    while(i<BUFF_SIZE){
        if(buff[i] == '\n')break;
        else if(buff[i] != ' '){
            int j = i;
            while(buff[j] != ' ' && buff[j] != '\n'){
                j++;
            }

            char *tok = (char *) calloc(j-i,sizeof(char));
            //memset(tok, '0', sizeof(char) * (j-i));
            for(int p=0; p<(j-i); p++){
                tok[p] = buff[i+p];
            }

            argv[arg_ind] = tok;
            arg_ind++;
            i = j;
        }
        else    i++;
    }

    return arg_ind;
}

void tokenize_buffer(char** param,int *nr,char *buf,const char *c){
	char *token;
	token=strtok(buf,c);
	int pc=-1;
	while(token){
		param[++pc]=malloc(sizeof(token)+1);
		strcpy(param[pc],token);

		if(buf[strlen(buf)-1]==' ' || buf[strlen(buf)-1]=='\n')
	    buf[strlen(buf)-1]='\0';
	    if(buf[0]==' ' || buf[0]=='\n') memmove(buf, buf+1, strlen(buf));

		token=strtok(NULL,c);
	}
	param[++pc]=NULL;
	*nr=pc;
}

void freemem(char * arr[], int len){
    for(int i=0; i<len; i++) free(arr[i]);
    return;
}

void RunSimpleCommand(char *argv[], int arg_ind, int bg){
    pid_t pid = fork();
    if(pid < 0){
        write(2, "Fork failed.", 13);
        // write(1, "entered less that 0 \n", 22);
    }

    else if(pid == 0){
        char *stf[] = {"ping", "-c", "1", "google.com", NULL};
        int err = execvp(argv[0], argv);
        if(err = -1){
            printf("No such command : %s\n", argv[0]);
        }
        return;
    }

    else{
        if(bg == 0){
            int wt = waitpid(pid, NULL, WUNTRACED);
        }
        else{
            write(1, &pid, sizeof(int));
        }
        // write(1, &wt, 1);
    }
    freemem(argv, arg_ind);

    return;
}

int freq(char buf[BUFF_SIZE], char chr[]){
    int count = 0, i, j = 0, k;
    // char str[100], str1[20];
 
    // printf("Enter the string:\n");
    // scanf(" %[^\n]s", str);
 
    // printf("Enter the substring to be matched:\n");
    // scanf(" %[^\n]s", str1);
 
    k = strlen(chr);
 
    for (i = 0; buf[i] != '\0'; i++){
        while (buf[i] == chr[j]){
            j ++;
        }
 
        if (j == k){
            i+=j;
            count ++;
            j = 0;
        }
    } 
    return count;
}

int multipipe(char userin[BUFF_SIZE]){
    
    // char *buff[ARG_COUNT];

    char user[BUFF_SIZE];
    strcpy(user, userin);

    int bg = 0;
    if(strstr(user, "&\n") != NULL || strstr(user, " & ") != NULL)   bg = 1;


    for(int i=0; i<BUFF_SIZE; i++) if(user[i] == '\n' || user[i] == '&') user[i] = '\0';
    int n = freq(user, "||");
    // int n = 3;
    n+=2;
    char *buff[ARG_COUNT];
    int k=0;
    tokenize_buffer(buff, &k, user, "||");

    int fd[n][2];
    for(int i=0; i<n; i++){
        if (pipe(fd[i]) < 0){
            return 1;
        }
    }

    int pidarr[n-1];

    for(int i=0; i<n-1; i++){
        pidarr[i] = fork();
        if(pidarr[i] < 0){
            write(2, "Error forking process", 22);
            return i+10;
        }
        else if(pidarr[i] == 0){
            close(fd[i][1]);
            close(fd[i+1][0]);
            for(int j=0; j<n; j++){
                if(j != i && j != i+1){
                    close(fd[j][1]);
                    close(fd[j][0]);
                }
            }

            char buf[300];
            

            int newFdIn = dup2(fd[i][0], STDIN_FILENO);
            int newFdOut = dup2(fd[i+1][1], STDOUT_FILENO);

            close(fd[i][0]);
            close(fd[i+1][1]);

            int l=0;
            char *argv[ARG_COUNT];
            tokenize_buffer(argv, &l, buff[i], " ");

            if(execvp(argv[0], argv) == -1){
                buf[299] = '\0';
                read(newFdIn, buf, 299);
                write(newFdOut, buf, 299);
                exit(1);
            }
            return 0;
        }
    }

    close(fd[0][0]);
    close(fd[n-1][1]);
    for(int j = 0; j< n; j++){
        if(j != 0 && j != (n-1)){
            close(fd[j][0]);
            close(fd[j][1]);
        }
    }
    // int x =0;
    // if(write(fd[0][1], &x, sizeof(int)) < 0){
    //     return 8;
    // }

    // if(read(fd[n-1][0] , &x, sizeof(int)) < 0){
    //     return 9;
    // }

    // printf("The result is %d\n", x);

    char buf[300];
    write(fd[0][1], buf, 299);
    read(fd[n-1][0], buf, 299);

    // fflush(stdin);
    // fflush(stdout);

    printf("%s", buf);

    close(fd[0][1]);
    close(fd[n-1][0]);

    if(bg == 0){
        // for(int i=0; i<(n-1); i++){
        //     waitpid(pidarr[i], NULL, WUNTRACED);
        // }
        waitpid(-1, NULL, WUNTRACED);
    }    
    else{
        write(1, "process running in background", 30);
    }
    return 0;
}

void runRedirect(char *argv[],int mode, char *file){
	int fd;

    pid_t pid = fork();
	if(pid == 0){

		switch(mode){
		case INPUT:  fd=open(file,O_RDONLY); break;
		case OUTPUT: fd=open(file,O_WRONLY | O_CREAT); break;
		case ERROR: fd=open(file,O_WRONLY | O_CREAT); break;
		default: return;
		}

		if(fd<0){
			perror("cannot open file\n");
			return;
		}

		switch(mode){
		case INPUT:  		dup2(fd,0); break;
		case OUTPUT: 		dup2(fd,1); break;
		case ERROR: 		dup2(fd,1); dup2(fd, 2); break;
		default: return;
		}
		execvp(argv[0],argv);
		perror("invalid input ");
		exit(1);//in case exec is not successfull, exit
	}
    else if(pid < 0){
        write(STDOUT_FILENO, "error forking process", sizeof(char) * 22);
    }

	else{
        switch(mode){
            case OUTPUT: write(STDOUT_FILENO, "output saved to file.", sizeof(char)*22);break;
            case INPUT: write(STDOUT_FILENO, "successfully read input from file.", 35);break;
            case ERROR: write(STDOUT_FILENO, "Successfully redirected output to stderr", 41); break;
            default: return;} 
        waitpid(-1, NULL, WUNTRACED);
    }
}

int main(){
	signal(SIGINT, sigintHandler);
    signal(SIGTSTP, sigtstpHandler);
    signal(SIGCONT, sigcontHandler);
    while(1){
        // if (getcwd(cwd, sizeof(cwd)) != NULL){
		//     write(1, cwd, 1024);
        // }
        write(1, "\n>>> ", 5);
        char buff[ BUFF_SIZE + 1];
        for(int i=0; i<BUFF_SIZE + 1; i++) buff[i] = '\0';
        int read_count = read(0, buff, BUFF_SIZE + 1);

        if(read_count > BUFF_SIZE){
            write(STDERR_FILENO, "Buffer size exceeded! \n", 24);
            fflush(stdin);
        }

        else if(read_count == 1) continue;
        else{
            char *argv[ARG_COUNT + 1];
            for(int i=0; i<ARG_COUNT + 1; i++) argv[i] = NULL;

            int arg_ind = ParseBuff(argv, buff);

            if(strcmp(argv[0], "cd") == 0){
                chdir(argv[1]);
            }
            else if(strcmp(argv[0], "pwd") == 0){
                char *cwd = (char *) malloc(sizeof(char) *1024);
                if (getcwd(cwd, sizeof(char) * 1024) != NULL){
                    write(STDOUT_FILENO, cwd,  1024);
                }
            }
            else if(strcmp(argv[0], "exit") == 0){
                exit(0);
            }
            else{
                if(strstr(buff, " || ")){
                    multipipe(buff);
                }
                else if(strstr(buff, " <<< ")){
                    char file[ARG_SIZE];
                    for(int i=0; i<arg_ind; i++){
                        if(argv[i] == NULL) break;
                        if(strstr(argv[i], "<<<") != NULL){
                            argv[i] = NULL;
                            if(i != arg_ind-1){
                                strcpy(file, argv[i+1]);
                                argv[i+1] = NULL;
                            }
                        }
                    }

                    runRedirect(argv, INPUT, file);
                }
                else if(strstr(buff, " >>> ")){
                    char file[ARG_SIZE];
                    for(int i=0; i<arg_ind; i++){
                        if(argv[i] == NULL) break;
                        if(strstr(argv[i], ">>>") != NULL){
                            argv[i] = NULL;
                            if(i != arg_ind-1){
                                strcpy(file, argv[i+1]);
                                argv[i+1] = NULL;
                            }
                        }
                    }

                    runRedirect(argv, OUTPUT, file);
                }
                else if(strstr(buff, " &>> ")){
                    char file[ARG_SIZE];
                    for(int i=0; i<arg_ind; i++){
                        if(argv[i] == NULL) break;
                        if(strstr(argv[i], "&>>") != NULL){
                            argv[i] = NULL;
                            if(i != arg_ind-1){
                                strcpy(file, argv[i+1]);
                                argv[i+1] = NULL;
                            }
                        }
                    }

                    runRedirect(argv, ERROR, file);
                }
                else{
                            int bg = 0;
                            if(strstr(argv[arg_ind-1], "&") != NULL) argv[arg_ind - 1] = NULL;
                            RunSimpleCommand(argv, arg_ind, bg);
                }
            }
        }
    }
    return 0;
}

