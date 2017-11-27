/*  x86 realmode pong
 * 
 *  runs on bare metal hardware, it just uses some BIOS
 *  interrupts.
 * 
 *  (c) 2016 <stecdose@gmail.com>
 * 
 */

#define SCR_X	80
#define SCR_Y	25

#define FIELD_X	80
#define FIELD_Y	24

#define LEFT	0
#define RIGHT	1
#define UP		0
#define DOWN	1

/* color definitions */
#define ATTRIB_BALL		0xCF
#define ATTRIB_PADDLE_L		0x2E
#define ATTRIB_PADDLE_R		0x5A
#define ATTRIB_PADDLE_CLEAR	0x00

#define PADDLE_SIZE_X	2
#define PADDLE_SIZE_Y	8

/* datatypes */
#define uint8_t		unsigned char
#define uint16_t 	unsigned short
#define NULL 		((void *)0)

/* prototypes: game functions */
void draw_score_bar();
void draw_top_bar();
void draw_paddle(int x, int y, uint16_t attr);
void draw_ball(uint16_t bx, uint16_t by, uint16_t attr);
void update_score_bar();

/* prototypes: screen related */
void clrscr();
void set_cursor(uint16_t col, uint16_t row);
void inc_cursor();

/* prototypes: text+number output */
void _putc(c);
void put_hex8(uint8_t hex8);
void put_hex16(uint16_t hex16);
void putc_attr(uint16_t attrib, uint16_t c);
void print_str(char *s);
char *_ultoa(unsigned long val);

/* prototypes: keyboard */
uint16_t _getch();
#define _getch_nonblocking() _inb(0x60)
void _kbhit();

/* prototypes: system related */
void simple_delay(uint16_t del);
uint16_t _inb(uint16_t port);

/* vars */
uint16_t color_l, color_r, color_ball;
uint16_t score_l, score_r;
uint16_t pos_l, pos_r;
uint16_t scancode;
uint16_t speed;
uint16_t ball_x, ball_y;
uint16_t dir_x, dir_y;

int main() {
	uint8_t u8temp;
		
	score_l = 0;
	score_r = 0;
	pos_l = 1;
	pos_r = 3;
	color_l = ATTRIB_PADDLE_L;
	color_r = ATTRIB_PADDLE_R;
	ball_x = SCR_X / 2;
	ball_y = SCR_Y / 2;
	color_ball = ATTRIB_BALL;
	speed = 1;
	dir_x = LEFT;
	dir_y = UP;
	
	#asm
		mov ah, #0x01
		mov cx, #0x2607		// invisible cursor
		int 0x10
	#endasm
	
	clrscr();
	draw_score_bar();
	draw_top_bar();
	update_score_bar();
	
	draw_paddle(0, pos_l, color_l);
	draw_paddle(SCR_X-PADDLE_SIZE_X-1, pos_r, color_r);
	draw_ball(ball_x, ball_y, color_ball);
	
	_kbhit();
	
	for(;;) {
		scancode = _getch_nonblocking();
		update_score_bar();
		switch(scancode) {
			case 0x13:	// reset, r
				score_l = 0;
				score_r = 0;
				draw_ball(ball_x, ball_y, 0x00);
				ball_x = SCR_X / 2;
				ball_y = SCR_Y / 2;
				draw_ball(ball_x, ball_y, color_ball);
				draw_score_bar();
				update_score_bar();
				_kbhit();				
				_kbhit();				
				break;
			case 0x19:	// pause, p
				_kbhit();
				_kbhit();				
				break;
			case 0x48:	// R UP, arrow up
				if(pos_r > 1) {
					pos_r--;
					draw_paddle(SCR_X-PADDLE_SIZE_X-1, pos_r+1, ATTRIB_PADDLE_CLEAR);
					draw_paddle(SCR_X-PADDLE_SIZE_X-1, pos_r, color_r);
				}
				break;
			case 0x50:	// R DOWN, arrow down
				if(pos_r < (SCR_Y-1-PADDLE_SIZE_Y)) {
					pos_r++;
					draw_paddle(SCR_X-PADDLE_SIZE_X-1, pos_r-1, ATTRIB_PADDLE_CLEAR);
					draw_paddle(SCR_X-PADDLE_SIZE_X-1, pos_r, color_r);
				}
				break;
			case 0x10:	// L UP, q
				if(pos_l > 1) {
					pos_l--;
					draw_paddle(0, pos_l+1, ATTRIB_PADDLE_CLEAR);
					draw_paddle(0, pos_l, color_l);
				}
				break;
			case 0x1E:	// L DOWN, a
				if(pos_l < (SCR_Y-1-PADDLE_SIZE_Y)) {
					pos_l++;
					draw_paddle(0, pos_l-1, ATTRIB_PADDLE_CLEAR);
					draw_paddle(0, pos_l, color_l);
				}
				break;
			case 0x0B:	// R color 2, 0
				u8temp = (uint8_t)color_r;
				u8temp++;
				u8temp &= 0x0f;
				color_r &= 0xf0;
				color_r = color_r | u8temp;
				draw_paddle(SCR_X-PADDLE_SIZE_X-1, pos_r, color_r);	
				simple_delay(10);
				break;
			case 0x0A:	// R color 1, 9
				u8temp = (uint8_t)color_r;
				u8temp >>= 4;
				u8temp++;
				u8temp &= 0x0f;
				u8temp <<= 4;
				color_r &= 0x0f;
				color_r = color_r | u8temp;
				draw_paddle(SCR_X-PADDLE_SIZE_X-1, pos_r, color_r);
				simple_delay(10);
				break;
			case 0x02:	// L color 1, 1
				u8temp = (uint8_t)color_l;
				u8temp >>= 4;
				u8temp++;
				u8temp &= 0x0f;
				u8temp <<= 4;
				color_l &= 0x0f;
				color_l = color_l | u8temp;
				draw_paddle(0, pos_l, color_l);
				simple_delay(10);
				break;
			case 0x03:	// L color 2, 2
				u8temp = (uint8_t)color_l;
				u8temp++;
				u8temp &= 0x0f;
				color_l &= 0xf0;
				color_l = color_l | u8temp;
				draw_paddle(0, pos_l, color_l);
				simple_delay(10);
				break;
			case 0x4e:	// speed up, numpad +
				speed--;
				if(speed == 0) speed = 1;
				simple_delay(3);
				break;
			case 0x4a:	// speed down, numpad -
				speed++;
				if(speed > 0xff) speed = 0xff;
				simple_delay(3);
				break;
			case 0x01:	// ESC, return to DOS
				#asm
					mov ah, 0x4c
					int 0x21
				#endasm
				break;
		}
		if(dir_x == LEFT) {
			draw_ball(ball_x, ball_y, 0x00);
			ball_x--;
			ball_x--;
			if(ball_x < 3) {	// hit left
				ball_x = 3;
				dir_x = RIGHT;
				if(!((ball_y > pos_l) && (ball_y < (pos_l+PADDLE_SIZE_Y)))) {
					score_r++;
				}
			}
		} else {		// dir = RIGHT
			draw_ball(ball_x, ball_y, 0x00);
			ball_x++;
			ball_x++;
			if(ball_x > 76) {	// hit right
				ball_x = 76;
				dir_x = LEFT;
				if(!((ball_y > pos_r) && (ball_y < (pos_r+PADDLE_SIZE_Y)))) {
					score_l++;
				}
			}
		}
		if(dir_y == UP) {
			draw_ball(ball_x, ball_y, 0x00);
			ball_y--;
			if(ball_y < 1) {	// hit top
				ball_y = 1;
				dir_y = DOWN;
			}
		} else {		// dir = DOWN
			draw_ball(ball_x, ball_y, 0x00);
			ball_y++;
			if(ball_y > 23) {	// hit bottom
				ball_y = 23;
				dir_y = UP;
			}
		}
		draw_ball(ball_x, ball_y, ATTRIB_BALL);
		simple_delay(speed);
	}
}

void simple_delay(uint16_t del) {
	#asm
		mov bx, sp
		mov di, [bx+2]
		mov ah, #0x00
		int 0x1A		; get time
		mov bx,dx
		del_loop:
                mov ah, #0x00
                int #0x1A
                sub dx,bx
                cmp di,dx
		ja del_loop
    #endasm
}

void draw_ball(uint16_t bx, uint16_t by, uint16_t attr) {
	set_cursor(bx, by);
	putc_attr(attr, 0xDB);
}

void update_score_bar() {
	set_cursor(11, 24);
	put_hex8(color_l);
	inc_cursor();
	put_hex8(color_r);
	inc_cursor();
	put_hex8(speed);
	set_cursor(5, SCR_Y-1);
	print_str(_ultoa((unsigned long)score_l));
	set_cursor(SCR_X-6, SCR_Y-1);
	print_str(_ultoa((unsigned long)score_r));
}

void draw_paddle(int x, int y, uint8_t attr) {
	int y_start = y;
	for(; y < (y_start+PADDLE_SIZE_Y); y++) {
		set_cursor(x,y);
		putc_attr(attr, '@');
		set_cursor(x+1,y);
		putc_attr(attr, '@');
		set_cursor(x+2,y);
		putc_attr(attr, '@');
	}
}

void draw_score_bar() {
	int i;
			
	for(i = 0; i < 80; i++) {
		set_cursor(i, SCR_Y-1);
		putc_attr(0x3F, ' ');
	}
}

void draw_top_bar() {
	int i;
	
	for(i = 0; i < 80; i++) {
		set_cursor(i, 0);
		putc_attr(0x4F, ' ');
	}
	set_cursor(0,0);
	print_str("KEYS LEFT> A Q 1 2                ESC=END                DOWN UP 9 0 <KEYS RIGHT");
}

uint16_t _inb(uint16_t port) {
	#asm
		mov bx, sp
		mov dx, [bx+2]	; get port
		xor ax, ax	; clear AX
		in al, dx		
	#endasm
}

void inc_cursor() {
	#asm
		xor bx, bx		; page 0
		mov ah, #0x03
		int 0x10		; get cursor
		inc dl			; inc row
		mov ah, #0x02
		int 0x10		; set cursor
	#endasm
}

void putc_attr(uint16_t attrib, uint16_t c) {
	#asm
		; put_char takes:
		; AL     = char (ASCII)
		; BL     = color
		; located at cursor
		
		mov bx, sp
		mov al, [bx+4]	; get c
		mov ah, #0x09	; interrupt function
		mov bl, [bx+2]	; get attrib
		mov bh, #0	; page
		mov cx, #1
		
		int 0x10
	#endasm
}

void _putc(c) {
	#asm
		mov ah, #0x0E	; int function Eh, write char, move cursor
		mov bx, sp
		mov al, [bx+2]	; arguments are on stack, 
				; see cdecl calling conventions
		mov bh, #0	; page
		mov cx, #1	; number of chars
		int 0x10
	#endasm
}

void clrscr() {
	set_cursor(0,0);
	#asm
		mov ah, #0x0A	; repeat printing chars
		mov al, #0x20	; space
		mov bh, #0
		mov cx, #2000
		int 0x10
	#endasm
}

void set_cursor(uint16_t col, uint16_t row) {
	#asm
		mov bx, sp
		mov dh, [bx+4]
		mov dl, [bx+2]
		xor bx, bx	; page 0
		mov ah, #0x02	; DH = row, DL = col
		int 0x10
	#endasm
}

/* ultoa is from others:
 * 
 * Copyright (C) 1995,1996 Robert de Bath <rdebath@cix.compulink.co.uk>
 * This file is part of the Linux-8086 C library and is distributed
 * under the GNU Library General Public License.
 * 
 * taken from bcc's source
 */
static char buf[12];
char *_ultoa(unsigned long val) {
   char *p;

   p = buf+sizeof(buf);
   *--p = '\0';

   do
   {
      *--p = '0' + val%10;
      val/=10;
   }
   while(val);
   return p;
}

void print_str(char *s) {
	while(*s) {
		_putc(*s);
		s++;
	}
}

void put_hex8(uint8_t hex8) {
	uint8_t save = hex8;
	
	hex8 = hex8 >> 4;
	hex8 = hex8 & 0x0f;
	hex8 += '0';
	if(hex8 > '9') hex8 += 7;
	_putc(hex8);
	
	save = save & 0x0f;
	save += '0';
	if(save > '9') save += 7;
	_putc(save);	
}

void put_hex16(uint16_t hex16) {
	uint8_t hex8 = (hex16 >> 8) & 0xff;
	put_hex8(hex8);
	
	hex8 = hex16 & 0xff;
	put_hex8(hex8);
}

void _kbhit() {
	#asm
		mov ah, #0x00	; read keyboard, blocking
		int 0x16
	#endasm
}

