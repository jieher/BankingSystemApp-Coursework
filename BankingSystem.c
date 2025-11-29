#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <direct.h>
#include <time.h>

struct account{
    char name[100];
    char idNo[13];
    char accType[100];
    char pin[5];
    int bankAccNo;
    float balance;
};

void writetolog(int action, struct account acc){
    char *activitylist[]={"create","delete","deposit","withdrawal","remittance"};
    char *activity=activitylist[action-1];
    time_t currentTime;
    time(&currentTime); 
    FILE *fptr;
    fptr=fopen("database\\transaction.log","a+");
    char logtemp[1000];
    sprintf(logtemp,"Time: %sActivity: %s\nBank account: %s\n\n",ctime(&currentTime),activity,acc.bankAccNo);
    fputs(logtemp,fptr);
    fclose(fptr);
}

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
        if (isdigit(userInput[0])){
            *inputNoSp_ptr = tolower(*ogInput++);
            *inputNoSp_ptr++;
        } else {
            *inputNoSp_ptr++ = tolower(*ogInput++);
        }
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
        if (inputNoSp[0]>'5' || inputNoSp[0]<'1'){
            *valid=0;
        }
    }
    strcpy(userInput,inputNoSp);
    // printf("%s %d\n\n",inputNoSp,*action);
}

void checkAction1(char *userInput, int *valid, int *action, char *actionlist[],int listSize){
    // printf("Im in\n");
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
    if (*validPtr==0){
        printf("Invalid input.\n");
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

void create(int *action){
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
        // printf("\n%s\n",accno);
        // fptr=fopen(accno,"r");
        // FILE *fptr;
        fptr=fopen("database\\index.txt","r");
        char c[10];
        // int i=0;
        do{
            sprintf(accno,"%d",bankAccNo);
            fgets(c,10,fptr);
            c[strcspn(c, "\n")] = '\0';
            // printf("%s %s\n",c,accno);
            if (checkInput1(accno,c)){
                bankAccNo = rand() % (max-min +1) +min; 
                // printf("new no: %d",bankAccNo);
                valid=0;
                break;
            }
            if (feof(fptr)){
                valid=1;
                break ;
            }
            // printf("%d. %s", i+1, c);
            // i=i+1;
        }  while(1);
        fclose(fptr);
        // if (fptr==NULL){
        //     valid=1;
        // } else {
        //     fclose(fptr);
        //     bankAccNo = rand() % (max-min +1) +min; 
        // }
    }
    printf("%d ", bankAccNo);
    
    acc.bankAccNo=bankAccNo;

    acc.balance=0.00;
    FILE* fptr;
    sprintf(accno, "database\\%d.txt", bankAccNo);
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

    writetolog(*action,acc);
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


void delete(int *action){
    struct account acc;
    printf("This is delete");
    printf("====================================\n");
    printf("List of all accounts:\n");
    char temp[1000];
    FILE *fptr;
    fptr=fopen("database\\index.txt","r");
    char c[10];
    int i=0;
    do{
        fgets(c,10,fptr);
        if (feof(fptr))
            break ;
        printf("%d. %s", i+1, c);
        i=i+1;
    }  while(1);
    fclose(fptr);
    printf("====================================\n");
    int validity=0;
    char accChosen[10];

    do {
        printf("Which account you want to delete? ");
        scanf("%9[^\n]",&accChosen);
        while((getchar()) != '\n');
        for (int i=7; i<=9; i++){
            if (validity==0){
                validity=checkInput(accChosen,i);
            }
        }
        if (validity==0){
            printf("Dont sohai");
            continue;
        }
        char pathtemp[30];
        FILE *fptr;
        sprintf(pathtemp,"database\\%s.txt",accChosen);
        fptr=fopen(pathtemp,"r");
        if (fptr==NULL){
            printf("Invalid input.");
        }
    }while(validity==0);
    fclose(fptr);
    fptr=fopen("database\\index.txt","r");
    int a=1;
    do
    {
        fgets(c,10,fptr);
        c[strcspn(c, "\n")] = '\0';
        if (checkInput1(accChosen,c)){
            validity=1;
            break;
        }
        a=a+1;
    }  while(1);
    fclose(fptr);
    printf("%s",c);
    int choice;
    do{
        printf("Do you really want to delete account %s (1. Yes/2. No)?\n",c);
        char input[10];
        scanf("%9[^\n]",input);
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
    fscanf(fptr,"%s\n%s\n%s\n%s\n%d\n%f",acc.name,acc.idNo,acc.accType,acc.pin,acc.bankAccNo,&acc.balance);
    // printf("%s",acc.idNo);
    char correctIdInput[5];
    sprintf(correctIdInput, "%s", acc.idNo + strlen(acc.idNo) - 4);
    // printf("%s",correctIdInput);
    fclose(fptr);

    char idEnteredtemp[6];
    validity=0;
    do{
        printf("Enter the last 4 numbers of your ID of this account: ");
        scanf("%5s",idEnteredtemp);
        while((getchar()) != '\n'); 
        validity=checkInput1(idEnteredtemp,correctIdInput);
    } while (validity==0);

    // printf("%s",idEnteredtemp);

    char pinEntered[6];
    validity=0;
    do{
        printf("Enter your 4-digit PIN: ");
        scanf("%5s",pinEntered);
        while((getchar()) != '\n'); 
        validity=checkInput1(pinEntered,acc.pin);
    } while (validity==0);

    remove(path);

    writetolog(*action,acc);
}

void rewrite(struct account acc){
    char path[30];
    char temp[1000];
    FILE *fptr;
    fptr=fopen("database\\temp.txt","w");
    sprintf(temp,"%s\n%s\n%s\n%s\n%d\n%.2f",acc.name,acc.idNo,acc.accType,acc.pin,acc.bankAccNo,acc.balance);
    sprintf(path, "database\\%d.txt", acc.bankAccNo);
    fputs(temp, fptr);
    fclose(fptr);
    printf("%s",path);
    if(remove(path)){
        printf("%s deleted.",path);
    }
    // remove(path);
    rename("database\\temp.txt",path);
}

void deposit(int *action){
    struct account acc;
    printf("This is deposit");
    int validation=0;
    char bankAccEntered[10];
    do{
        printf("Enter your bank account number: \n");
        scanf("%s",bankAccEntered);
        while((getchar()) != '\n'); 
        for (int i=7; i<=9; i++){
            if (validation==0){
                validation=checkInput(bankAccEntered,i);
            }
        }
    }while(validation==0);
    printf("%s",bankAccEntered);

    char path[30];
    FILE *fptr;
    sprintf(path,"database\\%s.txt",bankAccEntered);
    fptr=fopen(path,"r");
    if (fptr==NULL){
        printf("Invalid input.");
        return;
    }
    fscanf(fptr,"%s\n%s\n%s\n%s\n%d\n%f",acc.name,acc.idNo,acc.accType,acc.pin,&acc.bankAccNo,&acc.balance); 
    fclose(fptr);
    char pinEntered[6];
    do{
        printf("Enter your 4-digit PIN: ");
        scanf("%5s",pinEntered);
        while((getchar()) != '\n'); 
        validation=checkInput1(pinEntered,acc.pin);
    } while (validation==0);
    // printf("%s\n%s\n%s\n%s\n%d\n%.2f",acc.name,acc.idNo,acc.accType,acc.pin,&acc.bankAccNo,&acc.balance); 
    printf("Your current balance: %.2f\n",acc.balance);
    char depositAmountTemp[10];
    int depositAmount;
    do{
        printf("Desired deposit amount: \n");
        scanf("%s",depositAmountTemp);
        while((getchar()) != '\n');   
        validation=checkInput(depositAmountTemp,strlen(depositAmountTemp));
        if (validation){
            depositAmount=atoi(depositAmountTemp);
            if (depositAmount>0 && depositAmount<=50000){
                break;
            } else{
                printf("Please enter 0~50000 only.\n");
            }
        }
    }while (validation==0);
    acc.balance=acc.balance+depositAmount;
    // printf("%.2f",acc.balance);
    rewrite(acc);
    // printf("%s\n%s\n%s\n%s\n%d\n%.2f",acc.name,acc.idNo,acc.accType,acc.pin,&acc.bankAccNo,&acc.balance); 
    writetolog(*action,acc);
}

void withdrawal(int *action){
    printf("This is withdrawal");
    struct account acc;
    int validation=0;
    char bankAccEntered[10];
    do{
        printf("Enter your bank account number: \n");
        scanf("%s",bankAccEntered);
        while((getchar()) != '\n'); 
        for (int i=7; i<=9; i++){
            if (validation==0){
                validation=checkInput(bankAccEntered,i);
            }
        }
    }while(validation==0);
    printf("%s",bankAccEntered);

    char path[30];
    FILE *fptr;
    sprintf(path,"database\\%s.txt",bankAccEntered);
    fptr=fopen(path,"r");
    if (fptr==NULL){
        printf("Invalid input.");
        return;
    }
    fscanf(fptr,"%s\n%s\n%s\n%s\n%d\n%f",acc.name,acc.idNo,acc.accType,acc.pin,&acc.bankAccNo,&acc.balance); 
    fclose(fptr);
    char pinEntered[6];
    do{
        printf("Enter your 4-digit PIN: ");
        scanf("%5s",pinEntered);
        while((getchar()) != '\n'); 
        validation=checkInput1(pinEntered,acc.pin);
    } while (validation==0);
    // printf("%s\n%s\n%s\n%s\n%d\n%.2f",acc.name,acc.idNo,acc.accType,acc.pin,&acc.bankAccNo,&acc.balance); 
    printf("Your current balance: %.2f\n",acc.balance);
    char withdrawalAmountTemp[10];
    int withdrawalAmount;
    do{
        printf("Desired withdrawal amount: \n");
        scanf("%s",withdrawalAmountTemp);
        while((getchar()) != '\n');   
        validation=checkInput(withdrawalAmountTemp,strlen(withdrawalAmountTemp));
        if (validation){
            withdrawalAmount=atoi(withdrawalAmountTemp);
            if (withdrawalAmount>0 && withdrawalAmount<=acc.balance){
                break;
            } else{
                printf("Please dont sohai.\n");
            }
        }
    }while (validation==0);
    acc.balance=acc.balance-withdrawalAmount;
    // printf("%.2f",acc.balance);
    rewrite(acc);
    // printf("%s\n%s\n%s\n%s\n%d\n%.2f",acc.name,acc.idNo,acc.accType,acc.pin,&acc.bankAccNo,&acc.balance); 
    writetolog(*action,acc);
}

void remittance(int *action){
    printf("This is remmittance");
    struct account senderAcc;
    struct account receiverAcc;
    int validation=0;
    char senderBankAcc[10];
    do{
        printf("Enter your bank account number: \n");
        scanf("%s",senderBankAcc);
        while((getchar()) != '\n'); 
        for (int i=7; i<=9; i++){
            if (validation==0){
                validation=checkInput(senderBankAcc,i);
            }
        }
    }while(validation==0);
    printf("%s",senderBankAcc);

    char path[30];
    FILE *fptr;
    sprintf(path,"database\\%s.txt",senderBankAcc);
    fptr=fopen(path,"r");
    if (fptr==NULL){
        printf("Invalid input.");
        return;
    }
    fscanf(fptr,"%s\n%s\n%s\n%s\n%d\n%f",senderAcc.name,senderAcc.idNo,senderAcc.accType,senderAcc.pin,&senderAcc.bankAccNo,&senderAcc.balance); 
    fclose(fptr);
    char pinEntered[6];
    do{
        printf("Enter your 4-digit PIN: ");
        scanf("%5s",pinEntered);
        while((getchar()) != '\n'); 
        validation=checkInput1(pinEntered,senderAcc.pin);
    } while (validation==0);
    // printf("%s\n%s\n%s\n%s\n%d\n%.2f",acc.name,acc.idNo,acc.accType,acc.pin,&acc.bankAccNo,&acc.balance); 
    printf("Your current balance: %.2f\n",senderAcc.balance);
    char transferAmountTemp[10];
    int transferAmount;
    do{
        printf("Amount to be transferred: \n");
        scanf("%s",transferAmountTemp);
        while((getchar()) != '\n');   
        validation=checkInput(transferAmountTemp,strlen(transferAmountTemp));
        if (validation){
            transferAmount=atoi(transferAmountTemp);
            if (transferAmount>0 && transferAmount<=senderAcc.balance){
                break;
            } else{
                printf("Please dont sohai.\n");
            }
        }
    }while (validation==0);
    // acc.balance=acc.balance-transferAmount;
    // printf("%.2f",acc.balance);
    // rewrite(acc);

    char receiverBankAcc[10];
    do{
        printf("Enter receiver bank account number: \n");
        scanf("%s",receiverBankAcc);
        while((getchar()) != '\n'); 
        for (int i=7; i<=9; i++){
            if (validation==0){
                validation=checkInput(receiverBankAcc,i);
            }
        }
    }while(validation==0);
    // printf("%s",receiverBankAcc);

    sprintf(path,"database\\%s.txt",receiverBankAcc);
    fptr=fopen(path,"r");
    if (fptr==NULL){
        printf("Receiver doesn't exist.");
        return;
    }
    fclose(fptr);
    senderAcc.balance=senderAcc.balance-transferAmount;
    rewrite(senderAcc);
    writetolog(*action,senderAcc);

    sprintf(path,"database\\%s.txt",receiverBankAcc);
    fptr=fopen(path,"r");
    fscanf(fptr,"%s\n%s\n%s\n%s\n%d\n%f",receiverAcc.name,receiverAcc.idNo,receiverAcc.accType,receiverAcc.pin,&receiverAcc.bankAccNo,&receiverAcc.balance); 
    fclose(fptr);
    receiverAcc.balance=receiverAcc.balance+transferAmount;
    rewrite(receiverAcc);
    writetolog(*action,receiverAcc);
}

void menu(){
    char userInput[100];
    char action1[]="createnewbankaccount";
    char action2[]="deletebankaccount";
    char action3[]="deposit";
    char action4[]="withdrawal";
    char action5[]="remittance";
    char action6[]="quit";
    char *actionlist[]={action1,action2,action3,action4,action5};
    int valid=0;
    int action=0;
    int tempAction=0;
    valid=0;
    printf("====================================\n");
    printf("Choose you action:\n");
    printf("1. Create New Bank Account\n");
    printf("2. Delete Bank Account\n");
    printf("3. Deposit\n");
    printf("4. Withdrawal\n");
    printf("5. Remittance\n");
    printf("6. Quit\n");
    printf("====================================\n");
    while (valid==0){
        printf("Your action:");
        scanf("%[^\n]",userInput);
        while((getchar()) != '\n');
        checkAction(userInput,&valid,&action);
        if (checkInput1(userInput,"de") || checkInput1(userInput,"d")){
            valid=0;
            printf("Invalid input. 'de' and 'd' may lead to delete or deposit");
            continue;
        }
        if (action==0){
            int actionlistSize=sizeof(actionlist)/sizeof(actionlist[0]);
            checkAction1(userInput,&valid,&action,actionlist, actionlistSize);
        }
    }
    // printf("%s %d %d",userInput,valid,action);
    if (action!=0){
        if (action==1){
            create(&action);
        } else if (action==2){
            delete(&action);
        } else if (action==3){
            deposit(&action);
        } else if (action==4){
            withdrawal(&action);
        } else if (action==5){
            remittance(&action);
        } else if (action==6){
            exit(0);
        }
    }
}
int main(){
    // while(1){
        menu();
    // }
    return 0;
}