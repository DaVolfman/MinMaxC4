/*
 * connect4mm.cpp
 *
 *  Created on: Oct 26, 2017
 *      Author: stevenc
 */

#include "connect4mm.h"
#include <vector>
#include <string>
#include <queue>
#include <cstdio>
#include <cmath>

using std::vector;
using std::string;
using std::queue;
using std::isinf;

#include <iostream>
using std::cout;

vector<int64_t> connect4state::stripes;

inline connect4state::connect4state(int64_t play1, int64_t play2) {
	if (stripes.size() == 0)
		init_stripes();

	player1 = play1;
	player2 = play2;
	occupied = play1 | play2;
}

inline connect4state::connect4state() {
	player1 = 0;
	player2 = 0;
	occupied = 0;
	if (connect4state::stripes.size() == 0)
		init_stripes();
}


string connect4state::toString() const {
	string retval = "_6543210_\n";
	for (int j = 5; j >= 0; j--) {
		retval += "|";
		for (int i = 6; i >= 0; i--) {
			int64_t k = 0x1ll << ((j * 7) + i);
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
	retval += "---------\n";
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
	if (stripes.size() == 0) {
		stripes = vector<int64_t>();
		//add horizontal stripes
		for (int j = 0; j <= 5; ++j) {
			for (int i = 0; i <= 3; ++i) {
				startpoint = 1ll << (i + j * 7);
				stripes.push_back(
						startpoint | (startpoint << 1) | (startpoint << 2)
								| (startpoint << 3));
			}
		}
		//add vertical stripes
		for (int j = 0; j <= 2; ++j) {
			for (int i = 0; i <= 6; ++i) {
				startpoint = 1ll << (i + j * 7);
				stripes.push_back(
						startpoint | (startpoint << 7) | (startpoint << 14)
								| (startpoint << 21));
			}
		}
		//add right ascending stripes
		for (int j = 0; j <= 2; ++j) {
			for (int i = 0; i <= 3; ++i) {
				startpoint = 1ll << (i + j * 7);
				stripes.push_back(
						startpoint | (startpoint << 8) | (startpoint << 16)
								| (startpoint << 24));
			}
		}
		//add left ascending stripes
		for (int j = 0; j <= 2; ++j) {
			for (int i = 3; i <= 6; ++i) {
				startpoint = 1ll << (i + j * 7);
				stripes.push_back(
						startpoint | (startpoint << 6) | (startpoint << 12)
								| (startpoint << 18));
			}
		}
	}
}

string connect4state::check_stripes() {
	string retval;
	connect4state check;
	for(unsigned int i = 0; i < stripes.size(); ++i){
		check = connect4state(stripes[i], 0ll);
		retval += check.toString();
	}

	return retval;
}


inline bool connect4state::isvalid() const {
	return player1 >= 0 && player2 >= 0 && player1 < 0x40000000000
			&& player2 < 0x40000000000 && !(player1 & player2);
}

float connect4state::heuristic() const {
	float rval = 0.0;
	int64_t stripe1, stripe2;
	for (unsigned int i = 0; i < stripes.size(); ++i) {
		stripe1 = stripes[i] & player1;
		stripe2 = stripes[i] & player2;
		if(stripe1 and not stripe2){
			rval += 6.0 / (4.0 - (float)__builtin_popcount(stripe1));
		}else if(stripe2 and not stripe1){
			rval -= 6.0 / (4.0 - (float)__builtin_popcount(stripe2));
		}
	}
	return rval;
}

bool connect4state::player1wins() const{
	return heuristic() > 0 and isinf(heuristic());
}
bool connect4state::player2wins() const{
	return heuristic() < 0 and isinf(heuristic());
}


vector<connect4stateMoveTuple> connect4state::maxmoves() const {
	vector<connect4stateMoveTuple> retval;
	connect4state tempstate;
	connect4stateMoveTuple insert;
	for (unsigned int i = 0; i < 7; ++i) {
		tempstate = player1move(i);
		if (tempstate.isvalid()){
			insert.state = tempstate;
			insert.move = i;
			retval.push_back(insert);
		}
	}
	return retval;
}

vector<connect4stateMoveTuple> connect4state::minmoves() const {
	vector<connect4stateMoveTuple> retval;
	connect4state tempstate;
	connect4stateMoveTuple insert;
	for (int i = 0; i < 7; ++i) {
		tempstate = player2move(i);
		if (tempstate.isvalid()){
			insert.state = tempstate;
			insert.move = i;
			retval.push_back(insert);
		}
	}
	return retval;
}

inline connect4state connect4state::player1move(int column) const {
	if (column >= 7 || column < 0)
		return connect4state(-1, -1);

	int64_t move = 0x1 << column;
	while (move & occupied) {
		move = move << 7;
	}
	return connect4state(player1 | move, player2);
}

inline connect4state connect4state::player2move(int column) const {
	if (column >= 7 || column < 0)
		return connect4state(-1, -1);

	int64_t move = 0x1 << column;
	while (move & occupied) {
		move = move << 7;
	}
	return connect4state(player1, player2 | move);
}


moveHeuristicTuple MinmaxNode::maxChoiceAB(float atLeast, float atMost, unsigned int layers) const{
	moveHeuristicTuple retval;
	vector<connect4stateMoveTuple> movelist = state.maxmoves();

	if(layers < 1){
		retval.heuristic = state.heuristic();
		retval.move = 0;
		return retval;
	}

	retval.heuristic = -1.0;
	retval.move = -1;

	for(unsigned int i = 0; i < movelist.size() /*and atMost > atLeast*/; ++i){
		if(generatedChildren.size() <= i){
			generatedMoves.push_back(movelist[i].move);
			generatedChildren.push_back(new MinmaxNode(movelist[i].state));
			//generatedReturns.push_back(generatedChildren[i]->minChoiceAB(atLeast,atMost,layers-1).heuristic);
			generatedReturns.push_back(generatedChildren[i]->minChoiceAB(-1.0/0.0,1.0/0.0,layers-1).heuristic);
		}
		if(generatedReturns[i] > atLeast){
			cout << "Max " << generatedReturns[i] << " > " << atLeast << '\n';
			atLeast = generatedReturns[i];
			retval.heuristic = generatedReturns[i];
			retval.move=generatedMoves[i];
		}
	}

	return retval;
}


moveHeuristicTuple MinmaxNode::minChoiceAB(float atLeast, float atMost, unsigned int decisions) const{
	moveHeuristicTuple retval;
	vector<connect4stateMoveTuple> movelist = state.minmoves();

	if(decisions < 1){
		retval.heuristic = state.heuristic();
		retval.move = 0;
		return retval;
	}

	retval.heuristic = -1.0;
	retval.move = -1;

	for(unsigned int i = 0; i < movelist.size()/* and atMost > atLeast*/; ++i){
		if(generatedChildren.size() <= i){
			generatedMoves.push_back(movelist[i].move);
			generatedChildren.push_back(new MinmaxNode(movelist[i].state));
			//generatedReturns.push_back(generatedChildren[i]->maxChoiceAB(atLeast,atMost,decisions-1).heuristic);
			generatedReturns.push_back(generatedChildren[i]->maxChoiceAB(-1.0/0.0,1.0/0.0,decisions-1).heuristic);
		}
		if(generatedReturns[i] < atMost){
			cout << "Min " << generatedReturns[i] << " < " << atMost << '\n';
			atMost = generatedReturns[i];
			retval.heuristic = generatedReturns[i];
			retval.move=generatedMoves[i];
		}
	}

	return retval;
}

void MinmaxNode::trimExcept(MinmaxNode* subtree) {
	if (subtree == this)
		return;

	for (unsigned int i = 0; i < generatedChildren.size(); ++i) {
		generatedChildren[i]->trimExcept(subtree);
	}
	if (subtree != this)
		delete this;
}

MinmaxNode* MinmaxNode::findChild(int move) const {
	for (unsigned int i = 0; i < generatedChildren.size(); ++i) {
		if (generatedMoves[i] == move)
			return generatedChildren[i];
	}
	return NULL;
}

MinmaxNode* MinmaxNode::findChild(connect4state searchstate) const{
	for (unsigned int i = 0; i < generatedChildren.size(); ++i) {
		if (generatedChildren[i]->state == searchstate)
			return generatedChildren[i];
	}
	return NULL;
}

MinmaxNode* MinmaxNode::findGrandchild(connect4state searchstate) const{
	MinmaxNode* potential = NULL;

	for(unsigned int i=0; i<generatedChildren.size(); ++i){
		potential = generatedChildren[i]->findChild(searchstate);
		if(potential != NULL)
			return potential;
	}

	return NULL;
}

string MinmaxNode::outputTree() const {
	string retval;
	const MinmaxNode* scan = this;
	MinmaxNode* subscan;
	queue<const MinmaxNode*> procqueue, nextqueue;
	int layer = 0;
	char levelLabel[12];
	char movestring[64];
	procqueue.push(scan);

	while(!procqueue.empty()){
		if(nextqueue.empty()){
			sprintf(levelLabel, "Layer %d\n", layer);
			retval += levelLabel;
		}
		scan = procqueue.front();
		procqueue.pop();
		retval += "Node ID " + scan->state.toLabel() + '\n';
		retval += "Game State:\n" + scan->state.toString() + '\n';
		if(not scan->generatedChildren.empty()){
			retval += "Moves:\n";
			for(unsigned int i = 0; i< scan->generatedChildren.size(); ++i){
				subscan = scan->generatedChildren[i];
				nextqueue.push(subscan);
				sprintf(movestring,"Move %d, Value %f, Node ID ", scan->generatedMoves[i], scan->generatedReturns[i]);
				retval += movestring + subscan->state.toLabel() + '\n';
			}
			retval += '\n';
		}else{
			sprintf(movestring,"Leaf state, actual value is %f\n",scan->state.heuristic());
			retval += movestring;
		}

		if(procqueue.empty() and not nextqueue.empty()){
			procqueue = nextqueue;
			while(not nextqueue.empty()) nextqueue.pop();
		}
	}

	return retval;
}


