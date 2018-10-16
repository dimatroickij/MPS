#include "OneButton.h"
#include <iarduino_RTC.h>
#include <avr/pgmspace.h>                                            //  ���������� ���������� ��� ������ � PROGMEM Arduino ��� �������� �������� �������
#include <EEPROM.h>                                                  //  ���������� ���������� ��� ������ � EEPROM  Arduino ��� �������� �������� ��������
#include <Wire.h>                                                    //  ���������� ���������� ��� ������ � ����� I2C
#include <LiquidCrystal_I2C.h>                                       //  ���������� ���������� ��� ������ � LCD �������� �� ���� I2C


#define      OK    2
#define     BACK  1
#define     NEXT  3

//  ������ ARDUINO  //
const uint8_t relay1 = 5;   //  ����� �� 1 ����
const uint8_t relay2 = 6;   //  ����� �� 2 ����
const uint8_t relay3 = 9;   //  ����� �� 3 ����
const uint8_t relay4 = 10;  //  ����� �� 4 ����
const uint8_t buzzer = 12;  //  ����� �������

//   ��������� � ����������, ����������� ��� ������ ������: //

//  ������� ������� ��������
const byte      rusMem[40][8] PROGMEM = {
  { 31, 16, 16, 30, 17, 17, 30, 0 }, { 0, 0, 30, 17, 30, 17, 30, 0 }, //  �,  �,  � ������� ������� � �������: 00, 01
  { 31, 16, 16, 16, 16, 16, 16, 0 }, { 0, 0, 30, 16, 16, 16, 16, 0 }, //  �,  �,  � ������� ������� � �������: 02, 03
  { 6, 10, 10, 10, 10, 10, 31, 17 }, { 10, 10, 14, 2, 2, 0, 0, 0 },   //  �,  4,  � ������� ������� � �������: 04, 05
  { 17, 17, 17, 19, 21, 25, 17, 0 }, {31, 2, 4, 2, 1, 17, 14, 0 },    //  �,  �,  � ������� ������� � �������: 06, 07
  { 21, 17, 17, 19, 21, 25, 17, 0 }, { 10,4,17,19,21,25,17, 0 },    //  �,  �,  � ������� ������� � �������: 08, 09
  { 0, 0, 18, 20, 24, 20, 18, 0 }, { 0, 0,17,27,21,17,17, 0 },      //  �,  �,  � ������� ������� � �������: 10, 11
  { 7, 9, 9, 9, 9, 9, 17, 0 }, { 0, 0, 7, 9, 9, 9,17, 0 },        //  �,  �,  � ������� ������� � �������: 12, 13
  { 31, 17, 17, 17, 17, 17, 17, 0 }, { 0, 0, 17, 17, 31, 17, 17, 0 }, //  �   �,  � ������� ������� � �������: 14, 15
  { 17, 17, 17, 15, 1, 17, 14, 0 }, { 0, 0, 31, 4, 4, 4, 4, 0 },    //  �,  �,  � ������� ������� � �������: 16, 17
  { 17, 17, 17, 15, 1, 1, 1, 0 }, { 0, 0, 17, 17, 15, 1, 1, 0 },    //  �,  �,  � ������� ������� � �������: 18, 19
  { 17, 17, 17, 29, 19, 19, 29, 0 }, { 0, 0, 17, 17, 29, 19, 29, 0 }, //  �,  �,  � ������� ������� � �������: 20, 21
  { 16, 16, 16, 30, 17, 17, 30, 0 }, { 0, 0, 0, 0, 0, 0, 21, 0 },   //  �, ..., � ������� ������� � �������: 22, 23
  { 18, 21, 21, 29, 21, 21, 18, 0 }, { 0, 0, 18, 21, 29, 21, 18, 0 }, //  �,  �,  � ������� ������� � �������: 24, 25
  { 15, 17, 17, 15, 5, 9, 17, 0 }, { 0, 0, 15, 17, 15, 5, 9, 0 },   //  �,  �,  � ������� ������� � �������: 26, 27
  { 31, 4, 31, 0, 31, 16, 31, 0 }, { 16, 31, 16, 0, 10, 21, 31, 0 },  //  ��, ��  � ������� ������� � �������: 28, 29
  { 28, 20, 31, 0, 17, 17, 31, 0 }, { 16, 31, 16, 0, 31, 4, 28, 0 },  //  ��, ��  � ������� ������� � �������: 30, 31
  { 16, 31, 16, 0, 31, 16, 31, 0 }, { 23, 21, 31, 0, 17, 17, 31, 0 }, //  ��, ��  � ������� ������� � �������: 32, 33
  { 17, 17, 31, 0, 10, 21, 31, 0 }, { 4, 12, 4, 4, 14, 0, 0, 0 },   //  ��, 1   � ������� ������� � �������: 34, 35
  { 14, 2, 14, 8, 14, 0, 0, 0 }, { 14, 2, 14, 2, 14, 0, 0, 0 },     //  2, 3 � ������� ������� � �������: 36, 37
  { 18, 18, 18, 18, 18, 18, 31, 1 }, {17, 17, 19, 13, 1, 17, 14, 0} };  //  �, � � ������� ������� � �������: 38, 39

const int maxTimers = 20;           //  ������������ ���������� ���������� (�������� 146)
int     pressedButton = 0;          //  ������� ������
uint8_t   valArray[7] = { 0,0,0,0,0,0,0 };  //  ���������� ������ �������� �������� ����� ������� ��������� ���������� � ����������� �� ������
char    valChar[5] = "    ";        //  ���������� ������ �������� (������) ���������� ������� ����� ������������ �� ������� �����
int     valMode = 0;            //  ������� �����
uint8_t   valSubMode = 0;           //  ������� ��������
uint8_t   valTimerNum = 0;          //  ��������� ����������
bool    flgDisplayUpdate = 1;       //  ���� ���������� �������
int     numByte = 0;            //  ����� ����� EEPROM
int     valByte;              //  �������� ����� EEPROM
bool  connectPC = false;

LiquidCrystal_I2C lcd(0x27, 16, 2);     //  ������ - lcd ��� ������ � �������� (����� I2C = 0x27, ���������� �������� = 16, ���������� ����� = 2)
iarduino_RTC    time(RTC_DS1307);     //  ������ - time ��� ������ � ������ RTC (��� ������)
OneButton buttonBack(14, 1);          //  ������ - ������ "�����" (� ����, )
OneButton buttonNext(15, 1);          //  ������ - ������ "�����" (� ����, )
OneButton buttonOk(16, 1);          //  ������ - ������ "��" (� ����, )

//  �������, ������������ � ��������� //
void    buttonRead(void);       //  �������, ����������� ���������� �������� � ����������� �� ������� ������
void    displayUpdate(void);      //  ������� ����������� ������ �� �����
void    setRelay(void);         //  �������, ������� �������� ������ ���� � ����������� �� ����������
void    proveVoltage(void);       //  ������� �������� ����������
void    viewEEPROM(void);         //  �������� �������� EEPROM � COM �����
void    SetChars(           //  ������� ������ ������� �������� � ������ ������� (�������� 7 ��������)
	uint8_t = 255,
	uint8_t = 255,
	uint8_t = 255,
	uint8_t = 255,
	uint8_t = 255,
	uint8_t = 255,
	uint8_t = 255);
uint8_t   ReadTimer(uint8_t = 0,      //  ������ ���������� ������� (� �������, � ���������)
	uint8_t = 0);
void    SaveTimer(uint8_t = 0,      //  ������� ������ ������ �� ���������� ������� (� �������, � ���������, �������� ���������)
	uint8_t = 0,
	uint8_t = 0);
uint8_t   FindTimer(void);        //  ������� ������ � ���������� ���������� ����� ��� ������ ����������
bool    TestTimer(void);        //  ������� �������� ������ � ������� EEPROM �� ������������ (������ ������ ���������)

void setup() {

	Serial.begin(9600);

	//  ������ �� ���� � ������� � ����� ������ //
	pinMode(relay1, OUTPUT);
	pinMode(relay2, OUTPUT);
	pinMode(relay3, OUTPUT);
	pinMode(relay4, OUTPUT);
	pinMode(buzzer, OUTPUT);

	//  ������ �� ���� � ������� ��������� (���������� ����)  //
	digitalWrite(relay1, HIGH);
	digitalWrite(relay2, HIGH);
	digitalWrite(relay3, HIGH);
	digitalWrite(relay4, HIGH);

	time.begin();
	lcd.init();
	lcd.backlight();

	//  ����������� ������� "������� �� ������" //
	buttonBack.attachClick(clickButtonBack);
	buttonNext.attachClick(clickButtonNext);
	buttonOk.attachClick(clickButtonOK);

	SetChars(16, 26, 14, 6, 38, 8, 4);    //  "�", "�", "�", "�", "�", "�", "�"
	lcd.setCursor(0, 0);
	lcd.print(F("K\1PCOBA\2 \3O M\3C"));
	lcd.setCursor(1, 1);
	lcd.print(F("TPO\4\5K\4\6 \7. A."));
	delay(1000);
	lcd.clear();

	//  �������� �� ������ ������ ���������   //
	if (!TestTimer()) {
		SetChars(14, 4, 2);         //  "�", "�" � "�"
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
	//  ��������� ������������ ��������� ������ //
	buttonBack.tick();
	buttonNext.tick();
	buttonOk.tick();

	buttonRead();
	displayUpdate();
	setRelay();
	proveVoltage();

	//viewEEPROM();
}

//  �������, ����������� ���������� �������� � ����������� �� ������� ������  //
void buttonRead(void) {
	int j = 3000; //  �����, � ������� ��������� ��������� (3000 - �������� � ��� �� ������)
	switch (valMode) {

	case  0:  //  ������� ����
		if (pressedButton == OK) { j = 1; }
		flgDisplayUpdate = 1;
		pressedButton = 0;
		break;

	case  1:  //  ���� "�������"
		if (pressedButton == OK) {
			j = ReadTimer() ? 11 : 12;  //  �������� �� ������������� ���� �� ������ ����������. ���� ���, �� ���� "������� > ����� ������"
			valTimerNum = 0;
		}
		if (pressedButton == BACK) { j = 4; }
		if (pressedButton == NEXT) { j = 2; }
		pressedButton = 0;
		break;

	case  2:  //  ���� "����"
		if (pressedButton == OK) { j = 21; }
		if (pressedButton == BACK) { j = 1; }
		if (pressedButton == NEXT) { j = 3; }
		pressedButton = 0;
		break;

	case 3:   //  ���� "������ � ��"
		if (pressedButton == OK) { j = 31; }
		if (pressedButton == BACK) { j = 2; }
		if (pressedButton == NEXT) { j = 4; }
		pressedButton = 0;
		break;

	case  4:  //  ���� "�����"
		if (pressedButton == OK) {
			j = 0;
			valArray[0] = valArray[1] = valArray[2] = valArray[3] = 0;
		}
		if (pressedButton == BACK) { j = 3; }
		if (pressedButton == NEXT) { j = 1; }
		pressedButton = 0;
		break;

	case 11:  //  ���� "������� > ���������� ������"
		if (pressedButton == OK) { j = 111; }
		if (pressedButton == BACK) {
			if (valTimerNum) {  //  ���� ���������� ���������� ��� ����
				valTimerNum--;
				lcd.clear();
				flgDisplayUpdate = 1;
			}
			else { j = 14; }
		}
		if (pressedButton == NEXT) {
			valTimerNum++;
			if (valTimerNum >= maxTimers) { j = 13; } //  ���� ���������� ����. ���-�� ����������
			else if (ReadTimer(valTimerNum)) {    //  ���� ��������� ���������� ����������
				lcd.clear();
				flgDisplayUpdate = 1;
			}
			else { j = 12; }
		}
		pressedButton = 0;
		break;

	case 12:  //  ���� "������� > ����� ������"
		if (pressedButton == OK) { j = 121; }
		if (pressedButton == BACK) {
			j = ReadTimer() ? 11 : 14;  //  �������� �� ������������� ���� �� ������ ����������. ���� ���, �� ���� " ������� > �����"
			if (ReadTimer()) {
				valTimerNum = FindTimer() - 1;
			}
		}
		if (pressedButton == NEXT) { j = ReadTimer() ? 13 : 14; } //  �������� �� ������������� ���� �� ������ ����������. ���� ����, �� ����������� ���� "������� ��".
		pressedButton = 0;
		break;

	case 13:  // ���� "������� > ������� ��"
		if (pressedButton == OK) { j = 131; }
		if (pressedButton == BACK) {
			j = FindTimer() < maxTimers ? 12 : 11;  //  �������� �� ���������� ����� ����������. ���� �����������, �� ����������� ��������� ����������
			valTimerNum = j == 11 ? maxTimers - 1 : 0;
		}
		if (pressedButton == NEXT) {
			j = 14;
			valTimerNum = 0;
		}
		pressedButton = 0;
		break;

	case 14:  // ���� "������� > �����"
		if (pressedButton == OK) { j = 1; }
		if (pressedButton == BACK) {
			j = ReadTimer() ? 13 : (FindTimer() < maxTimers ? 12 : 11); //  �������� �� ������������� ���� �� ������ ����������. ���� ����, �� ���� "������� > ������� ��"
			valTimerNum = 0;
		}
		if (pressedButton == NEXT) {
			j = ReadTimer() ? 11 : (FindTimer() < maxTimers ? 12 : 13); // �������� �� ������������� ���� �� ������ ����������. ���� ���, �� �������� �� ��������������� ���������� ���������� 
			valTimerNum = 0;
		}
		pressedButton = 0;
		break;

	case 21:  //  ���� "���� > �����"
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
		pressedButton = 0;
		break;

	case 22:  //  ���� "���� > ����"
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
		pressedButton = 0;
		break;

	case 23:  //  ���� "���� > �����"
		if (pressedButton == OK) { j = 2; }
		if (pressedButton == BACK) { j = 22; }
		if (pressedButton == NEXT) { j = 21; }
		pressedButton = 0;
		break;

	case 211: //  ���� "���� > ����� > ��������� �������"
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
		pressedButton = 0;
		break;

	case 221: //  ���� "���� > ���� > ��������� ����"
		if (pressedButton == OK) {
			if (valSubMode == 0) { valSubMode = 1; }
			else if (valSubMode == 1) { valSubMode = 2; }
			else if (valSubMode == 2)
			{
				int a, y, m, R;
				a = (14 - valArray[1]) / 12;
				y = valArray[2] - a;
				m = valArray[1] + 12 * a - 2;
				R = 7000 + (valArray[0] + y + y / 4 - y / 100 + y / 400 + (31 * m) / 12);
				valArray[3] = R % 7;
				time.settime(-1, -1, -1, valArray[0], valArray[1], valArray[2], valArray[3]);
				j = 22;
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
		pressedButton = 0;
		break;

	case 121: //  ���� "������� > ����� ������ > ����� ������ ������"
		j = 1111;
		valTimerNum = FindTimer();
		valSubMode = 0;
		valArray[0] = valArray[1] = valArray[2] = valArray[3] = 0;
		valArray[4] = 1;
		SaveTimer(valTimerNum, 0, 1);
		SaveTimer(valTimerNum, 1);
		SaveTimer(valTimerNum, 2);
		SaveTimer(valTimerNum, 3);
		SaveTimer(valTimerNum, 4);
		SaveTimer(valTimerNum, 5, 1);
		SaveTimer(valTimerNum, 6, 127);

		delay(2000);
		pressedButton = 0;
		break;

	case 131: // ���� "������� > ������� �� > ��� ������ �����"
		j = 12;
		for (valArray[0] = 0; valArray[0] < maxTimers; valArray[0] ++) { SaveTimer(valArray[0]); }
		delay(2000);
		pressedButton = 0;
		break;

	case 1131:  //  ���� "������� > ���������� ������ > ������� ������ > ������ �����"
		j = 12;
		valArray[0] = valTimerNum;
		valArray[1] = FindTimer() - 1;

		// ����� ���������� ����������
		if (valArray[0] < valArray[1]) {
			j = 11;
			valTimerNum = valArray[0];
		}
		else if (valArray[1] > 0) {
			j = 11;
			valTimerNum = valArray[0] - 1;
		}

		//  ����� ��������� ���������� �� 7 ���� �����
		for (int k = valArray[0]; k < valArray[1]; k++) {
			for (uint8_t l = 0; l < 7; l++) {
				SaveTimer(k, l, ReadTimer(k + 1, l));
			}
		}
		SaveTimer(valArray[1]);
		delay(2000);
		pressedButton = 0;
		break;

	case 111: //  ���� "������� > ���������� ������ > ����� � �����"
		if (pressedButton == OK) {
			j = 1111;
			valSubMode = 0;
			valArray[0] = ReadTimer(valTimerNum, 1);  //  ��� ���������� �������
			valArray[1] = ReadTimer(valTimerNum, 2);  //  ������ ���������� �������
			valArray[2] = ReadTimer(valTimerNum, 3);  //  ��� ��������� �������
			valArray[3] = ReadTimer(valTimerNum, 4);  //  ������ ��������� �������
			valArray[4] = ReadTimer(valTimerNum, 5);  //  ����� �������
		}
		if (pressedButton == BACK) { j = 114; }
		if (pressedButton == NEXT) { j = 112; }
		pressedButton = 0;
		break;

	case 112: //  ���� "������� > ���������� ������ > �������"
		if (pressedButton == OK) {
			j = 1121;
			valSubMode = 0;
			valArray[0] = bitRead(ReadTimer(valTimerNum, 6), 6);
			valArray[1] = bitRead(ReadTimer(valTimerNum, 6), 5);
			valArray[2] = bitRead(ReadTimer(valTimerNum, 6), 4);
			valArray[3] = bitRead(ReadTimer(valTimerNum, 6), 3);
			valArray[4] = bitRead(ReadTimer(valTimerNum, 6), 2);
			valArray[5] = bitRead(ReadTimer(valTimerNum, 6), 1);
			valArray[6] = bitRead(ReadTimer(valTimerNum, 6), 0);
		}
		if (pressedButton == BACK) { j = 111; }
		if (pressedButton == NEXT) { j = 113; }
		pressedButton = 0;
		break;

	case 113: //  ���� "������� > ���������� ������ > ������� ������"
		if (pressedButton == OK) { j = 1131; }
		if (pressedButton == BACK) { j = 112; }
		if (pressedButton == NEXT) { j = 114; }
		pressedButton = 0;
		break;

	case 114: //  ���� "������� > ���������� ������ > �����"
		if (pressedButton == OK) { j = 11; }
		if (pressedButton == BACK) { j = 113; }
		if (pressedButton == NEXT) { j = 111; }
		pressedButton = 0;
		break;

	case 1111:  //  ���� "������� > ���������� ������ > ����� � ����� > ��������� ������� � ������"
		if (pressedButton == OK) {
			if (valSubMode == 0) { valSubMode = 1; }
			else if (valSubMode == 1) { valSubMode = 2; }
			else if (valSubMode == 2) { valSubMode = 3; }
			else if (valSubMode == 3) { valSubMode = 4; }
			else if (valSubMode == 4) {
				j = 111;
				SaveTimer(valTimerNum, 0, 1);
				SaveTimer(valTimerNum, 1, valArray[0]);
				SaveTimer(valTimerNum, 2, valArray[1]);
				SaveTimer(valTimerNum, 3, valArray[2]);
				SaveTimer(valTimerNum, 4, valArray[3]);
				SaveTimer(valTimerNum, 5, valArray[4]);
			}
		}
		if (pressedButton == BACK) {
			valArray[valSubMode] --;
			if (valArray[0] > 23) { valArray[0] = 23; }
			if (valArray[1] > 59) { valArray[1] = 59; }
			if (valArray[2] > 23) { valArray[2] = 23; }
			if (valArray[3] > 59) { valArray[3] = 59; }
			if (valArray[4] == 0) { valArray[4] = 4; }
		}
		if (pressedButton == NEXT) {
			valArray[valSubMode]++;
			if (valArray[0] > 23) { valArray[0] = 0; }
			if (valArray[1] > 59) { valArray[1] = 0; }
			if (valArray[2] > 23) { valArray[2] = 0; }
			if (valArray[3] > 59) { valArray[3] = 0; }
			if (valArray[4] > 4) { valArray[4] = 1; }
		}
		flgDisplayUpdate = 1;
		pressedButton = 0;
		break;

	case 1121:  //  ���� "������� > ���������� ������ > ������� > ��������� ��������"
		if (pressedButton == OK) {
			flgDisplayUpdate = 1;
			if (valSubMode == 0) { valSubMode = 1; }
			else if (valSubMode == 1) { valSubMode = 2; }
			else if (valSubMode == 2) { valSubMode = 3; }
			else if (valSubMode == 3) { valSubMode = 4; }
			else if (valSubMode == 4) { valSubMode = 5; }
			else if (valSubMode == 5) { valSubMode = 6; }
			else if (valSubMode == 6) {
				j = 112;
				uint8_t k = 0;
				bitWrite(k, 6, valArray[0]);
				bitWrite(k, 5, valArray[1]);
				bitWrite(k, 4, valArray[2]);
				bitWrite(k, 3, valArray[3]);
				bitWrite(k, 2, valArray[4]);
				bitWrite(k, 1, valArray[5]);
				bitWrite(k, 0, valArray[6]);
				SaveTimer(valTimerNum, 6, k);
				lcd.noBlink();
			}
		}
		if ((pressedButton == BACK) || (pressedButton == NEXT)) {
			flgDisplayUpdate = 1;
			if (valArray[valSubMode]) { valArray[valSubMode] = 0; }
			else { valArray[valSubMode] = 1; }
		}
		pressedButton = 0;
		break;

	case 31:    //  ���� "������ � �� > ��������� ����������"
		if (pressedButton == OK) {
			j = 311;
		}
		if (pressedButton == BACK) {
			j = 35;
		}
		if (pressedButton == NEXT) {
			j = 35;
		}
		pressedButton = 0;
		break;

	case 32:    //  ���� "������ � �� > �������� ������"
		if (!proveConnection())
			j = 31;
		else {
			if (pressedButton == OK) {
				j = 321;
			}
			if (pressedButton == BACK) {
				j = 34;
			}
			if (pressedButton == NEXT) {
				j = 33;
			}
		}
		pressedButton = 0;
		break;

	case 33:    //  ���� "������ � �� > ��������� ������"
		if (!proveConnection())
			j = 31;
		else {
			if (pressedButton == OK) {
				j = 331;
			}
			if (pressedButton == BACK) {
				j = 32;
			}
			if (pressedButton == NEXT) {
				j = 34;
			}
		}
		pressedButton = 0;
		break;

	case 34:    //  ���� "������ � �� > ������� ����������"
		if (!proveConnection())
			j = 31;
		else {
			if (pressedButton == OK) {
				connectPC = false;
				j = 31;
			}
			if (pressedButton == BACK) {
				j = 33;
			}
			if (pressedButton == NEXT) {
				j = 32;
			}
		}
		pressedButton = 0;
		break;

	case 35:    //  ���� "������ � �� > �����"
		if (pressedButton == OK) {
			j = 3;
		}
		if (pressedButton == BACK) {
			j = 31;
		}
		if (pressedButton == NEXT) {
			j = 31;
		}
		pressedButton = 0;
		break;

	case 311: //  ���� "��������� ����������"
		if (!connectPC) {
			if (Serial.available() > 0) {
				if (Serial.read() == 'y')
				{
					connectPC = true;
					j = 312;
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
		if (pressedButton == OK) {
			j = 31;
			connectPC = false;
		}
		pressedButton = 0;
		break;

	case 312: //  ���� "���������� �����������"
		delay(2000);
		pressedButton = 0;
		j = 32;
		break;

	case 321: //  ���� "�������� ������ �� ��"
		if (pressedButton == OK) {
			j = 32;
		}
		pressedButton = 0;
		break;

	case 331: //  ���� "��������� ������ � ��"
		if (pressedButton == OK) {
			j = 33;
		}
		else {
			if (Serial.available() > 0) {
				String value;
				value = Serial.readStringUntil('\n');
				valTimerNum = FindTimer();

				SaveTimer(valTimerNum, 0, 1);
				SaveTimer(valTimerNum, 1, uint8_t(value.substring(0, 2).toInt()));
				SaveTimer(valTimerNum, 2, uint8_t(value.substring(3, 5).toInt()));
				SaveTimer(valTimerNum, 3, uint8_t(value.substring(6, 8).toInt()));
				SaveTimer(valTimerNum, 4, uint8_t(value.substring(9, 11).toInt()));
				SaveTimer(valTimerNum, 5, uint8_t(value.substring(12, 13).toInt()));
				SaveTimer(valTimerNum, 6, uint8_t(value.substring(14).toInt()));
				
				delay(2000);
				Serial.println("y");
			}
		}
		pressedButton = 0;
		break;


	}

	//  ��������� �������� ��������� �� ���������, ���������� ������ �� ������
	if (j < 3000) {
		lcd.clear();
		flgDisplayUpdate = 1;
		valMode = j;
	}
}
//  ���������� ���������� �� �������  //
void displayUpdate() {
	if (flgDisplayUpdate) {
		flgDisplayUpdate = 0;
		switch (valMode) {

		case  0:  //  ������� ����
			SetChars(35, 36, 37, 5, 3, 0, time.weekday == 4 ? 18 : 14); // "^1", "^2", "^3", "^4", "�", "�", "�/�"

			// 00:00:00
			lcd.setCursor(0, 0);
			lcd.print(time.gettime("H:i:s"));

			// 00.00.0000�. ��"
			lcd.setCursor(0, 1);
			lcd.print(time.gettime("d.m.Y\5."));

			lcd.setCursor(13, 1);
			lcd.print(time.weekday == 1 ? "\7H" :
				(time.weekday == 2 ? "BT" :
				(time.weekday == 3 ? "CP" :
					(time.weekday == 4 ? "\7T" :
					(time.weekday == 5 ? "\7T" :
						(time.weekday == 6 ? "C\6" : ("BC")))))));

			lcd.setCursor(12, 0);
			lcd.print("    ");
			valArray[4] = 15;
			if (valArray[3]) {
				lcd.setCursor(valArray[4], 0);
				lcd.print("\4");
				valArray[4]--;
			}
			if (valArray[2]) {
				lcd.setCursor(valArray[4], 0);
				lcd.print("\3");
				valArray[4]--;
			}
			if (valArray[1]) {
				lcd.setCursor(valArray[4], 0);
				lcd.print("\2");
				valArray[4]--;
			}
			if (valArray[0]) {
				lcd.setCursor(valArray[4], 0);
				lcd.print("\1");
				valArray[4]--;
			}
			break;

		case  1:  //  ���� "�������"
			SetChars(11, 15, 25, 9, 20);  // "�", "�", "�", "�", "�"

			lcd.setCursor(0, 0);
			lcd.print(F("\1e\2\3:"));

			lcd.setCursor(0, 1);
			lcd.print(F("<   TA\4MEP\5    >"));
			break;

		case  2:  //  ���� "����"
			SetChars(11, 15, 25, 18, 20);   // "�", "�", "�", "�", "�"

			lcd.setCursor(0, 0);
			lcd.print(F("\1e\2\3:"));

			lcd.setCursor(0, 1);
			lcd.print(F("<     \4AC\5     >"));
			break;


		case  3:  //  ���� "������ � ��"
			SetChars(11, 15, 25, 0, 14);   // "�", "�", "�", "�", "�"

			lcd.setCursor(0, 0);
			lcd.print(F("\1e\2\3:"));

			lcd.setCursor(0, 1);
			lcd.print(F("< PA\4OTA C \5K  >"));
			break;

		case  4:  //  ���� "�����"
			SetChars(11, 15, 25, 4, 20);  // "�", "�", "�", "�", "�"

			lcd.setCursor(0, 0);
			lcd.print(F("\1e\2\3:"));

			lcd.setCursor(0, 1);
			lcd.print(F("<    B\5XO\4     >"));
			break;

		case 11:  //  ���� "������� > ���������� ������"
			SetChars(11, 15, 25, 17, 9, 21);  //  "�", "�", "�", "�", "�", "�"

			lcd.setCursor(0, 0);
			lcd.print(F("\1e\2\3>\4a\4\1ep\6:"));

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

		case 12:  //  ���� "������� > ����� ������"
			SetChars(15, 25, 17, 9, 21, 8, 20); // "�", "�", "�", "�", "�", "�", "�"

			lcd.setCursor(0, 0);
			lcd.print(F("me\1\2>\3a\4mep\5:"));

			lcd.setCursor(0, 1);
			lcd.print(F("< HOB\7\6 TA\6MEP >"));

			break;

		case 13:  // ���� "������� > ������� ��"
			SetChars(11, 15, 25, 17, 9, 21, 22);  //  "�", "�", "�", "�", "�", "�", "�"

			lcd.setCursor(0, 0);
			lcd.print(F("\1e\2\3>\4a\5\1ep\6:"));

			lcd.setCursor(0, 1);
			lcd.print(F("< CTEPET\7 BCE  >"));

			break;

		case 14:  // ���� "������� > �����"
			SetChars(15, 25, 17, 9, 21, 4, 20); // "�", "�", "�", "�", "�", "�", "�"

			lcd.setCursor(0, 0);
			lcd.print(F("me\1\2>\3a\4mep\5:"));

			lcd.setCursor(0, 1);
			lcd.print(F("<    B\7XO\6     >"));

			break;

		case 21:  //  ���� "���� > �����"
			SetChars(11, 15, 25, 19, 21, 26); //  "�", "�", "�", "�", "�", "�"

			lcd.setCursor(0, 0);
			lcd.print(F("\1e\2\3>\4ac\5:"));

			lcd.setCursor(0, 1);
			lcd.print(F("<    BPEM\6     >"));
			break;

		case 22:  //  ���� "���� > ����"
			SetChars(11, 15, 25, 19, 21, 4);  //  "�", "�", "�", "�", "�", "�"

			lcd.setCursor(0, 0);
			lcd.print(F("\1e\2\3>\4ac\5:"));

			lcd.setCursor(0, 1);
			lcd.print(F("<     \6ATA     >"));
			break;

		case 23:  //  ���� "���� > �����"
			SetChars(11, 15, 25, 19, 21, 4, 20);  //  "�", "�", "�", "�", "�", "�", "�"

			lcd.setCursor(0, 0);
			lcd.print(F("\1e\2\3>\4ac\5:"));

			lcd.setCursor(0, 1);
			lcd.print(F("<    B\7XO\6     >"));
			break;

		case 211: //  ���� "���� > ����� > ��������� �������"
			SetChars(11, 15, 25, 19, 21, 1, 27);  //  "�", "�", "�", "�", "�", "�", "�"

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

		case 221: //  ���� "���� > ���� > ��������� ����"
			SetChars(24, 18, 20, 4);  //  "�", "�", "�", "�"

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

		case 121: //  ���� "������� > ����� ������ > ����� ������ ������"
			SetChars(20, 8, 4); //  "�", "�", "�"

			lcd.setCursor(2, 0);
			lcd.print(F("HOB\1\2 TA\2MEP"));

			lcd.setCursor(5, 1); lcd.print(F("CO3\3AH"));

			break;

		case 131: // ���� "������� > ������� �� > ��� ������ �����"
			SetChars(20, 8, 4, 16, 12); //  "�", "�", "�", "�", "�"

			lcd.setCursor(2, 0);
			lcd.print(F("BCE TA\2MEP\1"));

			lcd.setCursor(4, 1);
			lcd.print(F("\4\3A\5EH\1"));
			break;

		case 1131:  //  ���� "������� > ���������� ������ > ������� ������ > ������ 
			SetChars(8, 16, 4, 12); //  "�", "�", "�", "�"

			lcd.setCursor(5, 0);
			lcd.print(F("TA\1MEP"));

			lcd.setCursor(5, 1);
			lcd.print(F("\2\3A\4EH"));
			break;

		case 111: //  ���� "������� > ���������� ������ > ����� � �����"
			SetChars(17, 9, 21, 23, 26, 6, 12); //  "�", "�", "�", "...", "�", "�", "�"

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

		case 112: //  ���� "������� > ���������� ������ > �������"
			SetChars(17, 9, 21, 23, 14, 20);  //  "�", "�", "�", "...", "�", "�"

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

		case 113: //  ���� "������� > ���������� ������ > ������� ������"
			SetChars(17, 9, 21, 23, 22, 8); //  "�", "�", "�", "...", "�", "�"

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

		case 114: //  ���� "������� > ���������� ������ > �����"
			SetChars(17, 9, 21, 23, 20, 4); //  "�", "�", "�", "...", "�", "�"

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

		case 1111:  //  ���� "������� > ���������� ������ > ����� � ����� > ��������� ������� � ������"
			SetChars(11, 17, 9, 21, 1, 27, 10); //  "�", "�", "�", "�", "�", "�", "�"

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

		case 1121:  //  ���� "������� > ���������� ������ > ������� > ��������� ��������"
			SetChars(28, 29, 30, 31, 32, 33, 34); //  "��", "��", "��", "��", "��", "��", "��"

			lcd.setCursor(1, 0);
			lcd.print("\1 \2 \3 \4 \5 \6 \7");

			lcd.setCursor(1, 1);
			lcd.print(valArray[0] ? "^" : " ");

			lcd.setCursor(3, 1);
			lcd.print(valArray[1] ? "^" : " ");

			lcd.setCursor(5, 1);
			lcd.print(valArray[2] ? "^" : " ");

			lcd.setCursor(7, 1);
			lcd.print(valArray[3] ? "^" : " ");

			lcd.setCursor(9, 1);
			lcd.print(valArray[4] ? "^" : " ");

			lcd.setCursor(11, 1);
			lcd.print(valArray[5] ? "^" : " ");

			lcd.setCursor(13, 1);
			lcd.print(valArray[6] ? "^" : " ");

			lcd.setCursor(valSubMode * 2 + 1, 1);
			lcd.blink();
			break;

		case 31:    //  ���� "������ � �� > ����������"
			SetChars(11, 15, 25, 14, 4, 6);   // "�", "�", "�", "�", "�", "�"

			lcd.setCursor(0, 0);
			lcd.print(F("\1e\2\3: \4K"));

			lcd.setCursor(0, 1);
			lcd.print(F("<  COE\5\6HEH\6E  >"));
			break;

		case 32:    //  ���� "������ � �� > �������� ������"
			SetChars(11, 15, 25, 14, 4, 6, 18);   // "�", "�", "�", "�", "�", "�", "�"

			lcd.setCursor(0, 0);
			lcd.print(F("\1e\2\3: \4K"));

			lcd.setCursor(0, 1);
			lcd.print(F("<   \4EPE\5A\7A   >"));
			break;

		case 33:    //  ���� "������ � �� > ��������� ������"
			SetChars(6, 15, 25, 14, 12, 16, 18);   // "�", "�", "�", "�", "�", "�", "�"

			lcd.setCursor(0, 0);
			lcd.print(F("me\2\3: \4K"));

			lcd.setCursor(0, 1);
			lcd.print(F("<   \4O\5\6\7EH\1E  >"));
			break;

		case 34:    //  ���� "������ � �� > ��������� ����������"
			SetChars(6, 15, 25, 14, 12, 24, 18);   // "�", "�", "�", "�", "�", "�", "�"

			lcd.setCursor(0, 0);
			lcd.print(F("me\2\3: \4K"));

			lcd.setCursor(0, 1);
			lcd.print(F("<  OTK\5\6\7EH\1E  >"));
			break;

		case 35:    //  ���� "������ � �� > �����"
			SetChars(11, 15, 25, 14, 20, 4);   // "�", "�", "�", "�", "�", "�"

			lcd.setCursor(0, 0);
			lcd.print(F("\1e\2\3: \4K"));

			lcd.setCursor(0, 1);
			lcd.print(F("<    B\5XO\6     >"));
			break;

		case 311: //  ���� "��������� ����������"
			SetChars(16, 4, 6, 26);   // "�", "�", "�", "�"

			lcd.setCursor(0, 0);
			lcd.print(F("\1CT. COE\2\3HEH\3\4"));

			lcd.setCursor(0, 1);
			lcd.print(F("<    OTMEHA    >"));
			break;

		case 312: //  ���� "���������� �����������"
			SetChars(4, 6, 16, 12);   // "�", "�", "�", "�"

			lcd.setCursor(0, 0);
			lcd.print(F("   COE\1\2HEH\2E   "));

			lcd.setCursor(0, 1);
			lcd.print(F("  \3CTAHOB\4EHO   "));
			break;

		case 321: //  ���� "�������� ������ �� ��"
			SetChars(14, 4, 18, 20);   // "�", "�", "�", "�"

			lcd.setCursor(0, 0);
			lcd.print(F("\1EPE\2A\3A \2AHH\4X"));

			lcd.setCursor(0, 1);
			lcd.print(F("<    OTMEHA    >"));
			break;

		case 331: //  ���� "��������� ������ � ��"
			SetChars(14, 12, 16, 18, 6, 4, 20);   // "�", "�", "�", "�", "�", "�", "�"

			lcd.setCursor(0, 0);
			lcd.print(F("\1O\2\3\4EH\5E \6AHH\7X"));

			lcd.setCursor(0, 1);
			lcd.print(F("<    OTMEHA    >"));
			break;

		}
	}
}

//  �������, ������� �������� ������ ���� � ����������� �� ����������
void setRelay(void) {
	static uint8_t setChanel[4] = { 0, 0, 0, 0 }; //  ����������: ���������� (1) ��� ����������� (0) ����
	if (valMode == 0) {
		uint8_t  getChanel[4] = { 0,0,0,0 };  //  ������: ���������� (1) ��� ����������� (0) ����
		uint32_t timeRTC = 0;     //  ������� ����� � �������� �� 00:00:00
		uint32_t timeTimerStart = 0;  //  ��������� ����� � �������� �� 00:00:00 (��� �����)
		uint32_t timeTimerStop = 0;   //  �������� ����� � �������� �� 00:00:00 (��� �����)
		uint8_t  timeWeekday = 0;   //  ������� ���� ������ � �������: 1 - ��, 2 - ��, 3 - ��, 4 - ��, 5 - ��, 6 - ��, 7 - ��
		valArray[0] = valArray[1] = valArray[2] = valArray[3] = 0;
		timeRTC = (uint32_t)time.Hours * 3600 + time.minutes * 60 + time.seconds;
		timeWeekday = time.weekday;

		if (timeWeekday == 0) { timeWeekday = 7; }
		for (uint8_t i = 0; i < maxTimers; i++) {
			//  ���� ������� ����������, �� ...
			if (ReadTimer(i)) {
				//  �������� �� ���������� ��� ������
				if (bitRead(ReadTimer(i, 6), 7 - timeWeekday)) {

					timeTimerStart = (uint32_t)ReadTimer(i, 1) * 3600 + ReadTimer(i, 2) * 60;
					timeTimerStop = (uint32_t)ReadTimer(i, 3) * 3600 + ReadTimer(i, 4) * 60;

					//  �������� �� ��������� � �������� �������� �������
					if (timeTimerStart <= timeRTC && timeRTC < timeTimerStop) {
						getChanel[ReadTimer(i, 5) - 1] = 1;
						valArray[ReadTimer(i, 5) - 1] = 1;
					}
				}
			}
		}

		// ��������� / ���������� 1 ����
		if (setChanel[0] != getChanel[0]) {
			setChanel[0] = getChanel[0];
			if (getChanel[0])
				digitalWrite(relay1, LOW);
			else
				digitalWrite(relay1, HIGH);
		}

		// ��������� / ���������� 2 ����
		if (setChanel[1] != getChanel[1]) {
			setChanel[1] = getChanel[1];
			if (getChanel[1])
				digitalWrite(relay2, LOW);
			else
				digitalWrite(relay2, HIGH);
		}

		// ��������� / ���������� 3 ����
		if (setChanel[2] != getChanel[2]) {
			setChanel[2] = getChanel[2];
			if (getChanel[2])
				digitalWrite(relay3, LOW);
			else
				digitalWrite(relay3, HIGH);
		}

		// ��������� / ���������� 4 ����
		if (setChanel[3] != getChanel[3]) {
			setChanel[3] = getChanel[3];
			if (getChanel[3])
				digitalWrite(relay4, LOW);
			else
				digitalWrite(relay4, HIGH);
		}
	}
}

//   ������ �� 7 �������� � CGRAM �������   //
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

//   ������ ������ �� ���������� �������   //
uint8_t ReadTimer(uint8_t i, uint8_t j) {
	return EEPROM[i * 7 + j];
}

//   ����� ������ �������   //
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

//   ���������� ������ �� ���������� �������   //
void SaveTimer(uint8_t i, uint8_t j, uint8_t k) {
	EEPROM[i * 7 + j] = k;
}

//   �������� ������ �������� � EEPROM   //
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

bool proveConnection() {
	if (connectPC) {
		if (Serial.available() > 0) {
			if (Serial.read() == 'e')
			{
				connectPC = false;
				return false;
			}
			else
				return true;
		}
		else
			return true;
	}
	else
		return true;
}