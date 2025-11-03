#include <stdio.h>
#include <stdlib.h>
void menu(){
    char userinput[100];
    char action1[]="1. Create New Bank Account";
    char action2[]="2. Delete Bank Account";
    char action3[]="3. Deposit";
    char action4[]="4. Withdrawal";
    char action5[]="5. Remittance";
    printf("====================================\n");
    printf("Choose you action:\n");
    printf("%s\n%s\n%s\n%s\n%s",action1,action2,action3,action4,action5);
    //next time add a quit action here
    printf("====================================\n");
    printf("Your action:");
    scanf("%s",userinput);
    //add check for user input validity and action here
}
int main(){
    menu();
    return 0;
}