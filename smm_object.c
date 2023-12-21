//
//  smm_node.c
//  sMMarble
//  created by Ayoung Cho on 2023.12.21



#include "smm_common.h"
#include "smm_object.h"
#include <string.h>

#define MAX_NODETYPE    7
#define MAX_GRADE       9
#define MAX_NODE        100

static char smmNodeName[SMMNODE_TYPE_MAX][MAX_CHARNAME] = {
	"강의",
	"식당",
	"실험실",
	"집",
	"실험실로이동",
	"음식찬스",
	"축제시간"
};

typedef enum smmObjGrade {
	smmObjGrade_Ap = 0,
	smmObjGrade_A0,
	smmObjGrade_Am,
	smmObjGrade_Bp,
	smmObjGrade_B0,
	smmObjGrade_Bm,
	smmObjGrade_Cp,
	smmObjGrade_C0,
	smmObjGrade_Cm
} smmObjGrade_e;
//save grades with number 

static char smmGradeName [9][2]={
	"A+",
	"A0",
	"A-",
	"B+",
	"B0",
	"B-",
	"C+",
	"C0",
	"C-",
};
//성적 유형 정의 

/// 1.구조체 형식 정의
typedef struct smmObject{
	char name[MAX_CHARNAME];
	smmObjType_e objType;
	int type;
	int credit;
	int energy;
	smmObjGrade_e grade;
} smmObject_t;

//2. 구조체 배열 변수 정의

//static smmObject_t smm_node[MAX_NODE];
 
#if 0  
static char smmObj_name[MAX_NODE][MAX_CHARNAME];
static int smmObj_type[MAX_NODE];
static int smmObj_credit[MAX_NODE];
static int smmObj_energy[MAX_NODE];
static int smmObj_noNode=0;
#endif

char* smmObj_getTypeName (int type)
{
	return (char*)smmNodeName[type];
}
/// 유형 이름을 출력하는 함수
//static smmObject_t smm_node[MAX_NODE];
//static int smmObj_noNode = 0;

//3. 관련 함수 변경
void* smmObj_genObject(char* name, smmObjType_e objType, int type, int credit, int energy, smmObjGrade_e grade)
{
	smmObject_t* ptr;
	
	ptr = (smmObject_t*)malloc(sizeof(smmObject_t));
	
	strcpy (ptr->name, name);
    ptr->objType = objType;
    ptr->type = type;
    ptr->credit = credit;
    ptr->energy = energy;
    ptr->grade = grade;
    
    return ptr;
    //strcpy (smm_node[smmObj_noNode].name, name);
    //smm_node[smmObj_noNode].type = type;
    //smm_node[smmObj_noNode].credit = credit;
    //smm_node[smmObj_noNode].energy = energy;
    
    //smmObj_noNode++;
}


//3.관련 함수 변경
int smmObj_getNodeType(void* obj)
{
	smmObject_t* ptr = (smmObject_t*)obj;
	
	return ptr->type;
    //return smm_node[node_nr].type;
}

int smmObj_getNodeCredit(void* obj)
{
	smmObject_t* ptr = (smmObject_t*)obj;
	
	return ptr->credit;
	//return smm_node[node_nr].credit;
}

int smmObj_getNodeEnergy(void* obj)
{
	smmObject_t* ptr = (smmObject_t*)obj;
	
	return ptr->energy;
	//return smm_node[node_nr].energy;
}



char* smmObj_getNodeName(void* obj)
{
	
	smmObject_t* ptr = (smmObject_t*)obj;
	
	return ptr->name;
    //return smmNodeName[type];
}
//recall the node name

int smmObj_getNodeGrade(void* obj){
	smmObject_t* ptr = (smmObject_t*)obj;
	return ptr->grade;	
}
//recall the grade

char* smmObj_getGradeName(int grade)
{
//printf("Grade Name Check %d!!!\n", grade);
//printf("%s\n",smmGradeName[grade]);
    return smmGradeName[grade];
}
//recall the grade 
