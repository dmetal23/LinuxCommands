/*
Daniel Rojas
Comp 322
TuTh 12:30PM 
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h> 
#include <unistd.h>
#include <time.h>

main(int argc, char *argv[]) {
   
   int errors;
   int k; 
   int perm;
   struct stat buf;
   char * p; 
   errors = 0;
   
   for(k = 1; k < argc; k++) {
      /*fetch node information */ 
      if(stat(argv[k], &buf) == (-1)) {
         fprintf(stderr, "%s: cannot access %s\n", argv[0], argv[k]);
         errors++;
         continue; 
      }
      
      printf("\n--User permissions-- \n");
      /*print file permissions */ 
      if(buf.st_mode &0400)
         printf("Read: Yes\n");
      else
         printf("Read: No\n");  
         
      if(buf.st_mode &0200)
         printf("Write: Yes\n");
      else
         printf("Write: No\n");  
         
      if(buf.st_mode &0100)
         printf("Execute: Yes\n");
      else
         printf("Execute: No\n"); 
          
      //Included in pwd.h 
      
      /* struct passwd {
         char *pw_name;
         char *pw_passw
         uid_t pw_uid;
         gid_t pw_gid;
         time_t pw_change;
         char *pw_class;
         char *pw_gecos;
         char *pw_dir;
         char *pw_shell;
         time_t pw_expire;
      };
      
      struct passwd *getpwnam(const char *name);
      struct passwd *getpwuid(uid_t uid)
      
      int getpwnam_r(const char *name, struct passwd *pwd, char *buffer, 
      size_t bufsize, struct passwd **result);
    
      int getpwuid_r(uid_t uid, struct passwd *pwd, char *buffer, 
      size_t bufsize, struct passwd **result);
      */
      
      int size = buf.st_size;
      uid_t c = buf.st_uid;
      struct passwd pwd;
      struct passwd *result;
      char *buff;
      size_t bufsize;
      int s;
      
      bufsize = sysconf(_SC_GETPW_R_SIZE_MAX);
      if (bufsize == -1)          
         bufsize = 16384;        
   
      buff = malloc(bufsize);
      if (buff == NULL) {
         exit(EXIT_FAILURE);
      }
      //s = getpwnam_r(argv[k], &pwd, buf, bufsize, &result);
      s = getpwuid_r(c, &pwd, buff, bufsize, &result);
      if (result == NULL) {
         if (s == 0)
            printf("\nInformation not found.\n");
         else {
            errors++; 
            continue;
         }
         exit(EXIT_FAILURE);
      } 
      printf("\n--User Information--\n");
   
      printf("Name = %s\n", pwd.pw_gecos);
      printf("Username = %s\n", pwd.pw_name);
      printf("Password = %s\n", pwd.pw_passwd);
      printf("User ID = %d\n", pwd.pw_uid);
      printf("Group ID = %d\n", pwd.pw_gid);
      printf("Directory = %s\n", pwd.pw_dir);
      printf("File Size = %d bytes\n", size);
      printf("Last status change: %s", ctime(&buf.st_ctime));
      printf("Last file access: %s", ctime(&buf.st_atime));
      printf("Last file modification: %s", ctime(&buf.st_mtime)); 
      printf("Shell = %s\n\n", pwd.pw_shell);
      exit(EXIT_SUCCESS); 
   }
    
  
   if(errors)
      return(1);
   else
      return(0); 
}
