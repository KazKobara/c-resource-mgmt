#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int32_t main(void)
{
  int32_t fd;
  int32_t rc=0;
  const u_int32_t read_size=30;
  char buff[read_size+1];

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

  /* close(0) shall be avoided unless it is intentional
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
