#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[]){
    pid_t pid;
    int pipefd[2];
    int status;

    //Creating the pipe
    if (pipe(pipefd) < 0){
        perror("pipe");
        exit(1);
    }

    //Creating the child processes
    if((pid = fork()) < 0){
        perror("Fork");
        exit(1);
    }

    // The child process executes the command "xmessage -file -"
    // This command reads text from stdin (until end of file or Ctrl-D)
    // and then displays a popup window on the screen.

    if (pid == 0){
        //Remap file descriptor 0 (standard input)
        //to refer to the pipe resource.
        dup2(pipefd[0], 0);
        close(pipefd[1]);

        excel("/usr/bin/xmessage", "xmessage", "-file", "-", NULL);
        perror("exec");
        exit(127);
     }
    // The parent process is not reading from the pipe.
    close(pipefd[0]);

    //Write a message to the pipe.

    write(pipefd[1], "Greeting. \n\n", 12);
    write(pipefd[1], "This is your program saying hello \n\n", 35);
    write(pipefd[1], "Hope You enjoy this week's prac. \n\n", 34);

    //Close the pipe and wait for the child to exit.
    close(pipefd[1]);
    waitpid(pid, &status, 0);

    exit(0);

}
