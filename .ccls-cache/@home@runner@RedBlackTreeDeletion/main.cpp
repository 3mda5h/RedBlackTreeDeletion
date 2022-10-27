/* red black tree - binary search tree but:
     - A node is either red or black.
     - The root is black.
     - All leaves (NULL) are black.
     - Both children of every red node are black.
     - Every simple path from a given node to any of its descendant leaves 
       contains the same number of black nodes.
this balances the bst
insertion AND deletion
Emily MacPherson
5/22/22
*/

#include <iostream>
#include <cstring>
#include <fstream>
#include <string>
#include <iomanip>
#include "Tree.h"

using namespace std;

void consoleInput(Tree* tree);
void fileInput(Tree* tree);

int main() 
{
  Tree* tree = new Tree();
  char input[100];
  while(strcmp(input, "quit") != 0)
  {
    cout << "Enter ADD, READ, DISPLAY, REMOVE, SEARCH or QUIT" << endl;
    cin.getline(input, 100);
    for(int i = 0; i < strlen(input); i++) input[i] = tolower(input[i]);
    if(strcmp(input, "add") == 0) consoleInput(tree); 
    else if(strcmp(input, "read") == 0) fileInput(tree);
    else if(strcmp(input, "remove") == 0)
    {
      cout << "Number?" << endl;
      cin.getline(input, 100);
      tree->preRemove(atoi(input));
    }
    else if(strcmp(input, "search") == 0)
    {
      cout << "Number?" << endl;
      cin.getline(input, 100);
      if(tree->search(atoi(input)) != NULL)
      {
        cout << "This number is in the tree" << endl;
      }
      else cout << "This number is not in the tree :(" << endl;
    }
    else if(strcmp(input, "display") == 0)
    {
      tree->display();
    }
    cout << "----------" << endl;
  }
}

//read in list of numbers seperated by spaces from the console
void consoleInput(Tree* tree)
{
  char input[10000];
  cout << "Enter a list of numbers seperated by spaces" << endl;
  cin.getline(input, 10000);
  int j = 0;
  char currentNumber[5]; //current number
  for(int i = 0; i < strlen(input); i++)
  {
    if(input[i] != ' ')
    {
      currentNumber[j] = input[i]; //add current char in input to current number 
      j++; //j keeps track of where we are in currentnumber[]
    }
    if(input[i] == ' ' || i == strlen(input) - 1)//if reaches space or end of input array 
    {
      tree->insert(atoi(currentNumber));
      j = 0; 
      for(int i = 0; i < 4; i++) //reset current number
      {
        currentNumber[i] = 0;
      }
    }
  }
  cout << "Insert successful" << endl;
  tree->display();
}

//read in list of numbers seperated by spaces from a file
void fileInput(Tree* tree)
{
  char input[100];
  int number;
  cout << "Name of file?" << endl;
  cin.getline(input, 100);
  ifstream file(input);
  if(!file.is_open())
  {
    cout << "Could not open file D:" << endl;
    return;
  }
  while (file >> number) //reads in ints from file till end of file reached
  {
    tree->insert(number);
  }
  cout << "Insert successful" << endl;
  tree->display();
}