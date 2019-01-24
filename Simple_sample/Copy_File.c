// refer http://160592857366.free.fr/joe/ebooks/ShareData/Design%20of%20the%20Unix%20Operating%20System%20By%20Maurice%20Bach.pdf
// page number : 9
// code snippet fig : 1.3

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
