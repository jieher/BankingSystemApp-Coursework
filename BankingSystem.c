#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
void create(){
    printf("This is create");
}

void delete(){
    printf("This is delete");
}

void deposit(){
    printf("This is deposit");
}

void withdrawal(){
    printf("This is withdrawal");
}

void remittance(){
    printf("This is remmittance");
}

void checkAction(char *userInput, int *valid, int *action){
    int *validPtr=valid;
    *validPtr=1;
    int *actionPtr=action; 
    char *inputNoSp=userInput;
    //Input with no spaces
    char *ogInput=userInput;
    //Means Original Input
    //Check for spaces and symbols
    int index_of_ogInput=0;
    int inputLength=strlen(userInput);
    while (*ogInput != '\0') {
        if (!isalnum(*ogInput)){
            ogInput++;
            continue;
        }
        *inputNoSp++ = *ogInput++;
        // Copy the character one by one
    }
    *inputNoSp='\0';
    printf("%s\n",userInput);

    int a=0;
    int i=0;
    for(i=0; userInput[i]!='\0'; i++){
        if (isdigit(userInput[i])){
            if (a==0){
                *actionPtr=userInput[i]- '0';
                a++;
            } else {
                printf("Error: Please enter number 1~6 only.\n");
                *validPtr=0;
            } 
        }
        else {
            userInput[i-a] = tolower(userInput[i]);
            continue;
        } 
    }
    userInput[i-1]='\0';
    printf("%s %d\n\n",userInput,*actionPtr);
    //add check for user input validity and action here
    // *validPtr=1;
}

void menu(){
    char userInput[100];
    char action1[]="1. Create New Bank Account";
    char action2[]="2. Delete Bank Account";
    char action3[]="3. Deposit";
    char action4[]="4. Withdrawal";
    char action5[]="5. Remittance";
    char *actionlist[]={action1,action2,action3,action4,action5};
    int valid=0;
    int action=0;
    while (valid==0){
        printf("====================================\n");
        printf("Choose you action:\n");
        printf("%s\n%s\n%s\n%s\n%s\n",action1,action2,action3,action4,action5);
        //next time add a quit action here
        printf("====================================\n");
        printf("Your action:");
        scanf("%[^\n]",userInput);
        while((getchar()) != '\n');
        //eg: input=1) Check Ne"
        checkAction(userInput,&valid,&action);
    }
    printf("%s %d %d",userInput,valid,action);
    int tempAction=0;
    for (int i=0;i<sizeof(actionlist)/sizeof(actionlist[0]);i++){
        checkAction(actionlist[i],&valid,&tempAction);
    }
    // for (int i=0;i<sizeof(actionlist)/sizeof(actionlist[0]);i++){
    //     printf("%s",actionlist[i]);
    // }

    if (action==0){
        int validity=0;
        action=1;
        while (validity<5){
            if (action<=5){
                for (int i=0; i<5; i++){
                    if (userInput[i]==actionlist[i][i]){
                        validity++;
                    }
                }
                if (validity<5){
                    action++;
                }
            } 
        }
    }
    if (action!=0){
        if (action==1){
            create();
        } else if (action==2){
            delete();
        } else if (action==3){
            deposit();
        } else if (action==4){
            withdrawal();
        } else if (action==5){
            remittance();
        }
    }
}
int main(){
    menu();

    return 0;
}