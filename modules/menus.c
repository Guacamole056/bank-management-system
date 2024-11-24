#include "../header_files/menus.h"
#include "../header_files/account_management.h"
#include "../header_files/balance_inquiry.h"
#include "../header_files/file_ops.h"
#include "../header_files/transactions.h"

void mainMenu() {
  int choice;

  do {
    printf("\n+=================================+\n");
    printf("|             Main Menu           |\n");
    printf("+=================================+\n");
    printf("| 1. Login                        |\n");
    printf("| 2. Create New Account           |\n");
    printf("| 3. Exit                         |\n");
    printf("+=================================+\n");

    printf("\nEnter your choice: ");
    scanf("%d", &choice);
    getchar(); // Consume newline after input

    switch (choice) {
    case 1:
      login(); // User login
      break;
    case 2:
      createAccount(); // Create new user account
      break;
    case 3:
      printf("Exiting system...\n");
      break;
    default:
      printf("Invalid choice. Please try again.\n");
    }
  } while (choice != 3);

  // Save users and accounts before exiting
  saveUsersToFile();
  saveAccountsToFile();
}

// Admin menu to manage bank accounts
void adminMenu(char *username) {
  int choice;

  do {
    printf("\n+=================================+\n");
    printf("|           Admin Menu            |\n");
    printf("+=================================+\n");
    printf("| 1. Add New Account              |\n");
    printf("| 2. View All Accounts            |\n");
    printf("| 3. View Account Details         |\n");
    printf("| 4. Edit Account                 |\n");
    printf("| 5. Delete Account               |\n");
    printf("| 6. Debit Account                |\n");
    printf("| 7. Credit Account               |\n");
    printf("| 8. Show All Users               |\n");
    printf("| 9. Logout                       |\n");
    printf("+=================================+\n");

    printf("\nEnter your choice: ");
    scanf("%d", &choice);
    getchar(); // Consume newline

    switch (choice) {
    case 1:
      addAccount(username); // Add a new account
      break;
    case 2:
      viewAccounts(username, 1); // Admin can view all accounts
      break;
    case 3:
      viewAccountDetails(username, 1); // Admin can view any account
      break;
    case 4:
      editAccount(username, 1); // Admin can edit any account
      break;
    case 5:
      deleteAccount(username, 1); // Admin can delete any account
      break;
    case 6:
      debitAccount(username, 1); // Admin can debit any account
      break;
    case 7:
      creditAccount(username, 1); // Admin can credit any account
      break;
    case 8:
      showUsers(); // Show all users
      break;
    case 9:
      printf("Logging out...\n");
      return;
    default:
      printf("Invalid choice. Please try again.\n");
    }
  } while (1);
}

void userMenu(char *username) {
  int choice;

  do {
    printf("\n+=================================+\n");
    printf("|            User Menu            |\n");
    printf("+=================================+\n");
    printf("| 1. View Account Balance         |\n");
    printf("| 2. Transfer Funds               |\n");
    printf("| 3. Logout                       |\n");
    printf("+=================================+\n");

    printf("\nEnter your choice: ");
    scanf("%d", &choice);
    getchar(); // Consume newline

    switch (choice) {
    case 1:
      balanceInquiry(username); // View own account balance
      break;
    case 2:
      fundTransfer(username, 0); // User can transfer from their own account
      break;
    case 3:
      printf("Logging out...\n");
      return; // Exit the user menu
    default:
      printf("Invalid choice. Please try again.\n");
    }
  } while (1);
}
