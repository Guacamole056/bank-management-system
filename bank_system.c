#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERS 100
#define USERNAME_LENGTH 30
#define PASSWORD_LENGTH 30
#define MAX_ACCOUNTS 100
#define LOYALTY_POINTS_RATE 0.1 // 10 points per 100 units deposited
#define FILENAME "users.txt"
#define ACCOUNT_FILENAME "accounts.txt"

// User structure to store login information (username and password)
typedef struct {
  char username[USERNAME_LENGTH];
  char password[PASSWORD_LENGTH];
  int isAdmin; // 1 for admin, 0 for regular user
} User;

// Account structure to store bank account details (balance, loyalty points,
// etc.)
typedef struct {
  int accountNumber;
  char name[50];
  float balance;
  int loyaltyPoints;                   // Loyalty points for the account
  char ownerUsername[USERNAME_LENGTH]; // Username of the account owner
} Account;

User users[MAX_USERS];          // Array to store user accounts
Account accounts[MAX_ACCOUNTS]; // Array to store bank accounts
int userCount = 0;              // Counter to track number of users
int accountCount = 0;           // Counter to track number of bank accounts
int nextAccountNumber = 1;      // Next available account number

// Function prototypes for various functionalities
void loadUsersFromFile();        // Loads users from file into the system
void saveUsersToFile();          // Saves the current user data to the file
void loadAccountsFromFile();     // Loads accounts from file into the system
void saveAccountsToFile();       // Saves the current account data to the file
void createAccount();            // Function to create a new user account
void login();                    // Function for user login
void adminMenu(char *username);  // Admin menu for managing bank accounts
void userMenu(char *username);   // User menu for regular users
void addAccount(char *username); // Function to add a new bank account
void viewAccounts(char *username,
                  int isAdmin); // Function to view bank accounts
void viewAccountDetails(char *username,
                        int isAdmin); // Function to view account details
void editAccount(char *username,
                 int isAdmin); // Function to edit a bank account
void deleteAccount(char *username,
                   int isAdmin); // Function to delete a bank account
void debitAccount(char *username,
                  int isAdmin); // Function to debit from a bank account
void creditAccount(char *username,
                   int isAdmin); // Function to credit to a bank account
void fundTransfer(char *username,
                  int isAdmin); // Function to transfer funds between accounts
void balanceInquiry(char *username); // Function to view account balance
void showUsers();                    // Function to display all users
void pauseScreen(); // Function to pause the screen and clear it after user
                    // input

// Function to load users from a file into the system
void loadUsersFromFile() {
  FILE *file = fopen(FILENAME, "r"); // Open the file for reading
  if (file == NULL) {
    printf("Error: User file not found. Please ensure '%s' exists with the "
           "admin account.\n",
           FILENAME);
    exit(1); // Exit the program if the user file is missing
  }

  // Read each user from the file and store it in the users array
  while (fscanf(file, "%s %s %d", users[userCount].username,
                users[userCount].password, &users[userCount].isAdmin) == 3) {
    userCount++;
  }

  fclose(file); // Close the file after reading
}

// Function to save users' data to the file
void saveUsersToFile() {
  int i;
  FILE *file = fopen(FILENAME, "w"); // Open the file for writing
  if (file == NULL) {
    printf("Error opening file for writing.\n");
    return;
  }

  // Write each user's username, password, and isAdmin flag to the file
  for (i = 0; i < userCount; i++) {
    fprintf(file, "%s %s %d\n", users[i].username, users[i].password,
            users[i].isAdmin);
  }

  fclose(file); // Close the file after writing
}

// Function to load accounts from a file into the system
void loadAccountsFromFile() {
  FILE *file = fopen(ACCOUNT_FILENAME, "r"); // Open the file for reading
  if (file == NULL) {
    printf("No existing account file found.\n");
    return;
  }

  int maxAccountNumber = 0;

  // Read each account from the file and store it in the accounts array
  while (fscanf(file, "%d %49s %f %d %s", &accounts[accountCount].accountNumber,
                accounts[accountCount].name, &accounts[accountCount].balance,
                &accounts[accountCount].loyaltyPoints,
                accounts[accountCount].ownerUsername) == 5) {

    // Update maxAccountNumber if the current account number is higher
    if (accounts[accountCount].accountNumber > maxAccountNumber) {
      maxAccountNumber = accounts[accountCount].accountNumber;
    }

    accountCount++;
  }

  fclose(file); // Close the file after reading

  // Update nextAccountNumber to be one greater than the max account number
  nextAccountNumber = maxAccountNumber + 1;
}

// Function to save accounts' data to the file
void saveAccountsToFile() {
  int i;
  FILE *file = fopen(ACCOUNT_FILENAME, "w"); // Open the file for writing
  if (file == NULL) {
    printf("Error opening account file for writing.\n");
    return;
  }

  // Write each account's data to the file
  for (i = 0; i < accountCount; i++) {
    fprintf(file, "%d %s %f %d %s\n", accounts[i].accountNumber,
            accounts[i].name, accounts[i].balance, accounts[i].loyaltyPoints,
            accounts[i].ownerUsername);
  }

  fclose(file); // Close the file after writing
}

// Function to create a new user account (regular users only)
void createAccount() {
  int i;

  if (userCount >= MAX_USERS) {
    printf("Maximum user limit reached. Cannot create more accounts.\n");
    return;
  }

  User newUser;

  printf("\n+-----------------------------+\n");
  printf("|       Create Account        |\n");
  printf("+-----------------------------+\n");

  printf("\nEnter username: ");
  fgets(newUser.username, sizeof(newUser.username), stdin);
  newUser.username[strcspn(newUser.username, "\n")] = '\0'; // Remove newline

  // Check if the username already exists
  for (i = 0; i < userCount; i++) {
    if (strcmp(users[i].username, newUser.username) == 0) {
      printf("Username already exists. Please choose a different username.\n");
      pauseScreen(); // Pause before returning
      return;
    }
  }

  printf("Enter password: ");
  fgets(newUser.password, sizeof(newUser.password), stdin);
  newUser.password[strcspn(newUser.password, "\n")] = '\0'; // Remove newline

  // Initialize isAdmin to 0 for regular users
  newUser.isAdmin = 0;

  // Store the new user in the users array
  users[userCount] = newUser;
  userCount++;

  // Save the new user data to the file
  saveUsersToFile();

  printf("Account created successfully!\n");
  pauseScreen(); // Pause before returning
}

// Function to login a user by matching the username and password
void login() {
  int i;
  char username[USERNAME_LENGTH];
  char password[PASSWORD_LENGTH];

  printf("\n+-----------------------------+\n");
  printf("|            Login            |\n");
  printf("+-----------------------------+\n");

  printf("\nEnter username: ");
  fgets(username, sizeof(username), stdin);
  username[strcspn(username, "\n")] = '\0'; // Remove newline

  printf("Enter password: ");
  fgets(password, sizeof(password), stdin);
  password[strcspn(password, "\n")] = '\0'; // Remove newline

  // Check the entered username and password against stored data
  for (i = 0; i < userCount; i++) {
    if (strcmp(users[i].username, username) == 0 &&
        strcmp(users[i].password, password) == 0) {
      printf("\nLogin successful! Welcome, %s.\n", username);

      if (users[i].isAdmin) {
        printf("Redirecting to admin menu...\n");

        pauseScreen();       // Pause before returning
        adminMenu(username); // Call the admin menu
      } else {
        printf("Redirecting to user menu...\n");

        pauseScreen();      // Pause before returning
        userMenu(username); // Call the user menu
      }
      return;
    }
  }

  printf("Invalid username or password. Please try again.\n");
  pauseScreen(); // Pause before returning
}

// Admin menu to manage bank accounts
void adminMenu(char *username) {
  int choice;

  do {
    system("cls");

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
    while (getchar() != '\n')
      ; // Clear the input buffer

    switch (choice) {
    case 1:
      addAccount(username); // Call to add a new account
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
      pauseScreen(); // Pause before re-displaying the menu
    }
  } while (1);
}

// User menu for regular users
void userMenu(char *username) {
  int choice;

  do {
    system("cls");

    printf("\n+=================================+\n");
    printf("|            User Menu            |\n");
    printf("+=================================+\n");
    printf("| 1. View Account Balance         |\n");
    printf("| 2. Transfer Funds               |\n");
    printf("| 3. Logout                       |\n");
    printf("+=================================+\n");

    printf("\nEnter your choice: ");
    scanf("%d", &choice);
    while (getchar() != '\n')
      ; // Clear the input buffer

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
      pauseScreen(); // Pause before re-displaying the menu
    }
  } while (1);
}

// Function to add a new bank account
// Function to add a new bank account
void addAccount(char *username) {
  if (accountCount >= MAX_ACCOUNTS) {
    printf("Maximum account limit reached.\n");
    return;
  }

  Account newAccount;
  newAccount.accountNumber =
      nextAccountNumber++; // Assign a unique account number

  // Prompt admin for the account holder's username
  char accountOwnerUsername[USERNAME_LENGTH];
  printf("\n+-----------------------------+\n");
  printf("|        Add New Account      |\n");
  printf("+-----------------------------+\n");

  printf("\nEnter account holder's username: ");
  fgets(accountOwnerUsername, sizeof(accountOwnerUsername), stdin);
  accountOwnerUsername[strcspn(accountOwnerUsername, "\n")] =
      '\0'; // Remove newline

  // Check if the entered username exists
  int userExists = 0;
  for (int i = 0; i < userCount; i++) {
    if (strcmp(users[i].username, accountOwnerUsername) == 0) {
      userExists = 1;
      break;
    }
  }

  if (!userExists) {
    printf("User '%s' does not exist. Cannot create account.\n",
           accountOwnerUsername);
    printf("\nPress Enter to continue...");
    while (getchar() != '\n')
      ; // Wait for user input
    system("cls");

    return;
  }

  printf("Enter account holder's name: ");
  fgets(newAccount.name, sizeof(newAccount.name), stdin);
  newAccount.name[strcspn(newAccount.name, "\n")] = '\0'; // Remove newline

  printf("Enter initial deposit: USD ");
  scanf("%f", &newAccount.balance);
  getchar(); // Consume newline

  // Set loyalty points based on the deposit amount
  newAccount.loyaltyPoints = (int)(newAccount.balance * LOYALTY_POINTS_RATE);

  // Associate the account with the entered username
  strcpy(newAccount.ownerUsername, accountOwnerUsername);

  // Store the new account in the accounts array
  accounts[accountCount] = newAccount;
  accountCount++;

  // Save accounts to file
  saveAccountsToFile();

  printf("Account created successfully! Account Number: %d\n",
         newAccount.accountNumber);

  pauseScreen(); // Pause before returning
}

// Function to view bank accounts
void viewAccounts(char *username, int isAdmin) {
  int i;
  if (accountCount == 0) {
    printf("No accounts found.\n");
    pauseScreen(); // Pause before returning
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
  pauseScreen(); // Pause before returning
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

  pauseScreen(); // Pause before returning
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
    pauseScreen(); // Pause before returning
  }

  pauseScreen(); // Pause before returning
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
    pauseScreen(); // Pause before returning
  }

  pauseScreen(); // Pause before returning
}

// Function to debit from an account
void debitAccount(char *username, int isAdmin) {
  int accountNumber;
  float amount;

  printf("\n+-----------------------------+\n");
  printf("|         Debit Account        |\n");
  printf("+-----------------------------+\n");

  printf("\nEnter account number to debit: ");
  scanf("%d", &accountNumber);
  getchar(); // Consume newline

  printf("Enter amount to debit: USD ");
  scanf("%f", &amount);
  getchar(); // Consume newline

  if (amount <= 0) {
    printf("Invalid amount.\n");
    return;
  }

  for (int i = 0; i < accountCount; i++) {
    if (accounts[i].accountNumber == accountNumber) {
      if (isAdmin || strcmp(accounts[i].ownerUsername, username) == 0) {
        if (accounts[i].balance >= amount) {
          accounts[i].balance -= amount;

          // Update loyalty points
          accounts[i].loyaltyPoints =
              (int)(accounts[i].balance * LOYALTY_POINTS_RATE);

          // Save accounts to file
          saveAccountsToFile();

          printf("Amount debited successfully. New balance: USD %.2f\n",
                 accounts[i].balance);
        } else {
          printf("Insufficient balance.\n");
          printf("\nPress Enter to continue...");
          while (getchar() != '\n')
            ; // Wait for user input
          system("cls");
        }
      } else {
        printf("You do not have permission to debit from this account.\n");
      }
      return;
    }

    printf("\nPress Enter to continue...");
    while (getchar() != '\n')
      ; // Wait for user input
    system("cls");
  }

  printf("Account not found.\n");
  pauseScreen(); // Pause before returning
}

// Function to credit an account
void creditAccount(char *username, int isAdmin) {
  int accountNumber;
  float amount;

  printf("\n+-----------------------------+\n");
  printf("|        Credit Account        |\n");
  printf("+-----------------------------+\n");

  printf("\nEnter account number to credit: ");
  scanf("%d", &accountNumber);
  getchar(); // Consume newline

  printf("Enter amount to credit: USD ");
  scanf("%f", &amount);
  getchar(); // Consume newline

  if (amount <= 0) {
    printf("Invalid amount.\n");
    return;
  }

  for (int i = 0; i < accountCount; i++) {
    if (accounts[i].accountNumber == accountNumber) {
      if (isAdmin || strcmp(accounts[i].ownerUsername, username) == 0) {
        accounts[i].balance += amount;

        // Update loyalty points
        accounts[i].loyaltyPoints =
            (int)(accounts[i].balance * LOYALTY_POINTS_RATE);

        // Save accounts to file
        saveAccountsToFile();

        printf("Amount credited successfully. New balance: USD %.2f\n",
               accounts[i].balance);
      } else {
        printf("You do not have permission to credit this account.\n");
        printf("\nPress Enter to continue...");
        while (getchar() != '\n')
          ; // Wait for user input
        system("cls");
      }
      return;
    }
  }

  printf("Account not found.\n");
  pauseScreen(); // Pause before returning
}

// Function to transfer funds between accounts
void fundTransfer(char *username, int isAdmin) {
  int senderAccountNumber, receiverAccountNumber;
  float amount;

  printf("\n+-----------------------------+\n");
  printf("|        Fund Transfer         |\n");
  printf("+-----------------------------+\n");

  printf("\nEnter sender account number: ");
  scanf("%d", &senderAccountNumber);
  getchar(); // Consume newline

  printf("Enter receiver account number: ");
  scanf("%d", &receiverAccountNumber);
  getchar(); // Consume newline

  printf("Enter amount to transfer: USD ");
  scanf("%f", &amount);
  getchar(); // Consume newline

  if (amount <= 0) {
    printf("Invalid amount.\n");
    return;
  }

  Account *sender = NULL, *receiver = NULL;

  // Find the sender and receiver accounts
  for (int i = 0; i < accountCount; i++) {
    if (accounts[i].accountNumber == senderAccountNumber) {
      sender = &accounts[i];
    }
    if (accounts[i].accountNumber == receiverAccountNumber) {
      receiver = &accounts[i];
    }
  }

  // Check if both accounts exist and if the user has permission
  if (sender == NULL) {
    printf("\nSender account not found.\n");
    pauseScreen(); // Pause before returning
    return;
  }

  if (receiver == NULL) {
    printf("\nReceiver account not found.\n");
    pauseScreen(); // Pause before returning
    return;
  }

  if (!isAdmin && strcmp(sender->ownerUsername, username) != 0) {
    printf("\nYou do not have permission to transfer from this account.\n");
    pauseScreen(); // Pause before returning
    return;
  }

  if (sender->balance < amount) {
    printf("Insufficient balance in sender account.\n");
    pauseScreen(); // Pause before returning
    return;
  }

  // Perform the transfer
  sender->balance -= amount;
  receiver->balance += amount;

  // Update loyalty points
  sender->loyaltyPoints = (int)(sender->balance * LOYALTY_POINTS_RATE);
  receiver->loyaltyPoints = (int)(receiver->balance * LOYALTY_POINTS_RATE);

  // Save accounts to file
  saveAccountsToFile();

  printf("Transfer successful! New balance: USD %.2f\n", sender->balance);

  pauseScreen(); // Pause before returning
}

// Function to inquire balance for the user's account
void balanceInquiry(char *username) {
  int i, found = 0;

  printf("\n+-----------------------------+\n");
  printf("|       Account Balance       |\n");
  printf("+-----------------------------+\n");

  for (i = 0; i < accountCount; i++) {
    if (strcmp(accounts[i].ownerUsername, username) == 0) {
      printf("\nAccount Number : %d\n", accounts[i].accountNumber);
      printf("Name           : %s\n", accounts[i].name);
      printf("Balance        : USD %.2f\n", accounts[i].balance);
      printf("Loyalty Points : %d\n", accounts[i].loyaltyPoints);
      found = 1;
    }
  }

  if (!found) {
    printf("\nYou have no accounts.\n");
    pauseScreen(); // Pause before returning
  }

  pauseScreen(); // Pause before returning
}

// Function to show all existing users
void showUsers() {
  int i, j;

  if (userCount == 0) {
    printf("No users found.\n");
    printf("\nPress Enter to continue...");
    while (getchar() != '\n')
      ; // Wait for user input

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

  pauseScreen(); // Pause before returning
}

// Function to pause the screen and clear it after user input
void pauseScreen() {
  printf("\nPress Enter to continue...");
  while (getchar() != '\n')
    ; // Consume any leftover input

  // Clear the screen after the user presses Enter
  system("cls");
}

// Main function to display the menu and manage the entire application
int main() {
  int choice;

  // Load users and accounts from file when the program starts
  loadUsersFromFile();
  loadAccountsFromFile();

  do {
    printf("\n+=================================+\n");
    printf("|            Main Menu            |\n");
    printf("+=================================+\n");
    printf("| 1. Login                        |\n");
    printf("| 2. Create New Account           |\n");
    printf("| 3. Exit                         |\n");
    printf("+=================================+\n");

    printf("\nEnter your choice: ");
    scanf("%d", &choice);
    while (getchar() != '\n')
      ; // Clear the input buffer

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
      pauseScreen(); // Pause before re-displaying the menu
    }
  } while (choice != 3);

  // Save users and accounts before exiting
  saveUsersToFile();
  saveAccountsToFile();

  return 0;
}
