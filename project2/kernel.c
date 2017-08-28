
void putChar(char car, byte col, int fil, int col);
void putStr(char* str, byte col, int fil, int col);
int printString(char *str);
int readChar();
int readString(char *buf);
int readSector(char *buf, int absSector);

int main()
{

	makeInterrupt21();
	interrupt(0x21,0x00,0,0,0);
while(1)
{
};

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
	int relSector = mod(absSector,18) + 1;
	int head = mod((absSector/18 ), 2);
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


