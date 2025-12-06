#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <direct.h>
#include <math.h>
#include <time.h>

struct account{
    char name[100];
    char idNo[14];
    char accType[100];
    char pin[5];
    int bankAccNo;
    float balance;
};

// Update transaction log
void writetolog(int action, struct account acc, int amount, int checkforrem){
    char *activitylist[]={"create","delete","deposit","withdrawal","remittance"};
    char *activity=activitylist[action-1];
    float amountindec=amount;
    time_t currentTime;
    time(&currentTime); 
    if (checkforrem==1){
        amountindec=amountindec*1.02;
    }
    FILE *fptr;
    fptr=fopen("database\\transaction.log","a+");
    char logtemp[1000];
    // Put all info to be saved in a single variable
    sprintf(logtemp,"Time: %sActivity: %s\nBank account: %d\nAmount involved:%.2f\n\n",ctime(&currentTime),activity,acc.bankAccNo,amountindec);
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
        // Eliminate not numbers and alphabets
        if (!isalnum(*ogInput)){
            ogInput++;
            continue;
        }
        // To lower case
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
    // check if the input's first character is a signle digit number but is not 1~6
    for(i=0; inputNoSp[i]!='\0'; i++){
        if (isdigit(inputNoSp[i])){
            if (a==0){
                *action=inputNoSp[i]- '0';
                a++;
                if (*action>6 || *action<1){
                    *valid=0;
                }
            } else {
                // printf("Error: Please enter index in the list only.\n");
                *valid=0;
            } 
        }
    }

    strcpy(userInput,inputNoSp);
    // printf("%s %d\n\n",inputNoSp,*action);
}

int checkInputSimilarity(char *input, char *correctInput){
    // printf("%s %s",input,correctInput);
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
            *validPtr=0;
            break;
        }
    }
    // if (*validPtr==0){
    //     printf("Invalid input.\n");
    // }
}

int checkDigit(char *input, int length){
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

void create(int *action){
    // printf("This is create");
    printf("====================================\n");
    printf("Create account\n");
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
    int validity=1;
    do{
        printf("Enter your identity number: \n");
        scanf("%13[^\n]",acc.idNo);
        while((getchar()) != '\n');
        validity=checkDigit(acc.idNo,12);
        if (validity==0){
            printf("Please enter numbers only. Eg: 012345678901");
        }
    }while (validity==0);
    while (createValid==0){
        printf("Create Savings or Current account? (1.Savings/2.Current): \n");
        scanf("%[^\n]",accTypeTemp);
        while((getchar()) != '\n');
        checkAction(accTypeTemp,&createValid,&typeListIndex);
        if (typeListIndex==0){
            int typelistSize=sizeof(accTypeList)/sizeof(accTypeList[0]);
            checkAction1(accTypeTemp,&createValid,&typeListIndex,accTypeList,typelistSize);
        } 
        if (!createValid){
            printf("Invalid input. Please enter index or the actual word only.");
        }
    }
    strcpy(acc.accType,accTypeList[typeListIndex-1]);
    // printf("%s %d",acc.accType,typeListIndex);
    validity=0;
    do{
        printf("Enter your 4 digit PIN:");
        scanf("%s",acc.pin);
        while((getchar())!='\n');
        validity=checkDigit(acc.pin,4);
        if (validity==0){
            printf("Invalid input. Please enter 4 numbers only.");
        }
    } while (validity==0);

    int max=999999999;
    int min=1000000;
    int bankAccNo = rand() % (max-min +1) +min;
    // printf("%d\n",bankAccNo);
    int valid=0;
    char accno[100];
    while (valid==0){
        FILE* fptr;
        fptr=fopen("database\\index.txt","r");
        char c[10];
        // int i=0;
        do{
            sprintf(accno,"%d",bankAccNo);
            fgets(c,10,fptr);
            c[strcspn(c, "\n")] = '\0';
            // printf("%s %s\n",c,accno);
            if (checkInputSimilarity(accno,c)){
                bankAccNo = rand() % (max-min +1) +min; 
                // printf("new no: %d",bankAccNo);
                valid=0;
                break;
            }
            if (feof(fptr)){
                valid=1;
                break ;
            }
        }  while(1);
        fclose(fptr);
    }
    // printf("%d ", bankAccNo);
    
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
    // printf("%s",acc.idNo);
    // printf("%s",temp);
    fputs(temp, fptr);
    fclose(fptr);

    fptr=fopen("database\\index.txt","a");
    fprintf(fptr, "%d\n", bankAccNo);
    fclose(fptr);

    printf("====================================\n");
    printf("Account created successfully!\n");
    printf("Bank account number: %d\n",acc.bankAccNo);
    printf("Name: %s\n",acc.name);
    printf("Identity number: %s",acc.idNo);
    printf("Bank account number: %d\n",acc.bankAccNo);
    printf("Account type: %s\n",acc.accType);
    printf("Current balance: %.2f\n",acc.balance);

    writetolog(*action,acc,0,0);
}

int checkYesNo(char *input){
    char *list[]={"yes","no"};
    int listIndex=0;
    int yesNoValid=0;
    checkAction(input,&yesNoValid,&listIndex);
    if (yesNoValid==0){
        printf("Invalid input. Please enter the index or keywords only.\n");
        listIndex=0;
        return listIndex;
    }
    if (listIndex==0){
        int listSize=sizeof(list)/sizeof(list[0]);
        checkAction1(input,&yesNoValid,&listIndex,list,listSize);
    } 
    // printf("%d",listIndex);
    return listIndex;
}


void delete(int *action){
    struct account acc;
    // printf("This is delete");
    printf("====================================\n");
    printf("Delete account\n");
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
                validity=checkDigit(accChosen,i);
            }
        }
        if (validity==0){
            printf("Invalid input. Please enter your full bank account number instead of index.\n");
            continue;
        }
        char pathtemp[30];
        FILE *fptr;
        sprintf(pathtemp,"database\\%s.txt",accChosen);
        fptr=fopen(pathtemp,"r");
        if (fptr==NULL){
            printf("Invalid input. Please enter your valid bank account number.\n");
            validity=0;
        }
    }while(validity==0);
    fclose(fptr);
    fptr=fopen("database\\index.txt","r");
    int a=1;
    do
    {
        fgets(c,10,fptr);
        c[strcspn(c, "\n")] = '\0';
        if (checkInputSimilarity(accChosen,c)){
            validity=1;
            break;
        }
        a=a+1;
    }  while(1);
    fclose(fptr);
    // printf("%s",c);
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
        printf("Returning to menu..");
        return;
    }

    char path[30];
    sprintf(path, "database\\%s.txt", c);
    // printf("%s",path);
    fptr=fopen(path,"r");
    if (!fptr){
        printf("error");
    }
    fscanf(fptr,"%s\n%s\n%s\n%s\n%d\n%f",acc.name,acc.idNo,acc.accType,acc.pin,&acc.bankAccNo,&acc.balance);
    // printf("%s",acc.idNo);
    char correctIdInput[5];
    sprintf(correctIdInput, "%s", acc.idNo + strlen(acc.idNo) - 4);
    printf("%s",correctIdInput);
    fclose(fptr);

    char idEnteredtemp[6];
    validity=1;
    do{
        printf("Enter the last 4 numbers of your ID of this account: ");
        scanf("%5s",idEnteredtemp);
        while((getchar()) != '\n'); 
        validity=checkInputSimilarity(idEnteredtemp,correctIdInput);
        if (validity==0){
            printf("ID entered incorrect.\n");
        }
    } while (validity==0);

    // printf("%s",idEnteredtemp);

    char pinEntered[6];
    do{
        printf("Enter your 4-digit PIN: ");
        scanf("%5s",pinEntered);
        while((getchar()) != '\n'); 
        validity=checkInputSimilarity(pinEntered,acc.pin);
        if (!validity){
            printf("Invalid PIN enetered.");
        }
    } while (validity==0);

    remove(path);

    fptr=fopen("database\\index.txt","r");
    FILE *fptrtemp;
    fptrtemp=fopen("database\\indextemp.txt","w");
    fclose(fptrtemp);
    fptrtemp=fopen("database\\indextemp.txt","a+");
    char prevaccno[10];
    do
    {
        fgets(c,10,fptr);
        if (feof(fptr)){
            break ;
        }
        c[strcspn(c, "\n")] = '\0';
        if (checkInputSimilarity(accChosen,c)){
            continue;
        }
        if (checkInputSimilarity(c,prevaccno)){
            continue;
        }
        char validaccno[10];
        sprintf(validaccno,"%s\n",c);
        fputs(validaccno,fptrtemp);
        sprintf(prevaccno,"%s",c);
    }  while(1);
    fclose(fptr);
    fclose(fptrtemp);
    remove("database\\index.txt");
    rename("database\\indextemp.txt","database\\index.txt");

    printf("Account deleted successfully!\n");
    writetolog(*action,acc,0,0);
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
    // printf("%s",path);
    if(remove(path)){
        printf("Account updated successfully!\n",path);
    }else{
        printf("Error deleting temp file.");
    }
    // remove(path);
    rename("database\\temp.txt",path);
}

int checkDec(char *amount, int validation){
    validation=1;
    int dotCount=0;
    int decimals=0;
    for (int i=0; amount[i]!='\0'; i++){
        if (amount[i]=='.'){
            dotCount++;
            if (dotCount>1){
                validation = 0;
            }
        } else if (!isdigit(amount[i])){
            validation=0;   
        } else if (dotCount==1){
            decimals++;  
        }
    }

    if (!validation || decimals>2) {
        validation=0;
    }
    return validation;
}

void deposit(int *action){
    struct account acc;
    // printf("This is deposit");
    printf("====================================\n");
    printf("Deposit\n");
    int validation=1;
    char bankAccEntered[10];
    do{
        printf("Enter your bank account number: \n");
        scanf("%s",bankAccEntered);
        while((getchar()) != '\n'); 
        for (int i=7; i<=9; i++){
            if (validation==0){
                validation=checkDigit(bankAccEntered,i);
            }
        }
        if (validation==0){
            printf("Invalid input. Please enter your bank account number again.\n");
            continue;
        }
        char path[30];
        FILE *fptr;
        sprintf(path,"database\\%s.txt",bankAccEntered);
        fptr=fopen(path,"r");
        if (fptr==NULL){
            validation=0;
            return;
        }
        fscanf(fptr,"%s\n%s\n%s\n%s\n%d\n%f",acc.name,acc.idNo,acc.accType,acc.pin,&acc.bankAccNo,&acc.balance); 
        fclose(fptr);
        if (validation==0){
            printf("Invalid input. Please enter your bank account number again.\n");
            continue;
        }
    }while(validation==0);
    // printf("%s",bankAccEntered);

    char pinEntered[6];
    do{
        printf("Enter your 4-digit PIN: ");
        scanf("%5s",pinEntered);
        while((getchar()) != '\n'); 
        validation=checkInputSimilarity(pinEntered,acc.pin);
        if (!validation){
            printf("Incorrect PIN enetered. Try again.\n");
        }
    } while (validation==0);
    // printf("%s\n%s\n%s\n%s\n%d\n%f",acc.name,acc.idNo,acc.accType,acc.pin,acc.bankAccNo,acc.balance); 
    printf("====================================\n");
    printf("Welcome back %s!\n",acc.name);
    printf("Your current balance: %.2f\n",acc.balance);
    char depositAmountTemp[10];
    float depositAmount;
    do{
        printf("Desired deposit amount: \n");
        scanf("%s",depositAmountTemp);
        while((getchar()) != '\n'); 
        validation=checkDec(depositAmountTemp,validation);
        // printf("%.2f",depositAmount);
        if (validation){
            depositAmount=strtof(depositAmountTemp,NULL);
            if (depositAmount>0 && depositAmount<=50000){
                break;
            } else{
                validation=0;
            }
        }
        if (!validation){
            printf("Invalid input. Please enter valid numbers within 1 to 50000 with 2 decimal place only.\n");
        }
    }while (validation==0);
    acc.balance=acc.balance+depositAmount;
    // printf("%f",acc.balance);
    // printf("%.2f",acc.balance);
    rewrite(acc);
    // printf("%s\n%s\n%s\n%s\n%d\n%.2f",acc.name,acc.idNo,acc.accType,acc.pin,&acc.bankAccNo,&acc.balance); 
    printf("Deposit successful!\nCurrent balance: %.2f\nReturning to menu..",acc.balance);
    writetolog(*action,acc,depositAmount,0);
}

void withdrawal(int *action){
    printf("====================================\n");
    printf("Withdrawal");
    // printf("This is withdrawal");
    struct account acc;
    int validation=1;
    char bankAccEntered[10];
    do{
        printf("Enter your bank account number: \n");
        scanf("%s",bankAccEntered);
        while((getchar()) != '\n'); 
        for (int i=7; i<=9; i++){
            if (validation==0){
                validation=checkDigit(bankAccEntered,i);
            }
        }
        if (!validation){
            printf("Invalid input. Please enter your bank account number again.");
            continue;
        }
        char path[30];
        FILE *fptr;
        sprintf(path,"database\\%s.txt",bankAccEntered);
        fptr=fopen(path,"r");
        if (fptr==NULL){
            validation=0;
            return;
        }
        fscanf(fptr,"%s\n%s\n%s\n%s\n%d\n%f",acc.name,acc.idNo,acc.accType,acc.pin,&acc.bankAccNo,&acc.balance); 
        fclose(fptr);
        if(!validation){
            printf("Invalid input. Please enter your bank account number again.");
        }
    }while(validation==0);
    // printf("%s",bankAccEntered);

    char pinEntered[6];
    do{
        printf("Enter your 4-digit PIN: ");
        scanf("%5s",pinEntered);
        while((getchar()) != '\n'); 
        validation=checkInputSimilarity(pinEntered,acc.pin);
        if (!validation){
            printf("Incorrect PIN entered.");
        }
    } while (validation==0);
    // printf("%s\n%s\n%s\n%s\n%d\n%.2f",acc.name,acc.idNo,acc.accType,acc.pin,&acc.bankAccNo,&acc.balance); 
    printf("====================================\n");
    printf("Welcome back %s!\n",acc.name); 
    printf("Your current balance: %.2f\n",acc.balance);
    char withdrawalAmountTemp[10];
    float withdrawalAmount;
    do{
        printf("Desired withdrawal amount: \n");
        scanf("%s",withdrawalAmountTemp);
        while((getchar()) != '\n');   
        // validation=checkDigit(depositAmountTemp,strlen(depositAmountTemp));
        validation=checkDec(withdrawalAmountTemp,validation);
        // printf("%.2f",withdrawalAmount);
        if (validation){
            withdrawalAmount=strtof(withdrawalAmountTemp,NULL);
            if (withdrawalAmount>0 && withdrawalAmount<=acc.balance){
                validation=1;
            } else{
                validation=0;
            }
        }
        if (!validation){
            printf("Invalid input. Please enter valid numbers within 1 to 50000 with 2 decimal place only.");
        }
    }while (validation==0);
    acc.balance=acc.balance-withdrawalAmount;
    // printf("%.2f",acc.balance);
    rewrite(acc);
    // printf("%s\n%s\n%s\n%s\n%d\n%.2f",acc.name,acc.idNo,acc.accType,acc.pin,&acc.bankAccNo,&acc.balance); 
    printf("====================================\n");
    printf("Withdrawal successful!\nCurrent balance: %.2f\nReturning to menu..",acc.balance);
    writetolog(*action,acc,withdrawalAmount,0);
}

void remittance(int *action){
    // printf("This is remmittance");
    printf("====================================\n");
    printf("Remittance");
    struct account senderAcc;
    struct account receiverAcc;
    int validation=0;
    char senderBankAcc[10];
    char path[30];
    FILE *fptr;
    do{
        printf("Enter your bank account number: \n");
        scanf("%s",senderBankAcc);
        while((getchar()) != '\n'); 
        for (int i=7; i<=9; i++){
            if (validation==0){
                validation=checkDigit(senderBankAcc,i);
            }
        }
        if (!validation){
            printf("Invalid input. Please enter your bank account number again.");
            continue;
        }
        sprintf(path,"database\\%s.txt",senderBankAcc);
        fptr=fopen(path,"r");
        if (fptr==NULL){
            printf("Invalid input.");
            return;
        }
        fscanf(fptr,"%s\n%s\n%s\n%s\n%d\n%f",senderAcc.name,senderAcc.idNo,senderAcc.accType,senderAcc.pin,&senderAcc.bankAccNo,&senderAcc.balance); 
        fclose(fptr);
        if (!validation){
            printf("Invalid input. Please enter your bank account number again.");
        }
    }while(validation==0);
    // printf("%s",senderBankAcc);

    char pinEntered[6];
    do{
        printf("Enter your 4-digit PIN: ");
        scanf("%5s",pinEntered);
        while((getchar()) != '\n'); 
        validation=checkInputSimilarity(pinEntered,senderAcc.pin);
        if (!validation){
            printf("Incorrect PIN entered. Please try again.");
        }
    } while (validation==0);
    // printf("%s\n%s\n%s\n%s\n%d\n%.2f",acc.name,acc.idNo,acc.accType,acc.pin,&acc.bankAccNo,&acc.balance); 
    printf("Welcome back %s!",senderAcc.name);
    printf("Your current balance: %.2f\n",senderAcc.balance);
    char transferAmountTemp[10];
    int transferAmount;
    do{
        printf("Amount to be transferred: \n");
        scanf("%s",transferAmountTemp);
        while((getchar()) != '\n');   
        validation=checkDec(transferAmountTemp,validation);
        if (validation){
            transferAmount=strtof(transferAmountTemp,NULL);
            if (transferAmount>0 && transferAmount<=senderAcc.balance){
                validation=1;
            } else{
                printf("Please dont sohai.Invalid input. Please enter valid numbers within 1 to 50000 with 2 decimal place only.\n");
                validation=0;
            }
        }
    }while (validation==0);

    char receiverBankAcc[10];
    do{
        printf("Enter receiver bank account number: \n");
        scanf("%s",receiverBankAcc);
        while((getchar()) != '\n'); 
        for (int i=7; i<=9; i++){
            if (validation==0){
                validation=checkDigit(receiverBankAcc,i);
            }
        }
        sprintf(path,"database\\%s.txt",receiverBankAcc);
        fptr=fopen(path,"r");
        if (fptr==NULL){
            printf("Receiver doesn't exist. Please try again.\n");
            validation=0;
        }
        fclose(fptr);
        if (checkInputSimilarity(senderBankAcc,receiverBankAcc)){
            validation=0;
            printf("Invalid input. Sender and receiver are the same.\n");
        }
    }while(validation==0);
    // printf("%s",receiverBankAcc);


    float remittancefee;
    if(!checkInputSimilarity(senderAcc.accType,receiverAcc.accType)){
        if (checkInputSimilarity(senderAcc.accType,"savings")){
            remittancefee=transferAmount*0.02;
        } else {
            remittancefee=transferAmount*0.03;
        }
        // return;
    }
    remittancefee=round(remittancefee);

    senderAcc.balance=senderAcc.balance-transferAmount-remittancefee;
    rewrite(senderAcc);
    writetolog(*action,senderAcc,transferAmount,1);

    sprintf(path,"database\\%s.txt",receiverBankAcc);
    fptr=fopen(path,"r");
    fscanf(fptr,"%s\n%s\n%s\n%s\n%d\n%f",receiverAcc.name,receiverAcc.idNo,receiverAcc.accType,receiverAcc.pin,&receiverAcc.bankAccNo,&receiverAcc.balance); 
    fclose(fptr);
    receiverAcc.balance=receiverAcc.balance+transferAmount;
    rewrite(receiverAcc);

    printf("====================================\n");
    printf("Transaction successful!\n");
    printf("Sender: %s\n",senderAcc.name);
    printf("Bank account: %d\n",senderAcc.bankAccNo);
    printf("Receiver: %s\n",receiverAcc.name);
    printf("Bank account: %d\n",receiverAcc.bankAccNo);
    printf("Amount transfered: %.2f\n",transferAmount);
    printf("Remittance fee: %.2f\n",remittancefee);

    writetolog(*action,receiverAcc,transferAmount,0);
}

void menu(){
    char userInput[100];
    char action1[]="createnewbankaccount";
    char action2[]="deletebankaccount";
    char action3[]="deposit";
    char action4[]="withdrawal";
    char action5[]="remittance";
    char action6[]="quit";
    char *actionlist[]={action1,action2,action3,action4,action5,action6};
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
        if (checkInputSimilarity(userInput,"de") || checkInputSimilarity(userInput,"d")){
            valid=0;
            printf("Invalid input. 'de' and 'd' may lead to delete or deposit.\n");
            continue;
        }
        if (action==0){
            int actionlistSize=sizeof(actionlist)/sizeof(actionlist[0]);
            checkAction1(userInput,&valid,&action,actionlist, actionlistSize);
        }
        if (valid==0){
            printf("Invalid input. Please enter index (within 1 to 6) or keywords only.\n");
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
    while(1){
        menu();
    }
    return 0;
}
