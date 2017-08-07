
void putChar(char car, byte col, int fil, int col);
void putStr(char* str, byte col, int fil, int col);

int main()
{
putInMemory(0xB000, 0x8140, 'H');
putInMemory(0xB000, 0x8141, 0x7);
putInMemory(0xB000, 0x8142, 'e');
putInMemory(0xB000, 0x8143, 0x7);
putInMemory(0xB000, 0x8144, 'l');
putInMemory(0xB000, 0x8145, 0x7);
putInMemory(0xB000, 0x8146, 'l');
putInMemory(0xB000, 0x8147, 0x7);
putInMemory(0xB000, 0x8148, 'o');
putInMemory(0xB000, 0x8149, 0x7);
putInMemory(0xB000, 0x814A, ' ');
putInMemory(0xB000, 0x814B, 0x7);
putInMemory(0xB000, 0x814C, 'W');
putInMemory(0xB000, 0x814D, 0x7);
putInMemory(0xB000, 0x814E, 'o');
putInMemory(0xB000, 0x814F, 0x7);
putInMemory(0xB000, 0x8150, 'r');
putInMemory(0xB000, 0x8151, 0x7);
putInMemory(0xB000, 0x8152, 'l');
putInMemory(0xB000, 0x8153, 0x7);
putInMemory(0xB000, 0x8154, 'd');
putInMemory(0xB000, 0x8155, 0x7);

putChar('H',0x4F,12,35);
putChar('e',0x4F,12,36);
putChar('l',0x4F,12,37);
putChar('l',0x4F,12,38);
putChar('o',0x4F,12,39);
putChar(' ',0x4F,12,40);
putChar('W',0x4F,12,41);
putChar('o',0x4F,12,42);
putChar('r',0x4F,12,43);
putChar('l',0x4F,12,44);
putChar('d',0x4F,12,45);

putStr("Hello World",0xF4,24,69);

while(1)
{
};

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
			if(columna>=80)
			{
				columna = 0;
				fila++;	
			}
			if(fila<25)
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
