void anim();
void setPixel(int x, int y, int color);
void fillBK(int color, int x, int y, int w,int h);
void sleep(int x);
void spin(int x,int y,int Np,int tackt, int w);
void eye(int pos, int x, int y,int w);
void RKN_tyan(int x,int y,int w);
void answer(int x, int y, int w,int flag);
void load(int sector, int offset,int amount);
void LoadFromDisk(int offset,int sectorID);
void manyload(int sector, int offset,int amount);
void disk_goint();
void pict(int x,int y,int start,int NoW,int w);
void mina(int x,int y,int w, int* colors);
void ospict(int x,int y,int w);
void fillword(int sym,int x, int y, int dsize,int color);
int GetKey(int key);
void bukvi(int x,int y,int w, int kol, char * clova,int f,int notster,int white);
void RKNanswer(char* answer,int *helpF, int *timerON, int *timerONanswer,int *sens,int *sensAnswer,int *i,int *j);
void prog();
void printCI(int ci,int x,int y,int w,int color);
int get_amount_memory();
int get_disk_type();
int basic();
int ext();
long long int get_cpuid_flags();
int get_MMX_support();
int get_SIMD_support();
int get_PSE_support();
int sek();
int min();
int hour();
int day();
int moun();
int year();
int year2();
int get_video();
int strcmp(char * str1,char* str2);
void time_and_date(int pos);
int exits(int pict);
int _start()
{
	asm("mov eax,0xB");//говорю часам быть адекватными
	asm("out 0x70, al");
	asm("in al, 0x71");
	asm("or al,0x04");
	asm("out 0x71,al");
	anim();//заставка
	prog();// программа
	
}
void prog()
{
	int helpF=0,timerON=0,timerONanswer=0, sens=0,sensAnswer=0;// флаги для функции отвечающей на текстовые команды
	int key,r=44,_i=0,i=5,j=0,t,last_t=127,f=1,BIGBUTT=0,kolvo_bukov=0;// переменные для приёма буков
	load(55,0x15000,7);// загрузка до 128 секторов за раз первое с какого сектора, второе на какой адрес, третье сколько
	load(51,0x20000,4);
	fillBK(0xffffff,0,0,1024,1280);// бесполезная фигня для интерфейса
	fillBK(0x2A2054,0,600,1024,5);
	fillBK(0x2A2054,220,600,5,680);
	pict(960,670,5088,0,2);
	RKN_tyan(0,1000,8);
	eye(2,0,1000,8);
	char pred[]="нажми большую кнопку";// полезный текст
	char clo[]="ввод";
	bukvi(0,0,5,4,clo,0,0,0);// вывод ввода и других слов 
	char answer[35];// область для ввода команд
	for(int i2=0;i2<35;i2++) answer[i2]=0;
	char scCodes[]={0x0B,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x21,0x33,0x20,0x16,0x26,
	0x14,0x27,0x19,0x30,0x10,0x13,0x25,0x2F,0x15,0x24,0x22,0x23,0x2E,0x31,0x12,0x1E,0x1A,0x11,
	0x2D,0x17,0x18,0x1B,0x1F,0x32,0x28,0x34,0x2C,0x39,0x35};// скан коды для русской клавы, не все
	while(f)
	{
		t=GetKey(t);//ловим  нажатую кнопку
		while(t==last_t) //предотвращение залипания клавиши
		{
			if(timerON) time_and_date(1);// тиканье часов внизу
			t=GetKey(t);
		}
		key=0;
		for(r=0;r<44;r++)// ищем нужную букву
		{	
			if (scCodes[r]==t)
			{
				if ((_i==1)&&(i==20))
				{
					if((t!=28)&&(!BIGBUTT))// проверка что не заполнили всю строку ввода
					{
						BIGBUTT=1;
						bukvi(630,20, 5,20, pred,1,0,0);
						continue;			
					}else if(BIGBUTT) continue;
				}	
				key = 1;
				break;
			}
		}
		last_t=t;
		if (key)
		{
			if(i==20)
			{
				j++;
				i=0;
				_i++;
			}
			// записывание в строку ввода введённый символ как char
			if ((r>=0)&&(r<=9)) answer[i-5+_i*20]=r+'0';
			if ((r>=10)&&(r<=41)) answer[i-5+_i*20]=r+'а'-10;
			if (r==42) answer[i-5+_i*20]=' ';
			if (r==43) answer[i-5+_i*20]='.';
			fillword(r,j*40,i*30, 5, 0x2A2054);
			i++;
			kolvo_bukov++;
		}
		else
		{
			if (t==28)
			{
				i=5;// если нажат энтер переход на новую строку обнуление
				if(_i!=2) j++;
				if (j==25)
				{
					fillBK(0xffffff,0,0,1024,600);// если все строки заняты чистим рабочую поверхность
					bukvi(0,0,5,4,clo,0,0,0);
					i=5;j=0;
				}
				
				RKNanswer(answer,&helpF,&timerON,&timerONanswer,&sens,&sensAnswer,&i,&j);// вызов функции  получения ответа на команду
				bukvi(0,j*40, 5,4, clo,0,0,0);// снова ввод
				for(int i2=0;i2<kolvo_bukov;i2++) answer[i2]=0;// обнуляем используемую строку
				BIGBUTT=0;
				kolvo_bukov=0;
				_i=0;
			}
			if (t==0x0E)// стираение(кривое) лучше всего использовать только на одной строке
			{
				if(i==5)continue;
				if (i) i--;
				else
				{
					if (j)
					{
						j--;
						i=20;
					}
				}
				fillword(60,j*40,i*30, 5, 0xffffff);
			}

		}	
	}	
}

void RKNanswer(char* answer, int *helpF,int *timerON, int *timerONanswer,int *sens,int *sensAnswer, int *i,int *j)
{	
	//название команд
	char help[]="помощь";
	char size_disk[]="размер диска";
	char type_disk[]="тип диска";
	char video_type[]="видео режим";
	char data_and_time[]="дата и время";
	char basic_memory[]="базовая память";
	char expanded_memory[]="расширенная память";
	char mmx[]="поддержка ммх";
	char pse[]="поддержка псе";
	char simd[]="поддержка симд";
	char cls[]="очистить экран";
	char guess[]="угадать число";
	char exit_par[]="параметр выхода";
	char exit[]="127.0.0.1";
	char picture[]="картинку";
	//текст вне облака
	char Yes[]="да";char No[]="нет";
	//ответ помощь
	char helpT1[]="размер диска\nтип диска\nвидео режим\nдата и время";//\nрасширеная память\nподдержка ммх\nподдержка псе\nподдержка симд\n";
	char helpT2[]="базовая память\nрасширенная память\nподдержка ммх";
	char helpT3[]="поддержка псе\nподдержка симд\nочистить экран";
	char helpT4[]="параметр выхода\n127.0.0.1\nкартинку";
	char clo[]="ввод";//ввод
	// завершение работы
	char ex[]="за тобой\nуже выехали!";
	//ответы РКН
	//ответ помощь
	char RKNHT1[]="ты что дурак?! внизу-же написаны все команды!";
	char RKNHT2[]="тебе же русским языком сказали внизу  бака!!";
	char RKNHT3[]="нет! ну ты полный идиот! бака! бака! бака! бака!";
	//ответ дата и время
	char RKNTD1[]="московское  время:";
	char RKNTD2[]="желаете постоянно видетьвремя, да   или нет?";
	char RKNTD3[]="сесть       хочешь!?!";
	//ответ поддержка simd
	char RKNSIMD1[]="конечно он поддерживает"; 
	char RKNSIMD10[]="одиночный поток команд, множественный поток данных";
	char RKNSIMD2[]="фу! что за дрянной компьютер. не поддерживает!";
	// ответ видео режим
	char RKNVR1[]="видео режим:";
	char RKNVR2[]="кзс, 1280*1024 пикселя, по 8 бит на цвет"; 
	//ответ поддержка mmx
	char RKNMMX1[]="конечно он поддерживает мультимедийные расширения"; 
	char RKNMMX2[]="фу! что за дрянной компьютер. не поддерживает!";
	//ответ поддержка pse
	char RKNPSE1[]="конечно он поддерживает расширенный размер страницы"; 
	char RKNPSE2[]="фу! что за дрянной компьютер. не поддерживает!";
	// ответ размер диска
	char RKNSD1[]="размер этого диска:";
	char RKNSD2[]="гигабайт";
	// ответ тип диска
	char RKNTDi1[]="о чем ты говоришь?";
	char RKNTDi2[]="у тебя висящий, без возможности заменить ха-ха";
	char RKNTDi3[]="ты можешь заменить свой висящий";
	char RKNTDi4[]="упорный диск";
	// ответ базовая память
	char RKNBM1[]="размер базовой памяти:";
	char RKNBM2[]="кб";
	// ответ расширеная память
	char RKNEM1[]="размер      расширенной памяти:";
	char RKNEM2[]="мб";
	// ответ на параметр выхода
	char RKNPE[]="хотите увидеть нецензурную картинку выхода? да или нет";
	if(*sensAnswer)// проверка флага  на выбор параметра выхода
	{
		if(strcmp(answer,Yes)){fillBK(0xFFFFFF,0,610,160,392);*sens=1;*sensAnswer=0;return;}
		else if(strcmp(answer,No)){fillBK(0xFFFFFF,0,610,160,392);*sens=0;*sensAnswer=0;return;}
		fillBK(0xFFFFFF,20,630,80,360);
		bukvi(630,20, 5,21, RKNTD3,1,1,0);
		*sensAnswer=0;
		return;
	}
	if (*timerONanswer)// проверка флага  на выбор отображения времени
	{
		if(strcmp(answer,Yes))
		{fillBK(0xFFFFFF,0,610,160,392);*timerON=1;*timerONanswer=0;return;}
		else if(strcmp(answer,No)){fillBK(0xFFFFFF,0,610,160,392);fillBK(0xFFFFFF,900,630,160,392);*timerON=0;*timerONanswer=0;return;}
		fillBK(0xFFFFFF,20,630,80,360);
		bukvi(630,20, 5,21, RKNTD3,1,1,0);
		*timerONanswer=0;
		return;
	}
	if(strcmp(answer,help))// вывод записей помощи
	{
		switch(*helpF)
		{
			case 0:
			{
				bukvi(630,20, 5,45, RKNHT1,1,1,0);
				break;
			}
			case 1:
			{
				bukvi(630,20, 5,44, RKNHT2,1,1,0);
				break;
			}
			default:
				bukvi(630,20, 5,48, RKNHT3,1,1,0);
		}
		*helpF=*(int*)helpF+1;
		fillBK(0xffffff,226,606,799,674);
		pict(960,670,5088,0,2);
		bukvi(630,250, 5,47, helpT1,0,0,0);// уточнение, почемуто не хочет выводить строку больше 60 символов
		bukvi(630,410, 5,47, helpT2,0,0,0);
		bukvi(630,530, 5,43, helpT3,0,0,0);
		bukvi(630,650, 5,34, helpT4,0,0,0);
	}else if(strcmp(answer,data_and_time))// дата и время
	{
		bukvi(630,20, 5,18, RKNTD1,1,0,1);
		time_and_date(2);
		bukvi(630,20, 5,44, RKNTD2,1,0,0);
		*timerONanswer=1;
	}else if(strcmp(answer,simd))// simd
	{
		if(get_SIMD_support()){bukvi(630,20, 5,23, RKNSIMD1,1,0,1);bukvi(630,20, 5,50, RKNSIMD10,1,1,0); return;}
		bukvi(630,20, 5,46, RKNSIMD2,1,1,0);
	}else if(strcmp(answer,video_type))// видео режим, кстати у меня здесь 1280*1024
	{
		bukvi(630,20, 5,12, RKNVR1,1,0,0);
		printCI(get_video(),60,630,5,0x2A2054);
		sleep(20000);
		fillBK(0xFFFFFF,20,630,80,360);
		bukvi(630,20, 5,40, RKNVR2,1,1,0);
	}else if(strcmp(answer,mmx))//mmx
	{
		if(get_MMX_support()){bukvi(630,20, 5,49, RKNMMX1,1,1,0); return;}
		bukvi(630,20, 5,46, RKNMMX2,1,1,0);
	}else if(strcmp(answer,pse))// pse
	{
		if(get_PSE_support()){bukvi(630,20, 5,51, RKNPSE1,1,1,0); return;}
		bukvi(630,20, 5,46, RKNPSE2,1,1,0);
	}else if(strcmp(answer,size_disk))// размер главного диска, работает тоже немного криво, выдаёт в кб
	{
		// но если делить на 1024 вообще ни то число что показывает наша ос, но если делить на 1000 то что то похожее выдаёт разница всего гигобай, и это с округлением в меньшую сторону
		int a=get_amount_memory();
		a/=1000000;
		bukvi(630,20, 5,19,RKNSD1,1,0,1);
		printCI(a,20,630,5,0x2A2054);
		bukvi(630,60, 5,8,RKNSD2,0,1,0);
	}else if(strcmp(answer,type_disk))// тип диска, сложная херня но по логике всегда должна возвращать тройку и говорить что это хард
	{
		switch(get_disk_type())
		{
			case 1:// обычно это выдаётся на все при использовании прерывания что используетя для типа диска, но типо чтобы узнать основного диска нужно писать 0x80 и тогда будет 3
			{
				bukvi(630,20, 5,46, RKNTDi2,1,1,0);
				break;
			}
			case 2:
			{
				bukvi(630,20, 5,31, RKNTDi3,1,1,0);
				break;
			}
			case 3:
			{
				bukvi(630,20, 5,12, RKNTDi4,1,1,0);
				break;
			}
			default:
				bukvi(630,20, 5,18, RKNTDi1,1,1,0);
		}
	}else if(strcmp(answer,basic_memory))// наша лаба по архитектуре номер 7
	{
		int a=basic();
		bukvi(630,20, 5,22, RKNBM1,1,0,1);
		printCI(a,20,630,5,0x2A2054);
		bukvi(750,20, 5,2, RKNBM2,0,1,0);
	}else if(strcmp(answer,expanded_memory))// наша лаба по архитектуре номер 7
	{
		int a=ext();
		a/=1024;
		a++;
		bukvi(630,20, 5,31, RKNEM1,1,0,0);
		printCI(a,60,630,5,0x2A2054);
		bukvi(720,60, 5,2, RKNEM2,0,1,0);
	} else if(strcmp(answer,cls))// чистка экрана
	{
		fillBK(0xffffff,0,0,1024,600);
		bukvi(0,0,5,4,clo,0,0,0);
		*i=5;*j=0;
	} else if(strcmp(answer,exit))//выход из проги
	{
		int answer;
		answer=exits(*sens);// функция выхода
		if(answer)// если говорим что не уходим то просто занова отрисовываем весь экран, как будто только включили
		{
			fillBK(0xffffff,0,0,1024,1280);
			fillBK(0x2A2054,0,600,1024,5);
			fillBK(0x2A2054,220,600,5,680);
			pict(960,670,5088,0,2);
			RKN_tyan(0,1000,8);
			eye(2,0,1000,8);
			bukvi(0,0,5,4,clo,0,0,0);
			*i=5;*j=0;
		}	
		else// пишим фразу и зависаем
		{
			fillBK(0xffffff,0,0,1024,1280);
			bukvi(200,350,12,21,ex,0,0,0);
			while(1);
			
		}	
	} else if(strcmp(answer,exit_par))// параметры для выхода
	{
		bukvi(630,20, 5,54, RKNPE,1,0,0);
		*sensAnswer=1;
	}else if(strcmp(answer,picture))// просим картинку, случайную из 10, если она "пошленькая" ркн стирает её
	{
		int picturelist[]={16431,18483,19972,23032,24383,25837,28368,30188,31539,32886};
		int a=sek();
		fillBK(0xffffff,226,606,799,674);
		pict(606,226,picturelist[a%10],0,1);
		if((a%10==4)||(a%10==6))
		{
			for (int i=0;i<3;i++)
			{
				int a[8]={0,0x8697B3,0x2A2054,0xCFDFEC,0x7FA0EF,0xFFFFFF,0xBE9897-(0x003232*i)+(0x100000*i),0xF8E7DF-(0x006464*i)};
				mina(0,1000,8,a);
				sleep(50000);
			}
			fillBK(0xffffff,226,606,799,674);
			pict(960,670,5088,0,2);
			RKN_tyan(0,1000,8);
			eye(2,0,1000,8);
		}
	}
	
}
int exits(int picts)// выход
{
	char exitT[]="вы действительно хотите выйти?";
	char yes[]="да";
	char no[]="нет";
	int end[]={11630,13829};
	int f=1,answer=1,o=1,t;
	fillBK(0xffffff,0,0,1024,1280);
	pict(270,0,end[picts],0,1);
	bukvi(10,620,7,30,exitT,0,0,0);
	bukvi(300,750,9,2,yes,0,0,0);
	bukvi(800,750,9,3,no,0,0,0);	
	while(o)// выход работает на стрелкаХ подчёркивая твой ответ
		{
			f=1;
			if (answer) {fillBK(0xFFFFFF,830,300,10,108);fillBK(0x2A2054,830,800,10,162);}
			else {fillBK(0xFFFFFF,830,800,10,162);fillBK(0x2A2054,830,300,10,108);}
			while(f)
				{
					t=GetKey(t);
					if (t==0x4B) 
						{
							answer=0;
							f=0;
						}
					if (t==0x4D) 
						{
							answer=1;
							f=0;
						}
					if(t==0x1C) 
						{
							o=0;
							f=0;
						}
				}
		
			}
	return answer;
	
}
void time_and_date(int pos)// кривая функция со временем, в зависимости от ситуации либо рисует время в разговорнике ркн
{// либо внизу экрана
	int p;
	char timedate[23];
	int a=sek(),b=min(),c=hour(),d=day(),e=moun(),f=year2(),g=year();// секунды минуты часы день месяц первая часть года из 4х цифр вторая часть
	if(c+3>=24){c=(c+3)%24;d++;}// условие перехода на новые сутки, ибо даётся время по гринвичу
	else c+=3;// так-то бесполезный твист, тупо к с напиши +3
	if(c<10)
	{
		timedate[0]='0';
		timedate[1]=c+'0';
	}
	else{
		timedate[1]=c%10+'0';
		timedate[0]=c/10+'0';
	}
	timedate[2]=':';
	if(b<10)
	{
		timedate[3]='0';
		timedate[4]=b+'0';
	}
	else{
		timedate[4]=b%10+'0';
		timedate[3]=b/10+'0';
	}
	timedate[5]=':';
	if(a<10)
	{
		timedate[6]='0';
		timedate[7]=a+'0';
	}
	else{
		timedate[7]=a%10+'0';
		timedate[6]=a/10+'0';
	}
	if (pos==1) {p=9;timedate[8]='\n';}
	if (pos==2) {p=12;timedate[8]=' ';timedate[9]=' ';timedate[10]=' ';timedate[11]=' ';timedate[12]=' ';}
	if(d<10)
	{
		timedate[p]='0';
		timedate[p+1]=d+'0';
	}
	else{
		timedate[p+1]=d%10+'0';
		timedate[p]=d/10+'0';
	}
	timedate[p+2]='.';
	if(e<10)
	{
		timedate[p+3]='0';
		timedate[p+4]=e+'0';
	}
	else{
		timedate[p+4]=e%10+'0';
		timedate[p+3]=e/10+'0';
	}
	timedate[p+5]='.';
	if(f<10)
	{
		timedate[p+6]='0';
		timedate[p+7]=f+'0';
	}
	else{
		timedate[p+7]=f%10+'0';
		timedate[p+6]=f/10+'0';
	}
	if(g<10)
	{
		timedate[p+8]='0';
		timedate[p+9]=g+'0';
	}
	else{
		timedate[p+9]=g%10+'0';
		timedate[p+8]=g/10+'0';
	}
	if (pos==1) bukvi(630,900, 5,19, timedate,0,0,0);
	if (pos==2) bukvi(630,20, 5,23, timedate,1,0,1);		
}
void printCI(int ci,int x,int y,int w,int color)// вывод числовых значений на экран, если они заранее не подготовленны
{
	int tmp=ci,n=0;
	do {// узнаём длину числа
		tmp=tmp/10;
		n++;
		} while (tmp>0);
		n--;
		int qwe,weq=0,eqw;
		do {//выводим с конца по одному числу, узнавая остаток от деления на 10
    		qwe = ci % 10;
    		fillword(qwe,x,y+n*30-weq*30, w, color);
    		ci = ci / 10;
    		weq++;
  		} while (ci>0);
}
void anim()// анимация, ничего особенного
{
	for(int i=0;i<16;i++) fillBK(i*0x111111,0,0,1024,1280);
	load(55,0x15000,7);
	manyload(62, 0x20000,636);// функция загрузки более 128 секторов за раз на определённый адрес
	RKN_tyan(100,950,10);
	int t=0,r=0,i=0, redFace=0,hamtime=0;
	int ham[]={698,990,1513,2348,3378};
	int hamx[]={700,580,500,420,425};
	int hamy[]={50,40,80,155,300};
	int x[]={259,338,375,369};
	int y[]={192,264,380,476};
	for(i=0;i<11;i++)
		{
			spin(50,50,0x20000,t,2);
			t++;
			t%=8;
			if (r<3) eye(r,100,950,10);
			else eye(4-r,100,950,10);
			r++;
			r%=4;
			sleep(20000);
			answer(40,1060,10,r%2);
			spin(50,50,0x20000,t,2);
			t++;
			t%=8;
			sleep(20000);
		}
	pict(350,630,4531,0,1);	
	for(i=0;i<6;i++)
	{
		spin(50,50,0x20000,t,2);
		t++;
		t%=8;
		if (i<2)
			{
				if (r<3) eye(r,100,950,10);
				else eye(4-r,100,950,10);
			}
		else if(i==3)
		{
			answer(40,1120,10,1);
			answer(40,1080,10,1);
			answer(40,1040,10,1);
			eye(2,100,950,10);
		}
		if(i<2)
		{
			r++;
			r%=4;
			answer(40,1060,10,r%2);
		}
		sleep(20000);
		spin(50,50,0x20000,t,2);	
		t++;
		t%=8;
		if ((i>2)&&(i<5))
		{
			redFace++;
			int a[8]={0,0x8697B3,0x2A2054,0xCFDFEC,0x7FA0EF,0xFFFFFF,0xBE9897-(0x003232*redFace)+(0x100000*redFace),0xF8E7DF-(0x006464*redFace)};
			mina(100,950,10,a);
		}
		sleep(20000);
	}
		for(i=0;i<5;i++)
		{
			sleep(20000);
			spin(50,50,0x20000,t,2);
			t++;
			t%=8;
			if (hamtime>0) fillBK(0xFFFFFF,hamy[hamtime-1],hamx[hamtime-1],x[hamtime-1],y[hamtime-1]);	
			pict(hamx[hamtime],hamy[hamtime],ham[hamtime],1,1);
			if (hamtime==4) 
			{
				pict(-1,-1,5239,1,1);
				sleep(20000);
			}
			hamtime++;
		}
		fillBK(0xFFFFFF,0,0,1024,1280);
		pict(348,0,33990,0,1);
		ospict(850,0, 10);
		sleep(100000);	
		fillBK(0xFFFFFF,0,0,1024,1280);
		sleep(20000);
}

void bukvi(int x,int y, int w, int kol, char * clova,int f,int notster,int white)//вывод текста, тоже немного кривой
{// координаты количество букв текс отрисовка окна и др, стирать окно или нет, заливка написаного тексата в окне белым
	if (f==1) pict(610,0,11262,0,1);	
	int mouth=0,line=0;
	for(int i=0;i<kol;i++,mouth++,line++)
	{
		if((i)&&(f))// условие перехода на новую строку если текст в окне диалога ркн
		{	
			if ((i%12==0)&&(f==1))
			{
				if ((i%24==0)&&(f==1))
				{
					sleep(6000);
					fillBK(0xFFFFFF,20,630,80,360);
					y-=w*8;
				}else y+=w*8;
				x-=w*72;
			}
		}
		sleep(1000);
		if(f==1)
		{
			if(mouth<2) fillBK(0xff0000,0+21*8,1000+11*8,8,16);	
			else 
			{
				fillBK(0x2A2054,0+21*8,1000+11*8,8,16);			
				mouth=-1;
			}
		}
		if ((clova[i]>='0')&&(clova[i]<='9'))
		{
			fillword(clova[i]-'0',y,x+line*w*6, w, 0x2A2054);
			continue;
		}
		if (clova[i]==' ') 
		{
			fillword(42,y,x+line*w*6, w, 0x2A2054);
			continue;
		}
		if (clova[i]=='.') 
		{
			fillword(43,y,x+line*w*6, w, 0x2A2054);
			continue;
		}
		if (clova[i]=='!') 
		{
			fillword(44,y,x+line*w*6, w, 0x2A2054);
			continue;
		}
		if (clova[i]=='-') 
		{
			fillword(45,y,x+line*w*6, w, 0x2A2054);
			continue;
		}
		if (clova[i]=='?') 
		{
			fillword(46,y,x+line*w*6, w, 0x2A2054);
			continue;
		}
		if (clova[i]=='*') 
		{
			fillword(47,y,x+line*w*6, w, 0x2A2054);
			continue;
		}
		if (clova[i]==',') 
		{
			fillword(48,y,x+line*w*6, w, 0x2A2054);
			continue;
		}
		if (clova[i]==':') 
		{
			fillword(49,y,x+line*w*6, w, 0x2A2054);
			continue;
		}
		if(clova[i]=='\n')// переход на новую строку пренудительно
		{
			y+=w*8;
			line=-1;
			continue;
		}
		if((clova[i]>='а')&&(clova[i]<='я'))
		{	
			fillword((clova[i]-'а')+10,y,x+line*w*6, w, 0x2A2054);
		}
	}
	if (f==1)
		{
			fillBK(0x2A2054,0+21*8,1000+11*8,8,16);
		sleep(20000);}
	if (white) {sleep(20000);fillBK(0xFFFFFF,20,630,80,360);}
	if (notster) {sleep(20000);fillBK(0xFFFFFF,0,610,160,392);}	
}
void fillBK(int color, int x, int y, int w,int h){// рисуем определённую область
// типичная структура для всех отрисовывающих функциий, цвет координаты начала (постоянно путаю что ширена а что высота)
	int i,j;
	for (i=x;i<w+x;i++)
		for (j=y;j<h+y;j++)
			setPixel(j,i,color);
}

void setPixel(int x, int y, int color)// закрашивание одного пикселя
{
	char *p;
	asm("mov dword ptr[ebp-4],esi");
	p+=((y*1280)+x)*3;// если не будешь юзать мой размер экрана то поменять нужно 1280 на 1024 или свой
	*(int*)p=*(int*)(p)& 0xff000000 |color &0x00ffffff;
}
void sleep(int x)// пауза
{
	int i=0,j=0,k;
	for(i;i<x;i++)
	{
		k=0;
		for(j;j<x*1000;j++)
			k++;
	}
}
void fillword(int sym,int x, int y, int dsize,int color)// отрисовка одной буквы
{
	int i,j;
	int p=0x20000+35*sym;// адрес символа
	for (i=0;i<7;i++){
		for (j=0;j<5;j++)
		{
			int k = *(char*) (p);
			p++;
			if (k==1) fillBK(color,x+i*dsize, y+j*dsize, dsize,dsize);
			else fillBK(0xffffff, x+i*dsize, y+j*dsize, dsize,dsize);
		}	
	}	
}
void spin(int x,int y,int Np,int tackt,int w)// функция для анимации, отрисовка символа ркн
{
	fillBK(0xffffff,y,x,130*w,130*w);
	int p;
	int diag=0,perped=0;
	if(tackt%2==0) 
	{
		diag=tackt/2;
		perped=tackt/2;
	}
	else
	{
		diag=(tackt/2)+1;
		perped=tackt/2;
	}
	p=Np+diag*96*512+perped*63*512;
	short height,width;
	int size =((short*)(p))[0];
	height=((short*)(p))[1];
	width=((short*)(p))[2];
	p+=6;
	if(tackt%2!=0)
	{
		x+=12*w;
		y+=12*w;
	}
	for(int i=0;i<height;i++){
            for(int j=0;j<width;j++){
            	int color=0;
            	for(int k=0;k<3;k++)
            	{	
            		color*=256;
            		color+=*(unsigned char*)(p);
            		p++;
				}

				if(color!=0xFFFFFF) fillBK(color,y+i*w,x+j*w,w,w);

 		}
	}

}

void ospict(int x,int y,int w)// отрисовка названия оси
	{
		int p=0x15600;
		int j=0,i=0;
		for (i=0;i<12;i++)
		{
			sleep(5000);
			for (j=0;j<128;j++)
			{
				int k = *(char*) (p);
				p++;
				if(k==1) continue; 
				fillBK(0x2A2054, x+i*w, y+j*w, w,w);
			}
		}	
	}
	
void mina(int x,int y,int w, int* colors)// работа с мордашкой ркн тян
	{
		int p=0x15444;//12 ГЁ 6
		int j=0,i=0,color;
		for (i=0;i<11;i++)
			{
				for (j=0;j<14;j++)
				{
					int k = *(char*) (p);
					p++;
					if(k==0) continue; 
					color=colors[k];
					fillBK(color, x+i*w+12*w, y+j*w+6*w, w,w);
				}
			}	
	}
void answer(int x, int y, int w,int flag)// рисуем впоросительные и восклицательные знаки
{
	fillBK(0xffffff,x,y,w*6,w*6);
	int colors[3]={0,0x8697B3,0x2A2054};
	int p=0x153fc+36*flag;
	int j=0,i=0,color;
	for(i=0;i<6;i++)
	{
		for(j=0;j<6;j++)
		{
				int k = *(char*) (p);
				p++;
				color=0;
				if(k==0) continue; 
				color=colors[k];
				fillBK(color, x+i*w, y+j*w, w,w);
		}
	}
}
void eye(int pos, int x, int y, int w)// работа с глазками 
	{
	int colors[8]={0,0x8697B3,0x2A2054,0xCFDFEC,0x7FA0EF,0xFFFFFF,0xBE9897,0xF8E7DF};
	int p=0x15360+52*pos;
	int j=0,i=0,color;
	for (i=0;i<4;i++)
		{
			for (j=0;j<13;j++)
			{
				int k = *(char*) (p);
				p++;
				if(k==0) continue; 
				color=colors[k];
				fillBK(color, x+i*w+14*w, y+j*w+7*w, w,w);
			}
		}
	
}
void RKN_tyan(int x,int y,int w)// рисуесм саму тяночку
{
	int colors[8]={0,0x8697B3,0x2A2054,0xCFDFEC,0x7FA0EF,0xFFFFFF,0xBE9897,0xF8E7DF};
	int p=0x15000;
	int j=0,i=0,color;
	for (i=0;i<27;i++)
	{
		for (j=0;j<32;j++)
		{
			int k = *(char*) (p);
			p++;
			if(k==0) continue; 
			color=colors[k];
			fillBK(color, x+i*w, y+j*w, w,w);
		}
	}
}

void pict(int x,int y,int start,int NoW,int w)// отрисовка любого изображения start номер первого сектора картинки
	{// дисклеймер, работает по дате переработанной мною, чтобы тупо загонять картинку нужно переписать 
		load(start,0x200000,1);// первый сектор с первичной инфой
		int p=0x200000;
		short height,width;
		int size =((short*)(p))[0];// количество секторов
		height=((short*)(p))[1];// высота
		width=((short*)(p))[2];// ширина
		p+=6;
		manyload(start,0x200000,size);
		int startx=0,starty=0;
		if (x!=-1)// в середине экрана аль свои координаты
		{
		startx=x;
	    starty=y;	
		}
		else
		{
		startx=(1280-width*w)/2;
	    starty=(1024-height*w)/2;
		}
		for(int i=0;i<height;i++){
	            for(int j=0;j<width;j++)
					{
		            	int color=0;
		            	for(int k=0;k<3;k++)// складываем цвета к+з+с
			            	{	
			            		color*=256;
			            		color+=*(unsigned char*)(p);
			            		p++;
							}
						if(NoW!=0)// не отрисовка белого цвета, для изоброжений где фон белый и может закрыть нужную территорию
						{
							if(color!=0xFFFFFF)
								fillBK(color,starty+i*w,startx+j*w,w,w);
						}
						else fillBK(color,starty+i*w,startx+j*w,w,w);
		 			}
		}
	}
int strcmp(char * str1,char* str2)// сравниваем строки
{
	
	while((*str1)&&(*str2))// пока что то есть в обоих
	{
		if((*str1)!=(*str2)) return 0;// если символы не ровны
		str1++;// переход к следующему сиволу
		str2++;
	}
	if((*str1)==0 && (*str2)==0) return 1;// если они закончили на одном месте, то говорим равны
	return 0;
}
void manyload(int sector, int offset,int amount)// загрузка много секторв, тупо делим на загрзки по 128 секторов
{
	int i;
	while(amount>0)
	{
		if (amount>=128)
			load(sector,offset,128);
		else 
			load(sector,offset,amount);
		sector+=128;
		offset+=0x10000;
		amount-=128;
	}
}
void load(int sector, int offset,int amount)
{
	int i;
	for(i=0; i<amount; i++)
	{
		LoadFromDisk(offset,sector);
		sector++;
		offset+=512;
	}
}
void LoadFromDisk(int offset,int sectorID)// сама загруска сектора, если что ссылайся на этот сайт https://wiki.osdev.org/ATA_read/write_sectors
	{
		asm("mov eax,dword ptr[ebp+8]");
		asm("mov edi,eax");
		asm("mov eax,dword ptr[ebp+12]");
		asm("mov cl,1");        
		asm("pushf");
		asm("and eax,dword ptr(0x0FFFFFFF)");
		asm("push eax");
		asm("push ebx");
		asm("push ecx");
		asm("push edx");
		asm("push edi");
		asm("mov ebx,eax");
		asm("mov edx,0x01f6");
		asm("shr eax,24");
		asm("or al,0xE0");
		asm("out dx,al");
		asm("mov edx,0x01f2");
		asm("mov al,cl");
		asm("out dx,al");
		asm("mov edx,0x1f3");
		asm("mov eax,ebx");
		asm("out dx,al");
		asm("mov edx,0x1f4");
		asm("mov eax,ebx");
		asm("shr eax,8");
		asm("out dx,al");
		asm("mov edx,0x1f5");
		asm("mov eax,ebx");
		asm("shr eax,16");
		asm("out dx,al");
		asm("mov edx,0x1f7");
		asm("mov al,0x20");
		asm("out dx,al");
		disk_goint();
		asm("mov eax,0x100");
		asm("xor bx,bx");
		asm("mov bl,cl");
		asm("mul bx");
		asm("mov ecx,eax");
		asm("mov edx,0x1f0");
		asm("rep insw");
		asm("pop edi");
		asm("pop edx");
		asm("pop ecx");
		asm("pop ebx");
		asm("pop eax");
		asm("popf");
		return;
	}
void disk_goint()
	{
		asm("still_goint:");
		asm("in al,dx");
		asm("test al,8");
		asm("jz still_goint");	
	}
int GetKey(int key)// получение значения с клавы
{// почему два? да я сам не знаю, иногда работает так иногда сяк
	asm ("mov eax, 0");
	asm ("in al, 0x60");
	asm ("mov byte ptr[ebp+8],al");
	return key;
}
int getkey()
	{
		int key;
		asm ("mov eax, 0");
		asm ("in al, 0x60");
		asm ("mov byte ptr[ebp-4],al");
		return key;
	}


int get_amount_memory()// размер диска
{ 
asm("mov eax, dword ptr[0xD000]");
}
int get_video()// число видео режима, всё это пихается в boot-е
{ 
asm("mov eax, dword ptr[0xD006]");
}
int get_disk_type()// тип диска
{ 
	asm("mov eax, 0");
	asm("mov al,[0xD004]");
}
int basic()// лаба эдика архитектура 7
{
	asm("mov al, 0x16 ");
	asm("out 0x70, al");
	asm("in al, 0x71");
	asm("mov bh,al");
	asm("mov al, 0x15 ");
	asm("out 0x70, al");
	asm("in al, 0x71");
	asm("mov ah,bh");
}
int ext()// лаба эдика архитектура 7
{
	asm("mov al, 0x18 ");
	asm("out 0x70, al");
	asm("in al, 0x71");
	asm("mov bh,al");
	asm("mov al, 0x17 ");
	asm("out 0x70, al");
	asm("in al, 0x71");
	asm("mov ah,bh");
}
long long int get_cpuid_flags()// флаги cpuid нужен для других функций
{
	long long int result;
	asm("mov eax,0x1");
	asm("cpuid");
	asm("mov dword ptr[ebp-8],eax");
	asm("mov dword ptr[ebp-4],edx");
	return result;
}
int get_MMX_support()// поддержка mmx
{
	return (get_cpuid_flags()>>(32+23))&1;
}
int get_SIMD_support()// поддержка simd
{
	return (get_cpuid_flags()>>(32+25))&1;
}
int get_PSE_support()// поддержка pse
{
	return (get_cpuid_flags()>>(32+3))&1;
}
int sek()// секунды
{
	asm("mov al, 0x0");
	asm("out 0x70, al");
	asm("in al, 0x71");
}
int min()// минуты
{
	asm("mov al, 0x02");
	asm("out 0x70, al");
	asm("in al, 0x71");
}
int hour()// часы
{
	asm("mov al, 0x04");
	asm("out 0x70, al");
	asm("in al, 0x71");
}
int day()// дни
{
	asm("mov al, 0x7");
	asm("out 0x70, al");
	asm("in al, 0x71");
}
int moun()// месяца
{
	asm("mov al, 0x08");
	asm("out 0x70, al");
	asm("in al, 0x71");
}
int year()// года 2 часть
{
	asm("mov al, 0x09");
	asm("out 0x70, al");
	asm("in al, 0x71");
}
int year2()//года 1 часть
{
	asm("mov al, 0x32");
	asm("out 0x70, al");
	asm("in al, 0x71");
}
