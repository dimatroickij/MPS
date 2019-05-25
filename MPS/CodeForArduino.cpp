#include "OneButton.h"
#include <iarduino_RTC.h>
#include <avr/pgmspace.h>                                            //  Подключаем библиотеку для работы с PROGMEM Arduino для хранения символов дисплея
#include <EEPROM.h>                                                  //  Подключаем библиотеку для работы с EEPROM  Arduino для хранения значений таймеров
#include <Wire.h>                                                    //  Подключаем библиотеку для работы с шиной I2C
#include <LiquidCrystal_I2C.h>                                       //  Подключаем библиотеку для работы с LCD дисплеем по шине I2C


#define    OK    2
#define     BACK  1
#define     NEXT  3
#define   COUNT 8 //  Количество релейных модулей

//  ВЫВОДЫ ARDUINO  //
const uint8_t relays[COUNT] = { 2, 3, 4, 5, 6, 7, 8, 9 }; //  Выводы реле
const uint8_t buzzer = 12;  //  Пьезо пищалка

//   КОНСТАНТЫ И ПЕРЕМЕННЫЕ, НЕОБХОДИМЫЕ ДЛЯ РАБОТЫ СКЕТЧА: //

//  Таблица русских символов
const byte      rusMem[40][8] PROGMEM = {
  { 31, 16, 16, 30, 17, 17, 30, 0 }, { 0, 0, 30, 17, 30, 17, 30, 0 }, //  Б,  в,  № матрицы символа в массиве: 00, 01
  { 31, 16, 16, 16, 16, 16, 16, 0 }, { 0, 0, 30, 16, 16, 16, 16, 0 }, //  Г,  г,  № матрицы символа в массиве: 02, 03
  { 6, 10, 10, 10, 10, 10, 31, 17 }, { 10, 10, 14, 2, 2, 0, 0, 0 },   //  Д,  4,  № матрицы символа в массиве: 04, 05
  { 17, 17, 17, 19, 21, 25, 17, 0 }, {31, 2, 4, 2, 1, 17, 14, 0 },    //  И,  З,  № матрицы символа в массиве: 06, 07
  { 21, 17, 17, 19, 21, 25, 17, 0 }, { 10, 4, 17, 19, 21, 25, 17, 0 },    //  Й,  й,  № матрицы символа в массиве: 08, 09
  { 0, 0, 18, 20, 24, 20, 18, 0 }, { 0, 0, 17, 27, 21, 17, 17, 0 },      //  к,  м,  № матрицы символа в массиве: 10, 11
  { 7, 9, 9, 9, 9, 9, 17, 0 }, { 31, 4, 4, 4, 4, 4, 4, 0 },        //  Л,  Т,  № матрицы символа в массиве: 12, 13
  { 31, 17, 17, 17, 17, 17, 17, 0 }, { 0, 0, 17, 17, 31, 17, 17, 0 }, //  П   н,  № матрицы символа в массиве: 14, 15
  { 17, 17, 17, 15, 1, 17, 14, 0 }, { 0, 0, 31, 4, 4, 4, 4, 0 },    //  У,  т,  № матрицы символа в массиве: 16, 17
  { 17, 17, 17, 15, 1, 1, 1, 0 }, { 0, 0, 17, 17, 15, 1, 1, 0 },    //  Ч,  ч,  № матрицы символа в массиве: 18, 19
  { 17, 17, 17, 29, 19, 19, 29, 0 }, { 0, 0, 17, 17, 29, 19, 29, 0 }, //  Ы,  ы,  № матрицы символа в массиве: 20, 21
  { 16, 16, 16, 30, 17, 17, 30, 0 }, { 0, 0, 0, 0, 0, 0, 21, 0 },   //  Ь, ..., № матрицы символа в массиве: 22, 23
  { 18, 21, 21, 29, 21, 21, 18, 0 }, { 0, 0, 18, 21, 29, 21, 18, 0 }, //  Ю,  ю,  № матрицы символа в массиве: 24, 25
  { 15, 17, 17, 15, 5, 9, 17, 0 }, { 0, 0, 15, 17, 15, 5, 9, 0 },   //  Я,  я,  № матрицы символа в массиве: 26, 27
  { 31, 4, 31, 0, 31, 16, 31, 0 }, { 16, 31, 16, 0, 10, 21, 31, 0 },  //  ПН, ВТ  № матрицы символа в массиве: 28, 29
  { 28, 20, 31, 0, 17, 17, 31, 0 }, { 16, 31, 16, 0, 31, 4, 28, 0 },  //  СР, ЧТ  № матрицы символа в массиве: 30, 31
  { 16, 31, 16, 0, 31, 16, 31, 0 }, { 23, 21, 31, 0, 17, 17, 31, 0 }, //  ПТ, СБ  № матрицы символа в массиве: 32, 33
  { 17, 17, 31, 0, 10, 21, 31, 0 }, { 4, 12, 4, 4, 14, 0, 0, 0 },   //  ВС, 1   № матрицы символа в массиве: 34, 35
  { 14, 2, 14, 8, 14, 0, 0, 0 }, { 14, 2, 14, 2, 14, 0, 0, 0 },     //  2, 3 № матрицы символа в массиве: 36, 37
  { 18, 18, 18, 18, 18, 18, 31, 1 }, {17, 17, 19, 13, 1, 17, 14, 0} };  //  Ц, У № матрицы символа в массиве: 38, 39

const int maxTimers = 145;           //  Максимальное количество расписаний (максимум 145)
int     pressedButton = 0;          //  Нажатая кнопка
uint8_t   valArray[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };  //  Определяем массив элементы которого будут хранить различную информацию в зависимости от режима

char    valChar[5] = "    ";        //  Определяем массив символов (строку) информация которой будет отображаться на дисплее мигая
int     valMode = 0;            //  Текущий режим
int   valSubMode = 0;           //  Текущий подрежим
uint8_t   valTimerNum = 0;          //  Выбранное расписание
bool    flgDisplayUpdate = 1;       //  Флаг обновления дисплея
int     numByte = 0;            //  Номер байта EEPROM
int     valByte;              //  Значение байта EEPROM
bool  connectPC = false;
uint8_t setChanel[COUNT]; //  Установить: Включённое (1) или выключенное (0) реле


LiquidCrystal_I2C lcd(0x27/*0x27*//*0x3f для протеуса*/, 16, 2);     //  Объект - lcd для работы с дисплеем (адрес I2C = 0x27, количество столбцов = 16, количество строк = 2)
iarduino_RTC    time(RTC_DS1307);     //  Объект - time для работы с часами RTC (тип модуля)
OneButton buttonBack(14, 1);          //  Объект - кнопка "Назад" (№ пина, )
OneButton buttonNext(15, 1);          //  Объект - кнопка "Вперёд" (№ пина, )
OneButton buttonOk(16, 1);          //  Объект - кнопка "ОК" (№ пина, )

//  ФУНКЦИИ, ИСПОЛЬЗУЕМЫЕ В ПРОГРАММЕ //
void    buttonRead(void);       //  Функция, выполняющая управление режимами в зависимости от нажатой кнопки
void    displayUpdate(void);      //  Функция отображения данных на экран
void    setRelay(void);         //  Функция, которая включает нужное реле в зависимости от расписания
void    proveVoltage(void);       //  Функция проверки напряжения
void    viewEEPROM(void);         //  Просмотр значений EEPROM в COM порте
void    SetChars(           //  Функция записи русских символов в память дисплея (максимум 7 символов)
	uint8_t = 255,
	uint8_t = 255,
	uint8_t = 255,
	uint8_t = 255,
	uint8_t = 255,
	uint8_t = 255,
	uint8_t = 255);
uint8_t   ReadTimer(uint8_t = 0,      //  Чтение параметров таймера (№ таймера, № параметра)
	uint8_t = 0);
void    SaveTimer(uint8_t = 0,      //  Функция записи одного из параметров таймера (№ таймера, № параметра, значение параметра)
	uint8_t = 0,
	uint8_t = 0);
uint8_t   FindTimer(void);        //  Функция поиска № следующего свободного места для записи расписания
bool    TestTimer(void);        //  Функция проверки данных в ячейках EEPROM на соответствие (первый запуск программы)
void setup() {
	Serial.begin(9600);

	//  Выводы на реле и пищалку в режим выхода //
	for (int i = 0; i < COUNT; i++)
	{
		pinMode(relays[i], OUTPUT);
		digitalWrite(relays[i], HIGH);
	}

	pinMode(buzzer, OUTPUT);

	for (int i = 0; i < COUNT; i++)
		setChanel[i] = 0;

	time.begin();
	lcd.init();
	lcd.backlight();

	//  Подключение события "Нажатие на кнопку" //
	buttonBack.attachClick(clickButtonBack);
	buttonNext.attachClick(clickButtonNext);
	buttonOk.attachClick(clickButtonOK);
	SetChars(16, 26, 14, 6, 38, 8, 4);    //  "У", "Я", "П", "И", "Ц", "Й", "Д"
	lcd.setCursor(0, 0);
	lcd.print(F("K\1PCOBA\2 \3O M\3C"));
	lcd.setCursor(1, 1);
	lcd.print(F("TPO\4\5K\4\6 \7. A."));
	delay(1000);
	lcd.clear();

	//  Проверка на первый запуск программы   //
	if (!TestTimer()) {
		SetChars(14, 4, 2);         //  "П", "Д" и "Г"
		lcd.setCursor(0, 0);
		lcd.print(F("\1O\2\3OTOBKA..."));

		for (uint8_t i = 0; i < maxTimers; i++) {
			for (uint8_t j = 0; j < 8; j++) {
				SaveTimer(i, j, 0);
			}
		}
		delay(1000);
		lcd.clear();
	}
}

void loop() {
	//  Включение отслеживания состояний кнопок //
	buttonBack.tick();
	buttonNext.tick();
	buttonOk.tick();

	buttonRead();
	displayUpdate();
	setRelay();
	proveVoltage();

	//viewEEPROM();
}

//  ФУНКЦИЯ, ВЫПОЛНЯЮЩАЯ УПРАВЛЕНИЕ РЕЖИМАМИ В ЗАВИСИМОСТИ ОТ НАЖАТОЙ КНОПКИ  //
void buttonRead(void) {
	int j = 4000; //  Режим, в который переходит программа (4000 - остаться в том же режиме)
	switch (valMode) {

	case  0:  //  Главное меню
		if (pressedButton == OK) { j = 1; }
		else if (Serial.available() > 0)
		{
			char s = Serial.read();
			if (s == 'c')
				j = 41;
		}
		flgDisplayUpdate = 1;
		break;

	case  1:  //  Меню "Таймеры"
		if (pressedButton == OK) {
			j = ReadTimer() ? 11 : 12;  //  Проверка на существование хотя бы одного расписания. Если нет, то меню "Таймеры > Новый таймер"
			valTimerNum = 0;
		}
		if (pressedButton == BACK) { j = 3; }
		if (pressedButton == NEXT) { j = 2; }
		break;

	case 11:  //  Меню "Таймеры > сохранённый таймер"
		if (pressedButton == OK) { j = 111; }
		if (pressedButton == BACK) {
			if (valTimerNum) {  //  Если сохранённое расписание уже есть
				valTimerNum--;
				lcd.clear();
				flgDisplayUpdate = 1;
			}
			else { j = 14; }
		}
		if (pressedButton == NEXT) {
			valTimerNum++;
			if (valTimerNum >= maxTimers) { j = 13; } //  Если достигнуто макс. кол-во расписаний
			else if (ReadTimer(valTimerNum)) {    //  Если следующее расписание существует
				lcd.clear();
				flgDisplayUpdate = 1;
			}
			else { j = 12; }
		}
		break;

	case 111: //  Меню "Таймеры > сохранённый таймер > Время и канал"
		if (pressedButton == OK) {
			j = 1111;
			valSubMode = 0;
			for (int i = 1; i < 6; i++)
				valArray[i - 1] = ReadTimer(valTimerNum, i);
		}
		if (pressedButton == BACK) { j = 114; }
		if (pressedButton == NEXT) { j = 112; }
		break;

	case 1111:  //  Меню "Таймеры > сохранённый таймер > Время и канал > изменение времени и канала"
		if (pressedButton == OK) {
			if (valSubMode == 0) { valSubMode = 1; }
			else if (valSubMode == 1) { valSubMode = 2; }
			else if (valSubMode == 2) { valSubMode = 3; }
			else if (valSubMode == 3) { valSubMode = 4; }
			else if (valSubMode == 4) {
				if (((valArray[0] >= valArray[2]) && (valArray[1] >= valArray[3])) || (valArray[0] > valArray[2]))
				{
					j = 5;
					valSubMode = 1111;
				}
				else
				{
					j = 111;
					SaveTimer(valTimerNum, 0, 1);
					for (int i = 1; i < 6; i++)
						SaveTimer(valTimerNum, i, valArray[i - 1]);
				}
			}
		}
		if (pressedButton == BACK) {
			valArray[valSubMode] --;
			if (valArray[0] > 23) { valArray[0] = 23; }
			if (valArray[1] > 59) { valArray[1] = 59; }
			if (valArray[2] > 23) { valArray[2] = 23; }
			if (valArray[3] > 59) { valArray[3] = 59; }
			if (valArray[4] == 0) { valArray[4] = COUNT; }
		}
		if (pressedButton == NEXT) {
			valArray[valSubMode]++;
			if (valArray[0] > 23) { valArray[0] = 0; }
			if (valArray[1] > 59) { valArray[1] = 0; }
			if (valArray[2] > 23) { valArray[2] = 0; }
			if (valArray[3] > 59) { valArray[3] = 0; }
			if (valArray[4] > COUNT) { valArray[4] = 1; }
		}
		flgDisplayUpdate = 1;
		break;

	case 112: //  Меню "Таймеры > сохранённый таймер > Повторы"
		if (pressedButton == OK) {
			j = 1121;
			valSubMode = 0;
			for (int i = 0; i < 7; i++)
				valArray[i] = bitRead(ReadTimer(valTimerNum, 6), 6 - i);
		}
		if (pressedButton == BACK) { j = 111; }
		if (pressedButton == NEXT) { j = 113; }
		break;

	case 1121:  //  Меню "Таймеры > сохранённый таймер > Повторы > изменение повторов"
		if (pressedButton == OK) {
			flgDisplayUpdate = 1;
			if (valSubMode == 0) { valSubMode = 1; }
			else if (valSubMode == 1) { valSubMode = 2; }
			else if (valSubMode == 2) { valSubMode = 3; }
			else if (valSubMode == 3) { valSubMode = 4; }
			else if (valSubMode == 4) { valSubMode = 5; }
			else if (valSubMode == 5) { valSubMode = 6; }
			else if (valSubMode == 6) {
				int summ = 0;
				for (int jj = 0; jj < 7; jj++)
					summ += valArray[jj];
				if (summ != 0)
				{
					j = 112;
					uint8_t k = 0;
					for (int i = 0; i < 7; i++)
						bitWrite(k, 6 - i, valArray[i]);
					SaveTimer(valTimerNum, 6, k);
					lcd.noBlink();
				}
				else
				{
					j = 5;
					valSubMode = 1121;
				}
			}
		}
		if ((pressedButton == BACK) || (pressedButton == NEXT)) {
			flgDisplayUpdate = 1;
			if (valArray[valSubMode]) { valArray[valSubMode] = 0; }
			else { valArray[valSubMode] = 1; }
		}
		break;

	case 113: //  Меню "Таймеры > сохранённый таймер > Стереть таймер"
		if (pressedButton == OK) { j = 1131; }
		if (pressedButton == BACK) { j = 112; }
		if (pressedButton == NEXT) { j = 114; }
		break;

	case 1131:  //  Меню "Таймеры > сохранённый таймер > Стереть таймер > Таймер удалён"
		j = 12;
		valArray[0] = valTimerNum;
		valArray[1] = FindTimer() - 1;

		// Поиск ближайшего расписания
		if (valArray[0] < valArray[1]) {
			j = 11;
			valTimerNum = valArray[0];
		}
		else if (valArray[1] > 0) {
			j = 11;
			valTimerNum = valArray[0] - 1;
		}

		//  Сдвиг следующих расписаний на 7 байт назад
		shift();

		delay(2000);
		break;

	case 114: //  Меню "Таймеры > сохранённый таймер > Выход"
		if (pressedButton == OK)
		{
			if (dublicate(valTimerNum))
				j = 6;
			else
				j = 11;
		}
		if (pressedButton == BACK) { j = 113; }
		if (pressedButton == NEXT) { j = 111; }
		break;

	case 12:  //  Меню "Таймеры > Новый таймер"
		if (pressedButton == OK) { j = 121; }
		if (pressedButton == BACK) {
			j = ReadTimer() ? 11 : 14;  //  Проверка на существование хотя бы одного расписания. Если нет, то меню " Таймеры > Выход"
			if (ReadTimer()) {
				valTimerNum = FindTimer() - 1;
			}
		}
		if (pressedButton == NEXT) { j = ReadTimer() ? 13 : 14; } //  Проверка на существование хотя бы одного расписания. Если есть, то открывается меню "Стереть всё".
		break;

	case 121: //  Меню "Таймеры > Новый таймер > Новый таймер создан"
		j = 1111;
		valTimerNum = FindTimer();
		valSubMode = 0;
		for (int i = 0; i < 4; i++)
			valArray[i] = 0;
		valArray[4] = 1;
		SaveTimer(valTimerNum, 0, 1);
		SaveTimer(valTimerNum, 1);
		SaveTimer(valTimerNum, 2);
		SaveTimer(valTimerNum, 3);
		SaveTimer(valTimerNum, 4);
		SaveTimer(valTimerNum, 5, 1);
		SaveTimer(valTimerNum, 6, 127);

		delay(2000);
		break;

	case 13:  // Меню "Таймеры > Стереть всё"
		if (pressedButton == OK) { j = 131; }
		if (pressedButton == BACK) {
			j = FindTimer() < maxTimers ? 12 : 11;  //  Проверка на предельное число расписаний. Если переполнено, то открывается последнее расписание
			valTimerNum = j == 11 ? maxTimers - 1 : 0;
		}
		if (pressedButton == NEXT) {
			j = 14;
			valTimerNum = 0;
		}
		break;

	case 131: // Меню "Таймеры > Стереть всё > Все данные стёрты"
		j = 12;
		for (valArray[0] = 0; valArray[0] < maxTimers; valArray[0] ++) { SaveTimer(valArray[0]); }
		delay(2000);
		break;

	case 14:  // Меню "Таймеры > Выход"
		if (pressedButton == OK) { j = 1; }
		if (pressedButton == BACK) {
			j = ReadTimer() ? 13 : (FindTimer() < maxTimers ? 12 : 11); //  Проверка на существование хотя бы одного расписания. Если есть, то меню "Таймеры > Стереть всё"
			valTimerNum = 0;
		}
		if (pressedButton == NEXT) {
			j = ReadTimer() ? 11 : (FindTimer() < maxTimers ? 12 : 13); // Проверка на существование хотя бы одного расписания. Если нет, то проверка на переполненность количества расписаний 
			valTimerNum = 0;
		}
		break;

	case  2:  //  Меню "Часы"
		if (pressedButton == OK) { j = 21; }
		if (pressedButton == BACK) { j = 1; }
		if (pressedButton == NEXT) { j = 3; }
		break;

	case 21:  //  Меню "Часы > Время"
		if (pressedButton == OK) {
			j = 211;
			valSubMode = 0;
			time.gettime();
			valArray[0] = time.Hours;
			valArray[1] = time.minutes;
			valArray[2] = time.seconds;
		}
		if (pressedButton == BACK) { j = 23; }
		if (pressedButton == NEXT) { j = 22; }
		break;

	case 211: //  Меню "Часы > Время > Установка времени"
		if (pressedButton == OK) {
			if (valSubMode == 0) { valSubMode = 1; }
			else if (valSubMode == 1) { valSubMode = 2; }
			else if (valSubMode == 2) {
				time.settime(valArray[2], valArray[1], valArray[0]);
				j = 21;
			}
		}
		if (pressedButton == BACK) {
			valArray[valSubMode] --;
			if (valArray[0] > 23) { valArray[0] = 23; }
			if (valArray[1] > 59) { valArray[1] = 59; }
			if (valArray[2] > 59) { valArray[2] = 59; }
		}
		if (pressedButton == NEXT) {
			valArray[valSubMode] ++;
			if (valArray[0] > 23) { valArray[0] = 0; }
			if (valArray[1] > 59) { valArray[1] = 0; }
			if (valArray[2] > 59) { valArray[2] = 0; }
		}
		flgDisplayUpdate = 1;
		break;

	case 22:  //  Меню "Часы > Дата"
		if (pressedButton == OK) {
			j = 221;
			valSubMode = 0;
			time.gettime();
			valArray[0] = time.day;
			valArray[1] = time.month;
			valArray[2] = time.year;
		}
		if (pressedButton == BACK) { j = 21; }
		if (pressedButton == NEXT) { j = 23; }
		break;

	case 221: //  Меню "Часы > Дата > Установка даты"
		if (pressedButton == OK) {
			if (valSubMode == 0) { valSubMode = 1; }
			else if (valSubMode == 1) { valSubMode = 2; }
			else if (valSubMode == 2)
			{
				if (checkDate())
				{
					int a, y, m, R;
					a = (14 - valArray[1]) / 12;
					y = valArray[2] - a;
					m = valArray[1] + 12 * a - 2;
					R = 7000 + (valArray[0] + y + y / 4 - y / 100 + y / 400 + (31 * m) / 12);
					valArray[3] = R % 7;
					time.settime(-1, -1, -1, valArray[0], valArray[1], valArray[2], valArray[3]);
					for (int i = 0; i < 4; i++)
						valArray[i] = 0;
					j = 22;
				}
				else {
					j = 5;
					valSubMode = 221;
				}
			}
		}
		if (pressedButton == BACK) {
			valArray[valSubMode] --;
			if (valArray[0] == 0) { valArray[0] = 31; }
			if (valArray[1] == 0) { valArray[1] = 12; }
			if (valArray[2] > 99) { valArray[2] = 99; }
		}
		if (pressedButton == NEXT) {
			valArray[valSubMode] ++;
			if (valArray[0] > 31) { valArray[0] = 1; }
			if (valArray[1] > 12) { valArray[1] = 1; }
			if (valArray[2] > 99) { valArray[2] = 0; }
		}
		flgDisplayUpdate = 1;
		break;

	case 23:  //  Меню "Часы > Выход"
		if (pressedButton == OK) { j = 2; }
		if (pressedButton == BACK) { j = 22; }
		if (pressedButton == NEXT) { j = 21; }
		break;

	case  3:  //  Меню "Выход"
		if (pressedButton == OK) {
			j = 0;
			for (int i = 0; i < 4; i++)
				valArray[i] = 0;
			while (Serial.available()) Serial.read();
		}
		if (pressedButton == BACK) { j = 2; }
		if (pressedButton == NEXT) { j = 1; }
		break;

	case 4:    //  Меню "Работа с ПК"
		int z;
		z = proveSP();
		if (z != 4)
			j = z;
		break;

	case 41: //  Меню "Установка соединения"
		if (!connectPC) {
			if (Serial.available() > 0) {
				if (Serial.read() == 'y')
				{
					Serial.println(String(FindTimer()) + "," + String(maxTimers));
					connectPC = true;
					j = 411;
				}
				else
				{
					Serial.println("Arduino");
					connectPC = false;
				}
			}
			else {
				Serial.println("Arduino");
				connectPC = false;
			}
		}
		break;

	case 411: //  Меню "Соединение установлено"
		delay(1000);
		j = 4;
		break;

	case 42: //  Меню "Передача данных на ПК"
		Serial.println("yesRead");
		for (int i = 0; i < FindTimer(); i++)
		{
			String s = "";
			s = ReadTimer(i, 1) + String(',') + ReadTimer(i, 2) + String(',') +
				ReadTimer(i, 3) + String(',') + ReadTimer(i, 4) + String(',') + ReadTimer(i, 5) + String(',') + ReadTimer(i, 6) + String(';');
			Serial.println(s);
		}
		Serial.println("endRead");
		j = 4;
		break;

	case 43: //  Меню "Получение данных с ПК"
		j = editData("endSave", 431);
		break;

	case 431: //  Меню "Данные сохранены"
		delay(1000);
		j = 4;
		break;

	case 44:  // Меню "Изменение данных расписания"
		j = editData("endEdit", 441);
		break;

	case 441: //  Меню "Данные изменены"
		delay(1000);
		j = 4;
		break;

	case 45:  // Меню "Удаление расписания"
		if (Serial.available() > 0)
		{
			String value;
			value = Serial.readStringUntil('\n');
			valArray[0] = value.toInt();
			valArray[1] = FindTimer() - 1;
			shift();
			valArray[0] = valArray[1] = 0;
			Serial.println("endDelete");
			j = 451;
		}
		break;

	case 451: //  Меню "Данные удалены"
		delay(1000);
		j = 4;
		break;

	case 46:  // Меню "Удаление всех расписаний"
		j = 4;
		for (valArray[0] = 0; valArray[0] < maxTimers; valArray[0] ++) { SaveTimer(valArray[0]); }
		delay(2000);
		break;

	case 5: //  Обработка ошибок
		delay(1000);
		j = valSubMode;
		valSubMode = 0;
		break;

	case 6: //  Дубликат
		delay(1000);
		j = 111;
		break;
	}
	pressedButton = 0;
	//  Изменение текущего состояния на следующее, обновление данных на экране
	if (j < 4000) {
		lcd.clear();
		flgDisplayUpdate = 1;
		valMode = j;
	}
}

//  ОБНОВЛЕНИЕ ИНФОРМАЦИИ НА ДИСПЛЕЕ  //
void displayUpdate() {
	if (flgDisplayUpdate) {
		flgDisplayUpdate = 0;
		switch (valMode) {

		case  0:  //  Главное меню
			SetChars(35, 36, 37, 5, 3, 0, time.weekday == 4 ? 18 : 14); // "^1", "^2", "^3", "^4", "г", "Б", "П/Ч"

			// 00:00:00
			lcd.setCursor(0, 0);
			lcd.print(time.gettime("H:i:s"));

			// 00.00.0000г. ДН"
			lcd.setCursor(0, 1);
			lcd.print(time.gettime("d.m.Y\5."));

			lcd.setCursor(13, 1);
			lcd.print(time.weekday == 1 ? "\7H" :
				(time.weekday == 2 ? "BT" :
				(time.weekday == 3 ? "CP" :
					(time.weekday == 4 ? "\7T" :
					(time.weekday == 5 ? "\7T" :
						(time.weekday == 6 ? "C\6" : ("BC")))))));

			lcd.setCursor(8, 0);
			lcd.print("        ");

			valArray[COUNT] = 15;
			for (int i = COUNT - 1; i >= 0; i--)
			{
				if (valArray[i]) {
					lcd.setCursor(valArray[COUNT], 0);
					lcd.print(String(i + 1));
					valArray[COUNT]--;
				}
			}

			break;

		case  1:  //  Меню "Таймеры"
			SetChars(11, 15, 25, 9, 20);  // "м", "н", "ю", "й", "Ы"

			lcd.setCursor(0, 0);
			lcd.print(F("\1e\2\3:"));

			lcd.setCursor(0, 1);
			lcd.print(F("<   TA\4MEP\5    >"));
			break;

		case 11:  //  Меню "Таймеры > сохранённый таймер"
			SetChars(11, 15, 25, 17, 9, 21);  //  "м", "н", "ю", "т", "й", "ы"

			lcd.setCursor(0, 0);
			lcd.print(F("\1e\2\3>\4a\5\1ep\6:"));

			lcd.setCursor(0, 1);
			lcd.print(F("< 00:00-00:00-0>"));

			lcd.setCursor(2, 1);
			lcd.print(ReadTimer(valTimerNum, 1) < 10 ? "0" : "");
			lcd.print(ReadTimer(valTimerNum, 1));

			lcd.setCursor(5, 1);
			lcd.print(ReadTimer(valTimerNum, 2) < 10 ? "0" : "");
			lcd.print(ReadTimer(valTimerNum, 2));

			lcd.setCursor(8, 1);
			lcd.print(ReadTimer(valTimerNum, 3) < 10 ? "0" : "");
			lcd.print(ReadTimer(valTimerNum, 3));

			lcd.setCursor(11, 1);
			lcd.print(ReadTimer(valTimerNum, 4) < 10 ? "0" : "");
			lcd.print(ReadTimer(valTimerNum, 4));

			lcd.setCursor(14, 1);
			lcd.print(ReadTimer(valTimerNum, 5));
			break;

		case 111: //  Меню "Таймеры > сохранённый таймер > Время и канал"
			SetChars(17, 9, 21, 23, 26, 6, 12); //  "т", "й", "ы", "...", "Я", "И", "Л"

			lcd.setCursor(0, 0);
			lcd.print(F("m>\1a\2mep\3>"));

			lcd.setCursor(10, 0);
			lcd.print(ReadTimer(valTimerNum, 1) < 10 ? "0" : "");
			lcd.print(ReadTimer(valTimerNum, 1));
			lcd.print(":");

			lcd.setCursor(13, 0);
			lcd.print(ReadTimer(valTimerNum, 2) < 10 ? "0" : "");
			lcd.print(ReadTimer(valTimerNum, 2));
			lcd.print("\4");

			lcd.setCursor(0, 1);
			lcd.print(F("<BPEM\5  \6 KAHA\7>"));
			break;

		case 1111:  //  Меню "Таймеры > сохранённый таймер > Время и канал > изменение времени и канала"
			SetChars(11, 17, 9, 21, 1, 27, 10); //  "м", "т", "й", "ы", "в", "я", "к"

			lcd.setCursor(0, 0);
			lcd.print(F("\1>\2a\3\1ep>\5pe\1\6:"));

			lcd.setCursor(1, 1);
			valChar[0] = valArray[0] / 10 + 48;
			valChar[1] = valArray[0] % 10 + 48;
			valChar[2] = 0;
			lcd.print((millis() % 1000 < 500 && valSubMode == 0) ? "  " : valChar);
			lcd.print(":");

			lcd.setCursor(4, 1);
			valChar[0] = valArray[1] / 10 + 48;
			valChar[1] = valArray[1] % 10 + 48;
			valChar[2] = 0;
			lcd.print((millis() % 1000 < 500 && valSubMode == 1) ? "  " : valChar);
			lcd.print("-");

			lcd.setCursor(7, 1);
			valChar[0] = valArray[2] / 10 + 48;
			valChar[1] = valArray[2] % 10 + 48;
			valChar[2] = 0;
			lcd.print((millis() % 1000 < 500 && valSubMode == 2) ? "  " : valChar);
			lcd.print(":");

			lcd.setCursor(10, 1);
			valChar[0] = valArray[3] / 10 + 48;
			valChar[1] = valArray[3] % 10 + 48; valChar[2] = 0;
			lcd.print((millis() % 1000 < 500 && valSubMode == 3) ? "  " : valChar);

			lcd.setCursor(13, 1);
			valChar[0] = 7;
			valChar[1] = valArray[4] + 48;
			valChar[2] = 0;
			lcd.print((millis() % 1000 < 500 && valSubMode == 4) ? "  " : valChar);
			break;

		case 112: //  Меню "Таймеры > сохранённый таймер > Повторы"
			SetChars(17, 9, 21, 23, 14, 20);  //  "т", "й", "ы", "...", "П", "Ы"

			lcd.setCursor(0, 0);
			lcd.print(F("m>\1a\2mep\3>"));

			lcd.setCursor(10, 0);
			lcd.print(ReadTimer(valTimerNum, 1) < 10 ? "0" : "");
			lcd.print(ReadTimer(valTimerNum, 1));
			lcd.print(":");

			lcd.setCursor(13, 0);
			lcd.print(ReadTimer(valTimerNum, 2) < 10 ? "0" : "");
			lcd.print(ReadTimer(valTimerNum, 2));
			lcd.print("\4");

			lcd.setCursor(0, 1);
			lcd.print(F("<   \5OBTOP\6    >"));
			break;

		case 1121:  //  Меню "Таймеры > сохранённый таймер > Повторы > изменение повторов"
			SetChars(28, 29, 30, 31, 32, 33, 34); //  "ПН", "ВТ", "СР", "ЧТ", "ПТ", "СБ", "ВС"

			lcd.setCursor(1, 0);
			lcd.print("\1 \2 \3 \4 \5 \6 \7");

			for (int i = 0; i < 7; i++)
			{
				lcd.setCursor(1 + 2 * i, 1);
				lcd.print(valArray[i] ? "^" : " ");
			}

			lcd.setCursor(valSubMode * 2 + 1, 1);
			lcd.blink();
			break;

		case 113: //  Меню "Таймеры > сохранённый таймер > Стереть таймер"
			SetChars(17, 9, 21, 23, 22, 8); //  "т", "й", "ы", "...", "Ь", "Й"

			lcd.setCursor(0, 0);
			lcd.print(F("m>\1a\2mep\3>"));

			lcd.setCursor(10, 0);
			lcd.print(ReadTimer(valTimerNum, 1) < 10 ? "0" : "");
			lcd.print(ReadTimer(valTimerNum, 1));
			lcd.print(":");

			lcd.setCursor(13, 0);
			lcd.print(ReadTimer(valTimerNum, 2) < 10 ? "0" : "");
			lcd.print(ReadTimer(valTimerNum, 2));
			lcd.print("\4");

			lcd.setCursor(0, 1);
			lcd.print(F("<CTEPET\5 TA\6MEP>"));
			break;

		case 1131:  //  Меню "Таймеры > сохранённый таймер > Стереть таймер > Таймер 
			SetChars(8, 16, 4, 12); //  "Й", "Д", "У", "Л"

			lcd.setCursor(5, 0);
			lcd.print(F("TA\1MEP"));

			lcd.setCursor(5, 1);
			lcd.print(F("\2\3A\4EH"));
			break;

		case 114: //  Меню "Таймеры > сохранённый таймер > Выход"
			SetChars(17, 9, 21, 23, 20, 4); //  "т", "й", "ы", "...", "Ы", "Д"

			lcd.setCursor(0, 0);
			lcd.print(F("m>\1a\2mep\3>"));

			lcd.setCursor(10, 0);
			lcd.print(ReadTimer(valTimerNum, 1) < 10 ? "0" : "");
			lcd.print(ReadTimer(valTimerNum, 1));
			lcd.print(":");

			lcd.setCursor(13, 0);
			lcd.print(ReadTimer(valTimerNum, 2) < 10 ? "0" : "");
			lcd.print(ReadTimer(valTimerNum, 2));
			lcd.print("\4");

			lcd.setCursor(0, 1);
			lcd.print(F("<    B\5XO\6     >"));
			break;

		case 12:  //  Меню "Таймеры > Новый таймер"
			SetChars(15, 25, 17, 9, 21, 8, 20); // "н", "ю", "т", "й", "ы", "Й", "Ы"

			lcd.setCursor(0, 0);
			lcd.print(F("me\1\2>\3a\4mep\5:"));

			lcd.setCursor(0, 1);
			lcd.print(F("< HOB\7\6 TA\6MEP >"));

			break;

		case 121: //  Меню "Таймеры > Новый таймер > Новый таймер создан"
			SetChars(20, 8, 4); //  "Ы", "Й", "Д"

			lcd.setCursor(2, 0);
			lcd.print(F("HOB\1\2 TA\2MEP"));

			lcd.setCursor(5, 1); lcd.print(F("CO3\3AH"));

			break;

		case 13:  // Меню "Таймеры > Стереть всё"
			SetChars(11, 15, 25, 17, 9, 21, 22);  //  "м", "н", "ю", "т", "й", "ы", "Ь"

			lcd.setCursor(0, 0);
			lcd.print(F("\1e\2\3>\4a\5\1ep\6:"));

			lcd.setCursor(0, 1);
			lcd.print(F("< CTEPET\7 BCE  >"));

			break;

		case 131: // Меню "Таймеры > Стереть всё > Все данные стёрты"
			SetChars(20, 8, 4, 16, 12); //  "Ы", "Й", "Д", "У", "Л"

			lcd.setCursor(2, 0);
			lcd.print(F("BCE TA\2MEP\1"));

			lcd.setCursor(4, 1);
			lcd.print(F("\4\3A\5EH\1"));
			break;

		case 14:  // Меню "Таймеры > Выход"
			SetChars(15, 25, 17, 9, 21, 4, 20); // "н", "ю", "т", "й", "ы", "Д", "Ы"

			lcd.setCursor(0, 0);
			lcd.print(F("me\1\2>\3a\4mep\5:"));

			lcd.setCursor(0, 1);
			lcd.print(F("<    B\7XO\6     >"));

			break;

		case  2:  //  Меню "Часы"
			SetChars(11, 15, 25, 18, 20);   // "м", "н", "ю", "Ч", "Ы"

			lcd.setCursor(0, 0);
			lcd.print(F("\1e\2\3:"));

			lcd.setCursor(0, 1);
			lcd.print(F("<     \4AC\5     >"));
			break;

		case 21:  //  Меню "Часы > Время"
			SetChars(11, 15, 25, 19, 21, 26); //  "м", "н", "ю", "ч", "ы", "Я"

			lcd.setCursor(0, 0);
			lcd.print(F("\1e\2\3>\4ac\5:"));

			lcd.setCursor(0, 1);
			lcd.print(F("<    BPEM\6     >"));
			break;

		case 211: //  Меню "Часы > Время > Установка времени"
			SetChars(11, 15, 25, 19, 21, 1, 27);  //  "м", "н", "ю", "ч", "ы", "в", "я"

			lcd.setCursor(0, 0);
			lcd.print(F("\1e\2\3>\4ac\5>\6pe\1\7:"));

			lcd.setCursor(4, 1);
			valChar[0] = valArray[0] / 10 + 48;
			valChar[1] = valArray[0] % 10 + 48;
			valChar[2] = 0;
			lcd.print((millis() % 1000 < 500 && valSubMode == 0) ? "  " : valChar);
			lcd.print(":");

			lcd.setCursor(7, 1);
			valChar[0] = valArray[1] / 10 + 48;
			valChar[1] = valArray[1] % 10 + 48;
			valChar[2] = 0;
			lcd.print((millis() % 1000 < 500 && valSubMode == 1) ? "  " : valChar);
			lcd.print(":");

			lcd.setCursor(10, 1);
			valChar[0] = valArray[2] / 10 + 48;
			valChar[1] = valArray[2] % 10 + 48;
			valChar[2] = 0;
			lcd.print((millis() % 1000 < 500 && valSubMode == 2) ? "  " : valChar);
			break;

		case 22:  //  Меню "Часы > Дата"
			SetChars(11, 15, 25, 19, 21, 4);  //  "м", "н", "ю", "ч", "ы", "Д"

			lcd.setCursor(0, 0);
			lcd.print(F("\1e\2\3>\4ac\5:"));

			lcd.setCursor(0, 1);
			lcd.print(F("<     \6ATA     >"));
			break;

		case 221: //  Меню "Часы > Дата > Установка даты"
			SetChars(24, 18, 20, 4);  //  "Ю", "Ч", "Ы", "Д"

			lcd.setCursor(0, 0);
			lcd.print(F("MEH\1>\2AC\3>\4ATA:"));

			lcd.setCursor(3, 1);
			valChar[0] = valArray[0] / 10 + 48;
			valChar[1] = valArray[0] % 10 + 48;
			valChar[2] = 0;
			lcd.print((millis() % 1000 < 500 && valSubMode == 0) ? "  " : valChar);
			lcd.print(".");

			lcd.setCursor(6, 1);
			valChar[0] = valArray[1] / 10 + 48;
			valChar[1] = valArray[1] % 10 + 48;
			valChar[2] = 0;
			lcd.print((millis() % 1000 < 500 && valSubMode == 1) ? "  " : valChar);
			lcd.print(".");

			lcd.setCursor(9, 1);
			valChar[0] = '2';
			valChar[1] = '0';
			valChar[2] = valArray[2] / 10 + 48;
			valChar[3] = valArray[2] % 10 + 48;
			valChar[4] = 0;
			lcd.print((millis() % 1000 < 500 && valSubMode == 2) ? "    " : valChar);
			break;

		case 23:  //  Меню "Часы > Выход"
			SetChars(11, 15, 25, 19, 21, 4, 20);  //  "м", "н", "ю", "ч", "ы", "Д", "Ы"

			lcd.setCursor(0, 0);
			lcd.print(F("\1e\2\3>\4ac\5:"));

			lcd.setCursor(0, 1);
			lcd.print(F("<    B\7XO\6     >"));
			break;

		case  3:  //  Меню "Выход"
			SetChars(11, 15, 25, 4, 20);  // "м", "н", "ю", "Д", "Ы"

			lcd.setCursor(0, 0);
			lcd.print(F("\1e\2\3:"));

			lcd.setCursor(0, 1);
			lcd.print(F("<    B\5XO\4     >"));
			break;

		case 4:    //  Меню "Работа с ПК"
			SetChars(11, 15, 25, 14, 0);   // "м", "н", "ю", "П"

			lcd.setCursor(0, 0);
			lcd.print(F("\1e\2\3: \4K"));

			lcd.setCursor(0, 1);
			lcd.print(F("  PA\5OTA C \4K   "));
			break;

		case 41:  //  Меню "Установка соединения"
			SetChars(16, 4, 6, 26);   // "У", "Д", "И", "Я"

			lcd.setCursor(0, 0);
			lcd.print(F("\1CT. COE\2\3HEH\3\4"));

			lcd.setCursor(0, 1);
			lcd.print(F("      ....      "));
			break;

		case 411: //  Меню "Соединение установлено"
			SetChars(4, 6, 16, 12);   // "Д", "И", "У", "Л"

			lcd.setCursor(0, 0);
			lcd.print(F("   COE\1\2HEH\2E   "));

			lcd.setCursor(0, 1);
			lcd.print(F("  \3CTAHOB\4EHO   "));
			break;

		case 42:  //  Меню "Передача данных на ПК"
			SetChars(14, 4, 18, 20);   // "П", "Д", "Ч", "Ы"

			lcd.setCursor(0, 0);
			lcd.print(F("\1EPE\2A\3A \2AHH\4X"));

			lcd.setCursor(0, 1);
			lcd.print(F("      ....      "));
			break;

		case 43:  //  Меню "Получение данных с ПК"
			SetChars(14, 12, 16, 18, 6, 4, 20);   // "П", "Л", "У", "Ч", "И", "Д", "Ы"

			lcd.setCursor(0, 0);
			lcd.print(F("\1O\2\3\4EH\5E \6AHH\7X"));

			lcd.setCursor(0, 1);
			lcd.print(F("      ....      "));
			break;

		case 431: //  Меню "Данные сохранены"
			SetChars(4, 20);   // "Д", "Ы"

			lcd.setCursor(0, 0);
			lcd.print(F("     \1AHH\2E     "));

			lcd.setCursor(0, 1);
			lcd.print(F("   COXPAHEH\2    "));
			break;

		case 44:  // Меню "Изменение данных расписания"
			SetChars(6, 7, 4, 20);   // "И", "З", "Д", "Ы"

			lcd.setCursor(0, 0);
			lcd.print(F("\1\2MEHEH\1E \3AHH\4X"));

			lcd.setCursor(0, 1);
			lcd.print(F("      ....      "));
			break;

		case 441: //  Меню "Данные изменены"
			SetChars(4, 20, 6, 7);   // "Д", "Ы", "И", "З"

			lcd.setCursor(0, 0);
			lcd.print(F("     \1AHH\2E     "));

			lcd.setCursor(0, 1);
			lcd.print(F("    \3\4MEHEH\2    "));
			break;


		case 45:  // Меню "Удаление расписания"
			SetChars(16, 4, 12, 6, 20);   // "У", "Д", "З", "Д", "Ы"

			lcd.setCursor(0, 0);
			lcd.print(F("\1\2A\3EH\4E  \2AHH\5X"));

			lcd.setCursor(0, 1);
			lcd.print(F("      ....      "));
			break;

		case 451: //  Меню "Данные удалены"
			SetChars(4, 20, 16, 12);   // "Д", "Ы", "У", "Л"

			lcd.setCursor(0, 0);
			lcd.print(F("     \1AHH\2E     "));

			lcd.setCursor(0, 1);
			lcd.print(F("    \3\1A\4EH\2     "));
			break;

		case 46: // Меню "Таймеры > Стереть всё > Все данные стёрты"
			SetChars(20, 8, 4, 16, 12); //  "Ы", "Й", "Д", "У", "Л"

			lcd.setCursor(2, 0);
			lcd.print(F("BCE TA\2MEP\1"));

			lcd.setCursor(4, 1);
			lcd.print(F("\4\3A\5EH\1"));
			break;

		case 5: //  Обработка ошибок
			SetChars(4, 20);   // "Д", "Ы"

			lcd.setCursor(0, 0);
			lcd.print(F("BBE\1EH\2 HEBEPH\2E"));

			lcd.setCursor(0, 1);
			lcd.print(F("     \1AHH\2E     "));
			break;

		case 6: //  Дубликат
			SetChars(4, 16, 0, 12, 6, 13, 20);   // "Д", "У", "Б", "Л", "И", "Т", "Ы"

			lcd.setCursor(0, 0);
			lcd.print(F("    \1\2\3\4\5KA\6    "));

			lcd.setCursor(0, 1);
			lcd.print(F("   \1AHH\7X !!!   "));
			break;
		}
	}
}

//  ФУНКЦИЯ, КОТОРАЯ ВКЛЮЧАЕТ НУЖНОЕ РЕЛЕ В ЗАВИСИМОСТИ ОТ РАСПИСАНИЯ
void setRelay(void) {
	if (valMode == 0) {
		uint8_t  getChanel[COUNT];  //  Чтение: Включённое (1) или выключенное (0) реле
		for (int i = 0; i < COUNT; i++)
		{
			getChanel[i] = 0;
			valArray[i] = 0;
		}
		uint32_t timeRTC = 0;     //  Текущее время в секундах от 00:00:00
		uint32_t timeTimerStart = 0;  //  Стартовое время в секундах от 00:00:00 (для цикла)
		uint32_t timeTimerStop = 0;   //  Конечное время в секундах от 00:00:00 (для цикла)
		uint8_t  timeWeekday = 0;   //  Текущей день недели в формате: 1 - ПН, 2 - ВТ, 3 - СР, 4 - ЧТ, 5 - ПТ, 6 - СБ, 7 - ВС
		timeRTC = (uint32_t)time.Hours * 3600 + time.minutes * 60 + time.seconds;
		timeWeekday = time.weekday;

		if (timeWeekday == 0) { timeWeekday = 7; }
		for (uint8_t i = 0; i < maxTimers; i++) {
			//  Если найдено расписание, то ...
			if (ReadTimer(i)) {
				//  Проверка на совпадение дня недели
				if (bitRead(ReadTimer(i, 6), 7 - timeWeekday)) {

					timeTimerStart = (uint32_t)ReadTimer(i, 1) * 3600 + ReadTimer(i, 2) * 60;
					timeTimerStop = (uint32_t)ReadTimer(i, 3) * 3600 + ReadTimer(i, 4) * 60;

					//  Проверка на попадание в интервал текущего времени
					if (timeTimerStart <= timeRTC && timeRTC < timeTimerStop) {
						getChanel[ReadTimer(i, 5) - 1] = 1;
						valArray[ReadTimer(i, 5) - 1] = 1;
					}
				}
			}
		}

		for (int i = 0; i < COUNT; i++)
		{
			if (setChanel[i] != getChanel[i]) {
				setChanel[i] = getChanel[i];
				if (getChanel[i])
					digitalWrite(relays[i], LOW);
				else
					digitalWrite(relays[i], HIGH);
			}
		}
	}
}

//   ЗАПИСЬ ДО 7 СИМВОЛОВ В CGRAM ДИСПЛЕЯ   //
void SetChars(uint8_t i1, uint8_t i2, uint8_t i3, uint8_t i4, uint8_t i5, uint8_t i6, uint8_t i7) {
	byte i[8];
	if (i1 < 255) {
		memcpy_P(i, rusMem[i1], 8);
		lcd.createChar(1, i);
	}
	if (i2 < 255) {
		memcpy_P(i, rusMem[i2], 8);
		lcd.createChar(2, i);
	}
	if (i3 < 255) {
		memcpy_P(i, rusMem[i3], 8);
		lcd.createChar(3, i);
	}
	if (i4 < 255) {
		memcpy_P(i, rusMem[i4], 8);
		lcd.createChar(4, i);
	}
	if (i5 < 255) {
		memcpy_P(i, rusMem[i5], 8);
		lcd.createChar(5, i);
	}
	if (i6 < 255) {
		memcpy_P(i, rusMem[i6], 8);
		lcd.createChar(6, i);
	}
	if (i7 < 255) {
		memcpy_P(i, rusMem[i7], 8);
		lcd.createChar(7, i);
	}
}

//   ЧТЕНИЕ ОДНОГО ИЗ ПАРАМЕТРОВ ТАЙМЕРА   //
uint8_t ReadTimer(uint8_t i, uint8_t j) {
	return EEPROM[i * 7 + j];
}

//   ПОИСК НОВОГО ТАЙМЕРА   //
uint8_t FindTimer(void) {
	uint8_t i = 0;
	while (ReadTimer(i)) {
		i++;
		if (i >= maxTimers) {
			break;
		}
	}
	return i;
}

//   СОХРАНЕНИЕ ОДНОГО ИЗ ПАРАМЕТРОВ ТАЙМЕРА   //
void SaveTimer(uint8_t i, uint8_t j, uint8_t k) {
	EEPROM[i * 7 + j] = k;
}

//   ПРОВЕРКА ДАННЫХ ТАЙМЕРОВ В EEPROM   //
bool TestTimer(void) {
	for (uint8_t i = 0; i < maxTimers; i++) {
		if (ReadTimer(i, 0) > 1) { return false; }
		if (ReadTimer(i, 1) > 23) { return false; }
		if (ReadTimer(i, 2) > 59) { return false; }
		if (ReadTimer(i, 3) > 23) { return false; }
		if (ReadTimer(i, 4) > 59) { return false; }
	}
	return true;
}

void clickButtonBack() {
	pressedButton = 1;
}

void clickButtonNext() {
	pressedButton = 3;
}

void clickButtonOK() {
	pressedButton = 2;
}

void proveVoltage() {
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
	ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
	ADMUX = _BV(MUX5) | _BV(MUX0);
#elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
	ADMUX = _BV(MUX3) | _BV(MUX2);
#else
	ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#endif

	delay(75);
	ADCSRA |= _BV(ADSC);
	while (bit_is_set(ADCSRA, ADSC));
	uint8_t low = ADCL;
	uint8_t high = ADCH;
	long result = (high << 8) | low;
	result = 1125300L / result;
	if (result < 600)
	{
		tone(buzzer, 2500, 1000);
	}

}

//  Сдвиг расписаний после удаления расписания внутри
void shift()
{
	for (int k = valArray[0]; k < valArray[1]; k++) {
		for (uint8_t l = 0; l < 7; l++) {
			SaveTimer(k, l, ReadTimer(k + 1, l));
		}
	}
	SaveTimer(valArray[1]);
}

//  Выдача информации из EEPROM
void viewEEPROM() {
	valByte = EEPROM.read(numByte);

	Serial.print(numByte);
	Serial.print("\t");
	Serial.print(valByte);
	Serial.println();

	numByte += 1;

	if (numByte == 512)
		numByte = 0;
	delay(500);
}

//  Добавление или изменение расписания по запросу с ПК
int editData(String sender, int m) {
	int mode = m / 10;
	if (Serial.available() > 0) {
		String value;
		value = Serial.readStringUntil('\n');
		valTimerNum = value.substring(0, 1).toInt();

		SaveTimer(valTimerNum, 0, 1);
		SaveTimer(valTimerNum, 1, uint8_t(value.substring(2, 4).toInt()));
		SaveTimer(valTimerNum, 2, uint8_t(value.substring(5, 7).toInt()));
		SaveTimer(valTimerNum, 3, uint8_t(value.substring(8, 10).toInt()));
		SaveTimer(valTimerNum, 4, uint8_t(value.substring(11, 13).toInt()));
		SaveTimer(valTimerNum, 5, uint8_t(value.substring(14, 15).toInt()));
		SaveTimer(valTimerNum, 6, uint8_t(value.substring(16).toInt()));
		Serial.println(sender);
		valTimerNum = 0;
		mode = m;
	}
	return mode;
}

//  Проверка правильности даты
bool checkDate()
{
	switch (valArray[1])
	{
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
		return checkDay(31);
	case 2:
		if (valArray[2] % 4 == 0 && (valArray[2] % 100 != 0 || valArray[2] % 400 == 0))
			return checkDay(29);
		else
			return checkDay(28);
	case 4:
	case 6:
	case 9:
	case 11:
		return checkDay(30);
	default:
		return false;
	}
}

//  Проверка правильности введённой даты в зависимости от месяца
bool checkDay(int count) {
	if (valArray[0] <= count)
		return true;
	else
		return false;
}

bool dublicate(uint8_t numTimer)
{
	int c = 0;
	for (int i = 0; i < FindTimer(); i++)
	{
		int startTimeS = timeSec(ReadTimer(numTimer, 1), ReadTimer(numTimer, 2));
		int endTimeS = timeSec(ReadTimer(numTimer, 3), ReadTimer(numTimer, 4));

		int startTimeI = timeSec(ReadTimer(i, 1), ReadTimer(i, 2));
		int endTimeI = timeSec(ReadTimer(i, 3), ReadTimer(i, 4));
		if ((numTimer != i) && (ReadTimer(numTimer, 5) == ReadTimer(i, 5)) &&
			(((startTimeS < endTimeI) && (startTimeS >= startTimeI)) ||
			((endTimeS > startTimeI) && (endTimeS <= endTimeI)) ||
				((startTimeS < startTimeI) && (endTimeS > endTimeI))
				) && (ReadTimer(numTimer, 6) & ReadTimer(i, 6) != 0)
			)
			c++;
	}
	if (c != 0)
		return true;
	else
		return false;
}

int timeSec(int t1, int t2)
{
	return t2 * 60 + t1 * 60 * 60;
}

//  Переключение режимов в зависимости от задачи с ПК
int proveSP()
{
	if (connectPC) {
		char s = Serial.read();
		switch (s) {

			//  turn off - отсоединение ардуино
		case 't':
			connectPC = false;
			return 0;
			break;

			// read - переход в режим чтения данных с ардуино 
		case 'r':
			return 42;
			break;

			// save - переход в режим сохранения данных с ПК
		case 's':
			return 43;
			break;

			// edit - Изменение данных расписания
		case 'e':
			return 44;
			break;

			// delete - удаление расписания
		case 'd':
			return 45;
			break;

		case 'h':
			return 46;

		default:
			return 4;
		}
	}
	else
		return 0;
}
