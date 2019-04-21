#ifndef __1602_H__
#define __1602_H__

#define uchar unsigned char
#define uint  unsigned int
//*****************命令定义*********
#define clear_screen 	0x01//指令1,清屏幕
#define cursor_return 	0x02//光标复位，光标返回地址00H

//*****************指令3：光标输入模式设置******
#define	cursor_right	0x06//输入时，光标右移
#define cursor_left		0x04//输入时，光标左移
#define fulltext_movon	0x05//读写操作时，画面平移
#define fulltext_movoff	0x04//读写操作时，画面不移动

//*****************指令4：显示开关控制**********
#define display_on		0x0c//整体显示开
#define display_off		0x08//整体显示关
#define cursor_on		0x0a//光标开
#define cursor_off		0x08//光标关
#define cursor_flashon	0x09//光标闪烁
#define cursor_flashoff	0x08//光标不闪烁

//***************指令5：光标或字符移位***********
//功能：光标、画面移动，不影响DDRAM
#define cursor_mov		0x10//移动光标
#define char_mov		0x18//移动文字
#define left			0x10//左移
#define right			0x14//右移

//***************指令6：置功能***************
#define eight_bus		0x30//8位总线
#define four_bus		0x20//4位总线
#define oneline_on		0x20//单行显示
#define twoline_on		0x28//双行显示
#define fxs				0x20//5x7的点阵字符
#define fxt				0x24//5x10的点整字符

//***************指令7：置字符发生存储地址***************
//功能：设置CGRAM地址。A5~A0=0~3FH
#define char_add		0x40//字符发生器的RAM地址设置

//***************指令8：置数据存储器地址***************
//1~8指令  RS=0，RW=0
#define	dischar_add		0x80//显示数据存储器地址

//***************指令9：读忙信号和光标地址***************
//功能：读忙BF值和地址计数器AC值，RS=0，RW=1
#define busy			0x80//忙
#define free			0x00//准备好
//**************指令10：写数据*****************
//RS=1,RW=0

//**************指令11：读数据*****************
//RS=1,RW=1

//unsigned int lastCap=0;
//unsigned int period=0;
//float fx;
void LCD_write_command(unsigned char command);
void LCD_write_data(unsigned char data);
void lcd_test(void);
void lcd1602_puts(const char *s);
#endif
