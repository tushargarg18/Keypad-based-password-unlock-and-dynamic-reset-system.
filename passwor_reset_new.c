#define F_CPU 8000000UL
#include<avr/io.h>
#include<util/delay.h>
#include<string.h>
#define row0 0
#define row1 1
#define row2 2
#define row3 3
#define col0 4
#define col1 5
#define col2 6
void lcd_command(char );
void lcd_init(void );
void lcd_data_enable_to_write();
void lcd_command_enable_to_write();
void lcd_data(char );
void lcd_string(const char *);
char keypad(void );
void password(int,char *);
int main()
{
	PORTA=0x0F;//activate pull up from PA0 to PA3
	DDRA=0xF0;
	int count=0;
	char *ptrn="123";
	lcd_init();
	password(count,ptrn);
}
char keypad()
{
	/*double press[3]={'PORTA &~(1<<col0)|(1<<col1)|(1<<col2)','PORTA &~(1<<col1)|(1<<col0)|(1<<col2)','PORTA &~(1<<col2)|(1<<col1)|(1<<col0)'};
	char c;
	int i;
	c=PINA;
		for(i=0;i<3;i++)
		{
			PORTA=press[i];
		c=c&0x0f;
		switch (c)
		{
			case 14:	
			{
				lcd_string("*");
				return('1');
				break;
			}
			case 13:
			{
				lcd_string("*");
				return('4');
				break;				
			}
			case 11:
				{
					lcd_string("*");
					return('7');
					break;
				}
			case 7:
			{
				lcd_string("*");
				return ('*');
				break;
			}
			default: return ('\0');	

		}
		
	}*/
	PORTA=PORTA&~(1<<col0);
	PORTA=PORTA|(1<<col1)|(1<<col2);
	if( (PINA&(1<<row0))==0)
	{
		lcd_string("*");
		_delay_ms(200);
		return('1');
		
	}
	else if( (PINA&(1<<row1))==0)
	{
		lcd_string("*");
		_delay_ms(200);
		return('4');
		
	}
	else if( (PINA&(1<<row2))==0)
	{
		lcd_string("*");
		_delay_ms(200);
		return('7');
		
	}
	else if( (PINA&(1<<row3))==0)
	{
		lcd_string("*");
		_delay_ms(200);
		return ('*');
		
	}

	PORTA=PORTA&~(1<<col1);
	PORTA=PORTA|(1<<col0)|(1<<col2);
	if( (PINA&(1<<row0))==0)
	{
		lcd_string("*");
		_delay_ms(200);
		return('2');
		
	}
	else if( (PINA&(1<<row1))==0)
	{
		lcd_string("*");
		_delay_ms(200);
		return('5');
		
		
	}
	else if( (PINA&(1<<row2))==0)
	{
		lcd_string("*");
		_delay_ms(200);
		return('8');
		
	}
	else if( (PINA&(1<<row3))==0)
	{
		lcd_string("*");
		_delay_ms(200);
		return('0');
		
	}

	PORTA=PORTA&~(1<<col2);
	PORTA=PORTA|(1<<col0)|(1<<col1);
	if( (PINA&(1<<row0))==0)
	{
		lcd_string("*");
		_delay_ms(200);
		return('3');
		
	}
	else if( (PINA&(1<<row1))==0)
	{
		lcd_string("*");
		_delay_ms(200);
		return('6');
		
	}
	else if( (PINA&(1<<row2))==0)
	{
		lcd_string("*");
		_delay_ms(200);
		return('9');
		
	}
	else if( (PINA&(1<<row3))==0)
	{
		lcd_string("*");
		_delay_ms(200);
		return('#');
		
	}
	else
	return ('\0');
}
void password(int count,char *ptrn)
{
			int i,j,q,p;
			char temp,*ptr,temp2;
			ptr=(char*)calloc(1, sizeof(char));
			if(count==2)									//for wrong pass
			{
				lcd_command(0x01);
				lcd_string("retry in 10 sec");
				_delay_ms(10000);
				lcd_command(0x01);
				lcd_string("Try Again");
				_delay_ms(200);
				count=0;
				password(count,ptrn);
				lcd_command(0x01);
			}
			i=0;
			do											//check string  
			{ 
				temp=keypad();
				if(temp!='\0')
				{
					*(ptr+i)=temp;
					ptr=(char*)realloc(ptr, sizeof(char));
					i++;
				}
			}while(*(ptrn+i)!='\0');		
			if(strcmp(ptrn,ptr)==0)					//comparison of pass and enter 
			{	q=0;
				count=0;
				lcd_command(0xc0);
				lcd_string("Unlocked");
				_delay_ms(2000);
				lcd_command(0x01);
				lcd_string("welcome");
				_delay_ms(2000);
				lcd_command(0x01);
				lcd_string("'#' to lock");
				lcd_command(0xc0);
				lcd_string("'*' to reset");
				while (q!=1)
				{
					p=0;
					temp=keypad();
					if(temp!='\0')
					{	
						if(temp=='#')
						{
							lcd_command(0x01);
							q=1;
							p=1;
						}
						else if(temp=='*')
						{
							q=1;
							lcd_command(0x01);
							lcd_string("press '#' for");
							lcd_command(0xc0);
							lcd_string("terminating");
							_delay_ms(4000);
							lcd_command(0x01);
							free(ptr);
							free(ptrn);
							ptrn=(char*)calloc(ptrn,sizeof(char));
							j=0;
							while(1)
							{
								temp2=keypad();
								if((temp2!='\0') && (temp2!='#'))
								{
									*(ptrn+j)=temp2;
									ptrn=(char*)realloc(ptrn,sizeof(char));
									j++;
								}
								if(temp2=='#')
								{
									//*(ptrn+j+1)='\0';
									p=1;
									break;
								}							
							}	
							lcd_command(0x01);
							lcd_string("New Pass:");
							lcd_command(0xc0);
							lcd_string(ptrn);
							_delay_ms(4000);	
						
						}

					}
					else
					q=0;
				}
				if(p==1)
				{
					lcd_command(0x01);
					password(count,ptrn);					
				}							
			}
			else
			{
				lcd_command(0xc0);
				lcd_string("Incorrect");
				_delay_ms(2000);
				++count;
				lcd_command(0x01);
				password(count,ptrn);
			}
}
void lcd_string(const char *x)//x=2000
{
	int a=0;
	while(*x !='\0')
	{   a++;
		if(a>=16)
		{
			lcd_command(0x8f);
			lcd_data(*x++);
			_delay_ms(100);
			lcd_command(0x1c);
		}
		else
		lcd_data(*x++);
		
		//_delay_ms(200);
	}

}
void lcd_init()
{
	DDRD=0xff;
	lcd_command(0x02);
	lcd_command(0x28);
	lcd_command(0x0E);

}
void lcd_command(char x)
{
	PORTD=(x&0xf0);  //upper 4 bit
	lcd_command_enable_to_write();
	PORTD=((x<<4)&0xf0); //lower 4 bit
	lcd_command_enable_to_write();
}
void lcd_data(char x)
{
	PORTD=(x&0xf0);  //upper 4 bit
	lcd_data_enable_to_write();
	PORTD=((x<<4)&0xf0); //lower 4 bit
	lcd_data_enable_to_write();
}
void lcd_data_enable_to_write()
{
	PORTD=PORTD |(1<<0);//RS=0;
	PORTD=PORTD&~(1<<1);//RW=0;
	PORTD=PORTD |(1<<2);//EN=1;
	PORTD=PORTD&~(1<<2);//EN=0;
	_delay_ms(1);
}
void lcd_command_enable_to_write()
{
	PORTD=PORTD&~(1<<0);//RS=0;
	PORTD=PORTD&~(1<<1);//RW=0;
	PORTD=PORTD |(1<<2);//EN=1;
	PORTD=PORTD&~(1<<2);//EN=0;
	_delay_ms(1);
}
