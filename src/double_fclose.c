#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>   // for open()
#include <unistd.h>  // for close()
#include <errno.h>

/**
 * @retval  0 success
 * @retval -1 failure
 */
int32_t main(void)
{
  FILE *fp, *fp2;
  int32_t rc, fd;
  u_int32_t rcu;
  const u_int32_t read_size=30;
  char buff[read_size+(u_int32_t)1];

  const char* const file_name = "CMakeCache.txt";
  struct stat orig_st;

  if (-1 == (fd = open("CMakeCache.txt", O_RDONLY))){
    perror("perror open()");
    return -1;
  }
  /* Check the property of opened file descriptor to avoid
   * TOCTOU race condition attack.
   * open-check-fdopen
   */
  if (0 != (fstat(fd, &orig_st)) || (!S_ISREG(orig_st.st_mode))){
    (void)fprintf(stderr, "Error: file {%s} is not a regular file!!", file_name);
    if (-1 == (rc = close(fd))){
      perror("perror close()");
      (void)fprintf(stderr, "close(%d), errno = %d\n", fd, errno);
    }
    return -1;
  }
  if (NULL == (fp = fdopen(fd, "r"))) {
    perror("perror fdopen()");
    if (-1 == (rc = close(fd))){
      perror("perror close()");
      (void)fprintf(stderr, "close(%d), errno = %d\n", fd, errno);
    }
    return -1;
  }

  #if 0
  /* Alternative */
  if ((NULL == (fp = fopen(file_name, "r")))){
    perror("perror fopen()");
    return -1;
  }
  /* Check the property of opened file descriptor to avoid */
   * TOCTOU race condition attack.
   * fopen-check-fd
   */
  if (-1 == (fd = fileno(fp))){
    perror("perror fileno()");
    ret = -1;
    goto Done;
  }
  if ((fstat(fd, &orig_st) != 0) || (!S_ISREG(orig_st.st_mode))){
    fprintf(stderr, "Error: file {%s} is not appropriate!!", file_name);
    ret = -1;
    goto Done;
  }
  #endif

  fp2 = fp;  // Do not duplicate a pointer to be closed

  /* fread() */
  if (read_size != (rcu = fread(buff, sizeof(char), read_size, fp))){
    if (feof(fp) == 0){
      perror("perror fread()");
      if (EOF == fclose(fp)){
        perror("perror fclose()");
      }
      return -1;
    }
  }
  buff[rcu*sizeof(char)] = '\0';
  (void)printf("fread fp  %u chars: '%s'\n", rcu, buff);

  if (read_size != (rcu = fread(buff, sizeof(char), read_size, fp2))){
    if (feof(fp2) == 0){
      perror("perror fread()");
      if (EOF == fclose(fp2)){
        perror("perror fclose()");
      }
      return-1;
    }
  }
  buff[rcu*sizeof(char)] = '\0';
  (void)printf("fread fp  %u chars: '%s'\n", rcu, buff);

  /* First fclose() to check the behavior of double fclose() */
  if (EOF == fclose(fp2)){
    perror("perror fclose()");
    return -1;
  }
  fp2 = NULL; // Set NULL after fclose() to avoid double-fclose

  /* Second fclose() to check the behavior of double fclose() */
  /* if (NULL != fp)  should be added */ 
  if (EOF == fclose(fp)){
    perror("perror fclose()");
    return -1;
  }
  fp = NULL; // Set NULL after fclose() to avoid double-fclose

  (void)printf("Done\n");
  return 0;
}
