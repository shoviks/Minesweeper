/*******************************************************
Filename: main.cpp
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

#include "Minesweeper.cpp"

int main(int argc, char *argv[])
{
  int numRows, numCols, numMines;
  int mineLocations [numMines][2];
  int count = 0;
  string line;
  if (argc != 2)
    {
      cout << "Usage: " << argv[0] << " <Filename>" << endl;
      exit(1);
    }
  char *seedFile = argv[1];
  ifstream infile(seedFile);
  while(!infile.eof()&&count < (numMines + 2))
    {
      getline(infile, line);
      if (count == 0)
	{
	  vector<string> tokens;
	  string temp;
	  stringstream os(line);
	 
	  while (os >> temp)
	    tokens.push_back(temp);

	  const char* front = tokens.front().c_str();
	  const char* back = tokens.back().c_str();

	  numRows = atoi(front);
	  numCols = atoi(back);
	}
      else if (count == 1)
	  numMines = atoi(line.c_str());
      if (count == 2)
	int mineLocations [numMines][2];
      if (count >= 2 && numMines > 0)
	{
	  vector<string> tokens2;
          string temp2;
          stringstream os(line);
          
          while (os >> temp2)
            tokens2.push_back(temp2);

          const char* front2 = tokens2.front().c_str();
          const char* back2 = tokens2.back().c_str();
          
	  mineLocations[count - 2][0] = atoi(front2);
	  mineLocations[count - 2][1] = atoi(back2);
	}
      count++;
    }
  

  Minesweeper * game = new Minesweeper(numRows, numCols, numMines, mineLocations);
  game->run();

}
 
