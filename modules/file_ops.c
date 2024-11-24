#include "../header_files/file_ops.h"

// Implementations of file handling functions

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
