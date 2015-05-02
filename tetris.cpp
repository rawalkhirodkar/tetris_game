#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <math.h>
#include<sstream>
using namespace std;

static const int tilesize = 30; //constants required
static const int xmaxtiles= 14;
static const int ymaxtiles= 20;
static const int bgcolor= 0;
static const double timeout = 0.3;

struct Tile {
// x and y position of a given tile on board matrix
int x; int y;
};

void ts_down (Tile ts[4]){						//functons to shift co-ordinates
	for (int i=0; i<4; i++) ts[i].y++; 
}
void ts_right (Tile ts[4]) {
	for(int i=0;i<4;i++) ts[i].x++;
}
void ts_left (Tile ts[4]) {
	for(int i=0;i<4;i++) ts[i].x--;
}
void ts_copy (Tile in[4], Tile out[4]) {
	for(int i=0;i<4;i++)
	 {out[i].x = in[i].x; out[i].y = in[i].y;}
}
void ts_rotate(Tile ts[4], int centerTile, int direction) {       // direction: counter(-1), clock(1)
	if(centerTile==2){
	
	for(int i=0;i<4;i++){
		int tmpx = ts[i].x,tmpy = ts[i].y;
		
		if(direction == -1){
			ts[i].x = ts[centerTile].x - (tmpy - ts[centerTile].y);
			ts[i].y = ts[centerTile].y + (tmpx - ts[centerTile].x);
		}
		
		if(direction == 1){
			ts[i].x = ts[centerTile].x + (tmpy - ts[centerTile].y);
			ts[i].y = ts[centerTile].y - (tmpx - ts[centerTile].x);
		}
	}//for loop ends
	}// if loop ends
}//function ends

class Board;

class ActivePiece {//active piece class
	
friend class Board;
Tile tileSet[4];
int color;
int midpointTile;
Board *b; // container board

public:

ActivePiece(Board *container);//constructor of Active piece
bool moveDown ();
bool moveLeft ();
bool moveRight ();
bool rotate(int direction);

};

class Board : public Fl_Widget {
	
int occup[xmaxtiles][ymaxtiles];// shows color different than bgcolor if occupied

ActivePiece *p,*q;

int score; // score variable,incremented by 10 for a complete row
char* scoreLabel; // label used to display the score
Fl_Box *scoreBox;// private member of class for scorebox

public:

	Board();
	
	bool active; //status of active piece
	bool down;  //if moving down is permissible or not
	int periodic();
	void setScoreBox(Fl_Box** sb); //to set scorebox pointer to class member
	bool isFree(Tile ts[4]); // before moving down
	bool touchdown(Tile ts[4]) ; // check for touchdown
	void transferTiles();// updation of occup array
	void deleteRow(int row);
	bool rowDone(int row);
	void draw();
	int handle(int e);// keypress events
};	
	
	
ActivePiece::ActivePiece(Board *container) {
int r = rand()%7;
int c = (int) (xmaxtiles/2); //xcenter
b = container;

switch(r) {
		case 0:
		
			tileSet[0].x=c-2; tileSet[0].y=0; // horizontal 
			tileSet[1].x=c-1; tileSet[1].y=0;
			tileSet[2].x=c; tileSet[2].y=0;
			tileSet[3].x=c+1; tileSet[3].y=0;
			midpointTile = 2;
			break;
			
		case 1:
		
			tileSet[0].x=c; tileSet[0].y=0; // Inverted T
			tileSet[1].x=c-1; tileSet[1].y=1;
			tileSet[2].x=c; tileSet[2].y=1;
			tileSet[3].x=c+1; tileSet[3].y=1;
			midpointTile = 2;
			break;	

		case 2:
		
			tileSet[0].x=c-1; tileSet[0].y=0; // a "four" shaped fig
			tileSet[1].x=c-1; tileSet[1].y=1;
			tileSet[2].x=c; tileSet[2].y=1;
			tileSet[3].x=c; tileSet[3].y=2;
			midpointTile = 2;
			break;
			
		case 3:
		
			tileSet[0].x=c; tileSet[0].y=0; // L shaped 
			tileSet[1].x=c; tileSet[1].y=1;
			tileSet[2].x=c; tileSet[2].y=2;
			tileSet[3].x=c+1; tileSet[3].y=2;
			midpointTile = 2;
			break;	

		case 4:
		
			tileSet[0].x=c+1; tileSet[0].y=0; // mirror image of seven
			tileSet[1].x=c; tileSet[1].y=0;
			tileSet[2].x=c; tileSet[2].y=1;
			tileSet[3].x=c; tileSet[3].y=2;
			midpointTile = 2;
			break;
			
		case 5:
		
			tileSet[0].x=c+1; tileSet[0].y=0; // mirror image of four
			tileSet[1].x=c+1; tileSet[1].y=1;
			tileSet[2].x=c; tileSet[2].y=1;
			tileSet[3].x=c; tileSet[3].y=2;
			midpointTile = 2;
			break;	

		case 6:
		
			tileSet[0].x=c-1; tileSet[0].y=0; // square
			tileSet[1].x=c; tileSet[1].y=0;
			tileSet[2].x=c; tileSet[2].y=1;
			tileSet[3].x=c-1; tileSet[3].y=1;
			midpointTile = -1;
			break;
		}		//switch ends
		
		color = 100 + rand()%50;
		if(color == bgcolor){color = color + 170;}

}//function ends

bool ActivePiece::moveRight () {			//functions to update the co-ordinate array according to keypress
			Tile proposedSet[4];
			ts_copy(tileSet, proposedSet);
			ts_right(proposedSet);
			
			if (b->isFree(proposedSet)) {
				
				ts_copy(proposedSet, tileSet);
				return true;
			}
			
			else return false;
}

bool ActivePiece::moveLeft () {
			Tile proposedSet[4];
			ts_copy(tileSet, proposedSet);
			ts_left(proposedSet);
			
			if (b->isFree(proposedSet)) {
				
				ts_copy(proposedSet, tileSet);
				return true;
			}
			
			else return false;
}

bool ActivePiece::moveDown () {
			Tile proposedSet[4];
			ts_copy(tileSet, proposedSet);
			ts_down(proposedSet);
			
			if (b->isFree(proposedSet)) {
				
				ts_copy(proposedSet, tileSet);
				return true;
			}
			
			else return false;
}

bool ActivePiece::rotate (int direction) {
			Tile proposedSet[4];
			ts_copy(tileSet, proposedSet);
			ts_rotate(proposedSet,midpointTile,direction);
			
			if (b->isFree(proposedSet)) {
				
				ts_copy(proposedSet, tileSet);
				return true;
			}
			
			else return false;
}

	
Board::Board(): Fl_Widget (0,0,800,700,"Tetris"){       //constructor of board
			 
			 p=new ActivePiece(this);
			 q=new ActivePiece(this);
			 			 
			 for(int i=0;i<xmaxtiles;i++){						//default grid created
					for(int j=0;j<ymaxtiles;j++) {occup[i][j] = bgcolor;}
			}
}


void Board::transferTiles(){               //updates the occup array i.e changes the color of grid
		
		for(int i=0;i<4;i++){
			if(occup[p->tileSet[i].x][p->tileSet[i].y]!=bgcolor){cout<<"SORRY :( ...GAME OVER!!!"<<endl; exit(0);}
			occup[p->tileSet[i].x][p->tileSet[i].y]=p->color;
		}
}		
	
	
bool Board::rowDone(int r){ 			//checks whether the row is completely filled or not
	
		 for (int i=0;i<xmaxtiles;i++){
		 if(occup[i][r]== bgcolor){return false;}
		}
		
	return true;
}

void Board::deleteRow(int r){			//deletes the row and refreshes color array
	int row = r;
	for(int j=r;j>0;j--){
		 for (int i=0;i<xmaxtiles;i++){
		  occup[i][j]=occup[i][j-1];
	 }
		 
	 }
	 for (int i=0;i<xmaxtiles;i++){
		  occup[i][0]=bgcolor;}
 
}		
		
bool Board::isFree(Tile ts[4]){			//checks for availability and restrictions on the piece
			
	for(int i=0;i<4;i++){if(ts[i].x>=xmaxtiles||ts[i].x<0){return false;}
		if(ts[i].y>=ymaxtiles||ts[i].y<0){return false;}
		if(occup[ts[i].x][ts[i].y]!=bgcolor){return false;}
	}
	return true;
}

void Board::setScoreBox(Fl_Box** sb)		//for the scorebox
{
	scoreBox = *sb ;
	score=0;
	scoreLabel = (char*) malloc(sizeof(char)*10);
}

void Board::draw () {					// drawing function
	fl_color(80);
	for(int i=0;i<4;i++){
		fl_line(0,ymaxtiles*tilesize+i*0.25,xmaxtiles*tilesize,ymaxtiles*tilesize+i*0.25);
	}
	
	for(int i=0;i<4;i++){
		fl_line(xmaxtiles*tilesize+i*0.25,0,xmaxtiles*tilesize+i*0.25,ymaxtiles*tilesize);
	}
	
	for(int j=0;j<ymaxtiles;j++){
		if(rowDone(j)){score=score+10;deleteRow(j);} //checks for any completed row 
	}
	
		for (int i=0; i<xmaxtiles;i++){
		for (int j=0; j<ymaxtiles;j++)
			{fl_draw_box(FL_BORDER_BOX,i*tilesize,j*tilesize,// draws the occupied color array
				tilesize,tilesize,occup[i][j]);
			}
		}
		for(int i=0;i<4;i++){
			if(down){fl_draw_box(FL_BORDER_BOX,p->tileSet[i].x*tilesize,p->tileSet[i].y*tilesize,tilesize,tilesize,p->color);}
			

			}	
	}

void timeractions(void *b) {			//timeout function
	((Board *)b)->periodic ();
}


int Board::periodic() {
	
		down = p->moveDown();
	
	if(!down){transferTiles();
		p=q;
		for(int i=0;i<4;i++){
		fl_draw_box(FL_FLAT_BOX,(q->tileSet[i].x+xmaxtiles/2+3)*tilesize,(q->tileSet[i].y+ymaxtiles/2+3)*tilesize,tilesize,tilesize,bgcolor);
		}
		q=new ActivePiece(this);
		}
	
	redraw();
	Fl::repeat_timeout (0.3,timeractions,this);
	
}


int Board::handle(int e) {//	cout << e << "  " << Fl::event_key() << endl;  // try uncommenting it..
	
	if (e==12)  // means it's a keyboard event
	switch (Fl::event_key()) {  // different keys are sensed here
		case FL_Left : p->moveLeft();redraw(); break;
		case FL_Up : p->rotate(-1);redraw(); break;
		case FL_Right : p->moveRight();redraw(); break;
		case FL_Down : p->rotate(1);redraw(); break;
		case 32 : while(p->moveDown()) {p->moveDown();redraw();}break;
	}
		stringstream strs;
		strs << score;
		string temp_str = strs.str();
		strcpy(scoreLabel,"Score: ");
		strcat(scoreLabel,(char*) temp_str.c_str() );

		scoreBox->label(scoreLabel);
		
		
}


int main(int argc, char *argv[]) {
    	Fl_Window *window = new Fl_Window (800,700,"Tetris"); // outer window
	window->color(bgcolor);  
	 
	Board *b = new Board();
	Fl_Box *scorebox = new Fl_Box(tilesize*xmaxtiles+20,50,250,200,"Score: 0\0");
		scorebox->box(FL_UP_BOX);
        scorebox->labelfont(FL_BOLD+FL_ITALIC);
        scorebox->labelsize(36);
        scorebox->labeltype(FL_ENGRAVED_LABEL);
		b->setScoreBox(&scorebox); 
       
	window->end();  
   	window->show();
   	Fl::add_timeout(0.1, timeractions,b);
    	return(Fl::run());  // the process waits from here on for events
}
		
		
		
			
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
			
			









