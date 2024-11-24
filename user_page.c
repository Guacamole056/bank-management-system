#include <stdio.h>
#include <string.h>

void menu();
int login();
void inputPassword(char *password, int showPassword);

int main() {
    printf("Welcome to the Bank Management System\n");

    if (login()) {
        printf("\nLogin successful!\n");
        menu();
    } else {
        printf("\nAccess denied! Exiting...\n");
    }

    return 0;
}

int login() {
    char username[20], password[20];
    const char validUsername[] = "admin";
    const char validPassword[] = "12345";
    int attempts = 3, showPassword;

    while (attempts > 0) {
        printf("\nLogin Page\n");
        printf("Enter Username: ");
        scanf("%s", username);

        // Clear the input buffer before taking password input
        while (getchar() != '\n'); // Consume the leftover newline

        printf("Show Password? (1 for Yes, 0 for No): ");
        scanf("%d", &showPassword);

        // Clear the buffer after reading the integer input (showPassword)
        while (getchar() != '\n'); // Consume the leftover newline

        printf("Enter Password: ");
        inputPassword(password, showPassword);

        if (strcmp(username, validUsername) == 0 && strcmp(password, validPassword) == 0) {
            return 1; // Successful login
        } else {
            attempts--;
            printf("\nInvalid credentials. You have %d attempt(s) left.\n", attempts);
        }
    }

    return 0; // Failed login after 3 attempts
}

void inputPassword(char *password, int showPassword) {
    int i = 0;
    char ch;

    while ((ch = getchar()) != '\n' && ch != EOF) {
        if (ch == '\r') {  // Enter key (optional)
            break;
        }
        if (ch == '\b' && i > 0) {  // Backspace key handling
            i--;
            printf("\b \b");  // erase the last character
            continue;
        }
        if (i < 19) {  // Limit password length
            password[i++] = ch;
            if (showPassword) {
                putchar(ch);  // Show character
            } else {
                putchar('*');  // Mask with asterisk
            }
        }
    }
    password[i] = '\0';  // Null-terminate the password string
}

void menu() {
    int choice;

    do {
        printf("\n\n=== Main Menu ===\n");
        printf("1. View Account Details\n");
        printf("2. Deposit Money\n");
        printf("3. Withdraw Money\n");
        printf("4. Transfer Money\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("\nViewing Account Details...\n");
                break;
            case 2:
                printf("\nDepositing Money...\n");
                break;
            case 3:
                printf("\nWithdrawing Money...\n");
                break;
            case 4:
                printf("\nTransferring Money...\n");
                break;
            case 5:
                printf("\nExiting...\n");
                break;
            default:
                printf("\nInvalid choice. Please try again.\n");
        }
    } while (choice != 5);
}
