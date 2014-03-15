#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <term.h>
#include <curses.h>
#include <unistd.h>

static struct termios fram_initial_settings, fram_new_settings;
static int fram_peek_character = -1;

void fram_init_keyboard();
void fram_close_keyboard();
int fram_kbhit();
int fram_readch();

/*
int main()
{
int ch = 0;

init_keyboard();
while(ch != 'q') {
printf("looping\n");
sleep(1);
if(kbhit()) {
ch = readch();
printf("you hit %c\n",ch);
}
}
close_keyboard();
exit(0);
}
*/

void fram_init_keyboard()
{
tcgetattr(0,&fram_initial_settings);
fram_new_settings = fram_initial_settings;
fram_new_settings.c_lflag &= ~ICANON;
fram_new_settings.c_lflag &= ~ECHO;
fram_new_settings.c_lflag &= ~ISIG;
fram_new_settings.c_cc[VMIN] = 1;
fram_new_settings.c_cc[VTIME] = 0;
tcsetattr(0, TCSANOW, &fram_new_settings);
}

void fram_close_keyboard()
{
tcsetattr(0, TCSANOW, &fram_initial_settings);
}

int fram_kbhit()
{
char ch;
int nread;

if(fram_peek_character != -1)
return 1;
fram_new_settings.c_cc[VMIN]=0;
tcsetattr(0, TCSANOW, &fram_new_settings);
nread = read(0,&ch,1);
fram_new_settings.c_cc[VMIN]=1;
tcsetattr(0, TCSANOW, &fram_new_settings);

if(nread == 1) {
fram_peek_character = ch;
return 1;
}
return 0;
}

int fram_readch()
{
char ch;

if(fram_peek_character != -1) {
ch = fram_peek_character;
fram_peek_character = -1;
return ch;
}
read(0,&ch,1);
return ch;
}
