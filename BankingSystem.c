#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <direct.h>

struct account{
    char name[100];
    char idNo[13];
    char accType[100];
    char pin[5];
    int bankAccNo;
    float balance;
};
void checkAction(char *userInput, int *valid, int *action){
    *valid=1;
    *action=0; 
    char inputNoSp[100];
    inputNoSp[0]='\0';
    char *inputNoSp_ptr=inputNoSp;
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
        *inputNoSp_ptr++ = tolower(*ogInput++);
        // Copy the character one by one
    }
    *inputNoSp_ptr='\0';
    // printf("%s\n",inputNoSp);

    int a=0;
    int i=0;
    for(i=0; inputNoSp[i]!='\0'; i++){
        if (isdigit(inputNoSp[i])){
            if (a==0){
                *action=inputNoSp[i]- '0';
                a++;
            } else {
                printf("Error: Please enter number 1~6 only.\n");
                *valid=0;
            } 
        }
    }
    if (isdigit(inputNoSp[0]) && i>=2){
        // if (i>=2){
        //     inputNoSp[i-1]='\0';
        // }
        if (inputNoSp[0]>'5' || inputNoSp[0]<'1'){
            *valid=0;
        }
    }
    // if (userInput[0] == '\n') {
    //     *validPtr = 0;
    //     printf("Invalid input: cannot be empty.\n");
    //     return;
    // }
    // if (userInput[0]=='\0'){
    //     *validPtr=0;
    //     return;
    // }
    strcpy(userInput,inputNoSp);
    // printf("%s %d\n\n",userInput,*action);
    //add check for user input validity and action here
    // *validPtr=1;

    
}

void checkAction1(char *userInput, int *valid, int *action, char *actionlist[],int listSize){
    printf("Im in\n");
    int *validPtr=valid;
    char *userInputPtr=userInput;
    // printf("%s",userInputPtr);
    int listIndex=0;
    // int listSize=sizeof(actionlist)/sizeof(actionlist[0]);
    for(int i=0; userInputPtr[i]!='\0';i++){
        if (actionlist[listIndex][i]=='\0'){
            break;
        }
        if(userInputPtr[i]==actionlist[listIndex][i]){
            *action=listIndex+1;
            *validPtr=1;
        } else {
            *validPtr=0;
            i=0;
            listIndex++;
        }
        if (listIndex==listSize){
            break;
        }
    }
}

int checkInput(char *input, int length){
    if (strlen(input)!=length){
        return 0;
    }
    for (int i=0; i<length;i++){
        if (!isdigit(input[i])){
            return 0;
        }
    }
    return 1;
}

int checkInput1(char *input, char *correctInput){
    printf("%s %s",input,correctInput);
    if (strlen(input)!=strlen(correctInput)){
        return 0;
    }
    for (int i=0; i<strlen(input); i++){
        if (input[i]!=correctInput[i]){
            return 0;
        }
    }
    return 1;
}

void create(){
    printf("This is create");
    struct account acc;
    char accTypeTemp[100];
    char type1[]="savings";
    char type2[]="current";
    char *accTypeList[]={type1,type2};
    int createValid=0;
    int typeListIndex=0;
    printf("Enter your name: \n");
    scanf("%[^\n]",acc.name);
    while((getchar()) != '\n');
    int validity=0;
    do{
        printf("Enter your identification number: \n");
        scanf("%12[^\n]",acc.idNo);
        while((getchar()) != '\n');
        validity=checkInput(acc.idNo,12);
    }while (validity==0);
    while (createValid==0){
        printf("Create Savings or Current account? (1.Savings/2.Cuurent): \n");
        scanf("%[^\n]",accTypeTemp);
        while((getchar()) != '\n');
        checkAction(accTypeTemp,&createValid,&typeListIndex);
        if (typeListIndex==0){
            int typelistSize=sizeof(accTypeList)/sizeof(accTypeList[0]);
            checkAction1(accTypeTemp,&createValid,&typeListIndex,accTypeList,typelistSize);
        } 
    }
    strcpy(acc.accType,accTypeList[typeListIndex-1]);
    printf("%s %d",acc.accType,typeListIndex);
    validity=0;
    do{
        printf("Enter your 4 digit PIN:");
        scanf("%s",acc.pin);
        while((getchar())!='\n');
        validity=checkInput(acc.pin,4);
    } while (validity==0);

    int max=999999999;
    int min=1000000;
    int bankAccNo = rand() % (max-min +1) +min;
    printf("%d\n",bankAccNo);
    int valid=0;
    char accno[100];
    while (valid==0){
        FILE* fptr;
        sprintf(accno, "database\\%d.txt", bankAccNo);
        printf("\n%s\n",accno);
        fptr=fopen(accno,"r");
        if (fptr==NULL){
            valid=1;
        } else {
            fclose(fptr);
            bankAccNo = rand() % (max-min +1) +min; 
        }
    }
    printf("%d ", bankAccNo);
    
    acc.bankAccNo=bankAccNo;

    acc.balance=0.00;
    FILE* fptr;
    fptr=fopen(accno,"w");
    if (!fptr){
        printf("Error openinf sia");
    }
    
    char temp[1000];
    sprintf(temp,"%s\n%s\n%s\n%s\n%d\n%.2f",acc.name,acc.idNo,acc.accType,acc.pin,acc.bankAccNo,acc.balance);
    printf("%s",acc.idNo);
    printf("%s",temp);
    fputs(temp, fptr);
    fclose(fptr);

    fptr=fopen("database\\index.txt","a");
    fprintf(fptr, "%d\n", bankAccNo);
    fclose(fptr);

    printf("Account created successfully!\n");

}

int checkYesNo(char *input){
    char *list[]={"yes","no"};
    int listIndex=0;
    int yesNoValid=0;
    checkAction(input,&yesNoValid,&listIndex);
    if (listIndex==0){
        int listSize=sizeof(list)/sizeof(list[0]);
        checkAction1(input,&yesNoValid,&listIndex,list,listSize);
    } 
    // printf("%d",listIndex);
    return listIndex;
}


void delete(){
    struct account acc;
    printf("This is delete");
    printf("====================================\n");
    printf("List of all accounts:\n");
    char temp[1000];
    FILE *fptr;
    fptr=fopen("database\\index.txt","r");
    char c[10];
    int i=0;
    do
    {
        fgets(c,10,fptr);

        // Checking for end of file
        if (feof(fptr))
            break ;

        printf("%d. %s", i+1, c);

        i=i+1;
    }  while(1);
    fclose(fptr);
    printf("====================================\n");
    printf("Which account you want to delete? ");
    int accChosen;
    do{
        scanf("%d",&accChosen);
        while((getchar()) != '\n');
    } while (accChosen<0 || accChosen>i);
    // printf("%d",accChosen);
    // printf("%d",i);

    fptr=fopen("database\\index.txt","r");
    int a=1;
    do
    {
        fgets(c,10,fptr);
        if (a==accChosen)
            break;
        a=a+1;
    }  while(1);
    c[strcspn(c, "\n")] = '\0';
    fclose(fptr);
    printf("%s",c);
    int choice;
    do{
        printf("Do you really want to delete account %s (1. Yes/2. No)?\n",c);
        char input[10];
        scanf("%[^\n]",&input);
        while((getchar()) != '\n');
        choice=checkYesNo(input);
    } while (choice!=1 && choice!=2);
    if (choice==1){
        printf("Yes I do.\n");
    } else {
        return;
    }

    char path[30];
    sprintf(path, "database\\%s.txt", c);
    fptr=fopen(path,"r");
    fscanf(fptr,"%s\n%s\n%s\n%s\n%d\n%.2f",acc.name,acc.idNo,acc.accType,acc.pin,&acc.bankAccNo,&acc.balance);
    // printf("%s",acc.idNo);
    char correctIdInput[5];
    sprintf(correctIdInput, "%s", acc.idNo + strlen(acc.idNo) - 4);
    printf("%s",correctIdInput);
    fclose(fptr);

    char idEnteredtemp[6];
    int validity=0;
    do{
        printf("Enter the last 4 numbers of your ID of this account: ");
        scanf("%5s",idEnteredtemp);
        while((getchar()) != '\n'); 
        validity=checkInput1(idEnteredtemp,correctIdInput);
    } while (validity==0);

    printf("%s",idEnteredtemp);

    char pinEntered[6];
    validity=0;
    do{
        printf("Enter your 4-digit PIN: ");
        scanf("%5s",pinEntered);
        while((getchar()) != '\n'); 
        validity=checkInput1(pinEntered,acc.pin);
    } while (validity==0);

    remove(path);
    // fptr=fopen("database\\index.txt","w");
    // a=0;
    // char temp[i][10];
    // do
    // {
    //     fgets(c,10,fptr);
    //     if (a==accChosen){
    //         continue;
    //     }
    // }  while(1);
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

void menu(){
    char userInput[100];
    char action1[]="1createnewbankaccount";
    char action2[]="2deletebankaccount";
    char action3[]="3deposit";
    char action4[]="4withdrawal";
    char action5[]="5remittance";
    char *actionlist[]={action1,action2,action3,action4,action5};
    int valid=0;
    int action=0;
    int tempAction=0;
    // for (int i=0;i<sizeof(actionlist)/sizeof(actionlist[0]);i++){
    //     checkAction(actionlist[i],&valid,&tempAction);
    // }
    // for (int i=0;i<sizeof(actionlist)/sizeof(actionlist[0]);i++){
    //     printf("%s",actionlist[i]);
    // }
    valid=0;
    while (valid==0){
        printf("====================================\n");
        printf("Choose you action:\n");
        printf("1. Create New Bank Account\n");
        printf("2. Delete Bank Account\n");
        printf("3.Deposit\n");
        printf("4. Withdrawal\n");
        printf("5. Remittance\n");
        //next time add a quit action here
        printf("====================================\n");
        printf("Your action:");
        scanf("%[^\n]",userInput);
        while((getchar()) != '\n');
        //eg: input=1) Check Ne"
        checkAction(userInput,&valid,&action);
        if (action==0){
            int actionlistSize=sizeof(actionlist)/sizeof(actionlist[0]);
            checkAction1(userInput,&valid,&action,actionlist, actionlistSize);
        }
    }
    // printf("%s %d %d",userInput,valid,action);
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
    // while (1){
    menu();
    // }

    return 0;
}