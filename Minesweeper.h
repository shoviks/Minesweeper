/*******************************************************
Filename: Minesweeper.h
Author: Shovik Shyamsundar
Class: CSCI 1730
Project 1
******************************************************/

#ifndef Minesweeper_h
#define Minesweeper_h 

#include <string>
#include <vector>

using namespace std;

class Minesweeper
{
 private:
  int numRows, numCols, numMines, rounds;
  string board[10][10];
  int **mineLocs;
  bool checked[10][10];
  bool lost;
  bool won;
 public:
  Minesweeper(int, int, int, int(*)[2]);
  ~Minesweeper();
  void display();
  void prompt();
  void mark(int, int);
  void guess(int, int);
  void reveal(int, int);
  int mineCount(int, int);
  bool isInBounds(int, int);
  void run();
};
#endif
