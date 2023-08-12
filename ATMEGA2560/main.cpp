/*==========================================================================
�������������� ���������� � ��� 
==========================================================================*/

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#define rows_mask 0x0f 					///00001111
#define columns_mask 0xf0 				///11110000

#define RS 6
#define E  5

volatile uint8_t row,column;
char key;
void send_a_command (unsigned char command);
void send_a_character(unsigned char character);
void send_a_string(char *string_of_characters);
int count = 0;

int main(){
   /* ������������� ������� */
   DDRB = 0xFF; 				// ��������� portB �� ����� ������
   DDRD = 0xFF; 				// ��������� portD �� ����� ������
   _delay_ms(50); 
   send_a_command(0x01); 			// ������� �������� ����� 0x01 = 00000001
   _delay_ms(50);
   send_a_command(0x38);			// 8 ������ ����� �������� ������/������
   _delay_ms(50);
   send_a_command(0b00001111); 			//�������� ������ � ������� ������� �� �� �������    
   send_a_string("ENTER DIGIT:"); 		//����������� ������ "" 
   send_a_command(0xC0);			// ����������� ������� �� ������ ������ �� �������
   _delay_ms(50);
    
   while (1){
     
      DDRF|=rows_mask;				// ��������� ������ �� �����
      DDRF&=~columns_mask;			// ��������� ������� �� ����
      PORTF|=columns_mask;			// �������� pull-up ��� ����� ��������
      PORTF&=~rows_mask;			// ������� � ������ ����
      column=PINF&columns_mask;			// �������� �������
      PORTF|=rows_mask;				// ������ ���� �� ����� => ������� �.�.
      DDRF|=columns_mask;			// ��������� ������� �� �����
      DDRF&=~rows_mask;				// ��������� ������ �� ����
      PORTF|=rows_mask;				// �������� pull-up ��� ����� �����
      PORTF&=~columns_mask;			// ������� � ������� ����
      row=PINF&rows_mask;			// �������� ������
      PORTF|=columns_mask;			// ������ ���� �� ��������

      switch(column|row){ 			// �������� ������� � ������ ������� ������
	 case 0b11010111: key='0'; break;
	 case 0b11101011: key='1'; break;
	 case 0b11011011: key='2'; break;
	 case 0b10111011: key='3'; break;
	 case 0b11101101: key='4'; break;
	 case 0b11011101: key='5'; break;
	 case 0b10111101: key='6'; break;
	 case 0b11101110: key='7'; break;
	 case 0b11011110: key='8'; break;
	 case 0b10111110: key='9'; break;
	 case 0b01110111: key='+'; break;
	 case 0b01111011: key='-'; break;
	 case 0b01111101: key='*'; break;
	 case 0b01111110: key='/'; break;
	 case 0b10110111: key='='; break;
	 case 0b11100111: key='C'; break;
	 default: key=0;				// ������ �� ������
	}
	
	 if (key){ 
	    send_a_character(key);
	    count ++;
	    _delay_ms(250);
	 }
	
	 /* ������� ������� ���� ������ ��������� */
	 if (count == 16){ 				// ���� 16 �������� �������� �� �� �������
	    send_a_command(0x01);			// �������� ����� �� �������
            _delay_ms(50);
	    send_a_string("ENTER DIGIT:"); 		//����������� ������ "" 
	    send_a_command(0xC0);			// ����������� ������� �� ������ ������ �� �������
	    _delay_ms(50);
            count=0;
	 }
      }
   return 0;
}

/* ������� ������� ������� */
void send_a_command (unsigned char command){
   PORTB=command;
   PORTD&= ~(1<<RS);
   PORTD|= (1<<E);
   _delay_ms(50);
   PORTD&= ~(1<<E);
   PORTB =0;
}

/* ������� ������� ������ */
void send_a_character (unsigned char character){
   PORTB=character;
   PORTD|= (1<<RS);
   PORTD|= (1<<E);
   _delay_ms(50);
   PORTD&= ~(1<<E);
   PORTB =0;
}

/* ������ ����� � ����� �� ������� */
void send_a_string(char *string_of_characters)
{
   while(*string_of_characters > 0){
      send_a_character(*string_of_characters++);
   }
}
