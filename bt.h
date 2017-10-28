#include <iostream>
#include <math.h>
#include <string>
using namespace std;

// ** you will need to add the state description to state
struct state
{
  int value;
  int level;
  string op;
};

class bt
{
  private:
  state Q[100];   // array holding states
  int count;   // how many states are in Q
  int lastinternal;   // the slot of the last internal node

  // utility functions (** to be changed)
  bool even(int);    // is the number even?  
  bool evenLevel(int);  // is the node's level even? 
  void blanks(int);  // display blanks

public:
    bt();
    ~bt();
    void build(int); // build the tree 

  int  minMaxDFS(int);  // minMAX values for internal nodes computed
  void displayAll(); // display the tree in a nice way


  void minMax();  // forget this BFS version
};
