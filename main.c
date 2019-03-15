/*********************************************************************
					������ʹ��˵��
	�����ܽ��ܡ���
		1. �����������ж��ֹ���
		2. ģʽ1����������ʱ����ģʽ�������˰��¿�ʼ��ť��ѡ�ֿ�ʼ����
		3. ģʽ2������ʱ�䵹��ʱģʽ�������˰��¿�ʼ��ť��ѡ�ֿ�ʼ����
		4. ģʽ3���鿴ѡ�ֵ÷�
		5. ģʽ4����������ʱ��ʹ���ʱ��


	������˵������
		
		[S5]:�л�ģʽ��ť
		
		{ģʽ 1}��
			[S2]:������ʱ��
			[S3]:��ͣ��ʱ��
		
		{ģʽ 2}��
			[S2]:������ʱ��
			[S3]:�ش����
			[S4]:�ش���ȷ��ע�⣺��һ�μӷ�һ�Σ������ΰ��£�
		{ģʽ 3}��
			��Ч
		{ģʽ 4}��
			[S2]������ʱ��
			[S3]: ����ʱ��
			[S4]: �л������õ�ʱ��						  
**********************************************************************/

/*
**********************************************************************
*                         ͷ�ļ�����
**********************************************************************
*/
#include "lcd1602.h"
#include "timer.h"
#include "keyScan.h"


/*
**********************************************************************
*                         ���غ궨��
**********************************************************************
*/


/*
**********************************************************************
*                         ����ȫ�ֱ���
**********************************************************************
*/

char str_time[5] = "00.00";		   	//ʱ���ַ���
char int_str[]="0123456789";	   	//����ת�ַ���
int score[8]={10,10,10,10,10,10,10,10};		//�ɼ�����
int key = 0;						//�ĸ�����������
int hot_key	= 0;					//�ĸ��ȼ�������
u8 hot_key_temp;					//������ʱ���ȼ��洢
int mode = 1;						//ģʽѡ��
int flag_hot_scan = 0;				//hot_scan�����Ƿ����б�־
u8 time_set_flag=1;					//���������ĸ�ʱ�䣬1������ʱ�䣻-1���ش�ʱ��
u8 score_set_flag=1;				//�����������ַ�����1����ʼ��ֵ��2����Լӷ֣�3�����۷֣�4��Υ��۷�
u8 start_flag = 0;					//��ʼ�����־λ��Ĭ��û�п�ʼ
int time_fight = 3000;				//����ʱ��
int time_answer = 3000;				//����ʱ��
int score_initial = 10;				//��ʼ��ֵ
int score_right = 5;				//��Լӷ�
int score_wrong = 0;				//���۷�
int score_illegal = 3;				//Υ��۷�
/*
**********************************************************************
*                         ����ԭ������
**********************************************************************
*/
void display_time(u16 x, u16 y);
void change_mode();
void display_key(u16 x, u16 y);
void display_hot_key(u16 x, u16 y);
void display_score(void);

void MODE1();
void MODE2();
void MODE4();
void MODE5();
int set(u16 *object, u16 num);	
void is_it_illegal(u8 start_flag);


void delay10ms(void);

/*********************************************************************
* �� �� ��       : display_time
* ��������		 : ��ʾ����ʱ
* �����б�       : ��
* �������    	 : ��
*********************************************************************/
void display_time(u16 x, u16 y)
{
	str_time[4]=int_str[get_t1()%10];
	str_time[3]=int_str[get_t1()/10%10];
	str_time[1]=int_str[get_t1()/100%10];
	str_time[0]=int_str[get_t1()/1000];
	Lcd1602ShowStr(x,y,str_time); 
}
/*********************************************************************
* �� �� ��       : display_key
* ��������		 : ��ʾ�ĸ����������£����ڵ���չʾ��
* �����б�       : ��
* �������    	 : ��
*********************************************************************/
void display_key(u16 x, u16 y)
{
	key = control_key_scan();
	switch(key)
	{
		case 1:Lcd1602ShowStr(x,y,"key1");break;
		case 2:Lcd1602ShowStr(x,y,"key2");break;
		case 3:Lcd1602ShowStr(x,y,"key3");break;
		case 4:Lcd1602ShowStr(x,y,"key4");break;
		default:Lcd1602ShowStr(x,y,"None");break;
	}	 
}

/*********************************************************************
* �� �� ��       : display_hot_key
* ��������		 : ��ʾ�ĸ����������£����ڵ���չʾ��
* �����б�       : ��
* �������    	 : ��
*********************************************************************/
void display_hot_key(u16 x, u16 y)
{
	Lcd1602ShowStr(x,y,"user ");
	Lcd1602ShowChar(x+5,y,int_str[hot_key]);
	Lcd1602ShowStr(x+7,y,"bingo!!");	 
}


/*********************************************************************
* �� �� ��       : display_score
* ��������		 : ��ʾ�ĸ����������£����ڵ���չʾ��
* �����б�       : ��
* �������    	 : ��
*********************************************************************/
void display_score(void)
{
	u8 x=0,y=0,i=0;
	for(i=0;i<8;i++){
//		Lcd1602ShowChar(x++,y,int_str[i+1]);
		Lcd1602ShowStr(x++,y,"[");
		Lcd1602ShowChar(x++,y,int_str[score[i]/10]);
		Lcd1602ShowChar(x++,y,int_str[score[i]%10]);
		Lcd1602ShowStr(x++,y,"]");
		if (x>=16){
			x=0;
			y++;
		}
	}	 
}


/*********************************************************************
* �� �� ��       : change_mode
* ��������		 : �ı�ģʽ
* �����б�       : ��
* �������    	 : ��
*********************************************************************/
void change_mode()
{	
	if (control_key_scan() == 4)
	{
		while(control_key_scan()!=0);
		mode++;
		if (mode>5){mode=1;}
		clear_screen();	 //�����Ļ
		stop_timer0();	 //�رն�ʱ��
		if(mode==1)
			set_t1(time_fight);	 //����ʱ����
		else if(mode==2)
			set_t1(time_answer);	 //����ʱ����
		else if(mode==4)
			set_t1(time_fight);	 //����ʱ����
		flag_hot_scan = 0;	 //�ر��ȼ�ɨ��
		start_flag=0;		 //��ʼ�����־λ��0����ʾδ��ʼ����
			
	}	 
}


/*********************************************************************
* �� �� ��       : MODE1
* ��������		 : ����ģʽ
* �����б�       : ��
* �������    	 : ��
*********************************************************************/
void MODE1()
{
	Lcd1602ShowStr(0,0,"time:");	 //��ʾ����ʱ
	display_time(5,0);
	Lcd1602ShowStr(11,0,"MODE1");	 //��ʾģʽ
	is_it_illegal(start_flag);
	if (key==1){	//�����ʼ��������
		start_timer0();		 //������ʱ������ʼ����ʱ
		flag_hot_scan = 1;	 //�����ȼ�ɨ��
		start_flag=1;		 //��ʼ�����־λ��1����ʾ��ʼ����
	}
	if (key==2){	//�����ͣ��������
		stop_timer0();		 //�رն�ʱ������ͣ����ʱ
		flag_hot_scan = 0;	 //�ر��ȼ�ɨ��
		start_flag=0;		 //��ʼ�����־λ��0����ʾδ��ʼ����
	}
	if (key==3){
		clear_screen();	 	 //�����Ļ
	}
	
	hot_key_temp = hot_key_scan(flag_hot_scan);
	if (hot_key_temp){	 //���ɨ�赽�ȼ�������
		while(hot_key_scan(flag_hot_scan)!=0);
		start_timer1();	 //����������
		hot_key = hot_key_temp;
		flag_hot_scan = 0;				 //ֹͣ�ȼ�ɨ��
		stop_timer0();					 //ֹͣ��ʱ���ĵ���ʱ
		display_hot_key(1,1);			 //��ʾ����ɹ����û�
//		beep_flag=1;      				 //����������
//		beep_ring();
	}
	if (get_t1()==0){
		flag_hot_scan = 0;	 //�ر��ȼ�ɨ��
		Lcd1602ShowStr(2,1,"--Time Out--");	   //��ʾʱ�䵽��
//		beep_ring();
		start_timer1();	 //����������
	}
}


/*********************************************************************
* �� �� ��       : MODE2
* ��������		 : ����ģʽ
* �����б�       : ��
* �������    	 : ��
*********************************************************************/
void MODE2()
{
	Lcd1602ShowStr(0,0,"time:");	 //��ʾ����ʱ
	display_time(5,0);
	Lcd1602ShowStr(11,0,"MODE2");	 //��ʾģʽ
	if (key==1){	//�����ʼ��������
		start_timer0();		 //������ʱ������ʼ����ʱ
	}
	if (key==2){	//�������������
		while(control_key_scan()!=0);	//�ȴ�����̧��
		stop_timer0();		 //�رն�ʱ������ͣ����ʱ
		score[hot_key-1] -= score_wrong;
		Lcd1602ShowStr(0,1,"Your Are Wrong!");
	}
	if (key==3){	//�����Լ�������
		while(control_key_scan()!=0);	//�ȴ�����̧��
		stop_timer0();		 //�رն�ʱ������ͣ����ʱ
		score[hot_key-1] += score_right;	  //����˵ķ�����5
		Lcd1602ShowStr(0,1,"Your Are Right!");
		start_timer1();	 //����������
	}
	if (get_t1()==0){
		Lcd1602ShowStr(2,1,"--Time Out--");	   //��ʾʱ�䵽��
//		beep_ring();
		score[hot_key-1] -= score_wrong;
		start_timer1();	 //����������
	}
}


/*********************************************************************
* �� �� ��       : MODE4
* ��������		 : ʱ������ģʽ
* �����б�       : ��
* �������    	 : ��
*********************************************************************/
void MODE4()
{
	Lcd1602ShowStr(3,0,"Time Set");
	if(time_set_flag==1){
		set_t1(time_fight);
		Lcd1602ShowStr(0,1,"Fight  Time");
	}
	else{
		set_t1(time_answer);
		Lcd1602ShowStr(0,1,"Answer Time");
	}
	if(key == 3){								//key3�л�����ģʽ
		while(control_key_scan()!=0);
		time_set_flag = -time_set_flag;
	}
	else if(key == 1){							//key1��������ʱ��
		while(control_key_scan()!=0);
		if(time_set_flag==1){
			time_fight += 100;
			set_t1(time_fight);	
		}
		else{
			time_answer += 100;
			set_t1(time_answer);	
		}
	}
	else if(key == 2){						   //key2��������ʱ��
		while(control_key_scan()!=0);
		if(time_set_flag==1){
			if(time_fight>0){
				time_fight -= 100;
				set_t1(time_fight);
			}	
		}
		else{
			if(time_answer>0){
				time_answer -= 100;
				set_t1(time_answer);
			}	
		}	
	}
	display_time(11,1);
}


/*********************************************************************
* �� �� ��       : MODE5
* ��������		 : �Ӽ�������ģʽ
* �����б�       : ��
* �������    	 : ��
*********************************************************************/
void MODE5()
{
/*
u8 score_set_flag=1;				//�����������ַ�����1����ʼ��ֵ��2����Լӷ֣�3�����۷֣�4��Υ��۷�
int score_initial = 10;				//��ʼ��ֵ
int score_right = 5;				//��Լӷ�
int score_wrong = 0;				//���۷�
int score_illegal = 3;				//Υ��۷�
*/
	Lcd1602ShowStr(3,0,"Score Set");
	if (key==3){
		while(control_key_scan()!=0);
		score_set_flag++;
		if (score_set_flag>4){
			score_set_flag=1;
		}
	}
	switch(score_set_flag){
		case 1:{//��ʼ��ֵ
			Lcd1602ShowStr(0,1,"initial: ");
			if(set(&score_initial, 1)){
				score[0]=score_initial;
				score[1]=score_initial;
				score[2]=score_initial;
				score[3]=score_initial;
				score[4]=score_initial;
				score[5]=score_initial;
				score[6]=score_initial;
				score[7]=score_initial;
			}
			Lcd1602ShowChar(10,1,int_str[score_initial/10]);
			Lcd1602ShowChar(11,1,int_str[score_initial%10]);
			break;	
		}
		case 2:{//��Լӷ�
			Lcd1602ShowStr(0,1,"right: ");
			set(&score_right, 1);
			Lcd1602ShowChar(10,1,int_str[score_right/10]);
			Lcd1602ShowChar(11,1,int_str[score_right%10]);
			break;	
		}
		case 3:{//���۷�
			Lcd1602ShowStr(0,1,"wrong: ");
			set(&score_wrong, 1);
			Lcd1602ShowChar(10,1,int_str[score_wrong/10]);
			Lcd1602ShowChar(11,1,int_str[score_wrong%10]);
			break;	
		}
		case 4:{//Υ��۷�
			Lcd1602ShowStr(0,1,"illegal: ");
			set(&score_illegal, 1);
			Lcd1602ShowChar(10,1,int_str[score_illegal/10]);
			Lcd1602ShowChar(11,1,int_str[score_illegal%10]);
			break;	
		}
		default:break;
	}
}

/*********************************************************************
* �� �� ��       : set
* ��������		 : ����ģʽ
* �����б�       : Ҫ�Ӽ��Ĳ����ĵ�ַ; ����
* �������    	 : ���ظ��ĺ��ֵ
*********************************************************************/
int set(u16 *object, u16 num)
{
	if(key == 1){							//key1������
		while(control_key_scan()!=0);
		*object += num;
		return 1;
	}
	else if(key == 2){						   //key2������
		while(control_key_scan()!=0);
		if ((*object)>=0){
			*object -= num;
			return 1;
		}	
	}
	return 0;
}					   

/*********************************************************************
* �� �� ��       : is_it_illegal
* ��������		 : �ж��Ƿ�Υ��
* �����б�       : start_flag: �Ƿ�ʼ����ı�־
* �������    	 : ��
*********************************************************************/
void is_it_illegal(u8 start_flag)
{
	if (start_flag==0){
		hot_key_temp=hot_key_scan(1);
		if (hot_key_temp){
			while(hot_key_scan(1)!=0);
			Lcd1602ShowStr(1,1,"user ");
			Lcd1602ShowChar(6,1,int_str[hot_key_temp]);
			Lcd1602ShowStr(8,1,"illegal");
			stop_timer0();		 //�رն�ʱ������ͣ����ʱ
			flag_hot_scan = 0;	 //�ر��ȼ�ɨ��
			start_flag=0;		 //��ʼ�����־λ��0����ʾδ��ʼ����
			start_timer1();	 	 //����������
			score[hot_key_temp-1] -= score_illegal;	  //��ǰ������˵ķ�����1
		}
	}
}



/*********************************************************************
* �� �� ��       : main
* ��������		 : ������
* �����б�       : ��
* �������    	 : ��
*********************************************************************/
void main(void)
{
	Lcd1602Init();
	Timer0Init();
	Timer1Init();
	while(1){
		change_mode();
		key = control_key_scan();
		
		switch(mode){			//ģʽ����
			case 1:{			//�����ģʽ1�Ļ�
				MODE1();	 
				break;
			}
			case 2:{
				MODE2();
				break;
			}
			case 3:{
				display_score();					 //��ʾ����
				break;
			}
			case 4:{
				MODE4();					 
				break;
			}
			case 5:{
				MODE5();
				break;
			}
		}
	}
}










