extern int wx, wy, W, H;

bool cmpNames(char* a, char* b){
	int t = strcmp(b,a);
	if(t<0){
		t=0;
	}
	return t;
}

int simMenu(){
	int selection = 0;
	while(TRUE){
		mvprintw(wy/2-3,(wx-12)/2,"Random World");
		mvprintw(wy/2,(wx-12)/2,  "Preset World");
		mvprintw(wy/2+3,(wx-12)/2,"  Quit Sim  ");
		mvchgat(wy/2+(selection-1)*3,(wx-12)/2,12,A_REVERSE,0,NULL);
		int c = getch();
		if(c == KEY_DOWN){
			selection++;
			selection%=3;
		}else if(c == KEY_UP){
			selection--;
			selection+=3;
			selection%=3;
		}else if(c == ' '){
			for(int i=0;i<3;i++){
				mvprintw(wy/2-(i-1)*3,(wx-14)/2,"                 ");
			}
			return selection;
		}
		this_thread::sleep_for(chrono::milliseconds(1000/60));
		refresh();
	}
}

int simPrep(int opt){
	int selection = 0;
	int screen = 0;
	vector<char*> file_names; 				//Store all file names somewhere
	int l;
	while(TRUE){
		int c = getch();
		//If you picked the random sim option...
		if(opt == 0){
			mvprintw(wy/2-3,(wx-11)/2," Width: %2d ",W);
			mvprintw(wy/2,(wx-11)/2,  "Height: %2d ",H);
			mvprintw(wy/2+3,(wx-11)/2,"   Start   ");
			mvprintw(wy/2+6,(wx-11)/2,"  Go Back  ");
			mvchgat(wy/2+(selection-1)*3,(wx-11)/2,11,A_REVERSE,0,NULL);
			//Select option
			if(c == KEY_DOWN){
				selection++;
				selection%=4;
			}else if(c == KEY_UP){
				selection--;
				selection+=4;
				selection%=4;
			}
			//Set size:
			if(selection == 0){
				if(c == KEY_LEFT && W>5){
					W--;
				}else if(c == KEY_RIGHT && W<(wx-36)/2){
					W++;
				}
			}else if(selection == 1){
				if(c == KEY_LEFT && H>5){
					H--;
				}else if(c == KEY_RIGHT && H<wy-3){
					H++;
				}
			}
			else if(selection == 2 && c == ' '){
				for(int i=0;i<4;i++){
					mvprintw(wy/2+(i-1)*3,(wx-14)/2,"                 ");
				}
				return 1;
			}else if(selection == 3 && c == ' '){
				for(int i=0;i<4;i++){
					mvprintw(wy/2+(i-1)*3,(wx-14)/2,"                 ");
				}
				opt = simMenu();
				selection = 0;
			}
		}else if(opt == 1){
			l = file_names.size();
			//If we don't know what files we got in the config folder, check that directory out
			if(l == 0){
				DIR* dr;
				struct dirent *en;
				dr = opendir("config");
				if(dr){
					while ((en = readdir(dr)) != NULL) {
						if(en->d_name[0]!='.'){			//Look for all files that don't start with . 
							char* t = new char[20];
							memcpy(t,en->d_name,21);
							if(strlen(en->d_name)>25){	//If file's name (with the ".txt") is bigger than 20 characters, truncate at 17 and add an elipsis
								t[18]='.';
								t[19]='.';
								t[20]='.';
								t[21]='\0';
							}else{
								int i=21;				//If file's name is under 20 characters, pad out the missing space with spaces
								for(i;i>0;i--){
									if(t[i]=='.'){
										break;
									}
								}
								for(i;i<21;i++){
									t[i]=' ';
								}
								t[21]='\0';
							}
							file_names.push_back(t);	//Add it to our list
						}
					}
					closedir(dr);
				}
				l = file_names.size();
				sort(file_names.begin(),file_names.end(), cmpNames);	//Sort names alphabetically
			}
			if(l>7){
				for(int i=0;i<5;i++){
					mvprintw(wy/2-2+i,wx/2-8,"%s",file_names[i+screen]);
				}
				mvprintw(wy/2-3,(wx-10)/2,"     ᐱ ᐱ ᐱ");
				mvprintw(wy/2+3,(wx-10)/2,"     ᐯ ᐯ ᐯ");
				mvprintw(wy/2+l,(wx-10)/2,"    Go Back  ");
				if(selection != l){
					mvchgat(wy/2+selection-screen-2,wx/2-8,21,A_REVERSE,0,NULL);
				}else{
					mvchgat(wy/2+selection,(wx-6)/2,11,A_REVERSE,0,NULL);
				}
				if(c == KEY_DOWN){
					selection++;
					selection%=l+1;
					if(selection == 0){
						screen = 0;
					}
					if(selection>screen+4 && screen<l-5 && selection != l){
						screen++;
					}
				}else if(c == KEY_UP){
					selection--;
					selection+=l+1;
					selection%=l+1;
					if(selection == l-1){
						screen = l-5;
					}
					if(selection<screen && screen>0 && selection != l){
						screen--;
					}
				}else if(c == ' '){
					if(selection == l){
						for(int i=0;i<l;i++){
							mvprintw(wy/2-4+i,wx/2-10,"                       ");
						}
						mvprintw(wy/2+l,(wx-10)/2,"               ");
						opt = simMenu();
						selection = 0;
						screen = 0;
					}
				}
			}else{
				mvprintw(wy/2+l,(wx-10)/2,"    Go Back  ");
				for(int i=0;i<l;i++){
					mvprintw(wy/2-4+i,wx/2-8,"%s",file_names[i]);
				}
				if(selection != l){
					mvchgat(wy/2+selection-4,wx/2-8,21,A_REVERSE,0,NULL);
				}else{
					mvchgat(wy/2+selection,(wx-6)/2,11,A_REVERSE,0,NULL);
				}
				if(c == KEY_DOWN){
					selection++;
					selection%=l+1;
				}else if(c == KEY_UP){
					selection--;
					selection+=l+1;
					selection%=l+1;
				}else if(c == ' '){
					if(selection == l){
						for(int i=0;i<l;i++){
							mvprintw(wy/2-4+i,wx/2-10,"                       ");
						}
						mvprintw(wy/2+l,(wx-10)/2,"               ");
						opt = simMenu();
						selection = 0;
					}
				}
			}
		}else if(opt == 2){
			endSim(0,"");
		}
		this_thread::sleep_for(chrono::milliseconds(1000/60));
		refresh();
	}
}

//Show controls at the bottom
void helpBar(){
	attron(COLOR_PAIR(7));
	mvprintw(wy-1,0,"Q");
	mvprintw(wy-1,8,"TAB");
	attroff(COLOR_PAIR(7));
	attron(COLOR_PAIR(0));
	mvprintw(wy-1,2,"Quit");
	mvprintw(wy-1,12,"Toggle Graphs");
	if(wx>=90){
		mvprintw(wy-1,29,"Altitude");
		mvprintw(wy-1,41,"Humidity");
		mvprintw(wy-1,53,"Temperature");
		mvprintw(wy-1,68,"Clouds");
		mvprintw(wy-1,78,"Life");
		attroff(COLOR_PAIR(0));
		attron(COLOR_PAIR(7));
		mvprintw(wy-1,27,"G");
		mvprintw(wy-1,39,"H");
		mvprintw(wy-1,51,"T");
		mvprintw(wy-1,66,"C");
		mvprintw(wy-1,76,"L");
		attroff(COLOR_PAIR(7));
	}else{
		mvprintw(wy-1,37,"Alternate Views");
		attroff(COLOR_PAIR(0));
		attron(COLOR_PAIR(7));
		mvprintw(wy-1,27,"G/H/T/C/L");
		attroff(COLOR_PAIR(7));
	}
}

void showStats(int d, Tile ground, Cloud sky, Plant grass, Fungi shroom){
	move(1,wx-30);
	attron(COLOR_PAIR(7));
	hline(' ',28);
	vline(' ',wy-3);
	move(wy-3,wx-30);
	hline(' ',28);
	move(1,wx-2);
	vline(' ',wy-3);
	attron(A_REVERSE);
	mvprintw(3,wx-28,"Current Mode:");
	mvprintw(6,wx-28,"Tile Stats:");
	attroff(COLOR_PAIR(7));
	
	int i,lim = 0;
	//Display Altitude
	attroff(A_REVERSE);
	mvprintw(8,wx-19,"               ");
	attron(A_REVERSE);
	attron(COLOR_PAIR(77));
	mvprintw(8,wx-28,"Altitude");
	attroff(COLOR_PAIR(77));
	lim = (ground.altit+500)/300;
	for(i=0;i<lim;i++){
		attron(COLOR_PAIR(i*2+59));
		mvprintw(8,wx-19+i,"■");
		attroff(COLOR_PAIR(i*2+59));
	}
	attron(COLOR_PAIR(i*2+59));
	mvprintw(8,wx-8,"%d",ground.altit);
	attroff(COLOR_PAIR(i*2+59));
	
	//Display Temperature
	attroff(A_REVERSE);
	mvprintw(9,wx-19,"               ");
	attron(A_REVERSE);
	attron(COLOR_PAIR(97));
	mvprintw(9,wx-28,"Heat");
	attroff(COLOR_PAIR(97));
	lim = (ground.temp+15)/5;
	for(i=0;i<lim;i++){
		attron(COLOR_PAIR(i*2+79));
		mvprintw(9,wx-19+i,"■");
		attroff(COLOR_PAIR(i*2+79));
	}
	attron(COLOR_PAIR(i*2+79));
	mvprintw(9,wx-8,"%.1f",ground.temp);
	attroff(COLOR_PAIR(i*2+79));
	
	//Display Humidity
	attroff(A_REVERSE);
	mvprintw(10,wx-19,"               ");
	attron(A_REVERSE);
	attron(COLOR_PAIR(117));
	mvprintw(10,wx-28,"Water");
	attroff(COLOR_PAIR(117));
	lim = ground.water/100;
	for(i=0;i<lim;i++){
		attron(COLOR_PAIR(i*2+99));
		mvprintw(10,wx-19+i,"■");
		attroff(COLOR_PAIR(i*2+99));
	}
	attron(COLOR_PAIR(i*2+99));
	mvprintw(10,wx-8,"%.1f",ground.water);
	attroff(COLOR_PAIR(i*2+99));
	
	//Display Clouds
	attroff(A_REVERSE);
	mvprintw(11,wx-19,"               ");
	attron(A_REVERSE);
	attron(COLOR_PAIR(137));
	mvprintw(11,wx-28,"Clouds");
	attroff(COLOR_PAIR(137));
	lim = sky.water/100;
	for(i=0;i<lim;i++){
		attron(COLOR_PAIR(i*2+119));
		mvprintw(11,wx-19+i,"■");
		attroff(COLOR_PAIR(i*2+119));
	}
	attron(COLOR_PAIR(i*2+119));
	mvprintw(11,wx-8,"%.1f",sky.water);
	attroff(COLOR_PAIR(i*2+119));
	
	//Display the Living:
	attroff(A_REVERSE);
	for(i=0;i<4;i++){
			mvprintw(13+i,wx-28,"                          ");
	}
	attron(A_REVERSE);
	switch(ground.id){
		case 0:{
			//Show Plants
			attron(COLOR_PAIR(17));
			mvprintw(13,wx-28,"Plants");
			attroff(COLOR_PAIR(17));
			for(i=0;i<grass.num;i++){
				attron(COLOR_PAIR(i+8));
				mvprintw(13,wx-19+i,"■");
				attroff(COLOR_PAIR(i+8));
			}
			attron(COLOR_PAIR(i+8));
			mvprintw(13,wx-8,"%d",grass.num);
			attroff(COLOR_PAIR(i+8));
			
			//Show Fungi
			attron(COLOR_PAIR(48));
			mvprintw(14,wx-28,"Fungi ");
			attroff(COLOR_PAIR(48));
			for(i=0;i<shroom.num;i++){
				attron(COLOR_PAIR(i*10+8));
				mvprintw(14,wx-19+i*2,"■■");
				attroff(COLOR_PAIR(i*10+8));
			}
			attron(COLOR_PAIR(i*10+8));
			mvprintw(14,wx-8,"%d",shroom.num);
			attroff(COLOR_PAIR(i*10+8));
			break;
		}case 1:{
			attroff(A_REVERSE);
			mvprintw(13,wx-28,"Entity ");
			mvprintw(14,wx-28,"Health ");
			mvprintw(15,wx-28,"Energy ");
			mvprintw(16,wx-28,"Food:   ");
			attron(A_REVERSE);
			attron(COLOR_PAIR(138));
			mvprintw(13,wx-19,"Tree");
			attroff(COLOR_PAIR(138));
			Tree* t = (Tree*)ground.entity;
			
			lim = (t->health-1)/2;
			for(i=0;i<lim;i++){
				attron(COLOR_PAIR(i+144));
				mvprintw(14,wx-19+i,"■");
				attroff(COLOR_PAIR(i+144));
			}
			attron(COLOR_PAIR(144+i));
			mvprintw(14,wx-8,"%d",t->health);
			attroff(COLOR_PAIR(144+i));
			
			lim = (t->energy-1)/3;
			for(i=0;i<lim;i++){
				attron(COLOR_PAIR(i+154));
				mvprintw(15,wx-19+i,"■");
				attroff(COLOR_PAIR(i+154));
			}
			attron(COLOR_PAIR(154+i));
			mvprintw(15,wx-8,"%d",t->energy);
			attroff(COLOR_PAIR(154+i));
			
			lim = t->food-1;
			for(i=0;i<lim+1;i++){
				attron(COLOR_PAIR(i+164));
				mvprintw(16,wx-19+i*2,"■■");
				attroff(COLOR_PAIR(i+164));
			}
			attron(COLOR_PAIR(i+163));
			mvprintw(16,wx-8,"%d",t->food);
			attroff(COLOR_PAIR(i+163));
			break;
		}case 2:{
			attroff(A_REVERSE);
			mvprintw(13,wx-28,"Entity ");
			mvprintw(14,wx-28,"Health ");
			attron(A_REVERSE);
			attron(COLOR_PAIR(139));
			mvprintw(13,wx-19,"Corpse");
			attroff(COLOR_PAIR(139));
			Corpse* b = (Corpse*)ground.entity;
			
			lim = b->health/30;
			for(i=0;i<lim;i++){
				attron(COLOR_PAIR(i+144));
				mvprintw(14,wx-19+i,"■");
				attroff(COLOR_PAIR(i+144));
			}
			attron(COLOR_PAIR(144+i));
			mvprintw(14,wx-8,"%d",b->health);
			attroff(COLOR_PAIR(144+i));
			break;
		}case 3:{
			attroff(A_REVERSE);
			mvprintw(13,wx-28,"Entity ");
			mvprintw(14,wx-28,"Health ");
			mvprintw(15,wx-28,"Energy ");
			attron(A_REVERSE);
			attron(COLOR_PAIR(140));
			mvprintw(13,wx-19,"Herbivore");
			attroff(COLOR_PAIR(140));
			Herbivore* h = (Herbivore*)ground.entity;
			
			lim = h->health/6;
			for(i=0;i<lim;i++){
				attron(COLOR_PAIR(i+144));
				mvprintw(14,wx-19+i,"■");
				attroff(COLOR_PAIR(i+144));
			}
			attron(COLOR_PAIR(144+i));
			mvprintw(14,wx-8,"%d",h->health);
			attroff(COLOR_PAIR(144+i));
			
			lim = h->energy/6;
			for(i=0;i<lim;i++){
				attron(COLOR_PAIR(i+154));
				mvprintw(15,wx-19+i,"■");
				attroff(COLOR_PAIR(i+154));
			}
			attron(COLOR_PAIR(154+i));
			mvprintw(15,wx-8,"%d",h->energy);
			attroff(COLOR_PAIR(154+i));
			break;
		}case 4:{
			attroff(A_REVERSE);
			mvprintw(13,wx-28,"Entity ");
			mvprintw(14,wx-28,"Health ");
			mvprintw(15,wx-28,"Energy ");
			attron(A_REVERSE);
			attron(COLOR_PAIR(141));
			mvprintw(13,wx-19,"Omnivore");
			attroff(COLOR_PAIR(141));
			Omnivore* o = (Omnivore*)ground.entity;
			
			lim = o->health/9;
			for(i=0;i<lim;i++){
				attron(COLOR_PAIR(i+144));
				mvprintw(14,wx-19+i,"■");
				attroff(COLOR_PAIR(i+144));
			}
			attron(COLOR_PAIR(144+i));
			mvprintw(14,wx-8,"%d",o->health);
			attroff(COLOR_PAIR(144+i));
			
			lim = o->energy/6;
			for(i=0;i<lim;i++){
				attron(COLOR_PAIR(i+154));
				mvprintw(15,wx-19+i,"■");
				attroff(COLOR_PAIR(i+154));
			}
			attron(COLOR_PAIR(154+i));
			mvprintw(15,wx-8,"%d",o->energy);
			attroff(COLOR_PAIR(154+i));
			break;
		}case 5:{
			attroff(A_REVERSE);
			mvprintw(13,wx-28,"Entity ");
			mvprintw(14,wx-28,"Health ");
			mvprintw(15,wx-28,"Energy ");
			attron(A_REVERSE);
			attron(COLOR_PAIR(142));
			mvprintw(13,wx-19,"Carnivore");
			attroff(COLOR_PAIR(142));
			Tree* t = (Tree*)ground.entity;
			
			lim = t->health/10;
			for(i=0;i<lim;i++){
				attron(COLOR_PAIR(i+144));
				mvprintw(14,wx-19+i,"■");
				attroff(COLOR_PAIR(i+144));
			}
			attron(COLOR_PAIR(144+i));
			mvprintw(14,wx-8,"%d",t->health);
			attroff(COLOR_PAIR(144+i));
			
			lim = t->energy/6;
			for(i=0;i<lim;i++){
				attron(COLOR_PAIR(i+154));
				mvprintw(15,wx-19+i,"■");
				attroff(COLOR_PAIR(i+154));
			}
			attron(COLOR_PAIR(154+i));
			mvprintw(15,wx-8,"%d",t->energy);
			attroff(COLOR_PAIR(154+i));
			break;
		}case 6:{
			attroff(A_REVERSE);
			mvprintw(13,wx-28,"Entity ");
			mvprintw(14,wx-28,"Health ");
			mvprintw(15,wx-28,"Energy ");
			attron(A_REVERSE);
			attron(COLOR_PAIR(143));
			mvprintw(13,wx-19,"Detritivore");
			attroff(COLOR_PAIR(143));
			Detritivore* d = (Detritivore*)ground.entity;
			
			lim = d->health/5;
			for(i=0;i<lim;i++){
				attron(COLOR_PAIR(i+144));
				mvprintw(14,wx-19+i,"■");
				attroff(COLOR_PAIR(i+144));
			}
			attron(COLOR_PAIR(144+i));
			mvprintw(14,wx-8,"%d",d->health);
			attroff(COLOR_PAIR(144+i));
			
			lim = d->energy/6;
			for(i=0;i<lim;i++){
				attron(COLOR_PAIR(i+154));
				mvprintw(15,wx-19+i,"■");
				attroff(COLOR_PAIR(i+154));
			}
			attron(COLOR_PAIR(154+i));
			mvprintw(15,wx-8,"%d",d->energy);
			attroff(COLOR_PAIR(154+i));
			break;
		}
	}
	
	switch(d){
		case 0:
			attron(COLOR_PAIR(17));
			mvprintw(3,wx-14,"Life       ");
			attroff(COLOR_PAIR(17));
		break;
		case 1:
			attron(COLOR_PAIR(77));
			mvprintw(3,wx-14,"Altitude   ");
			attroff(COLOR_PAIR(77));
		break;
		case 2:
			attron(COLOR_PAIR(97));
			mvprintw(3,wx-14,"Temperature");
			attroff(COLOR_PAIR(97));
		break;
		case 3:
			attron(COLOR_PAIR(117));
			mvprintw(3,wx-14,"Humidity   ");
			attroff(COLOR_PAIR(117));
		break;
		case 4:
			attron(COLOR_PAIR(130));
			mvprintw(3,wx-14,"Clouds     ");
			attroff(COLOR_PAIR(130));
		break;
	}
	attroff(A_REVERSE);
}

//Show the entire grid to the right of the center
void show_world(int d, Tile** world, Cloud** sky, Plant** grass, Fungi** shrooms){	
	for(int i=0;i<H;i++){
		move((wy-H-1)/2+i,(wx-32-W*2)/2);
		for(int j=0;j<W;j++){
			int a;
			if(cx == j && cy == i){
				a = 1;
			}else{
				if(d==0){
					if(world[i][j].id == 0){
						a = 8+grass[i][j].num + shrooms[i][j].num*10;
					}else{
						a = 137+world[i][j].id;
					}
				}else if(d==1){
					a = 58+(world[i][j].altit+500)/150;
				}else if(d==2){
					a = 78+(world[i][j].temp+15)/2.5;
				}else if(d==3){
					a = 98+world[i][j].water/50;
				}else{
					a = 118+sky[i][j].water/50;
				}
			}
			attron(COLOR_PAIR(a));
			addch(' ');
			addch(' ');
			attroff(COLOR_PAIR(a));
		}
	}
	showStats(d, world[cy][cx], sky[cy][cx], grass[cy][cx], shrooms[cy][cx]);
}

