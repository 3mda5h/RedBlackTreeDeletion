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
    //cout << "before case 3:" << endl;
    //display();    
    caseThreeIns(node);
    //fix tree higher up
    Node* current = grandpa;
    while(current != NULL && current->parent != NULL && current->parent->parent != NULL)
    {
      fixInsert(current);
      current = current->parent;
    }
    //cout << "after case 3:" << endl;
    //cout << "parent is " << parent->number << endl;
    //cout << "node is " << node->number << endl;
    //display();
  }
  else if(strcmp(node->color, "red") == 0 && strcmp(parent->color, "red") == 0 && (uncle == NULL || strcmp(uncle->color, "black") == 0)) //NULL is black
  {
    //4. uncle is black 
    if(parent == grandpa->left && node == parent->right) //parent is left of grandpa and node is right of parent
    {
      //cout << "before case 4:" << endl;
      //display();
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
      //cout << "after case 4:" << endl;
      //display();
    }
    else if(parent == grandpa->right && node == parent->left) //parent is right of grandpa and node is left of parent
    {
      //cout << "before case 4:" << endl;
      //display();
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
      //cout << "after case 4:" << endl;
      //display();
    }
    if(node->parent == grandpa->left && node == parent->left) // parent left node left
    {
      //5 - (happens after case 4) - grandparent and uncle are black
      //grandparent becomes red, parent becomes black
      //cout << "before case 5:" << endl;
      //cout << "parent is " << parent->number << endl;
      //cout << "node is " << node->number << endl;
      //display();
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
      //cout << "after case 5:" << endl;
      //display();
    }
    if(node->parent == grandpa->right && node == parent->right) //parent rght node right
    {
      //5 - (happens after case 4) - grandparent and uncle are black
      //grandparent becomes red, parent becomes black
      //cout << "before case 5:" << endl;
      //cout << "parent is " << parent->number << endl;
      //cout << "node is " << node->number << endl;
      //display();
      //display();
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
      //cout << "after case 5:" << endl;
      //display();
    }
  }
}

//(for insertion!!) parent and uncle of node are red - change parent and uncle to black, grandparent to red
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

//finds node with given number, returns that node
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

//find correct node to remove
void Tree::preRemove(int number)
{
  Node* removeThis = search(number);
  if(removeThis == NULL) cout << "this number is not in the tree" << endl;
  else if(removeThis == root && removeThis->left == NULL && removeThis->right == NULL) 
  {
    delete removeThis; //literally just deleting a tree w/ 1 value in it
    root = NULL;
  }
  //removeThis has two children - find next smallest number (left once, then right until end)
  else if(removeThis->left != NULL && removeThis->right != NULL)
  {
    Node* nextSmallest = removeThis->left;
    while(nextSmallest->right != NULL) 
    {
      nextSmallest = nextSmallest->right;
    }
    removeThis->number = nextSmallest->number; //replacing the number we're removing with the next smallest number in tree
    //now the one we actually want to delete is nextSmallest
    remove(nextSmallest);
  }
  else remove(removeThis);
} 

//removes node, fixes simple case
void Tree::remove(Node* removeThis)
{
  Node* child;
  Node* parent = removeThis->parent;
  if(removeThis->right == NULL) child = removeThis->left;
  else child = removeThis->right;
  //both black
  if(strcmp(removeThis->color, "black") == 0 && (child == NULL || strcmp(child->color, "black") == 0))
  {
    replace(removeThis, child);
    //now get sibling so we can call both black
    Node* sibling;  
    if(child == NULL) //if node is null the sibling is the non null chld of parent
    {
      if(parent->right != NULL) sibling = parent->right;
      else sibling = parent->left;
    }
    else
    {
      if(child == parent->left) sibling = parent->right;
      else sibling = child->parent->left;
    }
    bothBlack(parent, child, sibling); //passing in the parent bc the child could be null
    //cout << "After removal:" << endl;
    //display();
  }
  //one black the other red
  else if(strcmp(removeThis->color, "red") == 0 || (strcmp(removeThis->color, "black") == 0 && strcmp(child->color, "red") == 0)) 
  {
    replace(removeThis, child);
    if(child != NULL && strcmp(child->color, "red") == 0) child->color = "black"; //if removeThis was black and child was red
    //cout << "After removal:" << endl;
    //display();
  }
}

//child goes into parent's spot, parent is deleted
void Tree::replace(Node* removeThis, Node* child)
{
  if(removeThis != root)
  {
    if(removeThis == removeThis->parent->right) removeThis->parent->right = child;
    else removeThis->parent->left = child;
    if(child != NULL) child->parent = removeThis->parent;
  }
  else
  {
    child->parent = NULL;
    root = child;
  }
  delete removeThis;
}

//fixes red-black property when node removed and its child are black (double black node)
void Tree::bothBlack(Node* parent, Node* node, Node* sibling) 
{
  //'node' here is the node that replaced removeThis  
  //node is a double black node meaning the path from the root to node contains 1 less black node than it should
  //this is because node's parent was black and he was deleted, decreasing black count by 1
  //node can be NULL 
  
  //1. node is new root
  if(node == root) return;
  
  //2. sibling is red - rotate sibling thru parent
  if(sibling != NULL && strcmp(sibling->color, "red") == 0)
  {
    if(parent->parent != NULL)
    {
      if(parent == parent->parent->left) parent->parent->left = sibling;
      else parent->parent->right = sibling;
      sibling->parent = parent->parent;
    }
    else 
    {
      root = sibling;
      sibling->parent = NULL;
    }
    if(sibling == parent->right)
    { 
      parent->right = sibling->left;
      if(parent->right != NULL) parent->right->parent = parent;
      parent->parent = sibling;
      sibling->left = parent;
      sibling = parent->right;
    }
    else if(sibling == parent->left)
    {
      parent->left = sibling->right;
      if(parent->left != NULL) parent->left->parent = parent;
      sibling->right = parent;
      parent->parent = sibling;
      sibling = parent->left;
    }
    parent->parent->color = "black";
    parent->color = "red";
  }

  //3. parent is black, sibling is black and so are its kids
  if(sibling != NULL && strcmp(parent->color, "black") == 0 && strcmp(sibling->color, "black") == 0 && 
    (sibling->right == NULL || strcmp(sibling->right->color, "black") == 0) && (sibling->left == NULL || strcmp(sibling->left->color, "black") == 0)) 
  {
    sibling->color = "red";
    //recursive call on parent
    if(parent->parent != NULL)
    {
      Node* newSibling;
      if(parent == parent->parent->right) newSibling = parent->parent->left;
      else newSibling = parent->parent->right;
      bothBlack(parent->parent, parent, newSibling);
    }
  }
  
  //4. parent is red sibling and sibling's children are black
  if(sibling != NULL && strcmp(sibling->color, "black") == 0 && (sibling->left == NULL || strcmp(sibling->left->color, "black") == 0) 
    && (sibling->right == NULL || strcmp(sibling->right->color, "black") == 0) && strcmp(parent->color, "red") == 0)
  {
    parent->color = "black";
    sibling->color = "red";
    return;
  }
  
  //5 - sibling and siblingL are black, siblingR is red, and sibling is right of parent (or ise versa) -  rotate thru sibling
  if(sibling != NULL && strcmp(sibling->color, "black") == 0 && sibling == parent->left && sibling->right != NULL && strcmp(sibling->right->color, "red") == 0) //sibling left of parent, red child right of sibling
  {
    Node* siblingR = sibling->right;
    sibling->color = "red";
    siblingR->color = "black";
    parent->left = siblingR;
    siblingR->parent = parent;
    sibling->right = siblingR->left;
    if(sibling->left != NULL) sibling->left->parent = sibling;
    siblingR->left = sibling;
    sibling->parent = siblingR;
    sibling = siblingR;
  }
  else if(sibling != NULL && strcmp(sibling->color, "black") == 0 && sibling == parent->right && sibling->left != NULL && strcmp(sibling->left->color, "red") == 0)
  {  
    Node* siblingL = sibling->left;
    sibling->color = "red";
    siblingL->color = "black";
    parent->right = siblingL;
    siblingL->parent = parent;
    sibling->left = siblingL->right;
    if(sibling->right != NULL) sibling->right->parent = sibling;
    siblingL->right = sibling;
    sibling->parent = siblingL;
    sibling = siblingL;
  }
  
  //6 - sibling is black, sibling's left is red, sibling is left of parent (or vise versa) - rotate thru the parent
  if(sibling != NULL && (node == NULL || strcmp(node->color, "black") == 0) && strcmp(sibling->color, "black") == 0 
    && sibling->left != NULL && strcmp(sibling->left->color, "red") == 0 && node == parent->right)
  {
    //parent and sibling colors switch, sibling child becomes black
    if(strcmp(parent->color, "red") == 0) sibling->color = "red";
    parent->color = "black";
    sibling->left->color = "black";
    if(parent->parent != NULL)
    {
      if(parent == parent->parent->right) parent->parent->right = sibling;
      else parent->parent->left = sibling;
      sibling->parent = parent->parent;
    }
    else 
    {
      root = sibling;
      sibling->parent = NULL;
    }
    Node* siblingL = sibling->left;
    parent->left = sibling->right;
    if(sibling->right != NULL) sibling->right->parent = parent;
    sibling->right = parent;
    parent->parent = sibling;
    if(parent == root) root = sibling;
    return;
  }
  else if(sibling != NULL && (node == NULL || strcmp(node->color, "black") == 0) && strcmp(sibling->color, "black") == 0 
    && sibling->right != NULL && strcmp(sibling->right->color, "red") == 0 && node == parent->left)
  {
    //parent and sibling colors switch, sibling child becomes black
    if(strcmp(parent->color, "red") == 0) sibling->color = "red";
    parent->color = "black";
    sibling->right->color = "black";
    if(parent->parent != NULL)
    {
      if(parent == parent->parent->right) parent->parent->right = sibling;
      else parent->parent->left = sibling;
      sibling->parent = parent->parent;
    }
    else 
    {
      root = sibling;
      sibling->parent = NULL;
    }
    Node* siblingR = sibling->right;
    parent->right = sibling->left;
    if(sibling->left != NULL) sibling->left->parent = parent;
    sibling->left = parent;
    parent->parent = sibling;
    if(parent == root) root = sibling;
    return;
  }
}

void Tree::display()
{
  display_impl(root, 0);
}

//displays tree visually using tabs
void Tree::display_impl(Node* current, int level)
{ 
  if(root == NULL) cout << "There's no tree to display D:" << endl;
  else if(current == NULL && level == 0)
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