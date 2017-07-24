/*
* Author:Dorian-Benjamin Dressler
* Title: Programming Project: Word Search
* FILE: main function
* Date : 2015/04/28
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
#define ROWS 12
#define COLS 12
#define num_of_words 8
#define FALSE 0
#define TRUE 1

// prototypes
char **create2DArray();
void printArray(char **array);
void addWord_horizontal(char** mainArray, int stringArrIndex);
void addStringsToGrid(char** gameGrid);
int checkSpace_horizontal(int stringArrIndex, int randCol, int randRow);
void addWord_vertical(char** gameGrid ,int stringArrIndex);
int checkSpace_vertical(int stringArrIndex, int randCol, int randRow);
void addWord_diagonal(char** gameGrid ,int stringArrIndex);
int checkSpace_diagonal(int stringArrIndex, int randCol, int randRow);
void add_randomLetters(char** arr_2D);
void game_userEntry();
int game_matchesFound(int reset, int val);
void menu();
void gameFunction();
int **createCoordArray();
void print_coordIntArray(int **arr); //debugging only
void wordFoundColors(int word);

//constants and arrays
char **gameGrid; //is where all the work is done
char **copyGrid; // is what's actually displayed
int **coordArray; //stores all coordiantes of words added via addString
const char *animals[] = {"DOG", "CAT", "FISH", "PIG", "DUCK", "BIRD", "SKUNK", "GECKO"};
const char *countryCapitals[] = {"BERLIN", "ATHENS", "LONDON", "PARIS", "VIENNA", "BERN", "TOKYO", "MADRID"};
const char *monkeyIsland[] = {"DINKY", "SCABB", "MELEE", "PHATT", "BOOTY", "BLOOD", "SKULL", "PLUNDER"};
char **pointer;

	typedef enum
	{
		RED=64,ORANGE=41,YELLOW=14, GREEN=160, BLUE=9, INDIGO=11, VIOLET=13
	}colors;

int main(){

    srand(time(NULL));
    menu();

    return 0;
}

//menu goes here
void menu(){

	HANDLE hConsole; hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
	int gameOver = FALSE;
	int choice = 0;

	printf(" _ _ _           _    _____                 _   \n");
	printf("| | | |___ ___ _| |  |   __|___ ___ ___ ___| |_ \n");
	printf("| | | | . |  _| . |  |__   | -_| .'|  _|  _|   |\n");
	printf("|_____|___|_| |___|  |_____|___|__,|_| |___|_|_|\n");
	printf("\n\n\n\t\t(C)2015 - Dorian Dressler.");
	Sleep(2);

	while(gameOver == FALSE){

		system("cls");
		printf("******************\n");
		printf("Choose a category:\n");
		printf("******************\n\n");
		printf("1:Animals\n");
		printf("2:Capitals\n");
		printf("3:The Secret of Monkey Island\n");
		printf("0 To Quit\n\n");
		printf("Choice:> ");
		scanf("%d", &choice);
		getchar();

		//could've done switch here, but blah...
		if(choice == 1){

			pointer = animals;
			printf("\nFind 8 animals.\n\n");
			Sleep(1);
			printf("...and eat them.");
			Sleep(1);
			gameFunction();
		}
		if(choice == 2){

			pointer = countryCapitals;
			printf("\nFind 8 Capitals of the world!\n\n");
			Sleep(1);
			gameFunction();
		}
		if(choice == 3){

			pointer = monkeyIsland;
			printf("\nFind 8 Islands of Monkey Island(TM)!");
            Sleep(2);
			gameFunction();
		}
		if(choice == 0){

			gameOver = TRUE;
			system("CLS");
			printf("***********\n");
			printf("*Good Bye!*\n");
			printf("***********\n");
			break;
		}
	}

}//end of menu

//game function
void gameFunction(){

    int gameOver = FALSE;
    gameGrid = create2DArray();
    copyGrid = create2DArray(); //used for printing only
	coordArray = createCoordArray();
    addStringsToGrid(gameGrid);
    add_randomLetters(gameGrid);

	//filling the shadow array for printing
    int rows;
	for(rows = 0; rows < ROWS; rows++){

		strcpy(copyGrid[rows], gameGrid[rows]);
	}


    while(gameOver == FALSE){

        system("CLS");
        printArray(gameGrid);
        game_userEntry(gameGrid);
        gameOver = TRUE;
    }






} // end of game function

/*generates a 2D Array*/
char **create2DArray(){

	int i,j;
	char** array = {0};

	array = malloc(sizeof(char*) *ROWS);

	for(i=0; i < ROWS; i++){

		array[i] = (char *) malloc(sizeof(char) * COLS);
	}
	for(i=0;i<ROWS;i++){

		for(j=0;j<COLS;j++){

			array[i][j] = '.';
		}
	}
	return array;

	free(array);
}//end of create2DArray

/*prints a 2D array*/
void printArray(char **array){

	HANDLE hConsole; hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	int i,j;

	for( i = 0; i < ROWS; i++){

		for (j = 0; j < COLS;j++){

				if(array[i][j] == '#'){

					SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
				}
				else
				SetConsoleTextAttribute(hConsole, BLUE);
				printf(" %c ", copyGrid[i][j]);
		}
		printf("\n\n");
	}
}//end of printArray

/*add Strings to gameGrid*/ //this func needs to spoof them into the color array
void addStringsToGrid(char** gameGrid){

	int i = 0;
    int counter = 0;

    for (i; i < num_of_words; i++){


		//replace below with RNG based solution?
		if(counter <= 2){

            //addWord_vertical(gameGrid, i);
			addWord_horizontal(gameGrid, i);
			counter++;
			//i++;
		}
		else if(counter > 2 && counter < 6){
            addWord_vertical(gameGrid, i);
            counter++;
		}
		else {
            addWord_diagonal(gameGrid, i);
            counter++;
		}
	}

}//end of addStrings

void addWord_horizontal(char** gameGrid ,int stringArrIndex){

    static int chaLoc = 0;
	int 	i, randRow, randCol;
	int 	wordPlaced = 0;
	int		spaceClear = 0;
	
	char *tempWord = pointer[stringArrIndex];

	while(wordPlaced == 0){

        i = 0;
		randRow = (rand()%ROWS);
		randCol = (rand()%COLS);
		//printf("Row: %d ", randRow);
		//printf("Col: %d \n", randCol);
        //printf("you're stuck in a loop\n");

		if( (COLS - randCol) >= (strlen(tempWord)) ){

			spaceClear = checkSpace_horizontal(stringArrIndex, randCol, randRow);

			if(spaceClear == 1){
			//printf("index: %d\n", stringArrIndex);

				for(randCol; i < strlen(tempWord); randCol++, i++){

					gameGrid[randRow][randCol] = tempWord[i];
					coordArray[stringArrIndex][chaLoc] = randRow;
					//printf("Coord Row %d \n", coordArray[stringArrIndex][chaLoc]);
					++chaLoc;
					coordArray[stringArrIndex][chaLoc] = randCol;
					//printf("Coord Col %d\n", coordArray[stringArrIndex][chaLoc]);
					++chaLoc;
					//debugging

				}
				chaLoc = 0; //cleans static var
				wordPlaced = 1;
				//break;
			} else {

				continue;
			}
		}
	}
}

int checkSpace_horizontal(int stringArrIndex, int randCol, int randRow){

	int i = 0;
	int done = 0;
	int org = randCol;
	char *tempWord = pointer[stringArrIndex];

	while(done == 0){

		randCol = org;
		i = 0;

		for(randCol; i < strlen(tempWord); randCol++){

			if(gameGrid[randRow][randCol] != '.'){ // || gameGrid[randRow][randCol] != tempWord[i]

				return 0;
				break;
			}
			else{

				++i;
				//printf("Char Slot: %d ,\n", i);
			}
		}
		done = 1;
		//break;
    }
    return 1;
}

void addWord_vertical(char** gameGrid ,int stringArrIndex){

    static int chaLoc = 0; //location of char in stringArrindex of coordArray
	int i, randRow, randCol;
	int wordPlaced = 0;
	int spaceClear = 0;
	char *tempWord = pointer[stringArrIndex];

	while(wordPlaced == 0){

        i = 0;
		randRow = (rand()%ROWS);
		randCol = (rand()%COLS);
		//printf("Row: %d ", randRow);
		//printf("Col: %d \n", randCol);
        //printf("you're stuck in a loop\n");

		if( (ROWS - randRow) >= (strlen(tempWord)) ){

			spaceClear = checkSpace_vertical(stringArrIndex, randCol, randRow);

			if(spaceClear == 1){

				for(randCol; i < strlen(tempWord); randRow++, i++){

					gameGrid[randRow][randCol] = tempWord[i];
					//saving coordinates to coord array
					coordArray[stringArrIndex][chaLoc] = randRow;
					++chaLoc; //1 step forward
					coordArray[stringArrIndex][chaLoc] = randCol;
                    ++chaLoc;
				}
                chaLoc = 0; //reset
				wordPlaced = 1;
				//break;
			} else {

				continue;
			}
		}
	}
}

int checkSpace_vertical(int stringArrIndex, int randCol, int randRow){

	int 	i = 0;
	int 	done = 0;
	int 	org = randRow;
	
	char *tempWord = pointer[stringArrIndex];

	while(done == 0){

		randRow = org;
		i = 0;

		for(randRow; i < strlen(tempWord); randRow++){

			if(gameGrid[randRow][randCol] == '.' || gameGrid[randRow][randCol] == tempWord[i]){ // || gameGrid[randRow][randCol] != tempWord[i]


                //debugging area:
                //printf("V StrCh: %c ", *(tempWord+i));
                //printf("V GridCh: %c \n", gameGrid[randRow][randCol]);
                //system("PAUSE");
                ++i;
			}else{
                
				return 0;

			}
		}
		return 1;
    }

}//end of vertical



//must check for 2
void addWord_diagonal(char** gameGrid ,int stringArrIndex){

    static int chaLoc = 0;
	int i, randRow, randCol;
	int wordPlaced = 0;
	int spaceClear = 0;
	char *tempWord = pointer[stringArrIndex];

	while(wordPlaced == 0){

        i = 0;
		randRow = (rand()%ROWS);
		randCol = (rand()%COLS);
		//printf("Row: %d ", randRow);
		//printf("Col: %d \n", randCol);
        //printf("you're stuck in a loop\n");

		if( (ROWS - randRow) >= (strlen(tempWord)) && (COLS - randCol) >= (strlen(tempWord))){

			spaceClear = checkSpace_diagonal(stringArrIndex, randCol, randRow);

			if(spaceClear == 1){

				for(randCol; i < strlen(tempWord); randRow++, randCol++, i++){

					gameGrid[randRow][randCol] = tempWord[i];
					coordArray[stringArrIndex][chaLoc] = randRow;
					++chaLoc;
					coordArray[stringArrIndex][chaLoc] = randCol;
					++chaLoc;

				}
                chaLoc = 0;
				wordPlaced = 1;
				//break;
			} else {

				continue;
			}
		}
	}
}

int checkSpace_diagonal(int stringArrIndex, int randCol, int randRow){

	int i = 0;
	int done = 0;
	int orgRow = randRow;
	int orgCol = randCol;
	char *tempWord = pointer[stringArrIndex];

	while(done == 0){

		randRow = orgRow;
		randCol = orgCol;
		i = 0;

		for(randRow, randCol; i < strlen(tempWord); randRow++,randCol++){

			if(gameGrid[randRow][randCol] == '.' || gameGrid[randRow][randCol] == tempWord[i]){

				++i;
			}
			else{

                return 0;
				//printf("Char Slot: %d ,\n", i);
			}
		}
		return 1;
    }

}//end of checkspace diagonal

void add_randomLetters(char** arr_2D){

	int i, j;

	for(i=0;i<ROWS;i++){

		for(j=0;j<COLS;j++){

			if(gameGrid[i][j] == '.'){

				arr_2D[i][j] = (rand() % 25 + 65);	//mind the magic numbers
			}
		}
	}
}//end of random letters

void game_userEntry(){

	float timeLimit = 240;
	clock_t start = clock();
	//float seconds = (float)(tick - start)
	char userInput[20]; //previously str1
	int i,j;
	int matches = 0;
	int counter = 0;
	int gameOver = FALSE;
	//clock_t start = clock(); //starts the stopwatch
	//clock_t tick = clock();
	gameOver = game_matchesFound(1, 0);

	while(gameOver == FALSE){

		clock_t tick = clock();
		float seconds = (float)(tick - start) / CLOCKS_PER_SEC;
		timeLimit -= seconds;
		printf("\n%.f secs left\n", timeLimit);
		if(timeLimit <= 0){

			printf("Time out...");
			system("PAUSE");
			//sleep(2);
			break;
		}
		printf("\nPlease enter a word: ");
		scanf("%s", &userInput);
		//printf("\nseconds: %f\n", seconds);
		//converts entered string to uppercase
		for(i = 0; i < strlen(userInput); i++){
			if(userInput[i] > 96 && userInput[i] < 123){
				userInput[i] -= 32;
			}
		}

		for(i = 0;i < num_of_words; i++){

			char *selectedString = pointer[i];  //proper pointer, formerly str2
			counter = 0;
			for(j = 0; j < strlen(selectedString); j++){
			//call that function with the i val

				if(userInput[j] == selectedString[j]){
					//insert color call here, based on below? (or that x shit), tie into array
					//printf("\nChar str1: %c\tChar str2: %c", userInput[j], selectedString[j]);
					//gameGrid[j] = 'x';
					system("cls"); //clears screen after correct input PUT BACK IN
					printArray(gameGrid);
					counter++;
					//wordFoundColors(j);

				}
				else{
					system("cls");
					printArray(gameGrid);
					printf("nope");
					break;
				}
			}
			if(counter==strlen(selectedString)){

				//printf("\nMatch detected in position %d.\n\n", i);
				gameOver = game_matchesFound(0, i);
				//print2DArray(array);
				matches++;
				break;
			}

		}
	}
} //end of void user entry


int game_matchesFound(int reset, int val){

	static int matchCount = 0;
	static int matchArray[num_of_words] = {0}; //contains matches, rules out repeats
	int zeroFilled = FALSE;
	int i;
	int gameOver = FALSE;

	//resets from last game
	if(reset == 1){

		matchCount = 0;
		zeroFilled = FALSE;

		for(i = 0; i < num_of_words; i++){

            matchArray[i] = 0;
		}
		return FALSE;
	}
	while(gameOver==FALSE && reset == 0){ //think about using time func

		if(zeroFilled == FALSE && matchArray[0] == val){

		matchArray[0] = val;
		matchCount++;
		zeroFilled = TRUE;
		wordFoundColors(val);

	}else if(matchArray[val] != val){

		matchArray[val] = val;
		wordFoundColors(val);
		matchCount++;

	}else{

		printf("\nAlready entered.");
		Sleep(1);
    }

	if(matchCount == num_of_words){

		system("CLS");
		printf(" __ __ _____ _____    _ _ _ _____ _____ \n");
		printf("|  |  |     |  |  |  | | | |     |   | |\n");
		printf("|_   _|  |  |  |  |  | | | |  |  | | | |\n");
		printf("  |_| |_____|_____|  |_____|_____|_|___|\n");
		Sleep(5);

		//static variable cleanup for next run: (there is a problem with this)
		matchCount = 0;
		zeroFilled = FALSE;
		for(i = 0; i < num_of_words; i++){

            matchArray[i] = 0;
		}

		return TRUE;
	}else{

		return FALSE;
	}
}//end of while loop
} //end of matches found func

/*Adds # character to slot in gameGrid, where color will be added*/
void wordFoundColors(int word){
	//sHANDLE hConsole; hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    int i = 0;
    int j = 0;
    int row, col;
    char *tempWord = pointer[word];
    int length = (strlen(tempWord)*2);
    char x = '#';

    for(j, i; j < length; j++, i++){

		row = coordArray[word][j];
        ++j;
        col = coordArray[word][j];
        gameGrid[row][col] = x;//+ system("COLOR 6C")
		system("cls");
        printArray(gameGrid);
    }
}


//char locations stored in this
int **createCoordArray(){

	int i, j, colWidth;
	int** arr_2D = { 0 };

	arr_2D = malloc(sizeof(int*) *num_of_words);

	for(i = 0; i < num_of_words; i++){
		//get string length of each string and multiply by 2 for j length
		colWidth = (strlen(pointer[i])*2);
		//printf("Index: %d, width: %d\n", i, colWidth);
		arr_2D[i] = (int *)malloc(sizeof(int) * colWidth);
	}

	for(i = 0; i < num_of_words; i++){

		colWidth = ((strlen(pointer[i]))*2);

		for(j = 0; j < colWidth; j++){

			arr_2D[i][j] = 0;
			//printf("%d", arr_2D[i][j]);
		}
	}
	return arr_2D;
	free(arr_2D);
}
