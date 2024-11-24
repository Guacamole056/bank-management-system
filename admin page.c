#include <stdio.h>
#include <string.h>

#define MAX_ACCOUNTS 100
#define LOYALTY_POINTS_RATE 0.1 // 10 points per 100 units deposited

typedef struct {
    int accountNumber;
    char name[50];
    float balance;
    int loyaltyPoints; // Loyalty points field
} Account;

Account accounts[MAX_ACCOUNTS];
int accountCount = 0;

// Function prototypes
void addAccount();
void viewAccounts();
void viewAccountDetails();
void editAccount();
void deleteAccount();
void debitAccount();
void creditAccount();
void adminMenu();
void performDebitOrCredit(Account *account); // Function to allow debit/credit after creating account

int main() {
    adminMenu();
    return 0;
}

// Admin menu function
void adminMenu() {
    int choice;

    do {
        printf("\n==== Admin Page ====\n");
        printf("1. Add New Account\n");
        printf("2. View All Accounts\n");
        printf("3. View Account Details\n");
        printf("4. Edit Account\n");
        printf("5. Delete Account\n");
        printf("6. Debit Account\n");
        printf("7. Credit Account\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addAccount();
                break;
            case 2:
                viewAccounts();
                break;
            case 3:
                viewAccountDetails();
                break;
            case 4:
                editAccount();
                break;
            case 5:
                deleteAccount();
                break;
            case 6:
                debitAccount();
                break;
            case 7:
                creditAccount();
                break;
            case 8:
                printf("Exiting admin page...\n");
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (1);
}

// Add a new account
void addAccount() {
    if (accountCount >= MAX_ACCOUNTS) {
        printf("Maximum account limit reached.\n");
        return;
    }

    Account newAccount;
    printf("Enter account number: ");
    scanf("%d", &newAccount.accountNumber);

    // Consume newline character left by scanf
    getchar();

    printf("Enter name: ");
    fgets(newAccount.name, sizeof(newAccount.name), stdin);
    // Strip newline character from name if present
    newAccount.name[strcspn(newAccount.name, "\n")] = '\0';

    printf("Enter initial balance: ");
    while (scanf("%f", &newAccount.balance) != 1 || newAccount.balance < 0) {
        printf("Invalid input. Please enter a valid positive balance: ");
        while (getchar() != '\n');  // Clear the buffer
    }

    newAccount.loyaltyPoints = 0; // Set initial loyalty points to 0

    accounts[accountCount++] = newAccount;
    printf("Account added successfully!\n");

    // Allow user to perform debit, credit or both
    performDebitOrCredit(&newAccount);
}

// View all accounts
void viewAccounts() {
    if (accountCount == 0) {
        printf("No accounts available.\n");
        return;
    }

    printf("\n==== All Accounts ====\n");
    for (int i = 0; i < accountCount; i++) {
        printf("Account Number: %d, Name: %s, Balance: %.2f, Loyalty Points: %d\n",
               accounts[i].accountNumber, accounts[i].name, accounts[i].balance, accounts[i].loyaltyPoints);
    }
}

// View account details by account number
void viewAccountDetails() {
    int accountNumber, found = 0;
    printf("Enter account number: ");
    scanf("%d", &accountNumber);

    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountNumber == accountNumber) {
            printf("Account Number: %d\n", accounts[i].accountNumber);
            printf("Name: %s\n", accounts[i].name);
            printf("Balance: %.2f\n", accounts[i].balance);
            printf("Loyalty Points: %d\n", accounts[i].loyaltyPoints);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Account not found.\n");
    }
}

// Edit account details
void editAccount() {
    int accountNumber, found = 0;
    printf("Enter account number to edit: ");
    scanf("%d", &accountNumber);

    // Consume newline character left by scanf
    getchar();

    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountNumber == accountNumber) {
            printf("Editing account: %d\n", accounts[i].accountNumber);
            printf("Enter new name: ");
            fgets(accounts[i].name, sizeof(accounts[i].name), stdin);
            // Strip newline character from name if present
            accounts[i].name[strcspn(accounts[i].name, "\n")] = '\0';

            printf("Enter new balance: ");
            while (scanf("%f", &accounts[i].balance) != 1 || accounts[i].balance < 0) {
                printf("Invalid input. Please enter a valid positive balance: ");
                while (getchar() != '\n');  // Clear the buffer
            }

            printf("Account updated successfully!\n");
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Account not found.\n");
    }
}

// Delete an account
void deleteAccount() {
    int accountNumber, found = 0;
    printf("Enter account number to delete: ");
    scanf("%d", &accountNumber);

    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountNumber == accountNumber) {
            for (int j = i; j < accountCount - 1; j++) {
                accounts[j] = accounts[j + 1];
            }
            accountCount--;
            printf("Account deleted successfully!\n");
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Account not found.\n");
    }
}

// Debit account (withdraw money)
void debitAccount() {
    int accountNumber, found = 0;
    float amount;

    printf("Enter account number to debit: ");
    scanf("%d", &accountNumber);

    printf("Enter amount to debit: ");
    while (scanf("%f", &amount) != 1 || amount <= 0) {
        printf("Invalid amount. Please enter a valid positive amount to debit: ");
        while (getchar() != '\n');  // Clear the buffer
    }

    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountNumber == accountNumber) {
            if (accounts[i].balance >= amount) {
                accounts[i].balance -= amount;
                printf("Debit successful. New balance: %.2f\n", accounts[i].balance);
            } else {
                printf("Insufficient balance to debit %.2f\n", amount);
            }
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Account not found.\n");
    }
}

// Credit account (deposit money)
void creditAccount() {
    int accountNumber, found = 0;
    float amount;

    printf("Enter account number to credit: ");
    scanf("%d", &accountNumber);

    printf("Enter amount to credit: ");
    while (scanf("%f", &amount) != 1 || amount <= 0) {
        printf("Invalid amount. Please enter a valid positive amount to credit: ");
        while (getchar() != '\n');  // Clear the buffer
    }

    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountNumber == accountNumber) {
            accounts[i].balance += amount;

            // Loyalty points logic (for every 100 units, give 10 loyalty points)
            int pointsEarned = (int)(amount * LOYALTY_POINTS_RATE);
            accounts[i].loyaltyPoints += pointsEarned;

            printf("Credit successful. New balance: %.2f\n", accounts[i].balance);
            printf("Loyalty Points earned: %d\n", pointsEarned);
            printf("Total Loyalty Points: %d\n", accounts[i].loyaltyPoints);

            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Account not found.\n");
    }
}

// Function to perform debit or credit operations after account is created
void performDebitOrCredit(Account *account) {
    int actionChoice;
    printf("\nWhat would you like to do with this account?\n");
    printf("1. Debit Account\n");
    printf("2. Credit Account\n");
    printf("3. Both Debit and Credit\n");
    printf("4. None\n");
    printf("Enter your choice: ");
    scanf("%d", &actionChoice);

    switch (actionChoice) {
        case 1:
            debitAccount();
            break;
        case 2:
            creditAccount();
            break;
        case 3:
            debitAccount();
            creditAccount();
            break;
        case 4:
            printf("No operations performed on the account.\n");
            break;
        default:
            printf("Invalid choice. No operation performed.\n");
    }
}
