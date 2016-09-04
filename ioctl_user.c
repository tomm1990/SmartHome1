
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MY_MACIG 'G'
#define READ_IOCTL _IOR(MY_MACIG, 0, int)
#define WRITE_IOCTL _IOW(MY_MACIG, 1, int)
#define LSEEK_IOCTL _IOW(MY_MACIG, 3, int)
#define CHTAPE_IOCTL _IOR(MY_MACIG, 4, int)
#define FMTTAPE_IOCTL _IOW(MY_MACIG, 5, int)
#define GETSTATUS_IOCTL _IOR(MY_MACIG, 6, int)
#define SETSIZE_IOCTL _IOR(MY_MACIG,7,int)

static char buf[2000];
static struct stat fInfo;

void formatAll(int fdIoctl)
{
	int i=0;
	ioctl(fdIoctl,CHTAPE_IOCTL,1);
	for(;i<10;i++,ioctl(fdIoctl,CHTAPE_IOCTL,0))
		ioctl(fdIoctl,FMTTAPE_IOCTL,0);
}

void printTapesStatus(int fdIoctl)
{
	int size=ioctl(fdIoctl,GETSTATUS_IOCTL,0);
	int i=0;
	if(size==-1)
		printf("all tapes free\n");
	else
	{
		for(;i<=size;++i)
			printf("tape %d is used\n",i);
		if(size<10)
			for(int j=size+1;j<10;j++)
				printf("tape %d is free\n",j);
	}
}
void saveFile(int fdIoctl,int fdFile)
{
	fstat(fdFile,&fInfo);
	ioctl(fdIoctl,SETSIZE_IOCTL,fInfo.st_size);
	read(fdFile,buf,fInfo.st_size);
	if(ioctl(fdIoctl,WRITE_IOCTL,buf)<0)
		perror("write\n");
}
void restoreFile(int fdIoctl,int fdFile)
{
	if(ioctl(fdIoctl, CHTAPE_IOCTL, 1) < 0)
		perror("change tape\n");
	if(ioctl(fdIoctl,READ_IOCTL,fInfo.st_size)<0)
		perror("reade\n");
	write(fdFile,buf,fInfo.st_size);
	
}

int main( int argc, char *argv[])
{
int fdIoctl = -1;
int fdFile = -1;
fdIoctl= open("/dev/cdev_example", O_RDWR);
fdFile= open(argv[1], O_RDWR);
if (fdFile<0 || fdIoctl<0) 
{
	perror("open");
	return -1;
}
formatAll(fdIoctl);
printTapesStatus(fdIoctl);
saveFile(fdIoctl,fdFile);
if(remove(argv[1])<0)
	perror("remove");
fdFile=open(argv[2],O_CREAT|O_TRUNC|O_RDWR,0777);
if(fdFile<0)
	perror("open");
restoreFile(fdIoctl,fdFile);
printf("tape after changes\n");
printTapesStatus(fdIoctl);
}
