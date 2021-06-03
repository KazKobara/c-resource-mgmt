#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#define READ_SIZE 30

/**
 * @retval  0 success
 * @retval -1 failure
 */
int32_t main(void)
{
  int32_t fd;
  int32_t rc, ret=0;
  const u_int32_t read_size=READ_SIZE;
  char buff[READ_SIZE+1];

  if (-1 == (fd = open("CMakeCache.txt", O_RDONLY))){
    perror("perror open()");
    return -1;
  }

  /* read() */
  if (-1 == (rc = read(fd, buff, read_size))){
    perror("perror read()");
    ret=-1;
    goto Done;
  }
  buff[rc] = '\0';
  (void)printf("read %d octets: '%s'\n", rc, buff);

  /* Return code of close() should be checked. */
  /* First close() to check double close() */
  /* 'double close' shall be avoided
   since it might happen to close a file descriptor assigned by the other thread, etc.
  'double close' of a non-opened file descriptor returns -1 with errno 9 (Bad file descriptor).
  */
  if (-1 == (rc = close(fd))){
    perror("perror close()");
    return -1;
  }
  // fd = -1; should be added.

  Done:
  /* Bad Example!! */
  /* Second close() to check the behavior of double close() */
  /* if (-1 != fd) should be added. */
  if (-1 == (rc = close(fd))){
    perror("perror close()");
    (void)fprintf(stderr, "close(%d), errno = %d\n", fd, errno);
    return -1;
  }

  (void)printf("Done\n");
  return ret;
}
