#include<stdio.h>
#include<fcntl.h>

char buff[1024];

int main()
{
  char source[100];
  printf("Enter Source file name (full path ) : ");
  scanf("%s",&source);

  int fdSrc = open(source,O_RDONLY);

  int count;
  int name = 0;
  
  while((count = read(fdSrc,buff,sizeof(buff))) > 0)
  {
    name++;
    int fdDes = creat((char)name,0777);
    fdDes = write(fdDes,buff,count);
  }

  return(0);
}
