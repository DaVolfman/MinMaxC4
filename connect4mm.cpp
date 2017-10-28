/*
 * connect4mm.cpp
 *
 *  Created on: Oct 26, 2017
 *      Author: stevenc
 */

#include "connect4mm.h"
#include <vector>
#include <string>

using std::vector;
using std::string;

string connect4state::toString() const {
	string retval = "_______\n";
	for (int j = 5; j >= 0; j--) {
		retval += "|";
		for (int i = 6; i >= 0; i--) {
			int k = 1 << (j * 7 + i);
			if (player1 & k) {
				retval += "X";
			} else if (player2 & k) {
				retval += "O";
			} else {
				retval += " ";
			}
		}
		retval += "|\n";
	}
	retval += "-------\n";
	return retval;
}

string connect4state::toLabel() const {
	string retval;
	char temp;
	int i;
	//retval +='(';
	for(i = 40; i >=0; i -= 4){
		temp = (player1 >> i) & 0xf;
		if(temp > 9){
			temp += 'a';
		}else{
			temp += '0';
		}
		retval+=temp;
	}
	//retval+=',';
	for(i = 40; i >=0; i -= 4){
		temp = (player2 >> i) & 0xf;
		if(temp > 9){
			temp += 'a';
		}else{
			temp += '0';
		}
		retval+=temp;
	}
	//retval+=')';

	return retval;
}


void connect4state::init_stripes() {
	int64_t startpoint;
	int i, j;
	if (stripes.size() == 0) {
		stripes = vector<int64_t>();
		//add horizontal stripes
		for (int j = 0; j <= 5; ++j) {
			for (int i = 0; i <= 3; ++i) {
				startpoint = 1 << (i + j * 7);
				stripes.push_back(
						startpoint | (startpoint << 1) | (startpoint << 2)
								| (startpoint << 3));
			}
		}
		//add vertical stripes
		for (int j = 0; j <= 2; ++j) {
			for (int i = 0; i <= 6; ++i) {
				startpoint = 1 << (i + j * 7);
				stripes.push_back(
						startpoint | (startpoint << 7) | (startpoint << 14)
								| (startpoint << 21));
			}
		}
		//add right ascending stripes
		for (int j = 0; j <= 2; ++j) {
			for (int i = 0; i <= 3; ++i) {
				startpoint = 1 << (i + j * 7);
				stripes.push_back(
						startpoint | (startpoint << 8) | (startpoint << 16)
								| (startpoint << 24));
			}
		}
		//add left ascending stripes
		for (int j = 0; j <= 2; ++j) {
			for (int i = 3; i <= 6; ++i) {
				startpoint = 1 << (i + j * 7);
				stripes.push_back(
						startpoint | (startpoint << 6) | (startpoint << 12)
								| (startpoint << 18));
			}
		}
	}
}

inline bool connect4state::isvalid() const {
	return player1 >= 0 && player2 >= 0 && player1 < 0x40000000000
			&& player2 < 0x40000000000 && !(player1 & player2);
}

float connect4state::heuristic() const {
	float rval = 0.0;
	int64_t stripeuse;
	for (int i = 0; i < stripes.size(); ++i) {
		stripeuse = stripes[i] & player1;
		rval += 6 / (4 - __builtin_popcount(stripeuse));
		stripeuse = stripes[i] & player2;
		rval -= 6 / (4 - __builtin_popcount(stripeuse));
	}
	return rval;
}

vector<connect4stateMoveTuple> connect4state::maxmoves() const {
	vector<connect4stateMoveTuple> retval;
	connect4state tempstate;
	for (int i = 0; i < 7; ++i) {
		tempstate = player1move(i);
		if (tempstate.isvalid())
			retval.push_back( { tempstate, i });
	}
	return retval;
}

vector<connect4stateMoveTuple> connect4state::minmoves() const {
	vector<connect4stateMoveTuple> retval;
	connect4state tempstate;
	for (int i = 0; i < 7; ++i) {
		tempstate = player2move(i);
		if (tempstate.isvalid())
			retval.push_back( { tempstate, i });
	}
	return retval;
}

inline connect4state connect4state::player1move(int column) const {
	if (column >= 7 || column < 0)
		return connect4state(-1, -1);

	int64_t move = 0x1 << column;
	while (move & occupied) {
		move << 7;
	}
	return connect4state(player1 | move, player2);
}

inline connect4state connect4state::player2move(int column) const {
	if (column >= 7 || column < 0)
		return connect4state(-1, -1);

	int64_t move = 0x1 << column;
	while (move & occupied) {
		move << 7;
	}
	return connect4state(player1, player2 | move);
}


moveHeuristicTuple MinmaxNode::maxChoiceAB(float lowerbound, float upperbound, unsigned int layers) const{
	moveHeuristicTuple retval;
	vector<connect4stateMoveTuple> movelist = state.maxmoves();

	if(layers == 0)
		return {state.heuristic(),0};

	for(int i = 0; i < movelist.size() and upperbound > lowerbound; ++i){
		generatedMoves[i]= movelist[i].move;
		if(generatedChildren.size() < i){
			generatedChildren.push_back(new MinmaxNode(movelist[i].state));
			generatedReturns.push_back(generatedChildren[i]->minChoiceAB(lowerbound,upperbound,layers-1).heuristic);
		}
		if(generatedReturns[i] > lowerbound){
			lowerbound = generatedReturns[i];
			retval.heuristic = generatedReturns[i];
			retval.move=generatedMoves[i];
		}
	}

	return retval;
}


moveHeuristicTuple MinmaxNode::minChoiceAB(float lowerbound, float upperbound, unsigned int layers) const{
	moveHeuristicTuple retval;
	vector<connect4stateMoveTuple> movelist = state.minmoves();

	if(layers == 0)
		return {state.heuristic(),0};

	for(int i = 0; i < movelist.size() and upperbound > lowerbound; ++i){
		generatedMoves[i]= movelist[i].move;
		if(generatedChildren.size() < i){
			generatedChildren.push_back(new MinmaxNode(movelist[i].state));
			generatedReturns.push_back(generatedChildren[i]->maxChoiceAB(lowerbound,upperbound,layers-1).heuristic);
		}
		if(generatedReturns[i] < upperbound){
			upperbound = generatedReturns[i];
			retval.heuristic = generatedReturns[i];
			retval.move=generatedMoves[i];
		}
	}

	return retval;
}

inline void MinmaxNode::trimExcept(MinmaxNode* subtree) {
	if (subtree == this)
		return;

	for (int i = 0; i < generatedChildren.size(); ++i) {
		trimExcept (subTree);
	}
	if (subtree != this)
		delete this;
}

inline MinmaxNode* MinmaxNode::findChild(int move) const {
	for (int i = 0; i < generatedChildren.size(); ++i) {
		if (generatedMoves[i] == move)
			return generatedChildren[i];
	}
	return NULL;
}

inline MinmaxNode* MinmaxNode::findChild(connect4state searchstate) {
	for (int i = 0; i < generatedChildren.size(); ++i) {
		if (generatedChildren[i]->state == searchstate)
			return generatedChildren[i];
	}
	return NULL;
}

MinmaxNode* MinmaxNode::findGrandchild(connect4state searchstate) const{
	MinmaxNode* potential = NULL;

	for(int i=0; i<generatedChildren.size(); ++i){
		potential = generatedChildren[i]->findChild(searchstate);
		if(potential != NULL)
			return potential;
	}

	return NULL;
}
