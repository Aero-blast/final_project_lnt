#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct scores 
    {
        int wrongs;
        char word[20];
    }s;


void bubbleSort(s data[], int count) 
{
    for (int i = 0; i < count - 1; i++) 
    {
        for (int j = 0; j < count - i - 1; j++) 
        {
            if (data[j].wrongs > data[j + 1].wrongs) 
            {
                s temp = data[j];
                data[j] = data[j + 1];
                data[j + 1] = temp;
            }
        }
    }
}


void getHighScore() 
{
    FILE *scores = fopen("scores.txt", "r");
    if (!scores) 
    {
        printf("File not opened!\n");
        return;
    }


    s data[2000];
    int count = 0;

    while (fscanf(scores, " %d wrongs while guessing %[^,],\n", &data[count].wrongs, data[count].word) == 2) 
    {
        count++;
    }

    fclose(scores);

    bubbleSort(data, count);

    printf("Top %i High Scores:\n", (count < 10) ? count : 10);

    for (int i = 0; i < 10 && i < count; i++) 
    {
        printf("%-10s: %i wrong guesses.\n", data[i].word, data[i].wrongs);
    }
}

void printScore(int wrong, const char* word) 
{
    FILE *scores = fopen("scores.txt", "w");
    if (!scores) printf("File not opened!\n");

    fprintf(scores, "%i wrongs while guessing %s\n", wrong, word);
    fclose(scores);
}

void displayHeader() 
{
    printf("-----------------------------\n");
    printf("|          Hangman          |\n");
    printf("-----------------------------\n");
}

char* randomWord() 
{
    FILE *data = fopen("words.txt", "r");
    if (!data) printf("File not opened!\n");

    static char words[2000][20]; // 2000 words in data file
    int idx = 0;

    while (fscanf(data, " %s", words[idx++]) == 1) 
    {}

    fclose(data);
    srand(time(NULL));
    int randomNum = rand() % idx;

    return words[randomNum];
}

int findWord(char query, const char* word, char* currGuess) 
{
    int check = 0;

    for (int i = 0; i < strlen(word); i++) {
        if (query == word[i]) 
        {
            currGuess[i] = query;
            check = 1;
        }
    }
    return check;
}

void printCurrGuess(const char* currGuess) 
{
    printf("%s\n", currGuess);
}

int checkWin(const char* currGuess)
{
    for (int i = 0; i < strlen(currGuess); i++) 
    {
        if (currGuess[i] == '_')
            return 1;
    }
    return 0;
}

int checkDupe(char query, const char* guessed)
{
    for (int i = 0; i < strlen(guessed); i++) 
    {
        if (guessed[i] == query)
        {
            printf("%c has already been guessed!\n",query);
            return 1;
        }
    }
    return 0;
}

void printMan(int wrong) 
{
    switch (wrong) 
    {
        case 0:
            printf("\n");
            break;
        case 1:
            printf("\n  O \n");
            break;
        case 2:
            printf("\n  O \n  | \n");
            break;
        case 3:
            printf("\n  O \n /| \n");
            break;
        case 4:
            printf("\n  O \n /|\\ \n");
            break;
        case 5:
            printf("\n  O \n /|\\ \n / \n");
            break;
        case 6:
            printf("\n  O \n /|\\ \n / \\ \n");
            break;
        default:
            printf("\n  O \n /|\\ \n / \\ \n");
            break;
    }

}

int game() {
    char* word = randomWord();
    char currGuess[20]="";
    int wrong = 0;

    for (int i = 0; i < strlen(word)-1; i++) 
    {
        currGuess[i] = '_';
    }

    printf("\n");
    printf("***************************************\n");
    printf("* Welcome to Hangman! Guess the word! *\n");
    printf("***************************************\n");
    printf("\n");

    printf("Instructions:\n");
    printf("1. Try to guess the word, one letter at a time.\n");
    printf("2. Each wrong guess will add a part to the hangman.\n");
    printf("3. You have a maximal of 6 wrong guesses.\n");
    printf("\n");

    char guessed[27]="";

    while (checkWin(currGuess) != 0 && wrong < 6) 
    {
        char guess;
        int dupe=0;
        
        printCurrGuess(currGuess);
        do
        {
            printf("Enter your guess: ");
            scanf(" %c", &guess);
            char temp[2] = {guess, '\0'}; 
            dupe = checkDupe(guess,guessed);
            if (!dupe)strcat(guessed,temp);  
        } while (dupe);

        int check = findWord(guess, word, currGuess);
        if (check == 0) wrong++;
        
        printMan(wrong);
    }

    int len = strlen(word);
    word[len-1]='\0'; // ubah dari koma jd null

    if (wrong == 6) 
    {
        printf("You lost! The word was: %s.\n", word);
    } 
    else 
    {
        printf("You win! The word was: %s.\n", word);
    }

    printScore(wrong, word);
    return 0;
}

int showMenu() 
{
    int choice;
    printf("1. Play Hangman\n");
    printf("2. View High Scores\n");
    printf("3. Exit Game\n");
    printf("Your Choice: ");scanf("%i", &choice);
    return choice;
}

int task(int n) {
    switch (n) 
    {
        case 1:
            game();
            break;
        case 2:
            getHighScore();
            break;
        case 3:
            printf("Thanks for playing!\n");
            return 1;
        default:
            break;
    }
    return 0;
}

int main() 
{
    int n = 0;
    while (n != 3)
    {
        displayHeader();
        n = showMenu();
        task(n);
    }

    return 0;
}
