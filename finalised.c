#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERS 100
#define USERNAME_LENGTH 30
#define PASSWORD_LENGTH 30
#define SHIFT 3 // Shift for Caesar cipher to encrypt password
#define MAX_ACCOUNTS 100
#define LOYALTY_POINTS_RATE 0.1 // 10 points per 100 units deposited
#define FILENAME "users.txt"

// User structure to store login information (username and encrypted password)
typedef struct {
  char username[USERNAME_LENGTH];
  char password[PASSWORD_LENGTH];
} User;

// Account structure to store bank account details (balance, loyalty points,
// etc.)
typedef struct {
  int accountNumber;
  char name[50];
  float balance;
  int loyaltyPoints; // Loyalty points for the account
} Account;

User users[MAX_USERS];          // Array to store user accounts
Account accounts[MAX_ACCOUNTS]; // Array to store bank accounts
int userCount = 0;              // Counter to track number of users
int accountCount = 0;           // Counter to track number of bank accounts

// Function prototypes for various functionalities
void encryptPassword(const char *password, char *encryptedPassword);
void loadUsersFromFile();  // Loads users from file into the system
void saveUsersToFile();    // Saves the current user data to the file
void createAccount();      // Function to create a new user account
void showUsers();          // Displays all users (with encrypted passwords)
void login();              // Function for user login
void adminMenu();          // Admin menu for managing bank accounts
void userMenu();           // User menu for regular users
void addAccount();         // Function to add a new bank account
void viewAccounts();       // Function to view all bank accounts
void viewAccountDetails(); // Function to view details of a specific account
void editAccount();        // Function to edit a bank account
void deleteAccount();      // Function to delete a bank account
void debitAccount();       // Function to debit from a bank account
void creditAccount();      // Function to credit to a bank account
void fundTransfer(Account *accounts, int totalAccounts, int senderAccount,
                  int receiverAccount,
                  float amount); // Function to transfer funds between accounts
void balanceInquiry(Account *accounts, int totalAccounts,
                    int accountNumber);      // Function to view account balance
void performDebitOrCredit(Account *account); // Allow debit/credit to an account
void menu(); // Main menu to navigate the application

// Caesar cipher function to encrypt the password
void encryptPassword(const char *password, char *encryptedPassword) {
  int i;
  // Shift each character of the password by the defined SHIFT amount
  for (i = 0; password[i] != '\0' && i < PASSWORD_LENGTH; i++) {
    encryptedPassword[i] = password[i] + SHIFT; // Apply Caesar cipher shift
  }
  encryptedPassword[strlen(password)] =
      '\0'; // Null-terminate the encrypted password
}

// Function to load users from a file into the system
void loadUsersFromFile() {
  FILE *file = fopen(FILENAME, "r"); // Open the file for reading
  if (file == NULL) {
    printf("No existing user file found. Starting fresh.\n");
    return;
  }

  // Read each user from the file and store it in the users array
  while (fscanf(file, "%s %s", users[userCount].username,
                users[userCount].password) == 2) {
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

  // Write each user's username and encrypted password to the file
  for (i = 0; i < userCount; i++) {
    fprintf(file, "%s %s\n", users[i].username, users[i].password);
  }

  fclose(file); // Close the file after writing
}

// Function to create a new user account
void createAccount() {
  int i;

  if (userCount >= MAX_USERS) {
    printf("Maximum user limit reached. Cannot create more accounts.\n");
    return;
  }

  User newUser;

  printf("Enter username: ");
  if (fgets(newUser.username, sizeof(newUser.username), stdin) != NULL) {
    // Removing newline from the username input
    size_t len = strlen(newUser.username);
    if (len > 0 && newUser.username[len - 1] == '\n') {
      newUser.username[len - 1] = '\0';
    }
  } else {
    printf("Error reading username.\n");
    return;
  }

  // Check if the username already exists
  for (i = 0; i < userCount; i++) {
    if (strcmp(users[i].username, newUser.username) == 0) {
      printf("Username already exists. Please choose a different username.\n");
      return;
    }
  }

  printf("Enter password: ");
  if (fgets(newUser.password, sizeof(newUser.password), stdin) != NULL) {
    // Removing newline from the password input
    size_t len = strlen(newUser.password);
    if (len > 0 && newUser.password[len - 1] == '\n') {
      newUser.password[len - 1] = '\0';
    }
  } else {
    printf("Error reading password.\n");
    return;
  }

  // Encrypt the password before storing it
  char encryptedPassword[PASSWORD_LENGTH];
  encryptPassword(newUser.password, encryptedPassword);
  strcpy(newUser.password, encryptedPassword);

  // Store the new user in the users array
  users[userCount] = newUser;
  userCount++;

  // Save the new user data to the file
  saveUsersToFile();

  printf("Account created successfully!\n");

  char choice;
  printf("Do you want to create a bank account as well? (y/n): ");
  scanf(" %c", &choice);

  if (choice == 'y' || choice == 'Y') {
    if (accountCount >= MAX_ACCOUNTS) {
      printf(
          "Maximum bank account limit reached. Cannot create more accounts.\n");
      return;
    }

    Account newAccount;
    newAccount.accountNumber = accountCount + 1;
    strcpy(newAccount.name, newUser.username);

    printf("Enter initial deposit: ");
    scanf("%f", &newAccount.balance);

    newAccount.loyaltyPoints = newAccount.balance * LOYALTY_POINTS_RATE;

    accounts[accountCount] = newAccount;
    accountCount++;

    printf("Bank account created successfully!\n");
    printf("Account Number: %d, Name: %s, Balance: %.2f, Loyalty Points: %d\n",
           newAccount.accountNumber, newAccount.name, newAccount.balance,
           newAccount.loyaltyPoints);
  }
}

// Function to show all existing users with encrypted passwords
void showUsers() {
  int i;

  if (userCount == 0) {
    printf("No users found.\n");
    return;
  }

  printf("\n=============== Existing Users ===============\n");
  // Display each user with their encrypted password
  for (i = 0; i < userCount; i++) {
    printf("Username: %s, Encrypted Password: %s\n", users[i].username,
           users[i].password);
  }
  printf("==============================================\n");
}

// Function to login a user by matching the username and password
void login() {
  int i;
  char username[USERNAME_LENGTH];
  char password[PASSWORD_LENGTH];
  char encryptedPassword[PASSWORD_LENGTH];

  printf("Enter username: ");
  if (fgets(username, sizeof(username), stdin) != NULL) {
    size_t len = strlen(username);
    if (len > 0 && username[len - 1] == '\n') {
      username[len - 1] = '\0';
    }
  } else {
    printf("Error reading username.\n");
    return;
  }

  printf("Enter password: ");
  if (fgets(password, sizeof(password), stdin) != NULL) {
    size_t len = strlen(password);
    if (len > 0 && password[len - 1] == '\n') {
      password[len - 1] = '\0';
    }
  } else {
    printf("Error reading password.\n");
    return;
  }

  // Encrypt the entered password and check it against the stored encrypted
  // password
  encryptPassword(password, encryptedPassword);

  for (i = 0; i < userCount; i++) {
    if (strcmp(users[i].username, username) == 0 &&
        strcmp(users[i].password, encryptedPassword) == 0) {
      printf("Login successful! Welcome, %s.\n", username);

      if (strcmp(username, "admin") == 0) {
        printf("Redirecting to admin menu...\n");
        adminMenu(); // Call the admin menu
      } else {
        printf("Redirecting to user menu...\n");
        userMenu(); // Call a user menu function
      }
      return;
    }
  }

  printf("Invalid username or password. Please try again.\n");
}

// Admin menu to manage bank accounts
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
    printf("8. Fund Transfer\n");
    printf("9. Exit\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
    case 1:
      addAccount(); // Call to add a new account
      break;
    case 2:
      viewAccounts(); // Call to view all accounts
      break;
    case 3:
      viewAccountDetails(); // Call to view details of a specific account
      break;
    case 4:
      editAccount(); // Call to edit an existing account
      break;
    case 5:
      deleteAccount(); // Call to delete an account
      break;
    case 6:
      debitAccount(); // Call to debit from an account
      break;
    case 7:
      creditAccount(); // Call to credit an account
      break;
    case 8:
      fundTransfer(accounts, accountCount, 1001, 1002,
                   500); // Example fund transfer
      break;
    case 9:
      printf("Exiting admin page...\n");
      return;
    default:
      printf("Invalid choice. Please try again.\n");
    }
  } while (1);
}

void userMenu() {
  int choice;

  do {
    printf("\n==== User Menu ====\n");
    printf("1. View Account Balance\n");
    printf("2. Transfer Funds\n");
    printf("3. Logout\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
    case 1:
      // Functionality to view account balance
      printf("Enter your account number: ");
      int accountNumber;
      scanf("%d", &accountNumber);
      balanceInquiry(accounts, accountCount, accountNumber);
      break;
    case 2:
      // Functionality to transfer funds
      printf("Enter sender account number: ");
      int senderAccount;
      scanf("%d", &senderAccount);
      printf("Enter receiver account number: ");
      int receiverAccount;
      scanf("%d", &receiverAccount);
      printf("Enter amount to transfer: ");
      float amount;
      scanf("%f", &amount);
      fundTransfer(accounts, accountCount, senderAccount, receiverAccount,
                   amount);
      break;
    case 3:
      printf("Logging out...\n");
      return; // Exit the user menu
    default:
      printf("Invalid choice. Please try again.\n");
    }
  } while (1);
}

// Function to add a new bank account
void addAccount() {
  Account newAccount;
  newAccount.accountNumber = accountCount + 1; // Assign a unique account number
  printf("Enter account holder's name: ");
  getchar(); // Consume newline left over by previous input
  fgets(newAccount.name, sizeof(newAccount.name), stdin);
  newAccount.name[strcspn(newAccount.name, "\n")] = '\0'; // Remove newline

  printf("Enter initial deposit: ");
  scanf("%f", &newAccount.balance);

  // Set loyalty points based on the deposit amount
  newAccount.loyaltyPoints = newAccount.balance * LOYALTY_POINTS_RATE;

  // Store the new account in the accounts array
  accounts[accountCount] = newAccount;
  accountCount++;

  printf("Account created successfully!\n");
}

// Function to view all accounts
void viewAccounts() {
  int i;
  if (accountCount == 0) {
    printf("No accounts found.\n");
    return;
  }

  printf("\nExisting Accounts:\n");
  // Display each account with basic details
  for (i = 0; i < accountCount; i++) {
    printf("Account Number: %d, Name: %s, Balance: %.2f, Loyalty Points: %d\n",
           accounts[i].accountNumber, accounts[i].name, accounts[i].balance,
           accounts[i].loyaltyPoints);
  }
}

// Function to view the details of a specific account
void viewAccountDetails() {
  int accountNumber, i, found = 0;
  printf("Enter account number to view: ");
  scanf("%d", &accountNumber);

  for (i = 0; i < accountCount; i++) {
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

// Function to edit an account
void editAccount() {
  int accountNumber, i, found = 0;
  printf("Enter account number to edit: ");
  scanf("%d", &accountNumber);

  for (i = 0; i < accountCount; i++) {
    if (accounts[i].accountNumber == accountNumber) {
      printf("Edit Name: ");
      getchar(); // Consume newline
      fgets(accounts[i].name, sizeof(accounts[i].name), stdin);
      accounts[i].name[strcspn(accounts[i].name, "\n")] =
          '\0'; // Remove newline

      printf("Edit Balance: ");
      scanf("%f", &accounts[i].balance);
      printf("Account updated.\n");
      found = 1;
      break;
    }
  }

  if (!found) {
    printf("Account not found.\n");
  }
}

// Function to delete an account
void deleteAccount() {
  int accountNumber, i, found = 0;
  printf("Enter account number to delete: ");
  scanf("%d", &accountNumber);

  for (i = 0; i < accountCount; i++) {
    if (accounts[i].accountNumber == accountNumber) {
      for (int j = i; j < accountCount - 1; j++) {
        accounts[j] = accounts[j + 1];
      }
      accountCount--;
      printf("Account deleted successfully.\n");
      found = 1;
      break;
    }
  }

  if (!found) {
    printf("Account not found.\n");
  }
}

// Function to debit from an account
void debitAccount() {
  int accountNumber;
  float amount;
  printf("Enter account number to debit: ");
  scanf("%d", &accountNumber);
  printf("Enter amount to debit: ");
  scanf("%f", &amount);

  for (int i = 0; i < accountCount; i++) {
    if (accounts[i].accountNumber == accountNumber) {
      if (accounts[i].balance >= amount) {
        accounts[i].balance -= amount;
        printf("Amount debited successfully. New balance: %.2f\n",
               accounts[i].balance);
      } else {
        printf("Insufficient balance.\n");
      }
      return;
    }
  }

  printf("Account not found.\n");
}

// Function to credit an account
void creditAccount() {
  int accountNumber;
  float amount;
  printf("Enter account number to credit: ");
  scanf("%d", &accountNumber);
  printf("Enter amount to credit: ");
  scanf("%f", &amount);

  for (int i = 0; i < accountCount; i++) {
    if (accounts[i].accountNumber == accountNumber) {
      accounts[i].balance += amount;
      printf("Amount credited successfully. New balance: %.2f\n",
             accounts[i].balance);
      return;
    }
  }

  printf("Account not found.\n");
}

// Function to transfer funds between two accounts
void fundTransfer(Account *accounts, int totalAccounts, int senderAccount,
                  int receiverAccount, float amount) {
  int i;
  Account *sender = NULL, *receiver = NULL;

  // Find the sender and receiver accounts
  for (i = 0; i < totalAccounts; i++) {
    if (accounts[i].accountNumber == senderAccount) {
      sender = &accounts[i];
    }
    if (accounts[i].accountNumber == receiverAccount) {
      receiver = &accounts[i];
    }
  }

  // Check if both accounts exist and if the sender has sufficient balance
  if (sender == NULL) {
    printf("Sender account not found.\n");
    return;
  }

  if (receiver == NULL) {
    printf("Receiver account not found.\n");
    return;
  }

  if (sender->balance < amount) {
    printf("Insufficient balance in sender account.\n");
    return;
  }

  // Perform the transfer
  sender->balance -= amount;
  receiver->balance += amount;

  printf("Transfer successful! New balances:\n");
  printf("Sender (Account %d): %.2f\n", sender->accountNumber, sender->balance);
  printf("Receiver (Account %d): %.2f\n", receiver->accountNumber,
         receiver->balance);
}

// Main function to display the menu and manage the entire application
int main() {
  int choice;

  // Load users from file when the program starts
  loadUsersFromFile();

  do {
    printf("\n==== Main Menu ======\n");
    printf("1. Login\n");
    printf("2. Create New Account\n");
    printf("3. Show All Users\n");
    printf("4. Exit\n");
    printf("=====================\n");

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
      showUsers(); // Show all users
      break;
    case 4:
      printf("Exiting system...\n");
      break;
    default:
      printf("Invalid choice. Please try again.\n");
    }
  } while (choice != 4);

  return 0;
}
