#ifndef TRANSACTIONS_H
#define TRANSACTIONS_H

#include "globals.h"

// Function prototypes
void fundTransfer(char *username, int isAdmin);
void debitAccount(char *username, int isAdmin);
void creditAccount(char *username, int isAdmin);

#endif // TRANSACTIONS_H
