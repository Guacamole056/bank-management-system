#include "../header_files/account_creation.h"
#include "../header_files/file_ops.h"

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
}

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
        adminMenu(username); // Call the admin menu
      } else {
        printf("Redirecting to user menu...\n");
        userMenu(username); // Call the user menu
      }
      return;
    }
  }

  printf("Invalid username or password. Please try again.\n");
}
