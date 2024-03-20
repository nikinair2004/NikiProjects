#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Struct for LPG-list node
struct LPGNode {
    char lpg_subsidy[4]; // Assuming YES or NO
    struct LPGNode* next;
    char name[50];
    char bank_account_number[20];
};

// Structure definition for Bank Account node
typedef struct BankAccountNode {
	char name[50];
    char bank_name[50];
    char account_number[20];
    float deposited_amount;
    struct BankAccountNode* next;
    struct LPGNode* LPG_list;
}BankAccountNode;

// Structure definition for PAN node
struct PANNode {
	char name[50];
    char pan_number[11]; // Assuming PAN number is a string of 10 characters
    struct BankAccountNode* bank_accounts; // Linked list of bank accounts associated with this PAN number
    struct PANNode* next;
};

// Structure definition for Aadhar node
struct AadharNode {
    char name[50];
    char address[100];
    char aadhar_number[13]; // Assuming Aadhar number is a string of 12 digits
    struct PANNode* pan_list; // Linked list of PAN cards associated with this Aadhar number
    struct AadharNode* next;
};

// Function to create a new PAN node
struct PANNode* createPANNode(char pan_number[], char name[]) {
    struct PANNode* newNode = (struct PANNode*)malloc(sizeof(struct PANNode));
    if (newNode == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    strcpy(newNode->pan_number, pan_number);
    strcpy(newNode->name, name);
    newNode->next = NULL;
    return newNode;
}

// Function to create a new Aadhar node
struct AadharNode* createAadharNode(char name[], char address[], char aadhar_number[]) {
    struct AadharNode* newNode = (struct AadharNode*)malloc(sizeof(struct AadharNode));
    if (newNode == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    strcpy(newNode->name, name);
    strcpy(newNode->address, address);
    strcpy(newNode->aadhar_number, aadhar_number);
    newNode->pan_list = NULL;
    newNode->next = NULL;
    return newNode;
}

// Function to insert a new PAN node at the end of the PAN list
void insertPANNode(struct PANNode** head, char pan_number[], char name[]) {
    struct PANNode* newNode = createPANNode(pan_number,name);
    if (*head == NULL) {
        *head = newNode;
    } else {
        struct PANNode* temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
    newNode->bank_accounts = NULL;
}

void insertAadharNode(struct AadharNode** head, char name[], char address[], char aadhar_number[]) {
    struct AadharNode* newNode = createAadharNode(name, address, aadhar_number);
    if (*head == NULL) {
        *head = newNode; // If the list is empty, make the new node as the head
    } else {
        struct AadharNode* temp = *head;
        while (temp->next != NULL) {
            temp = temp->next; // Traverse to the last node
        }
        temp->next = newNode; // Link the new node to the last node
    }
     newNode->pan_list = NULL;
}

// Function to display Aadhar list along with PAN details
void displayAadharList(struct AadharNode* head) {
    struct AadharNode* current = head;
    printf("Aadhar List with PAN Details:\n");
    while (current != NULL) {
        printf("Name: %s\n", current->name);
        printf("Address: %s\n", current->address);
        printf("Aadhar Number: %s\n", current->aadhar_number);
        printf("PAN Numbers:\n");
        struct PANNode* pan_current = current->pan_list;
        while (pan_current != NULL) {
            printf("- %s\n", pan_current->pan_number);
            pan_current = pan_current->next;
        }
        printf("\n");
        current = current->next;
    }
}

// Function to free memory allocated for Aadhar list and associated PAN lists
void freeAadharList(struct AadharNode* head) {
    struct AadharNode* current = head;
    while (current != NULL) {
        struct PANNode* pan_current = current->pan_list;
        while (pan_current != NULL) {
            struct PANNode* temp_Pan = pan_current;
            pan_current = pan_current->next;

            // Free bank account nodes associated with the PAN node
            struct BankAccountNode* bank_current = temp_Pan->bank_accounts;
            while (bank_current != NULL) {
                struct BankAccountNode* bank_temp = bank_current;
                bank_current = bank_current->next;
                free(bank_temp); // Free bank account node
            }

            free(temp_Pan); // Free PAN node
        }
        struct AadharNode* temp = current;
        current = current->next;
        free(temp);
    }
}

struct AadharNode* findAadharNode(struct AadharNode* head, char aadhar_number[]) {
    struct AadharNode* current = head;
    while (current != NULL) {
        if (strcmp(current->aadhar_number, aadhar_number) == 0) {
            return current; // Aadhar node found
        }
        current = current->next;
    }
    return NULL; // Aadhar node not found
}
//display details of person or aadhar without pannumber
void AadharwithoutPANList(struct AadharNode* head) {
    struct AadharNode* current = head;
    printf("People with Aadhar numbers but no PAN numbers:\n");
    while (current != NULL) {
        // If PAN list is empty, print details
        if (current->pan_list == NULL) {
            printf("Name: %s\n", current->name);
            printf("Address: %s\n", current->address);
            printf("Aadhar Number: %s\n\n", current->aadhar_number);
             printf("-----------------------\n");
        }
        current = current->next;
    }
}

// Function to display Aadhar list along with PAN details for people with multiple PAN numbers
void printMultiplePANs(struct AadharNode* head) {
    struct AadharNode* current = head;
    printf("People with multiple PAN numbers:\n");
    while (current != NULL) {
        int panCount = 0;
        struct PANNode* pan_current = current->pan_list;
        while (pan_current != NULL) {
            panCount++;
            pan_current = pan_current->next;
        }
        if (panCount > 1) {
            printf("Name: %s\n", current->name);
            printf("Address: %s\n", current->address);
            printf("Aadhar Number: %s\n", current->aadhar_number);
            printf("PAN Numbers:\n");
            pan_current = current->pan_list;
            while (pan_current != NULL) {
                printf("- %s\n", pan_current->pan_number);
                pan_current = pan_current->next;
            }
            printf("-----------------------\n");
            printf("\n");
        }
        current = current->next;
    }
}

// Function to display Aadhar list along with PAN and bank account details
void displayAadharListWithBankAccounts(struct AadharNode* head) {
    struct AadharNode* currentAadhar = head;
    while (currentAadhar != NULL) {
        printf("Name: %s\n", currentAadhar->name);
        printf("Address: %s\n", currentAadhar->address);
        printf("Aadhar Number: %s\n", currentAadhar->aadhar_number);
        
        struct PANNode* currentPAN = currentAadhar->pan_list;
        while (currentPAN != NULL) {
            printf("  PAN Number: %s\n", currentPAN->pan_number);

            struct BankAccountNode* currentBankAccount = currentPAN->bank_accounts;
            while (currentBankAccount != NULL) {
                printf("    Bank Name: %s\n", currentBankAccount->bank_name);
                printf("    Account Number: %s\n", currentBankAccount->account_number);
                printf("    Deposited Amount: %.2f\n", currentBankAccount->deposited_amount);
                 printf("-----------------------\n");
                currentBankAccount = currentBankAccount->next;
            }

            currentPAN = currentPAN->next;
        }
        
        printf("\n");
        currentAadhar = currentAadhar->next;
    }
}

// Function to find the PAN node based on PAN number
struct PANNode* findPANNode(struct PANNode* head, char pan_number[]) {
    struct PANNode* current = head;
    
     while (current != NULL) {
            if (strcmp(current->pan_number, pan_number) == 0) {
                return current; // PAN node found
            }
            current = current->next;
        }

    return NULL; // PAN node not found
}

struct BankAccountNode* insertBankAccountToPAN(struct AadharNode* head,char name[], char aadhar_number[], char pan_number[], char bankName[], char accNum[], float d) {
    // Find Aadhar node
    struct AadharNode* aadharNode = findAadharNode(head, aadhar_number);
    if (aadharNode == NULL) {
        printf("Aadhar number %s not found.\n", aadhar_number);
        return NULL;
    }

    // Find PAN node in the Aadhar node's PAN list
    struct PANNode* panNode = findPANNode(aadharNode->pan_list, pan_number);
    if (panNode == NULL) {
        printf("PAN number %s not found for Aadhar number %s.\n", pan_number, aadhar_number);
        return NULL;
    }
	printf("*");//ie. no error in finding
    // Insert bank account node at the end of the bank account list of the PAN node
    struct BankAccountNode* newBankAccount = (struct BankAccountNode*)malloc(sizeof(struct BankAccountNode));
    if (newBankAccount == NULL) {
        printf("Memory allocation failed for bank account node.\n");
        exit(1);
    }
    strcpy(newBankAccount->bank_name, bankName);
    strcpy(newBankAccount->account_number, accNum);
    strcpy(newBankAccount->name,name);
    newBankAccount->deposited_amount = d;
    newBankAccount->next = NULL;
    newBankAccount->LPG_list = NULL; // Initialize lpg_list to NULL
//	printf("*");//no error till here too
    if (panNode->bank_accounts == NULL) {
    	//insert in empty linked list
        // If bank account list is empty, make the new bank account as the head
      //  printf("*");
        panNode->bank_accounts = newBankAccount;
       // printf("*");
    } else {
        // Otherwise, traverse the bank account list and insert at the end
        struct BankAccountNode* currentBankAccount = panNode->bank_accounts;
        while (currentBankAccount->next != NULL) {
            currentBankAccount = currentBankAccount->next;
        }
        currentBankAccount->next = newBankAccount;
        //printf("*");
    }

    return newBankAccount;
}

int countBankAccounts(struct BankAccountNode* bankAccounts) {
    int count = 0;
    struct BankAccountNode* currentAccount = bankAccounts;
    
    // Count the number of bank accounts
    while (currentAccount != NULL) {
        count++;
        currentAccount = currentAccount->next;
    }
    
    return count;
}

void printPeopleWithMultipleBankAccounts(struct AadharNode* head) {
    struct AadharNode* currentAadhar = head;
    
    while (currentAadhar != NULL) {
        int multiplePANs = 0; // Counter for multiple PAN numbers
        
        // Iterate through PAN numbers for the current Aadhar node
        struct PANNode* currentPAN = currentAadhar->pan_list;
        while (currentPAN != NULL) {
            int bankAccountCount = countBankAccounts(currentPAN->bank_accounts);
            
            // Check if the count of bank accounts is greater than 1
            if (bankAccountCount > 1) {
                multiplePANs++;
            }
            
            currentPAN = currentPAN->next;
        }
        
        // If the count of PAN numbers with multiple bank accounts is greater than 1
        // Print the details of the person associated with the Aadhar number
        if (multiplePANs > 1) {
            printf("Name: %s\n", currentAadhar->name);
            printf("Address: %s\n", currentAadhar->address);
            printf("Aadhar Number: %s\n", currentAadhar->aadhar_number);
            printf("-----------------------\n");
        }
        
        currentAadhar = currentAadhar->next;
    }
}

// Function to create a new LPG node
struct LPGNode* createLPGNode(char name[], char bank_account_number[], char lpg_subsidy[]) {
    struct LPGNode* newNode = (struct LPGNode*)malloc(sizeof(struct LPGNode));
    if (newNode == NULL) {
        printf("Memory allocation failed for LPG node.\n");
        exit(1);
    }
    strcpy(newNode->name, name);
    strcpy(newNode->bank_account_number, bank_account_number);
    strcpy(newNode->lpg_subsidy, lpg_subsidy);
    newNode->next = NULL;
    return newNode;
}

// Function to insert a new LPG node at the end of the LPG list
void insertLPGNode(struct LPGNode** head, char name[], char bank_account_number[], char lpg_subsidy[]) {
    struct LPGNode* newNode = createLPGNode(name, bank_account_number, lpg_subsidy);
    if (*head == NULL) {
        *head = newNode;
    } else {
        struct LPGNode* temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

void printLPGList(struct AadharNode* head) {
struct AadharNode* currentAadhar = head;

    while (currentAadhar != NULL) {
        printf("Aadhar Number: %s\n", currentAadhar->aadhar_number);
        printf("Name: %s\n", currentAadhar->name);
        printf("Address: %s\n", currentAadhar->address);
        printf("Bank Accounts and LPG Subsidy:\n");
        // Iterate through the PAN nodes of the current Aadhar node
        struct PANNode* currentPAN = currentAadhar->pan_list;
        while (currentPAN != NULL) {
            struct BankAccountNode* currentBankAccount = currentPAN->bank_accounts;
            while (currentBankAccount != NULL) {
                struct LPGNode* currentLPG = currentBankAccount->LPG_list;
                while (currentLPG != NULL) {
                    printf("Bank Account Number: %s, LPG Subsidy: %s\n", currentLPG->bank_account_number, currentLPG->lpg_subsidy);
                    currentLPG = currentLPG->next;
                }
                currentBankAccount = currentBankAccount->next;
            }
            currentPAN = currentPAN->next;
        }
        printf("-----------------------\n");
        currentAadhar = currentAadhar->next;
    }}

void printQualifiedPersons(struct AadharNode* head, float amountX) {
    struct AadharNode* currentAadhar = head;

    while (currentAadhar != NULL) {
        float totalSavings = 0.0;
        int hasLPGSubsidy = 0;

        // Iterate through PAN nodes of current Aadhar node
        struct PANNode* currentPAN = currentAadhar->pan_list;
        while (currentPAN != NULL) {
            // Iterate through bank accounts of current PAN node
            struct BankAccountNode* currentBankAccount = currentPAN->bank_accounts;
            while (currentBankAccount != NULL) {
                totalSavings += currentBankAccount->deposited_amount;
                
                // Check if the person has availed LPG subsidy
                struct LPGNode* currentLPG = currentBankAccount->LPG_list;
                while (currentLPG != NULL) {
                    if (strcmp(currentLPG->lpg_subsidy, "YES") == 0) {
                        hasLPGSubsidy = 1;
                        break;
                    }
                    currentLPG = currentLPG->next;
                }

                currentBankAccount = currentBankAccount->next;
            }

            currentPAN = currentPAN->next;
        }

        // Check if total savings are greater than amount X and the person has availed LPG subsidy
        if (totalSavings > amountX && hasLPGSubsidy) {
            printf("Name: %s\n", currentAadhar->name);
            printf("Address: %s\n", currentAadhar->address);
            printf("Aadhar Number: %s\n", currentAadhar->aadhar_number);
            printf("-----------------------\n");
        }

        currentAadhar = currentAadhar->next;
    }
}

// Function to print inconsistent data
void printInconsistentData(struct AadharNode* head) {
    struct AadharNode* currentAadhar = head;

    while (currentAadhar != NULL) {
        char aadharName[100];
        strcpy(aadharName, currentAadhar->name);

        // Check inconsistency in PAN numbers
        struct PANNode* currentPAN = currentAadhar->pan_list;
        while (currentPAN != NULL) {
            if (strcmp(currentPAN->name, aadharName) != 0) {
                printf("Inconsistent data for Aadhar number %s:\n", currentAadhar->aadhar_number);
                printf("Name in PAN number %s: %s\n", currentPAN->pan_number, currentPAN->name);
                printf("Name in Aadhar: %s\n", aadharName);
                printf("Address: %s\n", currentAadhar->address);
                printf("-----------------------\n");
                break;  // No need to check further PAN numbers
            }
            currentPAN = currentPAN->next;
        }

        // Check inconsistency in bank accounts
        struct PANNode* currentPANBank = currentAadhar->pan_list;
        while (currentPANBank != NULL) {
            struct BankAccountNode* currentBankAccount = currentPANBank->bank_accounts;
            while (currentBankAccount != NULL) {
                if (strcmp(currentBankAccount->name, aadharName) != 0) {
                    printf("Inconsistent data for Aadhar number %s:\n", currentAadhar->aadhar_number);
                    printf("Name in Bank Account: %s\n", currentBankAccount->name);
                    printf("Name in Aadhar: %s\n", aadharName);
                    printf("Address: %s\n", currentAadhar->address);
                    printf("-----------------------\n");
                    break;  // No need to check further bank accounts
                }
                currentBankAccount = currentBankAccount->next;
            }
            currentPANBank = currentPANBank->next;
        }

        // Check inconsistency in LPG connection
        struct PANNode* currentPANLPG = currentAadhar->pan_list;
        while (currentPANLPG != NULL) {
            struct BankAccountNode* currentBankAccountLPG = currentPANLPG->bank_accounts;
            while (currentBankAccountLPG != NULL) {
                struct LPGNode* currentLPG = currentBankAccountLPG->LPG_list;
                while (currentLPG != NULL) {
                    if (strcmp(currentLPG->name, aadharName) != 0) {
                        printf("Inconsistent data for Aadhar number %s:\n", currentAadhar->aadhar_number);
                        printf("Name in LPG Connection: %s\n", currentLPG->name);
                        printf("Name in Aadhar: %s\n", aadharName);
                        printf("Address: %s\n", currentAadhar->address);
                        printf("-----------------------\n");
                        break;  // No need to check further LPG connections
                    }
                    currentLPG = currentLPG->next;
                }
                currentBankAccountLPG = currentBankAccountLPG->next;
            }
            currentPANLPG = currentPANLPG->next;
        }

        currentAadhar = currentAadhar->next;
    }
}

BankAccountNode* mergeBankLists(BankAccountNode *list1, BankAccountNode *list2) {
    if (list1 == NULL) {
        return list2; // If the first list is empty, return the second list
    }

    // Traverse to the end of the first list
    BankAccountNode *current = list1;
    while (current->next != NULL) {
        current = current->next;
    }

    // Append the second list to the end of the first list
    current->next = list2;

    return list1; // Return the merged list
}

// Function to print the merged bank account list
void printBankList(BankAccountNode *list) {
    printf("Merged Bank Account List:\n");
    while (list != NULL) {
        printf("Name: %s\n", list->name);
        printf("Bank Name: %s\n", list->bank_name);
        printf("Account Number: %s\n", list->account_number);
        printf("Deposited Amount: %.2f\n\n", list->deposited_amount);
        list = list->next;
    }
}

BankAccountNode* createBankNode(char *name, char *bank_name, char *account_number, float deposited_amount) {
    BankAccountNode *newNode = (BankAccountNode*)malloc(sizeof(BankAccountNode));
    if (newNode == NULL) {
        printf("Memory allocation failed\n");
        return NULL;
    }

    strcpy(newNode->name, name);
    strcpy(newNode->bank_name, bank_name);
    strcpy(newNode->account_number, account_number);
    newNode->deposited_amount = deposited_amount;
    newNode->LPG_list = NULL;
    newNode->next = NULL;
	
    return newNode;
}

void insertNode(BankAccountNode **head, BankAccountNode *newNode) {
    if (*head == NULL) {
        *head = newNode;
        return;
    }

    BankAccountNode *current = *head;
    while (current->next != NULL) {
        current = current->next;
    }

    current->next = newNode;
}

int main() {
    // Initialize Aadhar list
    struct AadharNode* aadharList = NULL;
    FILE *file = fopen("dta.txt", "r"); // Open the file in read mode
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    char name[50];
    char address[100];
    char aadharNumber[14];

    // Read and parse each line of the file
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%[^,],%[^,],%s", name, address, aadharNumber);
        insertAadharNode(&aadharList, name, address,aadharNumber );
//        printf("Name: %s\n", name);
//        printf("Address: %s\n", address);
//        printf("Aadhar Number: %s\n", aadharNumber);
//        printf("\n");
    }

    fclose(file); // Close the file
	//insert pan number
	struct AadharNode* JohnDoeAadhar = findAadharNode(aadharList, "123456789012");
    if (JohnDoeAadhar != NULL) {
        insertPANNode(&(JohnDoeAadhar->pan_list), "JOHNPAN1","JohnDoe");
    } else {
        printf("John's Aadhar not found.\n");
    }
    
	struct AadharNode* AliceAadhar = findAadharNode(aadharList, "987654321098");
    if (AliceAadhar != NULL) {
        insertPANNode(&(AliceAadhar->pan_list), "ALICEPAN1","Alice");
    } else {
        printf("Alice's Aadhar not found.\n");
    }
    
	JohnDoeAadhar = findAadharNode(aadharList, "123456789012");
	if (JohnDoeAadhar != NULL) {
        insertPANNode(&(JohnDoeAadhar->pan_list), "JOHNPAN2","John");
    } else {
        printf("John's Aadhar not found.\n");
    }
    
//   	struct AadharNode* KimAadhar = findAadharNode(aadharList, "245364728293");
//   	if (KimAadhar != NULL) {
//        insertPANNode(&(KimAadhar->pan_list), "KIMPAN1","Kim");
//    } else {
//        printf("Kim's Aadhar not found.\n");
//    }
    
    struct AadharNode* BobAadhar = findAadharNode(aadharList, "876543210987");
   	if (BobAadhar != NULL) {
        insertPANNode(&(BobAadhar->pan_list), "BOBPAN1","Bob");
    } else {
        printf("Bob's Aadhar not found.\n");
    }
     BobAadhar = findAadharNode(aadharList, "876543210987");
   	if (BobAadhar != NULL) {
        insertPANNode(&(BobAadhar->pan_list), "BOBPAN2","Bob");
    } else {
        printf("Bob's Aadhar not found.\n");
    }
    
    // Display Aadhar list along with PAN details
    displayAadharList(aadharList);
	printf("que1)\n");
	AadharwithoutPANList(aadharList);
	printf("que2)\n");
	printMultiplePANs(aadharList);
	  BankAccountNode* bankList1=insertBankAccountToPAN(aadharList,"John", "123456789012", "JOHNPAN1", "Bank X", "1111111111", 5000.00);
  	  BankAccountNode* bankList2=insertBankAccountToPAN(aadharList,"JohnDoe", "123456789012", "JOHNPAN1", "Bank Y", "2222222222", 6000.00);
  	  BankAccountNode* bankList3=insertBankAccountToPAN(aadharList,"Alice", "987654321098", "ALICEPAN1", "Bank Z", "3333333333", 7000.00);
	  BankAccountNode* bankList4=insertBankAccountToPAN(aadharList,"JohnDoe","123456789012", "JOHNPAN2", "Bank A", "10000000000", 500.00);
  	  BankAccountNode* bankList5=insertBankAccountToPAN(aadharList,"JohnDoe","123456789012", "JOHNPAN2", "Bank B", "20000000000", 600.00);
  	  BankAccountNode* bankList6=insertBankAccountToPAN(aadharList,"Bob","876543210987", "BOBPAN1", "Bank W", "4444444444", 550.00);
  	  BankAccountNode* bankList7=insertBankAccountToPAN(aadharList,"Bob", "876543210987", "BOBPAN1", "Bank Z", "255555555555", 650.00);
  	  BankAccountNode* bankList8=insertBankAccountToPAN(aadharList,"Bobby","876543210987", "BOBPAN2", "Bank A", "444444444410", 550.00);
  	  BankAccountNode* bankList9=insertBankAccountToPAN(aadharList,"Bobby","876543210987", "BOBPAN2", "Bank B", "235555555555", 650.00);
	printf("info till bankaccount:\n");
	displayAadharListWithBankAccounts(aadharList);
	printf("Que3)\n");
	printPeopleWithMultipleBankAccounts(aadharList);
    // Free memory allocated for Aadhar list and associated PAN lists
	// Find Aadhar node
	struct AadharNode* aadharNode = findAadharNode(aadharList,"987654321098");
	if (aadharNode != NULL) {
	    // Find the bank account associated with Alice
	    struct PANNode* panNode = aadharNode->pan_list;
	    while (panNode != NULL) {
	        struct BankAccountNode* bankAccount = panNode->bank_accounts;
	        while (bankAccount != NULL) {
	            if (strcmp(bankAccount->account_number, "3333333333") == 0) {
	                // Found the bank account, now insert LPG node
	                insertLPGNode(&(bankAccount->LPG_list), "Aali", "3333333333", "YES");
	                break;
	            }
			 	//just insert multiple if statements to add several LPG nodes
	            bankAccount = bankAccount->next;
	        }
	        panNode = panNode->next;
	    }
	}
	//resume from here
	printf("Que4)\n");
	printf("Contents of the LPG list:\n");
    printLPGList(aadharList);
    printf("Que5)\n");
    printQualifiedPersons(aadharList,1000);
    printf("Que6)\n");
    printInconsistentData(aadharList);
//    freeAadharList(aadharList);
//	free(bankList1);
//    free(bankList2);
//    free(bankList3);
//    free(bankList4);
//    free(bankList5);
//    free(bankList6);
//    free(bankList7);
//    free(bankList8);
//    free(bankList9);
    printf("Que7)\n");
    BankAccountNode *list1 = (BankAccountNode*)malloc(sizeof(BankAccountNode));
    list1=NULL;//BankAccountNode *list1 ;
   	insertNode(&list1, createBankNode("John", "Bank of America", "1234567890", 1000.0));
    insertNode(&list1, createBankNode("Alice", "Chase Bank", "9876543210", 2000.0));
    insertNode(&list1, createBankNode("Bob", "Wells Fargo", "5678901234", 1500.0));
      BankAccountNode *list2 = (BankAccountNode*)malloc(sizeof(BankAccountNode));	
	// Insert some nodes into the second list
	list2=NULL;
	insertNode(&list2, createBankNode("Michael", "Citibank", "1357924680", 3000.0));
	insertNode(&list2, createBankNode("Emma", "HSBC", "0246813579", 2500.0));
	insertNode(&list2, createBankNode("Sophia", "Barclays", "9876543210", 4000.0));
    // Merge the two lists
    BankAccountNode *mergedList = mergeBankLists(list1, list2);
	printf("*");
    // Print the merged list
    printBankList(mergedList);
    freeAadharList(aadharList);
	free(bankList1);
    free(bankList2);
    free(bankList3);
    free(bankList4);
    free(bankList5);
    free(bankList6);
    free(bankList7);
    free(bankList8);
    free(bankList9);
    free(list1);
    free(list2);
    free(mergedList);
    return 0;
}