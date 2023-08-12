/*==========================================================================
Взаимодействие клавиатуры и ЖКИ 
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
   /* Инициализация дисплея */
   DDRB = 0xFF; 				// установка portB на вывод данных
   DDRD = 0xFF; 				// установка portD на вывод данных
   _delay_ms(50); 
   send_a_command(0x01); 			// команда очистить экран 0x01 = 00000001
   _delay_ms(50);
   send_a_command(0x38);			// 8 битный режим передачи данных/команд
   _delay_ms(50);
   send_a_command(0b00001111); 			//включаем курсор и мигание курсора на ЖК дисплее    
   send_a_string("ENTER DIGIT:"); 		//отображение строки "" 
   send_a_command(0xC0);			// перемещение курсора на вторую строку ЖК дисплея
   _delay_ms(50);
    
   while (1){
     
      DDRF|=rows_mask;				// Настроить строки на вывод
      DDRF&=~columns_mask;			// Настроить столбцы на ввод
      PORTF|=columns_mask;			// Включить pull-up для ввода столбцов
      PORTF&=~rows_mask;			// Вывести в строки нули
      column=PINF&columns_mask;			// Прочесть столбцы
      PORTF|=rows_mask;				// Убрать нули из строк => никаких к.з.
      DDRF|=columns_mask;			// Настроить столбцы на вывод
      DDRF&=~rows_mask;				// Настроить строки на ввод
      PORTF|=rows_mask;				// Включить pull-up для ввода строк
      PORTF&=~columns_mask;			// Вывести в столбцы нули
      row=PINF&rows_mask;			// Прочесть строки
      PORTF|=columns_mask;			// Убрать нули из столбцов

      switch(column|row){ 			// Соединим тетрады и выявим нажатую кнопку
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
	 default: key=0;				// Ничего не нажато
	}
	
	 if (key){ 
	    send_a_character(key);
	    count ++;
	    _delay_ms(250);
	 }
	
	 /* очитска дисплея если строка заполнена */
	 if (count == 16){ 				// если 16 символов показаны на ЖК дисплее
	    send_a_command(0x01);			// очистить экран ЖК дисплея
            _delay_ms(50);
	    send_a_string("ENTER DIGIT:"); 		//отображение строки "" 
	    send_a_command(0xC0);			// перемещение курсора на вторую строку ЖК дисплея
	    _delay_ms(50);
            count=0;
	 }
      }
   return 0;
}

/* Функция посылки команды */
void send_a_command (unsigned char command){
   PORTB=command;
   PORTD&= ~(1<<RS);
   PORTD|= (1<<E);
   _delay_ms(50);
   PORTD&= ~(1<<E);
   PORTB =0;
}

/* Функция посылки данных */
void send_a_character (unsigned char character){
   PORTB=character;
   PORTD|= (1<<RS);
   PORTD|= (1<<E);
   _delay_ms(50);
   PORTD&= ~(1<<E);
   PORTB =0;
}

/* массив чаров и вывод на дисплей */
void send_a_string(char *string_of_characters)
{
   while(*string_of_characters > 0){
      send_a_character(*string_of_characters++);
   }
}
