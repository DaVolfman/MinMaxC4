#include <iostream>
#include "connect4mm.h"
using std::cout;
using std::cerr;
using std::cin;
using std::endl;


void testWins(){
	connect4state htester;

	cerr << "Checking heuristic for player 1 wins:\n";
	for(unsigned int i = 0; i < connect4state::stripes.size(); ++i){
		htester = connect4state(connect4state::stripes[i],0ll);
		cerr << htester.heuristic() << endl;
	}

	cerr << "Checking heuristic for player 1 wins:\n";
	for(unsigned int i = 0; i < connect4state::stripes.size(); ++i){
		htester = connect4state(0ll, connect4state::stripes[i]);
		cerr << htester.heuristic() << endl;
	}
}


int main(){
	int playerchoice=-1;
	MinmaxNode* gametree = new MinmaxNode;
	MinmaxNode* nextstate = NULL;
	moveHeuristicTuple choice = gametree->maxChoice(6);


	cerr << "I'm going first and choosing " << choice.move
		  << " because it looks like I can get " << choice.heuristic << endl;
	nextstate = gametree->findChild(choice.move);
	if(nextstate == NULL){
		cerr << "ERROR: AI took unexpected path.\n";
		return 1;
	}
	gametree->trimExcept(nextstate);
	gametree=nextstate;
	while(not gametree->state.player1wins() and not gametree->state.player2wins()){
		cerr << gametree->state.toString();
		nextstate = NULL;

		playerchoice = -1;
		//choice = gametree->minChoice(5);
		while(playerchoice < 0 or playerchoice >= 7){
			cerr << "Choose a move(0-6):";
			cin >> playerchoice;
		}


		nextstate = gametree->findChild(playerchoice);
		if(nextstate == NULL){
			cerr << "I planned, but not for " << playerchoice << endl;
			nextstate = new MinmaxNode(gametree->state.player2move(playerchoice));
			gametree->trimExcept(NULL);
		}else{
			gametree->trimExcept(nextstate);
		}
		gametree = nextstate;
		cerr << gametree->state.toString();
		choice = gametree->maxChoice(6);
		if(gametree->state.player2wins())
			break;
		cerr << "I'm choosing " << choice.move
			  << " because it looks like I can get " << choice.heuristic << endl;
		nextstate = gametree->findChild(choice.move);
		if(nextstate == NULL){
			cerr << "ERROR: AI took unexpected path.\n";
			return 1;
		}
		gametree->trimExcept(nextstate);
		gametree=nextstate;
		if(gametree->state.player1wins())
			break;
	}
	if(gametree->state.player2wins()){
		cerr << "You win!  How did you do that?" << endl;
	}else if(gametree->state.player1wins()){
		cerr << gametree->state.toString();
		cerr << "I win!  As expected." << endl;
	}else{
		cerr << "Unexpected state reached.\n";
	}


	gametree->trimExcept((MinmaxNode*)NULL);



	return 0;
}
