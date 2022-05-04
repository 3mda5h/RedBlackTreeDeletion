#include <iostream>
#include "Tree.h"
#include <cstring>

using namespace std;

Tree::Tree()
{
  root = NULL;
}

void Tree::insert(int number)
{
  insert_impl(root, number); 
}

//insert a node into tree maintaining red black property
void Tree::insert_impl(Node* current, int number)
{
  //1. inserting root - root must be black
  if(root == NULL)
  {
    Node* newRoot = new Node();
    newRoot->number = number;
    newRoot->color = "black";
    root = newRoot;
    return;
  }
  if(current != NULL)
  {
    if(number > current->number)
    {
      if(current->right != NULL)
      {
        insert_impl(current->right, number);
      }
      else //insert new node in empty slot
      {
        Node* newChild = new Node();
        newChild->number = number;
        newChild->parent = current;
        current->right = newChild;
        if(strcmp(current->color, "black") != 0) fixInsert(newChild);  //else: case 2. parent is red, don't need to fix colors
      }
    }
    else //number is less than or equal to current
    {
      if(current->left != NULL)
      {
        insert_impl(current->left, number);
      }
      else
      {
        Node* newChild = new Node();
        newChild->number = number;
        newChild->parent = current;
        current->left = newChild;
        if(strcmp(current->color, "black") != 0) fixInsert(newChild);
      }
    }
  }
}

//re-establish red-black property
void Tree::fixInsert(Node* node)
{
  Node* parent = node->parent;
  Node* grandpa = parent->parent;
  Node* uncle;
  if(parent == grandpa->left) uncle = grandpa->right;
  else uncle = grandpa->left; 
  //3. parent and uncle of node are red - change parent and uncle to black, grandparent to red
  if(strcmp(node->color, "red") == 0 && strcmp(parent->color, "red") == 0 && uncle != NULL && strcmp(uncle->color, "red") == 0)
  {    
    caseThreeIns(node);
    //fix tree higher up
    Node* current = grandpa;
    while(current != NULL && current->parent != NULL && current->parent->parent != NULL)
    {
      fixInsert(current);
      current = current->parent;
    }
  }
  else if(strcmp(node->color, "red") == 0 && strcmp(parent->color, "red") == 0 && (uncle == NULL || strcmp(uncle->color, "black") == 0)) //NULL is black
  {
    //4. uncle is black 
    if(parent == grandpa->left && node == parent->right) //parent is left of grandpa and node is right of parent
    {
      //tree rotation - node goes to parent's spot 
      grandpa->left = node;
      node->parent = grandpa;
      //left child of node becomes right child of parent
      parent->right = node->left;
      if(parent->right != NULL) parent->right->parent = parent;
      //parent becomes the left child of node
      node->left = parent;
      parent->parent = node;
      //update parent and node pointers
      node = node->left; 
      parent = node->parent;
      //update uncle
      if(parent == grandpa->left) uncle = grandpa->right;
      else uncle = grandpa->left;
    }
    else if(parent == grandpa->right && node == parent->left) //parent is right of grandpa and node is left of parent
    {
      //tree rotation - node goes to parent's spot 
      grandpa->right = node;
      node->parent = grandpa;
      //right child of node becomes left child of parent
      parent->left = node->right;
      if(parent->left != NULL) parent->left->parent = parent;
      //parent becomes the right child of node
      node->right = parent;
      parent->parent = node;
      //update parent and node pointers
      node = node->right; 
      parent = node->parent;
      //update uncle
      if(parent == grandpa->left) uncle = grandpa->right;
      else uncle = grandpa->left;
    }
    if(node->parent == grandpa->left && node == parent->left) // parent left node left
    {
      //5 - (happens after case 4) - grandparent and uncle are black
      //grandparent becomes red, parent becomes black
      grandpa->color = "red";
      parent->color = "black";
      //parent takes the place of grandpa
      if(grandpa->parent != NULL)
      {
        if(grandpa == grandpa->parent->right) grandpa->parent->right = parent; //if grandpa was right of his parent
        else grandpa->parent->left = parent; //grandpa was left of his parent
      }
      parent->parent = grandpa->parent;
      //right child of parent transfers to left child of grandpa
      grandpa->left = parent->right;
      if(grandpa->left != NULL) grandpa->left->parent = grandpa;
      //fix family tree stuff
      parent->right = grandpa;
      grandpa->parent = parent;
      if(root == grandpa) root = parent; //if granpa was old root parent is now root
    }
    if(node->parent == grandpa->right && node == parent->right) //parent rght node right
    {
      //5 - (happens after case 4) - grandparent and uncle are black
      //grandparent becomes red, parent becomes black
      grandpa->color = "red";
      parent->color = "black";
      //parent takes the place of grandpa
      if(grandpa->parent != NULL)
      {
        if(grandpa == grandpa->parent->right) grandpa->parent->right = parent; //if grandpa was right of his parent
        else grandpa->parent->left = parent; //grandpa was left of his parent
      }
      parent->parent = grandpa->parent;
      //left child of parent transfers to right child of grandpa
      grandpa->right = parent->left;
      if(grandpa->right != NULL) grandpa->right->parent = grandpa;
      //fix family tree stuff
      parent->left = grandpa;
      grandpa->parent = parent;
      if(root == grandpa) root = parent; //if granpa was old root parent is now root
    }
  }
}

//3. parent and uncle of node are red - change parent and uncle to black, grandparent to red
void Tree::caseThreeIns(Node* node)
{
  if(node != NULL && node->parent != NULL && node->parent->parent != NULL)
  {
    Node* grandpa = node->parent->parent;
    Node* uncle;
    if(node->parent == grandpa->left) uncle = grandpa->right;
    else uncle = grandpa->left;
    if(uncle != NULL && strcmp(node->parent->color, "red") == 0 && strcmp(uncle->color, "red") == 0)
    {
      node->parent->color = "black";       
      uncle->color = "black";
      if(grandpa != NULL && grandpa != root) grandpa->color = "red";
      caseThreeIns(grandpa);
    }
  }
}

Node* Tree::search(int number)
{
  Node* current = root;
  while(current != NULL && current->number != number) //for now removethis is the current node
  {
    if(number > current->number) current = current->right;
    else current = current->left;
  }
  if(current == NULL) return NULL;
  else return current;
}

void Tree::remove(int number)
{
  Node* removeThis = search(number);
  Node* parent = removeThis->parent;
  if(removeThis->left == NULL && removeThis->right == NULL && strcmp(removeThis->color, "red") == 0) //removeThis has no children and is red  
  {
    if(removeThis->number > parent->number) parent->right = NULL;
    else parent->left = NULL;
    delete removeThis;
    cout << "Number removed" << endl;
    return;
  }
  
  //2. removeThis has only has one non-leaf child
  if(removeThis->left == NULL ^ removeThis->right == NULL) //^ = exclusive or - returns true if only one of the statements is true (so if one child is null but not both)
  {
    Node* child;
    if(removeThis->right == NULL) child = removeThis->left;
    else child = removeThis->right;
    //one black the other red
    if(strcmp(removeThis->color, "red") == 0 || (strcmp(removeThis->color, "black") == 0 && strcmp(child->color, "red") == 0)) 
    {
      replace(removeThis, child);
      if(strcmp(child->color, "red") == 0) child->color = "black"; //if removeThis was black and child was red
    }
    //both black
    if(strcmp(removeThis->color, "black") == 0 && strcmp(child->color, "black")
    {
      replace(removeThis, child);
      Node* node = child;
      Node* parent = node->parent;
      Node* sibling;
      if(node == node->parent->left) sibling = node->parent->right
      else sibling = node->parent->left;
      //1. node is new root
      if(node == root) return;
      //2. sibling is red - rotate sibling thru parent
      if(sibling != NULL && strcmp(sibling->color, "red") == 0)
      {
        //is this the same rotation as used before??
        if(sibing == sibling->parent->right)
        {
          if(parent->parent != NULL)
          {
            if(parent == parent->parent->left) parent->parent->left = sibling
            else parent->parent->right = sibling
          }
          sibling->parent = parent->parent;
          parent->right = sibling->left
          sibling->left = parent;
          sibling = parent->right;
        }
        else if(sibing == sibling->parent->left)
        {
          if(parent->parent != NULL)
          {
            if(parent == parent->parent->left) parent->parent->left = sibling
            else parent->parent->right = sibling
          }
          sibling->parent = parent->parent;
          parent->left = sibling->right
          sibling->right = parent;
          sibling = parent->left;
        }
      }
      //3. sibling is black
      if(sibling == NULL || strcmp(sibling->color, "black") == 0)
      {
        sibling->color = "red";
        //case 1;
        if(parent == root) return;
      }
      //4. parent is read sibling and sibling's children are black
      if(strcmp(sibling->color, "black") == 0 && (sibling->left == NULL || strcmp(sibling->left->color, "black") == 0) 
        && (sibling->right != NULL || strcmp(sibling->right->color, "black") == 0))
      {
        parent->color = "black";
        sibling->color = "red";
      }
      //5 - rotate thru sibling
      if(strcmp(sibling->color, "black") == 0 && (strcmp(sibling->right->color, "red") == 0 ^ strcmp(sibling->left->color, "red") == 0)) //if one of sibling's children is red not both
      {
        if(sibling == parent->left && strcmp(sibling->right->color, "red") == 0) //sibling left of parent, red child right of sibling
        {
          sibling->color = "red";
          sibling->right->color = "black"
        }
        else if(sibling == parent->right && strcmp(sibling->left->color, "red") == 0)
        {
          sibling->color = "red";
          sibling->left->color = "black"
        }
      }
    }
    /*if(removeThis->number > parent->number) //removeThis is the right child of parent
    {
      if(removeThis->right != NULL) parent->right = removeThis->right; //removeThis has a right child, connect this child with parent of removeThis
      else parent->right = removeThis->left;
    }
    else //removeThis is the left child of parent
    {
      if(removeThis->right != NULL) parent->left = removeThis->right; 
      else parent->left = removeThis->left;
    }
    delete removeThis;
    cout << "Number removed" << endl;
    return; */
    }

  //3. removeThis has two children - find next largest number (right once, then left until end)
  Node* previous = removeThis;
  Node* nextLargest = removeThis->right;
  while(nextLargest->left != NULL) 
  {
    previous = nextLargest;
    nextLargest = nextLargest->left;
  }
  removeThis->number = nextLargest->number; //replacing the number were removing with the next largest number in tree
  if(removeThis == previous)//found next highest by just going right once
  {
    if(nextLargest->right != NULL) previous->right = nextLargest->right;
    else previous->right = NULL;
  }
  else //had to go left to find next highest
  {
    if(nextLargest->right != NULL) previous->left = nextLargest->right;
    else previous->left = NULL; 
  }
  delete nextLargest;
  cout << "Number removed" << endl;
}

//child goes into parent's spot, parent is deleted
void Tree::replace(Node* removeThis, Node* child)
{
  if(removeThis == removeThis->parent->right) removeThis->parent->right = child;
  else removeThis->parent->left = child;
  child->parent = removeThis->parent;
  if(root == removeThis) root = child;
  delete removeThis;
}

void Tree::display()
{
  display_impl(root, 0);
}

//displays tree visually using tabs
void Tree::display_impl(Node* current, int level)
{ 
  if(current == NULL && level == 0)
  {
    cout << "Tree is empty :(" << endl;
    return;
  }
  else if(current != NULL)
  {
    display_impl(current->right, level + 1);
    printTabs(level);
    if(strcmp(current->color, "red") == 0) cout << "\033[31m";
    cout << current->number << endl;
    cout << "\033[37m";
    display_impl(current->left, level + 1);
  }
  else return;
}

//prints tabs according to level of tree that value is at
void Tree::printTabs(int level)
{
  for(int i = 0; i < level; i++)
  {
    cout << "     ";    
  }
}