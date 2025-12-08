# BankingSystemApp-Coursework
This is a simple banking system that allow users to perform several actions including create account, delete account, deposit money, withdraw money and remittance/transfer money to other accounts.

IMPORTANT NOTES: 
- Please ensure you have a folder named "database" before using this program.
- There is no returning back, you have to enter until u got the correct input. Use ctrl+c to terminate the code halfway.
- After finishing one function, it loops back to the menu where you can exit the code there.

FUNCTIONS AND INPUT RULES:
1) Menu
   - Users can choose their action here.
   - Several inputs are allowed, including entering index number from the list (eg: "1","2"), keywords (eg: "create","delete") or both (eg: "1. Create", "1 Create").
     - If user input "1) Creatw ", even if the keyword behind has a wrong spelling, the system will lead to the action according to the number in front.
     - Same goes to "1) Delete", it will enter create as its the first in the list instead of delete.
2) Create
   - Users have to enter their name, identity number, account type and 4 digit PIN number. Account number is generated randomly and the initial balance will be 0.00.
   - Name: There's no input rules/limit for names. Symbols can be included (eg: Mathuram a/p Anbuselvan).
   - Identity number: Only numbers are allowed (eg: 123456-12-1234 (x) 123456121234(/))
   - Account type: similar to the input rule for selecting action in menu, just that the keywords are changed to "savings" and "current".
   - 4 digit PIN: Only accept numbers and not more than 4 digit.
   - Account number: Generated randomly. Could be 7 digits to 9 digits.
   - Initial balance: 0.00
3) Delete
   - User have to enter bank account number at first, then reconfirm if they are sure to delete the account, next enter the last 4 digits of their identity number and 4 digit PIN to evaluate if they are the actual owner of the account.
   - Bank account number: Only accept the full bank account number. Entering index from the list will loop again and require user to re-enter.
   - Reconfirm: Similar to input rule in menu, keywords changed to "yes" and "no". Index input is accepted.
   - Identity number: Must be exactly same as the last 4 digits of the identity number stored in the file. All other inputs will be rejected.
   - 4 digit PIN: Must be exactly same as the 4 digit PIN stored in the file. All other inputs will be rejected.
4) Deposit
   - User have to input bank account number and 4 digit PIN to enter. Then it will show the current balance and prompts user to enter deposit amount.
   - Bank account number: Only accept input that is exactly the same as the user's bank account number in the folder. All other inputs will be rejected.
   - 4 digit PIN: Must be exactly same as the 4 digit PIN stored in the file. All other inputs will be rejected.
   - Deposit amount: Accept numbers and one "." to represent cents only. Entering alphabets, more than one "." (eg:100.10.10) and more than 2 decimal place (eg: 100.999) will be rejected.
5) Withdrawal
   - User have to input bank account number and 4 digit PIN to enter. Then it will show the current balance and prompts user to enter withdrawal amount.
   - Bank account number: Only accept input that is exactly the same as the user's bank account number in the folder. All other inputs will be rejected.
   - 4 digit PIN: Must be exactly same as the 4 digit PIN stored in the file. All other inputs will be rejected.
   - Withdrawal amount: Accept numbers and one "." to represent cents only. Entering alphabets, more than one "." (eg:100.10.10) and more than 2 decimal place (eg: 100.999) will be rejected.
6) Remittance (transaction)
   - User have to input bank account number and 4 digit PIN to enter.
   - Then it will show the current balance and prompts user to enter transaction amount.
   - User will then have to enter the bank account number of the receiver.
   - Bank account number (sender and receiver): Only accept input that is exactly the same as the user's bank account number in the folder. All other inputs will be rejected.
   - 4 digit PIN: Must be exactly same as the 4 digit PIN stored in the file. All other inputs will be rejected.
   - Transfer amount: Accept numbers and one "." to represent cents only. Entering alphabets, more than one "." (eg:100.10.10) and more than 2 decimal place (eg: 100.999) will be rejected.
