/*
Zach Martin | 357-07 | PROJECT 5 | Du.c | 5/29/2016
*/

#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <ftw.h>
#include "Du.h"

static Flags *flags;
static off_t total[100];

int main(int argc, char **argv) {
   flags = calloc(sizeof(Flags), 1);
   ParseCmds(argc, argv, flags);
   
   if (nftw(".", PrintInfo, MAX_FD, FTW_DEPTH | FTW_PHYS) != 0) {
      perror("ftw");
	  exit(EXIT_FAILURE);
   }   

   if (flags->total)
      PrintTotal();
   
   free(flags);
   return 0;
}

void ParseCmds(int argc, char **argv, Flags *flags) {
   int option;
   char *targ;
   
   while ((option = getopt(argc, argv, "achm:")) != -1) {
      switch (option) {
         case 'a' :
            flags->all = 1;
            break;
         case 'c' :
            flags->total = 1;
            break;
         case 'h' :
            flags->human = 1;			
            break;
         case 'm' :
            flags->maxDepth = atoi(optarg);
            break;
         default :
            printf("Usage: du -a(files+dirs) -c(total) -h(human) -mDEPTH\n");
            exit(EXIT_FAILURE);			
	  }
   }
}

int PrintInfo(const char *fpath, const struct stat *sb, 
 int typeflag, struct FTW *ftwbuf) {
   char *size = "BKMGT";
   float tempFile, tempDir;
   static off_t dirTotal = 0;
   int itr = 1;
   
   if (typeflag == FTW_DNR) {
      printf("Could not open dir: %s\n", fpath);
   }

   if ((flags->maxDepth && ftwbuf->level <= flags->maxDepth) ||
    (flags->maxDepth == 0)) {
      if (typeflag == FTW_DP) {
         if (ftwbuf->level == 0) 
            tempDir = total[0];
         else {
            tempDir = total[ftwbuf->level + 1];
		    total[ftwbuf->level] += total[ftwbuf->level + 1];
            total[ftwbuf->level + 1] = 0;
		 }

         if (flags->human) {
            while (tempDir > CNVRSN) {
               tempDir /= CNVRSN;
               size++;
            }
            printf("%.1f%c", tempDir, *size);
	     }
         else
            printf("%.0f", tempDir);
         printf("\t%s\n", fpath);
		 if (ftwbuf->level == 1) {
            while (itr < MAX_SUB) 
		       total[itr++] = 0;		
		 }
         if (tempDir == 0)
            printf("Cannot access files in %s because of MaxDepth\n\n", fpath);
            

      }

      else if (typeflag == FTW_F) {
         total[ftwbuf->level] += sb->st_size; 
		 total [0] += sb->st_size;
         tempFile = sb->st_size;
         if (flags->all) {
            if (flags->human) {
               while (tempFile > CNVRSN) {
                  tempFile /= CNVRSN;
                  size++;
		       }
               printf("%.1f%c", tempFile, *size);
	        }
            else
               printf("%.0f", tempFile);
            printf("\t%s\n", fpath);
		 }
      }
   }
   return 0;
}


void PrintTotal() {
   char *size = "BKMGT";
   float temp = total[0];
   
   if (flags->human) {
      while (temp > CNVRSN) {
         temp /= CNVRSN;
         size++;
      }
      printf("Total: %.1f%c\n", temp, *size);
   }
   else 
      printf("Total: %.0f\n", temp);
   
}