#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <libgen.h>
#include <linux/limits.h>
#include <string.h>
#include "CHK.h"
#include "getword.h"
#define MAXITEM 100 /* max number of words per line */
#define FULLSTORAGE MAXITEM * STORAGE
#define MAXPIPES 11


int BGFLAG;
int HIFLAG;

void sighandler();
void pipecode();
int parse();
