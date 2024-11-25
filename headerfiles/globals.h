#ifndef GLOBALS_H
#define GLOBALS_H

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

// Global variables (defined in one source file, declared as extern in others)
extern User users[MAX_USERS];
extern Account accounts[MAX_ACCOUNTS];
extern int userCount;
extern int accountCount;
extern int nextAccountNumber;

#endif // GLOBALS_H
