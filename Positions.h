class Position
{
public:
	int Numbers[4][4];

	void GetPositionsFileName(char FileName[MAX_PATH])
	{
		HMODULE hModule = GetModuleHandle(0);
		GetModuleFileName(hModule, FileName, MAX_PATH);
		char *LastBackslash = strrchr(FileName, 92);
		LastBackslash[1] = 0;
		strcat(FileName,"positions.csv");
	};

	void Read()
	{
		//work with file

		char FileName[MAX_PATH];
		GetPositionsFileName(FileName);

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
		char* End;
		char* Start = FileText;
		End= 2 + strstr( FileText,"\r\n");

		for( int j=0; j<4; j++)
		{	
			for( int i=0; i<4; i++)
			{	
				//looking for position
				int Length = End - Start;
				char * String = (char*)malloc(Length + 1);
				memcpy(String, Start, Length);
				memset(String + Length, 0, 1);

				//separating them
				char * NamePointer = strstr(String,";");
				Numbers[i][j]=atoi((char*)(NamePointer+1));

				//preparation for new cycle
				Start=End;
				End = 2 + strstr(Start, "\r\n");
				free(String);
				String = 0;
			}

			free(FileText);
			FileText = 0;
		}
	};

	void Write()
	{
		//preparing data to write down
		char buffer[1024];
		sprintf
			(
			buffer,
			"%d\r\n;%d\r\n;%d\r\n;%d\r\n;%d\r\n%d\r\n;%d\r\n;%d\r\n;%d\r\n;%d\r\n%d\r\n;%d\r\n;%d\r\n;%d\r\n;%d\r\n;%d\r\n", 
			Numbers[0][0],
			Numbers[0][1],
			Numbers[0][2],
			Numbers[0][3],
			
			Numbers[1][0],
			Numbers[1][1],
			Numbers[1][2],
			Numbers[1][3],
			
			Numbers[2][0],
			Numbers[2][1],
			Numbers[2][2],
			Numbers[2][3],
		
			Numbers[3][0],
			Numbers[3][1],
			Numbers[3][2],
			Numbers[3][3]
			);

		// creating the file and copying the data there.

		char FileName[MAX_PATH];
		GetPositionsFileName(FileName);
		int OpenHandle = _open(FileName, _O_BINARY | _O_CREAT | _O_WRONLY | _O_TRUNC, _S_IWRITE);
		if(-1 != OpenHandle)
		{
			_write(OpenHandle, buffer, strlen(buffer));
			_close(OpenHandle);
			OpenHandle = -1;
		}

	};
};