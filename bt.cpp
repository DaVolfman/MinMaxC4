#include "bt.h"

bt::bt()
{  count = 0; lastinternal = 0;
}

bt::~bt() {}


// this function builds a game tree to the maxlevel (0...maxlevel)
void bt::build(int maxlevel)
{
  int i = 0;  // current index into Q
  int l = 0;  // current tree level

  while (l < maxlevel)  // one level at a time
    {
      for (int k = 1; k <= pow(2,l); k++) // for the current level nodes
	{ 
	  // ** operator description needs to be changed in the following
	  if (!even(i)) Q[i].op = "go L"; else Q[i].op = "go R";
	  // ** store the state features too here
	  Q[i].value = 0;  // no value yet
	  Q[i].level = l;
          i++;
	}
      l++;
    }

  lastinternal  = i-1;  // the last internal node is here
  int v;
  
  cout << "There are " << pow(2,l) << " leaves.. " << endl;

  // ** you will take this out because you have to compute
  // the values based on state features
  cout << "Please enter the value of each leaf (left to right)." << endl;
  
  for (int k = 1; k <= pow(2,l); k++) // for each leaf
    { // ** operator description needs to be changed in the following
      if (!even(i)) Q[i].op = "go L"; else Q[i].op = "go R";
      // ** store the state features here
      // ** then compute the value based on the features instead of next
      cout << "value: "; cin >> v; 
      Q[i].value = v;      

      // level number stored
      Q[i].level = l;
      i++;
    }
  count = i--;   // count slots have been used
}

// to display blanks m times
void bt::blanks(int m)
{ for (int i = 1; i <= m; i++) cout << " "; }


// to display the game tree
void bt::displayAll()
{ 
  cout << 0 << ": " << "can get " << Q[0].value << endl;
  for (int i = 1; i < count; i++)
    {   
        blanks(Q[i].level);
        cout << i << ": " << Q[i].op << " and get " << Q[i].value << endl;}
} 

// ** this is needed because of binary decision but you may need
// some other function to determine which branch
bool bt::even(int x)
{
  if (x%2 == 0) return true; else return false;
}

// to determine MIN or MAX level (MIN is odd)
bool bt::evenLevel(int x)
{
  if ((Q[x].level % 2) == 0) return true; else return false;
}

// ** Change for more branches to find Max
int maxof(int a, int b)
{  if (a > b) return a; else return b; }


//** Change for more branches to find Min
int minof(int a, int b)
{  if (a > b) return b; else return a; }


// this function does the minMAX value computation
// ** locations of children determined based on binary decision
// ** you must change it based on the number of branches
int bt::minMaxDFS(int x)
{ int v1, v2;

  if (x > lastinternal) return (Q[x].value);  // reached leaf
  // otherwise
  v1 = minMaxDFS(2*x+1);  // left child
  v2 = minMaxDFS(2*x+2);  // right child
  if (evenLevel(x))   Q[x].value = maxof(v1,v2); else 
		      Q[x].value = minof(v1,v2);
  return Q[x].value;
}  


