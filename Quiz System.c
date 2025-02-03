#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERNAME 50
#define MAX_PASSWORD 50
#define MAX_STRING 100
#define MAX_QUESTIONS 50
#define MAX_OPTIONS 4

struct User
{
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
    char type[10];
};

struct Question
{
    char question[MAX_STRING];
    char options[MAX_OPTIONS][MAX_STRING];
    int correct_answer;
};

struct Quiz
{
    char module_name[MAX_STRING];
    int question_count;
    struct Question questions[MAX_QUESTIONS];
};

void userRegister();
void loginUser();
void lecturerMenu(char *username);
void createQuiz();
void studentMenu(char *username);
void attemptQuiz(char *username);
void vresult(char *username);
void allResults();

int main()
{
    int choice;

    while (1)
    {
        printf("\n=== University Quiz System ===\n");
        printf("1. Register\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice)
        {
        case 1:
            userRegister();
            break;
        case 2:
            loginUser();
            break;
        case 3:

            return 0;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    }
    return 0;
}
void userRegister()
{

    struct User newUser;
    int userType;

    printf("\nSelect user type:\n");
    printf("1. Lecturer\n");
    printf("2. Student\n");
    printf("Enter choice (1 or 2): ");
    scanf("%d", &userType);
    getchar();

    if (userType != 1 && userType != 2)
    {
        printf("Invalid user type selected!\n");
        return;
    }

    strcpy(newUser.type, (userType == 1) ? "lecturer" : "student");

    printf("Enter username (max %d characters): ", MAX_USERNAME - 1);
    fgets(newUser.username, MAX_USERNAME, stdin);
    newUser.username[strcspn(newUser.username, "\n")] = 0;

    printf("Enter password (max %d characters): ", MAX_PASSWORD - 1);
    fgets(newUser.password, MAX_PASSWORD, stdin);
    newUser.password[strcspn(newUser.password, "\n")] = 0;

    FILE *file = fopen("userdetails.txt", "a");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    fprintf(file, "%s %s %s\n", newUser.username, newUser.password, newUser.type);
    fclose(file);

    system("cls");

    printf("\nRegistration successful!\n");
    printf("User type: %s\n", newUser.type);
    printf("Username: %s\n", newUser.username);

}

// Function to login
void loginUser()
{
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
    int loginSuccess = 0;
    struct User currentUser;

    printf("\nLOGIN\n");
    printf("Enter username: ");
    fgets(username, MAX_USERNAME, stdin);
    username[strcspn(username, "\n")] = 0;

    printf("Enter password: ");
    fgets(password, MAX_PASSWORD, stdin);
    password[strcspn(password, "\n")] = 0;

    FILE *file = fopen("userdetails.txt", "r");
    if (file == NULL)
    {
        printf("No registered users found!\n");
        return;
    }

    char line[150];
    while (fgets(line, sizeof(line), file))
    {
        sscanf(line, "%s %s %s", currentUser.username, currentUser.password, currentUser.type);
        if (strcmp(currentUser.username, username) == 0 &&
            strcmp(currentUser.password, password) == 0)
        {
            loginSuccess = 1;
            break;
        }
    }

    system("cls");

    if (loginSuccess)
    {
        printf("\nLogin successful!\n");
        printf("Welcome, %s (%s)\n", currentUser.username, currentUser.type);

        if (strcmp(currentUser.type, "lecturer") == 0)
        {
            lecturerMenu(currentUser.username);
        }
        else if (strcmp(currentUser.type, "student") == 0)
        {
            studentMenu(currentUser.username);
        }
    }
    else
    {
        printf("\nLogin failed! Invalid username or password.\n");
    }

    fclose(file);
}
// Lecturer menu
void lecturerMenu(char *username)
{
    int choice;
    while (1)
    {
        printf("\n=== Lecturer Menu ===\n");
        printf("1. Create Quiz\n");
        printf("2. View Quiz Results\n");
        printf("3. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice)
        {
        case 1:
            createQuiz();
            break;
        case 2:
            allResults();
            break;
        case 3:
            return;
        default:
            printf("Invalid choice!\n");
        }
    }
}

void createQuiz()
{
    struct Quiz quiz;
    FILE *file;
    char filename[MAX_STRING];

    printf("\nCREATE QUIZ\n");
    printf("Enter module name: ");

    fgets(quiz.module_name, MAX_STRING, stdin);
    quiz.module_name[strcspn(quiz.module_name, "\n")] = 0;

    printf("Enter number of questions (max %d): ", MAX_QUESTIONS);
    scanf("%d", &quiz.question_count);
    getchar();

    if (quiz.question_count > MAX_QUESTIONS || quiz.question_count <= 0)
    {
        printf("Invalid number of questions!\n");
        return;
    }

    file = fopen("createdQuiz.txt", "w");
    if (file == NULL)
    {
        printf("Error creating quiz file!\n");
        return;
    }

    fprintf(file, "%s\n%d\n", quiz.module_name, quiz.question_count);

    for (int i = 0; i < quiz.question_count; i++)
    {
        printf("\nQuestion %d:\n", i + 1);
        printf("Enter question: ");
        fgets(quiz.questions[i].question, MAX_STRING, stdin);
        quiz.questions[i].question[strcspn(quiz.questions[i].question, "\n")] = 0;

        for (int j = 0; j < MAX_OPTIONS; j++)
        {
            printf("Enter option %d: ", j + 1);
            fgets(quiz.questions[i].options[j], MAX_STRING, stdin);
            quiz.questions[i].options[j][strcspn(quiz.questions[i].options[j], "\n")] = 0;
        }

        printf("Enter correct answer (1-4): ");
        scanf("%d", &quiz.questions[i].correct_answer);
        getchar(); // Clear input buffer

        if (quiz.questions[i].correct_answer < 1 || quiz.questions[i].correct_answer > 4)
        {
            printf("Invalid correct answer! Must be between 1 and 4.\n");
            fclose(file);
            return;
        }

        fprintf(file, "%s\n", quiz.questions[i].question);
        for (int j = 0; j < MAX_OPTIONS; j++)
        {
            fprintf(file, "%s\n", quiz.questions[i].options[j]);
        }
        fprintf(file, "%d\n", quiz.questions[i].correct_answer);
    }

    fclose(file);
    printf("\nQuiz created successfully!\n");
}

void allResults()
{
    FILE *file = fopen("result.txt", "r");
    if (file == NULL)
    {
        printf("Error: Unable to open result file.\n");
        return;
    }

    char r_module[MAX_STRING], r_username[MAX_USERNAME];
    int r_score, r_total;

    printf("\n=== All Quiz Results ===\n");

    while (fscanf(file, "%s %s %d %d", r_module, r_username, &r_score, &r_total) == 4)
    {
        printf("Module: %s, Username: %s, Score: %d/%d\n", r_module, r_username, r_score, r_total);
    }

    fclose(file);
}


// Student menu
void studentMenu(char *username)
{
    int choice;
    while (1)
    {
        printf("\n=== Student Menu ===\n");
        printf("1. Attempt Quiz\n");
        printf("2. View My Results\n");
        printf("3. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice)
        {
        case 1:
            attemptQuiz(username);
            break;
        case 2:
            vresult(username);
            break;
        case 3:
            return;
        default:
            printf("Invalid choice!\n");
        }
    }
}

void attemptQuiz(char *username)
{
    char module_name[MAX_STRING];
    FILE *file;
    struct Quiz quiz;
    int score = 0;
    int answer;

    printf("\nATTEMPT QUIZ\n");
    printf("Enter module name: ");
    fgets(module_name, MAX_STRING, stdin);
    module_name[strcspn(module_name, "\n")] = 0;

    file = fopen("createdQuiz.txt", "r");
    FILE *file_1 = fopen("result.txt", "a+");

    if (file_1 == NULL)
    {
        printf("Error: Unable to open result file.\n");
        fclose(file);
        return;
    }

    if (file == NULL)
    {
        printf("Quiz not found!\n");
        return;
    }

    fgets(quiz.module_name, MAX_STRING, file);
    quiz.module_name[strcspn(quiz.module_name, "\n")] = 0;
    fscanf(file, "%d\n", &quiz.question_count);

    printf("\nModule: %s\n", quiz.module_name);
    printf("Number of questions: %d\n\n", quiz.question_count);

    for (int i = 0; i < quiz.question_count; i++)
    {
        fgets(quiz.questions[i].question, MAX_STRING, file);
        quiz.questions[i].question[strcspn(quiz.questions[i].question, "\n")] = 0;

        printf("\nQuestion %d: %s\n", i + 1, quiz.questions[i].question);

        for (int j = 0; j < MAX_OPTIONS; j++)
        {
            fgets(quiz.questions[i].options[j], MAX_STRING, file);
            quiz.questions[i].options[j][strcspn(quiz.questions[i].options[j], "\n")] = 0;
            printf("%d. %s\n", j + 1, quiz.questions[i].options[j]);
        }

        fscanf(file, "%d\n", &quiz.questions[i].correct_answer);

        printf("Your answer (1-4): ");
        scanf("%d", &answer);

        if (answer == quiz.questions[i].correct_answer)
        {
            score++;
        }
    }

    fprintf(file_1, "%s %s %d %d\n", module_name, username, score, quiz.question_count);
    fclose(file_1);

    fclose(file);

    printf("\nQuiz completed!\n");
    printf("Your score: %d out of %d\n", score, quiz.question_count);
    float percentage = (float)score / quiz.question_count * 100;
    printf("Percentage: %.2f%%\n", percentage);
}
void vresult(char *username)
{
    FILE *file_1 = fopen("result.txt", "r");
    if (file_1 == NULL)
    {
        printf("Error: Unable to open result file.\n");
        return;
    }

    char r_module[MAX_STRING], r_username[MAX_USERNAME];
    int r_score, r_total;
    int found = 0;

    printf("\n=== Your Quiz Results ===\n");

    while (fscanf(file_1, "%s %s %d %d", r_module, r_username, &r_score, &r_total) == 4)
    {
        if (strcmp(r_username, username) == 0)
        {
            printf("Module: %s, Username: %s, Score: %d/%d\n", r_module, r_username, r_score, r_total);
            found = 1;
        }
    }

    if (!found)
    {
        printf("No results found for username: %s\n", username);
    }

    fclose(file_1);
}

