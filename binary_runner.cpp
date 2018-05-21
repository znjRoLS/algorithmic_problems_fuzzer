#include <iostream>
#include <unistd.h>
#include <sstream>
#include <sys/wait.h>

#include "binary_runner.h"

#include <errno.h>
#include <cstring>

Binary::Binary(string _cmd) :
        cmd(_cmd) {

}

void Binary::PutInput(string input, int pipe_id) {

    write(pipe_id, input.c_str(), input.length());

//	FILE *stream;

//	if ( (stream = fdopen( pipe_id, "w" )) == NULL ) {
//		perror("fdopen() r");
//		exit(255);
//	}

//	fputs(input.c_str(), stream);
    close(pipe_id);
}


string Binary::GetOutput(int pipe_id) {

    stringstream receive_output;
    char readbuffer[80];

    while (1) {
        int bytes_read = read(pipe_id, readbuffer, sizeof(readbuffer) - 1);

        if (bytes_read <= 0)
            break;

        readbuffer[bytes_read] = '\0';
        receive_output << readbuffer;
    }
    close(pipe_id);

    return receive_output.str();


//	FILE *stream;
//	int ch;
//	stringstream output;
//	
//	if ( (stream = fdopen( pipe_id, "r" )) == NULL )
//	{
//			perror( "fdopen() r" );
//			exit(255);
//	}
//	while ( (ch = getc( stream )) != EOF ) {
//		output << (char)ch;
//		cout << (char)ch;
//	}
//  
//	close(pipe_id);
//
//	return output.str();
}

string Binary::Run(string input, vector<string> args) {

    pid_t nPid;
    int pipeto[2];      /* pipe to feed the exec'ed program input */
    int pipefrom[2];    /* pipe to get the exec'ed program output */

    if (pipe(pipeto) != 0) {
        perror("pipe() to");
        exit(255);
    }
    if (pipe(pipefrom) != 0) {
        perror("pipe() from");
        exit(255);
    }

    nPid = fork();
    if (nPid < 0) {
        perror("fork() 1");
        exit(255);
    } else if (nPid == 0) {
        /* dup pipe read/write to stdin/stdout */
        dup2(pipeto[0], 0);
        dup2(pipefrom[1], 1);
        /* close unnecessary pipe descriptors for a clean environment */
        close(pipeto[0]);
        close(pipeto[1]);
        close(pipefrom[0]);
        close(pipefrom[1]);
        /* call od as an example, with hex and char output */

        if (args.size() == 0) {
            execlp(cmd.c_str(), cmd.c_str(), NULL);
        } else {
//            cout << "first arg " << endl;
//            cout << cmd.c_str() << endl;
//            cout << "second arg " << endl;
//            for (int i = 0; i < 6; i ++) {
//                cout << args[i] << endl;
//            }
            char ** real_args = new char*[args.size()+2];
            real_args[0] = new char[cmd.size()+1];
            strcpy(real_args[0], cmd.c_str());

            for (int i = 0 ; i < args.size(); i ++) {
                real_args[i+1] = new char[args[i].size() + 1];
                strcpy(real_args[i+1], args[i].c_str());
            }
            real_args[args.size()+1] = 0;

            execvp(cmd.c_str(), real_args);

            for (int i = 0 ; i < args.size() + 1; i ++) delete [] real_args[i];
            delete [] real_args;
        }
        int err = errno;

        cout << "error, so executing cat...." << endl;
        cout << "error: " << err << endl;
        execlp("/bin/cat", "/bin/cat", (char *) NULL);
        perror("execlp()");
        exit(255);
    }

    /* Close unused pipe ends. This is especially important for the
    * pipefrom[1] write descriptor, otherwise ReadFromPipe will never
    * get an EOF. */
    close(pipeto[0]);
    close(pipefrom[1]);

    PutInput(input, pipeto[1]);

//	sleep(2);
//
//	if (write(pipeto[1], input.c_str(), input.length()) != input.length()) {
//		cerr << "cuti gugler" << endl;
//	}
//
//	cerr << "written1" << endl;
//
//	sleep(2);
//
//	if (write(pipeto[1], input.c_str(), input.length()) != input.length()) {
//		cerr << "cuti gugler" << endl;
//	}
//
//	close(pipeto[1]);

//    int status;
//    sleep(timelimit);
//    pid_t return_pid= waitpid(nPid, &status, WNOHANG);
//    if (return_pid == -1) {
//        perror( "error while reading status of child" );
//        exit(255);
//    } else if (return_pid == 0){
//        //still runnning! kill it !
//        kill(nPid, SIGKILL);
//        return "***** TLE";
//
//
//    } else if (return_pid == nPid) {
//        //child is finished.
//        string output = GetOutput(pipefrom[0]);
//        return output;
//    }


    string output = GetOutput(pipefrom[0]);

    int status;
    waitpid(nPid, &status, 0);

    return output;
}
