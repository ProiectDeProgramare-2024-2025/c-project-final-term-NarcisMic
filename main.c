#include <stdio.h>
#include <string.h>
#ifdef _WIN32
    #define CLEAR "cls"
#else
    #define CLEAR "clear"
#endif
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define PURPLE "\x1b[35m"
#define CYAN "\x1b[36m"
#define RESET "\x1b[0m"
#define TOTAL 15
#define MAX_RECORDS 100
typedef struct{
    char question[256];
    char options[4][128];
    int correct;
}Question;
const char* optionColors[4] = {RED, GREEN, YELLOW, BLUE};
Question questions[TOTAL] = {
    {"According to the old saying, \"love of\" what \"is the root of all evil\"?", {"Food", "Money", "Clothing", "Reality television"}, 1},
    {"A hideaway bed is typically \"hidden away\" inside which of these pieces of furniture?", {"Sofa", "Wing chair", "Coffee table", "Footstool"}, 0},
    {"Where on the human body is the umbilicus?", {"Head", "Arm", "Stomach", "Foot"}, 2},
    {"In the Olympics, a decathlon has how many more events than a triathlon?", {"5", "6", "7", "8"}, 2},
    {"An April 2007 issue of Time magazine featuring a penguin on its cover was dedicated to what hot-button issue?", {"Tax cuts", "Global warming", "Gun control", "Same-sex marriage"}, 1},
    {"Media pundits often refer to which of these time periods by the moniker \"the Decade of Greed\"?", {"1950s", "1960s", "1970s", "1980s"}, 3},
    {"What firearm shares its name with a classic Beatles album?", {"Pistol", "Revolver", "Rifle", "Musket"}, 1},
    {"What ancient civilization domesticated the Caffre cat, believed by many to be the ancestor of short-haired domestic cats?", {"Roman", "Aztec", "Chinese", "Egyptian"}, 3},
    {"In the name of the A&P supermarket chain, the \"A\" and the \"P\" stand for the names of two what?", {"U.S. cities", "Bodies of water", "U.S. states", "Mountain ranges"}, 1},
    {"Fictional author Nathan Zuckerman is featured in several works by what U.S. novelist?", {"Joseph Heller", "Don DeLillo", "Philip Roth", "Richard Russo"}, 2},
    {"Located in Russia, Chechnya is a volatile region that neighbors what nation?", {"Georgia", "Ukraine", "Moldova", "Belarus"}, 0},
    {"England coronated Queen Elizabeth II in 1953, how many years after the death of Queen Elizabeth I?", {"200", "250", "300", "350"}, 3},
    {"Whose New York Times obituary mentioned that \"Since 1889\" he \"had been hopelessly insane\"?", {"Vincent Van Gogh", "Franz Kafka", "Johannes Brahms", "Friedrich Nietzsche"}, 3},
    {"To the nearest cent, the Bureau of Engraving and Printing spends an average of how much to make each currency note in circulation?", {"2 cents", "4 cents", "6 cents", "8 cents"}, 2}
};
typedef struct {
    char name[50];
    float scores[10];
    int gamesPlayed;
} Player;
Player leaderboard[MAX_RECORDS];
int recordCount = 0;
int compareScores(const void *a, const void *b) {
    Player *playerA = (Player *)a;
    Player *playerB = (Player *)b;
    float bestScoreA = 0, bestScoreB = 0;
    for(int i = 0; i < playerA->gamesPlayed; i++)
        if(playerA->scores[i] > bestScoreA)
            bestScoreA = playerA->scores[i];
    for(int i = 0; i < playerB->gamesPlayed; i++)
        if(playerB->scores[i] > bestScoreB)
            bestScoreB = playerB->scores[i];
    if(bestScoreA < bestScoreB)
        return 1;
    if(bestScoreA > bestScoreB)
        return -1;
    return 0;
}
void saveLeaderboard() {
    FILE *file = fopen("leaderboard.txt", "w");
    fprintf(file, "%d\n", recordCount);
    for(int i = 0; i < recordCount; i++){
        fprintf(file, "%s %d ", leaderboard[i].name, leaderboard[i].gamesPlayed);
        for(int j = 0; j < leaderboard[i].gamesPlayed; j++)
            fprintf(file, "%.1f ", leaderboard[i].scores[j]);
        fprintf(file, "\n");
    }
    fclose(file);
}
void loadLeaderboard() {
    FILE *file = fopen("leaderboard.txt", "r");
    if(fscanf(file, "%d", &recordCount) != 1){
        fclose(file);
        return;
    }
    for(int i = 0; i < recordCount; i++){
        if(fscanf(file, "%s %d", leaderboard[i].name, &leaderboard[i].gamesPlayed) != 2)
            break;
        for(int j = 0; j < leaderboard[i].gamesPlayed; j++)
            if(fscanf(file, "%f", &leaderboard[i].scores[j]) != 1)
                break;
    }
    fclose(file);
}
void showLeaderboard(){
    qsort(leaderboard, recordCount, sizeof(Player), compareScores);
    for(int i = 0; i < recordCount; i++){
        float bestScore = 0;
        for(int j = 0; j < leaderboard[i].gamesPlayed; j++)
            if (leaderboard[i].scores[j] > bestScore) bestScore = leaderboard[i].scores[j];
        printf("%d. " GREEN "%s" RESET " - Best Score: " YELLOW "%.1f\n" RESET, i + 1, leaderboard[i].name, bestScore);
    }
}
void showUserHistory(int index){
    system(CLEAR);
    if(index < 1 || index > recordCount){
        printf("Invalid User!\n");
        return;
    }
    Player *p = &leaderboard[index - 1];
    printf("=========== User History ===========\n");
    printf("Player: %s\n", p->name);
    printf("Game History:\n");
    for(int i = 0; i < p->gamesPlayed; i++)
        printf("Game %d: %.1f points\n", i + 1, p->scores[i]);
    printf("\nPress Enter to go back...");
    while(getchar() != '\n');
    getchar();
}
void playGame(void){
    float score = 0.0;
    int i;
    for(i = 0; i < TOTAL; i++){
        int answer = 0, lifelineUsed = 0;
        system(CLEAR);
        printf(PURPLE "Question %d:\n" RESET, i + 1);
        printf(CYAN "%s\n" RESET, questions[i].question);
        printf("Options:\n");
        for(int j = 0; j < 4; j++)
            printf("%d. %s%s%s\n", j + 1, optionColors[j], questions[i].options[j], RESET);
        printf("5. Use 50/50 (this will reduce the question's score to 0.5 if answered correctly)\n");
        printf("Enter your answer (1-4 or 5 for 50/50): ");
        if(scanf("%d", &answer) != 1){
            while(getchar() != '\n');
            i--;
            continue;
        }
        if(answer == 5){
            lifelineUsed = 1;
            int option2 = -1;
            for(int j = 0; j < 4; j++){
                if(j != questions[i].correct){
                    option2 = j;
                    break;
                }
            }
            system(CLEAR);
            printf("50/50 Activated!\n");
            printf("Question %d:\n", i + 1);
            printf("%s\n", questions[i].question);
            printf("1. %s\n", questions[i].options[questions[i].correct]);
            printf("2. %s\n", questions[i].options[option2]);
            printf("Enter your answer (1 or 2): ");
            int lifelineAnswer;
            if(scanf("%d", &lifelineAnswer) != 1){
                while(getchar() != '\n');
                printf("Invalid input. Game Over!\n");
                break;
            }
            int chosenOption;
            if(lifelineAnswer == 1)
                chosenOption = questions[i].correct;
            else if (lifelineAnswer == 2)
                chosenOption = option2;
            else{
                printf("Invalid input. Game Over!\n");
                break;
            }
            if(chosenOption == questions[i].correct){
                score += 0.5;
                printf("Correct! (+0.5 point)\n");
            }else{
                printf("Incorrect. Game Over!\n");
                break;
            }
        }else if(answer >= 1 && answer <= 4){
            if(answer - 1 == questions[i].correct){
                score += 1.0;
                printf("Correct! (+1 point)\n");
            }else{
                printf("Incorrect. Game Over!\n");
                break;
            }
        }else{
            printf("Invalid option. Try again.\n");
            i--;
            while(getchar() != '\n');
            getchar();
            continue;
        }
        printf("Current Score: %.1f\n", score);
        printf("Press Enter to continue to the next question...");
        while(getchar() != '\n');
        getchar();
    }
    system(CLEAR);
    printf("Game over! Your final score is: %.1f\n", score);
    printf("Enter your name: ");
    char name[50];
    scanf("%s", name);
    int found = 0;
    for(int j = 0; j < recordCount; j++){
        if(strcmp(leaderboard[j].name, name) == 0){
            found = 1;
            if(leaderboard[j].gamesPlayed < 10){
                leaderboard[j].scores[leaderboard[j].gamesPlayed] = score;
                leaderboard[j].gamesPlayed++;
            }else{
                for(int k = 1; k < 10; k++)
                    leaderboard[j].scores[k - 1] = leaderboard[j].scores[k];
                leaderboard[j].scores[9] = score;
            }
            break;
        }
    }
    if(!found && recordCount < MAX_RECORDS){
        strcpy(leaderboard[recordCount].name, name);
        leaderboard[recordCount].scores[0] = score;
        leaderboard[recordCount].gamesPlayed = 1;
        recordCount++;
    }
    saveLeaderboard();
}
void startGameMenu(void){
    int t;
    while(1){
        system(CLEAR);
        printf("=========== Start Game ===========\n");
        printf("1. New Game\n");
        printf("2. Back to Main Menu\n");
        printf("Enter your choice: ");
        if(scanf("%d", &t) != 1) {
            while(getchar() != '\n');
            continue;
        }
        switch(t){
            case 1:
                playGame();
                break;
            case 2:
                return;
            default:
                printf("Invalid choice! Try again.\n");
                printf("Press Enter to continue...");
                while(getchar() != '\n');
                getchar();
        }
    }
}
void leaderboardMenu(void) {
    int choice;
    while (1) {
        system(CLEAR);
        printf("=========== Leaderboard ===========\n");
        showLeaderboard();
        printf("\nEnter player index to view history or 0 to return: ");
        if(scanf("%d", &choice) != 1){
            while(getchar() != '\n');
            continue;
        }
        if(choice == 0)
            return;
        showUserHistory(choice);
    }
}
int main(){
    loadLeaderboard();
    int choice;
    while(1){
        system(CLEAR);
        printf("======== Who Wants to Be a Millionaire? ========\n");
        printf("1. Play Game\n");
        printf("2. Leaderboard\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        if(scanf("%d", &choice) != 1){
            while(getchar() != '\n');
            continue;
        }
        switch (choice) {
            case 1:
                playGame();
                break;
            case 2:
                leaderboardMenu();
                break;
            case 3:
                printf("Exiting app.\n");
                exit(0);
            default:
                printf("Invalid choice! Try again.\n");
                printf("Press Enter to continue...");
                while(getchar() != '\n');
                getchar();
        }
    }
    return 0;
}
