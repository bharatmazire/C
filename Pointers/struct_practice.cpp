#include<stdio.h>
#include<stdlib.h>

struct MY_INFO
{
    int roll_no;
    char *name;
};

struct MY_INFO * fill_struct()
{
    struct MY_INFO * info_list  = NULL;
    info_list                   = (struct MY_INFO *) malloc(sizeof(struct MY_INFO) * 10);
    
    for(int i = 0; i < 10; i++)
    {
        info_list[i].roll_no    = i+1;
        info_list[i].name       = (char *)malloc(sizeof(char *) * 10);
        for(int j = 0; j<9;j++)
        {
            info_list[i].name[j] = 56 + i + j;
        }
    }
    
    return(info_list);
}

void read_struct(struct MY_INFO * info_list)
{
    for(int i = 0 ; i < 10 ; i ++)
    {
        printf("Roll Number : %d\n",info_list[i].roll_no);
        printf("Name : %s\n",info_list[i].name);
        printf("\n\n");
    }
}

int main()
{
    struct MY_INFO * info_list = fill_struct();
    
    read_struct(info_list);

    printf("Printing in MAIN \n");
    printf("Roll Number : %d\n",info_list[0].roll_no);
    printf("Name : %s\n",info_list[1].name);
    printf("\n\n");
    
    printf("pointer value of info_list : %s",info_list[0].name);

    return(0);
}
