#include <iostream>
#include <cstdlib>
//#include <map>
using namespace std;

const int X=1, O=-1, EMPTY=0;
int board[3][3];
int currentPlayer;

void clearBoard(){
	for (int i=0;i<3;i++){
		for (int j=0;j<3;j++){
			board[i][j] = EMPTY;
		}
	}
	currentPlayer = X;
}

void putMark(int i, int j){
	board[i][j] = currentPlayer;
	currentPlayer = -currentPlayer;
}

bool isWin(int mark){
	int win = 3*mark;
	return (
		(board[0][0]+board[0][1]+board[0][2] == win)||
		(board[1][0]+board[1][1]+board[1][2] == win)||
		(board[2][0]+board[2][1]+board[2][2] == win)||
		(board[0][0]+board[1][0]+board[2][0] == win)||
		(board[0][1]+board[1][1]+board[2][1] == win)||
		(board[0][2]+board[1][2]+board[2][2] == win)||
		(board[0][0]+board[1][1]+board[2][2] == win)||
		(board[0][2]+board[1][1]+board[2][0] == win)
	);
}

int getWinner(){
	if (isWin(X)) {return X;}
	else if (isWin(O)) {return O;}
	else {return EMPTY;}
}

bool gameOver(){
	for (int i=0;i<3;i++){
		for (int j=0;j<3;j++){
			if (board[i][j] == EMPTY) return false;
		}
	}
	return true;
}

void printBoard(){
	for (int i=0;i<3;i++){
		for (int j=0;j<3;j++){
			switch(board[i][j]){
				case X: cout<<'X'; break;
				case O: cout<<'O'; break;
				case EMPTY: cout<<' '; break;
			}
			if (j < 2) cout<<'|';
		}
		if (i<2) cout<<"\n-+-+-\n";
	}
	cout<<'\n';
}

int main(){
	int x, y;
	//map<int, char> players{{1, 'X'}, {-1, 'O'}};
	clearBoard();
	printBoard();
	while (!(gameOver() || (isWin(X)||isWin(O)))){
		cout<<"\nIt's "<<(currentPlayer==1 ? 'X' : 'O')<<"'s turn"<<'\n'
			<<"which coordinate do you want to play? ";
		cin>>x>>y;
		putMark(x, y);
		printBoard();
	}
	
	int winner = getWinner();
	if (winner != EMPTY){
		cout<<"\n\n "<<(winner == X ? 'X' : 'O')<< " wins"<<'\n';
	}
	else{
		cout<<"\n\n Tie"<<'\n';
	}
	return 0;
}