/*******************************************************
Filename: Minesweeper.cpp
Author: Shovik Shyamsundar
Class: CSCI 1730
Project 1
******************************************************/
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <fstream>
#include <cerrno>
#include <cstdlib>
#include <vector>
#include <sstream>
#include <algorithm>

#include "Minesweeper.h"

using namespace std;

/**
 * Creates Minesweeper object
 * @param numOfRows number of rows on grid
 * @param numOfCols number of columns on grid
 * @param numOfMines number of mines on grid
 * @param mineSet array containing mine coordinates
 */
Minesweeper::Minesweeper(int numOfRows, int numOfCols, int numOfMines, int (*mineSet)[2])
{
  numMines = numOfMines;
  numRows = numOfRows;
  numCols = numOfCols;
  rounds = 0;
  lost = false;
  won = false;

  if ((numRows < 0) || (numRows > 10) || (numCols < 0) || numCols > 10)
    {
      cout << endl;
      cout <<"ಠ_ಠ says, \"Cannot create a mine filed with that many rows and/or columns!\"" << endl;
      exit(0);
    }

  mineLocs = new int *[numMines];
  for (int n = 0; n < numMines; n++)
    mineLocs[n] = new int [2];

  for (int a = 0; a < numMines; a++)
    {
      for (int b = 0; b < 2; b++)
        {
          mineLocs[a][b] = mineSet[a][b];
        }
    }
  for (int c = 0; c < numRows; c++)
    {
      for (int d = 0; d < numCols; d++)
        {
          checked[c][d] = false;
        }
    }

  for (int i = 0; i < numRows; i++)
    {
      for (int j = 0; j < numCols; j++)
        {
	  board[i][j] = " ";
        }
    }
  
}

/**
 * Destructor for Minesweeper object
 */
Minesweeper::~Minesweeper()
{
  for (int i =0; i < numMines; i++)
    delete [] mineLocs[i];
  delete [] mineLocs;
}

/**
 * Displays number of rounds consumed and grid
 */
void Minesweeper::display() 
{
  cout << endl;
  cout << "Rounds Completed: " << rounds << endl;
  cout << endl;
  for (int i = 0; i < numRows; i++)
    {
    for (int j = 0; j < numCols;  j++)
	{
	  if (j == 0)
	    {
	      cout << i << " ";
	      for (int k = 0; k < (numCols+1); k++)
		{
		  if (k < numCols)
		    cout << "| " << board[i][k] << " ";
		  else
		    cout << "|";
		}
	      cout << endl;
	    }  
	  if (i == (numRows - 1))
	    {
	      if (j == 0)
		cout << "    " << j;
	      else
		cout << "   " <<  j;
		}
	    }
    }
    cout << " " << endl;
  }

/**
 * Displays prompt screen and takes in user input
 */
void Minesweeper::prompt()
{
  cout << "minesweeper$ ";
  string input;
  getline(cin, input);

  vector<string> tokens;
  string temp;
  stringstream os(input);

  while (os >> temp)
    tokens.push_back(temp);
  
  if (((tokens.front()== "r" || tokens.front()== "reveal" || tokens.front()== "m" || tokens.front()== "mark"
       || tokens.front()== "g" || tokens.front()== "guess") && tokens.size()==3) || ((tokens.front()== "h" || 
							       tokens.front()== "help") && tokens.size() ==1) 
      || ((tokens.front()== "q" || tokens.front()== "quit") && tokens.size()==1))
    {
      if (tokens.front() == "r" || tokens.front() == "reveal") 
	{
	  rounds++;
	  int col = atoi(tokens.back().c_str());
	  tokens.pop_back();
	  int row = atoi(tokens.back().c_str());
	  reveal(row, col);
	  
	}
      
      if (tokens.front() == "m" || tokens.front() == "mark")
	{
	  int col = atoi(tokens.back().c_str());
	  tokens.pop_back();
	  int row = atoi(tokens.back().c_str());
	  mark(row, col);
	}
      
      if (tokens.front() == "g" || tokens.front() == "guess")
	{
	  int col = atoi(tokens.back().c_str());
	  tokens.pop_back();
	  int row = atoi(tokens.back().c_str());
	  guess(row, col);
	}
      
      if (tokens.front() == "h" || tokens.front() == "help")
	{
	  cout << " " << endl;
	  cout << "Commands Available..." << endl;
	  cout << " - Reveal: r/reveal row col" << endl;
	  cout << " -   Mark: m/mark   row col" << endl;
	  cout << " -  Guess: g/guess  row col" << endl;
	  cout << " -   Help: h/help" << endl;
	  cout << " -   Quit: q/quit" << endl;
	}
      if (tokens.front() == "q" || tokens.front() == "quit")
	{ 
	  cout << endl;
	  cout << "ლ(ಠ_ಠლ)" << endl;
	  cout << "Y U NO PLAY MORE?" << endl;
	  cout << "Bye!" << endl;
	  exit(0);	    
	}
    }
  else
    {
      rounds++;
      cout << endl;
      cout << "ಠ_ಠ says, \"Command not recognized!\"" << endl;
    }

}

/**
 * Places a F in corrsponding cell
 * Marks as definitely containing a mine
 * @param row row number of cell
 * @param col column number of cell
 * @return number of adjacent mines
 */
void Minesweeper::mark(int row, int col)
{
  if (isInBounds(row, col))
    {
      board[row][col] = "F";
      rounds++;
    }
 
}

/**
 * Places a ? in corresponding cell
 * Marks as potentially containing a mine
 * @param row row number of cell
 * @param col column number of cell
 */
void Minesweeper::guess(int row, int col)
{
  if (isInBounds(row, col))
    {
      board[row][col] = "?";
      rounds++;
    }
}

/**
 * Reveals number of mines adjacent to row and column provided.
 * Puts number of mines adjacent to row and column provided in 
 * that cell.  If number of mines is 0 then the function is 
 * called for each adjacent cell using recursion.
 * @param row row number of cell
 * @param col column number of cell
 */
void Minesweeper::reveal(int row, int col)
{  
  bool mineFound = false;
  if (isInBounds(row, col) && checked[row][col] == false)
    {
      for (int i = 0; i < numMines; i++)
	{
	  if (mineLocs[i][0] == row && mineLocs[i][1] == col)
	    {
	      mineFound = true;
	      lost = true;
	      break;
	    }
	}
      if (!mineFound)
	{
	  int mines = mineCount(row, col);
	  string mineString;
	  ostringstream convert;
	  convert << mines;
	  mineString = convert.str();
	  board[row][col] = mineString;
	  checked[row][col] = true;
 
	  if(board[row][col] == "0" || board[row][col] == "F" || board[row][col] == "?")
	    {
	      reveal(row-1, col-1);
	      reveal(row-1, col);
	      reveal(row-1, col+1);
	      reveal(row, col-1);
	      reveal(row+1, col);
	      reveal(row, col+1);
	      reveal(row+1, col-1);
	      reveal(row+1, col+1);
	    }
	}
      else
	return;
    } 
}

/**
 * Counts number of mines adjacent to row and column provided
 * @param row row number of cell
 * @param col column number of cell
 * @return number of adjacent mines
 */
int Minesweeper::mineCount(int row, int col)
{
  int count = 0;
  for (int i=-1; i<=1; i++){
    for (int j=-1; j<=1; j++){
      if (i==0 && j==0) continue;
      if (row + i < 0 || row + i >= numRows) continue;
      if (col + j < 0 || col + j >= numCols) continue;
      for (int k = 0; k < numMines; k++)
	{
	  if (mineLocs[k][0] == row + i && mineLocs[k][1] == col + j)
	    count++;
	}
    }
  }
  return count;
}

/**
 * Checks to see if row and column provided are within the grid
 * @param row row number of cell
 * @param cols column number of cell
 * @return true if in bounds or false if not in bounds
 */
bool Minesweeper::isInBounds(int row, int col)
{
  if (row < numRows && row >=0 && col < numCols && col >=0)
    return true;
  else 
    return false;
}

/**
 * Runs minesweeper game
 */
void Minesweeper::run()
{
  
  int markCount;
  int guessCount;
  
  cout << "        _ " << endl
       << "  /\\/\\ (_)_ __   ___  _____      _____  ___ _ __   ___ _ __" << endl
       << " /    \\| | '_ \\ / _ \\/ __\\ \\ /\\ / / _ \\/ _ \\ '_ \\ / _ \\ '__|" << endl 
       << "/ /\\/\\ \\ | | | |  __/\\__ \\\\ V  V /  __/  __/ |_) |  __/ |" << endl
       << "\\/    \\/_|_| |_|\\___||___/ \\_/\\_/ \\___|\\___| .__/ \\___|_|" << endl
       << "                                 RECURSIVE |_| EDITION" << endl;

  while (!lost && !won)
    {
      markCount = 0;
      guessCount = 0;
      for (int i = 0; i < numRows; i++)
	{
	  for (int j = 0; j < numCols; j++)
	    {
	      if (board[i][j] == "F")
		markCount++;
	      if (board[i][j] == "?")
		guessCount++;
	    }
	}
      if (markCount == numMines && guessCount == 0)
	{
	  won = true;
	  break;
	}
      prompt();
      if(!lost && !won)
	display();
    }
  if (lost)
     {
      cout << endl
	   << "Oh no... You revealed a mine!" << endl
	   << endl
	   << "  __ _  __ _ _ __ ___   ___    _____   _____ _ __" << endl
	   << " / _` |/ _` | '_ ` _ \\ / _ \\  / _ \\ \\ / / _ \\ '__|" << endl
	   << "| (_| | (_| | | | | | |  __/ | (_) \\ V /  __/ |" << endl
	   << " \\__, |\\__,_|_| |_| |_|\\___|  \\___/ \\_/ \\___|_|" << endl
      << " |___/" << endl;
      cout << endl;
      
      exit(0);
    }
   if (won)
    {
      int score = (numRows * numCols) - numMines - rounds;
      cout << endl
	   << " ░░░░░░░░░▄░░░░░░░░░░░░░░▄░░░░ \"So Doge\"" << endl
	   << " ░░░░░░░░▌▒█░░░░░░░░░░░▄▀▒▌░░░ " << endl
	   << " ░░░░░░░░▌▒▒█░░░░░░░░▄▀▒▒▒▐░░░ \"Such Score\"" << endl
	   << " ░░░░░░░▐▄▀▒▒▀▀▀▀▄▄▄▀▒▒▒▒▒▐░░░ " << endl
	   << " ░░░░░▄▄▀▒░▒▒▒▒▒▒▒▒▒█▒▒▄█▒▐░░░ \"Much Minesweeping\"" << endl
	   << " ░░░▄▀▒▒▒░░░▒▒▒░░░▒▒▒▀██▀▒▌░░░ " << endl
	   << " ░░▐▒▒▒▄▄▒▒▒▒░░░▒▒▒▒▒▒▒▀▄▒▒▌░░ \"Wow\"" << endl
	   << " ░░▌░░▌█▀▒▒▒▒▒▄▀█▄▒▒▒▒▒▒▒█▒▐░░ " << endl
	   << " ░▐░░░▒▒▒▒▒▒▒▒▌██▀▒▒░░░▒▒▒▀▄▌░ " << endl
	   << " ░▌░▒▄██▄▒▒▒▒▒▒▒▒▒░░░░░░▒▒▒▒▌░ " << endl
	   << " ▀▒▀▐▄█▄█▌▄░▀▒▒░░░░░░░░░░▒▒▒▐░ " << endl
	   << " ▐▒▒▐▀▐▀▒░▄▄▒▄▒▒▒▒▒▒░▒░▒░▒▒▒▒▌ " << endl
	   << " ▐▒▒▒▀▀▄▄▒▒▒▄▒▒▒▒▒▒▒▒░▒░▒░▒▒▐░ " << endl
	   << " ░▌▒▒▒▒▒▒▀▀▀▒▒▒▒▒▒░▒░▒░▒░▒▒▒▌░ " << endl
	   << " ░▐▒▒▒▒▒▒▒▒▒▒▒▒▒▒░▒░▒░▒▒▄▒▒▐░░ " << endl
	   << " ░░▀▄▒▒▒▒▒▒▒▒▒▒▒░▒░▒░▒▄▒▒▒▒▌░░ " << endl
	   << " ░░░░▀▄▒▒▒▒▒▒▒▒▒▒▄▄▄▀▒▒▒▒▄▀░░░ CONGRATULATIONS!" << endl
	   << " ░░░░░░▀▄▄▄▄▄▄▀▀▀▒▒▒▒▒▄▄▀░░░░░ YOU HAVE WON!" << endl
	   << " ░░░░░░░░░▒▒▒▒▒▒▒▒▒▒▀▀░░░░░░░░ SCORE: " << score << endl
	   << endl;
     }
}

