#include <stdio.h>
#include <string.h>

#define MAX_ACCOUNTS 100
#define LOYALTY_POINTS_RATE 0.1 // 10 points per 100 units deposited

typedef struct {
    int accountNumber;
    char name[50];
    float balance;
    int loyaltyPoints;
} Account;

Account accounts[MAX_ACCOUNTS];
int accountCount = 0;

void addAccount();
void viewAccounts();
void editAccount();
void deleteAccount();
void debitAccount();
void creditAccount();
void adminMenu();
void performTransaction(Account *account);
Account* lookupAccount(int accountNumber);
float getValidAmount();

int main() {
    adminMenu();
    return 0;
}

void adminMenu() {
    int choice;
    do {
        printf("\n==== Admin Page ====\n");
        printf("1. Add Account\n");
        printf("2. View Accounts\n");
        printf("3. Edit Account\n");
        printf("4. Delete Account\n");
        printf("5. Debit Account\n");
        printf("6. Credit Account\n");
        printf("7. Exit\n");
        printf("\nEnter choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1: addAccount(); break;
            case 2: viewAccounts(); break;
            case 3: editAccount(); break;
            case 4: deleteAccount(); break;
            case 5: debitAccount(); break;
            case 6: creditAccount(); break;
            case 7: printf("Exiting...\n"); return;
            default: printf("Invalid choice. Please try again.\n");
        }
    } while (1);
}

void addAccount() {
    if (accountCount >= MAX_ACCOUNTS) { 
        printf("\nAccount limit reached.\n");
        return; 
    }

    Account newAccount;
    printf("\nEnter account number: ");
    scanf("%d", &newAccount.accountNumber);
    getchar(); // Consume newline
    
    printf("Enter account holder's name: ");
    fgets(newAccount.name, sizeof(newAccount.name), stdin);
    newAccount.name[strcspn(newAccount.name, "\n")] = '\0';  // Remove newline
    
    printf("Enter initial balance: ");
    while (scanf("%f", &newAccount.balance) != 1 || newAccount.balance < 0) { 
        printf("Invalid balance. Please enter a valid amount: ");
        while (getchar() != '\n');
    }
    
    newAccount.loyaltyPoints = 0;  // No points initially
    
    // Store the account
    accounts[accountCount++] = newAccount;
    printf("\nAccount added successfully.\n");

    // Ask if they want to perform a transaction
    int choice;
    printf("\nWould you like to perform a transaction?\n");
    printf("1. Debit Account\n");
    printf("2. Credit Account\n");
    printf("3. Both Debit and Credit\n");
    printf("4. No transaction\n");
    printf("\nEnter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1: debitAccount(); break;
        case 2: creditAccount(); break;
        case 3: debitAccount(); creditAccount(); break;
        case 4: printf("No transaction performed.\n"); break;
        default: printf("Invalid choice. No transaction performed.\n"); break;
    }
}

void viewAccounts() {
    if (accountCount == 0) { 
        printf("\nNo accounts found.\n"); 
        return; 
    }
    
    printf("\n=== Account List ===\n");
    for (int i = 0; i < accountCount; i++) {
        printf("\nAccount Number: %d\n", accounts[i].accountNumber);
        printf("Account Holder: %s\n", accounts[i].name);
        printf("Balance: %.2f\n", accounts[i].balance);
        printf("Loyalty Points: %d\n", accounts[i].loyaltyPoints);
        printf("=====================\n");
    }
}

void editAccount() {
    int accountNumber;
    printf("\nEnter account number to edit: ");
    scanf("%d", &accountNumber);
    
    Account* account = lookupAccount(accountNumber);
    if (account) {
        printf("\nEditing account: %d\n", account->accountNumber);
        getchar();  // Consume newline
        
        printf("New name: "); 
        fgets(account->name, sizeof(account->name), stdin);
        account->name[strcspn(account->name, "\n")] = '\0';  // Remove newline
        
        printf("New balance: ");
        while (scanf("%f", &account->balance) != 1 || account->balance < 0) { 
            printf("Invalid balance. Try again: "); 
            while (getchar() != '\n'); 
        }
        printf("\nAccount updated successfully.\n");
    } else {
        printf("\nAccount not found.\n");
    }
}

void deleteAccount() {
    int accountNumber;
    printf("\nEnter account number to delete: "); 
    scanf("%d", &accountNumber);
    
    Account* account = lookupAccount(accountNumber);
    if (account) {
        // Shift remaining accounts to delete
        for (int i = account - accounts; i < accountCount - 1; i++) {
            accounts[i] = accounts[i + 1];
        }
        accountCount--;
        printf("\nAccount deleted successfully.\n");
    } else {
        printf("\nAccount not found.\n");
    }
}

void debitAccount() {
    int accountNumber;
    float amount;
    printf("\nEnter account number to debit: ");
    scanf("%d", &accountNumber);
    
    printf("Enter amount to debit: ");
    amount = getValidAmount();
    
    Account* account = lookupAccount(accountNumber);
    if (account) {
        if (account->balance >= amount) {
            account->balance -= amount;
            printf("\nDebit successful. New balance: %.2f\n", account->balance);
        } else {
            printf("\nInsufficient balance for debit.\n");
        }
    } else {
        printf("\nAccount not found.\n");
    }
}

void creditAccount() {
    int accountNumber; 
    float amount;
    printf("\nEnter account number to credit: ");
    scanf("%d", &accountNumber);
    
    printf("Enter amount to credit: ");
    amount = getValidAmount();
    
    Account* account = lookupAccount(accountNumber);
    if (account) {
        account->balance += amount;
        int pointsEarned = (int)(amount * LOYALTY_POINTS_RATE);
        account->loyaltyPoints += pointsEarned;
        printf("\nCredit successful. New balance: %.2f\n", account->balance);
        printf("Loyalty points earned: %d\n", pointsEarned);
        printf("Total points: %d\n", account->loyaltyPoints);
    } else {
        printf("\nAccount not found.\n");
    }
}

void performTransaction(Account *account) {
    int choice;
    printf("\n1. Debit Account\n2. Credit Account\n3. No transaction\nEnter choice: ");
    scanf("%d", &choice);
    if (choice == 1) debitAccount();
    else if (choice == 2) creditAccount();
    else printf("No transaction performed.\n");
}

Account* lookupAccount(int accountNumber) {
    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountNumber == accountNumber) {
            return &accounts[i];
        }
    }
    return NULL;
}

float getValidAmount() {
    float amount;
    while (scanf("%f", &amount) != 1 || amount <= 0) {
        printf("Invalid amount. Try again: ");
        while (getchar() != '\n');
    }
    return amount;
}
