#include <xc.h>
#include <util/delay.h>
#include <avr/interrupt.h>
int index = 0;
int input = 0;
int flag = 0;
int i = 0;
int player1[] = {0, 0, 0, 0};
int player2[] = {0, 0, 0, 0};
int numbers[] = {128, 64, 32, 16};
#define F_CPU 8000000


void interrupt(int *player) {
	if (flag && (player[1] != 0 || player[0] != 0 || player[3] != 0 || player[2] != 0)) {
		player[3]--;
	}
	if (player[3] < 0) {
		player[2] --;
		player[3] = 9;
	}
	if (player[2] < 0) {
		player[1] --;
		player[2] = 5;
	}
	if (player[1] < 0) {
		player[0] --;
		player[1] = 9;
	}
}
ISR (TIMER1_OVF_vect) {
		switch (input) {
			case 0:
				interrupt(player1);
				break;
			case 1:
				interrupt(player2);
				break;
		}
		TCNT1 = 65535 - (F_CPU/1024);
}
ISR(INT0_vect){
	flag = 1;
	input = !input;
	_delay_ms(100);
}
ISR(INT1_vect){
	if (player1[1] >= 9) {
		player1[0] += 1;
		player2[0] += 1;
		player1[1] = player1[1] - 9;
		player2[1] = player2[1] -9;
	} else {
		player1[1] += 1;
		player2[1] += 1;	
	}
	_delay_ms(100);
}
int main(void)
{
	flag = 0;
	input = 0;
	DDRB = 0xFF;
	DDRC = 0xFF;
	DDRD = 0b11110000;
	PORTD = 0b00001111;
	TCNT1 = 65535 - (F_CPU/1024);
	TCCR1B = (1 << CS10) | (1 << CS12);
	TCCR1A = 0x00;
	TIMSK1 = (1 << TOIE1);
	EICRA = 0x0a;
	EIMSK |= 0x03;
	sei();
	while(1)
	{
		if (!(PIND & 0b00000010)){
			for (i = 0; i < 4; i++) {
				player1[i] = 0;
				player2[i] = 0;
			}
			flag = 0;
			_delay_ms(10);
		}
		switch (index) {
			case 0:
				print(player1[3], 0);
				print(player2[3], 1);
				break;
			case 1:
				print(player1[2], 0);
				print(player2[2], 1);
				break;
			case 2:
				print(player1[1], 0);
				print(player2[1], 1);
				break;
			case 3:
				print(player1[0], 0);
				print(player2[0], 1);
				break;
		}
		PORTD = numbers[index];
		index = (index + 1) % 4;
		_delay_ms(20);
	}
}
// BCD decoder
void print(int number, int port) {
	if (port == 0) {
		switch (number) {
			case 0:
			PORTC = 0b11000000;
			break;
			case 1:
			PORTC = 0b11111001;
			break;
			case 2:
			PORTC = 0b00100100;
			break;
			case 3:
			PORTC = 0b00110000;
			break;
			case 4:
			PORTC = 0b00011001;
			break;
			case 5:
			PORTC = 0b00010010;
			break;
			case 6:
			PORTC = 0b00000010;
			break;
			case 7:
			PORTC = 0b11111000;
			break;
			case 8:
			PORTC = 0b00000000;
			break;
			case 9:
			PORTC =  0b00011000;
			break;
		}
	}
	if (port == 1) {
		switch (number) {
			case 0:
			PORTB = 0b11000000;
			break;
			case 1:
			PORTB = 0b11111001;
			break;
			case 2:
			PORTB = 0b00100100;
			break;
			case 3:
			PORTB = 0b00110000;
			break;
			case 4:
			PORTB = 0b00011001;
			break;
			case 5:
			PORTB = 0b00010010;
			break;
			case 6:
			PORTB = 0b00000010;
			break;
			case 7:
			PORTB = 0b11111000;
			break;
			case 8:
			PORTB = 0b00000000;
			break;
			case 9:
			PORTB =  0b00011000;
			break;
		}
	}
}