#include <dirent.h>
#include<conio.h>
#include<fstream.h>
#include<stdlib.h>
#include<stdio.h>
#include<dos.h>
#include<dir.h>
#include<string.h>
#include<time.h>
#include<bios.h>
#include<graphics.h>
#include "QRius\\MYMACROS.h"
char abbrev[MAX_ABBREV][100], fullname[MAX_ABBREV][100];
int nabbrev;
char references[REFERENCES][20] = {"Monika", "Just monika", "monika"};
char* SPECIAL_CHARS = "*!@#$%^&*()!";
int corrupted = 0;
void log(char* text) {
	ofstream fout("C:\\JCreator\\Log.txt", ios::app);
	fout << text << endl;	
}
void lognumber(char* text, int number) {
	char buffer[300];
	ofstream fout("C:\\JCreator\\Log.txt", ios::app);
	fout << text << " " << itoa(number, buffer, 10) << endl;	
}
void main() {
	int i,j, key, flag = 0;
	if ( !CheckFile(PATH,NULL,MONIKACHR,NULL)
		&& CheckFile(PATH,NULL,CPP_MONIKA,NULL) )
			Restore();
	Player player;
	int result;
	char buffer[300];
	DeleteFolder(DUMP);
	DeleteFolder(QDUMP);
	mkdir(pathof(DUMP, NULL, NULL, buffer));
	mkdir(pathof(QDUMP, NULL, NULL, buffer));
	int gdriver = DETECT, gmode, errorcode;
	initgraph(&gdriver, &gmode, GRAPHICSPATH);
	cleardevice();
	if(!ReadTextFiles())  {
		outtextxy(OFFSET_X, OFFSET_Y + 90, gettext(20, buffer));		//No QFiles detected
		getch();
		exit(0);
	}

		for (j = 0; j <= 290; j+=5) {
		cleardevice();
		if (flag) break;
		key = bioskey(1);
		if(key > 0) flag = 1;
		if (flag) break;
		settextstyle(10,0,6);
		setcolor(7);
		outtextxy(480 - 270, j - 100, "R");
		settextstyle(10,0,5);
		outtextxy(510 - 250,j - 40,"I");
		settextstyle(10,0,4);
		setcolor(8);
		outtextxy(530 - 245,j,"U");
		outtextxy(560 - 237,j,"S");
		delay(40);
	}
	setfillstyle(SOLID_FILL, BLACK);
	for (i = 275, j = 120; i <= 390 || j <= 290; i+=5, j+=5) {
		if (flag) break;
		key = bioskey(1);
		if(key > 0) flag = 1;
		settextstyle(10,0,16);
		setcolor(12);
		outtextxy(470- 205 + 10,20 + 30,"Q");
		bar(i,j,390,290);
		delay(80);
	}


	for (i = 0; i <= 260; i+=5) {
		cleardevice();
		if (flag) break;
		key = bioskey(1);
		if(key > 0) flag = 1;
		settextstyle(10,0,16);
		setcolor(12);
		outtextxy(470 - 195 + i,20 + 30,"Q");
		settextstyle(10,0,6);
		setcolor(7);
		outtextxy(480 - 270 + i, 260 - 70, "R");
		settextstyle(10,0,5);
		outtextxy(510 - 250 + i, 280 - 30,"I");
		settextstyle(10,0,4);
		setcolor(8);
		outtextxy(530 - 245 + i,300 - 10,"U");
		outtextxy(560 - 237 + i,300 - 10 ,"S");
		delay(20);
	}

	delay(50);
	displaymainscreen();
	char k = 0;
	while (k != 27) {
		switch(k) {
			case '2':
				ShowLeaderBoard(SCORE);
				char kk = '0';
				while (kk != 27) {
					switch(kk) {
						case '1':  ShowLeaderBoard(SCORE); break;
						case '2':  ShowLeaderBoard(SPEED); break;
						case '3':  ShowLeaderBoard(DURATION); break;
						case '4':  ShowLeaderBoard(ACCURACY); break;
					}
					kk = getch();
				}
				cleardevice();
				displaymainscreen();
				break;
			case '1':
				cleardevice();
				if (CheckFile(PATH,NULL,MONIKACHR,NULL)) {
					result = login_monika();
					strcpy(player.uname,"Monika");
					strcpy(player.password,"Just Monika");
				}
				else 
					result = Login(player);
				if (result == -1) {
					displaymainscreen();
					break;
				}
				NewGame(player);
				cleardevice();
				displaymainscreen();
				break;
		}
		k = getch();
	}
	DeleteFolder(DUMP);
	DeleteFolder(QDUMP);
}

void displaymainscreen () {
	char buffer[300];
	int i,j;
	
	cleardevice();
	settextstyle(10,0,16);
	setcolor(12);
	outtextxy(470 - 195 + 260,20 + 30,"Q");
	settextstyle(10,0,6);
	setcolor(7);
	outtextxy(480 - 270 + 260, 260 - 70, "R");
	settextstyle(10,0,5);
	outtextxy(510 - 250 + 260, 280 - 30,"I");
	settextstyle(10,0,4);
	setcolor(8);
	outtextxy(530 - 245 + 260,300 - 10,"U");
	outtextxy(560 - 237 + 260,300 - 10 ,"S");
	delay(20);

	setcolor(4);
	settextstyle(3,0,2);
	outtextxy(10,60,"Press 1 to login, 2 to enter leaderboard");
	settextstyle(9,0,2);
	setcolor(7);
	outtextxy(55,120,gettext(1, buffer));		// Login
	outtextxy(55,270,gettext(2, buffer));			// Leaderboards
	setcolor(8);
	settextstyle(10,0,5);
	outtextxy(10,100,"1.");
	outtextxy(10,250,"2.");
}

int Login(Player& player)
{
   int result=0;
   *(player.uname) = 0;
   *(player.password) = 0;
   print_login_screen(UNAME, player, "", 1);
   char buf[300];
   result = getdata(player, SIGNIN, UNAME);
   if (result==-1) displaymainscreen();
   return result;
}

/* Read from TXT only if there are no leader board entries  - Sample view of LBoard*/
int ReadLeaderBoard() {
	char buffer[300], buffer1[300];
	int i;
	i = countinfile(DATA, LBOARD, DAT, sizeof(LeaderboardEntry));
	if(i > 0) return i;
	ifstream fin(pathof(DATA, LBOARD, TXT, buffer), ios::in);
	if (!fin) return BADFILE;
	ofstream fout(pathof(DATA, LBOARD, DAT, buffer), ios::out|ios::binary);	
	fin.seekg(0);
	LeaderboardEntry lbe;
	for(i = 0; !fin.eof(); i++) {
		fin.getline(lbe.uname, 100, '\n');
		fin.getline(lbe.gamemode, 100, '\n');
		fin.getline(lbe.date, 100, '\n');
		fin.getline(buffer, 100, '\n'); lbe.score = atoi(buffer);
		fin.getline(buffer, 100, '\n'); lbe.accuracy = atof(buffer);
		fin.getline(buffer, 100, '\n'); lbe.speed = atof(buffer);
		fin.getline(buffer, 100, '\n'); lbe.duration = atoi(buffer);
		fin.getline(buffer, 100, '\n'); lbe.qresult.qright = atoi(buffer);
		fin.getline(buffer, 100, '\n'); lbe.qresult.qwrong = atoi(buffer);
		fout.write((char *)&lbe, sizeof(LeaderboardEntry));
	}
	fout.write((char *)&lbe, sizeof(LeaderboardEntry));
	return i;
}

void ShowLeaderBoard(int attribute) {
	char buffer[300];
	int a[] = {10,10,10,10};
	attribute == SCORE ? a[0] = YELLOW : attribute == SPEED ? a[1] = YELLOW : attribute == DURATION ? a[2] = YELLOW : a[3] = YELLOW;
	cleardevice();
	settextstyle(5,0,7);
	setcolor(5);
    outtextxy(190,10, gettext(30, buffer));			//Leaderboard
	settextstyle(0,0,1);
	setcolor(10);
	outtextxy(OFFSET_X, OFFSET_Y + 30, gettext(31, buffer));			//Name
	outtextxy(OFFSET_X + 100, OFFSET_Y + 30, gettext(32, buffer));		//Date
	outtextxy(OFFSET_X + 200, OFFSET_Y + 30, gettext(33, buffer));		//Game
	setcolor(a[0]);
	outtextxy(OFFSET_X + 290, OFFSET_Y + 30, gettext(34, buffer));		//Score
	setcolor(a[1]);
	outtextxy(OFFSET_X + 370, OFFSET_Y + 30, gettext(35, buffer));		//Speed
	setcolor(a[2]);
	outtextxy(OFFSET_X + 460, OFFSET_Y + 30, gettext(36, buffer));		//Duration
	setcolor(a[3]);
	outtextxy(OFFSET_X + 550, OFFSET_Y + 30, gettext(37, buffer));		//Accuracy
	LeaderboardEntry lbe;
	LBoardSort sortedarray[LEADERBOARD_FILE_LIMIT];
	int i, n = SortLeaderBoard(attribute, sortedarray);
	if(n > LEADERBOARD_LIMIT) n = LEADERBOARD_LIMIT;
	setcolor(11);
	for(i = 0; i < n; i++) {
		lbe = getleaderboardentry(sortedarray[i].index);
		setcolor(25);
		outtextxy(OFFSET_X, OFFSET_Y + (i + 1) * DY/2 + 30, lbe.uname);
		outtextxy(OFFSET_X + 100, OFFSET_Y + (i + 1) * DY/2 + 30, lbe.date);
		outtextxy(OFFSET_X + 205, OFFSET_Y + (i + 1) * DY/2 + 30, lbe.gamemode);
		outtextxy(OFFSET_X + 297, OFFSET_Y + (i + 1) * DY/2 + 30, itoa(lbe.score, buffer, 10));
		sprintf(buffer, "%.2f", lbe.speed);
		outtextxy(OFFSET_X + 375, OFFSET_Y + (i + 1) * DY/2 + 30, buffer);
		outtextxy(OFFSET_X + 467, OFFSET_Y + (i + 1) * DY/2 + 30, itoa(lbe.duration/1000, buffer, 10));
		itoa((int) lbe.accuracy, buffer, 10);
		strcat(buffer, " %");
		outtextxy(OFFSET_X + 560, OFFSET_Y + (i + 1) * DY/2 + 30, buffer);
	}
	setcolor(YELLOW);
	outtextxy(10, getmaxy() - 15, gettext(38, buffer));			// 1, 2, 3, 4 to sort
}

void NewGame(Player player) {
	LeaderboardEntry lbe;
	int selected[MAX_TOPICS];
	char buffer[300];
	int sel;
	int mode[] = { 0, 0 };
	remove(pathof(DUMP, WORK, DAT, buffer));
	SelectMode(mode);
	char topics[MAX_TOPICS][100];
	SelectTopic(topics, selected);
	cleardevice();
	sel = mode[0] == CLASSIC
		? GameUI(player, CLASSIC, 0, mode[1], lbe)
		: mode[0] == ARCADE
		? GameUI(player, ARCADE, mode[1] * 1000L, 1000, lbe)
		: GameUI(player, TIME_TRIAL, mode[1] * (long) 1000, 1000, lbe);
	setcolor(WHITE);
	if(sel == ABORTED) return;
	AddLeaderboardEntry(lbe);
	PostGame(lbe);
}

int getdata(Player& player, int op, int field) {
	int i, a,key,len,x,y;
	char buffer[300];
	char* string = field == UNAME ? player.uname : player.password;
	char* p;
	p = string;
	int aborted = 0;
	while (!aborted) {
		key = bioskey(1);
		if(key <= 0) continue;
		key = bioskey(0);
		char sz[10];
		switch(key) {
				case B_F2_KEY:
					if(op == SIGNIN) {
						*(player.uname) = 0;
						*(player.password) = 0;
						print_login_screen(field, player, gettext(40, buffer), 1);			//Creating...
						return getdata(player, CREATE, UNAME);
					}
					break;
				case B_ESC: aborted = 1; cleardevice(); break;
				case B_BACKSPACE:
					if(p == string) break;
					p--;
					*p = 0;
					print_login_screen(field, player, "", 1)	;
					break;
				case B_ENTER: switch(op) {
					case SIGNIN:
						if(field == PASS) {
							if(-1 != (i = authenticate(player.uname, string))) return i;
						} else {
							if(uname_matches(op, string, buffer) == 1) {
								strcpy(player.uname, buffer);
								p = string + strlen(player.uname);
								print_login_screen(PASS, player, "", 1);
								return getdata(player, SIGNIN, PASS);

							}
						}
						print_login_screen(field, player, (field == UNAME
							? gettext(41, buffer)		//Username does not exist
							: gettext(42, buffer)), 1);		//Invalid password
						break;
					case CREATE:
						if(field == UNAME) {
							if (uname_matches(op, string, buffer) == 0) {
								print_login_screen(PASS, player, "", 1);
								return getdata(player, CREATE, PASS);
							}
						}
						else {
							ofstream fout(pathof(DATA, PLAYERS, DAT, buffer), ios::app|ios::binary);
							fout.write((char *)&player, sizeof(Player));
							fout.close();
							print_login_screen(field, player, gettext(43, buffer), 1);		//Thank you for creating your account
							getch();
							return countinfile(DATA, PLAYERS, DAT, sizeof(Player));
							}
						break;
					}
					break;
				default:
					if(strlen(string) > 20) {
						print_login_screen(field, player, gettext(44, buffer), 1);			//Cannot exceed 20 characters
						break;
					}
					*p++ = key; *p = 0;
					print_login_screen(field, player, "", 1);
					switch(op) {
						case SIGNIN:
							if(field == UNAME && uname_matches(op, string, buffer) == 1) {
								strcpy(player.uname, buffer);
								p = string + strlen(player.uname);
								print_login_screen(UNAME, player, "", 1);
								}
							else if(field == PASS && -1 != (i
								= authenticate(player.uname, string))) return i;
								break;
						case CREATE:
							if(field == UNAME && uname_matches(op, string, buffer) > 0) {
								print_login_screen(UNAME,player, gettext(45, buffer), 1);		//Username already exists
							}
							break;
					}
					break;
			}
	}
	return -1;
}

void print_login_screen(int state, Player player, char* error, int hidepassword) {
   int maxx, maxy, key, a, i, len;
   char buffer[300];
   settextstyle(0,0,1);
   setcolor(4);
   outtextxy(20,450, gettext(4, buffer));		//Press ESC to exit
   setcolor(6);
   outtextxy(400,450, gettext(5, buffer));		//To create new account press F2
   settextstyle(5,0,7);
   setcolor(5);
   outtextxy(220,20, gettext(6, buffer));		//Login
   settextstyle(0,0,2);
   setcolor(11);
   outtextxy(150,170, gettext(7, buffer));		//Username
   setcolor(7);
   settextstyle(0,0,2);
   setfillstyle(SOLID_FILL, BLACK);
   bar(350, 170, 650, 190);
   outtextxy(350,170, player.uname);
   if(state == PASS) {
	   setcolor(9);
	   settextstyle(0,0,2);
	   setfillstyle(SOLID_FILL, BLACK);
	   bar(150, 240, 650, 260);
	   outtextxy(150,240, gettext(8, buffer));		//Password
	   if (hidepassword) {
			len = strlen(player.password);
			char *p = buffer;
			for(i = 0; i < len; i++) *p++ = '*';
			*p = 0;
	   }
	   else 
		   strcpy (buffer, player.password);
	   setcolor(8);
	   settextstyle(0,0,2);
	   outtextxy(350,240, buffer);
	}
	settextstyle(0,0,1);
	setfillstyle(SOLID_FILL, BLACK);
	if (state == UNAME) {
		bar(200, 240, 650, 260);
		outtextxy(200,240, error);
	} else {
		bar(200, 310, 650, 330);
		outtextxy(200,310, error);
	}
}

int uname_matches(int op, char* string, char* match) {
	Player player;
	char buffer[300];
	int result = 0;
	*match = 0;
	if(*string == 0) return -1;
	ifstream fin(pathof(DATA, PLAYERS, DAT, buffer), ios::in|ios::binary);
	if (!fin) return BADFILE;
	fin.seekg(0);
	for(int count = 0; count < LIMIT; count++) {
		if(0 == fin.read((char *)&player, sizeof(Player))) break;
		if(op == SIGNIN && strstr(player.uname, string)) {
			strcpy(match, player.uname);
			result++;
		}
	}
	fin.close();
	return result;
}

/* Returns index of uname entry if password matches, -1 otherwise */
int authenticate(char* uname, char* password) {
	Player player;
	char buffer[300];
	if(*password == 0) return -1;
	ifstream fin(pathof(DATA, PLAYERS, DAT, buffer), ios::in|ios::binary);
	if (!fin) return BADFILE;
	fin.seekg(0);
	for(int count = 0; count < LIMIT; count++) {
		if(0 == fin.read((char *)&player, sizeof(Player))) break;
		if(strcmp(uname,  player.uname) == 0) {
			fin.close();
			return strcmp(password, player.password) == 0 ? count : -1;
		}
	}
	fin.close();
	return -1;
}

void SelectMode(int* cursor) {
	int key = 0, len;
	DisplayModeSelection(cursor);
	while(1) {
		key = bioskey(1);
		if(key <= 0) continue;
		key = bioskey(0);
		switch(key) {
			case B_SPACE: break;
			case B_ENTER:
				int classic[] = { 10, 20, 30 };
				int timetrial[] = { 10, 30, 60 };
				int arcade[] = { 30, 60, 120 };
				int* amounts = cursor[0] == CLASSIC ? classic : cursor[0] == TIME_TRIAL ? timetrial : arcade;
				cursor[1] = amounts[cursor[1]];
				cleardevice();
				return;
			case B_UP_ARROW: cursor[0] = (cursor[0] + 3 - 1) % 3;  break;
			case B_LEFT_ARROW: cursor[1] = (cursor[1] + 3 - 1) % 3;  break;
			case B_DOWN_ARROW: cursor[0] = (cursor[0] + 1) % 3;   break;
			case B_RIGHT_ARROW: cursor[1] = (cursor[1] + 1) % 3;   break;
		}
		DisplayModeSelection(cursor);
	}
}

void DisplayModeSelection(int* cursor) {
	cleardevice();
	settextstyle(0,0,1);
	setcolor(WHITE);	
	int len  = NO_OF_MODES;
	int i;
	char buffer[300], buffer2[300], buffer3[300], s[2];
//	setcolor(RED);
	setcolor(5);
	settextstyle(5,0,7);
	outtextxy(OFFSET_X, OFFSET_Y - 60, gettext(9, buffer));			//Select Mode
	for(i = 0; i < len; i++) {
		setcolor(LIGHTGREEN);
		settextstyle(0,0,1);
		s[1] = 0; s[0] = (cursor[0] == i) ? MYCURSOR : ' '; 
		outtextxy(OFFSET_X + 20, OFFSET_Y + 50 + i * DY/2, s);

		if (cursor[0] == i) 
			setcolor(15);
		else 
			setcolor(8);
		gettext(i + 10, buffer);			//CLASSIC, TIME TRIAL, ARCADE

		outtextxy(OFFSET_X + 20 + CURSOR_PADDING, OFFSET_Y + 50 + i * DY/2, buffer);
		
	}
	switch (cursor[0]) {
		case CLASSIC: 
			strcpy(buffer3, gettext(46,buffer));
			strcat(buffer3, gettext(47,buffer));
			break;
		case TIME_TRIAL:
			strcpy(buffer3, gettext(48,buffer));
			strcat(buffer3, gettext(49,buffer));
			strcat(buffer3, gettext(50,buffer));
			break;
		case ARCADE:
			strcpy(buffer3, gettext(51,buffer));
			strcat(buffer3, gettext(52,buffer));
			break;
	}
	int classic[] = { 10, 20, 30 };
	int timetrial[] = { 10, 30, 60 };
	int arcade[] = { 30, 60, 120 };
	int* amounts = cursor[0] == CLASSIC ? classic : cursor[0] == TIME_TRIAL ? timetrial : arcade;
//	setcolor(RED);
	setcolor(5);
	settextstyle(5,0,4);
	outtextxy(OFFSET_X, OFFSET_Y + 160, gettext(cursor[0] == CLASSIC  ? 13 : 14, buffer));			//Select Number of Questions/ Time Limit
	for(i = 0; i < 3; i++) {
		
		setcolor(LIGHTGREEN);
		settextstyle(0,0,1);
		s[1] = 0; s[0] = (cursor[1] == i) ? MYCURSOR : ' ';
		outtextxy(OFFSET_X + 20 - CURSOR_PADDING + i * (DX + 90), OFFSET_Y + 200 + 30, s);

		strcpy(buffer, itoa(amounts[i], buffer2, 10));
		if (cursor[1] == i) setcolor(15);
		else setcolor(8);
		outtextxy(OFFSET_X + 20 + i * (DX + 90), OFFSET_Y + 200 + 30, buffer);
	}
	
	setcolor(YELLOW);
	settextstyle(0,0,1);
	char* p = strtok(buffer3, " ");
	for(i =0; p; i++) {
		buffer[0] = 0;
		while(p && textwidth(buffer) + OFFSET_X < getmaxx() * 11/15) {
			strcat(buffer, p);
			strcat(buffer, " ");
			p = strtok(NULL, " ");
		}
		outtextxy(OFFSET_X, 400 + 20 * i, buffer);
	}
	
}
int ExtractAllTopics(char topics[][100], int* selected, int flag) {
	char buffer[300], buffer1[300];
	char* topic;
	struct ffblk ffblk;
	int done = findfirst(pathof(QDUMP, "*", DAT, buffer), &ffblk, 0);
	int i, topiccount = 0;
	while(!done) {
		strcpy(buffer, ffblk.ff_name);
		topic = strtok(buffer, ".");
		if(flag) topic = RelabelTopic(topic, buffer1);
		for(i = 0; i < topiccount; i++) {
			if(strcmpi(topics[i], topic) == 0) break;
		}
		if(i == topiccount) {
			strcpy(topics[topiccount], topic);
			if(selected) selected[topiccount] = 0;
			topiccount++;
		}
		done = findnext(&ffblk);
	}
	return topiccount;
}
char* RelabelTopic(char* name, char* buffer) {
	int i, len = strlen(name);
	strcpy(buffer, name);
	if(CheckFile(PATH, DATA, ABBREV, TXT)) {
		for(i = 0; i < len; i++) {
			if(buffer[i] >= '0' && buffer[i] <= '9') break;
		}
		buffer[i] = 0;
		for(i = 0; i < nabbrev; i++) {
			if(strcmpi(buffer, 	abbrev[i]) == 0) strcpy(buffer, fullname[i]);
		}
	}
	return buffer;
}


int SelectTopic(char topics[][100], int* selected) {
	char buffer[300], buffer1[300];
	struct ffblk ffblk;
	int i, done, rows, columns, lastrowcolumns,status = NOT_COMPLETED;
	ifstream fin(pathof(DATA, ABBREV, TXT, buffer), ios::in);
	if (fin) {
		fin.seekg(0);
		for(nabbrev = 0; !fin.eof(); nabbrev++) {
			fin.getline(abbrev[nabbrev], 100, '\n');
			fin.getline(fullname[nabbrev], 100, '\n');
		}
	}
	int topiccount = ExtractAllTopics(topics, selected, 1);
	int selectedtopiccount = 0;
	columns = MAX_COLS;
	if(topiccount < columns) columns = topiccount;
	rows = (topiccount / MAX_COLS);
	if(topiccount % MAX_COLS != 0) rows += 1;
	lastrowcolumns = topiccount % columns;
	int key = 1, currentpos, flag = 0;
	Cursor cursor = { 0, 0 };
	DisplayTopicSelection(topics, selected, topiccount, rows, columns, cursor, flag);
	while(status != COMPLETED) {
		key = bioskey(1);
		if(key <= 0) continue;
		key = bioskey(0);
		switch(key) {
			case B_SPACE:
				if (flag==1) {
					setcolor(BLACK);
					outtextxy(30,40, gettext(15, buffer));		//Select topic
					flag = 0;
				}
				currentpos = cursor.x + columns * cursor.y;
				selected[currentpos] = (selected[currentpos] + 1) % 2;
				if (selected[currentpos] == 1) selectedtopiccount++;
				else selectedtopiccount--;
				break;
			case B_LEFT_ARROW:
				if (flag==1) {
					setcolor(BLACK);
					outtextxy(30,40, gettext(15, buffer));		//Select topic
					flag = 0;
				}
				cursor.x = cursor.x == 0 ? columns - 1 : cursor.x - 1;  break;
			case B_RIGHT_ARROW:
				if (flag==1) {
					setcolor(BLACK);
					outtextxy(30,40, gettext(15, buffer));		//Select topic
					flag = 0;
				}
				cursor.x = cursor.x == columns - 1 ? 0 : cursor.x + 1;   break;
			case B_UP_ARROW:
				if (flag==1) {
					setcolor(BLACK);
					outtextxy(30,40, gettext(15, buffer));		//Select topic
					flag = 0;
				}
				cursor.y = cursor.y == 0 ? rows - 1 : cursor.y - 1;  break;
			case B_DOWN_ARROW:
				if (flag==1) {
					setcolor(BLACK);
					outtextxy(30,40, gettext(15, buffer));		//Select topic
					flag = 0;
				}
				cursor.y = cursor.y == rows - 1 ? 0 : cursor.y + 1;   break;
			case B_ENTER:
				setcolor(12);
				if (selectedtopiccount == 0) {
					outtextxy(30,40, gettext(15, buffer));		//Select topic
					flag = 1;
					break;
				}
				else
					status = COMPLETED; break;
		}
		if(cursor.y == rows - 1  && lastrowcolumns && cursor.x > lastrowcolumns - 1) {
			switch(key) {
				case B_LEFT_ARROW: cursor.x = cursor.x == 0 ? lastrowcolumns - 1 : cursor.x - 1; break;
				case B_RIGHT_ARROW: cursor.x = 0; break;
				case B_UP_ARROW: cursor.y = cursor.y - 1; break;
				case B_DOWN_ARROW: cursor.y = 0; break;
			}
		}
		DisplayTopicSelection(topics, selected, topiccount, rows, columns, cursor, flag);
	}
	int j = 0;
	for(i = 0; i < topiccount; i++) {
		if(selected[i] == 1) {
			strcpy(buffer, topics[i]);
			buffer[4] = 0;
			MakeQuestions(j++, buffer);
		}
	}
	int n = countinfile(DUMP, WORK, DAT, sizeof(Question));
	for(int ii = 0; ii < n; ii++) {
	   Question q = fetchquestion(pathof(DUMP, WORK, DAT, buffer), ii);
	}
	return topiccount;
}

void DisplayTopicSelection(char topics[][100], int* selected, int topiccount, int rows, int columns, Cursor cursor, int flag) {
	cleardevice();
	char buffer[300];
	if (flag) {
		setcolor(BLACK);
		settextstyle(0,0,1);
		outtextxy(OFFSET_X, 400 , gettext(53, buffer));		//Toggle space to select
		setcolor(RED);
		outtextxy(OFFSET_X, 400, gettext(15, buffer));		//Select topic error
	}
	settextstyle(0,0,1);
	setcolor(WHITE);
	int i, j, k, count,x;
	char* p;
	char s[2];
	count = 0;

//	setcolor(RED);
	setcolor(5);
	settextstyle(5,0,7);
	outtextxy(OFFSET_X, OFFSET_Y - 50, gettext(16, buffer));		//Select Topic 
	
	if (!flag) {
		setcolor(BROWN);
		settextstyle(0,0,1);
		outtextxy(OFFSET_X, 400 , gettext(53, buffer));		//Toggle space to select
	}	
	setcolor(2);
	settextstyle(0,0,1);
	for(i = 0; i < rows; i++) {
		for(j = 0; j < columns; j++) {
			if(count == topiccount) break;

			setcolor(LIGHTGREEN);
			s[1] = 0; s[0] = (cursor.x == j && cursor.y == i) ? MYCURSOR : ' '; 
			outtextxy(OFFSET_X + j * DX + 20 , OFFSET_Y + 60 + i * DY, s);

			setcolor(LIGHTCYAN);
			s[1] = 0; s[0] = (selected[count] == 1) ? SELECT_START : ' ';
			outtextxy(OFFSET_X + j * DX + 20 + CURSOR_PADDING , OFFSET_Y + 60 + i * DY - VERT_PADDING, s);

			if (selected[count] == 1) setcolor(YELLOW);
			else setcolor(LIGHTGRAY);
			outtextxy(OFFSET_X + j * DX + 20 + 2 * CURSOR_PADDING + HORIZ_PADDING, OFFSET_Y + 60 + i * DY, topics[count]);
			x = textwidth(topics[count]);
			
			setcolor(LIGHTCYAN);
			s[1] = 0; s[0] = (selected[count] == 1) ? SELECT_END : ' ';
			outtextxy(OFFSET_X + j * DX + x + 30 + 2 * CURSOR_PADDING + HORIZ_PADDING, OFFSET_Y + 60 + i * DY + VERT_PADDING, s);

			count++;
		}
	}
}

/* Copy all .txt to .dat files in the directory PATH QBANK*/
int ReadTextFiles() {
	char answer[20];
	char buffer[300], buffer1[300];
	struct ffblk ffblk;
//	strcpy(buffer, PATH);
//	strcat(buffer, QBANK);
	pathof(QBANK,NULL,NULL,buffer);
	strcat(buffer, "\\*.txt");
	int topiccount = 0, done = findfirst(buffer, &ffblk, 0);
	while(!done) {
//		strcpy(buffer, PATH);
//		strcat(buffer, QBANK);
		pathof(QBANK,NULL,NULL,buffer);
		strcat(buffer, "\\");
		strcat(buffer, ffblk.ff_name);
		
		strcpy(buffer1, PATH);
		strcat(buffer1, QDUMP);
		strcat(buffer1, "\\");
		strcat(buffer1, strtok(ffblk.ff_name, "."));
		strcat(buffer1, DAT);
		
		ifstream fin(buffer, ios::in);
		if (!fin) return BADFILE;
		ofstream fout(buffer1, ios::out|ios::binary);
		fin.seekg(0);
		Question q;
		while (!fin.eof()) {
			fin.getline(q.question, 300, '\n');
			for (int j = 0;j < 4; j++) {
				fin.getline(q.option[j], 300, '\n');
			}
			fin.getline(answer, 20, '\n');
			q.answer = atoi(answer);
			q.answer--;
			fout.write((char *)&q, sizeof(Question));
		}
		topiccount++;
		done = findnext(&ffblk);
	}
	return topiccount;
}


void MakeQuestions(int index, char* topic) {
	struct ffblk ffblk;
	int i = index, j = 0, done;
	char copy_command[300], pattern[100], buffer[300], fn[300];
	strcpy(copy_command, "copy ");
	strcpy(pattern, topic);
	strcat(pattern, "*");
	done = findfirst(pathof(QDUMP, pattern, DAT, buffer), &ffblk, 0);
	ofstream fout(pathof(DUMP, WORK, DAT, buffer), ios::app|ios::binary);
	for(i = 0; !done; i++) {
		strcpy(fn, ffblk.ff_name);
		int n = countinfile(QDUMP, fn, "", sizeof(Question));
		for(j = 0; j < n; j++) {
			Question q = fetchquestion(pathof(QDUMP, fn, "", buffer), j);
			if(!strlen(q.question)) break;
			fout.write((char *)&q, sizeof(Question));
		}
		done = findnext(&ffblk);
	}
	fout.close();
//	int n = countinfile(DUMP, WORK, DAT, sizeof(Question));
}

void FisherYates(int* a, int n) {
	int i; for(i = 0; i < n; i++) a[i] = i;
	for(int k = 0; k < n; k++) {
		swap(a, k, k + random(n - k));
	}
}

void swap(int* a, int i, int j) {
	int temp = a[i];
	a[i] = a[j];
	a[j] = temp;
}


int GameUI(Player player, int mode, long timebank, int qlimit, LeaderboardEntry& lbe) {
	randomize();
	int score, j, i, n, array[300], answer = 1, flagcolor = 0, monkia_index = 0;
	QResult qresult = {0,0,0,0};
	char buffer[300];
	long duration,flagtime = -1;
	duration = mode == CLASSIC ? 0 : timebank;
	Question q;
	UQuestion uq;
	FisherYates(array, n = countinfile(DUMP, WORK, DAT, sizeof(Question)));
	for(int ii = 0; ii < n; ii++) {
	   q = fetchquestion(pathof(DUMP, WORK, DAT, buffer), ii);
	}
	for(score = 0; qresult.qtotal < n && qresult.qtotal < qlimit && timebank >= 0;) {
		if(corrupted) log("I am here");
		if(answer > 0) {
			int count = countinfile(DUMP, WORK, DAT, sizeof(Question));
			if(qresult.qtotal >= count) {
				cleardevice();
				settextstyle(10,0,15);
				outtextxy(140,20,"OUT");
				settextstyle(9,0,1);
				setcolor(4);
				outtextxy(160,270,"   OF QUESTIONS");
			//	outtext(gettext(19, buffer));		//Out of questions
			//	log("....Asking for question beyond limit");
				break;
			}
			q = fetchquestion(pathof(DUMP, WORK, DAT, buffer), array[qresult.qtotal]);
			show_question(qresult.qtotal, q, SHOW);
		}
		switch (flagcolor) {
				case PLUSTEN : answer = GetAnswer(mode, timebank, PLUSTEN, flagtime, monkia_index);  break;
				case MINUSFIVE : answer = GetAnswer(mode, timebank, MINUSFIVE, flagtime, monkia_index);  break;
				default : answer = GetAnswer(mode, timebank,0,flagtime, monkia_index);  break;
		}
		switch(answer) {
			default:
				break;
			case -1:
				log("Got -1 Got To Go Aborting...");
				cleardevice();
				remove(pathof(DUMP, WORK, DAT, buffer));
				return ABORTED;
			case -2:
				log("Got -2 Got To Go Aborting...");
				cleardevice();
				remove(pathof(DUMP, WORK, DAT, buffer));
				continue;
			case -3:
				MakeMonikaGreatAgain();
				show_question(qresult.qtotal, q, SHOW);
				break;
		}
		// Checking answer
		if(answer <= 0) ;
		else {
			uq.uanswer = answer;
			uq.answer = q.answer;
			strcpy(uq.question, q.question);
			for(j = 0; j < 4; j++) strcpy(uq.option[j], q.option[j]);
			ofstream fout (pathof(DUMP, QPLAYED, DAT, buffer),ios::app|ios::binary);
			fout.write((char*)&uq,sizeof(UQuestion));
			fout.close();

			if (uq.uanswer == 5) qresult.qpassed++;
			else if(uq.uanswer == q.answer) {
				score += 4;
				qresult.qright++;
				if (mode == ARCADE)	{
					timebank += 10000;
					duration += 10000;
					flagcolor = PLUSTEN;
					flagtime = 1000;
				}
			}
			else {
				qresult.qwrong++;
				score -= mode == ARCADE ? 0 : mode == CLASSIC ? 2 : 1;
				if (mode == ARCADE) {
					if (timebank < 5000) {
						duration -= timebank;
						timebank = 0;
					}
					else {
						timebank -= 5000;
						duration -= 5000;
						flagcolor = MINUSFIVE;
						flagtime = 1000;
					}
				}
			}
		}
		delay(DELAY);
		timebank += (mode == CLASSIC ? 1 : -1) * DELAY;
		flagtime -= DELAY;
		if (answer > 0) {
			show_question(qresult.qtotal++, q, ERASE);
			log("Erased question");
		}

		if (qresult.qtotal == n-1) {
/*			cleardevice();
			setcolor(15);
			settextstyle(10,0,15);
			outtextxy(140,20,"OUT");
			settextstyle(9,0,1);
			setcolor(4);
			outtextxy(160,270,"   OF QUESTIONS");
		//	outtext(gettext(19, buffer));		//Out of questions
*/			getch();
			break;
		}
		if (timebank % CORRUPTFREQUENCY == 0
			&& CheckFile(PATH, NULL, MONIKACHR, NULL)) {
				corrupted = 1;
				log("Corrupt Messages and Questions");
				char topics[MAX_TOPICS][100];
				int topiccount = ExtractAllTopics(topics, NULL, 0);
				log("ExtractedAllTopics");
				CorruptLinesTXT(RandLineTXT(QBANK, topics[random(topiccount)]));
				log("Corrupted questions text");
				CorruptLinesDAT(RandLineDAT(DUMP, WORK, sizeof(Question)));
				lognumber("Corrupted Work DAT Question Count!", countinfile(DUMP, WORK, DAT, sizeof(Question)));
				CorruptLinesTXT(RandLineTXT(DATA, TEXT));

	ifstream fin (pathof(DATA, TEXT, TXT, buffer), ios::in);
	fin.seekg(0);
	int count = 0;
	for(int i = 0; !fin.eof(); i++) {
		fin.getline(buffer, 300, '\n');
		count++;
	}
	fin.close();


				lognumber("Corrupted Labels", count);
		//		CorruptLinesTXT(RandLineTXT(DATA, LBOARD));
		//		log("Corrupted Lboard");
		}
	}
	char date[15];
	_strdate(date);
	strcpy(lbe.date,date);
	if (mode == CLASSIC) duration = timebank;
	strcpy(lbe.uname, player.uname);
	lbe.score = score;
	lbe.duration = duration;
	lbe.accuracy = qresult.qtotal ? (100.0 * qresult.qright)/qresult.qtotal : 0;
	lbe.speed = (1000 * ((float)qresult.qtotal)/(duration ? duration : 1));
	lbe.qresult = qresult;
	mode == CLASSIC ? strcpy(lbe.gamemode,gettext(10, buffer))			//CLASSIC
		: mode == ARCADE ? strcpy(lbe.gamemode,gettext(12, buffer))			//ARCADE
		: strcpy(lbe.gamemode,gettext(11, buffer));			//TIME TRIAL
	remove(pathof(DUMP, WORK, DAT, buffer));
	return SUCCESS;
}

Question fetchquestion(char* fname, int n) {
	char answer[20], buffer[300];
	Question q;
	memset((void*)&q, 0, sizeof(Question));
	ifstream fin(fname, ios::in|ios::binary);
	if (!fin) return q;
	fin.seekg((long) n * sizeof(Question));
	fin.read((char *)&q, sizeof(Question));
 //	int workdatlen = countinfile(DUMP, WORK, DAT, sizeof(Question));
	if(0 == strlen(q.option[0])) {
		lognumber("EMPTY FIRST OPTION question number!", n);
		log(q.question);
		lognumber("EMPTY FIRST OPTION WHEN Question Count!", countinfile(DUMP, WORK, DAT, sizeof(Question)));
	}
	fin.close();
	return q;
}

/* answer 1,2,3,4, or 5 if TAB key, or -1 to exit,-2 for timeout, -3 for Monika, 0 otherwise */
/* F2 twice to come out of pause */
int GetAnswer(int mode, long& timebank, int flagcolor, long& flagtime, int& monkia_index) {
	int bkeysmonika[10] = {B_m, B_o, B_n, B_i, B_k, B_a};
	int key;
	while(1) {
		show_time(mode, timebank,flagcolor,flagtime);
		delay(DELAY);
		timebank += (mode == CLASSIC ? 1 : -1) * DELAY;
		flagtime -= DELAY;
		if(timebank < 0) return TIMEDOUT;
		if(bioskey(1) > 0) {
			key = bioskey(0);
			if(monkia_index == strlen("MONIKA") - 1 && !CheckFile(PATH,NULL,MONIKACHR,NULL)) return -3;
			if(key == bkeysmonika[monkia_index]) monkia_index++;
			else monkia_index = 0;
			switch(key) {
				case B_F2_KEY: pause(); return 0;
				case B_ESC: return -1;
				case B_TAB: return 5;
				case B_1: return 1;
				case B_2: return 2;
				case B_3: return 3;
				case B_4: return 4;					
				default: return 0;
			}
		}
	}
}

void show_question(int index, Question q, int erase) {
	if(index < 0) return;
	settextstyle(2,0,6);
	setcolor(RED);
	if(erase == ERASE) {
//		setcolor(BLACK);
		cleardevice();
		return;	
	}
	char* p = strtok(q.question, " ");
	int i;
	char buffer[300];
	for(i =0; p; i++) {
		buffer[0] = 0;
		while(p && textwidth(buffer) + OFFSET_X < getmaxx() * 11/15) {
			strcat(buffer, p);
			strcat(buffer, " ");
			p = strtok(NULL, " ");
		}
		outtextxy(CX, CY + 20 * i, buffer);
	}
	setcolor(WHITE);
	for(int j = 0; j < 4; j++) {
		outtextxy(CX, CY + 35 + j * 30 + i * 20, itoa(j + 1, buffer, 10));
		outtextxy(CX + 40, CY + 35 + j * 30 + i * 20, q.option[j]);
	}
	setcolor(14);
	if(erase == ERASE) setcolor(BLACK);
	outtextxy(CX, 400, gettext(17, buffer));			//Tab to pass, F2 to pause
	//CY + 200 + i * 20
}

void show_time(int mode, long timebank, int flagcolor, long flagtime) {
	int blink = (mode != CLASSIC && timebank < 10000 ) ? BLINKRED : 0;
	char buffer[300];
	int a, b;
	setfillstyle(SOLID_FILL, BLACK);
	a = CX * 30 + 170;
	b = 1 * CY;
	b -= 30;
	bar(a, b, a + 400, b + 230);
	if (blink == BLINKRED) setcolor(RED);
	else setcolor(WHITE);
	if (flagtime>=0) {
		if (flagcolor == PLUSTEN) setcolor(GREEN);
		else if (flagcolor == MINUSFIVE) setcolor (RED);
		else setcolor(WHITE);
	}
	settextstyle(0,0,2);
	outtextxy(CX * 30, b, gettext(18, buffer));			//Time:
	timebank /=  10;
	if(blink == BLINKRED) {
		if(timebank % 20 != 0) return;
		sprintf(buffer, "%ld : %ld", timebank / 100, timebank % 100);
	}
	else sprintf(buffer, "%ld : %ld", timebank / 100, timebank % 100);
	settextstyle(4,0,2);
	outtextxy(a, b, buffer);
}

void PostGame(LeaderboardEntry lbe) {
	int i = 0,x,y;
	char buffer[300];
	cleardevice();
	int dy = DY/2;
	int label = 150;
	settextstyle(5,0,7);
	setcolor(12);
    outtextxy(220,20, gettext(21, buffer));			//Post Game
	
	settextstyle(0,0,1);
	setcolor(4);
	
	settextstyle(0,0,2);
    setcolor(11);
	strcpy(buffer,lbe.uname);
	strcat(buffer," || ");
	strcat(buffer, lbe.gamemode);
	outtextxy(OFFSET_X , OFFSET_Y + i * dy + 40, buffer);
	i++;
	
	settextstyle(0,0,1);
	
	//Time Elapsed
	setcolor(15);
	outtextxy(getmaxx() - 200,OFFSET_Y + i * dy + 70, gettext(22, buffer));			
	sprintf(buffer, "%ld", lbe.duration/1000);
	outtextxy(getmaxx() - 200 + label, OFFSET_Y + i * dy + 70, buffer);
	
	//Attemtped
	setcolor(3);
	outtextxy(OFFSET_X, OFFSET_Y + i * dy + 70, gettext(23, buffer));			
	outtextxy(OFFSET_X + label, OFFSET_Y + i++ * dy + 70, itoa(lbe.qresult.qtotal, buffer, 10));
	
	//Score
	setcolor(13);
	outtextxy(getmaxx() - 200, OFFSET_Y + i * dy + 70, gettext(24, buffer));			
	outtextxy(getmaxx() -200 + 70, OFFSET_Y + i * dy + 70, itoa(lbe.score, buffer, 10));

	//Right
	setcolor(14);
	outtextxy(OFFSET_X, OFFSET_Y + i * dy + 70, gettext(25, buffer));			
	outtextxy(OFFSET_X + label, OFFSET_Y + i++ * dy + 70, itoa(lbe.qresult.qright, buffer, 10));

	//Wrong
	outtextxy(OFFSET_X, OFFSET_Y + i * dy + 70, gettext(26, buffer));			
	outtextxy(OFFSET_X + label, OFFSET_Y + i++ * dy + 70, itoa(lbe.qresult.qwrong, buffer, 10));

	//Passed
	outtextxy(OFFSET_X, OFFSET_Y + i * dy + 70, gettext(27, buffer));
	outtextxy(OFFSET_X + label, OFFSET_Y + i++ * dy + 70, itoa(lbe.qresult.qpassed, buffer, 10));

	//Accuracy
	setcolor(10);
	outtextxy(OFFSET_X, OFFSET_Y + i * dy + 70, gettext(28, buffer));
	itoa((int) lbe.accuracy, buffer, 10);
	strcat(buffer, " %");
	outtextxy(OFFSET_X + label, OFFSET_Y + i++ * dy + 70, buffer);
	
	//Speed
	outtextxy(OFFSET_X, OFFSET_Y + i * dy + 70, gettext(29, buffer));
	sprintf(buffer, "%.2f", lbe.speed);
	strcat(buffer, "");
	outtextxy(OFFSET_X + label, OFFSET_Y + i++ * dy + 70, buffer);

	int key;
	while(1) {
		if(bioskey(1) > 0) {
			key = bioskey(0);
			if(key == B_ENTER || key == B_ESC)
				break;
		}
	}
	return;
}

int AddLeaderboardEntry(LeaderboardEntry lbe) {
	char buffer[300];
	ofstream fout(pathof(DATA, LBOARD, DAT, buffer), ios::app|ios::binary);
	fout.write((char *)&lbe, sizeof(LeaderboardEntry));
	fout.close();
	return countinfile(DATA, LBOARD, DAT, sizeof(LeaderboardEntry));
}

LeaderboardEntry getleaderboardentry(int index) {
	char buffer[300];
	LeaderboardEntry lbe;
	memset((void*)&lbe,0,sizeof(LeaderboardEntry));
	ifstream fin(pathof(DATA, LBOARD, DAT, buffer), ios::in|ios::binary);
	if (!fin) return lbe;
	fin.seekg(0);
	fin.read((char *) &lbe, sizeof(LeaderboardEntry));
	for(int i = 0; i < index; i++) {
		fin.read((char *) &lbe, sizeof(LeaderboardEntry));
	}
	fin.close();
	return lbe;
}

// Sort by duration, accuracy, speed, score
int SortLeaderBoard(int attribute, LBoardSort arr[]) {
	LeaderboardEntry lbe;
	char buffer[300];
	int max, i, j, n = countinfile(DATA, LBOARD, DAT, sizeof(LeaderboardEntry));
	ifstream fin(pathof(DATA, LBOARD, DAT, buffer), ios::in|ios::binary);
	if (!fin) return BADFILE;
	fin.seekg(0);
	for(i = 0; i < n; i++) {
		fin.read((char *) &lbe, sizeof(LeaderboardEntry));
		arr[i].index = i;
		arr[i].attrib = attribute == SCORE
			? (float) lbe.score
			: attribute == DURATION
				? (float) lbe.duration
				: attribute == SPEED
					? lbe.speed
					: lbe.accuracy;
	}
	fin.close();
	for(i = 0; i < n - 1; i++) {
		for(j = i + 1; j < n; j++) {
			if(arr[j].attrib > arr[i].attrib) swapLBoardSort(arr, i, j);
		}
	}
	return n;
}

void swapLBoardSort(LBoardSort* a, int i, int j) {
	int temp;
	temp = a[i].index;
	a[i].index = a[j].index;
	a[j].index = temp;
	float ftemp;
	ftemp = a[i].attrib;
	a[i].attrib = a[j].attrib;
	a[j].attrib = ftemp;
}

void DeleteFolder(char* folder) {
	char buffer[300];
	struct ffblk ffblk;
	int done = findfirst(pathof(folder, "*", ".*", buffer), &ffblk, 0);
	while(!done) {
		unlink(pathof(folder, ffblk.ff_name, NULL, buffer));
		done = findnext(&ffblk);
	}
	rmdir(pathof(folder, NULL, NULL, buffer));
}

char* gettext(int index, char *buffer) {
	ifstream fin(pathof(DATA, TEXT, TXT, buffer), ios::in|ios::binary);
	if (!fin) return "BADFILE";
	fin.seekg(0);
	for(int i = 0; !fin.eof(); i++) {
		fin.getline(buffer, 300, '\n');
		if(i == index) { 
			fin.close(); 
			buffer[strlen(buffer)-1] = 0;
			return buffer; 
		}
	}
	fin.close();
	buffer[0] = 0;
	return buffer;
}	

int countinfile(char* folder, char *name, char* extn, int size) {
	char buffer[300];
	ifstream fin(pathof(folder, name, extn, buffer), ios::in|ios::binary);
	if (!fin) return BADFILE;
	fin.seekg(0, ios::end);
	int n = fin.tellg()/size;
	fin.close();
	return n;
}

char* pathof(char* folder, char* name, char* extn, char* result) {
	strcpy(result, PATH);
	int i = CheckFile(PATH,NULL,MONIKACHR,NULL);
	if(folder) {
		strcat(result,
			(i && 0 == strcmp(folder, DATA)) ? DATA_MONIKA
			: (i && 0 == strcmp(folder, QBANK)) ? QBANKNEW_MONIKA
			: folder);
		if(name) strcat(result, "\\");
	}
	if(name) strcat(result,
		(i && 0 == strcmp(name, MAINCPP)) ? CPP_MONIKA
		: name);
	if(extn) strcat(result, extn);
	return result;
}
void pause() {
	// TO DO - add the output for "Paused" on the screen
	while(1) {
		if(bioskey(1) > 0) {
			if(bioskey(0) == B_F2_KEY) {
			   // remove the word paused
			   return;
			}
		}
	}
}

void print(Player player) {
	char buffer[300];
	ifstream fin(pathof(DATA, PLAYERS, DAT, buffer), ios::in|ios::binary);
	fin.seekg(0);
	char someVar;
	for(int count = 0; count < LIMIT; count++) {
		if(0 == fin.read((char *)&player, sizeof(Player))) break;
		cout << (count + 1) << " " << player.uname << " " << player.password << endl;
	}
	fin.close();
}

void MakeMonikaGreatAgain() {
	log("MakeMonikaGreatAgain...");
	char buffer[300];
	delay(4000);
	cleardevice();
	outtextxy(10,15, gettext(39, buffer));		//Just Monika
	delay(2000);
	
	//to be completed
	
	int result;

	strcpy(buffer, DESTINATION);
	buffer[strlen(buffer) - 1] = 0;
	mkdir(buffer);
	CopyFolder(PATH, NULL, DESTINATION, NULL);
	CopyFolder(PATH, DATA, DESTINATION, DATA);
	CopyFolder(PATH, QBANK, DESTINATION, QBANK);
	strcpy(buffer, ABBREV);
	strcat(buffer, DAT);
	DeleteFile(PATH, DATA, buffer);

	MangleLBE();	
	CorruptFolder(PATH, DATA, PATH, DATA_MONIKA);
	CorruptFolder(PATH, QBANK, PATH, QBANK_MONIKA);

	//CorruptFolder(PATH, DUMP, PATH, "KAMONI");
	CorruptFile(PATH, NULL, MAINCPP, PATH, CPP_MONIKA);
//	CorruptFile(PATH, NULL, MYMACROSH, PATH, H_MONIKA);
	strcpy(buffer, ABBREV);
	strcat(buffer, TXT);
	DeleteFile(PATH, DATA_MONIKA, buffer);
	strcpy(buffer, PLAYERS);
	strcat(buffer, DAT);
	DeleteFile(PATH, DATA_MONIKA, buffer);

	CorruptTopics();

	ofstream fout(pathof(NULL, "Monika", ".CHR", buffer), ios::out|ios::binary);
	fout.write("Just Monika", 12);
	fout.close();

	cleardevice();
}

void Restore () {
	CopyFolder(DESTINATION, NULL, PATH, NULL);
	CopyFolder(DESTINATION, DATA, PATH, DATA);
	CopyFolder(DESTINATION, QBANK, PATH, QBANK);

	DeleteFolder2(PATH, DATA_MONIKA);
	DeleteFolder2(PATH, QBANKNEW_MONIKA);
	DeleteFile(PATH, NULL, CPP_MONIKA);
//	DeleteFile(PATH, NULL, H_MONIKA);
	DeleteFolder2(DESTINATION, DATA);
	DeleteFolder2(DESTINATION, QBANK);
	DeleteFolder2(DESTINATION, NULL);
}

int login_monika () {	
	char unameMonika[50], passMonika[20];
	Player player;
	player.uname[0] = 0;
	player.password[0] = 0;
	strcpy(unameMonika,"Monika");
	strcpy(passMonika,"Just Monika");
	int i,key, aborted = 0, counter = 1;
	print_login_screen(UNAME, player, "", 1);
	while (!aborted) {
		key = bioskey(1);
		if(key <= 0) continue;
		key = bioskey(0);
		switch(key) {
			case B_ESC: 
				aborted = 1; cleardevice(); break;
			default: 
				if (counter >= strlen(unameMonika) + strlen(passMonika) + 1) return 0;
				if (counter <= strlen(unameMonika)) {
					for (i = 0; i < counter; i++) 
						player.uname[i] = unameMonika[i];
					player.uname[i] = 0;
					print_login_screen(UNAME, player, "", 1);
				}
				else {
					for (i = 0; i < counter - strlen(unameMonika); i++) 
						player.password[i] = passMonika[i];
					player.password[i] = 0;
					print_login_screen(PASS, player, "", 0);
				}
				counter++;
				break;
		}
	}  
	return -1;
}

char* InsertMonika(char* text, int maxcycles, int splchars, int maxsize) {
	int i, len = strlen(text);
	int random1, pos;
	int repeat = random(maxcycles);
	while (repeat >= 0) {
		random1 = random (REFERENCES);
		pos = random(len);
		for (i = 0; i < strlen(references[random1]); i++) {
			if (pos + i > maxsize - 3) break;
			text[pos + i] = references[random1][i];
		}
		if (pos + i > len) text[pos + i] = 0;
		random1 = random (5);
		pos = random (len);
		for (i = 0; i < random1; i++) {
			if (pos + i > maxsize - 3) break;
			text[pos + i] = '0' + random(10);
		}
		if (pos + i > len) text[pos + i] = 0;
		if(splchars) {
			pos = random (len);
			random1 = random (3);
			for (i = 0; i < random1; i++) {
				if (pos + i > maxsize - 3) break;
				text[pos + i] = SPECIAL_CHARS[random(strlen(SPECIAL_CHARS))];
			}
			if (pos + i > len) text[pos + i] = 0;
		}
		repeat--;
		log(text);
	}
	return text;
}


void DeleteFolder2(char* path, char* folder) {
	char buffer[300];
	struct ffblk ffblk;
	strcpy(buffer, path);
	if(folder) strcat(buffer, folder);
	if(folder) strcat(buffer, "\\"); 
	strcat(buffer, "*.*");
	int done = findfirst(buffer, &ffblk, 0);
	while(!done) {
		strcpy(buffer, path);
		if(folder) strcat(buffer, folder);
		if(folder) strcat(buffer, "\\"); 
		strcat(buffer, ffblk.ff_name);
		unlink(buffer);
		done = findnext(&ffblk);
	}
	strcpy(buffer, path);
	if(folder) strcat(buffer, folder);
	if(folder) rmdir(buffer);
	strcpy(buffer, path);
	buffer[strlen(buffer) - 1] = 0;
	if(!folder) rmdir(buffer);
}
void DeleteFile(char* path, char* folder, char* file) {	
	char buffer[300];
	strcpy(buffer, path);
	if(folder) strcat(buffer, folder);
	if(folder) strcat(buffer, "\\");
	strcat(buffer, file);
	unlink(buffer);
}
void CorruptFolder(char* srcpath, char* src, char* destpath, char* dest) {
	char buffer[300], buffer1[300];
	strcpy(buffer, srcpath);
	strcat(buffer, src);
	strcpy(buffer1, destpath);
	strcat(buffer1, dest);
	mkdir(buffer1);
	CopyFolder(srcpath, src, destpath, dest);
	DeleteFolder2(srcpath, src);
}
/* To Rename the file Copy the File and Delete the Old File */
void MangleLBE() {
	char buffer[300], buffer1[300];
	strcpy(buffer, PATH);
	strcat(buffer, DATA_MONIKA);
	mkdir(buffer);
	ifstream fin(pathof(DATA, LBOARD, DAT, buffer), ios::in|ios::binary);
	ofstream fout(pathof(DATA_MONIKA, "temp", DAT, buffer), ios::out|ios::binary);
	fin.seekg(0);
	LeaderboardEntry lbe;
	while(!fin.eof()) {
			if(0 == fin.read((char*)&lbe, sizeof(LeaderboardEntry))) break;
			strcpy(lbe.uname, "Monika");
			fout.write((char *)&lbe, sizeof(LeaderboardEntry));
	}
	fin.close();
	fout.close();
	unlink(pathof(DATA, LBOARD, DAT, buffer));
	rename(pathof(DATA_MONIKA, "temp", DAT, buffer), pathof(DATA_MONIKA, LBOARD, DAT, buffer1));
}
void CorruptFile(char* srcpath, char* srcfolder, char* src, char* destpath, char* dest) {
	char buffer[300];
	strcpy(buffer, "copy ");
	strcat(buffer, srcpath);
	if(srcfolder) strcat(buffer, srcfolder);
	if(srcfolder) strcat(buffer, "\\");
	strcat(buffer, src);
	strcat(buffer, " ");
	strcat(buffer, destpath);
	strcat(buffer, dest);
	perror(buffer);
	system(buffer);
	strcpy(buffer, srcpath);
	strcat(buffer, src);
	unlink(buffer);
}
void CopyFolder(char* source, char* sourcefolder, char* dest, char* destfolder) {
	char buffer[300];	
	if(destfolder) {
		strcpy(buffer, dest);
		strcat(buffer, destfolder);
		mkdir(buffer);
	}
	strcpy(buffer, "COPY ");
	strcat(buffer, source);
	if(sourcefolder) strcat(buffer, sourcefolder);
	if(sourcefolder) strcat(buffer, "\\");
	strcat(buffer, "*.*");
	strcat(buffer, " ");
	strcat(buffer, dest);
	if(destfolder) strcat(buffer, destfolder);
	else buffer[strlen(buffer) - 1] = 0;
	perror(buffer);
	system(buffer);
}

int CheckFile(char *path, char *folder, char *file, char *ext) {
	char buffer[300];
	strcpy(buffer,path);
	if (folder) {
			strcat(buffer, folder);
			strcat(buffer, "//");
	}
	strcat(buffer, file);
	if (ext) {
		strcat(buffer, ext);
	}
	ifstream checkFile(buffer);
    if(checkFile.fail())
        return 0;
	else 
		return 1;
}

void CorruptTopics () {
	char buffer[300], buffer1[300], buffer2[300];
	mkdir(pathof(QBANKNEW_MONIKA, NULL, NULL, buffer2));
	struct ffblk ffblk;
	int done = findfirst(pathof(QBANK_MONIKA, "*", ".*", buffer1), &ffblk, 0);
	while(!done) {
		strcpy(buffer, ffblk.ff_name);
		buffer[strlen(buffer) - 4] = 0;
		InsertMonika(buffer, TOPICCYCLES, 0, 95);
		strcpy(buffer1, "copy ");
		strcat(buffer1, pathof(QBANK_MONIKA, ffblk.ff_name, NULL, buffer2));
		strcat(buffer1, " ");
		strcat(buffer1, pathof(QBANKNEW_MONIKA, buffer, TXT, buffer2));
		system(buffer1);

		done = findnext(&ffblk);
	}
	DeleteFolder2(PATH, QBANK_MONIKA);
}

LineEntry RandLineDAT (char* folder, char* file, int sizeofstruct) {
	char buffer[300];
	LineEntry randline;
	memset((void*)&randline, 0, sizeof(LineEntry));
	ifstream fin (pathof(folder, file, DAT, buffer), ios::in|ios::binary);
	if (!fin) return randline;
	LeaderboardEntry lbe;
	Question question;
	Player player;
	char* p = (sizeofstruct == sizeof(LeaderboardEntry) ? (char *)&lbe 
		: sizeofstruct == sizeof(Player) ? (char *)&player : (char *)&question);
	fin.seekg(0);
	strcpy(randline.path, buffer);
	for(int count = 0; !fin.eof(); count++)
		if(0 == fin.read(p, sizeofstruct)) break;
	fin.close();
	randline.index = random (count);
	randline.sizeofstruct = sizeofstruct;
	return randline;
}

LineEntry RandLineTXT (char* folder, char* file) {
	LineEntry randline;
	char buffer[300];
	memset((void*)&randline,0,sizeof(LineEntry));
	ifstream fin (pathof(folder, file, TXT, buffer), ios::in);
	if (!fin) return randline;
	fin.seekg(0);
	strcpy(randline.path, buffer);
	randline.sizeofstruct = -1;
	for (int count = 0; !fin.eof(); count++)
		if(!fin.getline(buffer, 300, '\n')) break;
	fin.close();
	randline.index = random (count);
	return randline;
}

void CorruptLinesDAT (LineEntry randline) {
	if (randline.sizeofstruct <= 0) return;
	char buffer[300], buffer2[300], buffer3[300];
	strcpy(buffer3, randline.path);
	strcpy(buffer2, strtok(buffer3, "."));
	strcat(buffer2, "temp");
	strcat(buffer2, DAT);
	ofstream fout (buffer2, ios::out|ios::binary);
	ifstream fin (randline.path, ios::in|ios::binary);
	fin.seekg(0);
	fout.seekp(0);
	LeaderboardEntry lbe;
	Question question;
	Player player;
	char* p = (randline.sizeofstruct == sizeof(LeaderboardEntry) ? (char *)&lbe
		: randline.sizeofstruct == sizeof(Player) ? (char *)&player : (char *)&question);
	for(int i = 0; !fin.eof(); i++) {
		if(0 == fin.read(p, randline.sizeofstruct)) break;
		if (i == randline.index) {
			switch (randline.sizeofstruct) {
				case sizeof(Question):
					InsertMonika(question.question, QUESTIONCYCLES, 1, 150);
					for (int j = 0; j < 4; j++)
						if(!random(2)) strcpy(question.option[j], InsertMonika(question.option[j], OPTIONCYCLES, 1, 100));
					break;
				case sizeof(Player) :
					InsertMonika(player.uname, PLAYERCYCLES, 1, 50);
					break;
				case sizeof(LeaderboardEntry) :
					InsertMonika(lbe.uname, LBEUNAMECYCLES, 1, 50);
					InsertMonika(lbe.gamemode, LBEGMODECYCLES, 1, 20);
					InsertMonika(lbe.date, LBEDATECYCLES, 1, 10);
					break;
			}
		}
		fout.write(p, randline.sizeofstruct);
	}
	fin.close();
	fout.close();
	unlink(randline.path);
	rename(buffer2, randline.path);
}
void CorruptLinesTXT (LineEntry randline) {
	if (randline.index == 0) return;
	char* buffer1;
	char buffer[300], buffer2[300], buffer3[300];
	strcpy(buffer3, randline.path);
	buffer1 = strstr(buffer3, QBANK);
	if (buffer1 && randline.index % 6 == 5) return;
	/* Example PATH//TOPIC1.TXT =>  PATH//TOPIC1temp.TXT*/
	char* p = strrchr(buffer3, '\\');
	*(p+1) = 0;
	strcpy(buffer2, buffer3);
	strcat(buffer2, "temp");
	strcat(buffer2, TXT);
	ofstream fout (buffer2, ios::out);
	ifstream fin1 (randline.path, ios::in);
	fin1.seekg(0);
	fout.seekp(0);
	int count = 0;
	for(int i = 0; !fin1.eof(); i++) {
		fin1.getline(buffer, 300, '\n');
		count++;
	}
	fin1.close();
	ifstream fin (randline.path, ios::in);
	fin.seekg(0);
	for( i = 0; !fin.eof(); i++) {
		fin.getline(buffer, 300, '\n');
		if (i == randline.index) {
			fout << InsertMonika(buffer, TEXTCYCLES, 1, 1000);
			if(i < count - 1) fout << endl;
		}
		else {
			fout << buffer;
			if(i < count - 1) fout << endl;
		}
	}
	fin.close();
	fout.close();
	unlink(randline.path);
	rename(buffer2, randline.path);
}