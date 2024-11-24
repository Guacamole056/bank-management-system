#include "../header_files/account_management.h"
#include "../header_files/file_ops.h"

// Function to view bank accounts
void viewAccounts(char *username, int isAdmin) {
  int i;
  if (accountCount == 0) {
    printf("No accounts found.\n");
    return;
  }

  printf("\n+-----------------------------+\n");
  printf("|      Existing Accounts      |\n");
  printf("+-----------------------------+\n");

  // Display accounts based on user privileges
  for (i = 0; i < accountCount; i++) {
    if (isAdmin || strcmp(accounts[i].ownerUsername, username) == 0) {
      printf("Account Number: %d, Name: %s, Balance: USD %.2f, Loyalty Points: "
             "%d\n",
             accounts[i].accountNumber, accounts[i].name, accounts[i].balance,
             accounts[i].loyaltyPoints);
    }
  }
}

// Function to view the details of a specific account
void viewAccountDetails(char *username, int isAdmin) {
  int accountNumber, i, found = 0;

  printf("\n+-----------------------------+\n");
  printf("|     View Account Details    |\n");
  printf("+-----------------------------+\n");

  printf("Enter account number to view: ");
  scanf("%d", &accountNumber);
  getchar(); // Consume newline

  for (i = 0; i < accountCount; i++) {
    if (accounts[i].accountNumber == accountNumber) {
      if (isAdmin || strcmp(accounts[i].ownerUsername, username) == 0) {
        printf("\n+-----------------------------+\n");
        printf("|     Account Information     |\n");
        printf("+-----------------------------+\n");
        printf("Account Number : %d\n", accounts[i].accountNumber);
        printf("Name           : %s\n", accounts[i].name);
        printf("Balance        : USD %.2f\n", accounts[i].balance);
        printf("Loyalty Points : %d\n", accounts[i].loyaltyPoints);
        printf("Owner Username : %s\n", accounts[i].ownerUsername);
        found = 1;
      } else {
        printf("You do not have permission to view this account.\n");
      }
      break;
    }
  }

  if (!found) {
    printf("Account not found.\n");
  }
}

// Function to edit an account
void editAccount(char *username, int isAdmin) {
  int accountNumber, i, found = 0;

  printf("\n+-----------------------------+\n");
  printf("|         Edit Account         |\n");
  printf("+-----------------------------+\n");

  printf("\nEnter account number to edit: ");
  scanf("%d", &accountNumber);
  getchar(); // Consume newline

  for (i = 0; i < accountCount; i++) {
    if (accounts[i].accountNumber == accountNumber) {
      if (isAdmin || strcmp(accounts[i].ownerUsername, username) == 0) {
        printf("Edit Name: ");
        fgets(accounts[i].name, sizeof(accounts[i].name), stdin);
        accounts[i].name[strcspn(accounts[i].name, "\n")] =
            '\0'; // Remove newline

        // Save accounts to file
        saveAccountsToFile();

        printf("Account updated.\n");
        found = 1;
      } else {
        printf("You do not have permission to edit this account.\n");
      }
      break;
    }
  }

  if (!found) {
    printf("Account not found.\n");
  }
}

// Function to delete an account
void deleteAccount(char *username, int isAdmin) {
  int accountNumber, i, found = 0;

  printf("\n+-----------------------------+\n");
  printf("|        Delete Account        |\n");
  printf("+-----------------------------+\n");

  printf("\nEnter account number to delete: ");
  scanf("%d", &accountNumber);
  getchar(); // Consume newline

  for (i = 0; i < accountCount; i++) {
    if (accounts[i].accountNumber == accountNumber) {
      if (isAdmin || strcmp(accounts[i].ownerUsername, username) == 0) {
        for (int j = i; j < accountCount - 1; j++) {
          accounts[j] = accounts[j + 1];
        }
        accountCount--;

        // Save accounts to file
        saveAccountsToFile();

        printf("Account deleted successfully.\n");
        found = 1;
      } else {
        printf("You do not have permission to delete this account.\n");
      }
      break;
    }
  }

  if (!found) {
    printf("Account not found.\n");
  }
}

// Function to show all existing users
void showUsers() {
  int i, j;

  if (userCount == 0) {
    printf("No users found.\n");
    return;
  }

  printf("\n+=================================+\n");
  printf("|          Existing Users         |\n");
  printf("+=================================+\n");

  // Display each user and whether they have an account
  for (i = 0; i < userCount; i++) {
    int hasAccount = 0;
    // Check if the user has at least one account
    for (j = 0; j < accountCount; j++) {
      if (strcmp(accounts[j].ownerUsername, users[i].username) == 0) {
        hasAccount = 1;
        break;
      }
    }
    printf("Username: %s, Has Account: %s\n", users[i].username,
           hasAccount ? "Yes" : "No");
  }
}
