//
//  main.c
//  SMmarble
//  
// Created by Ayoung Cho on 2023/12/21.
//

#include <time.h>
#include <string.h>
#include "smm_object.h"
#include "smm_database.h"
#include "smm_common.h"



#define BOARDFILEPATH "marbleBoardConfig.txt"
#define FOODFILEPATH "marbleFoodConfig.txt"
#define FESTFILEPATH "marbleFestivalConfig.txt"

#define MAX_NODE		100
//board configuration parameters
static int board_nr;
static int food_nr;
static int festival_nr;
static int player_nr;

typedef struct player {
	int energy;
	int position; //player's position
	char name[MAX_CHARNAME];
	int accumCredit; //누적학점
	int flag_graduate; //졸업여부 확인하기 위해서 : 1 - graduated, 0 - yet
	int inLab; //실험중인지 
} player_t;

static player_t *cur_player;
//static player_t cur_player[MAX_PLAYER];

#if 0
static int player_energy[MAX_PLAYER];
static int player_position[MAX_PLAYER];
static char player_name[MAX_PLAYER][MAX_CHARNAME];
#endif


//function prototypes
#if 0
int isGraduated(void); //check if any player is graduated
 //print grade history of the player
void goForward(int player, int step); //make player go "step" steps on the board (check if player is graduated)
void printPlayerStatus(void); //print all player status at the beginning of each turn
float calcAverageGrade(int player); //calculate average grade of the player
smmGrade_e takeLecture(int player, char *lectureName, int credit); //take the lecture (insert a grade of the player)
void* findGrade(int player, char *lectureName); //find the grade from the player's grade history
void printGrades(int player); //print all the grade history of the player
#endif



int isGraduated(int player)
{
	if (cur_player[player].position == 0 && cur_player[player].accumCredit >= GRADUATE_CREDIT)
		{
			cur_player[player].flag_graduate = 1;
		}
	else
		cur_player[player].flag_graduate = 0;
		
	return cur_player[player].flag_graduate;
	//if any player is at Home Node and the player's accumCredit > GRADUATE_CREDIT, that player is defined as Graduated 
} ////check if any player is graduated




void goForward(int player, int step)
{
	void *boardPtr;
	
	if((cur_player[player].position + step) >= smmdb_len(LISTNO_NODE))
	{
		cur_player[player].position -= smmdb_len(LISTNO_NODE);
	}
	
	cur_player[player].position += step;
	boardPtr = smmdb_getData(LISTNO_NODE, cur_player[player].position);
	
	printf("\n%s는 %i로 이동한다. (노드 이름: %s)\n",
		cur_player[player].name, cur_player[player].position,
		smmObj_getNodeName(boardPtr));
		
		
	
} //make player go "step" steps on the board


void printGrades(int player)
{
	int i;
	void *gradePtr;
//	void *lectureGradePtr;
//printf("PRINT GRADES of player %d\n", player);
	
    for (i=0;i<smmdb_len(LISTNO_OFFSET_GRADE + player);i++)
	{
		gradePtr = smmdb_getData(LISTNO_OFFSET_GRADE + player, i);
		
		// smmObj_getGradeName 함수에서 반환된 문자열에서 
        char *gradeName = smmObj_getGradeName(smmObj_getNodeGrade(gradePtr));
        char firstTwoChars[3];  
        strncpy(firstTwoChars, gradeName, 2);
        firstTwoChars[2] = '\0'; // NULL 문자 추가
		printf("\n%s 의 학점: %d , 성적 : %s\n", smmObj_getNodeName(gradePtr), smmObj_getNodeCredit(gradePtr), firstTwoChars);
     
	//	printf("%s: %s\n", smmObj_getNodeName(gradePtr), smmObj_getGradeName(smmObj_getNodeGrade(gradePtr)));
	}
/*	lectureGradePtr = smmdb_getData(LISTNO_OFFSET_GRADE + player, rand()%9);
	printf("%s: %s\n", smmObj_getNodeName(lectureGradePtr), smmObj_getGradeName(smmObj_getNodeGrade(lectureGradePtr)));
*/
}
//플레이어가 수강한 강의목록과 성적 출력

void printPlayerStatus()
{
	int i;
	for (i=0; i<player_nr; i++)
	{
		printf("\n%s: 에너지는 %i, 누적된 학점은 %i, 위치는 %i, 실험 상태(0이면 실험 아님, 1이면 실험중) %i",
		cur_player[i].name,
		cur_player[i].energy,
        cur_player[i].accumCredit,
		cur_player[i].position,
		cur_player[i].inLab );
	}
}
 //print all player status at the beginning of each turn: name, accumulated credit, energy, now position, whether or not this player is in Lab time
 



int checkLecture(int list_nr, char *lectureName)
{
	int index = 0;
	int length = smmdb_len(list_nr);
	
//printf("lecture name is %s\n", lectureName);
	for (index = 0; index < length; index++)
	{
//printf("��: %s , %s\n\n", smmObj_getNodeName(smmdb_getData(list_nr, index)), lectureName);
		if(strcmp(smmObj_getNodeName(smmdb_getData(list_nr, index)), lectureName) == 0)
        {
		//	 printf("Index is %d\n", index);
			 return index;
		}
	}
	
	return -1;
}
 //check if this player has taken this lecture before, by searching the lecture name on the list


void generatePlayers(int n, int initEnergy) //generate a new player
{
	int i;
	//n time loop
	for (i=0; i<n; i++)
	{
		//input name
		printf("\n 학생이름을 입력하라: "); //안내멘트 
		//scanf("%s", player_name[i][0]);
		scanf("%s", cur_player[i].name);
		fflush(stdin);
		//set position
		//player_position[i] = 0;
		cur_player[i].position = 0;
		
		//set energy
		//player_energy[i] = initEnergy; 
		cur_player[i].energy = initEnergy;
		cur_player[i].accumCredit = 0;
		cur_player[i].flag_graduate = 0;
	}
}

	


int rolldice(int player)
{
    char c;
    int dice_no;
    printf("\n\n%d 번째 플레이어의 순서입니다. 아무 키나 눌러 주사위를 굴리세요. (성적확인을 원한다면 g를 입력하세요): ", player+1);
    c = getchar();
    fflush(stdin);
    

    if (c == 'g')
        printGrades(player);

    dice_no = (rand()%MAX_DICE + 1);
	printf("\n 주사위 값은 %d 입니다.\n", dice_no);
    return dice_no;
}
//roll the dice and if you press 'g': show the grades by 'printGrades(player)' function


//action code when a player stays at a node
void actionNode(int player)
{
	void *boardPtr = smmdb_getData(LISTNO_NODE, cur_player[player].position);
	//int type = smmObj_getNodeType(cur_player[player].position); 
	int type = smmObj_getNodeType(boardPtr);
	char *name = smmObj_getNodeName(boardPtr);
	void *gradePtr;
	int labPosition;
	int randomFoodCard;
	char *CardName;
	int randomFestCard;
	int answer;
	
	
	

    switch(type)
    {

//printf("type is %d \n\n", type);

        //case lecture:
        case SMMNODE_TYPE_LECTURE:

        		if(cur_player[player].energy < smmObj_getNodeEnergy(boardPtr)){
					printf("\n 에너지가 부족합니다.\n");
					
        			break;
				}//if player doesn't have enough energy to take lecture: break
        		if(checkLecture(LISTNO_OFFSET_GRADE + player, smmObj_getNodeName(boardPtr)) > -1){
        			printf("\n 이 강의는 이미 수강하였습니다.\n");
					break;
				} //if player has taken this lecture before: break
				

				
				printf("\n 수강하시겠어요? Y: 1, N: press any key - "); //수강할건지 묻기
				scanf("%d", &answer);
				fflush(stdin);
				
                if (answer == 1)
				{
					cur_player[player].accumCredit += smmObj_getNodeCredit(boardPtr);
        			cur_player[player].energy -= smmObj_getNodeEnergy(boardPtr);
        			//grade generation 
       				gradePtr = smmObj_genObject(smmObj_getNodeName(boardPtr), smmObjType_grade, 0, smmObj_getNodeCredit(boardPtr), cur_player[player].position, rand()%9);
       				smmdb_addTail(LISTNO_OFFSET_GRADE + player, gradePtr);
       				printGrades(player); //플레이어 성적표 출력!
       				break;
				}
				else
					printf("수강 하지 않기 \n");
        		//check if player wants to take this lecture -> 'y': credit+, energy-, give grade
        	break;
        	
        case SMMNODE_TYPE_RESTAURANT:
        	cur_player[player].energy += smmObj_getNodeEnergy(boardPtr);
        	printf("\n%d 만큼 에너지를 얻었습니다. \n\n\n", smmObj_getNodeEnergy(boardPtr));
        	break;
         
        	
        case SMMNODE_TYPE_LABORATORY:
        	break;
       
        
        case SMMNODE_TYPE_HOME:
        	cur_player[player].energy += smmObj_getNodeEnergy(boardPtr);
        	//If player is at Home Node; Player gets energy
        	break;
        
        case SMMNODE_TYPE_GOTOLAB:
        	labPosition = checkLecture(LISTNO_NODE,"실험실");
        	cur_player[player].position = labPosition; // player goes to Lab Node
        	cur_player[player].inLab = 1; //player is in the lab time
        	break;
        	
        case SMMNODE_TYPE_FOODCHANCE:
			randomFoodCard = rand()%smmdb_len(LISTNO_FOODCARD);
      		CardName = smmObj_getNodeName(smmdb_getData(LISTNO_FOODCARD, randomFoodCard));
	    	cur_player[player].energy += smmObj_getNodeEnergy(smmdb_getData(LISTNO_FOODCARD, randomFoodCard));
        	
        	printf("\n %s이 선택되었습니다. %d 만큼 에너지를 얻습니다.\n\n", CardName, smmObj_getNodeEnergy(smmdb_getData(LISTNO_FOODCARD, randomFoodCard)));
        	break;
        	
        case SMMNODE_TYPE_FESTIVAL:
        	randomFestCard = rand()%smmdb_len(LISTNO_FESTCARD);
        	printf("\n%s\n\n", smmObj_getNodeName(smmdb_getData(LISTNO_FESTCARD, randomFestCard)));
        	break;
        	
        default:
            break;
    }
}



int main(int argc, const char * argv[])
{
    
    FILE* fp;
    char name[MAX_CHARNAME];
    int type;
    int credit;
    int energy;
    int i;
    int initEnergy;
    int turn = 0;
    
    
    board_nr = 0;
    food_nr = 0;
    festival_nr = 0;
    
    srand(time(NULL));
    
    //1. import parameters ---------------------------------------------------------------------------------
    //1-1. boardConfig 
    if ((fp = fopen(BOARDFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", BOARDFILEPATH);
        getchar();
        return -1;
    }
    
    printf("Reading board component......\n\n");
    while ( fscanf(fp, "%s %i %i %i", name, &type, &credit, &energy) == 4 ) //read a node parameter set
    {
        //store the parameter set
        //(char* name, smmObjType_e objType, int type, int credit, int energy, smmObjGrade_e grade)
        void *boardObj = smmObj_genObject(name, smmObjType_board, type, credit, energy, 0);
        smmdb_addTail(LISTNO_NODE, boardObj);
		  
        if (type == SMMNODE_TYPE_HOME)
        	initEnergy = energy;
		
		board_nr++;
    }
    fclose(fp);
    printf("Total number of board nodes : %i\n\n", board_nr);
    
    for (i = 0;i<board_nr;i++)
    {
    	void *boardObj = smmdb_getData(LISTNO_NODE, i);
    	
        printf("node %i : %s, %i(%s), credit %i, energy %i\n\n",
			i, smmObj_getNodeName(boardObj),
			smmObj_getNodeType(boardObj), smmObj_getTypeName(smmObj_getNodeType(boardObj)),
			smmObj_getNodeCredit(boardObj), smmObj_getNodeEnergy(boardObj));
		 
	}
    

    //2. food card config 
    if ((fp = fopen(FOODFILEPATH,"r")) == NULL)
    {
        printf("<<ERROR>> failed to open %s. This file should be in the same directory of SMMarble.exe.\n", FOODFILEPATH);
        return -1;
    }
    
    printf("..Reading food card component..\n\n");

    
    while ( fscanf(fp, "%s %i", name, &energy) == 2 ) //read a food parameter set
    {
        //store the parameter set
        void *foodObj = smmObj_genObject(name, smmObjType_card, 0, 0, energy, 0);
        smmdb_addTail(LISTNO_FOODCARD, foodObj);
		food_nr++;
    }
    fclose(fp);

    printf("Total number of food cards : %i\n\n", food_nr);
    
    
    
    //3. festival card config 
    if ((fp = fopen(FESTFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", FESTFILEPATH);
        return -1;
    }
    
    printf("\n\nReading festival card component......\n\n");


    while ( fscanf(fp, "%s", name) == 1 ) //read a festival card string
    {
        //store the parameter set
        void *festObj = smmObj_genObject(name, smmObjType_card, 0, 0, 0, 0);
        smmdb_addTail(LISTNO_FESTCARD, festObj);
		  
        
		festival_nr++;
    }
    fclose(fp);
    printf("Total number of festival cards : %i\n\n", festival_nr);
    
    

    
    //2. Player configuration ---------------------------------------------------------------------------------
    do
    {
        //input player number to player_nr
        printf(" 플레이어의 수를 입력해주세요 :\n ");
  		scanf("%d", &player_nr);
  		fflush(stdin);
    }
    while (player_nr<0 || player_nr > MAX_PLAYER);

    cur_player = (player_t*)malloc(player_nr*sizeof(player_t));
    generatePlayers(player_nr, initEnergy);
    

    


    //3. SM Marble game starts ---------------------------------------------------------------------------------
    while (1) //is anybody graduated?
    {
        int dice_result;
        int outLabNo;

        
        //4-1. initial printing

        printPlayerStatus();

        if (cur_player[turn].inLab > 0){
        	outLabNo = rand()%MAX_DICE+1;
        	cur_player[turn].energy -= smmObj_getNodeEnergy(smmdb_getData(LISTNO_NODE, SMMNODE_TYPE_LABORATORY));
        	printf("\n%s 는 아직 실험을 하고 있습니다. \n", cur_player[turn].name);
	        printf("\n Lab exodus no is. %d !\n", outLabNo);
	        if (rolldice(turn) >= outLabNo){
	        	cur_player[turn].inLab = 0;
	        	printf("실험이 끝났습니다. \n\n");
			}
			else
	        	printf("실험이 아직 남았습니다. \n\n");
		}
		else
		{ //Normal Process

	        //4-2. die rolling (if not in experiment)
	        dice_result = rolldice(turn);
	        
	        //4-3. go forward
	        goForward(turn, dice_result);//node 순서 확인, 주사위값만큼 전진 
	        
	        //졸업 학점 확인하기
			if(isGraduated(turn) == 1)
				break;
			//isGraduated() == 1이면 게임 끝
			//isGraduated() == 0이면 다시 actionNode
			
			//4-4. take action at the destination node of the board
	        actionNode(turn);
	        ////home node에서는 energy얻음
			////lecture node에서는 energy 잃고 credit 얻음; energy 부족하면 수강못함; 수강 성공시 grade print
			

		}
        
        //4-5. next turn
        
        turn ++;
        if (turn == player_nr) turn = 0;
        
    }
    
    
    //게임이 끝나면 안내하는 멘트 
    printf("\n 게임이 종료되었습니다 \n");
    
    //player별 성적표
    for (i=0;i<player_nr;i++)
	{
		printf("player %d의 성적표 \n\n", i+1);
    	printGrades(i);
    	printf("\n\n");
	}
    
    free(cur_player);

    system("PAUSE");
    return 0;
	
}