#include <iostream>
#include "connect4mm.h"
using std::cout;
using std::endl;

int main()
{
  MinmaxNode* gametree = new MinmaxNode;
  moveHeuristicTuple choice = gametree->minChoice(5);
  cout << "I'm going first and chosing " << choice.move
		  << " because it looks like I can get " << choice.heuristic << endl;

  cout << "Here's the work:\n" << gametree->outputTree();

  gametree->trimExcept((MinmaxNode*)NULL);

  return 0;
}  
