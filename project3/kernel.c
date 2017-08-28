void putChar(char car, byte col, int fil, int col);
void putStr(char* str, byte col, int fil, int col);
int printString(char *str);
int readChar();
int readString(char *buf);
int readSector(char *buf, int absSector);
int readfile(char *filename, char *buf);
void executeProgram(char* name, int segment); 
int strComp(char* buf, char* fileName);
void terminate();
void resetSegments();

int main()
{
	makeInterrupt21();
	interrupt(0x21, 0x04, "uprog1\0", 0x2000, 0);
	interrupt(0x21, 0x00, "Done!\n\r\0", 0, 0);
	while(1);
}

int MOD(int x, int y) 
{
	return (x - y) * ( x / y); 
}

void putChar(char caracter, int color, int fila, int columna) 
{
	int BeginningVMemory = 0xB8000;
	int AddressRelative = ((80*(fila-1))+(columna-1)) * 2;
	int MemoryAddress = BeginningVMemory + AddressRelative;
    	int segment = 0xB0000;

    	int offset = MemoryAddress - segment;

	putInMemory(0xB000, offset, caracter);
	putInMemory(0xB000, offset+1, color);
}

void putStr(char* string, int color, int fila, int columna)
{
	if (fila>= 0 && columna >= 0 && fila < 25)
	{
		int i = 0;

		while(string[i] != '\0')
		{
			if(columna >= 80)
			{
				columna = 0;
				fila++;	
			}
			if(fila < 25)
			{
				putChar(string[i], color, fila, columna);
			    	columna++;
				i++;
			}
			else
			{
			 	break;
			}
		}
	}
	
}

int printString(char *str)
{
	int cont = 0;

	char ah = 0x0E;
	
	while(str[cont] != '\0')
	{
		char al = str[cont];  	  
		int ax = ah * 256 + al; 
		interrupt(0x10, ax, 0, 0, 0);		
		cont++;	
	}

	return cont;	
}

int readChar()
{
	return interrupt(0x16, 0, 0, 0, 0);
}

int readString(char *buf)
{
	char caracter = readChar();
	int contador = 0;
	char ah = 0x0E;

	while(caracter != 0x0D)
	{
		if(caracter == 0x08)
		{
			if(caracter > 0)
			{
		                int ax = ah * 256 + caracter;
				int ax2 = ah * 256 + ' ';

				interrupt(0x10, ax , 0, 0, 0);
				interrupt(0x10, ax2, 0, 0, 0);
				interrupt(0x10, ax, 0, 0, 0);
				contador--;
		
			}
		}		
		else
		{
			int ax = ah * 256 + caracter; 
			interrupt(0x10, ax, 0, 0, 0);

			*(buf + contador) = caracter;
			contador++;			
		}

			caracter = readChar();
	}
return 0;
}

int readSector(char *buf, int absSector)
{
	//3.5"
	int relSector = MOD(absSector,18) + 1;
	int head = MOD((absSector/18 ), 2);
	int track = ( absSector / 36 );

	int ax = 0x02 * 256 + 0x01;
	int bx = buf;
	int cx = track * 256 + relSector;
	int dx = head*256 + 0x00;
	
	interrupt(0x13, ax, bx, cx, dx);
	
	return 0;
}

int handleInterrupt21(int ax, int bx, int cx, int dx)
{
	if(ax == 0x00)
	{
		return printString(bx);
	}
	else if(ax == 0x11)
	{
		char* aCh = bx;
		aCh[0] = readChar();
		return 1;
	}
	else if(ax == 0x01)
	{
		char* aLn = bx;
		int cont;
		cont = readString(aLn,50);
		return cont;
	}
	return -1;
}

int readFile(char* fileName, char* buf)
{ 
        int file=0;
        int name = 0;
	int index, h, k, i;
	int sectores[27];
	int j = 0;
	int buffAddress = 0;
	
    	readSector(buf, 2);  

	file = strComp(buf,fileName);

	if (file != 0)
	{
		index = file*32+6;

		for (j=0; j<26; j++)
		{
			sectores[j] = buf[index+j];
		}

		sectores[26] = 0;
		k = 0;

		while(sectores[k]!=0x0)
		{
			readSector(buf + buffAddress, sectores[k]);
			buffAddress += 512;
			k++;
		}
		
	}
	else
	{
		printString("No se encontro el archivo!");
		return -1;
	}		

}

int strComp(char* buf, char* fileName)
{ 
	int i, j;

	int check = 0;


     	for (i = 0; i < 16; i++)
 	{
		if (buf[32*i] != 0x0)
		{
			for (j=0; j < 6; j++)
			{
				if (buf[j+32*1] == 0x0 || buf[j+32*1] == '\r' || buf[j+32*1] == '\n' || fileName[j] == 0x0 || fileName[j] == '\r' || fileName[j] == '\n')
				{
					break;
				}
				else if (buf[j+32*i] == fileName[j]){
					check = 1;	
				}
				else {
					check = 0;
					break;
				}
				
			}
		 	
			if (check == 1){

				 return i;
			}
		}
		}
			if (check == 0)
			{
				 for (i=0; i<13312; i++)
				 {
					buf[i] = 0x0;
				 }	 
				return 0;
			 }
}

void executeProgram(char* name, int segment)
{ 
	int i;
	char readingBuffer[13312];

	readFile(name, readingBuffer);

	for (i=0; i<13312; i++)
	{
		putInMemory(segment, i, readingBuffer[i]);
	} 

	launchProgram(segment);
}

void terminate()
{
	char shell[6];
	shell[0] = 's';
	shell[1] = 'h';
	shell[2] = 'e';
	shell[3] = 'l';
	shell[4] = 'l';
	shell[5] = 0x0;
	interrupt(0x21, 4, shell, 0x2000,0);
}
