/*
Zach Martin | 357-07 | PROJECT 4 | Du.h | 5/19/2016
*/

#ifndef DU_H
#define DU_H

#define CNVRSN 1000
#define MAX_FD 10
#define MAX_SUB 100


typedef struct {
   int all;
   int maxDepth;
   int total;
   int human;
} Flags;


void ParseCmds(int argc, char **argv, Flags *flags);

int PrintInfo(const char *fpath, const struct stat *sb, 
 int typeflag, struct FTW *ftwbuf);
 
void PrintTotal();

#endif