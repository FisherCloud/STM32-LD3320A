#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ff.h"
#include "fatfs_posix.h"

static FIL fsrc;

int open(const char *file, int flags, int mode)
{
	 memset(&fsrc,0,sizeof(FIL));

	 if( f_open(&fsrc, file, FA_OPEN_EXISTING | FA_READ) == FR_OK)
	 {
	 	 return (int)&fsrc;
	 }

	 return -1;
	
}

int close(int fd)
{
   if(f_close ((FIL *)fd) == FR_OK) 
   {
      return 0;
   }
    
   return -1;

}

int read(int fd, void *buf, size_t len)
{
   UINT br;

   if( f_read((FIL *)fd, buf, len, &br) == FR_OK ) 
   {
   	 return br;
   }

   return -1;

}

int write(int fd, const void *buf, size_t len)
{

  return 0;
}

long tell(int fd)
{
	FIL *pfsrc = (FIL *)fd;

	return pfsrc->fptr;
}


long lseek(int fd, long offset, int whence)
{

   if( whence == SEEK_SET)
   {
   
	   if( f_lseek((FIL *)fd,offset) == FR_OK)
	   {
		   return tell(fd);
	   }  
	   
   }
   else
   if( whence == SEEK_CUR)
   {
 	   if( f_lseek((FIL *)fd,tell(fd)+offset) == FR_OK)
	   {
	   	  return tell(fd);
	   }  
   }
   else
   if( whence == SEEK_END)
   {
		return tell(fd);
   }

   return -1;

}

long get_file_size(int fd)
{
	FIL *pfsrc = (FIL *)fd;

	return pfsrc->fsize;
}
