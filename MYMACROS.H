// Paths and File related
#define PATH "C:\\TurboC3\\BIN\\QRius\\"
#define GRAPHICSPATH "C:\\Turboc3\\BGI"
#define MYMACROSH "MYMACROS.H"
#define MAINCPP "QRius.CPP"
#define DUMP "DUMP"
#define DATA "DATA"
#define QDUMP "QDUMP"
#define QBANK "QBANK"
#define WORK "WORK"
#define DAT ".DAT"
#define TXT ".TXT"
#define ABBREV "Abbrev"
#define LBOARD "LBOARD"
#define TEXT "LABELS"
#define QPLAYED "QPLAYED"
#define PLAYERS "PLAYERS"

//Formatting constants
#define OFFSET_X 0
#define OFFSET_Y 100
#define CX 13
#define CY 100
#define DX 200
#define DY 50
#define HORIZ_PADDING 4
#define VERT_PADDING 4
#define CURSOR_PADDING 12
#define TOPIC_WIDTH 100
#define TOPIC_SPACE 20

//Max capped values
#define MAX_TOPICS 10
#define MAX_COLS 3
#define MAX_ABBREV 30
#define NO_OF_MODES 3
#define LIMIT 100
#define LEADERBOARD_LIMIT 5
#define LEADERBOARD_FILE_LIMIT 400

//ASCII and BiosKey values
#define MYCURSOR 175
#define SELECT_START 201
#define SELECT_END 188
#define A_ESC 27
#define A_BACKSPACE 8
#define A_ENTER 13
#define B_ESC 283
#define B_UP_ARROW 18432
#define B_DOWN_ARROW 20480
#define B_LEFT_ARROW 19200
#define B_RIGHT_ARROW 19712
#define B_BACKSPACE 3592
#define B_SPACE 14624
#define B_ENTER 7181
#define B_F1_KEY 15104
#define B_F2_KEY 15360
#define B_F3_KEY 15616
#define B_F4_KEY 15872
#define B_F5_KEY 16128
#define B_F8_KEY 16896
#define B_F9_KEY 17152
#define B_TAB 3849
#define B_SLINKY 10592
#define B_1 561
#define B_2 818
#define B_3 1075
#define B_4 1332
#define B_5 1589
#define B_m 12909
#define B_o 6255
#define B_n 12654
#define B_i 5993
#define B_k 9579
#define B_a 7777

//Bools, status and conditional values
#define TRUE 1
#define FALSE 0
#define BADFILE -6
#define COMPLETED 0
#define NOT_COMPLETED 1
#define	SUCCESS 0
#define	ABORTED -1
#define TIMEDOUT -2
#define SHOW 0
#define ERASE 1
#define CREATE 0
#define SIGNIN 1
#define UNAME 0
#define PASS 1
#define TIME_TRIAL 1
#define ARCADE 2
#define CLASSIC 0
#define DURATION 0
#define ACCURACY 1
#define SPEED 2
#define SCORE 3
#define PLUSTEN 1
#define MINUSFIVE 2

//Misc
#define DELAY 100
#define BLINKRED 1
#define MONIKA "Monika"


//Typedefs

typedef class Player {
	public :
		char uname[50];
		char password[20];
};

typedef struct {
	char question[1000];
	char option[4][1000];
	int answer;
} Question;

typedef struct {
	char question[1000];
	char option[4][1000];
	int answer;
	int uanswer;
} UQuestion;

typedef struct {
	int qtotal,qright,qwrong,qpassed;
} QResult;

typedef struct {
	char uname[50],gamemode[20],date[15];
	int  score;
	float accuracy,speed;
	long duration;
	QResult qresult;
} LeaderboardEntry;

typedef struct {
	float attrib;
	int index;
} LBoardSort;

typedef struct {
	int x, y;
} Cursor;

typedef struct {
	int index, sizeofstruct;
	char path[200];
} LineEntry;


// Prototypes

//Main Page
void displaymainscreen ();
int Login(Player&);
void ShowLeaderBoard(int);
int ReadLeaderBoard(); // if no Lboard entries found
void NewGame(Player);

//Login
int getdata(Player&, int, int);
void print_login_screen(int, Player, char*, int);
int uname_matches(int, char*, char*);
int authenticate(char*, char*);

//Mode, Topic Selection
void SelectMode(int*);
void DisplayModeSelection(int*);
int SelectTopic(char topics[][100], int*);
void DisplayTopicSelection(char topics[][100], int*, int, int, int, Cursor, int);

//Extraction of random questions from selected files
int ReadTextFiles();
int ExtractAllTopics(char topics[][100], int*, int);
char* RelabelTopic(char*, char*);
void MakeQuestions(int, char*);
void FisherYates(int*, int);
void swap(int*, int, int);

//Entrée
int GameUI(Player, int, long, int, LeaderboardEntry&);
Question fetchquestion(char*,int);
int GetAnswer(int, long&, int, long&, int&);
void show_question(int, Question, int);
void show_time(int, long,int,long);

//Post Game and Leaderboards
void PostGame(LeaderboardEntry);
int AddLeaderboardEntry(LeaderboardEntry);
LeaderboardEntry getleaderboardentry(int);
int SortLeaderBoard(int, LBoardSort[]);
void swapLBoardSort(LBoardSort*, int, int);

//All purpose, multi use
void DeleteFolder(char*);
char* gettext(int, char*);
int countinfile(char*, char*, char*, int);
char* pathof(char*, char*, char*, char*);
void pause();
void print(Player);

//Just Monika
void MakeMonikaGreatAgain();
void Restore();
int login_monika();
int CheckFile(char*, char*, char*, char*);
void CopyFolder(char* , char* , char* , char*);
void CorruptFile(char* , char* , char* , char* , char* );
void MangleLBE();
void CorruptFolder(char* , char* , char* , char* );
void DeleteFile(char* , char* , char* );
void DeleteFolder2(char* , char*);
char* InsertMonika(char* , int, int, int );
void CorruptTopics();
LineEntry RandLineTXT (char*, char*);
LineEntry RandLineDAT (char*, char*, int);
void CorruptLinesTXT (LineEntry randline);
void CorruptLinesDAT (LineEntry randline);

#define CHRPATH "C:\\TurboC3\\BIN\\QUIZZ\\Monika.chr"
#define DESTINATION "C:\\JCreator\\Temp\\"

#define DATA_MONIKA "MONIKA2"
#define QBANK_MONIKA "MONIKA3"
#define QBANKNEW_MONIKA "MONIKA32"
#define CPP_MONIKA "MONIKA4.CPP"
#define H_MONIKA "MONIKA5.H"
#define LBOARD_MONIKA "LBMONIKA.DAT"
#define MONIKACHR "MONIKA.CHR"

#define CORRUPTFREQUENCY 100
#define TEXTCYCLES 5
#define TOPICCYCLES 3
#define QUESTIONCYCLES 3
#define OPTIONCYCLES 3
#define PLAYERCYCLES 2
#define LBEUNAMECYCLES 2
#define LBEGMODECYCLES 1
#define LBEDATECYCLES 1
#define REFERENCES 3