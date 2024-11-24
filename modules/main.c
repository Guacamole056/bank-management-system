#include "../header_files/file_ops.h"
#include "../header_files/globals.h"
#include "../header_files/menus.h"

// Global variable definitions
User users[MAX_USERS];
Account accounts[MAX_ACCOUNTS];
int userCount = 0;
int accountCount = 0;
int nextAccountNumber = 1;

int main() {
  // Load users and accounts from file when the program starts
  loadUsersFromFile();
  loadAccountsFromFile();

  // Display the main menu
  mainMenu();

  return 0;
}
