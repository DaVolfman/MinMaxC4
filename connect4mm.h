/*
 * connect4mm.h
 *
 *  Created on: Oct 19, 2017
 *      Author: stevenc
 */

#ifndef CONNECT4MM_H_
#define CONNECT4MM_H_

#include <stdint.h>
#include <iostream>
#include <vector>
#include <string>
using std::vector;
using std::string;

class connect4state;
struct connect4stateMoveTuple{
	connect4state state;
	int move;
};

class connect4state{
protected:
	int64_t player1;
	int64_t player2;
	mutable int64_t occupied;

	//stripes is the set of all connect 4s
	static vector<int64_t> stripes;
	static void init_stripes();
public:
	connect4state(){
		player1 = 0; player2 = 0; occupied = 0;
		if(stripes.size() == 0)
			init_stripes();
	}

	connect4state(int64_t play1, int64_t play2){
		if(stripes.size() == 0)
			init_stripes();
		player1=play1;
		player2=play2;
		occupied = play1 | play2;
	}

	string toString() const;
	string toLabel() const;


	bool isvalid() const;

	//return the gamestate after dropping a piece for player 1 in the given column
	//returns an intentionally invalid state if parameters out of range
	//an illegal move will result in a different kind of invalid state
	connect4state player1move(int column) const;

	connect4state player2move(int column) const;

	float heuristic() const;

	vector<connect4stateMoveTuple> maxmoves() const;

	vector<connect4stateMoveTuple> minmoves() const;

};

struct moveHeuristicTuple{
	int move;
	float heuristic;
};

class MinmaxNode{
protected:
	connect4state state;
	mutable vector<int> generatedMoves;
	mutable vector<MinmaxNode *> generatedChildren;
	mutable vector<float> generatedReturns;

public:
	MinmaxNode(connect4state gstate){
		state = gstate;
	}




	moveHeuristicTuple maxChoice(unsigned int layers) const{
		return maxChoiceAB(-1/0, 1/0, layers);
	}
	moveHeuristicTuple minChoice(unsigned int layers) const{
		return minChoiceAB(-1/0, 1/0, layers);
	}
	moveHeuristicTuple maxChoiceAB(float lowerbound, float upperbound, unsigned int layers) const;
	moveHeuristicTuple minChoiceAB(float lowerbound, float upperbound, unsigned int layers) const;

	void trimExcept(MinmaxNode* subtree);
	MinmaxNode* findChild(int move) const;
	MinmaxNode* findChild(connect4state searchstate);
	MinmaxNode* findGrandchild(connect4state searchstate) const;

};


#endif /* CONNECT4MM_H_ */
