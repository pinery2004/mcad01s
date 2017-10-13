//==========================================================================================
//  Copyright (C) 2006-2008. K.Matsu. All rights reserved.
//
//  MODULE: Mgs.cpp
//
//		処理
//
//
//  K.Matsu			10/07/07	Created.
//==========================================================================================
//
#include "stdafx.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
int tmpFlag1 = _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
//NG int tmpFlag2 = _CrtSetBreakAlloc(2);


int main1();
int main2();
int main3();
int main4();
int main5();
int main6();
int main7();
int main8();
int main9();

int _tmain(int argc, _TCHAR* argv[])
{
	int ii = 1;
	int i1;

	do {

		printf( " ====================================================\n");
		printf( " Solid Test\n");
		printf( "\n");
		printf( " 1: test-block\n");
		printf( " 2: test-cuben\n");
		printf( " 3: test-cube-with-hole\n");
		printf( " 4: test-cube-with-hole-2\n");
		printf( " 5: test-sphere\n");
		printf( " 6: test-torus\n");
		printf( " 7: test-disk\n");
		printf( " 8: test-cylinder\n");
		printf( " 9: test-cylinder-2\n");
		printf( "10: all_test\n");
		printf( " ====================================================\n");
		printf( "\n\n テスト番号 ( 1 - 9 ) 終了 ( 0 ) を入力してください   > ");
		scanf_s( "%d", &i1);

		switch ( i1 ) 
		{
		case 1:
			ii = main1();
			break;	

		case 2:
			ii = main2();
			break;
	
		case 3:
			ii = main3();
			break;
	
		case 4:
			ii = main4();
			break;
	
		case 5:
			ii = main5();
			break;
	
		case 6:
			ii = main6();
			break;
	
		case 7:
			ii = main7();
			break;
	
		case 8:
			ii = main8();
			break;
	
		case 9:
			ii = main9();
			break;

		case 10:
			ii = main1();
			ii = main2();
			ii = main3();
			ii = main4();
			ii = main5();
			ii = main6();
			ii = main7();
			ii = main8();
			ii = main9();
			break;	
		}

	} while( 10 > i1 && i1 > 0 && ii == 0);

//	_CrtDumpMemoryLeaks();
	return ii;
}

