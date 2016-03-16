//#define TEST_SMALL //omit inverse trig to fit normal UNO for testing

//LCD pins
#define LCD_D0 4
#define LCD_D1 5
#define LCD_D2 6
#define LCD_D3 7
#define LCD_RS 8
#define LCD_EN 9

//key columns (shared with LCD)
#define KEY_C0 LCD_D0
#define KEY_C1 LCD_D1
#define KEY_C2 LCD_D2
#define KEY_C3 LCD_D3
#define KEY_C4 LCD_RS

//key rows
#define KEY_R0 A0
#define KEY_R1 A1
#define KEY_R2 A2
#define KEY_R3 A3
#define KEY_R4 A4
#define KEY_R5 A5
#define KEY_R6 2  //must be INT0 to wake from sleep

