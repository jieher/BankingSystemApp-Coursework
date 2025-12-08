#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
// #include <direct.h>
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

int checkInputSimilarity(char *input, char *correctInput){
    // If the input and correct string has different length, validity will be false
    // printf("%s %s",input,correctInput);
    if (strlen(input)!=strlen(correctInput)){
        return 0;
    }
    //check alphabet one by one 
    for (int i=0; i<strlen(input); i++){
        if (input[i]!=correctInput[i]){
            return 0;
        }
    }
    return 1;
}

// Update transaction log
void writetolog(int action, struct account acc, float amount, int checkforrem){
    char *activitylist[]={"create","delete","deposit","withdrawal","remittance"};
    char *activity=activitylist[action-1];
    // float amountindec=amount;
    // Get current time
    time_t currentTime;
    time(&currentTime); 
    // If its remittance snd is sender, count for total amount deducted.
    if (checkforrem==1){
        if (checkInputSimilarity(acc.accType,"savings")){
            amount=amount*1.02;
        } else {
            amount=amount*1.03;
        }
    }
    // Update log file
    FILE *fptr;
    fptr=fopen("database\\transaction.log","a+");
    char logtemp[1000];
    // Put all info to be saved in a single variable
    sprintf(logtemp,"Time: %sActivity: %s\nBank account: %d\nAmount involved: RM%.2f\n\n",ctime(&currentTime),activity,acc.bankAccNo,amount);
    fputs(logtemp,fptr);
    fclose(fptr);
}

void checkAction(char *userInput, int *valid, int *action){
    *valid=1;
    *action=0; 
    char inputNoSp[100];
    inputNoSp[0]='\0';
    char *inputNoSp_ptr=inputNoSp;
    char *ogInput=userInput;
    //Check for spaces and symbols and save to inputNoSp
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

//This function evaluate the action/action index from action list received according to user input.
void checkAction1(char *userInput, int *valid, int *action, char *actionlist[],int listSize){
    int *validPtr=valid;
    char *userInputPtr=userInput;
    // printf("%s",userInputPtr);
    int listIndex=0;
    // int listSize=sizeof(actionlist)/sizeof(actionlist[0]);
    //For every character in the input:
    for(int i=0; userInputPtr[i]!='\0';i++){
        // Stop if its the end of the correct string
        if (actionlist[listIndex][i]=='\0'){
            *validPtr=0; 
            break;
        }
        //if the character being checked is the same, valid=1
        if(userInputPtr[i]==actionlist[listIndex][i]){
            *action=listIndex+1;
            *validPtr=1;
        } else {
            //if not, valid=0 and check the string again from the first character with the next variable in the array.
            *validPtr=0;
            i=-1;
            listIndex++;
        }
        //if until the end still cannot match any correct string to the input, valid=0
        if (listIndex==listSize){
            *validPtr=0;
            break;
        }
    }
}

//This function checks if the input is all numbers only
int checkDigit(char *input, int length){
    //If the input and desired length is not the same, valid=0
    if (strlen(input)!=length){
        return 0;
    }
    //Check the characters one by one if they are numbers
    for (int i=0; i<length;i++){
        if (!isdigit(input[i])){
            return 0;
        }
    }
    return 1;
}

//This function creates new account
void create(int *action){
    printf("====================================\n");
    printf("Create account\n");
    //Initialize evrything needed
    struct account acc;
    char accTypeTemp[100];
    //create account type array
    char type1[]="savings";
    char type2[]="current";
    char *accTypeList[]={type1,type2};
    int createValid=0;
    int typeListIndex=0;
    printf("Enter your name: \n");
    scanf("%[^\n]",acc.name);
    while((getchar()) != '\n');
    //Check if input for identity number is all numbers only
    int validity=1;
    do{
        printf("Enter your identity number: \n");
        scanf("%13[^\n]",acc.idNo);
        while((getchar()) != '\n');
        validity=checkDigit(acc.idNo,12);
        if (validity==0){
            printf("Please enter numbers only. Eg: 012345678901\n");
        }
    }while (validity==0);
    //Input and check if account type input is valid
    //evaluates account type according to account type list initialized above
    while (createValid==0){
        printf("Create Savings or Current account? (1.Savings/2.Current): \n");
        scanf("%[^\n]",accTypeTemp);
        while((getchar()) != '\n');
        //check if input got directly enter the index in front
        checkAction(accTypeTemp,&createValid,&typeListIndex);
        //if user didnt enter the index, check for the type using the list and input
        if (typeListIndex==0){
            int typelistSize=sizeof(accTypeList)/sizeof(accTypeList[0]);
            checkAction1(accTypeTemp,&createValid,&typeListIndex,accTypeList,typelistSize);
        } 
        //if still cannot find the type, valid=0 and prompt user to enter again.
        if (!createValid){
            printf("Invalid input. Please enter index or the actual word only.\n");
        }
    }
    //set account type to the type evaluated above
    strcpy(acc.accType,accTypeList[typeListIndex-1]);
    //Enter and check for 4 digit PIN
    do{
        printf("Enter your 4 digit PIN:");
        scanf("%s",acc.pin);
        while((getchar())!='\n');
        //check if it is all numbers and 4 digits long only
        validity=checkDigit(acc.pin,4);
        //if not, loop again
        if (validity==0){
            printf("Invalid input. Please enter 4 numbers only.\n");
        }
    } while (validity==0);

    //generate bank account number randomly but 7~9 digits long
    int max=999999999;
    int min=1000000;
    int bankAccNo = rand() % (max-min +1) +min;
    int valid=0;
    char accno[100];
    //check if the bank account number already exist in index file
    while (valid==0){
        FILE* fptr;
        fptr=fopen("database\\index.txt","r");
        char c[10];
        // int i=0;
        do{
            sprintf(accno,"%d",bankAccNo);
            //get bank acc number stored in index file line by line/one by one
            fgets(c,10,fptr);
            c[strcspn(c, "\n")] = '\0';
            //check if the number in the index file is the same with the generated one.
            //If so, generate another one and evaluate again
            if (checkInputSimilarity(accno,c)){
                bankAccNo = rand() % (max-min +1) +min; 
                valid=0;
                break;
            }
            //if check till the end of the file also no similar number, valid=1
            if (feof(fptr)){
                valid=1;
                break ;
            }
        }  while(1);
        fclose(fptr);
    }
    //set the bank account number to the valid number generated
    acc.bankAccNo=bankAccNo;
    //initialize account balance
    acc.balance=0.00;
    //create and save data to file
    FILE* fptr;
    sprintf(accno, "database\\%d.txt", bankAccNo);
    fptr=fopen(accno,"w");
    if (!fptr){
        printf("Error opening.");
    }
    //make all data to be stored into one line/variable
    char temp[1000];
    sprintf(temp,"%s\n%s\n%s\n%s\n%d\n%.2f",acc.name,acc.idNo,acc.accType,acc.pin,acc.bankAccNo,acc.balance);
    // save it in
    fputs(temp, fptr);
    fclose(fptr);

    // Add the bank number into index file
    fptr=fopen("database\\index.txt","a");
    fprintf(fptr, "%d\n", bankAccNo);
    fclose(fptr);

    //report to the user
    printf("====================================\n");
    printf("Account created successfully!\n");
    printf("Bank account number: %d\n",acc.bankAccNo);
    printf("Name: %s\n",acc.name);
    printf("Identity number: %s\n",acc.idNo);
    printf("Bank account number: %d\n",acc.bankAccNo);
    printf("Account type: %s\n",acc.accType);
    printf("Current balance: RM%.2f\n",acc.balance);
    //add activity to log
    writetolog(*action,acc,0,0);
}

//This function checks for yes or no
int checkYesNo(char *input){
    //create yes or no list
    char *list[]={"yes","no"};
    int listIndex=0;
    int yesNoValid=0;
    //check if input has index in front and convert input to lowercase without any other symbols
    checkAction(input,&yesNoValid,&listIndex);
    if (yesNoValid==0){
        listIndex=0;
        return listIndex;
    }
    if (listIndex==0){
        int listSize=sizeof(list)/sizeof(list[0]);
        //Check the input with the variable stored in the list one by one
        checkAction1(input,&yesNoValid,&listIndex,list,listSize);
    } 
    return listIndex;
}


void delete(int *action){
    //Initialize variables needed
    struct account acc;
    printf("====================================\n");
    printf("Delete account\n");
    printf("List of all accounts:\n");
    char temp[1000];
    //show all bank account numbers stored in the index file one by one
    FILE *fptr;
    fptr=fopen("database\\index.txt","r");
    //"c" is the bank account number retrieved from index file
    char c[10];
    //"i" will be like the index printed out
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
    //Enter account number that user wants to delete
    do {
        printf("Which account you want to delete? ");
        scanf("%9[^\n]",&accChosen);
        while((getchar()) != '\n');
        //Check if all input are numbers only
        for (int i=7; i<=9; i++){
            if (validity==0){
                validity=checkDigit(accChosen,i);
            }
        }
        if (validity==0){
            printf("Invalid input. Please enter your full bank account number instead of index.\n");
            continue;
        }
        //check if the file exist
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
    //Check if bank account number entered exists in index file
    //"c" is the valid bank account number entered by user, evaluate and copy from index file
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
    //reconfirm with user if they truely want to delete the account
    int choice;
    do{
        printf("Do you really want to delete account %s (1. Yes/2. No)?\n",c);
        char input[10];
        scanf("%9[^\n]",input);
        while((getchar()) != '\n');
        choice=checkYesNo(input);
        if (choice!=1 && choice!=2){
            printf("Invalid input. Please enter the index or keywords only.\n");
        }
    } while (choice!=1 && choice!=2);
    //if user regret, return to menu
    if (choice==2){
        printf("Returning to menu..");
        return;
    }

    char path[30];
    //c is the valid bank account number chosen
    sprintf(path, "database\\%s.txt", c);
    //open the bank account file chosen.
    fptr=fopen(path,"r");
    if (!fptr){
        printf("error");
    }
    //get all data from the file
    fscanf(fptr,"%99[^\n]\n%s\n%s\n%s\n%d\n%f",acc.name,acc.idNo,acc.accType,acc.pin,&acc.bankAccNo,&acc.balance);
    // get the correct last 4 digit of the id number
    char correctIdInput[5];
    sprintf(correctIdInput, "%s", acc.idNo + strlen(acc.idNo) - 4);
    fclose(fptr);

    //ask the user to enter the last 4 digit of their id number and check if its valid
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

    // Ask user to input the 4 digit PIN and check if its the same as the PIN stored in the file
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
    //Delete the account file
    remove(path);
    //remove it from the index file
    fptr=fopen("database\\index.txt","r");
    FILE *fptrtemp;
    fptrtemp=fopen("database\\indextemp.txt","w");
    fclose(fptrtemp);
    fptrtemp=fopen("database\\indextemp.txt","a+");
    char prevaccno[10];
    //copy all bank account number to the temp index file
    do
    {
        fgets(c,10,fptr);
        if (feof(fptr)){
            break ;
        }
        c[strcspn(c, "\n")] = '\0';
        //except for the account number thats wants to be deleted
        if (checkInputSimilarity(accChosen,c)){
            continue;
        }
        //avoid duplicating bank account numbers in the temp index file
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
    //remove old index file
    remove("database\\index.txt");
    //rename temp file 
    rename("database\\indextemp.txt","database\\index.txt");
    printf("Account deleted successfully!\n");
    //write to log
    writetolog(*action,acc,0,0);
}

//Function to update the account file
void rewrite(struct account acc){
    char path[30];
    char temp[1000];
    FILE *fptr;
    //create temp file
    fptr=fopen("database\\temp.txt","w");
    //save all data as a variable
    sprintf(temp,"%s\n%s\n%s\n%s\n%d\n%.2f",acc.name,acc.idNo,acc.accType,acc.pin,acc.bankAccNo,acc.balance);
    sprintf(path, "database\\%d.txt", acc.bankAccNo);
    //save it in temp file
    fputs(temp, fptr);
    fclose(fptr);
    //remove old file
    if(remove(path)){
        printf("Error deleting temp file.");
    }
    //rename temp file as the official bank account number to replace the old (deleted) one 
    rename("database\\temp.txt",path);
}

//Check if input for prices are decimals with 2 decimal places only.
int checkDec(char *amount, int validation){
    validation=1;
    int dotCount=0;
    int decimals=0;
    for (int i=0; amount[i]!='\0'; i++){
        if (amount[i]=='.'){
            dotCount++;
            //ensure that the input only got one . (eg: 1.10.1 cannot but 1.00 ok)
            if (dotCount>1){
                validation = 0;
            }
        //if the input is not ".", it has to be number
        } else if (!isdigit(amount[i])){
            validation=0;   
        //check many decimal place are there
        } else if (dotCount==1){
            decimals++;  
        }
    }
    // if decimal place more than 2 then invalid
    if (!validation || decimals>2) {
        validation=0;
    }
    return validation;
}
//This function allow users to deposit money
void deposit(int *action){
    //initialize variables needed
    struct account acc;
    printf("====================================\n");
    printf("Deposit\n");
    int validation=1;
    char bankAccEntered[10];
    //ask user to enter bank acc number and check if its valid
    do{
        printf("Enter your bank account number: \n");
        scanf("%s",bankAccEntered);
        while((getchar()) != '\n'); 
        //check if input is all numbers
        for (int i=7; i<=9; i++){
            if (validation==0){
                validation=checkDigit(bankAccEntered,i);
            }
        }
        if (validation==0){
            printf("Invalid input. Please enter your bank account number again.\n");
            continue;
        }
        //check if bank account exist
        char path[30];
        FILE *fptr;
        sprintf(path,"database\\%s.txt",bankAccEntered);
        fptr=fopen(path,"r");
        if (fptr==NULL){
            validation=0;
            continue;
        }
        //retrieve all data from the file
        fscanf(fptr,"%99[^\n]\n%s\n%s\n%s\n%d\n%f",acc.name,acc.idNo,acc.accType,acc.pin,&acc.bankAccNo,&acc.balance); 
        // printf("%s",acc.pin);
        fclose(fptr);
        if (validation==0){
            printf("Invalid input. Please enter your bank account number again.\n");
            continue;
        }
    }while(validation==0);

    //Ask user to enter 4 digit pin
    char pinEntered[6];
    do{
        printf("Enter your 4-digit PIN: ");
        scanf("%5s",pinEntered);
        while((getchar()) != '\n'); 
        //check if its the same as the PIN stored in the file
        validation=checkInputSimilarity(pinEntered,acc.pin);
        if (!validation){
            printf("Incorrect PIN enetered. Try again.\n");
        }
    } while (validation==0);
    printf("====================================\n");
    printf("Welcome back %s!\n",acc.name);
    printf("Your current balance: RM%.2f\n",acc.balance);
    char depositAmountTemp[10];
    float depositAmount;
    //let user input deposit amount
    do{
        printf("Desired deposit amount: RM");
        scanf("%s",depositAmountTemp);
        while((getchar()) != '\n'); 
        //check if input is valid for price
        validation=checkDec(depositAmountTemp,validation);
        if (validation){
            //copy from the temp variable and make it float instead of string
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
    //update file
    rewrite(acc);
    //report
    printf("====================================\n");
    printf("Deposit successful!\nCurrent balance: RM%.2f\nReturning to menu..\n",acc.balance);
    //write to log
    writetolog(*action,acc,depositAmount,0);
}
//This function allow users  to withdraw money
void withdrawal(int *action){
    printf("====================================\n");
    printf("Withdrawal\n");
    struct account acc;
    int validation=1;
    char bankAccEntered[10];
    //Ask user to enter the bank account number and check if its valid
    do{
        printf("Enter your bank account number: \n");
        scanf("%s",bankAccEntered);
        while((getchar()) != '\n'); 
        for (int i=7; i<=9; i++){
            if (validation==0){
                //valid if all of em are numbers
                validation=checkDigit(bankAccEntered,i);
            }
        }
        if (!validation){
            printf("Invalid input. Please enter your bank account number again.\n");
            continue;
        }
        //valid if the account exist
        char path[30];
        FILE *fptr;
        sprintf(path,"database\\%s.txt",bankAccEntered);
        fptr=fopen(path,"r");
        if (fptr==NULL){
            validation=0;
            return;
        }
        //retrieve all data if file exist
        fscanf(fptr,"%99[^\n]\n%s\n%s\n%s\n%d\n%f",acc.name,acc.idNo,acc.accType,acc.pin,&acc.bankAccNo,&acc.balance); 
        fclose(fptr);
        if(!validation){
            printf("Invalid input. Please enter your bank account number again.\n");
        }
    }while(validation==0);
    //Ask user to enter 4 digit PIN and check its validity
    char pinEntered[6];
    do{
        printf("Enter your 4-digit PIN: ");
        scanf("%5s",pinEntered);
        while((getchar()) != '\n'); 
        //if its the same as the PIN saved in file
        validation=checkInputSimilarity(pinEntered,acc.pin);
        if (!validation){
            printf("Incorrect PIN entered.\n");
        }
    } while (validation==0);
    printf("====================================\n");
    printf("Welcome back %s!\n",acc.name); 
    printf("Your current balance: RM%.2f\n",acc.balance);
    //ask user to enter withdrawal amount
    char withdrawalAmountTemp[10];
    float withdrawalAmount;
    do{
        printf("Desired withdrawal amount: RM");
        scanf("%s",withdrawalAmountTemp);
        while((getchar()) != '\n');   
        // check if input is valid format for price
        validation=checkDec(withdrawalAmountTemp,validation);
        if (validation){
            //copy input as float instead of string
            withdrawalAmount=strtof(withdrawalAmountTemp,NULL);
            if (withdrawalAmount>0 && withdrawalAmount<=acc.balance){
                validation=1;
            } else{
                validation=0;
            }
        }
        if (!validation){
            printf("Invalid input. Please enter valid numbers within your current balance with 2 decimal place only.\n");
        }
    }while (validation==0);
    acc.balance=acc.balance-withdrawalAmount;
    // update file
    rewrite(acc);
    printf("====================================\n");
    printf("Withdrawal successful!\nCurrent balance: RM%.2f\nReturning to menu..\n",acc.balance);
    //write to file
    writetolog(*action,acc,withdrawalAmount,0);
}
//Function for remittance/transaction
void remittance(int *action){
    // Initialize all variables needed
    printf("====================================\n");
    printf("Remittance\n");
    struct account senderAcc;
    struct account receiverAcc;
    int validation=0;
    char senderBankAcc[10];
    char path[30];
    FILE *fptr;
    //ask user (sender) to enter bank account number and check its validity
    do{
        printf("Enter your bank account number: \n");
        scanf("%s",senderBankAcc);
        while((getchar()) != '\n'); 
        for (int i=7; i<=9; i++){
            if (validation==0){
                //Chcek if all of em are numbers
                validation=checkDigit(senderBankAcc,i);
            }
        }
        if (!validation){
            printf("Invalid input. Please enter your bank account number again.\n");
            continue;
        }
        sprintf(path,"database\\%s.txt",senderBankAcc);
        //try to open the file
        fptr=fopen(path,"r");
        if (fptr==NULL){
            validation=0;
        }
        //retrieve the data from the file if exist
        fscanf(fptr,"%99[^\n]\n%s\n%s\n%s\n%d\n%f",senderAcc.name,senderAcc.idNo,senderAcc.accType,senderAcc.pin,&senderAcc.bankAccNo,&senderAcc.balance); 
        fclose(fptr);
        if (!validation){
            printf("Invalid input. Please enter your bank account number again.\n");
        }
    }while(validation==0);

    // ask user to enter 4 digit PIN and check if it's valid
    char pinEntered[6];
    do{
        printf("Enter your 4-digit PIN: ");
        scanf("%5s",pinEntered);
        while((getchar()) != '\n'); 
        //valid if input is the same as the PIN stored in file
        validation=checkInputSimilarity(pinEntered,senderAcc.pin);
        if (!validation){
            printf("Incorrect PIN entered. Please try again.\n");
        }
    } while (validation==0);
    printf("====================================\n");
    printf("Welcome back %s!\n",senderAcc.name);
    printf("Your current balance: RM%.2f\n",senderAcc.balance);
    char transferAmountTemp[10];
    float transferAmount;
    //ask user to enter receiver bank account number
    char receiverBankAcc[10];
    do{
        printf("Enter receiver bank account number: \n");
        scanf("%s",receiverBankAcc);
        while((getchar()) != '\n'); 
        for (int i=7; i<=9; i++){
            if (validation==0){
                //check if the input is all numbers only
                validation=checkDigit(receiverBankAcc,i);
            }
        }
        sprintf(path,"database\\%s.txt",receiverBankAcc);
        //check if user entered the same bank account number for sender and receiver
        if (checkInputSimilarity(senderBankAcc,receiverBankAcc)){
            validation=0;
            printf("Invalid input. Sender and receiver are the same.\n");
        }
        //check if file exists
        fptr=fopen(path,"r");
        if (fptr==NULL){
            printf("Receiver doesn't exist. Please try again.\n");
            validation=0;
        } else {
            //retrieve data if file exist
            fscanf(fptr,"%99[^\n]\n%s\n%s\n%s\n%d\n%f",receiverAcc.name,receiverAcc.idNo,receiverAcc.accType,receiverAcc.pin,&receiverAcc.bankAccNo,&receiverAcc.balance); 
        }
        fclose(fptr);
    }while(validation==0);
    float remittancefee;
    //ask user to enter amount to be transfered.
    do{
        printf("Amount to be transferred (Limit: 50000): RM");
        scanf("%s",transferAmountTemp);
        while((getchar()) != '\n');   
        //check if input format is valid
        validation=checkDec(transferAmountTemp,validation);
        if (validation){
            //copy as float instead of string
            transferAmount=strtof(transferAmountTemp,NULL);
            //check if remittance fee should be applied
            if(!checkInputSimilarity(senderAcc.accType,receiverAcc.accType)){
                //saving to current is 2%
                if (checkInputSimilarity(senderAcc.accType,"savings")){
                    remittancefee=transferAmount*0.02;
                } else {
                    //current to savings is 3%
                    remittancefee=transferAmount*0.03;
                }
            }
            if ((transferAmount+remittancefee)>0 && (transferAmount+remittancefee)<=senderAcc.balance){
                validation=1;
            } else{
                printf("Invalid input. Please enter valid numbers within your balance with 2 decimal place only.\n");
                printf("Please note that remittance fee might be included. Ensure that you bank account has extra money for it.\n");
                validation=0;
            }
        }
    }while (validation==0);

    senderAcc.balance=senderAcc.balance-transferAmount-remittancefee;
    //update sender's file
    rewrite(senderAcc);
    //update log
    // float amounttemp=(transferAmount+remittancefee);
    writetolog(*action,senderAcc,transferAmount,1);

    //update receiver's file
    receiverAcc.balance=receiverAcc.balance+transferAmount;
    rewrite(receiverAcc);

    //report
    printf("====================================\n");
    printf("Transaction successful!\n");
    printf("Sender: %s\n",senderAcc.name);
    printf("Bank account: %d\n",senderAcc.bankAccNo);
    printf("Receiver: %s\n",receiverAcc.name);
    printf("Bank account: %d\n",receiverAcc.bankAccNo);
    printf("Amount transfered: RM%.2f\n",transferAmount);
    printf("Remittance fee: RM%.2f\n",remittancefee);

    //update log
    writetolog(*action,receiverAcc,transferAmount,0);
}

//Main menu
void menu(){
    time_t currentTime;
    time(&currentTime); 
    printf("====================================\n");
    printf("SESSION START\n");
    printf("Current Date and Time: %s",ctime(&currentTime));
    //count all bank account numbers stored in the index file one by one
    FILE *fptr;
    fptr=fopen("database\\index.txt","r");
    //"c" is the bank account number retrieved from index file
    char c[10];
    int i=0;
    do{
        fgets(c,10,fptr);
        if (feof(fptr))
            break ;
        i=i+1;
    }  while(1);
    fclose(fptr);
    printf("Numbers of account present: %d\n",i);
    //initialize variables needed
    char userInput[100];
    char action1[]="createnewbankaccount";
    char action2[]="deletebankaccount";
    char action3[]="deposit";
    char action4[]="withdrawal";
    char action5[]="remittance";
    char action6[]="quit";
    //create action list
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
    //check user input
    while (valid==0){
        printf("Your action:");
        scanf("%[^\n]",userInput);
        while((getchar()) != '\n');
        //check if user got enter index from list directly
        checkAction(userInput,&valid,&action);
        //input de or d could lead to both delete or deposit, thus rejected
        if (checkInputSimilarity(userInput,"de") || checkInputSimilarity(userInput,"d")){
            valid=0;
            printf("Invalid input. 'de' and 'd' may lead to delete or deposit.\n");
            continue;
        }
        // if no action number input, evaluate base on text input and action list
        if (action==0){
            int actionlistSize=sizeof(actionlist)/sizeof(actionlist[0]);
            checkAction1(userInput,&valid,&action,actionlist, actionlistSize);
        }
        if (valid==0){
            printf("Invalid input. Please enter index (within 1 to 6) or keywords only.\n");
        }
    }
    //lead to functions
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
    FILE *fptr;
    fptr=fopen("database\\index.txt", "r");
    if (fptr==NULL) {
        fptr=fopen("database\\index.txt", "w");
    }
    fclose(fptr);
    fptr=fopen("database\\transaction.log", "r");
    if (fptr==NULL) {
        fptr=fopen("database\\transaction.log", "w");
    }
    fclose(fptr);
    while(1){
        menu();
    }
    return 0;
}