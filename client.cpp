#include <iostream>
#include "bt.h"
using namespace std;

// ** Your job is to change based on ** comments so that it will do
// MINMAX computation for some small game of your choice.
// EC = 10pts 
// Submit the 3 files plus the output.
// Due: before Oct 30th 11:55pm 

int main()
{
  bt States;
  int m;

  // ** change the instructions
  cout << "In this game, there is a binary decision made at each state.";
  cout << "One can begin to move left or right." << endl; 


  cout << "Maximum level of lookahead is: "; cin >> m;
  cout << "I am going to build the tree in my head..." << endl;
  States.build(m);

  cout << "Tree is:" << endl;
  States.displayAll();

  States.minMaxDFS(0);

  cout << "After MinMax:" << endl;
  States.displayAll();

}  
