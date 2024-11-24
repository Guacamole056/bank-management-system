#include "../header_files/transactions.h"
#include "../header_files/file_ops.h"

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
    return;
  }

  if (receiver == NULL) {
    printf("\nReceiver account not found.\n");
    return;
  }

  if (!isAdmin && strcmp(sender->ownerUsername, username) != 0) {
    printf("\nYou do not have permission to transfer from this account.\n");
    return;
  }

  if (sender->balance < amount) {
    printf("Insufficient balance in sender account.\n");
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
        }
      } else {
        printf("You do not have permission to debit from this account.\n");
      }
      return;
    }
  }

  printf("Account not found.\n");
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
      }
      return;
    }
  }

  printf("Account not found.\n");
}
