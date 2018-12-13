// including headers
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/msg.h>
 
// defining macros
#define MAX_TEXT 512
#define BUF_SIZE 512
 
// defining msg struct
struct msg_struct
{
        char my_msg[MAX_TEXT];
};
 
// main
int main()
{
        // declaring and initializing variables
        char alias_name[20];
        char chat_name[20];
        int running = 1;
        int alias_msgid, chat_msgid;
        int choice = 1;
        struct msg_struct my_struct;
        char buffer[BUF_SIZE];

        printf("\tWelcome!\n");
        printf("\t\tEnter Your alias name:");
        scanf("%s",&name);
 
        alias_msgid = msgget((key_t)alias_name, 0666 | IPC_CREAT);
        if(alias_msgid == -1)
        {
                printf("Error while creating msgid for : %s",alias_name);
                exit(EXIT_FAILURE);
        }
 
        while(choice != 4)
        {
                printf("\t\t\t1. Chat \n\t\t\t2. View your message \n\t\t\t3. Vi
                scanf("%d",&choice);
                int type = 1;
                switch(choice)
                {
                case 1: printf("Enter chat person name : ");
                        scanf("%s",&chat_name);
                        chat_msgid = msgget((key_t)chat_name, 0666 | IPC_CREAT);
                        if(chat_msgid == -1)
                        {
                                printf("Error while creating msgid for : %s",cha
                                exit(EXIT_FAILURE);
                        }
                        printf("Type your text :\n");
                        while(type)
                        {
                                printf("%s : ",alias_name);
                                fgets(buffer,BUF_SIZE,stdin);
                                strcpy(my_struct.my_msg,buffer);
                                if(msgsnd(chat_msgid,(void *)&my_struct,MAX_TEXT
                                {
                                        printf("Message Send Failed\n");
                                        exit(EXIT_FAILURE);
                                }
                                if(strcmp(buffer,"end",3) == 0)
                                {
                                        type = 0;
                                }
 
                        }
                        break;
                case 2: printf("view \n");
                        break;
                case 3: printf("v \n");
                        break;
                case 4: printf("Exiting\n");
                        break;
                default:printf("Please Enter Valid Choice \n");
                        break;
                }
        }
        return(0);
        }

                          
