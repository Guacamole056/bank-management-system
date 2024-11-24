#include "../header_files/balance_inquiry.h"

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
  }
}
