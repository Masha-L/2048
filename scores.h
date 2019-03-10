struct Score
{
public:
	int score;
};

class Scores
{
public:
	Score player[5];

	Scores()
	{
		for(int i=0; i<5; i++)
			player[i].score = 0;
	};

	

	void GetScoresFileName(char FileName[MAX_PATH])
	{
		HMODULE hModule = GetModuleHandle(0);
		GetModuleFileName(hModule, FileName, MAX_PATH);
		char *LastBackslash = strrchr(FileName, 92);
		LastBackslash[1] = 0;
		strcat(FileName,"scores.csv");
	};

	void Read()
	{
		//work with file

		char FileName[MAX_PATH];
		GetScoresFileName(FileName);

		int FileHandle = _open(FileName, _O_BINARY | _O_RDONLY);
		if(-1 == FileHandle)
			return;
		int FileLength =_lseek(FileHandle,0,SEEK_END);
		_lseek(FileHandle,0,SEEK_SET);
		char * FileText = (char*)malloc(FileLength+1);
		_read(FileHandle,FileText,FileLength);
		_close(FileHandle);
		FileHandle = -1;

		//work with string
		memset(FileText + FileLength, 0, 1);
		char* End=0;
		char* Start = FileText;
		End= 2 + strstr( FileText,"\r\n");

		for( int counter=0; counter<5; counter++)
		{	
			//looking for name & result
			int Length = End - Start;
			char * String = (char*)malloc(Length+1);
			memcpy(String, Start, Length);
			memset(String + Length, 0, 1);

			//separating them
			//char * NamePointer = strstr(String,";");
			
			player[counter].score=atoi((char*)(String));

			//preparation for new cycle
			Start=End;
			End = 2 + strstr(Start, "\r\n");
			free(String);
			String = 0;
		}

		free(FileText);
		FileText = 0;
		Sorting();
	};

	void Write()
	{
		//preparing data to write down
		char buffer[1024];
		sprintf
			(
			buffer,
			"%d\r\n;%d\r\n;%d\r\n;%d\r\n;%d\r\n", 
			 player[0].score,
			 player[1].score,
			 player[2].score,
			 player[3].score,
			 player[4].score
			);

		// creating the file and copying the data there.

		char FileName[MAX_PATH];
		GetScoresFileName(FileName);
		int OpenHandle = _open(FileName, _O_BINARY | _O_CREAT | _O_WRONLY | _O_TRUNC, _S_IWRITE);
		if(-1 != OpenHandle)
		{
			_write(OpenHandle, buffer, strlen(buffer));
			_close(OpenHandle);
			OpenHandle = -1;
		}

	};

	void Sorting()
	{
		for(int i=0 ; i<5; i++)
		{
			for(int j=0 ; j<4; j++)
			{
				if(player[j].score < player[j+1].score)
				{
					int Buffer = player[j].score;
					player[j].score=player[j+1].score;
					player[j+1].score = Buffer; 
				}
			}
		}

	};

	void Add(int Score)
	{
	//  looking for the place where to insertt a new value
		int Place = -1;
		for(int i = 0; i<5; i++)
			if(Score>player[i].score)
			{
				Place = i;
				break;
			}
		if (-1 == Place)
			return;

	//  shifting all entries starting from <Place> down
		for(int i=3; i>=Place; i--)
		{
			player[i+1].score = player[i].score;
		}

		player[Place].score=Score;
	}

};