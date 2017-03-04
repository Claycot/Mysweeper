// Mysweeper.cpp : Defines the entry point for the console application.
// Code by claycot

#include <iostream>
#include <string>
#include "stdlib.h"
#include "time.h"
#include <iomanip>
#include "stdio.h"
#include "ctype.h"

using namespace std;

//Prototyping functions
void initBoard(char gameBoard[][22], char solnBoard[][22], int size);
void setMine(char gameBoard[][22], char solnBoard[][22], int size);
void calcBoard(char solnBoard[][22], int size);
void drawBoard(char board[][22], int size);
int drawMenu(string diffLev);
void startGame(char gameBoard[][22], char solnBoard[][22], int size);
void endGame(bool win);
string setDiff();
void makeMove(char gameBoard[][22], char solnBoard[][22], int size, bool firstMove);
int letterToNumber(string letterIn);
int verifyNumber(string numberIn);
char numNeighboringMines(int row, int col, char solnBoard[][22], int size);
bool checkWin(char gameBoard[][22], char solnBoard[][22], int size);
bool checkLoss(char gameBoard[][22], char solnBoard[][22], int size);
void cascadeZeroes(char gameBoard[][22], char solnBoard[][22], int size, int row, int col);

//Handles game action
int main()
{
	char gameBoard[22][22];
	char solnBoard[22][22];
	string diffLev = "Beginner";
	int size;
	
	//Start of the program.
	cout << "Welcome to Mysweeper!\nThis is a text-based, C++ rendition of the Windows classic, Minesweeper!\n";

	//Display the menu options and proceed based on user interaction.
	for (;;) {
		switch (drawMenu(diffLev)) {
		case 1: if (diffLev == "Beginner") {
					size = 9;
				}
				else if (diffLev == "Intermediate") {
					size = 16;
				}
				else if (diffLev == "Expert") {
					size = 20;
				}
			startGame(gameBoard, solnBoard, size);
			do {
				drawBoard(gameBoard, size);
				makeMove(gameBoard, solnBoard, size, false);
			} while (!checkWin(gameBoard, solnBoard, size) && !checkLoss(gameBoard, solnBoard, size));
			drawBoard(solnBoard, size);
			cout << "Press enter to return to the main menu.\n";
			cin.ignore();
			break;
		case 2: diffLev = setDiff();
			break;
		case 3: return 0;
		}
	}
}

//Initialize both boards
void initBoard(char gameBoard[][22], char solnBoard[][22], int size)
{	
	for (int row = 0; row <= size; row++) {
		for (int col = 0; col <= size; col++) {
			gameBoard[row][col] = '?';
		}
	}

	for (int row = 0; row <= size; row++) {
		for (int col = 0; col <= size; col++) {
			solnBoard[row][col] = 'b';
		}
	}
}

//Set mines on the board.
void setMine(char gameBoard[][22], char solnBoard[][22], int size)
{
	int row, col;
	int numMines;

	//Vary number of mines based on board size
	switch (size) {
	case 9:
		numMines = 10;
		break;
	case 16:
		numMines = 40;
		break;
	case 20:
		numMines = 99;
		break;
	}
	
	//Set mines randomly. Don't place mines in same square twice or in user's starting square
	srand(time(0));
	for (int mine = 0; mine < numMines; mine++) {
		do {
			row = rand() % size + 1;
			col = rand() % size + 1;
		} while (solnBoard[row][col] == 'x' || solnBoard[row][col] == 's');
		solnBoard[row][col] = 'x';
	}
}

//Call neighboring mines function to populate board with numbers
void calcBoard(char solnBoard[][22], int size)
{
	for (int row = 1; row <= size; row++) {
		for (int col = 1; col <= size; col++) {
			if (solnBoard[row][col] == 'x') {
			}
			else {
				solnBoard[row][col] = numNeighboringMines(row, col, solnBoard, size);
			}
		}
	}
}

//Draw whichever board is requested
void drawBoard(char board[][22], int size)
{
	/*
	CHARACTER CODE:
	x = mine
	? = covered space
	0 = blank, uncovered space
	f = flag
	1 - 8 = dangerous, uncovered space
	*/

	cout << endl;

	//Change header depending on the size of the board
	switch (size) {
	case 9: cout << "     A B C D E F G H I\n";
		break;
	case 16: cout << "     A B C D E F G H I J K L M N O P\n";
		break;
	case 20: cout << "     A B C D E F G H I J K L M N O P Q R S T\n";
		break;
	}

	for (int row = 1; row <= size; row++) {
		cout << setw(2) << row << " | ";
		for (int col = 1; col <= size; col++) {
			cout << board[row][col] << " ";
		}
		cout << endl;
	}
}

//Display the menu and return the menu selection.
int drawMenu(string diffLev)
{	
	//Variables used for processing user menu selection.
	string rawInput;
	int trueInput;

	//Prompt user with menu options.
	cout << "\nThis is the main menu. What would you like to do?\n"
		<< "1. Start a new game.\n"
		<< "2. Change the difficulty. Current difficulty: " << diffLev << ".\n"
		<< "3. Exit.\n";

	//Allow user to input selection. Check if the user has entered a valid selection.
	do {
		cout << "Type your selection number: ";
		cin >> rawInput;
	} while (rawInput != "1" && rawInput != "2" && rawInput != "3");

	//Convert the verified user input string to an integer and return the selection.
	if (rawInput == "1"){
		trueInput = 1;
	} 
	else if (rawInput == "2"){
		trueInput = 2;
	}
	else if (rawInput == "3"){
		trueInput = 3;
	}
	
	return trueInput;
}

//Collect function calls which are used at the start of the game
void startGame(char gameBoard[][22], char solnBoard[][22], int size)
{
	//Initialize both boards
	initBoard(gameBoard, solnBoard, size);
	//Draw the game board
	drawBoard(gameBoard, size);
	//Ask the user to make a move, use it to set mines
	makeMove(gameBoard, solnBoard, size, true);
}

//Output message based on if winner has won or lost
void endGame(bool win)
{
	//If the player has lost, give a sad message
	if (!win) {
		cout << "\nWhat a horrible night to have a curse.\n"
			 << "You have lost.\n"
			 << "Press enter to view the solution board.";
	}
	//If the player has won, give a happy message
	else if(win) {
		cout << "\nYou've done it. What a masterpiece.\n"
			 << "You completed the sweep in: MISSINGNO !\n"
			 << "Press enter to view the solution board.";
	}

	//Allow the user to press enter to proceed
	cin.ignore();
	//cin.get();
	return;
}


//Changing difficulty; number of mines and board size change (BROKEN atm)
string setDiff() 
{
	//Variables used for processing user menu selection.
	//This function is broke as hell because arrays are pre-allocated
	string rawInput;
	int trueInput;

	cout << "\nWhich difficulty would you like?\n"
		<< "1. Beginner\n"
		<< "2. Intermediate\n"
		<< "3. Expert\n";
	do {
		cout << "Type your selection number: ";
		cin >> rawInput;
	} while (rawInput != "1" && rawInput != "2" && rawInput != "3");

	//Convert the verified user input string to an integer and return the selection.
	if (rawInput == "1"){
		trueInput = 1;
	} 
	else if (rawInput == "2"){
		trueInput = 2;
	}
	else if (rawInput == "3"){
		trueInput = 3;
	}
	
	switch (trueInput) {
	case 1: return "Beginner";
	case 2: return "Intermediate";
	case 3: return "Expert";
	}
}

//Allow user to make a move
void makeMove(char gameBoard[][22], char solnBoard[][22], int size, bool firstMove)
{
	string rawInput;
	string cell;
	bool flagMode = false;
	bool goodInput = false;
	int row;
	string rawCol;
	int col;
	string rawRow;

	//Prompt user with outdated jokel
	cout << "It's all in your hands. Make a move. (Type h for help)\n";
	do {
		cout << ">> ";
		cin >> ws;
		getline(cin, rawInput);

		//Allow a help function
		if (rawInput == "h" || rawInput == "H") {
			cout << "TO UNCOVER A CELL: Type the name of the cell you'd like to uncover. EG: G5\n"
				 << "TO FLAG A CELL: Type f followed by the name of the cell you'd like to flag. EG: f G5\n";
			goodInput = false;
		}
		//Allow a flagging function
		else if (rawInput.substr(0, 2) == "f " || rawInput.substr(0, 2) == "F ") {
			flagMode = true;
			goodInput = true;
			cell = rawInput.substr(2);
		}
		//Allow an uncovering function
		else if (rawInput.substr(0, 2) != "f " && rawInput.substr(0, 2) != "F " && rawInput.length() > 1 && rawInput.length() < 4) {
			flagMode = false;
			goodInput = true;
			cell = rawInput;
		}
		else {
			cout << "The input is formatted incorrectly. Type h for help.\n";
		}
	} while (goodInput == false);	
	
	//Convert user input to cell data
	rawRow = cell.substr(1);
	row = verifyNumber(rawRow);
	rawCol = cell.substr(0, 1);
	col = letterToNumber(rawCol);	
	
	//cout << row << " " << col << "\n";
	
	//Verify user input
	if (row < 1 || row > size) {
		cout << "Invalid row selected. Please enter a valid cell.\n";
	}
	if (col < 1 || col > size) {
		cout << "Invalid column selected. Please enter a valid cell.\n";
	}	
	else if (flagMode == true){
		gameBoard[row][col] = 'f';
	}
	else if (flagMode == false) {
		//If it's the first move, use an 's' so that the mines can be set properly
		if (firstMove) {
			solnBoard[row][col] = 's';
			setMine(gameBoard, solnBoard, size);
			calcBoard(solnBoard, size);
			gameBoard[row][col] = solnBoard[row][col];
		}
		else {
			//Uncover the solution board cell
			gameBoard[row][col] = solnBoard[row][col];
		}
		//Cascade the zeroes if 0 uncovered
		if (solnBoard[row][col] == '0') {
			cascadeZeroes(gameBoard, solnBoard, size, row, col);
		}
	}
}

//Convert letter to number
int letterToNumber(string letterIn)
{
	//This is a masterpiece do not criticize
	//Create reference strings for the alphabet
	string alphabet = "abcdefghijklmnopqrstuvwxyz";
	string ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	for (int i = 0; i < 26; i++) {
		if (letterIn == alphabet.substr(i, 1) || letterIn == ALPHABET.substr(i, 1)) {
			return i + 1;
		}		
	}
	return 0;
}

//Convert number to number
int verifyNumber(string numberIn) 
{
	//This is a masterpiece do not criticize
	//Create reference strings for the numbers
	string ones = "01234567890";
	string tens = "1011121314151617181920";

	for (int i = 0; i < 11; i++) {
		if (numberIn == ones.substr(i, 1)) {
			return i;
		}
		else if (numberIn == tens.substr(2*i, 2)) {
			return 10 + i;
		}
	}
	return 0;
}

//Check neighboring cells to calculate neighboring mines
char numNeighboringMines(int row, int col, char solnBoard[][22], int size)
{
	int closeMines = 0;

	if (solnBoard[row - 1][col - 1] == 'x') {
		closeMines++;
	}
	if (solnBoard[row - 1][col] == 'x') {
		closeMines++;
	}
	if (solnBoard[row - 1][col + 1] == 'x') {
		closeMines++;
	}
	if (solnBoard[row][col + 1] == 'x') {
		closeMines++;
	}
	if (solnBoard[row + 1][col + 1] == 'x') {
		closeMines++;
	}
	if (solnBoard[row + 1][col] == 'x') {
		closeMines++;
	}
	if (solnBoard[row + 1][col - 1] == 'x') {
		closeMines++;
	}
	if (solnBoard[row][col - 1] == 'x') {
		closeMines++;
	}

	//Warning do not look at this
	//Convert integer numbers to char numbers
	switch (closeMines) {
	case 0:
		return '0';
		break;
	case 1:
		return '1';
		break;
	case 2:
		return '2';
		break;
	case 3:
		return '3';
		break;
	case 4:
		return '4';
		break;
	case 5:
		return '5';
		break;
	case 6:
		return '6';
		break;
	case 7:
		return '7';
		break;
	case 8:
		return '8';
		break;
	}
}

//Check to see if game is over with a win
bool checkWin(char gameBoard[][22], char solnBoard[][22], int size)
{
	for (int row = 1; row <= size; row++) {
		for (int col = 1; col <= size; col++) {
			//If the cell is not a mine, it must be uncovered
			if (solnBoard[row][col] != 'x') {
				if (gameBoard[row][col] == '?' || gameBoard[row][col] == 'f') {
					return false;
				}
			}
		}
	}
	endGame(true);
	return true;
}

//Check to see if game is over with a loss
bool checkLoss(char gameBoard[][22], char solnBoard[][22], int size)
{
	for (int row = 1; row <= size; row++) {
		for (int col = 1; col <= size; col++) {
			//If a mine is uncovered, loss
			if (gameBoard[row][col] == 'x') {
				endGame(false);
				return true;
			}
		}
	}
	return false;
}

//If a zero is uncovered, all adjacent squares must be uncovered
void cascadeZeroes(char gameBoard[][22], char solnBoard[][22], int size, int row, int col)
{
	if (gameBoard[row - 1][col - 1] == '?') {
		gameBoard[row - 1][col - 1] = solnBoard[row - 1][col - 1];
		if (solnBoard[row - 1][col - 1] == '0') {
			cascadeZeroes(gameBoard, solnBoard, size, row - 1, col - 1);
		}
	}
	if (gameBoard[row - 1][col] == '?') {
		gameBoard[row - 1][col] = solnBoard[row - 1][col];
		if (solnBoard[row - 1][col] == '0') {
			cascadeZeroes(gameBoard, solnBoard, size, row - 1, col);
		}
	}
	if (gameBoard[row - 1][col + 1] == '?') {
		gameBoard[row - 1][col + 1] = solnBoard[row - 1][col + 1];
		if (solnBoard[row - 1][col + 1] == '0') {
			cascadeZeroes(gameBoard, solnBoard, size, row - 1, col + 1);
		}
	}
	if (gameBoard[row][col + 1] == '?') {
		gameBoard[row][col + 1] = solnBoard[row][col + 1];
		if (solnBoard[row][col + 1] == '0') {
			cascadeZeroes(gameBoard, solnBoard, size, row, col + 1);
		}
	}
	if (gameBoard[row + 1][col + 1] == '?') {
		gameBoard[row + 1][col + 1] = solnBoard[row + 1][col + 1];
		if (solnBoard[row + 1][col + 1] == '0') {
			cascadeZeroes(gameBoard, solnBoard, size, row + 1, col + 1);
		}
	}
	if (gameBoard[row + 1][col] == '?') {
		gameBoard[row + 1][col] = solnBoard[row + 1][col];
		if (solnBoard[row + 1][col] == '0') {
			cascadeZeroes(gameBoard, solnBoard, size, row + 1, col);
		}
	}
	if (gameBoard[row + 1][col - 1] == '?') {
		gameBoard[row + 1][col - 1] = solnBoard[row + 1][col - 1];
		if (solnBoard[row + 1][col - 1] == '0') {
			cascadeZeroes(gameBoard, solnBoard, size, row + 1, col - 1);
		}
	}
	if (gameBoard[row][col - 1] == '?') {
		gameBoard[row][col - 1] = solnBoard[row][col - 1];
		if (solnBoard[row][col - 1] == '0') {
			cascadeZeroes(gameBoard, solnBoard, size, row, col - 1);
		}
	}
}
