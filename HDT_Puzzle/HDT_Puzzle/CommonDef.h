#pragma once

#define MAX_SIZE_MAP 15
#define MIN_SIZE_MAP 5
#define MAX_VALUE 99
#define START_NUM 1
#define VEC_PAIR vector<pair<int,int> >
#define INT_PAIR pair<int,int>

//남 남서 서 북서 북 북동 동 남동 아니죠
//북 북서 서 남서 남 남동 동 북동
static int dirRow[8] = { -1,-1,0,1,1,1,0,-1 };
static int dirCol[8] = { 0,-1,-1,-1,0,1,1,1 };