#ifndef TREE_H
#define TREE_H

#include <iostream>

using namespace std;

struct Node
{
  Node* left = NULL;
  Node* right = NULL;
  Node* sibling = NULL;
  Node* parent = NULL;
  const char* color = "red";
  int number;
};

class Tree
{
  public:
  Tree();
  void insert(int number);
  void insert_impl(Node* current, int number); //implementation functions so main doesn't have to acess root
  void fixInsert(Node* newChild);
  void caseThreeIns(Node* current);
  Node* search(int number);
  void preRemove(int number);
  void remove(Node* removeThis);
  void bothBlack(Node* parent, Node* child);
  void replace(Node* parent, Node* child);
  void display();
  void display_impl(Node* current, int level);
  void printTabs(int level);
  private: 
  Node* root;
};

#endif