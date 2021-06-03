#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define READ_SIZE 30

/**
 * @retval  0 success
 * @retval -1 failure
 */
int32_t main(void)
{
  int32_t fd;
  int32_t rc=0;
  const u_int32_t read_size=READ_SIZE;
  char buff[READ_SIZE+1];

  if (-1 == (fd = open("CMakeCache.txt", O_RDONLY))){
    perror("perror open()");
    return -1;
  }

  /* read() */
  if (-1 == (rc = read(fd, buff, read_size))){
    perror("perror read()");
    goto Done;
  }
  buff[rc] = '\0';
  (void)printf("read %d octets: '%s'\n", rc, buff);

  /* Return code of close() should be checked. */
  if (-1 == (rc = close(fd))){
    perror("perror close()");
    (void)fprintf(stderr, "close(%d), errno = %d\n", fd, errno);
    return -1;
  }

  /* Bad Example!! 
    close(0) shall be avoided unless it is intentional
    since it closes standard input and does not cause any error at that point.
  */
  fd = 0;
  Done:
  if (-1 == (rc = close(fd))){
    perror("perror close()");
    (void)fprintf(stderr, "close(%d), errno = %d\n", fd, errno);
  }

  (void)printf("Done\n");
  return rc;
}
