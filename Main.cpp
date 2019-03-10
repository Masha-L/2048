#include <windows.h>
#include <malloc.h>
#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <time.h>
#include "scores.h"

HINSTANCE g_hInstance;

#define ID_B11  3
#define ID_B12	5	
#define ID_B13	7
#define ID_B14	9
#define ID_B21	11
#define ID_B22	13
#define ID_B23	15
#define ID_B24	17
#define ID_B31	19
#define ID_B32	21
#define ID_B33	23
#define ID_B34	25
#define ID_B41	27
#define ID_B42	29
#define ID_B43	31
#define ID_B44	33

#define ID_NEW_GAME	41
#define ID_EXIT	43
#define ID_BACK	45
#define ID_LAST_RECORDS	47
#define ID_RULES 49
#define ID_ABOUT 51
#define ID_RECORD 53
#define ID_RESULT1 11111111
#define ID_RESULT2 11111112
#define ID_RESULT3 11111113
#define ID_RESULT4 11111114
#define ID_RESULT5 11111115

#define ID_LEFT 37
#define ID_RIGHT 39
#define ID_UP 38
#define ID_DOWN 40


#define ID_SCORE 69
#define ID_CURRENT_SCORE 61

int NumbersMove(HWND hWnd, int CellFirst,int CellSecond,int CellThird,int CellForth);

 /*void Result(int score, char * result)
{
	
	sprintf(result, "%d", score);
	
}*/
int GetCell(HWND hWnd, int id_static)
{
	HWND hwndStatic = GetDlgItem(hWnd, id_static);
	
	int length = SendMessage(hwndStatic, WM_GETTEXTLENGTH, 0, 0);
	if (0 == length)
		return 0;
	char* text = (char*)malloc(length + 1);
	text[length] = 0;
	int number = 0;
	
	SendMessage(hwndStatic, WM_GETTEXT,length + 1, (LPARAM)text);
	
	number=atoi(text);
	free(text);
	text=0;
	return number;
}
int FullCellsCount(HWND hWnd)
{ 
	int sum = 0;
	for(int i=3; i <35; i=i+2)
	{
		sum=sum+(0!=GetCell(hWnd,i));
	}
	return sum;
}

void SetCell(HWND hWnd, int id_static,int value)
{
	HWND hwndButton=GetDlgItem(hWnd, id_static);
	
	char cell[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	if(0!=value)
		sprintf(cell, "%d", value);
	SendMessage(hwndButton, WM_SETTEXT, 0, (LPARAM)cell);
	 
}

void SetStatic(HWND hWnd, int id_static, char * value)
{
	HWND hwndButton=GetDlgItem(hWnd, id_static);

	SendMessage(hwndButton, WM_SETTEXT, 0, (LPARAM)value);
}

void SetScore(HWND hWnd)
{
	Scores score;
	score.Read();
	for(int i = 11111111; i<11111116 ; i++)
	{
		if(/*scores.player[i-57].name!=0 &&*/ score.player[i-11111111].score!=0)
		{
			char cell[25];
			//here is a mistake!
			sprintf(cell, "%d" , /*scores.player[i].name,*/score.player[i-11111111].score);
			SetStatic(hWnd, i, cell);
			
		}
	}
};

int RandomVar()
{
	if (0 == rand() % 10)
	{
		return 4;
	}
	else
	{
		return 2;
	}
}

HWND CreateCell(HWND hwndParent, char* wnd_caption, int x, int y, int ID)
{
	HWND hwndStatic = CreateWindow
		(
		"STATIC",
		wnd_caption,
		WS_CHILD | WS_VISIBLE|WS_BORDER|SS_CENTER,
		x,
		y,
		50,
		50,
		hwndParent,
		(HMENU)ID,
		g_hInstance,
		0
		);
	if(0 == hwndStatic)
		MessageBox(NULL, "Error: Window not created", "2048", 0);
	return hwndStatic;
}
HWND CreateScore(HWND hwndParent, char* wnd_caption, int x, int y, int ID)
{
	HWND hwndStatic = CreateWindow
		(
		"STATIC",
		wnd_caption,
		WS_CHILD | WS_VISIBLE|WS_BORDER,
		x,
		y,
		120,
		20,
		hwndParent,
		(HMENU)ID,
		g_hInstance,
		0
		);
	if(0 == hwndStatic)
		MessageBox(NULL, "Error: Window not created", "2048", 0);
	return hwndStatic;
}
HWND CreateScoreNumber(HWND hwndParent, char* wnd_caption, int x, int y, int ID)
{
	HWND hwndStatic = CreateWindow
		(
		"STATIC",
		wnd_caption,
		WS_CHILD | WS_VISIBLE|WS_BORDER,
		x,
		y,
		60,
		20,
		hwndParent,
		(HMENU)ID,
		g_hInstance,
		0
		);
	if(0 == hwndStatic)
		MessageBox(NULL, "Error: Window not created", "2048", 0);
	return hwndStatic;
}
HWND CreateStatic(HWND hwndParent, char* wnd_caption, int x, int y, int ID)
{
	HWND hwndStatic = CreateWindow
		(
		"STATIC",
		wnd_caption,
		WS_CHILD | WS_VISIBLE,
		x,
		y,
		85,
		30,
		hwndParent,
		(HMENU)ID,
		g_hInstance,
		0
		);
	if(0 == hwndStatic)
		MessageBox(NULL, "Error: Window not created", "2048", 0);
	return hwndStatic;
}

void OnGetMinMaxInfo(MINMAXINFO* pMinMaxInfo)
{
	pMinMaxInfo->ptMinTrackSize.x = 400;
	pMinMaxInfo->ptMinTrackSize.y = 340;
	pMinMaxInfo->ptMaxTrackSize.x = 400;
	pMinMaxInfo->ptMaxTrackSize.y = 340;
}
void CreateMainMenu(HWND hWnd)
{
	// Creating the menu and getting its handle.
	HMENU hMenu = CreateMenu();
	// Creating the first popup.
	HMENU hMenuPopup = CreatePopupMenu();
	// Creating the second popup.
	HMENU hRulesPopup = CreatePopupMenu();
	//Creating the third popup
	HMENU hAboutPopup = CreatePopupMenu();

	//Appending the menu items.
	AppendMenu(hMenuPopup, MF_STRING, ID_NEW_GAME,  "Новая игра");
	
	AppendMenu(hAboutPopup, MF_STRING, ID_ABOUT, "О Программе");
	AppendMenu(hMenuPopup, MF_STRING, ID_BACK,  "Отменить ход");
	AppendMenu(hRulesPopup, MF_STRING, ID_RULES,  "Правила игры");
	AppendMenu(hMenuPopup, MF_SEPARATOR, 0, 0);
	AppendMenu(hMenuPopup, MF_STRING, ID_EXIT, "Выход" );
//	Appending the popups to the menu.
	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hMenuPopup, "Файл");
	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hRulesPopup, "Правила игры");
	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hAboutPopup, "Справка");
	SetMenu(hWnd, hMenu);
}

int ChooseCell(HWND hWnd, int var)
{
	int cell=0;
	if(FullCellsCount(hWnd)<16)
	{
		cell = 3 + 2 * (rand() % 16);
		if(GetCell(hWnd,cell)==0)
			SetCell(hWnd, cell, var);
		else
			ChooseCell(hWnd, RandomVar());
	}
	return cell;
}
void StartNewGame(HWND hWnd)
{
	//free all
	for(int i=3; i<34; i=i+2)
		SetCell(hWnd, i, 0);
	SetCell(hWnd,ID_CURRENT_SCORE, 0);
	//first number
	ChooseCell(hWnd, 2);

	//second number
	ChooseCell(hWnd, RandomVar());


}


int MoveDirection(HWND hWnd, int direction)
{
	int ScoreChanges = 0;

	switch (direction)
	{
	case ID_LEFT:
		ScoreChanges += NumbersMove(hWnd, ID_B11, ID_B12,ID_B13,ID_B14);
		ScoreChanges += NumbersMove(hWnd, ID_B21, ID_B22,ID_B23,ID_B24);
		ScoreChanges += NumbersMove(hWnd, ID_B31, ID_B32,ID_B33,ID_B34);
		ScoreChanges += NumbersMove(hWnd, ID_B41, ID_B42,ID_B43,ID_B44);
		return ScoreChanges;
	
	case ID_RIGHT:
		
		ScoreChanges +=NumbersMove(hWnd, ID_B14, ID_B13,ID_B12,ID_B11);
		ScoreChanges +=NumbersMove(hWnd, ID_B24, ID_B23,ID_B22,ID_B21);
		ScoreChanges +=NumbersMove(hWnd, ID_B34, ID_B33,ID_B32,ID_B31);
		ScoreChanges +=NumbersMove(hWnd, ID_B44, ID_B43,ID_B42,ID_B41);
		return ScoreChanges;;
	case ID_DOWN:
		ScoreChanges +=NumbersMove(hWnd, ID_B41, ID_B31,ID_B21,ID_B11);
		ScoreChanges +=NumbersMove(hWnd, ID_B42, ID_B32,ID_B22,ID_B12);
		ScoreChanges +=NumbersMove(hWnd, ID_B43, ID_B33,ID_B23,ID_B13);
		ScoreChanges +=NumbersMove(hWnd, ID_B44, ID_B34,ID_B24,ID_B14);
		return ScoreChanges;;
	case ID_UP:
		ScoreChanges +=NumbersMove(hWnd, ID_B11, ID_B21,ID_B31,ID_B41);
		ScoreChanges +=NumbersMove(hWnd, ID_B12, ID_B22,ID_B32,ID_B42);
		ScoreChanges +=NumbersMove(hWnd, ID_B13, ID_B23,ID_B33,ID_B43);
		ScoreChanges +=NumbersMove(hWnd, ID_B14, ID_B24,ID_B34,ID_B44);
		return ScoreChanges;

	}

	return 0;
}
int Move2Cells(HWND hWnd, int CellFirst,int CellSecond, int SummCells)
{
	if(0==GetCell(hWnd, CellFirst) && 0!=GetCell(hWnd, CellSecond) && SummCells==0)
	{
		SetCell(hWnd, CellFirst,GetCell(hWnd, CellSecond));
		SetCell(hWnd, CellSecond, 0);
		return 0;
	}
	else
		if(0!= GetCell(hWnd, CellFirst) && GetCell(hWnd, CellFirst)==GetCell(hWnd, CellSecond) && SummCells==1)
		{
			SetCell(hWnd, CellFirst, GetCell(hWnd, CellFirst)+GetCell(hWnd, CellSecond));
			SetCell(hWnd, CellSecond, 0);
			return (GetCell(hWnd, CellFirst)+GetCell(hWnd, CellSecond));

		}
	return 0;

}

int NumbersMove(HWND hWnd, int CellFirst,int CellSecond,int CellThird,int CellForth)
{
	int ScoreChanges = 0;
	for(int i=0; i<9; i++)
	{
		
		if((i<3)||(i>5))
		{
			Move2Cells(hWnd,CellFirst, CellSecond, i==4);
			Move2Cells(hWnd, CellSecond,CellThird, i==4);
			Move2Cells(hWnd, CellThird, CellForth, i==4);
		}
		else
			if(i==3||i==4||i==5)
			{	
				if(ScoreChanges!=0)
				{
					ScoreChanges += Move2Cells(hWnd,CellFirst, CellSecond, i==2);
					ScoreChanges += Move2Cells(hWnd, CellSecond,CellThird, i==2);
					ScoreChanges += Move2Cells(hWnd, CellThird, CellForth, i==2);
				}
				else
				{
					ScoreChanges += Move2Cells(hWnd,CellFirst, CellSecond, i>2);
					ScoreChanges += Move2Cells(hWnd, CellSecond,CellThird, i>2);
					ScoreChanges += Move2Cells(hWnd, CellThird, CellForth, i>2);
				}
					
			}
	}
	return ScoreChanges;
}

void SavePosition(HWND hWnd, int Position[4][4])
{
	Position[0][0]=GetCell(hWnd, ID_B11);
	Position[0][1]=GetCell(hWnd, ID_B12);
	Position[0][2]=GetCell(hWnd, ID_B13);
	Position[0][3]=GetCell(hWnd, ID_B14);

	Position[1][0]=GetCell(hWnd, ID_B21);
	Position[1][1]=GetCell(hWnd, ID_B22);
	Position[1][2]=GetCell(hWnd, ID_B23);
	Position[1][3]=GetCell(hWnd, ID_B24);

	Position[2][0]=GetCell(hWnd, ID_B31);
	Position[2][1]=GetCell(hWnd, ID_B32);
	Position[2][2]=GetCell(hWnd, ID_B33);
	Position[2][3]=GetCell(hWnd, ID_B34);

	Position[3][0]=GetCell(hWnd, ID_B41);
	Position[3][1]=GetCell(hWnd, ID_B42);
	Position[3][2]=GetCell(hWnd, ID_B43);
	Position[3][3]=GetCell(hWnd, ID_B44);
}
void LoadPosition(HWND hWnd, int Position[4][4])
{
	SetCell(hWnd, ID_B11, Position[0][0]);
	SetCell(hWnd, ID_B12, Position[0][1]);
	SetCell(hWnd, ID_B13, Position[0][2]);
	SetCell(hWnd, ID_B14, Position[0][3]);

	SetCell(hWnd, ID_B21, Position[1][0]);
	SetCell(hWnd, ID_B22, Position[1][1]);
	SetCell(hWnd, ID_B23, Position[1][2]);
	SetCell(hWnd, ID_B24, Position[1][3]);

	SetCell(hWnd, ID_B31, Position[2][0]);
	SetCell(hWnd, ID_B32, Position[2][1]);
	SetCell(hWnd, ID_B33, Position[2][2]);
	SetCell(hWnd, ID_B34, Position[2][3]);

	SetCell(hWnd, ID_B41, Position[3][0]);
	SetCell(hWnd, ID_B42, Position[3][1]);
	SetCell(hWnd, ID_B43, Position[3][2]);
	SetCell(hWnd, ID_B44, Position[3][3]);
}
int ArePositionsSame(int PreviousPosition[4][4], int CurrentPosition[4][4])
{
	for(int i=0; i< 4; i++)
		for (int j = 0; j<4; j++)
			if (PreviousPosition[i][j] != CurrentPosition[i][j])
				return 0;
	return 1;
}

void Undo(HWND hWnd,  int PreviousPosition[4][4], int CurrentPosition[4][4])
{
	SetCell(hWnd,ID_B11,PreviousPosition[0][0]);
	SetCell(hWnd,ID_B12,PreviousPosition[0][1]);
	SetCell(hWnd,ID_B13,PreviousPosition[0][2]);
	SetCell(hWnd,ID_B14,PreviousPosition[0][3]);

	SetCell(hWnd,ID_B21,PreviousPosition[1][0]);
	SetCell(hWnd,ID_B22,PreviousPosition[1][1]);
	SetCell(hWnd,ID_B23,PreviousPosition[1][2]);
	SetCell(hWnd,ID_B24,PreviousPosition[1][3]);

	SetCell(hWnd,ID_B31,PreviousPosition[2][0]);
	SetCell(hWnd,ID_B32,PreviousPosition[2][1]);
	SetCell(hWnd,ID_B33,PreviousPosition[2][2]);
	SetCell(hWnd,ID_B34,PreviousPosition[2][3]);

	SetCell(hWnd,ID_B41,PreviousPosition[3][0]);
	SetCell(hWnd,ID_B42,PreviousPosition[3][1]);
	SetCell(hWnd,ID_B43,PreviousPosition[3][2]);
	SetCell(hWnd,ID_B44,PreviousPosition[3][3]);
}
//проверка - есть ли возможность хода
int Move2CellsVoid(HWND hWnd, int CellFirst, int CellSecond)
{
	if(GetCell(hWnd,CellFirst)==GetCell(hWnd,CellSecond))
		return 1;
	else
		return 0;
}
int NumbersMoveVoid(HWND hWnd, int CellFirst,int CellSecond,int CellThird,int CellForth)
{
	int Summ = 0;
	if((Move2CellsVoid(hWnd,CellFirst, CellSecond)+
		Move2CellsVoid(hWnd, CellSecond,CellThird)+
		Move2CellsVoid(hWnd, CellThird, CellForth))>0)
		Summ=Summ+1;	
	if(Summ==0)
		return 0;
	else
		return 1;
}
int OnVoidMove(HWND hWnd)
{
	if(FullCellsCount(hWnd)==16)
	{
		if((NumbersMoveVoid(hWnd, ID_B11, ID_B12,ID_B13,ID_B14)+
			NumbersMoveVoid(hWnd, ID_B21, ID_B22,ID_B23,ID_B24)+
			NumbersMoveVoid(hWnd, ID_B31, ID_B32,ID_B33,ID_B34)+
			NumbersMoveVoid(hWnd, ID_B41, ID_B42,ID_B43,ID_B44)+
			NumbersMoveVoid(hWnd, ID_B14, ID_B13,ID_B12,ID_B11)+
			NumbersMoveVoid(hWnd, ID_B24, ID_B23,ID_B22,ID_B21)+
			NumbersMoveVoid(hWnd, ID_B34, ID_B33,ID_B32,ID_B31)+
			NumbersMoveVoid(hWnd, ID_B44, ID_B43,ID_B42,ID_B41)+
			NumbersMoveVoid(hWnd, ID_B41, ID_B31,ID_B21,ID_B11)+
			NumbersMoveVoid(hWnd, ID_B42, ID_B32,ID_B22,ID_B12)+
			NumbersMoveVoid(hWnd, ID_B43, ID_B33,ID_B23,ID_B13)+
			NumbersMoveVoid(hWnd, ID_B44, ID_B34,ID_B24,ID_B14)+
			NumbersMoveVoid(hWnd, ID_B11, ID_B21,ID_B31,ID_B41)+
			NumbersMoveVoid(hWnd, ID_B12, ID_B22,ID_B32,ID_B42)+
			NumbersMoveVoid(hWnd, ID_B13, ID_B23,ID_B33,ID_B43)+
			NumbersMoveVoid(hWnd, ID_B14, ID_B24,ID_B34,ID_B44))==0)
		{
			MessageBox(hWnd,"Игры закончена. Начните новую игру", "2048", 0);
			return 0;
		}

	}
	return 1;
}

//обновление текущего счета	
int UpdateScore(HWND hWnd, int ScoreChanges)
{
	int	FinalScore=GetCell(hWnd, ID_CURRENT_SCORE)+ScoreChanges;
	SetCell(hWnd, ID_CURRENT_SCORE, FinalScore);
	return FinalScore;
};
void SaveResults(HWND hWnd, int result)
{
	Scores Game;
	Game.Add(result);
	Game.Sorting();
	Game.Write();
};
void GetRulesFileName(char FileName[MAX_PATH])
{
	HMODULE hModule = GetModuleHandle(0);
	GetModuleFileName(hModule, FileName, MAX_PATH);
	char *LastBackslash = strrchr(FileName, 92);
	LastBackslash[1] = 0;
	strcat(FileName,"rules.htm");
};

void OnKeyDown(HWND hWnd, UINT uiButton, static int PreviousPosition[4][4], static int CurrentPosition[4][4])
{
	Scores Game;
	SavePosition(hWnd, PreviousPosition);
			if(0==OnVoidMove(hWnd))
				SaveResults(hWnd,UpdateScore(hWnd, MoveDirection(hWnd,uiButton)));
			else
				UpdateScore(hWnd, MoveDirection(hWnd,uiButton));
			SavePosition(hWnd, CurrentPosition);
			if (!ArePositionsSame(PreviousPosition, CurrentPosition))
				ChooseCell(hWnd, RandomVar());
};


LRESULT CALLBACK WindowFunction(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	static int PreviousPosition[4][4];
	static int CurrentPosition[4][4];

	char FileName[MAX_PATH];

	switch (msg)
	{
		//why not just case Lw? What do we need WM_KEY etc for?
	case WM_KEYDOWN:
		OnKeyDown(hWnd, LOWORD(wParam), PreviousPosition, CurrentPosition);
		//cannot convert parametr 4 from int to int[][4]
		
		
		/*switch(LOWORD(wParam))
		{
			//Just do Left(), Up() etc
		case ID_LEFT:
			SavePosition(hWnd, PreviousPosition);
			if(0==OnVoidMove(hWnd))
				Game.Write();
			UpdateScore(hWnd, MoveDirection(hWnd,ID_LEFT));
			SavePosition(hWnd, CurrentPosition);
			if (!ArePositionsSame(PreviousPosition, CurrentPosition))
				ChooseCell(hWnd, RandomVar());


			return 0;

		case ID_UP:
			SavePosition(hWnd, PreviousPosition);
			if(0==OnVoidMove(hWnd))
				Game.Write();
			UpdateScore(hWnd, MoveDirection(hWnd,ID_UP));
			SavePosition(hWnd, CurrentPosition);
			if (!ArePositionsSame(PreviousPosition, CurrentPosition))
				ChooseCell(hWnd, RandomVar());

			return 0;

		case ID_RIGHT:
			SavePosition(hWnd, PreviousPosition);
			if(0==OnVoidMove(hWnd))
				Game.Write();
			UpdateScore(hWnd, MoveDirection(hWnd,ID_RIGHT));
			SavePosition(hWnd, CurrentPosition);
			if (!ArePositionsSame(PreviousPosition, CurrentPosition))
				ChooseCell(hWnd, RandomVar());

			return 0;

		case ID_DOWN:
			SavePosition(hWnd, PreviousPosition);
			if(0==OnVoidMove(hWnd))
				Game.Write();
			UpdateScore(hWnd, MoveDirection(hWnd,ID_DOWN));
			SavePosition(hWnd, CurrentPosition);
			if (!ArePositionsSame(PreviousPosition, CurrentPosition))
				ChooseCell(hWnd, RandomVar());

			return 0;
		}*/
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_ABOUT:
			MessageBox(NULL," Made by Masha Lifshits in 2015",  "2048",0);
			return 0;
		case ID_EXIT:
			SendMessage(hWnd, WM_CLOSE, 0, 0);
			return 0;
		case ID_NEW_GAME:
			{
				StartNewGame(hWnd);
				SavePosition(hWnd, PreviousPosition);
			}
			return 0;
		case ID_BACK:
			Undo(hWnd,PreviousPosition, CurrentPosition);
			return 0;
		case ID_RULES:			
			GetRulesFileName(FileName);
			ShellExecute(0,0,FileName,0,0,SW_SHOW);
			return 0;

		}
		return 0;
	case WM_CREATE:

		CreateCell(hWnd,NULL,  50, 50,  ID_B11);
		CreateCell(hWnd,NULL,  100, 50,  ID_B12);
		CreateCell(hWnd,NULL,  150, 50,  ID_B13);
		CreateCell(hWnd,NULL,  200, 50,  ID_B14);
		CreateCell(hWnd,NULL,  50,  100,  ID_B21);
		CreateCell(hWnd,NULL,  100, 100, ID_B22);
		CreateCell(hWnd,NULL,  150, 100, ID_B23);
		CreateCell(hWnd,NULL,  200, 100, ID_B24);
		CreateCell(hWnd,NULL,  50,  150, ID_B31);
		CreateCell(hWnd,NULL,  100, 150, ID_B32);
		CreateCell(hWnd,NULL,  150, 150, ID_B33);
		CreateCell(hWnd,NULL,  200, 150, ID_B34);
		CreateCell(hWnd,NULL,  50,  200, ID_B41);
		CreateCell(hWnd,NULL,  100, 200, ID_B42);
		CreateCell(hWnd,NULL,  150, 200, ID_B43);
		CreateCell(hWnd,NULL,  200, 200, ID_B44);


		CreateMainMenu(hWnd);
		
		CreateStatic(hWnd,"Рекорды", 270, 60, ID_RECORD);
		CreateStatic(hWnd,"1. 50", 270, 90, ID_RESULT1);
		CreateStatic(hWnd,"2. 50", 270, 120, ID_RESULT2);
		CreateStatic(hWnd,"3. 50", 270, 150, ID_RESULT3);
		CreateStatic(hWnd,"4. 50", 270, 180, ID_RESULT4);
		CreateStatic(hWnd,"5. 50", 270, 210, ID_RESULT5);	
	
		
		CreateScore(hWnd, "Текущий счет", 80, 10, ID_SCORE);
		CreateScoreNumber(hWnd, 0 , 200, 10, ID_CURRENT_SCORE);
		
		
		StartNewGame(hWnd);
		SetScore(hWnd);
	
		return 0;
		
	case WM_GETMINMAXINFO:
		OnGetMinMaxInfo((MINMAXINFO*)lParam);
		return 0;

	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;


	
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);

};


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreviousInstance, LPSTR lpCmdLine, int nCmdShow)
{
	char szClassName[] = "2048",
		szWindowCaption[] = "2048";

	srand((unsigned)time(NULL));

	WNDCLASS wndClass;
	wndClass.style = CS_HREDRAW|CS_VREDRAW;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.lpfnWndProc = WindowFunction;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 20);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = szClassName;

	int iClassID = RegisterClass(&wndClass);
	if (0 == iClassID)
	{
		MessageBox(NULL, "Error! Window class not registred", "2048",0);
		return 0;
	}


	HWND hWnd = CreateWindow
					(
						szClassName,
						szWindowCaption,
						WS_OVERLAPPEDWINDOW | WS_VISIBLE,
						0,
						0,
						400,
						340,
						0,
						0,
						hInstance,
						0
					);



	if( 0==hWnd)
	{
		MessageBox(NULL, "Error! Window not created",  "2048",0);
		return 0;
	}

	MSG message;
	while(GetMessage(&message, NULL, 0,0))
		DispatchMessage(&message);
	
	return (int)message.wParam;
};