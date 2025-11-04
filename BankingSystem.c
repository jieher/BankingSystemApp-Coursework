#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
int checkAction(char *userInput){
    char *inputNoSp=userInput;
    //Input with no spaces
    char *ogInput=userInput;
    //Means Original Input
    //Check for spaces and symbols
    int i=0;
    int a=0;
    int inputLength=strlen(userInput);
    do {
        // while (*ogInput == ' ' || *ogInput=='/' || *ogInput==')') {
        //     ++ogInput;
        // }
        if (inputLength==i){
            while (!isalnum(*ogInput)){
                ++ogInput;
                a++;
            }
            if (a>0){
                i+=a;
                a=0;
            }
            //Yes, this complicated thing, I CREATED MYSELF WITHOUT USING AI!!!!
        }
    } while (*inputNoSp++ = *ogInput++);
    printf("%s\n",userInput);
    char actionno[100];
    a=0;
    for(int i=0; userInput[i]; i++){
        if (a<=1){
            if (isdigit(userInput[i])){
                actionno[a]=userInput[i];
                a++;
            }
            userInput[i] = tolower(userInput[i]);
        } else {
            printf("Error: Please enter number 1~6 only.\n");
            return 0;
        }
    }
    printf("%s %s",userInput,actionno);
    //add check for user input validity and action here
}
void menu(){
    char userInput[100];
    char action1[]="1. Create New Bank Account";
    char action2[]="2. Delete Bank Account";
    char action3[]="3. Deposit";
    char action4[]="4. Withdrawal";
    char action5[]="5. Remittance";
    int valid=0;
    while (!valid){
        printf("====================================\n");
        printf("Choose you action:\n");
        printf("%s\n%s\n%s\n%s\n%s\n",action1,action2,action3,action4,action5);
        //next time add a quit action here
        printf("====================================\n");
        printf("Your action:");
        scanf("%[^\n]",userInput);
        while((getchar()) != '\n');
        //eg: input=1) Check New
        int valid=checkAction(userInput);
    }
}
int main(){
    menu();

    return 0;
}