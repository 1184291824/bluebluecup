/*********************************************************************
					抢答器使用说明
	【功能介绍】：
		1. 本抢答器具有多种功能
		2. 模式1：基本倒计时抢答模式，主持人按下开始按钮后，选手开始抢答
		3. 模式2：答题时间倒计时模式，主持人按下开始按钮后，选手开始作答
		4. 模式3：查看选手得分
		5. 模式4：设置抢答时间和答题时间


	【按键说明】：
		
		[S5]:切换模式按钮
		
		{模式 1}：
			[S2]:开启定时器
			[S3]:暂停定时器
		
		{模式 2}：
			[S2]:开启定时器
			[S3]:回答错误
			[S4]:回答正确（注意：按一次加分一次，请勿多次按下）
		{模式 3}：
			无效
		{模式 4}：
			[S2]：增加时间
			[S3]: 减少时间
			[S4]: 切换所设置的时间						  
**********************************************************************/

/*
**********************************************************************
*                         头文件包含
**********************************************************************
*/
#include "lcd1602.h"
#include "timer.h"
#include "keyScan.h"


/*
**********************************************************************
*                         本地宏定义
**********************************************************************
*/


/*
**********************************************************************
*                         本地全局变量
**********************************************************************
*/

char str_time[5] = "00.00";		   	//时间字符串
char int_str[]="0123456789";	   	//数字转字符串
int score[8]={10,10,10,10,10,10,10,10};		//成绩数组
int key = 0;						//哪个按键被按下
int hot_key	= 0;					//哪个热键被按下
u8 hot_key_temp;					//用于临时的热键存储
int mode = 1;						//模式选择，
int flag_hot_scan = 0;				//hot_scan函数是否运行标志
u8 time_set_flag=1;					//调整的是哪个时间，1：抢答时间；-1：回答时间
u8 score_set_flag=1;				//调整的是哪种分数，1：初始分值；2：答对加分；3：打错扣分；4：违规扣分
u8 start_flag = 0;					//开始抢答标志位，默认没有开始
int time_fight = 3000;				//抢答时间
int time_answer = 3000;				//答题时间
int score_initial = 10;				//初始分值
int score_right = 5;				//答对加分
int score_wrong = 0;				//打错扣分
int score_illegal = 3;				//违规扣分
/*
**********************************************************************
*                         函数原型声明
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
* 函 数 名       : display_time
* 函数功能		 : 显示倒计时
* 参数列表       : 无
* 函数输出    	 : 无
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
* 函 数 名       : display_key
* 函数功能		 : 显示哪个按键被按下（用于调试展示）
* 参数列表       : 无
* 函数输出    	 : 无
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
* 函 数 名       : display_hot_key
* 函数功能		 : 显示哪个按键被按下（用于调试展示）
* 参数列表       : 无
* 函数输出    	 : 无
*********************************************************************/
void display_hot_key(u16 x, u16 y)
{
	Lcd1602ShowStr(x,y,"user ");
	Lcd1602ShowChar(x+5,y,int_str[hot_key]);
	Lcd1602ShowStr(x+7,y,"bingo!!");	 
}


/*********************************************************************
* 函 数 名       : display_score
* 函数功能		 : 显示哪个按键被按下（用于调试展示）
* 参数列表       : 无
* 函数输出    	 : 无
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
* 函 数 名       : change_mode
* 函数功能		 : 改变模式
* 参数列表       : 无
* 函数输出    	 : 无
*********************************************************************/
void change_mode()
{	
	if (control_key_scan() == 4)
	{
		while(control_key_scan()!=0);
		mode++;
		if (mode>5){mode=1;}
		clear_screen();	 //清空屏幕
		stop_timer0();	 //关闭定时器
		if(mode==1)
			set_t1(time_fight);	 //倒计时归零
		else if(mode==2)
			set_t1(time_answer);	 //倒计时归零
		else if(mode==4)
			set_t1(time_fight);	 //倒计时归零
		flag_hot_scan = 0;	 //关闭热键扫描
		start_flag=0;		 //开始抢答标志位置0，表示未开始抢答
			
	}	 
}


/*********************************************************************
* 函 数 名       : MODE1
* 函数功能		 : 抢答模式
* 参数列表       : 无
* 函数输出    	 : 无
*********************************************************************/
void MODE1()
{
	Lcd1602ShowStr(0,0,"time:");	 //显示倒计时
	display_time(5,0);
	Lcd1602ShowStr(11,0,"MODE1");	 //显示模式
	is_it_illegal(start_flag);
	if (key==1){	//如果开始键被按下
		start_timer0();		 //开启定时器，开始倒计时
		flag_hot_scan = 1;	 //开启热键扫描
		start_flag=1;		 //开始抢答标志位置1，表示开始抢答
	}
	if (key==2){	//如果暂停键被按下
		stop_timer0();		 //关闭定时器，暂停倒计时
		flag_hot_scan = 0;	 //关闭热键扫描
		start_flag=0;		 //开始抢答标志位置0，表示未开始抢答
	}
	if (key==3){
		clear_screen();	 	 //清空屏幕
	}
	
	hot_key_temp = hot_key_scan(flag_hot_scan);
	if (hot_key_temp){	 //如果扫描到热键被按下
		while(hot_key_scan(flag_hot_scan)!=0);
		start_timer1();	 //启动蜂鸣器
		hot_key = hot_key_temp;
		flag_hot_scan = 0;				 //停止热键扫描
		stop_timer0();					 //停止定时器的倒计时
		display_hot_key(1,1);			 //显示抢答成功的用户
//		beep_flag=1;      				 //蜂鸣器触发
//		beep_ring();
	}
	if (get_t1()==0){
		flag_hot_scan = 0;	 //关闭热键扫描
		Lcd1602ShowStr(2,1,"--Time Out--");	   //显示时间到了
//		beep_ring();
		start_timer1();	 //启动蜂鸣器
	}
}


/*********************************************************************
* 函 数 名       : MODE2
* 函数功能		 : 答题模式
* 参数列表       : 无
* 函数输出    	 : 无
*********************************************************************/
void MODE2()
{
	Lcd1602ShowStr(0,0,"time:");	 //显示倒计时
	display_time(5,0);
	Lcd1602ShowStr(11,0,"MODE2");	 //显示模式
	if (key==1){	//如果开始键被按下
		start_timer0();		 //开启定时器，开始倒计时
	}
	if (key==2){	//如果答错键被按下
		while(control_key_scan()!=0);	//等待按键抬起
		stop_timer0();		 //关闭定时器，暂停倒计时
		score[hot_key-1] -= score_wrong;
		Lcd1602ShowStr(0,1,"Your Are Wrong!");
	}
	if (key==3){	//如果答对键被按下
		while(control_key_scan()!=0);	//等待按键抬起
		stop_timer0();		 //关闭定时器，暂停倒计时
		score[hot_key-1] += score_right;	  //答对人的分数加5
		Lcd1602ShowStr(0,1,"Your Are Right!");
		start_timer1();	 //启动蜂鸣器
	}
	if (get_t1()==0){
		Lcd1602ShowStr(2,1,"--Time Out--");	   //显示时间到了
//		beep_ring();
		score[hot_key-1] -= score_wrong;
		start_timer1();	 //启动蜂鸣器
	}
}


/*********************************************************************
* 函 数 名       : MODE4
* 函数功能		 : 时间设置模式
* 参数列表       : 无
* 函数输出    	 : 无
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
	if(key == 3){								//key3切换调整模式
		while(control_key_scan()!=0);
		time_set_flag = -time_set_flag;
	}
	else if(key == 1){							//key1用来增加时间
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
	else if(key == 2){						   //key2用来减少时间
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
* 函 数 名       : MODE5
* 函数功能		 : 加减分设置模式
* 参数列表       : 无
* 函数输出    	 : 无
*********************************************************************/
void MODE5()
{
/*
u8 score_set_flag=1;				//调整的是哪种分数，1：初始分值；2：答对加分；3：打错扣分；4：违规扣分
int score_initial = 10;				//初始分值
int score_right = 5;				//答对加分
int score_wrong = 0;				//打错扣分
int score_illegal = 3;				//违规扣分
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
		case 1:{//初始分值
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
		case 2:{//答对加分
			Lcd1602ShowStr(0,1,"right: ");
			set(&score_right, 1);
			Lcd1602ShowChar(10,1,int_str[score_right/10]);
			Lcd1602ShowChar(11,1,int_str[score_right%10]);
			break;	
		}
		case 3:{//打错扣分
			Lcd1602ShowStr(0,1,"wrong: ");
			set(&score_wrong, 1);
			Lcd1602ShowChar(10,1,int_str[score_wrong/10]);
			Lcd1602ShowChar(11,1,int_str[score_wrong%10]);
			break;	
		}
		case 4:{//违规扣分
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
* 函 数 名       : set
* 函数功能		 : 设置模式
* 参数列表       : 要加减的参数的地址; 增量
* 函数输出    	 : 返回更改后的值
*********************************************************************/
int set(u16 *object, u16 num)
{
	if(key == 1){							//key1用来增
		while(control_key_scan()!=0);
		*object += num;
		return 1;
	}
	else if(key == 2){						   //key2用来减
		while(control_key_scan()!=0);
		if ((*object)>=0){
			*object -= num;
			return 1;
		}	
	}
	return 0;
}					   

/*********************************************************************
* 函 数 名       : is_it_illegal
* 函数功能		 : 判定是否违规
* 参数列表       : start_flag: 是否开始答题的标志
* 函数输出    	 : 无
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
			stop_timer0();		 //关闭定时器，暂停倒计时
			flag_hot_scan = 0;	 //关闭热键扫描
			start_flag=0;		 //开始抢答标志位置0，表示未开始抢答
			start_timer1();	 	 //启动蜂鸣器
			score[hot_key_temp-1] -= score_illegal;	  //提前抢答的人的分数减1
		}
	}
}



/*********************************************************************
* 函 数 名       : main
* 函数功能		 : 主函数
* 参数列表       : 无
* 函数输出    	 : 无
*********************************************************************/
void main(void)
{
	Lcd1602Init();
	Timer0Init();
	Timer1Init();
	while(1){
		change_mode();
		key = control_key_scan();
		
		switch(mode){			//模式查验
			case 1:{			//如果是模式1的话
				MODE1();	 
				break;
			}
			case 2:{
				MODE2();
				break;
			}
			case 3:{
				display_score();					 //显示分数
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










