//
//  Tastenblinky.c
//  Tastenblinky
//
//  Created by Sysadmin on 03.10.07.
//  Copyright Ruedi Heimlihcer 2007. All rights reserved.
//



#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
//#include <avr/sleep.h>
#include <inttypes.h>
//#define F_CPU 4000000UL  // 4 MHz
//#include <avr/delay.h>
#include <avr/wdt.h>

#include "def.h"
#include "lcd.c"
#include "adc.c"
#include "kbd.h"
//#include "fifo.c"
//#include <util/setbaud.h>

#include "SPI_slave.c"
#include "vga.c"
#include "vga.h"

//#include "onewire.c"
//#include "ds18x20.c"
//#include "crc8.c"
#include "zahlfunktionen.c"

volatile uint16_t loopcount1=0;

volatile uint16_t                manuellcounter=0; // counter fuer Timeout
volatile uint8_t                 programmstatus=0x00;
volatile uint16_t                prellcounter=0; // counter fuer Prellen, in timer0 OVF incr
volatile uint8_t                 incounter=0; // counter fuer ankommende Daten. Steuert renew

volatile uint8_t callbackstatus=0;
volatile uint8_t lastcallbackstatus=0;
volatile uint8_t callbackcounter=0;

void delay_ms(unsigned int ms)
/* delay for a minimum of <ms> */
{
   // we use a calibrated macro. This is more
   // accurate and not so much compiler dependent
   // as self made code.
   while(ms)
   {
      _delay_ms(0.96);
      ms--;
   }
}

void setHomeCentral(void)
{
   // window,left,top,width,height,border,title
   vga_command("r");
   //delay_ms(1000);
   uint8_t posy=0;
   //vga_command(header);       // Create header
   setFeld(1,0,0,100,3,1,"");
   vga_command("f,1");
   vga_puts("Home Central Rueti");
   
   char buffer[12] = {};
   
   posy+= 3;
   setFeld(2,0,posy,48,38,1,""); // Heizung
   vga_command("f,2");
   strcpy_P(buffer,(PGM_P)pgm_read_word(&(raum_table[0])));
   vga_puts(buffer);
   //newline();
   
 
   setFeld(3,48,3,51,38,1,"Data"); // Daten
   vga_command("f,3");
   delay_ms(2);
   
   
   setFeld(4,0,44,51,3,1,"Data"); // Daten
    vga_command("f,4");

}
void setHomeCentral0(void)
{
   
   //char header[]  = {"w,1,0,0,100,3,1,"};                 // no title
   
   //char footer[]  = {"w,5,0,49,100,1,0,"};
   // char window2[] = {"w,2,0,3,60,40,1,Data Webserver"};
   vga_command("r");
   //delay_ms(1000);
   uint8_t posy=0;
   //vga_command(header);       // Create header
   setFeld(1,0,0,100,3,1,"");
   vga_command("f,1");
   vga_puts("Home Central Rueti");
   
   char buffer[12] = {};
   
   posy+= 3;
   setFeld(2,0,posy,46,9,1,""); // Heizung
   vga_command("f,2");
   strcpy_P(buffer,(PGM_P)pgm_read_word(&(raum_table[0])));
   vga_puts(buffer);
   //newline();
   
   
   posy+= 9;
   setFeld(3,0,posy,46,7,1,""); // Werkstatt
   vga_command("f,3");
   strcpy_P(buffer,(PGM_P)pgm_read_word(&(raum_table[1])));
   vga_puts(buffer);
   
   posy+= 7;
   setFeld(4,0,posy,46,6,1,""); // WoZi
   vga_command("f,4");
   //vga_puts("WoZi");
   strcpy_P(buffer,(PGM_P)pgm_read_word(&(raum_table[2])));
   vga_puts(buffer);
   
   
   posy+= 6;
   setFeld(5,0,posy,46,4,1,""); // Buero
   vga_command("f,5");
   strcpy_P(buffer,(PGM_P)pgm_read_word(&(raum_table[3])));
   vga_puts(buffer);
   
   //vga_puts("Buero");
   
   posy+= 4;
   setFeld(6,0,posy,46,4,1,""); // Labor
   vga_command("f,6");
   strcpy_P(buffer,(PGM_P)pgm_read_word(&(raum_table[4])));
   vga_puts(buffer);
   //vga_puts("Labor");
   
   posy+= 4;
   setFeld(7,0,posy,46,4,1,""); // OG
   vga_command("f,7");
   strcpy_P(buffer,(PGM_P)pgm_read_word(&(raum_table[5])));
   vga_puts(buffer);
   //vga_puts("OG");
   
   posy+= 12;
   posy = 45;
   setFeld(8,0,posy,100,4,1,""); // Estrich
   vga_command("f,8");
   strcpy_P(buffer,(PGM_P)pgm_read_word(&(raum_table[6])));
   vga_puts(buffer);
   vga_leerschlag(2);
   vga_putint(posy);
   //vga_puts("Estrich");
   
   
   
   
   setFeld(9,47,3,52,26,1,"Data"); // Daten
   // vga_command("f,9");
   // vga_puts("Data");
   
   
   
}

void setHomeCentralblank(void)
{
   
   vga_command("r");
   //delay_ms(1000);
   uint8_t posy=0;
   uint8_t tab= 48;
   uint8_t maxy= 99;
   //vga_command(header);       // Create header
   setFeld(1,0,0,100,3,1,"");
   vga_command("f,1");
   vga_puts("Home Central Rueti");
   
   char buffer[12] = {};

   posy= 3;
   setFeld(2,0,posy,tab,38,1,""); // Raumdaten
   vga_command("f,2");
   //strcpy_P(buffer,(PGM_P)pgm_read_word(&(raum_table[0])));
   //vga_puts(buffer);
   //newline();
   
   
   posy = 41;
   setFeld(3,0,posy,tab,9,1,""); // Status
   vga_command("f,3");
   //strcpy_P(buffer,(PGM_P)pgm_read_word(&(raum_table[1])));
   //vga_puts(buffer);
   
   /*
   posy= 19;
   setFeld(4,0,posy,46,6,1,""); // WoZi
   vga_command("f,4");
   //vga_puts("WoZi");
   strcpy_P(buffer,(PGM_P)pgm_read_word(&(raum_table[2])));
   vga_puts(buffer);
   */
   /*
   posy= 25;
   setFeld(5,0,posy,46,4,1,""); // Buero
   vga_command("f,5");
   strcpy_P(buffer,(PGM_P)pgm_read_word(&(raum_table[3])));
   vga_puts(buffer);
   */
   //vga_puts("Buero");
   /*
   posy= 29;
   setFeld(6,0,posy,46,4,1,""); // Labor
   vga_command("f,6");
   strcpy_P(buffer,(PGM_P)pgm_read_word(&(raum_table[4])));
   vga_puts(buffer);
   //vga_puts("Labor");
   */
   /*
   posy = 33;
   setFeld(7,0,posy,46,4,1,""); // OG
   vga_command("f,7");
   strcpy_P(buffer,(PGM_P)pgm_read_word(&(raum_table[5])));
   vga_puts(buffer);
   //vga_puts("OG");
   */
   posy = 41;
   setFeld(4,tab,posy,(maxy-tab),9,1,""); // Error
   vga_command("f,4");
   //strcpy_P(buffer,(PGM_P)pgm_read_word(&(raum_table[6])));
   //vga_puts(buffer);
   //vga_leerschlag(2);
   //vga_putint(posy);
   //vga_puts("Estrich");
   
   // window,left,top,width,height,border,title

   
   posy=3;
   setFeld(5,tab,3,(maxy-tab),38,1,"Data"); // Daten
   vga_command("f,5");
  // vga_puts("Data");
   
   
   
}

void setHeizung(uint8_t renew)
{
   /*
    #define TAB0   0
    #define TAB1   17
    #define TAB2   32
    
    #define TAB100   0
    #define TAB101   11
    #define TAB110   16
    #define TAB111   22
    #define TAB120   32
    #define TAB121   38
    
    
    #define TAB200   0
    #define TAB201   6
    #define TAB210   16
    #define TAB211   22
    #define TAB220   32
    #define TAB221   38
    
    */
   uint8_t pos_y = HEIZUNG_Y;
   char buffer[12]={};
   
   if (renew == HEIZUNG)
   {
      vga_command("f,2");
      vga_gotoxy(TAB100,pos_y);
      vga_command("f,2");
      strcpy_P(buffer,(PGM_P)pgm_read_word(&(raum_table[0])));
      vga_puts(buffer);
   }
   // Zeile 1
   pos_y++;
   if (renew == HEIZUNG)
   {
      vga_gotoxy(TAB100,pos_y);
      vga_command("f,2");
      // Vorlauf
      strcpy_P(buffer,(PGM_P)pgm_read_word(&(heizung_table[0])));
      vga_puts(buffer);
   }
   vga_gotoxy(TAB101,pos_y);
   vga_command("f,2");
   vga_putint_right(outbuffer[2]/2);
   
   // Mode,  Heizungstundencode, bit 4,5
   if (renew == HEIZUNG)
   {
      vga_gotoxy(TAB110,pos_y);
      vga_command("f,2");
      strcpy_P(buffer,(PGM_P)pgm_read_word(&(heizung_table[3])));
      vga_puts(buffer);
   }
   vga_gotoxy(TAB111,pos_y);
   vga_command("f,2");
   uint8_t Heizungstundencode = outbuffer[5] & 0x30; // bit 4,5
   Heizungstundencode >>= 4;
   vga_puthex(Heizungstundencode);
   
   //vga_puthex(outbuffer[27]);
   
   //vga_leerschlag(3);
   
   // Brenner
   if (renew == HEIZUNG)
   {
      vga_gotoxy(TAB120,pos_y);
      vga_command("f,2");
      strcpy_P(buffer,(PGM_P)pgm_read_word(&(heizung_table[4])));
      vga_puts(buffer);
   }
   vga_gotoxy(TAB121,pos_y);
   vga_command("f,2");
   
   if (outbuffer[5] & 0x04)
   {
      vga_puts("OFF");
   }
   else
   {
      vga_puts(" ON");
   }
   
   
   //2. Zeile
   pos_y++;
   
   // Ruecklauf
   if (renew == HEIZUNG)
   {
      vga_gotoxy(TAB100,pos_y);
      vga_command("f,2");
      strcpy_P(buffer,(PGM_P)pgm_read_word(&(heizung_table[1])));
      vga_puts(buffer);
   }
   vga_gotoxy(TAB101,pos_y);
   vga_command("f,2");
   vga_putint_right(outbuffer[3]/2);
   //vga_leerschlag(3);
   
   // Schalterposition
   if (renew == HEIZUNG)
   {
      vga_gotoxy(TAB110,pos_y);
      vga_command("f,2");
      strcpy_P(buffer,(PGM_P)pgm_read_word(&(heizung_table[5])));
      vga_puts(buffer);
   }
   uint8_t mode = outbuffer[5] & 0x30;
   mode >>=4;

   vga_gotoxy(TAB111,pos_y);
   vga_command("f,2");
   vga_putint_right(mode);
   //vga_leerschlag(3);
   
   // Beschreibung
   /*
    pos-table
    const char pos_0[] PROGMEM = "OFF";
    const char pos_1[] PROGMEM = "F/-";
    const char pos_2[] PROGMEM = "F/R";
    const char pos_3[] PROGMEM = "R/R";
    const char pos_4[] PROGMEM = "V/V";
    const char pos_5[] PROGMEM = "Hnd";

    */
   /*
   if (renew == HEIZUNG)
   {
      vga_gotoxy(TAB120,pos_y);
      vga_command("f,2");
      //strcpy_P(buffer,(PGM_P)pgm_read_word(&(pos_table[outbuffer[0]%4])));
      strcpy_P(buffer,(char*)PSTR("Mode: "));
      vga_puts(buffer);

   }
    */
   vga_gotoxy(TAB120,pos_y);
   vga_command("f,2");
   switch (mode)
   {
      case 0:
      {
         strcpy_P(buffer,(char*)PSTR("Red/Red"));
      }break;
      case 1:
      {
         strcpy_P(buffer,(char*)PSTR("Full/OFF"));
      }break;
      case 2:
      {
         strcpy_P(buffer,(char*)PSTR("Full/Red"));
      }break;
      case 3:
      {
         strcpy_P(buffer,(char*)PSTR("Full/Full"));
      }break;
   }
   vga_puts(buffer);
   //vga_puthex(mode);
   
   // dritte Zeile
   pos_y++;
   if (renew == HEIZUNG)
   {
      vga_gotoxy(TAB100,pos_y);
      vga_command("f,2");
      strcpy_P(buffer,(PGM_P)pgm_read_word(&(heizung_table[2])));
      vga_puts(buffer);
   }
   vga_gotoxy(TAB101,pos_y);
   vga_command("f,2");
   char tempbuffer[12] = {};
   //vga_tempbis99(outbuffer[4]/2-0x20,tempbuffer);
   vga_tempbis99(outbuffer[4]/2,tempbuffer);
   
   vga_puts(tempbuffer);
   
   // Rinne
   if (renew == HEIZUNG)
   {
      vga_gotoxy(TAB110,pos_y);
      vga_command("f,2");
      strcpy_P(buffer,(PGM_P)pgm_read_word(&(heizung_table[6])));
      vga_puts(buffer);
   }
   vga_gotoxy(TAB111,pos_y);
   vga_command("f,2");
   
   if (outbuffer[5] & 0xC0)
   {
      vga_puts(" ON");
   }
   else
   {
      vga_puts("OFF");
   }
   
}


void setWerkstatt(uint8_t renew)
{
   /*
    #define TAB200   0
    #define TAB201   6
    #define TAB210   16
    #define TAB211   22
    #define TAB220   32
    #define TAB221   38
    
    */
   
   char buffer[12]={};
   uint8_t pos_y = WERKSTATT_Y;
   // Werkstatt
   if (renew == WERKSTATT)
   {
      vga_command("f,2");
      vga_gotoxy(TAB200,pos_y);
      vga_command("f,2");
      strcpy_P(buffer,(PGM_P)pgm_read_word(&(raum_table[1])));
      vga_puts(buffer);
   }
   // Zeile 1
   // Lampe
   pos_y++;
   if (renew == WERKSTATT)
   {
      vga_gotoxy(TAB200,pos_y);
      vga_command("f,2");
      strcpy_P(buffer,(PGM_P)pgm_read_word(&(WS_table[0])));
      vga_puts(buffer);
   }
   vga_gotoxy(TAB201,pos_y);
   vga_command("f,2");
   
   if (outbuffer[18] & 0x01)
   {
      vga_puts(" ON");
   }
   else
   {
      vga_puts("OFF");
   }
   
   // Ofen
   if (renew == WERKSTATT)
   {
      vga_gotoxy(TAB210,pos_y);
      vga_command("f,2");
      strcpy_P(buffer,(PGM_P)pgm_read_word(&(WS_table[1])));
      vga_puts(buffer);
   }
   vga_gotoxy(TAB211,pos_y);
   vga_command("f,2");
   
   if (outbuffer[5] & 0x04)
   {
      vga_puts("OFF");
   }
   else
   {
      vga_puts(" ON");
   }
   if (renew == WERKSTATT)
   {
      vga_gotoxy(TAB220,pos_y);
      vga_command("f,2");
      strcpy_P(buffer,(PGM_P)pgm_read_word(&(WS_table[2])));
      vga_puts(buffer);
   }
   vga_gotoxy(TAB221,pos_y);
   vga_command("f,2");
   char tempbuffer[8]={};
   vga_tempbis99(slavestatus[WERKSTATT][1]/2,tempbuffer);
   vga_puts(tempbuffer);
   
   
   //Zeile 2
   // Wasser
   pos_y++;
   if (renew == WERKSTATT)
   {
      vga_gotoxy(TAB210,pos_y);
      vga_command("f,2");
      strcpy_P(buffer,(PGM_P)pgm_read_word(&(WS_table[4])));
      vga_puts(buffer);
   }
   vga_gotoxy(TAB211,pos_y);
   vga_command("f,2");
   
   if (outbuffer[5] & 0x04)
   {
      vga_puts(" OK  ");
   }
   else
   {
      vga_puts("Alarm");
   }
   
   
}

void setWoZi(uint8_t renew)
{
   char buffer[12]={};
   uint8_t pos_y = RAUM_Y;
   // WoZi
   if (renew == WOZI)
   {
   vga_command("f,2");
   
   vga_gotoxy(TAB200,pos_y);
   vga_command("f,2");
   strcpy_P(buffer,(PGM_P)pgm_read_word(&(raum_table[2])));
   vga_puts(buffer);
   }
   // Zeile 1
   // Lampe
   pos_y++;
   if (renew == WOZI)
   {
   vga_gotoxy(TAB200,pos_y);
   vga_command("f,2");
   strcpy_P(buffer,(PGM_P)pgm_read_word(&(RAUM_table[0])));
   vga_puts(buffer);
   }
   vga_gotoxy(TAB201,pos_y);
   vga_command("f,2");
   
   if (outbuffer[20] & 0x01)
   {
      vga_puts(" ON");
   }
   else
   {
      vga_puts("OFF");
   }
   
   // Innen
   if (renew == WOZI)
   {
   vga_gotoxy(TAB220,pos_y);
   vga_command("f,2");
   strcpy_P(buffer,(PGM_P)pgm_read_word(&(RAUM_table[1])));
   vga_puts(buffer);
   }
   vga_gotoxy(TAB221,pos_y);
   vga_command("f,2");
   char tempbuffer[8]={};
   //vga_tempbis99(slavestatus[WOZI][3]/2,tempbuffer);
   vga_tempbis99(outbuffer[21],tempbuffer);
   
   vga_puts(tempbuffer);
   
   
   //Zeile 2
   // Radiator
   pos_y++;
   if (renew == WOZI)
   {
   vga_gotoxy(TAB200,pos_y);
   vga_command("f,2");
   strcpy_P(buffer,(PGM_P)pgm_read_word(&(RAUM_table[2])));
   vga_puts(buffer);
   }
   vga_gotoxy(TAB201,pos_y);
   vga_command("f,2");
   if (outbuffer[20] & 0x02)
   {
      vga_puts(" ON");
   }
   else
   {
      vga_puts("OFF");
   }
   
   
}


void setBuero(uint8_t renew)
{
   char buffer[12]={};
   uint8_t pos_y = BUERO_Y;
   if (renew == BUERO)
   {
      // Buero
      vga_command("f,2");
      vga_gotoxy(TAB200,pos_y);
      vga_command("f,2");
      strcpy_P(buffer,(PGM_P)pgm_read_word(&(raum_table[3])));
      vga_puts(buffer);
   }
   // Zeile 1
   // Lampe
   pos_y++;
   if (renew == BUERO)
   {
      vga_gotoxy(TAB200,pos_y);
      vga_command("f,2");
      strcpy_P(buffer,(PGM_P)pgm_read_word(&(RAUM_table[0])));
      vga_puts(buffer);
   }
   vga_gotoxy(TAB201,pos_y);
   vga_command("f,2");
   
   if (outbuffer[22] & 0x01)
   {
      vga_puts(" ON");
   }
   else
   {
      vga_puts("OFF");
   }
   
   // Innen
   if (renew == BUERO)
   {
      vga_gotoxy(TAB220,pos_y);
      vga_command("f,2");
      strcpy_P(buffer,(PGM_P)pgm_read_word(&(RAUM_table[1])));
      vga_puts(buffer);
   }
   vga_gotoxy(TAB221,pos_y);
   vga_command("f,2");
   char tempbuffer[8]={};
   //vga_tempbis99(slavestatus[BUERO][1]/2,tempbuffer);
   vga_tempbis99(outbuffer[23],tempbuffer);
   vga_puts(tempbuffer);
   
   
   //Zeile 2
   // Radiator
   pos_y++;
   if (renew == BUERO)
   {
      vga_gotoxy(TAB200,pos_y);
      vga_command("f,2");
      strcpy_P(buffer,(PGM_P)pgm_read_word(&(RAUM_table[2])));
      vga_puts(buffer);
   }
   vga_gotoxy(TAB201,pos_y);
   vga_command("f,2");
   if (outbuffer[22] & 0x02)
   {
      vga_puts("OFF");
   }
   else
   {
      vga_puts(" ON");
   }
}



void setLabor(uint8_t renew)
{
   char buffer[12]={};
   uint8_t pos_y = LABOR_Y;
   // Labor
   if (renew == LABOR)
   {
      vga_command("f,2");
      vga_gotoxy(TAB200,pos_y);
      vga_command("f,2");
      strcpy_P(buffer,(PGM_P)pgm_read_word(&(raum_table[4])));
      vga_puts(buffer);
   }
   // Zeile 1
   // Lampe
   pos_y++;
   if (renew == LABOR)
   {
      vga_gotoxy(TAB200,pos_y);
      vga_command("f,2");
      strcpy_P(buffer,(PGM_P)pgm_read_word(&(RAUM_table[0])));
      vga_puts(buffer);
   }
   vga_gotoxy(TAB201,pos_y);
   vga_command("f,2");
   
   if (outbuffer[24] & 0x01)
   {
      vga_puts(" ON");
   }
   else
   {
      vga_puts("OFF");
   }
   
   // Innen
   if (renew == LABOR)
   {
      vga_gotoxy(TAB220,pos_y);
      vga_command("f,2");
      strcpy_P(buffer,(PGM_P)pgm_read_word(&(RAUM_table[1])));
      vga_puts(buffer);
   }
   vga_gotoxy(TAB221,pos_y);
   vga_command("f,2");
   char tempbuffer[8]={};
   vga_tempbis99(outbuffer[25],tempbuffer);
   vga_puts(tempbuffer);
   
   
   //Zeile 2
   // Radiator
   pos_y++;
   if (renew == LABOR)
   {
      vga_gotoxy(TAB200,pos_y);
      vga_command("f,2");
      strcpy_P(buffer,(PGM_P)pgm_read_word(&(RAUM_table[2])));
      vga_puts(buffer);
   }
   vga_gotoxy(TAB201,pos_y);
   vga_command("f,2");
   if (outbuffer[24] & 0x02)
   {
      vga_puts(" ON");
   }
   else
   {
      vga_puts("OFF");
   }
   
}

void setOG1(uint8_t renew)
{
   char buffer[12]={};
   uint8_t pos_y = OG1_Y;
   // OG 1
   if (renew == OG1)
   {
      vga_command("f,2");
      vga_gotoxy(TAB200,pos_y);
      vga_command("f,2");
      strcpy_P(buffer,(PGM_P)pgm_read_word(&(raum_table[5])));
      vga_puts(buffer);
   }
   // Zeile 1
   // Lampe
   pos_y++;
   if (renew == OG1)
   {
      vga_gotoxy(TAB200,pos_y);
      vga_command("f,2");
      strcpy_P(buffer,(PGM_P)pgm_read_word(&(RAUM_table[0])));
      vga_puts(buffer);
   }
   vga_gotoxy(TAB201,pos_y);
   vga_command("f,2");
   
   if (outbuffer[26] & 0x01)
   {
      vga_puts(" ON");
   }
   else
   {
      vga_puts("OFF");
   }
   
   // Innen
   if (renew == OG1)
   {
      vga_gotoxy(TAB220,pos_y);
      vga_command("f,2");
      strcpy_P(buffer,(PGM_P)pgm_read_word(&(RAUM_table[1])));
      vga_puts(buffer);
   }
   vga_gotoxy(TAB221,pos_y);
   vga_command("f,2");
   char tempbuffer[8]={};
   vga_tempbis99(slavestatus[OG1][3]/2,tempbuffer);
   vga_puts(tempbuffer);
   
   
   //Zeile 2
   // Radiator
   pos_y++;
   if (renew == OG1)
   {
      vga_gotoxy(TAB200,pos_y);
      vga_command("f,2");
      strcpy_P(buffer,(PGM_P)pgm_read_word(&(RAUM_table[2])));
      vga_puts(buffer);
   }
   vga_gotoxy(TAB201,pos_y);
   vga_command("f,2");
   if (outbuffer[26] & 0x02)
   {
      vga_puts(" ON");
   }
   else
   {
      vga_puts("OFF");
   }
   
}

void setOG2(uint8_t renew)
{
   char buffer[12]={};
   uint8_t pos_y = OG2_Y;
   // OG 2
   if (renew == OG2)
   {
      vga_command("f,2");
      vga_gotoxy(TAB200,pos_y);
      vga_command("f,2");
      strcpy_P(buffer,(PGM_P)pgm_read_word(&(raum_table[6])));
      vga_puts(buffer);
   }
   // Zeile 1
   // Lampe
   pos_y++;
   if (renew == OG2)
   {
      vga_gotoxy(TAB200,pos_y);
      vga_command("f,2");
      strcpy_P(buffer,(PGM_P)pgm_read_word(&(RAUM_table[0])));
      vga_puts(buffer);
   }
   vga_gotoxy(TAB201,pos_y);
   vga_command("f,2");
   
   if (outbuffer[28] & 0x01)
   {
      vga_puts(" ON");
   }
   else
   {
      vga_puts("OFF");
   }
   
   // Innen
   
   if (renew == OG2)
   {
      vga_gotoxy(TAB220,pos_y);
      vga_command("f,2");
      strcpy_P(buffer,(PGM_P)pgm_read_word(&(RAUM_table[1])));
      vga_puts(buffer);
   }
   vga_gotoxy(TAB221,pos_y);
   vga_command("f,2");
   char tempbuffer[8]={};
   vga_tempbis99(slavestatus[OG2][3]/2,tempbuffer);
   vga_puts(tempbuffer);
   
   
   //Zeile 2
   // Radiator
   pos_y++;
   if (renew == OG2)
   {
      vga_gotoxy(TAB200,pos_y);
      vga_command("f,2");
      strcpy_P(buffer,(PGM_P)pgm_read_word(&(RAUM_table[2])));
      vga_puts(buffer);
   }
   vga_gotoxy(TAB201,pos_y);
   vga_command("f,2");
   if (outbuffer[28] & 0x02)
   {
      vga_puts(" ON");
   }
   else
   {
      vga_puts("OFF");
   }
   
}

void setEstrich(uint8_t renew)
{
   char buffer[12]={};
   uint8_t pos_y = ESTRICH_Y;
   // Estrich
   if (renew == ESTRICH)
   {
      vga_command("f,2");
      
      vga_gotoxy(TAB100,pos_y);
      vga_command("f,2");
      strcpy_P(buffer,(PGM_P)pgm_read_word(&(raum_table[7])));
      vga_puts(buffer);
   }
   //Zeile 1
   pos_y++;
   // Vorlauf
   if (renew == ESTRICH)
   {
      vga_gotoxy(TAB100,pos_y);
      vga_command("f,2");
      strcpy_P(buffer,(PGM_P)pgm_read_word(&(ES_table[0])));
      vga_puts(buffer);
   }
   vga_gotoxy(TAB101,pos_y);
   vga_command("f,2");
   vga_putint_right(outbuffer[9]/2);
   
   // Boiler O
   if (renew == ESTRICH)
   {
      vga_gotoxy(TAB110,pos_y);
      vga_command("f,2");
      strcpy_P(buffer,(PGM_P)pgm_read_word(&(ES_table[4])));
      vga_puts(buffer);
   }
   vga_gotoxy(TAB111,pos_y);
   vga_command("f,2");
   vga_putint_right(outbuffer[13]/2);
   
   // Pumpe
   if (renew == ESTRICH)
   {
      vga_gotoxy(TAB120,pos_y);
      vga_command("f,2");
      strcpy_P(buffer,(PGM_P)pgm_read_word(&(ES_table[6])));
      
      vga_puts(buffer);
   }
   vga_gotoxy(TAB121,pos_y);
   vga_command("f,2");
   if (outbuffer[15] & 0x04)
   {
      vga_puts(" ON");
   }
   else
   {
      vga_puts("OFF");
   }
   
   // Zeile 2
   pos_y++;
   // Ruecklauf
   if (renew == ESTRICH)
   {
      vga_gotoxy(TAB100,pos_y);
      vga_command("f,2");
      strcpy_P(buffer,(PGM_P)pgm_read_word(&(ES_table[1])));
      vga_puts(buffer);
   }
   vga_gotoxy(TAB101,pos_y);
   vga_command("f,2");
   vga_putint_right(outbuffer[10]/2);
   
   // Boiler M
   if (renew == ESTRICH)
   {
      vga_gotoxy(TAB110,pos_y);
      vga_command("f,2");
      strcpy_P(buffer,(PGM_P)pgm_read_word(&(ES_table[3])));
      vga_puts(buffer);
   }
   vga_gotoxy(TAB111,pos_y);
   vga_command("f,2");
   vga_putint_right(outbuffer[12]/2);
   
   // Elektro
   if (renew == ESTRICH)
   {
      vga_gotoxy(TAB120,pos_y);
      vga_command("f,2");
      strcpy_P(buffer,(PGM_P)pgm_read_word(&(ES_table[7])));
      vga_puts(buffer);
   }
   vga_gotoxy(TAB121,pos_y);
   vga_command("f,2");
   if (outbuffer[15] & 0x04)
   {
      vga_puts(" ON");
   }
   else
   {
      vga_puts("OFF");
   }
   
   // Kollektor
   
   
   // Boiler U
   
   
   //Alarm
   // Zeile 3
   pos_y++;
   // Kollektor
   if (renew == ESTRICH)
   {
      vga_gotoxy(TAB100,pos_y);
      vga_command("f,2");
      strcpy_P(buffer,(PGM_P)pgm_read_word(&(ES_table[5])));
      vga_puts(buffer);
   }
   vga_gotoxy(TAB101,pos_y);
   vga_command("f,2");
   vga_putint_right(outbuffer[14]/2);
   
   // Boiler U
   if (renew == ESTRICH)
   {
      vga_gotoxy(TAB110,pos_y);
      vga_command("f,2");
      strcpy_P(buffer,(PGM_P)pgm_read_word(&(ES_table[2])));
      vga_puts(buffer);
   }
   vga_gotoxy(TAB111,pos_y);
   vga_command("f,2");
   vga_putint_right(outbuffer[11]/2);
   
   // Alarm
   if (renew == ESTRICH)
   {
      vga_gotoxy(TAB120,pos_y);
      vga_command("f,2");
      strcpy_P(buffer,(PGM_P)pgm_read_word(&(ES_table[8])));
      vga_puts(buffer);
   }
   vga_gotoxy(TAB121,pos_y);
   vga_command("f,2");
   if (outbuffer[15] & 0x04)
   {
      vga_puts(" ON");
   }
   else
   {
      vga_puts("OFF");
   }
   
   
}


void setStatusData(void)
{
   /*
    const char St_0[] PROGMEM = "EE : ";
    const char St_1[] PROGMEM = "Read :";
    const char St_2[] PROGMEM = "Write:";
    const char St_3[] PROGMEM = "Err  :";
    const char St_4[] PROGMEM = "Raum:";
    const char St_5[] PROGMEM = "R: ";
    const char St_6[] PROGMEM = "W: ";
    */
   char buffer[12]={};
   char data_string[4];
   uint8_t renew = incounter%8;
   uint8_t pos_y = 0;
   if (renew == 1)
   {
      vga_command("f,3");
      vga_gotoxy(TAB0,pos_y);
      vga_command("f,3");
      strcpy_P(buffer,(char*)PSTR("Status: "));
      vga_puts(buffer);
   }
   // TASK
   
   if (renew == 1)
   {
      vga_command("f,3");
      vga_gotoxy(TAB12,pos_y);
      vga_command("f,3");
      strcpy_P(buffer,(char*)PSTR("TASK: "));
      vga_puts(buffer);
   }
   vga_command("f,3");
   vga_gotoxy(TAB12+6,pos_y);
   vga_command("f,3");
   
   int2hexstring(outbuffer[42], (char*)&data_string);
   vga_puts(data_string);
   
   
   pos_y++;
   // EE
   if (renew == 1)
   {
      vga_command("f,3");
      vga_gotoxy(TAB10,pos_y);
      vga_command("f,3");
      strcpy_P(buffer,(PGM_P)pgm_read_word(&(status_table[0])));
      vga_puts(buffer);
      
      // Read Fehler
      vga_command("f,3");
      vga_gotoxy(TAB11,pos_y);
      vga_command("f,3");
      strcpy_P(buffer,(PGM_P)pgm_read_word(&(status_table[1])));
      vga_puts(buffer);
   }
   vga_command("f,3");
   vga_gotoxy(TAB11+6,pos_y);
   vga_command("f,3");
   int2hexstring(outbuffer[24], (char*)&data_string);
   vga_puts(data_string);
   
   // Write fehler
   if (renew == 1)
   {
      vga_command("f,3");
      vga_gotoxy(TAB12,pos_y);
      vga_command("f,3");
      strcpy_P(buffer,(PGM_P)pgm_read_word(&(status_table[2])));
      vga_puts(buffer);
   }
   vga_command("f,3");
   vga_gotoxy(TAB12+6,pos_y);
   vga_command("f,3");
   int2hexstring(outbuffer[25], (char*)&data_string);
   vga_puts(data_string);
   
   // Err Fehler
   if (renew == 1)
   {
      vga_command("f,3");
      vga_gotoxy(TAB13,pos_y);
      vga_command("f,3");
      strcpy_P(buffer,(PGM_P)pgm_read_word(&(status_table[3])));
      vga_puts(buffer);
   }
   vga_command("f,3");
   vga_gotoxy(TAB13+6,pos_y);
   vga_command("f,3");
   int2hexstring(outbuffer[26], (char*)&data_string);
   vga_puts(data_string);
   
   
   
   // Belegung
   pos_y++;
   // Belegung Read
   if (renew == 1)
   {
      vga_command("f,3");
      vga_gotoxy(TAB10,pos_y);
      vga_command("f,3");
      strcpy_P(buffer,(PGM_P)pgm_read_word(&(status_table[4])));
      vga_puts(buffer);
      vga_command("f,3");
      vga_gotoxy(TAB11,pos_y);
      vga_command("f,3");
      strcpy_P(buffer,(PGM_P)pgm_read_word(&(status_table[1])));
      vga_puts(buffer);
   }
   vga_command("f,3");
   vga_gotoxy(TAB11+6,pos_y);
   vga_command("f,3");
   int2hexstring(outbuffer[43], (char*)&data_string);
   vga_puts(data_string);
   
   // Belegung Write
   if (renew == 1)
   {
      vga_command("f,3");
      vga_gotoxy(TAB12,pos_y);
      vga_command("f,3");
      strcpy_P(buffer,(PGM_P)pgm_read_word(&(status_table[2])));
      vga_puts(buffer);
   }
   vga_command("f,3");
   vga_gotoxy(TAB12+6,pos_y);
   vga_command("f,3");
   vga_puthex(outbuffer[44]);
   pos_y++;
   
   if (renew == 1)
   {
      vga_command("f,3");
      vga_gotoxy(TAB10,pos_y);
      vga_command("f,3");
      strcpy_P(buffer,(char*)PSTR("Raum:"));
      vga_puts(buffer);
   }
   vga_command("f,3");
   vga_gotoxy(TAB10+6,pos_y);
   vga_command("f,3");
   
   vga_putint1(outbuffer[SLAVESTATUSRAUM]);
   vga_command("f,3");
   vga_gotoxy(TAB12,pos_y);
   vga_command("f,3");
   
   uint8_t i=0;
   for (i=0;i<4;i++)
   {
      vga_putch(' ');
      vga_puthex(outbuffer[SLAVESTATUSPOS+i]);
   }
   
   // callbackstatus
   
   pos_y++;
   
   if (renew == 1)
   {
      vga_command("f,3");
      vga_gotoxy(TAB10,pos_y);
      vga_command("f,3");
      strcpy_P(buffer,(char*)PSTR("Callback:"));
      vga_puts(buffer);
    }
   uint8_t newcallbackstatus = inbuffer[39];
   if ((callbackstatus == 0) && newcallbackstatus) // start
   {
      callbackstatus = newcallbackstatus;
   }
  
   if (newcallbackstatus == lastcallbackstatus) // zweimal gleicher wert
   {
      //callbackstatus = newcallbackstatus;
      callbackcounter++;
      if (callbackcounter == 2)
      {
         callbackstatus = newcallbackstatus;
         //lastcallbackstatus = newcallbackstatus
      }
      
      
   }
   else
   {
      callbackcounter = 0; // Count von vorn beginnen
      lastcallbackstatus = newcallbackstatus; // neuen wert behalten
   }
   
   vga_command("f,3");
   vga_gotoxy(TAB11+4,pos_y);
   vga_command("f,3");
   vga_puthex(newcallbackstatus);
 
   // homecallback
   if (renew == 1)
   {
      vga_command("f,3");
      vga_gotoxy(TAB12,pos_y);
      vga_command("f,3");
      strcpy_P(buffer,(char*)PSTR("home :"));
      vga_puts(buffer);
   }
   vga_command("f,3");
   vga_gotoxy(TAB12+5,pos_y);
   vga_command("f,3");
  if (callbackstatus & (1<<HOMECALLBACK))
   {
      vga_puts(" OK");
   }
   else
   {
      vga_puts("ERR");
   }
   
   // solarcallback
   if (renew == 1)
   {
      vga_command("f,3");
      vga_gotoxy(TAB13,pos_y);
      vga_command("f,3");
      strcpy_P(buffer,(char*)PSTR("solar:"));
      vga_puts(buffer);
   }
   vga_command("f,3");
   vga_gotoxy(TAB13+5,pos_y);
   vga_command("f,3");
   if (callbackstatus & (1<<SOLARCALLBACK))
   {
      vga_puts(" OK");
   }
   else
   {
      vga_puts("ERR");
   }
 
   // alarmcallback
   if (renew == 1)
   {
      vga_command("f,3");
      vga_gotoxy(TAB14,pos_y);
      vga_command("f,3");
      strcpy_P(buffer,(char*)PSTR("alarm:"));
      vga_puts(buffer);
   }
   vga_command("f,3");
   vga_gotoxy(TAB14+5,pos_y);
   vga_command("f,3");
   if (callbackstatus & (1<<SOLARCALLBACK))
   {
      vga_puts(" OK");
   }
   else
   {
      vga_puts("ERR");
   }

  
   
   
}
void setErrorData(void)
{
 
   char buffer[12]={};
   char data_string[4];
   vga_command("f,4");
   uint8_t pos_y = 0;
   vga_gotoxy(TAB0,pos_y);
   vga_command("f,4");
   strcpy_P(buffer,(char*)PSTR("Error: "));
   vga_puts(buffer);
   
   // Error-Bits
   vga_gotoxy(TAB12,pos_y);
   vga_command("f,4");
   strcpy_P(buffer,(char*)PSTR("ERR: "));
   vga_puts(buffer);
   int2hexstring(outbuffer[39], (char*)&data_string);
   vga_puts(data_string);

   // Alarm-Bits
   vga_gotoxy(TAB13,pos_y);
   vga_command("f,4");
   strcpy_P(buffer,(char*)PSTR("ALARM: "));
   vga_puts(buffer);
   int2hexstring(outbuffer[40], (char*)&data_string);
   vga_puts(data_string);

   
   pos_y++;
   // TWI:
   vga_gotoxy(TAB10,pos_y);
   vga_command("f,4");
   strcpy_P(buffer,(PGM_P)pgm_read_word(&(error_table[0])));
   vga_puts(buffer);
   
   // Call count
   vga_gotoxy(TAB11,pos_y);
   vga_command("f,4");
   strcpy_P(buffer,(PGM_P)pgm_read_word(&(error_table[1])));
   vga_puts(buffer);
   int2hexstring(outbuffer[26], (char*)&data_string);
   vga_puts(data_string);
   
   // Reply count
   vga_gotoxy(TAB12,pos_y);
   vga_command("f,4");
   strcpy_P(buffer,(PGM_P)pgm_read_word(&(error_table[2])));
   vga_puts(buffer);
   int2hexstring(outbuffer[27], (char*)&data_string);
   vga_puts(data_string);

   pos_y++;
   // SPI:
   vga_gotoxy(TAB10,pos_y);
   vga_command("f,4");
   strcpy_P(buffer,(PGM_P)pgm_read_word(&(error_table[4])));
   vga_puts(buffer);
   
   // out_startdaten
   vga_gotoxy(TAB11,pos_y);
   vga_command("f,4");
   strcpy_P(buffer,(PGM_P)pgm_read_word(&(error_table[5])));
   vga_puts(buffer);
   int2hexstring(outbuffer[31], (char*)&data_string);
   vga_puts(data_string);
   
   // in_enddaten
   vga_gotoxy(TAB12,pos_y);
   vga_command("f,4");
   strcpy_P(buffer,(PGM_P)pgm_read_word(&(error_table[6])));
   vga_puts(buffer);
   int2hexstring(outbuffer[42], (char*)&data_string);
   vga_puts(data_string);

   
   // bytecount
   vga_gotoxy(TAB13,pos_y);
   vga_command("f,4");
   strcpy_P(buffer,(PGM_P)pgm_read_word(&(error_table[7])));
   vga_puts(buffer);
   int2hexstring(outbuffer[33], (char*)&data_string);
   vga_puts(data_string);

   
   
 
}



void setRaumData(uint8_t renew)
{
   
   setHeizung(renew);
   setWerkstatt(renew);
   setWoZi(renew);
   setBuero(renew);
   setLabor(renew);
   setOG1(renew);
   setOG2(renew);
   setEstrich(renew);
   
 }

void setSPI_DATA(void)
{
   uint8_t i=0;
   char in_string[4];
   newline();
   char buffer[32]={};
   
   strcpy_P(buffer,(char*)PSTR("Daten vom Webserver: "));
   vga_puts(buffer);
   vga_putch(' ');
   
   //strcpy_P(buffer,(char*)PSTR("Status: "));
   //vga_puts(buffer);
   switch (in_startdaten)
   {
      case 0xB0: // Null
      {
         
      }break;
      case 0xB1: // Status
      {
         strcpy_P(buffer,(PGM_P)pgm_read_word(&(task_table_B[1])));
         vga_puts(buffer);
         if (in_hbdaten == 1)
         {
            strcpy_P(buffer,(char*)PSTR("TWI ON"));
            
         }
         else if (in_hbdaten == 0)
         {
            strcpy_P(buffer,(char*)PSTR("TWI OFF"));
            
         }
         else
         {
            strcpy_P(buffer,(char*)PSTR("XXX"));
         }
         
         vga_puts(buffer);
         
         vga_putch(' ');
         //vga_puthex(in_startdaten);
         //         int2hexstring(in_startdaten, (char*)&in_string);
         //         vga_puts(in_string);
         //         newline();
         //vga_putint1(in_hbdaten);
         
      }break;
      case 0xB2: // STATUSCONFIRM
      {
         strcpy_P(buffer,(PGM_P)pgm_read_word(&(task_table_B[2])));
         vga_puts(buffer);
         
      }break;
      case 0xB4: // EEPROMREPORT
      {
         strcpy_P(buffer,(char*)PSTR("EEPROMREPORT"));
         vga_puts(buffer);
         
      }break;
      case 0xB5: // EEPROMCONFIRM
      {
         strcpy_P(buffer,(char*)PSTR("EEPROMCONFIRM"));
         vga_puts(buffer);
         vga_putch(' ');
         vga_puthex(outbuffer[1]);

      }break;
         
      case 0xB7: // EEPROMWRITE
      {
         strcpy_P(buffer,(char*)PSTR("EEPROMWRITE"));
         vga_puts(buffer);
         vga_putch(' ');
         vga_puthex(in_startdaten);
         newline();
         vga_puts("lb: ");
         vga_puthex(in_hbdaten);
         vga_putch(' ');
         vga_puts("hb: ");
         vga_puthex(in_lbdaten);
         newline();
         for (i=0;i<8;i++)
         {
            vga_putch(' ');
            vga_puthex(inbuffer[i]);
       
            //lcd_puthex(inbuffer[i+32]);
            //lcd_putc(' ');
            
         }


      }break;
         
      case 0xC0:
      {
         
         
         strcpy_P(buffer,(PGM_P)pgm_read_word(&(task_table_C[0]))); //
         vga_puts(buffer);
         
         //     vga_puts("DATATASK: ");
         // Daten ausgeben
         vga_putch(' ');
         int2hexstring(in_startdaten, (char*)&in_string);
         vga_puts(in_string);
         vga_putch(' ');
         newline();
         vga_puts("lb: ");
         int2hexstring(in_lbdaten, (char*)&in_string);
         vga_puts(in_string);
         vga_putch(' ');
         vga_puts("hb: ");
         int2hexstring(in_hbdaten, (char*)&in_string);
         vga_puts(in_string);
         vga_putch(' ');
         strcpy_P(buffer,(char*)PSTR("DATA: "));
         vga_puts(buffer);
         vga_putch(' ');
         lcd_gotoxy(0,0);
         for (i=0;i<8;i++)
         {
            vga_putch(' ');
            vga_puthex(inbuffer[i]);
         }
         
         /*
         for (i=0;i<SPI_BUFSIZE;i++)
         {
            if (i%16 ==0)
            {
               newline();
            }
            {
               vga_putch(' ');
               char data_string[4];
               int2hexstring(inbuffer[i], (char*)&data_string);
               vga_puts(data_string);
            }
         }
         */
         
         for (i=0;i<3;i++)
         {
            //lcd_puthex(inbuffer[i+32]);
            //lcd_putc(' ');
            
         }
         
         
         
         
      }break;
         
         
         
   }// switch in_startdaten
   
   newline();
   vga_puts("Daten vom  Master");
   vga_putch(' ');
   //vga_puts("TASK: ");
   char out_string[4];
   int2hexstring(out_startdaten, (char*)&out_string);
   
   switch (out_startdaten)
   {
      case 0xC0: // DATA
      {
         // Task anzeigen
         strcpy_P(buffer,(PGM_P)pgm_read_word(&(task_table_C[0]))); //
         vga_puts(buffer);
         vga_putch(' ');
         vga_puts(out_string);
         

         newline();
         strcpy_P(buffer,(char*)PSTR("HOME:  "));
         vga_puts(buffer);
         for (i=0;i<8;i++)
         {
            vga_putch(' ');
            char data_string[4];
            int2hexstring(outbuffer[i], (char*)&data_string);
            vga_puts(data_string);
            
         }
         
         
         newline();
         strcpy_P(buffer,(char*)PSTR("SOLAR: "));
         vga_puts(buffer);
         for (i=9;i<17;i++)
         {
            vga_putch(' ');
            char data_string[4];
            int2hexstring(outbuffer[i], (char*)&data_string);
            vga_puts(data_string);
            
         }
         
         newline();
         strcpy_P(buffer,(char*)PSTR("ALARM: "));
         vga_puts(buffer);
         for (i=23;i<32;i++)
         {
            vga_putch(' ');
            char data_string[4];
            int2hexstring(outbuffer[i], (char*)&data_string);
            vga_puts(data_string);
            
         }
         
         
         

      }break;
   }// switch out_startdaten
   
   // Dump ausgeben
   /*
   newline();
   strcpy_P(buffer,(char*)PSTR("DUMP: "));
   vga_puts(buffer);
   newline();
   vga_puts("Startadresse lb: ");
   int2hexstring(out_lbdaten, (char*)&out_string);
   vga_puts(out_string);
   vga_putch(' ');
   vga_puts("hb: ");
   int2hexstring(out_hbdaten, (char*)&out_string);
   vga_puts(out_string);
   vga_putch(' ');
   
   
   for (i=0;i<SPI_BUFSIZE;i++)
   {
      if (i%16 ==0)
      {
         newline();
      }
      {
         vga_putch(' ');
         char data_string[4];
         int2hexstring(outbuffer[i], (char*)&data_string);
         vga_puts(data_string);
      }
   }

   */
  
   
   
} // end  setSPI_DATA


void updateSPI_DATA(void)
{
   uint8_t i=0;
   char in_string[4];
   newline();
   char buffer[32]={};
   
   strcpy_P(buffer,(char*)PSTR("Daten vom Webserver: "));
   vga_puts(buffer);
   vga_putch(' ');
   
   //strcpy_P(buffer,(char*)PSTR("Status: "));
   //vga_puts(buffer);
   switch (in_startdaten)
   {
      case 0xB0: // Null
      {
         
      }break;
      case 0xB1: // Status
      {
         strcpy_P(buffer,(PGM_P)pgm_read_word(&(task_table_B[1])));
         vga_puts(buffer);
         if (in_hbdaten == 1)
         {
            strcpy_P(buffer,(char*)PSTR("TWI ON"));
            
         }
         else if (in_hbdaten == 0)
         {
            strcpy_P(buffer,(char*)PSTR("TWI OFF"));
            
         }
         else
         {
            strcpy_P(buffer,(char*)PSTR("XXX"));
         }
         
         vga_puts(buffer);
         
         vga_putch(' ');
         //vga_puthex(in_startdaten);
         //         int2hexstring(in_startdaten, (char*)&in_string);
         //         vga_puts(in_string);
         //         newline();
         //vga_putint1(in_hbdaten);
         
      }break;
      case 0xB2: // STATUSCONFIRM
      {
         strcpy_P(buffer,(PGM_P)pgm_read_word(&(task_table_B[2])));
         vga_puts(buffer);
         
      }break;
      case 0xB4: // EEPROMREPORT
      {
         
      }break;
      case 0xB5: // EEPROMCONFIRM
      {
         
      }break;
         
         
      case 0xC0:
      {
         
         
         strcpy_P(buffer,(PGM_P)pgm_read_word(&(task_table_C[0]))); //
         vga_puts(buffer);
         
         //     vga_puts("DATATASK: ");
         // Daten ausgeben
         vga_putch(' ');
         int2hexstring(in_startdaten, (char*)&in_string);
         vga_puts(in_string);
         
         vga_putch(' ');
         newline();
         vga_puts("Startadresse lb: ");
         int2hexstring(in_lbdaten, (char*)&in_string);
         vga_puts(in_string);
         vga_putch(' ');
         vga_puts("hb: ");
         int2hexstring(in_hbdaten, (char*)&in_string);
         vga_puts(in_string);
         vga_putch(' ');
         vga_putch(' ');
         lcd_gotoxy(0,0);
         for (i=0;i<2;i++)
         {
            //lcd_puthex(inbuffer[i+32]);
            //lcd_putc(' ');
            
         }
         for (i=0;i<SPI_BUFSIZE;i++)
         {
            if (i%16 ==0)
            {
               newline();
            }
            {
               vga_putch(' ');
               char data_string[4];
               int2hexstring(inbuffer[i], (char*)&data_string);
               vga_puts(data_string);
            }
         }
         
         
         for (i=0;i<3;i++)
         {
            //lcd_puthex(inbuffer[i+32]);
            //lcd_putc(' ');
            
         }
         
         
         
         
      }break;
         
         
   }// switch in_startdaten
   
   newline();
   vga_puts("Daten vom  Master");
   vga_putch(' ');
   //vga_puts("TASK: ");
   char out_string[4];
   int2hexstring(out_startdaten, (char*)&out_string);
   
   switch (out_startdaten)
   {
      case 0xC0: // DATA
      {
         // Task anzeigen
         strcpy_P(buffer,(PGM_P)pgm_read_word(&(task_table_C[0]))); //
         vga_puts(buffer);
         vga_putch(' ');
         vga_puts(out_string);
         
         
         newline();
         strcpy_P(buffer,(char*)PSTR("HOME:  "));
         vga_puts(buffer);
         for (i=0;i<8;i++)
         {
            vga_putch(' ');
            char data_string[4];
            int2hexstring(outbuffer[i], (char*)&data_string);
            vga_puts(data_string);
            
         }
         
         
         newline();
         strcpy_P(buffer,(char*)PSTR("SOLAR: "));
         vga_puts(buffer);
         for (i=9;i<17;i++)
         {
            vga_putch(' ');
            char data_string[4];
            int2hexstring(outbuffer[i], (char*)&data_string);
            vga_puts(data_string);
            
         }
         
         newline();
         strcpy_P(buffer,(char*)PSTR("ALARM: "));
         vga_puts(buffer);
         for (i=23;i<32;i++)
         {
            vga_putch(' ');
            char data_string[4];
            int2hexstring(outbuffer[i], (char*)&data_string);
            vga_puts(data_string);
            
         }
         
         
         
         
      }break;
   }// switch out_startdaten
   
   // Dump ausgeben
   /*
   newline();
   strcpy_P(buffer,(char*)PSTR("DUMP: "));
   vga_puts(buffer);
   newline();
   vga_puts("Startadresse lb: ");
   int2hexstring(out_lbdaten, (char*)&out_string);
   vga_puts(out_string);
   vga_putch(' ');
   vga_puts("hb: ");
   int2hexstring(out_hbdaten, (char*)&out_string);
   vga_puts(out_string);
   vga_putch(' ');
   
   
   for (i=0;i<SPI_BUFSIZE;i++)
   {
      if (i%16 ==0)
      {
         newline();
      }
      {
         vga_putch(' ');
         char data_string[4];
         int2hexstring(outbuffer[i], (char*)&data_string);
         vga_puts(data_string);
      }
   }
   */
   
   
   
   
}



uint8_t Tastenwahl(uint8_t Tastaturwert)
{
   //lcd_gotoxy(0,1);
   //lcd_putint(Tastaturwert);
   if (Tastaturwert < TASTE1)
      return 1;
   if (Tastaturwert < TASTE2)
      return 2;
   if (Tastaturwert < TASTE3)
      return 3;
   if (Tastaturwert < TASTE4)
      return 4;
   if (Tastaturwert < TASTE5)
      return 5;
   if (Tastaturwert < TASTE6)
      return 6;
   if (Tastaturwert < TASTE7)
      return 7;
   if (Tastaturwert < TASTE8)
      return 8;
   if (Tastaturwert < TASTE9)
      return 9;
   if (Tastaturwert < TASTEL)
      return 10;
   if (Tastaturwert < TASTE0)
      return 0;
   if (Tastaturwert < TASTER)
      return 12;
   
   return -1;
}




void slaveinit(void)
{
   MANUELL_DDR |= (1<<MANUELLPIN);		//Pin 5 von PORT D als Ausgang fuer Manuell
   MANUELL_PORT &= ~(1<<MANUELLPIN);
   
   LOOPLED_DDR |= (1<<LOOPLED_PIN);
   
   DDRB &= ~(1<<PORTB0);	//Bit 0 von PORT B als Eingang fuer Taste
   PORTB |= (1<<PORTB0);	//HI
   
   
   
   //LCD
   LCD_DDR |= (1<<LCD_RSDS_PIN);	//Pin 5 von PORT B als Ausgang fuer LCD
   LCD_DDR |= (1<<LCD_ENABLE_PIN);	//Pin 6 von PORT B als Ausgang fuer LCD
   LCD_DDR |= (1<<LCD_CLOCK_PIN);	//Pin 7 von PORT B als Ausgang fuer LCD
   
   /*
    // TWI vorbereiten
    TWI_DDR &= ~(1<<SDAPIN);//Bit 4 von PORT C als Eingang für SDA
    TWI_PORT |= (1<<SDAPIN); // HI
    
    TWI_DDR &= ~(1<<SCLPIN);//Bit 5 von PORT C als Eingang für SCL
    TWI_PORT |= (1<<SCLPIN); // HI
    */
   SR_DDR |= (1<<SR_PIN); // Schiebetakt
   SR_PORT |= (1<<SR_PIN);
   
   //TASTATURDDR &= ~(1<<TASTATURKANAL);	//Pin 0 von PORT C als Eingang fuer Tastatur
   //TASTATURPORT &= ~(1<<TASTATURKANAL); //Pull-up
   DDRC &= ~(1<<PORTC0);
   PORTC &= ~(1<<PORTC0);
   
   
   OSZIDDR |= (1<<OSZIA);
   OSZIPORT |= (1<<OSZIA);
   /*
    OSZIDDR |= (1<<OSZIB);
    OSZIPORT |= (1<<OSZIB);
    */
}

void timer0(void) // Analoganzeige
{
   //----------------------------------------------------
   // Set up timer 0
   //----------------------------------------------------
 
   
   TCCR0B = 0;
   TCCR0A = 0;
   
   //TCCR0B|=(1<<CS10);
   //TCCR0A |= (1<<COM0A1);   // set OC0A at bottom, clear OC0A on compare match
   TCCR0B |= 1<<CS02; // /1024
   TCCR0B |= 1<<CS00;
   
   OCR0A=0;
   TIMSK0 |= (1<<TOIE0);
   
   
}

ISR(TIMER0_OVF_vect)
{
   
   if (tastaturstatus & (1<<1)) // Taste gestartet
   {
      prellcounter++;
   }
   else
   {
      prellcounter=0;
   }
    
}


// Interrupt Routine Slave Mode (interrupt controlled)
// Aufgerufen bei fallender Flanke an INT0

ISR( INT1_vect )
{
   
   if (spistatus & (1<<ACTIVE_BIT))									// CS ist LO, Interrupt ist OK
   {
      
      uartstatus |= (1<<SUCCESS_BIT);
      
      //BitCounter++;
      _delay_us(30);																	// PIN lesen:
      
      if (spistatus & (1<<STARTDATEN_BIT))						// out_startdaten senden, in_startdaten laden
      {
         // Output des Webservers laden
         
         if (SPI_CONTROL_PORTPIN & (1<<SPI_CONTROL_MOSI))	// bit ist HI
         {
            in_startdaten |= (1<<(7-bitpos));
         }
         else																// bit ist LO
         {
            in_startdaten |= (0<<(7-bitpos));
         }
         
         
         // Output des Masters laden
         if (SPI_CONTROL_PORTPIN & (1<<SPI_CONTROL_MISO))	// bit ist HI
         {
            out_startdaten |= (1<<(7-bitpos));
            
         }
         else																// bit ist LO
         {
            out_startdaten |= (0<<(7-bitpos));
            
         }
         
         
         // Output laden
         
         /*
          if (out_startdaten & (1<<(7-bitpos)))
          {
          SPI_CONTROL_PORT |= (1<<SPI_CONTROL_MISO);
          }
          else
          {
          SPI_CONTROL_PORT &= ~(1<<SPI_CONTROL_MISO);
          }
          */
         
         bitpos++;
         
         
         if (bitpos>=8) // Byte fertig
         {
            complement=	~in_startdaten;		// Zweiercomplement aus Startdaten bestimmen fuer Enddaten
            
            spistatus &= ~(1<<STARTDATEN_BIT);					// Bit fuer Startdaten zuruecksetzen
            spistatus |= (1<<LB_BIT);								// Bit fuer lb setzen
            
            bitpos=0;
            
         }
         
      }
      
      
      //	LB
      else if (spistatus & (1<<LB_BIT))					// out_lbdaten senden, in_lbdaten laden
      {
         
         if (SPI_CONTROL_PORTPIN & (1<<SPI_CONTROL_MOSI))	// bit ist HI
         {
            in_lbdaten |= (1<<(7-bitpos));
            
            // Echo laden
            //SPI_CONTROL_PORT |= (1<<SPI_CONTROL_MISO);
         }
         else																// bit ist LO
         {
            in_lbdaten |= (0<<(7-bitpos));
            
            // Echo laden
            //SPI_CONTROL_PORT &= ~(1<<SPI_CONTROL_MISO);
         }
         
         // Output des Masters laden
         if (SPI_CONTROL_PORTPIN & (1<<SPI_CONTROL_MISO))	// bit ist HI
         {
            out_lbdaten |= (1<<(7-bitpos));
            
         }
         else																// bit ist LO
         {
            out_lbdaten |= (0<<(7-bitpos));
            
         }
         
         bitpos++;
         
         
         if (bitpos>=8)	// Byte fertig
         {
            spistatus &= ~(1<<LB_BIT);						// Bit fuer lb zuruecksetzen
            spistatus |= (1<<HB_BIT);						// Bit fuer hb setzen
            bitpos=0;
         }
         
      }
      //LB end
      
      //	HB
      else if (spistatus & (1<<HB_BIT))					// out_hbdaten senden, in_hbdaten laden
      {
         
         if (SPI_CONTROL_PORTPIN & (1<<SPI_CONTROL_MOSI))	// bit ist HI
         {
            in_hbdaten |= (1<<(7-bitpos));
            
            // Echo laden
            //SPI_CONTROL_PORT |= (1<<SPI_CONTROL_MISO);
         }
         else																// bit ist LO
         {
            in_hbdaten |= (0<<(7-bitpos));
            
            // Echo laden
            //SPI_CONTROL_PORT &= ~(1<<SPI_CONTROL_MISO);
         }
         
         // Output des Masters laden
         if (SPI_CONTROL_PORTPIN & (1<<SPI_CONTROL_MISO))	// bit ist HI
         {
            out_hbdaten |= (1<<(7-bitpos));
            
         }
         else																// bit ist LO
         {
            out_hbdaten |= (0<<(7-bitpos));
            
         }
         bitpos++;
         
         if (bitpos>=8)	// Byte fertig
         {
            spistatus &= ~(1<<HB_BIT);						// Bit fuer hb zuruecksetzen
            
            bitpos=0;
         }
         
      }
      //HB end
      
      
      
      
      else if (spistatus & (1<<ENDDATEN_BIT))					// out_enddaten senden, in_enddaten laden
      {
         
         if (SPI_CONTROL_PORTPIN & (1<<SPI_CONTROL_MOSI))	// bit ist HI
         {
            in_enddaten |= (1<<(7-bitpos));
            
            // Echo laden
            //SPI_CONTROL_PORT |= (1<<SPI_CONTROL_MISO);
         }
         else																// bit ist LO
         {
            in_enddaten |= (0<<(7-bitpos));
            
            // Echo laden
            //SPI_CONTROL_PORT &= ~(1<<SPI_CONTROL_MISO);
         }
         
         
         // Output des Masters laden
         if (SPI_CONTROL_PORTPIN & (1<<SPI_CONTROL_MISO))	// bit ist HI
         {
            out_enddaten |= (1<<(7-bitpos));
            
         }
         else																// bit ist LO
         {
            out_enddaten |= (0<<(7-bitpos));
            
         }
         
         bitpos++;
         
         
         if (bitpos>=8)	// Byte fertig
         {
            spistatus &= ~(1<<ENDDATEN_BIT);						// Bit fuer Enddaten zuruecksetzen
            bitpos=0;
            //lcd_gotoxy(19,1);
            // lcd_putc('+');
            spistatus |= (1<<SUCCESS_BIT);
            uartstatus |= (1<<SUCCESS_BIT);
            /*
             if (out_startdaten + in_enddaten==0xFF)
             {
             lcd_putc('+');
             //spistatus |= (1<<SUCCESS_BIT);					// Datenserie korrekt geladen
             
             }
             else
             {
             //lcd_putc('-');
             //spistatus &= ~(1<<SUCCESS_BIT);					// Datenserie nicht korrekt geladen
             errCounter++;
             }
             */
            // 24.6.2010
            //				out_startdaten=0xC0; ergab nicht korrekte Pruefsumme mit in_enddaten
            
            //18.7.10
            //				out_hbdaten=0;
            //				out_lbdaten=0;
            
         }
         
      }
      
      else			// Datenarray in inbuffer laden, Daten von outbuffer senden
         
      {
         if (SPI_CONTROL_PORTPIN & (1<<SPI_CONTROL_MOSI))	// bit ist HI
         {
            
            inbuffer[ByteCounter] |= (1<<(7-bitpos));
            
            // Echo laden
            //SPI_CONTROL_PORT |= (1<<SPI_CONTROL_MISO);
         }
         else																// bit ist LO
         {
            inbuffer[ByteCounter] |= (0<<(7-bitpos));
            
            // Echo laden
            //SPI_CONTROL_PORT &= ~(1<<SPI_CONTROL_MISO);
         }
         
         
         // Output des Masters laden
         if (SPI_CONTROL_PORTPIN & (1<<SPI_CONTROL_MISO))	// bit ist HI
         {
            outbuffer[ByteCounter] |= (1<<(7-bitpos));
            
         }
         else																// bit ist LO
         {
            outbuffer[ByteCounter] |= (0<<(7-bitpos));
            
         }
         
         bitpos++;
         
         if (bitpos>=8) // Byte fertig
         {
            if (ByteCounter<SPI_BUFSIZE -1 )						// zweitletztes Byte
            {
               ByteCounter++;
            }
            else
            {
               spistatus |= (1<<ENDDATEN_BIT);					// Bit fuer Enddaten setzen
            }
            bitpos=0;
         }	// if bitpos
      }						//  Datenarray in inbuffer laden, Daten von outbuffer senden
   }						// if (spistatus & (1<<ACTIVE_BIT))
   
}		// ISR


int main (void)
{
   /* INITIALIZE */
   MCUCR |=(1<<7);
   MCUCR |=(1<<7);
   
   
   slaveinit();
   
   lcd_initialize(LCD_FUNCTION_8x2, LCD_CMD_ENTRY_INC, LCD_CMD_ON);
   lcd_puts("Guten Tag\0");
   delay_ms(1000);
   
   //lcd_cls();
   //lcd_puts("READY\0");
   delay_ms(1000);
   // DS1820 init-stuff begin
   
   uart_init();
   
   InitSPI_Slave();
   
   uint8_t linecounter=0;
   
   uint8_t SPI_Call_count0=0;
   lcd_cls();
   lcd_puts("UART\0");
   
   
   // OSZIA_HI;
   //char teststring[] = {"p,10,12"};
   
   
   initADC(TASTATURKANAL);
   
   // vga_start();
   
   setHomeCentralblank();
   vga_command("f,1");
   startcounter = 0;
   linecounter=0;
   uint8_t lastrand=rand();
   //srand(1);
   //timer0();
   sei();
      /*
    uint8_t i=0;
    for (i=0;i<20;i++)
    {
    lcd_gotoxy(i,0);
    lcd_putc(65+i);
    }
    */
   
   
   
#pragma mark while
   while (1)
   {
      loopCount0 ++;
      
      if (loopCount0 >=0x00FF)
      {
         //readSR();
         loopCount1++;
         
         if ((loopCount1 >0x01FF) )//&& (!(Programmstatus & (1<<MANUELL))))
         {
            LOOPLED_PORT ^= (1<<LOOPLED_PIN);
            //lcd_gotoxy(0,0);
            //lcd_putint(startcounter++);
            //lcd_putc('*');
            
            if ((uartstatus & (1<< SUCCESS_BIT))&& (SPI_CONTROL_PORTPIN & (1<<SPI_CONTROL_CS_HC)))
            {
               if (!(uartstatus & (1<< UART_STOP)))
               {
#pragma mark Ausgabe
                  //UART Stop anfang
                  
                  //lcd_clr_line(1);
                  //lcd_gotoxy(0,0);
                  //lcd_puts("OK \0");
                  //                  lcd_puthex(in_startdaten+in_enddaten);
                  //                  lcd_putc(' ');
                  //lcd_putint(incounter);
                  //lcd_puthex(in_enddaten);
                  //lcd_putc(' ');
                  //                  lcd_puthex(out_startdaten+out_enddaten);
                  //lcd_putc(' ');
                  //lcd_putint(incounter);
                  //lcd_puthex(out_enddaten);
                  
              //
            cli();
                  //delay_ms(100);
                  
                  
                  
                  
                  //if (!(in_startdaten == 0xC0))
                  {
                     char buffer[32]={};
                     /*
                     newline();
                     vga_puts("Daten vom Webserver:");
                     vga_putch(' ');

                     
                     //strcpy_P(buffer,(char*)PSTR("Status: "));
                     //vga_puts(buffer);
                     switch (in_startdaten)
                     {
                        case 0xC0:
                        {
                           
                           //strcpy_P(buffer,getString(task_C0));
                           strcpy_P(buffer,(PGM_P)pgm_read_word(&(task_table_C[0])));
                           //strcpy_P(buffer, (const char*)pgm_read_word(&(task_C0)));
                           //strcpy_P(buffer,(PGM_P)pgm_read_word(&task_C0));
                           vga_puts(buffer);
                           
                           //     vga_puts("DATATASK: ");
                           
                        }break;
                        case 0xB0: // Null
                        {
                           
                        }break;
                        case 0xB1: // Status
                        {
                           strcpy_P(buffer,(char*)PSTR("Status: "));
                           vga_puts(buffer);
                           vga_putint1(in_hbdaten);
                           
                        }break;
                        case 0xB2: // STATUSCONFIRM
                        {
                           
                        }break;
                        case 0xB4: // EEPROMREPORT
                        {
                           
                        }break;
                        case 0xB5: // EEPROMCONFIRM
                        {
                           
                        }break;
                           
                           
                     }// switch in_startdate
                     */
                //     vga_command("f,5");
                //     newline();

                //     setSPI_DATA();
                     //
                      // Ausgang Master
                     
                     //   Zeit im Titelbalken angeben
                     //uint8_t stunde = (outbuffer[0] & 0x1F); // Stunde, Bit 0-4
                     //uint8_t minute = (outbuffer[1] & 0x3F); // Minute, Bit 0-5
                     uint8_t stunde = (outbuffer[46] ); // Stunde, Bit 0-4
                     uint8_t minute = (outbuffer[47] ); // Minute, Bit 0-5
                     
                     uint8_t tagdesmonats = outbuffer[40] ; // datum tag

                     uint8_t monat = (outbuffer[41] & 0x0F ); // datum monat: 1-3 jahr ab 2010: 4-7
                     uint8_t jahr = ((outbuffer[41] & 0xF0 )>>4) + 10; // datum monat: 1-3 jahr ab 2010: 4-7

                     // Tag ausgeben
                     
                     
                     vga_command("f,1");
                     vga_gotoxy(00,0);
                     vga_command("f,1");
                     vga_puts("Home Central Rueti");

                     
                     
                     // Zeit ausgeben
                     
                     vga_command("f,1");
                     vga_gotoxy(75,0);
                     vga_command("f,1");
                     
                     
                     strcpy_P(buffer,(PGM_P)pgm_read_word(&(tag_table[outbuffer[45]-1])));
                     vga_puts(buffer);
                    
                     vga_putch(' ');
                     //vga_puthex(outbuffer[41]);
                     //vga_putch(' ');
                     vga_putint2(tagdesmonats);
                     vga_putch(':');
                    
                     vga_putint2(monat);
                     vga_putch(' ');
                     vga_putint2(20);
                     vga_putint2(jahr);
                     vga_putch(' ');
                     vga_putch(' ');
                     /*
                     vga_putint2(stunde);
                     vga_putch(':');
                     vga_putint2(minute);
                     vga_putch(' ');
                     */
                     vga_put_zeit(minute, stunde);
                     
                     //vga_putch(' ');
                     //vga_putint(incounter);
                     
                     vga_command("f,2");
                     vga_command("p,0,1");
                     vga_command("f,2");
                     //   setHeizung(outbuffer[2]/2,outbuffer[3]/2,outbuffer[4],outbuffer[5]);
                     
                     uint8_t renew = incounter%8;
                     if (incounter==0) // start
                     {
                        //renew = 1;
                     }
                     
                     /*
                     switch (incounter%8)
                     {
                        case 0:
                        {
                        }break;
                        case 1:
                        {
                         }break;

                        case 2:
                        {
                        }break;
                        case 3:
                        {
                         }break;
                        case 4:
                        {
                         }break;
                        case 5:
                        {
                         }break;
                        case 6:
                        {
                        }break;
                        case 7:
                        {
                         }break;
                     }// switch incounter
                      */
                     setRaumData(renew);
                     
                     setStatusData();
                     
                     setErrorData();
                  }
 /*
                  vga_command("f,2");
  
                  // Heizung ausgeben
                  
                  vga_command("f,2");
                  vga_command("p,0,1");
                  vga_command("f,2");
                  setHeizung(outbuffer[2]/2,outbuffer[3]/2,outbuffer[4],outbuffer[5]);
                  
                  vga_command("f,4");
                  vga_gotoxy(18,1);
                  vga_command("f,4");
                  uint8_t innen = (outbuffer[7]);
                  //vga_puthex(aussen);
                  //vga_putch(' ');
                  //vga_putint_right(innen);
                  //vga_putch(' ');
                  //char tempbuffer[8]={};
                  char tempbuffer[8]={};
                  vga_tempbis99(innen,tempbuffer);
                  vga_puts(tempbuffer);
                  
                  vga_command("f,8");
                  vga_command("p,0,1");
                  vga_command("f,8");
                  setEstrich(outbuffer[9]/2,outbuffer[10]/2,outbuffer[11]/2,outbuffer[12]/2,outbuffer[13]/2,outbuffer[14]/2,outbuffer[15]);
                  
                  //setEstrich(uint8_t vorlauf, uint8_t ruecklauf, uint8_t kollektor, uint8_t boileru, uint8_t boilerm, uint8_t boilero, uint8_t status)
 */
                  incounter++;
               }
               // UART Stop end
               
               uartstatus &= ~(1<< SUCCESS_BIT);
               
               // aufraeumen
               out_startdaten=0x00;
               out_hbdaten=0;
               out_lbdaten=0;
               out_enddaten=0x00;
               
               in_startdaten=0x00;
               in_hbdaten=0;
               in_lbdaten=0;
               in_enddaten=0x00;
               
               for (int i=0;i<SPI_BUFSIZE;i++)
               {
                  outbuffer[i]=0;
                  inbuffer[i]=0;
               }
               
               
               sei();
               
            }
            else
            {
               //lcd_gotoxy(0,0);
               //lcd_puts("  \0");
               
            }
            {
               loopCount1=0;
               
            } // if startcounter
            
         }
         
         loopCount0 =0;
         //
#pragma mark Tastatur
         //goto NEXT;
         {
           // cli();
            
            {
               Tastenwert=(uint8_t)(adc_read(TASTATURKANAL)>>2);
               Tastenwert=0;
               //lcd_gotoxy(12,1);
               //lcd_puts("TW:\0");
               //lcd_putint(Tastenwert);
               if ((Tastenwert>5))// && (! (tastaturstatus & (1<<1)) )) //
               {
                  if ( tastaturstatus & (1<<1)) // Taste war schon gedrückt, Aktion starten
                  {
                     
                     TastaturCount++; // Zaehlen
                     if (TastaturCount > 60)
                     {
                        tastaturstatus |= (1<<7);
                     }
                  }
                  else
                  {
                     tastaturstatus |= (1<<1);
                     
                  }
                  
                  
                  //			wdt_reset();
                  /*
                   0:
                   1:
                   2: Menu der aktuellen Ebene nach oben
                   3: lcd_initialize
                   4: Auf aktueller Ebene nach rechts ()
                   5: Ebene tiefer
                   6: Auf aktueller Ebene nach links ()
                   7:
                   8: Menu der aktuellen Ebene nach unten
                   9:
                   10:
                   11:
                   12: Ebene höher
                   */
                  
                 
                  if (prellcounter > 100)
                  {
 
                     //tastaturstatus &= ~(1<<1);
                     prellcounter=0;
                  }
                   if (tastaturstatus & (1<<7))	//	Taste erkannt
                  {
                     tastaturstatus &= ~(1<<7);
                     Taste=Tastenwahl(Tastenwert);
 
                     lcd_gotoxy(16,1);
                     lcd_putc('T');
                     //
                     //lcd_putc(' ');
                     lcd_putint(Tastenwert);
                     //lcd_putc(' ');
                     if (Taste >=0)
                     {
                        //lcd_putint2(Taste);
                     }
                     else
                     {
                        //lcd_putc('*');
                     }
                     
                     //lcd_gotoxy(14,1);
                     
                     //Taste = 2;
                     //lcd_putint(linecounter);
                     //lcd_putc(' ');
                     //lcd_putint((uint8_t)rand()%40);
                     
                     switch (Taste)
                     {
                        case 1:
                        {
                           if (uartstatus & (1<<UART_STOP)) // UART gestopt
                           {
                              vga_command("f,1");
                              vga_gotoxy(60,0);
                              vga_command("f,1");
                              vga_puts("    ");
                              uartstatus &= ~(1<<UART_STOP);
                           }
                           else
                           {
                              vga_command("f,1");
                              vga_gotoxy(60,0);
                              vga_command("f,1");
                              vga_puts("STOP");
                              uartstatus |= (1<<UART_STOP);
                              
                           }
                        }break;
                        case 2:
                        {
                           
                           switch (curr_screen)
                           {
                              case HOMESCREEN: // home
                              {
                                 
                              }break;
                                 
                              case RAUMSCREEN: // Settings
                              {
                                 
                              }
                           }//switch curr-screen
                           vga_command("f,3");
                           vga_gotoxy(cursorx,cursory);
                           //vga_command("f,3");
                           //vga_putch(' ');
                           
                           cursory--;
                           vga_gotoxy(cursorx,cursory);
                           //vga_command("f,3");
                           //vga_putch('>');
                           
                        }break;
                        case 3:
                        {
                           lcd_initialize(LCD_FUNCTION_8x2, LCD_CMD_ENTRY_INC, LCD_CMD_ON);
                           lcd_puts("Guten Tag\0");
                           delay_ms(1000);
                           
                        }break;
                        case 4:
                        {
                           if (menuebene)
                           {
                              menuebene--;
                              if (cursorx>10)
                              {
                                 vga_command("f,3");
                                 vga_gotoxy(cursorx,cursory);
                                 vga_command("f,3");
                                 //         vga_putch(' ');
                                 cursorx-=10;
                                 vga_gotoxy(cursorx,cursory);
                                 vga_command("f,3");
                                 //         vga_putch('>');
                              }
                           }
                           
                        }break;
                        case 5:
                        {
                           vga_command("f,3");
                           vga_gotoxy(cursorx,cursory);
                           vga_command("f,3");
                           vga_putch('>');
                           
                        }break;
                        case 6:
                        {
                           if (menuebene<MAXMENUEBENE)
                           {
                              menuebene++;
                              if (cursorx<40)
                              {
                                 vga_command("f,3");
                                 vga_gotoxy(cursorx,cursory);
                                 vga_command("f,3");
                                 vga_putch(' ');
                                 
                                 cursorx+=10;
                                 vga_gotoxy(cursorx,cursory);
                                 vga_command("f,3");
                                 vga_putch('>');
                              }
                           }
                        }break;
                        case 7:
                        {
                           
                           setFeld(3,70,3,30,32,1,"");
                           vga_command("f,3");
                           //vga_putch('x');
                           //lcd_putc('+');
                           
                        }break;
                        case 8:
                        {
                           vga_command("f,3");
                           vga_gotoxy(cursorx,cursory);
                           vga_command("f,3");
                           vga_putch(' ');
                           
                           cursory++;
                           vga_gotoxy(cursorx,cursory);
                           vga_command("f,3");
                           vga_putch('>');
                           
                        }break;
                        case 9:
                        {
                           
                        }break;
                        case 10:
                        {
                           setHomeCentral();
                        }break;
                        case 12:
                        {
                           vga_command("f,3");
                           vga_command("e");
                           vga_gotoxy(cursorx,cursory);
                           vga_command("f,3");
                           vga_putch(' ');
                           cursorx = CURSORX;
                           cursory = CURSORY;
                           vga_gotoxy(CURSORX+1,CURSORY);
                           vga_command("f,3");
                           vga_puts("Alpha");
                           vga_gotoxy(CURSORX+1,CURSORY+1);
                           vga_command("f,3");
                           vga_puts("Beta");
                           vga_gotoxy(CURSORX+1,CURSORY+2);
                           vga_command("f,3");
                           vga_puts("Gamma");
                           vga_gotoxy(CURSORX+1,CURSORY+3);
                           vga_command("f,3");
                           vga_puts("Delta");
                           vga_gotoxy(cursorx,cursory);
                           vga_command("f,3");
                           vga_putch('>');
                           
                        }break;
                           
                           
                     } // switch Taste
                     
                     
                     
                     lastrand = rand();
                     vga_command("f,2");
                     //vga_putch(' ');
                     
                     //vga_gotoxy(4,linecounter);
                     //lcd_gotoxy(16,1);
                     //lcd_putint(erg);
                     /*
                      putint(linecounter);
                      vga_putch(' ');
                      putint_right(Tastenwert);
                      vga_putch(' ');
                      putint_right(Taste);
                      */
                     //newline();
                     linecounter++;
                     TastaturCount=0;
                     tastaturstatus &= ~(1<<1);
                  }
                  
               } // if Tastenwert
               
            }
            
         }
         
         //x=0;
         sei();
         //
      }
      
      /* *** SPI begin **************************************************************/
      
      //lcd_gotoxy(19,0);
      //lcd_putc('-');
      
      // ***********************
      //goto ENDSPI;
      if (SPI_CONTROL_PORTPIN & (1<< SPI_CONTROL_CS_HC)) // PIN ist Hi, SPI ist Passiv
      {
         // lcd_gotoxy(19,1);
         //  lcd_putc('*');
         
         // ***********************
         /*
          Eine Uebertragung hat stattgefunden.
          (Die out-Daten sind auf dem Webserver.)
          Die in-Daten vom Webserver sind geladen.
          Sie muessen noch je nach in_startdaten ausgewertet werden.
          */
         
         // ***********************
         //			SPI_CONTROL_PORT |= (1<<SPI_CONTROL_MISO); // MISO ist HI in Pausen
         
#pragma mark PASSIVE
         
         if (spistatus &(1<<ACTIVE_BIT)) // Slave ist neu passiv geworden. Aufraeumen, Daten uebernehmen
         {
            wdt_reset();
            SPI_Call_count0++;
            // Eingang von Interrupt-Routine, Daten von Webserver
            //lcd_gotoxy(19,0);
            //lcd_putc(' ');
            
            // in lcd verschoben
            lcd_clr_line(0);
            lcd_gotoxy(0,0);
            
            // Eingang anzeigen
            lcd_puts("iW \0");
            lcd_puthex(in_startdaten);
            lcd_putc(' ');
            //lcd_puthex(in_hbdaten);
            //lcd_puthex(in_lbdaten);
            lcd_putc(' ');
            uint8_t j=0;
            
            for (j=0;j<4;j++)
            {
               //lcd_putc(' ');
               lcd_puthex(inbuffer[j]);
               //lcd_putc(inbuffer[j]);
            }
            
            lcd_clr_line(1);
            lcd_gotoxy(0,1);
            // Ausgang Master anzeigen
            lcd_puts("iM \0");
            lcd_puthex(out_startdaten);
            lcd_putc(' ');
            //lcd_puthex(out_hbdaten);
            //lcd_puthex(out_lbdaten);
            lcd_putc(' ');
            
            
            for (j=0;j<4;j++)
            {
               //lcd_putc(' ');
               lcd_puthex(outbuffer[j]);
               //lcd_putc(inbuffer[j]);
            }
            
            OutCounter++;
            
            
            // Uebertragung pruefen
            
            //lcd_gotoxy(7,0);
            //lcd_puts("bc:\0");
            //lcd_puthex(ByteCounter);
            
            //lcd_gotoxy(0,0);
            //lcd_puts("      \0");
            
            lcd_gotoxy(19,0);
            lcd_putc(' ');
            
            lcd_gotoxy(19,3);
            lcd_putc(' ');
            
            if (ByteCounter == SPI_BUFSIZE-1) // Uebertragung war vollstaendig
            {
               lcd_gotoxy(18,1);
               lcd_putc('f');
               
               //if (out_startdaten + in_enddaten==0xFF)
               {
                  lcd_gotoxy(19,1);
                  lcd_putc('+');
                  spistatus |= (1<<SUCCESS_BIT); // Bit fuer vollstaendige und korrekte  Uebertragung setzen
                  lcd_gotoxy(19,0);
                  lcd_putc(' ');
                  lcd_gotoxy(16,0);
                  lcd_putc(' ');
                  lcd_puthex(outbuffer[SLAVESTATUSRAUM]);
                  uint8_t i=0;
                  for (i=0;i<6;i++)
                  {
                     
                   //  slavestatus[outbuffer[SLAVESTATUSRAUM]][i] = outbuffer[SLAVESTATUSPOS+i];
                     slavestatus[i][0] = outbuffer[SLAVESTATUSPOS+2*i];
                     slavestatus[i][1] = outbuffer[SLAVESTATUSPOS+2*i+1];
                              
                  }
                  
                  lcd_gotoxy(0,2);
                  lcd_puthex(outbuffer[SLAVESTATUSRAUM]);
                  lcd_puthex(outbuffer[SLAVESTATUSRAUM+1]);
                  lcd_putc(' ');

                  lcd_puthex(outbuffer[SLAVESTATUSRAUM+2]);
                  lcd_puthex(outbuffer[SLAVESTATUSRAUM+3]);
                  lcd_putc(' ');

                  lcd_puthex(outbuffer[SLAVESTATUSRAUM+4]);
                  lcd_puthex(outbuffer[SLAVESTATUSRAUM+5]);
 
                  lcd_gotoxy(0,3);
                  lcd_puthex(outbuffer[SLAVESTATUSRAUM+6]);
                  lcd_puthex(outbuffer[SLAVESTATUSRAUM+7]);
                  lcd_putc(' ');
                  
                  lcd_puthex(outbuffer[SLAVESTATUSRAUM+8]);
                  lcd_puthex(outbuffer[SLAVESTATUSRAUM+9]);
                  lcd_putc(' ');
                  
                  lcd_puthex(outbuffer[SLAVESTATUSRAUM+10]);
                  lcd_puthex(outbuffer[SLAVESTATUSRAUM+11]);

                  //lcd_clr_line(3);
                  //lcd_gotoxy(0,1);
                  //lcd_puthex(loopCounterSPI++);
                  //lcd_puts("OK \0");
                  
                  //lcd_puthex(out_startdaten + in_enddaten);
                  //					if (out_startdaten==0xB1)
                  {
                     SendOKCounter++;
                  }
                  spistatus |= (1<<SPI_SHIFT_IN_OK_BIT);
               }
               
               
               
               /*
                else
                {
                spistatus &= ~(1<<SUCCESS_BIT); // Uebertragung fehlerhaft, Bit loeschen
                lcd_putc('-');
                lcd_clr_line(1);
                lcd_gotoxy(12,1);
                lcd_puts("ER1\0");
                lcd_putc(' ');
                lcd_puthex(out_startdaten);
                lcd_puthex(in_enddaten);
                lcd_putc(' ');
                lcd_puthex(out_startdaten + in_enddaten);
                
                spistatus &= ~(1<<SPI_SHIFT_IN_OK_BIT);
                {
                SendErrCounter++;
                }
                //errCounter++;
                }
                */
               
               
               
               vga_command("f,5");
               newline();

               setSPI_DATA();
            }
            else
            {
               lcd_gotoxy(18,1);
               lcd_putc('e');
               
               spistatus &= ~(1<<SUCCESS_BIT); //  Uebertragung unvollstaendig, Bit loeschen
               //lcd_clr_line(0);
               lcd_gotoxy(10,1);
               lcd_puts("ER2\0");
               
               //lcd_putc(' ');
               //lcd_puthex(BitCounter);
               
               /*
                lcd_putc(' ');
                lcd_puthex(out_startdaten);
                lcd_puthex(in_enddaten);
                lcd_putc(' ');
                lcd_puthex(out_startdaten + in_enddaten);
                */
               //delay_ms(100);
               //errCounter++;
               IncompleteCounter++;
               lcd_gotoxy(14,1);
               lcd_putint(IncompleteCounter);
               
               spistatus &= ~(1<<SPI_SHIFT_IN_OK_BIT);
            }
            
            //lcd_gotoxy(11, 1);							// Events zahelen
            //lcd_puthex(OutCounter);
            /*
             lcd_puthex(SendOKCounter);
             lcd_puthex(SendErrCounter);
             lcd_puthex(IncompleteCounter);
             */
            /*
             lcd_gotoxy(0,0);
             lcd_putc('i');
             lcd_puthex(in_startdaten);
             lcd_puthex(complement);
             lcd_putc(' ');
             lcd_putc('a');
             lcd_puthex(out_startdaten);
             lcd_puthex(in_enddaten);
             lcd_putc(' ');
             lcd_putc('l');
             lcd_puthex(in_lbdaten);
             lcd_putc(' ');
             lcd_putc('h');
             lcd_puthex(in_hbdaten);
             out_hbdaten++;
             out_lbdaten--;
             
             lcd_putc(out_startdaten);
             */
            /*
             lcd_gotoxy(0,0);
             lcd_puthex(inbuffer[9]);
             lcd_puthex(inbuffer[10]);
             lcd_puthex(inbuffer[11]);
             lcd_puthex(inbuffer[12]);
             lcd_puthex(inbuffer[13]);
             */
            //lcd_gotoxy(13,0);								// SPI - Fehler zaehlen
            //lcd_puts("ERR    \0");
            //lcd_gotoxy(17,0);
            //lcd_puthex(errCounter);
            
            // Bits im Zusammenhang mit der Uebertragung zuruecksetzen. Wurden in ISR gesetzt
            spistatus &= ~(1<<ACTIVE_BIT);		// Bit 0 loeschen
            spistatus &= ~(1<<STARTDATEN_BIT);	// Bit 1 loeschen
            spistatus &= ~(1<<ENDDATEN_BIT);		// Bit 2 loeschen
            spistatus &= ~(1<<SUCCESS_BIT);		// Bit 3 loeschen
            spistatus &= ~(1<<LB_BIT);				// Bit 4 loeschen
            spistatus &= ~(1<<HB_BIT);				// Bit 5 loeschen
            
            // aufraeumen
            //out_startdaten=0x00;
            //out_hbdaten=0;
            //out_lbdaten=0;
            for (int i=0;i<SPI_BUFSIZE;i++)
            {
               //outbuffer[i]=0;
            }
            
            /*
             lcd_gotoxy(0,0);				// Fehler zaehlen
             lcd_puts("IC   \0");
             lcd_gotoxy(2,0);
             lcd_puthex(IncompleteCounter);
             lcd_gotoxy(5,0);
             lcd_puts("TW   \0");
             lcd_gotoxy(7,0);
             lcd_puthex(TWI_errCounter);
             
             lcd_gotoxy(5,1);
             lcd_puts("SE   \0");
             lcd_gotoxy(7,1);
             lcd_puthex(SendErrCounter);
             */
         } // if Active-Bit
         
#pragma mark HomeCentral-Tasks 
         
      } // neu Passiv
      
      // letzte Daten vom Webserver sind in inbuffer und in in_startdaten, in_lbdaten, in_hbdaten
      
      
      else						// (IS_CS_HC_ACTIVE)
      {
         if (!(spistatus & (1<<ACTIVE_BIT))) // CS ist neu aktiv geworden, Daten werden gesendet, Active-Bit 0 ist noch nicht gesetzt
         {
            // Aufnahme der Daten vom Webserver vorbereiten
            
            uint8_t j=0;
            in_startdaten=0;
            in_enddaten=0;
            in_lbdaten=0;
            in_hbdaten=0;
            for (j=0;j<SPI_BUFSIZE;j++)
            {
               inbuffer[j]=0;
            }
            
            spistatus |=(1<<ACTIVE_BIT); // Bit 0 setzen: neue Datenserie
            spistatus |=(1<<STARTDATEN_BIT); // Bit 1 setzen: erster Wert ergibt StartDaten
            
            bitpos=0;
            ByteCounter=0;
            //timer0(); // Ueberwachung der Zeit zwischen zwei Bytes. ISR setzt bitpos und ByteCounter zurueck, loescht Bit 0 in spistatus
            
            // Anzeige, das  rxdata vorhanden ist
            lcd_gotoxy(19,0);
            lcd_putc('$');
            lcd_gotoxy(19,1);
            lcd_putc(' ');
            
            
            
            
         }//										if (!(spistatus & (1<<ACTIVE_BIT)))
         
      }//											(IS_CS_HC_ACTIVE)
# pragma mark    ENDSPI:
      /* *** SPI end **************************************************************/
      
      
# pragma mark Tasten
      /*
       if (!(PINB & (1<<PORTB0))) // Taste 0
       {
       //lcd_gotoxy(12,0);
       //lcd_puts("P0 Down\0");
       //wdt_reset();
       if (! (TastenStatus & (1<<PORTB0))) //Taste 0 war nicht nicht gedrueckt
       {
       TastenStatus |= (1<<PORTB0);
       Tastencount=0;
       //lcd_gotoxy(12,0);
       //lcd_puts("P0\0");
       //lcd_putint(TastenStatus);
       //delay_ms(800);
       }
       else
       {
       
       Tastencount ++;
       //lcd_gotoxy(7,1);
       //lcd_puts("TC \0");
       //lcd_putint(Tastencount);
       wdt_reset();
       if (Tastencount >= Tastenprellen)
       {
       //lcd_gotoxy(18,0);
       //lcd_puts("ON\0");
       //lcd_putint(TastenStatus);
       
       Tastencount=0;
       TastenStatus &= ~(1<<PORTB0);
       
       }
       }//else
       
       }
       */
      
      
      
   } // while
   
   
   
   return 0;
}
