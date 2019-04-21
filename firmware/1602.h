#ifndef __1602_H__
#define __1602_H__

#define uchar unsigned char
#define uint  unsigned int
//*****************�����*********
#define clear_screen 	0x01//ָ��1,����Ļ
#define cursor_return 	0x02//��긴λ����귵�ص�ַ00H

//*****************ָ��3���������ģʽ����******
#define	cursor_right	0x06//����ʱ���������
#define cursor_left		0x04//����ʱ���������
#define fulltext_movon	0x05//��д����ʱ������ƽ��
#define fulltext_movoff	0x04//��д����ʱ�����治�ƶ�

//*****************ָ��4����ʾ���ؿ���**********
#define display_on		0x0c//������ʾ��
#define display_off		0x08//������ʾ��
#define cursor_on		0x0a//��꿪
#define cursor_off		0x08//����
#define cursor_flashon	0x09//�����˸
#define cursor_flashoff	0x08//��겻��˸

//***************ָ��5�������ַ���λ***********
//���ܣ���ꡢ�����ƶ�����Ӱ��DDRAM
#define cursor_mov		0x10//�ƶ����
#define char_mov		0x18//�ƶ�����
#define left			0x10//����
#define right			0x14//����

//***************ָ��6���ù���***************
#define eight_bus		0x30//8λ����
#define four_bus		0x20//4λ����
#define oneline_on		0x20//������ʾ
#define twoline_on		0x28//˫����ʾ
#define fxs				0x20//5x7�ĵ����ַ�
#define fxt				0x24//5x10�ĵ����ַ�

//***************ָ��7�����ַ������洢��ַ***************
//���ܣ�����CGRAM��ַ��A5~A0=0~3FH
#define char_add		0x40//�ַ���������RAM��ַ����

//***************ָ��8�������ݴ洢����ַ***************
//1~8ָ��  RS=0��RW=0
#define	dischar_add		0x80//��ʾ���ݴ洢����ַ

//***************ָ��9����æ�źź͹���ַ***************
//���ܣ���æBFֵ�͵�ַ������ACֵ��RS=0��RW=1
#define busy			0x80//æ
#define free			0x00//׼����
//**************ָ��10��д����*****************
//RS=1,RW=0

//**************ָ��11��������*****************
//RS=1,RW=1

//unsigned int lastCap=0;
//unsigned int period=0;
//float fx;
void LCD_write_command(unsigned char command);
void LCD_write_data(unsigned char data);
void lcd_test(void);
void lcd1602_puts(const char *s);
#endif
