#ifndef ACCOUNT_MANAGEMENT_H
#define ACCOUNT_MANAGEMENT_H

#include "globals.h"

// Function prototypes
void viewAccounts(char *username, int isAdmin);
void viewAccountDetails(char *username, int isAdmin);
void editAccount(char *username, int isAdmin);
void deleteAccount(char *username, int isAdmin);
void showUsers();

#endif // ACCOUNT_MANAGEMENT_H
