#include <cstdlib>
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <algorithm>
#include <string>
#include <ncurses.h>
#include <dirent.h>
#include <cstring>

using namespace std;

int timer = 0;
int display = 0;
int wy = 0, wx = 0;
int cy = 10, cx = 10;
int edit_mode = 0;
int W, H;

void endSim(int exit_status, const char* exit_msg){
	delwin(stdscr);
	endwin();
	printf("%s",exit_msg);
	exit(exit_status);
}

#include "colors.hpp"
#include "class.hpp"
#include "world_fun.hpp"
#include "display.hpp"

int main(){
	setlocale(LC_ALL, "");					// Allow UTF-8 characters
	initscr();								// Curse the Terminal
	noecho();								// Turn off char echoing
	raw();									// Turn off line buffering
	nodelay(stdscr, TRUE);					// Turn off getch() halt
	keypad(stdscr, TRUE);					// Enable reading function & arrow keys
	curs_set(0);
	start_color();
	srand(time(0));

	if((has_colors() || can_change_color())==FALSE){	// Check if colors are supported in the first place
		endSim(1,"Your terminal does not support color/color modifications!\n");
	}
	initPallette();
	
	getmaxyx(stdscr,wy,wx);					// Get current screen size;

	if(wx<120 || wy<30){						// Check if Terminal is big enough to fit the game
		endSim(1,"Make the screen bigger! (At least 120x30)\n");
	}
	
	W = (wx-36)/2-10;
	H = wy-13;
	
	int rand_world = simPrep(simMenu());
	
	//helpBar();
	
	Tile** world = new Tile* [H];
	Cloud** sky = new Cloud* [H];
	Plant** weed = new Plant* [H];
	Fungi** shroom = new Fungi* [H];
	vector<Tree*> trees;
	vector<Herbivore*> a_herb;
	vector<Omnivore*> a_omni;
	vector<Carnivore*> a_carn;
	vector<Detritivore*> a_detr;
	vector<Corpse*> bodies;
	
	for(int i=0;i<H;i++){
		world[i] = new Tile[W];
		sky[i] = new Cloud[W];
		weed[i] = new Plant[W];
		shroom[i] = new Fungi[W];
		for(int j=0;j<W;j++){
			world[i][j] = *(new Tile());
			sky[i][j] = *(new Cloud());
			weed[i][j] = *(new Plant());
			shroom[i][j] = *(new Fungi());
		}
	}
	
	move(0,0);
	
	if(rand_world){
		smoothen(world);
	}
	
	while(TRUE){
		if(edit_mode == 0){
			wh_transfer(world);
			form_weather(world,sky);
			grow_grass(world,weed,shroom,&trees);
			grow_shrooms(world,shroom);
			grow_trees(world,sky,&trees);
			noahs_arc(world,&a_herb,&a_omni,&a_carn,&a_detr);
			animal_logic(world,shroom,&a_herb,&a_omni,&a_carn,&a_detr,&bodies);
			death(world,&trees,&a_herb,&a_omni,&a_carn,&a_detr,&bodies);
		}
		
		int c = getch();
		if(c == 'q'){
			break;
		}else if(c >= '1' && c<= '5'){
			display = c-'0'-1;
		}else if(c == 'e'){
			//TODO: Add edit mode, save from edit mode to txt in config file, read from config into sim
			if(edit_mode){
				edit_mode = 0;
				mvprintw(18,wx-21,"            ");
			}else{
				edit_mode = 1;
				mvprintw(18,wx-21,"Edit Mode ON");
			}
		}else if(c == KEY_UP){
			cy--;
		}else if(c == KEY_DOWN){
			cy++;
		}else if(c == KEY_LEFT){
			cx--;
		}else if(c == KEY_RIGHT){
			cx++;
		}
		if(edit_mode == 1){
			if(c == '+'){
				//Increase current visible stat on the tile/Increase Plants
			}else if(c == '*'){
				//Increase by 10x/Increase Fungi
			}else if(c == '-'){
				//Decrease current visible stat on the tile/Decrease Plants
			}else if(c == '/'){
				//Decrease by 10x/Decrease Fungi
			}else if(c == 'k'){
				//Kill entity on tile, if present
			}else if(c == 's'){
				//Spawn entity on tile, select by ID, if none present
			}
		}
		//TODO: Add graphs, save historical data somewhere
		cy+=H;cy%=H;
		cx+=W;cx%=W;
		
		show_world(display,world,sky,weed,shroom);
		timer++;
		this_thread::sleep_for(chrono::milliseconds(1000/60));
		refresh();
	}
	
	delwin(stdscr);							// Pour Holy Water over the Terminal
	endwin();								// Lift the nCurses
	return 0;
}
