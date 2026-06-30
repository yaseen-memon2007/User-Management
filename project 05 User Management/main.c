#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>

#define FILE_NAME "users.txt"
#define MAX_LEN 50

#define COLOR_RESET   7
#define COLOR_GREEN   10
#define COLOR_RED     12
#define COLOR_BLUE    9
#define COLOR_YELLOW  14

typedef struct
{
    char username[MAX_LEN];
    char password[MAX_LEN];
} User;

void set_color(int color);
void clear_screen();
void show_menu();
void register_user();
void login_user();
int username_exists(const char *username);
void read_password(char *password);

int main()
{
    int choice;

    while (1)
    {
        clear_screen();
        show_menu();

        printf("Select an option (Write just 1 or 2 or 3): ");
        if (scanf("%d", &choice) != 1)
        {
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n'); 

        switch (choice)
        {
            case 1:
                clear_screen();
                register_user();
                break;
            case 2:
                clear_screen();
                login_user();
                break;
            case 3:
                set_color(COLOR_YELLOW);
                printf("\nExiting the program. Goodbye!\n");
                set_color(COLOR_RESET);
                return 0;
            default:
                set_color(COLOR_RED);
                printf("\nInvalid option! Please try again.\n");
                set_color(COLOR_RESET);
                Sleep(1200);
        }
    }

    return 0;
}

void show_menu()
{
    set_color(COLOR_BLUE);
    printf("=========================================\n");
    printf("|         User Management System       |\n");
    printf("=========================================\n");
    set_color(COLOR_RESET);
    printf("1. Register\n");
    printf("2. Login\n");
    printf("3. Exit\n");
    printf("-----------------------------------------\n");
}

void clear_screen()
{
    system("cls");
}

void set_color(int color)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void read_password(char *password)
{
    int i = 0;
    char ch;

    while (1)
    {
        ch = _getch();

        if (ch == '\r' || ch == '\n') 
        {
            break;
        }
        else if (ch == '\b') 
        {
            if (i > 0)
            {
                i--;
                printf("\b \b"); 
            }
        }
        else if (i < MAX_LEN - 1)
        {
            password[i] = ch;
            i++;
            printf("*");
        }
    }

    password[i] = '\0';
    printf("\n");
}

int username_exists(const char *username)
{
    FILE *fp = fopen(FILE_NAME, "r");
    if (fp == NULL)
        return 0; 

    User temp;
    while (fscanf(fp, "%49s %49s", temp.username, temp.password) == 2)
    {
        if (strcmp(temp.username, username) == 0)
        {
            fclose(fp);
            return 1; 
        }
    }

    fclose(fp);
    return 0; 
}

void register_user()
{
    User new_user;

    set_color(COLOR_BLUE);
    printf("============ Register New User ============\n\n");
    set_color(COLOR_RESET);

    printf("Username: ");
    scanf("%49s", new_user.username);
    while (getchar() != '\n'); 

    if (username_exists(new_user.username))
    {
        set_color(COLOR_RED);
        printf("\nThis username already exists! Please try a different one.\n");
        set_color(COLOR_RESET);
        printf("\nPress Enter to return to the menu...");
        getchar();
        return;
    }

    printf("Password: ");
    read_password(new_user.password);

    FILE *fp = fopen(FILE_NAME, "a");
    if (fp == NULL)
    {
        set_color(COLOR_RED);
        printf("\nError: Could not open the file.\n");
        set_color(COLOR_RESET);
        Sleep(1500);
        return;
    }

    fprintf(fp, "%s %s\n", new_user.username, new_user.password);
    fclose(fp);

    set_color(COLOR_GREEN);
    printf("\nRegistration Successful!\n");
    set_color(COLOR_RESET);
    printf("\nPress Enter to return to the menu...");
    getchar();
}

void login_user()
{
    char username[MAX_LEN];
    char password[MAX_LEN];

    set_color(COLOR_BLUE);
    printf("================ Login =====================\n\n");
    set_color(COLOR_RESET);

    printf("Username: ");
    scanf("%49s", username);
    while (getchar() != '\n'); 

    printf("Password: ");
    read_password(password);

    FILE *fp = fopen(FILE_NAME, "r");
    if (fp == NULL)
    {
        set_color(COLOR_RED);
        printf("\nNo users are registered yet. Please register first.\n");
        set_color(COLOR_RESET);
        printf("\nPress Enter to return to the menu...");
        getchar();
        return;
    }

    User temp;
    int found = 0;

    while (fscanf(fp, "%49s %49s", temp.username, temp.password) == 2)
    {
        if (strcmp(temp.username, username) == 0)
        {
            found = 1;
            if (strcmp(temp.password, password) == 0)
            {
                set_color(COLOR_GREEN);
                printf("\nLogin Successful! Welcome, %s.\n", username);
                set_color(COLOR_RESET);
            }
            else
            {
                set_color(COLOR_RED);
                printf("\nLogin Failed! Incorrect password.\n");
                set_color(COLOR_RESET);
            }
            break;
        }
    }

    if (!found)
    {
        set_color(COLOR_RED);
        printf("\nLogin Failed! Username does not exist.\n");
        set_color(COLOR_RESET);
    }

    fclose(fp);
    printf("\nPress Enter to return to the menu...");
    getchar();
}