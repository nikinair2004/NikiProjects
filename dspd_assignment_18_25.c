#include <stdio.h>
#include <string.h>
#define MAX 1000
#define MAX_SHARED_USERS 4
typedef struct User
{
    int userid;
    char username[30];
    float tot_amt;
}
User;
typedef struct Expense
{
    int expenseid;
    float amt;
    int paidByUserid;
    int sharedUserid[MAX_SHARED_USERS];
    int numSharedUsers;
}
Expense;
void bubblesort_user(User users[],int n)
{
    User temp;
    for(int i=0;i<n-1;i++)
    {
        for(int j=0;j<n-i-1;j++){
            if(users[j].userid>users[j+1].userid){
                temp=users[j];
                users[j]=users[j+1];
                users[j+1]=temp;
            }
        }
    }
}
void bubblesort_exp(Expense exp[],int n)
{
    Expense temp;
    for(int i=0;i<n-1;i++)
    {
        for(int j=0;j<n-i-1;j++){
            if(exp[j].expenseid>exp[j+1].expenseid){
                temp=exp[j];
                exp[j]=exp[j+1];
                exp[j+1]=temp;
            }
        }
    }
}
void initialize(User users[],int numUsers)
{
for(int i=0;i<numUsers;i++)
{
    printf("Enter unique userid (integer of 3 digits): ");
    scanf("%d",&users[i].userid);
    if (users[i].userid < 100 || users[i].userid > 999) {
        printf("Invalid User ID. It must be a unique 3-digit integer.\n");
        return;
    }
    printf("Enter username: ");
    scanf("%s",users[i].username);
    users[i].tot_amt=0.0;
}
}
int findUserIndex(User users[], int numUsers, int userid) {
    for (int i=0;i<numUsers;i++) {
        if (users[i].userid==userid) {
            return i;
        }
    }
    return -1;
}
void RecalculateBalances(User users[],int numUsers,Expense exp[],int numExpenses,int deletedUserID) {
    for (int i = 0; i < numUsers; i++) {
        if (users[i].userid != deletedUserID) {
            double balance = 0.0;
            for (int j = 0; j < numExpenses; j++) {
                Expense currentExpense = exp[j];
                if (currentExpense.paidByUserid == users[i].userid) {
                    balance -= currentExpense.amt; // User paid an expense
                } else if (findUserIndex(users, numUsers, currentExpense.paidByUserid) != -1) {
                    balance += currentExpense.amt / (currentExpense.numSharedUsers + 1); // User is owed money
                }
            }
            users[i].tot_amt = balance;
        }
    }
}
void AddUser(User users[],int *numUsers)
{
    int n=*numUsers;
    if (n >= MAX) {
        printf("Maximum number of users reached.\n");
        return;
    }
    User newUser;
    printf("Enter the User ID (3 digits) for the user: ");
    scanf("%d", &newUser.userid);
    if (newUser.userid < 100 || newUser.userid > 999) {
        printf("Invalid User ID. It must be a unique 3-digit integer.\n");
        return;
    }
    users[n].userid=newUser.userid;
    printf("Enter username: ");
    scanf("%s",newUser.username);
    //sort
    strcpy(users[n].username,newUser.username);
    users[n].tot_amt=0.0;
    n=n+1;
    *numUsers=n;
    
    bubblesort_user(users,n);
    printf("Successfully added new user id and sorted it\n");
    for(int i=0;i<*numUsers;i++){
        printf("%d %s %f",users[i].userid,users[i].username,users[i].tot_amt);
        printf("\n");
    }
    return;
}
void AddExpense(User users[],int numUsers,Expense exp[],int *numExpenses)
{
    if (*numExpenses >= MAX) {
        printf("Maximum number of expenses reached.\n");
        return;
    }
    Expense newExpense;
    printf("Enter the Expense ID (3 digits): ");
    scanf("%d", &newExpense.expenseid);

    if (newExpense.expenseid < 100 || newExpense.expenseid > 999) {
        printf("Invalid Expense ID. It must be a unique 3-digit integer.\n");
        return;
    }
    printf("Enter the amount of the expense: ");
    scanf("%f", &newExpense.amt);

    printf("Enter the User ID who paid the expense: ");
    scanf("%d", &newExpense.paidByUserid);

    int payerIndex = findUserIndex(users,numUsers,newExpense.paidByUserid);

    if (payerIndex == -1) {
        printf("User not found.\n");
        return;
    }
    users[payerIndex].tot_amt += newExpense.amt;
    printf("Enter number of people you wish to share with: (<4) ");
    scanf("%d",&newExpense.numSharedUsers);
    printf("Enter the User IDs of people to share the expense with (up to 4): ");
    for (int i = 0; i < newExpense.numSharedUsers; i++) {
        int sharedUserid;
        scanf("%d", &sharedUserid);

        if (sharedUserid<100 || sharedUserid>999) {
            break;
        }

        int sharedIndex = findUserIndex(users, numUsers, sharedUserid);
        if (sharedIndex != -1) {
            newExpense.sharedUserid[i] = sharedUserid;
            //newExpense.numSharedUsers++;
        } else {
            printf("User not found. This share will be ignored.\n");
        }
    }
    int n=*numExpenses;
    exp[n].expenseid=newExpense.expenseid;
    exp[n].amt=newExpense.amt;
    exp[n].paidByUserid=newExpense.paidByUserid;
    for(int i=0;i<newExpense.numSharedUsers;i++){
        exp[n].sharedUserid[i]=newExpense.sharedUserid[i];
    }
    exp[n].numSharedUsers=newExpense.numSharedUsers;
    n=n+1;
    *numExpenses=n;
    
    bubblesort_exp(exp,n);
    printf("Successfully added new expense and sorted it according to expense id\n");
    for(int i=0;i<n;i++){
        printf("%d %f %d %d",exp[i].expenseid,exp[i].amt,exp[i].paidByUserid,exp[i].numSharedUsers);
        printf("\n");
     }
    return;
}

void AmountOwed(User users[], int numUsers, int targetUserID,Expense exp[], int numExpenses) {
    float totalOwed = 0.0;
    printf("User ID: %d, Name: %s\n", targetUserID, users[findUserIndex(users, numUsers, targetUserID)].username);

    for (int i = 0; i < numExpenses; i++) {
        Expense currentExpense = exp[i];
        
        if (currentExpense.paidByUserid == targetUserID) {
            for (int j = 0; j < currentExpense.numSharedUsers; j++) {
                int sharedUserid = currentExpense.sharedUserid[j];
                int sharedUserIndex = findUserIndex(users, numUsers, sharedUserid);
                
                if (sharedUserIndex == -1) {
                    printf("User not found for Expense ID: %d. This share will be ignored.\n", currentExpense.expenseid);
                    continue;
                }
                
                float sharedAmount = currentExpense.amt / (currentExpense.numSharedUsers + 1);
                float amountOwed = -(users[sharedUserIndex].tot_amt - sharedAmount);
                
                printf("User ID: %d, Name: %s, Owes: %.2lf for Expense ID: %d\n", sharedUserid, users[sharedUserIndex].username, amountOwed, currentExpense.expenseid);
                totalOwed += amountOwed;
            }
        }
    }

    printf("Total Amount Owed: %.2lf\n", totalOwed);
}


void AmountToPay(User users[], int numUsers, int targetUserID,Expense exp[], int numExpenses) {
    float totalToPay = 0.0;
    printf("User ID: %d, Name: %s\n", targetUserID, users[findUserIndex(users, numUsers, targetUserID)].username);

    for (int i = 0; i < numExpenses; i++) {
        Expense currentExpense = exp[i];
        
        if (currentExpense.paidByUserid != targetUserID) {
            if (currentExpense.paidByUserid == targetUserID) {
                continue;  // Skip expenses paid by the target user themselves
            }

            if (findUserIndex(users, numUsers, currentExpense.paidByUserid) == -1) {
                printf("User not found for Expense ID: %d. This expense will be ignored.\n", currentExpense.expenseid);
                continue;
            }

            if (currentExpense.numSharedUsers > 0) {
                double sharedAmount = currentExpense.amt / (currentExpense.numSharedUsers + 1);
                int targetUserIndex = findUserIndex(users, numUsers, targetUserID);
                double amountToPay = sharedAmount;

                printf("User ID: %d, Name: %s, To Pay: %.2lf for Expense ID: %d\n", currentExpense.paidByUserid, users[findUserIndex(users, numUsers, currentExpense.paidByUserid)].username, amountToPay, currentExpense.expenseid);
                totalToPay += amountToPay;
            }
        }
    }

    printf("Total Amount to Pay: %.2lf\n", totalToPay);
}

void UserBalances(struct User users[], int numUsers, struct Expense exp[], int numExpenses) {
    printf("User Balances:\n");
    
    for (int i = 0; i < numUsers; i++) {
        int userid= users[i].userid;
        const char* userName = users[i].username;
        float balance = 0.0;

        // Calculate the balance for the user
        for (int j = 0; j < numExpenses; j++) {
            struct Expense currentExpense = exp[j];

            if (currentExpense.paidByUserid == userid) {
                balance -= (currentExpense.amt - currentExpense.amt / (currentExpense.numSharedUsers + 1)); // User paid an expense
                
            } else if (findUserIndex(users, numUsers, currentExpense.paidByUserid) != -1) {
                balance += currentExpense.amt / (currentExpense.numSharedUsers + 1); // User is owed money
            }
        }

        printf("User ID: %d, Name: %s, Balance: %.2lf\n", userid, userName, balance);
    }
}
void SettleUp(User users[], int numUsers,Expense exp[], int *numExpenses, int USER_ID_1, int USER_ID_2) {
    // Find the indices of USER_ID_1 and USER_ID_2
    int index1 = findUserIndex(users, numUsers, USER_ID_1);
    int index2 = findUserIndex(users, numUsers, USER_ID_2);

    // Check if both users exist
    if (index1 == -1 || index2 == -1) {
        printf("One or both users not found.\n");
        return;
    }

    // Display all the amount USER_ID_1 owes to USER_ID_2 with the expense ID
    printf("Expenses to settle from USER_ID_%d to USER_ID_%d:\n", USER_ID_1, USER_ID_2);
    for (int i = 0; i < *numExpenses; i++) {
        struct Expense currentExpense = exp[i];
        if (currentExpense.paidByUserid == USER_ID_2) {
            int j;
            for (j = 0; j < currentExpense.numSharedUsers; j++) {
                if (currentExpense.sharedUserid[j] == USER_ID_1) {
                    printf("Expense ID: %d, Amount: %.2lf\n", currentExpense.expenseid, currentExpense.amt / (currentExpense.numSharedUsers + 1));
                    break;
                }
            }
            if (j < currentExpense.numSharedUsers) {
                // Found a shared expense, so it can be settled
                double amountToSettle = currentExpense.amt / (currentExpense.numSharedUsers + 1);

                // Update USER_ID_1's balance
                users[index1].tot_amt -= amountToSettle;

                // Remove USER_ID_1 from the shared users
                for (int k = j; k < currentExpense.numSharedUsers - 1; k++) {
                    currentExpense.sharedUserid[k] = currentExpense.sharedUserid[k + 1];
                }
                currentExpense.numSharedUsers--;

                // Update USER_ID_2's balance
                users[index2].tot_amt += amountToSettle;

                // Remove the expense if there are no more shared users
                if (currentExpense.numSharedUsers == 0) {
                    // Shift the remaining expenses to remove the current expense
                    for (int k = i; k < *numExpenses - 1; k++) {
                        exp[k] = exp[k + 1];
                    }
                    (*numExpenses)--;
                    i--; // Check the same index again after the shift
                }
            }
        }
    }
}

void DeleteUser(User users[], int *numUsers,Expense exp[], int numExpenses, int userIDToDelete) {
    int userIndex = findUserIndex(users, *numUsers, userIDToDelete);
    
    if (userIndex == -1) {
        printf("User with User ID %d not found.\n", userIDToDelete);
        return;
    }
    
    float balance = 0.0;
    
    // Calculate the balance for the user
    for (int i = 0; i < numExpenses; i++) {
        struct Expense currentExpense = exp[i];
        
        if (currentExpense.paidByUserid == userIDToDelete) {
            balance -= (currentExpense.amt - currentExpense.amt / (currentExpense.numSharedUsers + 1)) ; // User paid an expense
        } else if (findUserIndex(users, *numUsers, currentExpense.paidByUserid) != -1) {
            balance += currentExpense.amt / (currentExpense.numSharedUsers + 1); // User is owed money
        }
    }
    
    if (balance == 0.0) {
        // User's balance is settled
        for (int i = userIndex; i < *numUsers - 1; i++) {
            users[i] = users[i + 1];
        }
        (*numUsers)--;
        printf("User with User ID %d has been deleted.\n", userIDToDelete);
        //RecalculateBalances(users,*numUsers,exp,numExpenses,userIDToDelete);
    } else {
        // User's balance is not settled
        printf("User with User ID %d has an outstanding balance of %.2lf.\n", userIDToDelete, balance);
        printf("Please settle the outstanding balance before deleting the user.\n");
    }
}
void DeleteExpense(Expense exp[], int *numExpenses, int expenseID, int userID) {
    int deleted = 0;  // Variable to track whether the expense was deleted

    int i=0;
    while(i<*numExpenses && deleted==0) {
        if (exp[i].expenseid == expenseID && exp[i].paidByUserid == userID) {
            // Remove the matched expense by shifting the rest of the expenses
            for (int j = i; j < *numExpenses - 1; j++) {
                exp[j] = exp[j + 1];
            }
            (*numExpenses)--;
            deleted = 1;  // Expense was deleted
            printf("Expense deleted.\n");
        }
        i++;
    }
    
    if (!deleted) {
        printf("Enter correct user and expense ID.\n");
    }
}
int main()
{
    User users[MAX];
    Expense exp[MAX];
    int numUsers,numExpenses=0;
    printf("Enter the number of users (up to %d): ", MAX);
    scanf("%d", &numUsers);
    if (numUsers <= 0 || numUsers > MAX) 
    {
        printf("Invalid number of users.\n");
    }
    initialize(users,numUsers);
    int targetuserid;
    int flag=1;
    while(flag==1)
    {
        int choice;
        printf("\nOptions:\n");
        printf("1. Add User\n");
        printf("2. Add Expense\n");
        printf("3. Amount Owed\n");
        printf("4. Amount to Pay\n");
        printf("5. User Balances\n");
        printf("6. Settle Up\n");
        printf("7. Delete User\n");
        printf("8. Delete Expense\n");
        printf("Any other number to Quit\n");
        printf("Enter your choice: ");
        scanf("%d",&choice);

        switch(choice){
            case 1:
                AddUser(users,&numUsers);
                break;
            case 2:
                AddExpense(users,numUsers,exp,&numExpenses);
                break;
            case 3:
                printf("Enter userid of the user to which others owe the money:");
                scanf("%d",&targetuserid);
                AmountOwed(users,numUsers,targetuserid,exp,numExpenses);
                break;
            case 4:
                printf("Enter userid of the user who needs to pay:");
                scanf("%d",&targetuserid);
                AmountToPay(users,numUsers,targetuserid,exp,numExpenses);
                break;
            case 5:
                UserBalances(users,numUsers,exp,numExpenses);
                break;
            case 6:
                int USER_ID_1,USER_ID_2;
                printf("Enter userid1 and userid2(userid1 is the payer and userid2 is the receiver: ");
                scanf("%d %d",&USER_ID_1,&USER_ID_2);
                SettleUp(users,numUsers,exp,&numExpenses,USER_ID_1,USER_ID_2);
                UserBalances(users,numUsers,exp,numExpenses);
                break;
            case 7:
                int userIDToDelete;
                printf("Enter userid of person with all settles balances: ");
                scanf("%d",&userIDToDelete);
                DeleteUser(users,&numUsers,exp,numExpenses,userIDToDelete);
                break;
            case 8:
                int expenseID,userID;
                printf("Enter expense id and user id who payed for it:");
                scanf("%d %d",expenseID,userID);
                DeleteExpense(exp,&numExpenses,expenseID,userID);
                break;
            default:
                flag=0;
                break;
        }
    }
    printf("Exited the loop");
    return 0;
}