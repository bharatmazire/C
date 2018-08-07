#include <iostream>
#include <stdio.h>

using namespace std;

int main()
{
    char str1[100] = "bharat";
    char str2[100] = "bhau";
    
    int count1[26] = {0};
    int count2[26] = {0};
    
    
    int arrayIndex = 0;
    for(int i = 0 ; str1[i] != '\0' ; i ++)
    {    
        arrayIndex = str1[i] - 'a';
        cout <<"We are changing value of index : "<<arrayIndex<<"\n";
        count1[str1[i]-'a'] = count1[str1[i]-'a'] + 1;
    }
    
    for(int i = 0 ; i < 26 ; i ++)
    {
        cout<<"Count of alphabates in 1st string is :\n";
        printf("%c : %d \n",i+65,count1[i]);
        
    }    
    
    arrayIndex = 0;
    for(int i = 0 ; str2[i] != '\0' ; i ++)
    {
        arrayIndex = str2[i] - 'a';
        cout <<"We are changing value of index : \n"<<arrayIndex<<"\n";
        count2[str2[i]-'a'] = count2[str2[i]-'a'] + 1;
    }
    
    for(int i = 0 ; i < 26 ; i ++)
    {
        cout<<"Count of alphabates in 2nd string is :\n";
        printf("%c : %d \n",i+65,count2[i]);
        
    }
    
    int finalCount = 0;
    for(int i = 0 ; i < 26 ; i ++)
        finalCount = finalCount + abs(count1[i] - count2[i]);

    cout << "Final count To remove for anagram is : "<< finalCount;
    return 0;
}
