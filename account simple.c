#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERS 100
#define USERNAME_LENGTH 30
#define PASSWORD_LENGTH 30
#define SHIFT 3  // Shift for Caesar cipher
#define FILENAME "users.txt"

typedef struct {
    char username[USERNAME_LENGTH];
    char password[PASSWORD_LENGTH];
} User;

User  users[MAX_USERS];
int userCount = 0;

// Function to encrypt the password using a VERY simple Caesar cipher
void encryptPassword(const char *password, char *encryptedPassword) {
	int i;
    for (i = 0; password[i] != '\0' && i < PASSWORD_LENGTH; i++) {
        encryptedPassword[i] = password[i] + SHIFT;  // shift function for the cipher
    }
    encryptedPassword[strlen(password)] = '\0'; //removing null values

// Function to load the users from a file
void loadUsersFromFile() {
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        printf("No existing user file found. Starting fresh.\n");
        return;
    }

    while (fscanf(file, "%s %s", users[userCount].username, users[userCount].password) == 2) {
        userCount++;
    }

    fclose(file);
}

// Function to save the created users to a file (same file as the one where users are loaded from)
void saveUsersToFile() {
	int i;
    FILE *file = fopen(FILENAME, "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    for (i = 0; i < userCount; i++) {
        fprintf(file, "%s %s\n", users[i].username, users[i].password);
    }

    fclose(file);
}

// Function to create an account
void createAccount() {
	int i;
    if (userCount >= MAX_USERS) {
        printf("Maximum user limit reached. Cannot create more accounts.\n");
        return;
    }

    User newUser ;

    printf("Enter username: ");
    if (fgets(newUser .username, sizeof(newUser .username), stdin) != NULL) {
        // Removing newlines
        size_t len = strlen(newUser .username);
        if (len > 0 && newUser .username[len - 1] == '\n') {
            newUser .username[len - 1] = '\0';
        }
    } else {
        printf("Error reading username.\n");
        return;
    }

    // Checking whether the username already exists
    for (i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, newUser .username) == 0) {
            printf("Username already exists. Please choose a different username.\n");
            return;
        }
    }

    printf("Enter password: ");
    if (fgets(newUser .password, sizeof(newUser .password), stdin) != NULL) {
        // Removing newline again (standard)
        size_t len = strlen(newUser .password);
        if (len > 0 && newUser .password[len - 1] == '\n') {
            newUser .password[len - 1] = '\0';
        }
    } else {
        printf("Error reading password.\n");
        return;
    }

    // Only the encrypted password is stored to prevent security breach 
    char encryptedPassword[PASSWORD_LENGTH];
    encryptPassword(newUser .password, encryptedPassword);
    strcpy(newUser .password, encryptedPassword);

    // Save the new user to the user array
    users[userCount] = newUser ;
    userCount++;

    // Save the users to the file
    saveUsersToFile();

    printf("Account created successfully!\n");
}

// Function to display existing users with their pass (encrypted)
void showUsers() {
	int i;
    if (userCount == 0) {
        printf("No users found.\n");
        return;
    }

    printf("\nExisting Users:\n");
    for (i = 0; i < userCount; i++) {
        printf("Username: %s, Encrypted Password: %s\n", users[i].username, users[i].password);
    }
}

// Function to login a user
void login() {
	int i;
    char username[USERNAME_LENGTH];
    char password[PASSWORD_LENGTH];
    char encryptedPassword[PASSWORD_LENGTH];

    printf("Enter username: ");
    if (fgets(username, sizeof(username), stdin) != NULL) {
        // Removing newline
        size_t len = strlen(username);
        if (len > 0 && username[len - 1] == '\n') {
            username[len - 1] = '\0';
        }
    } else {
       return printf("Error reading username.");
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

    // Encrypting the password
    encryptPassword(password, encryptedPassword);

    // Check if the username and encrypted password match any stored user
    for (i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, encryptedPassword) == 0) {
            printf("Login successful! Welcome, %s.\n", username);
            return;
        }
    }

    printf("Invalid username or password. Please try again.\n");
}

int main() {
    int choice;

    // Load existing users from file at the start
    loadUsersFromFile();

    while (1) {
        printf("\nBank Management System\n");
        printf("1. Create Account\n");
        printf("2. Show Existing Users\n");
        printf("3. Login\n");
        printf("4. Exit\n");
        printf("Choose an option: ");
        
        // Use scanf to read the user's choice
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            // Clear the input buffer
            while (getchar() != '\n');
            continue;
        }
        
        // Clear the input buffer after reading the user's choice
        while (getchar() != '\n');

        switch (choice) {
            case 1:
                createAccount();
                break;
            case 2:
                showUsers();
                break;
            case 3:
                login();
                break;
            case 4:
                printf("Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}

