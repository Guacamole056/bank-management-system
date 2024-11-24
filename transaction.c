struct Account {
  int accountNumber;
  char name[50];
  float balance;
};

int main() {
  struct Account accounts[10] = {{1001, "Alice", 5000.0},
                                 {1002, "Bob", 3000.0}};
  int totalAccounts = 2;
  int senderAccount, receiverAccount, amount;

  printf("Enter your Account Number:");
  scanf("%d", &senderAccount);

  printf("Enter the Account Number of the receiver:");
  scanf("%d", &receiverAccount);

  printf("Enter the amount you want to transfer:");
  scanf("%d", &amount);

  printf("\nFund Transfer:\n");
  fundTransfer(accounts, totalAccounts, senderAccount, receiverAccount, amount);

  printf("Balance Inquiry:\n");
  balanceInquiry(accounts, totalAccounts, 1001);
}

void fundTransfer(struct Account accounts[], int totalAccounts,
                  int senderAccount, int receiverAccount, float amount) {
  int senderIndex = -1, receiverIndex = -1;

  // Find sender and receiver accounts
  for (int i = 0; i < totalAccounts; i++) {
    if (accounts[i].accountNumber == senderAccount)
      senderIndex = i;
    if (accounts[i].accountNumber == receiverAccount)
      receiverIndex = i;
  }

  if (senderIndex == -1) {
    printf("Sender account not found!\n");
    return;
  }
  if (receiverIndex == -1) {
    printf("Receiver account not found!\n");
    return;
  }

  // Check balance
  if (accounts[senderIndex].balance < amount) {
    printf("Insufficient balance!\n");
    return;
  }

  // Perform transfer
  accounts[senderIndex].balance -= amount;
  accounts[receiverIndex].balance += amount;

  printf("Transfer successful!\n");
  printf("Sender's New Balance: $%.2f\n", accounts[senderIndex].balance);
  printf("Receiver's New Balance: $%.2f\n", accounts[receiverIndex].balance);
}

void balanceInquiry(struct Account accounts[], int totalAccounts,
                    int accountNumber) {
  for (int i = 0; i < totalAccounts; i++) {
    if (accounts[i].accountNumber == accountNumber) {
      printf("Account Number: %d\n", accounts[i].accountNumber);
      printf("Name: %s\n", accounts[i].name);
      printf("Balance: $%.2f\n", accounts[i].balance);
      return;
    }
  }
  printf("Account not found!\n");
  int senderAccount, receiverAccount, amount;
}
