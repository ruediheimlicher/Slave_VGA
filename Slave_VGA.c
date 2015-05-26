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

volatile uint16_t                manuellcounter=0; // Countr fuer Timeout
volatile uint8_t                 programmstatus=0x00;
volatile uint16_t                prellcounter=0; // Countr fuer Prellen, in timer0 OVF incr

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

void setHeizung(void)
{
   vga_command("f,2");
   uint8_t pos_y = HEIZUNG_Y;
   char buffer[12]={};
   vga_gotoxy(TAB0,pos_y);
   vga_command("f,2");
   strcpy_P(buffer,(PGM_P)pgm_read_word(&(raum_table[0])));
   vga_puts(buffer);
   
   pos_y++;
   vga_gotoxy(TAB0,pos_y);
   vga_command("f,2");

   strcpy_P(buffer,(PGM_P)pgm_read_word(&(heizung_table[0])));
   vga_puts(buffer);
   vga_putint_right(outbuffer[2]/2);
   
   // Mode
   vga_gotoxy(TAB1,pos_y);
   vga_command("f,2");
   strcpy_P(buffer,(PGM_P)pgm_read_word(&(heizung_table[3])));
   vga_puts(buffer);
   if (outbuffer[5] & 0x04)
   {
      vga_puts("OFF");
   }
   else
   {
      vga_puts(" ON");
   }
   //vga_leerschlag(3);
   
   // Brenner
   vga_gotoxy(TAB2,pos_y);
   vga_command("f,2");
   strcpy_P(buffer,(PGM_P)pgm_read_word(&(heizung_table[4])));
   vga_puts(buffer);
   
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
   vga_gotoxy(TAB0,pos_y);
   vga_command("f,2");
   strcpy_P(buffer,(PGM_P)pgm_read_word(&(heizung_table[1])));
   vga_puts(buffer);
   vga_putint_right(outbuffer[3]/2);
   //vga_leerschlag(3);
   
   // Schalterposition
   vga_gotoxy(TAB1,pos_y);
   vga_command("f,2");
   strcpy_P(buffer,(PGM_P)pgm_read_word(&(heizung_table[5])));
   vga_puts(buffer);
   vga_putint_right(outbuffer[0]%4);
   //vga_leerschlag(3);
   
   // Beschreibung
   vga_gotoxy(TAB2,pos_y);
   vga_command("f,2");
   strcpy_P(buffer,(PGM_P)pgm_read_word(&(pos_table[outbuffer[0]%4])));
   vga_puts(buffer);
   
   
   // dritte Zeile
   pos_y++;
   vga_gotoxy(TAB0,pos_y);
   vga_command("f,2");
   char tempbuffer[12] = {};
   strcpy_P(buffer,(PGM_P)pgm_read_word(&(heizung_table[2])));
   vga_puts(buffer);
   vga_tempbis99(outbuffer[4]/2-0x20,tempbuffer);
   vga_puts(tempbuffer);
   
   // Rinne
   vga_gotoxy(TAB2,pos_y);
   vga_command("f,2");
   strcpy_P(buffer,(PGM_P)pgm_read_word(&(heizung_table[6])));
   vga_puts(buffer);
   if (outbuffer[5] & 0xC0)
   {
      vga_puts(" ON");
   }
   else
   {
      vga_puts("OFF");
   }

}

void setWerkstatt(void)
{
   char buffer[12]={};
   // Werkstatt
   vga_command("f,2");
   uint8_t pos_y = WERKSTATT_Y;
   vga_gotoxy(TAB0,pos_y);
   vga_command("f,2");
   strcpy_P(buffer,(PGM_P)pgm_read_word(&(raum_table[1])));
   vga_puts(buffer);
   
   // Zeile 1
   // Lampe
   pos_y++;
   vga_gotoxy(TAB0,pos_y);
   vga_command("f,2");
   strcpy_P(buffer,(PGM_P)pgm_read_word(&(WS_table[0])));
   vga_puts(buffer);
   if (outbuffer[5] & 0x04)
   {
      vga_puts("OFF");
   }
   else
   {
      vga_puts(" ON");
   }
   
   // Ofen
   
   vga_gotoxy(TAB1,pos_y);
   vga_command("f,2");
   strcpy_P(buffer,(PGM_P)pgm_read_word(&(WS_table[1])));
   vga_puts(buffer);
   if (outbuffer[5] & 0x04)
   {
      vga_puts("OFF");
   }
   else
   {
      vga_puts(" ON");
   }
   
   vga_gotoxy(TAB2,pos_y);
   vga_command("f,2");
   strcpy_P(buffer,(PGM_P)pgm_read_word(&(WS_table[2])));
   vga_puts(buffer);
   vga_putint_right(outbuffer[3]/2);

   
   //Zeile 2
   // Wasser
   pos_y++;
   vga_gotoxy(TAB1,pos_y);
   vga_command("f,2");
   strcpy_P(buffer,(PGM_P)pgm_read_word(&(WS_table[4])));
   vga_puts(buffer);
   if (outbuffer[5] & 0x04)
   {
      vga_puts("  OK");
   }
   else
   {
      vga_puts("Alarm");
   }

   
}


void setWoZi(void)
{
   char buffer[12]={};
   // WoZi
   vga_command("f,2");
   uint8_t pos_y = WOZI_Y;
   vga_gotoxy(TAB0,pos_y);
   vga_command("f,2");
   strcpy_P(buffer,(PGM_P)pgm_read_word(&(raum_table[2])));
   vga_puts(buffer);
   
   // Zeile 1
   // Lampe
   pos_y++;
   vga_gotoxy(TAB0,pos_y);
   vga_command("f,2");
   strcpy_P(buffer,(PGM_P)pgm_read_word(&(WOZI_table[0])));
   vga_puts(buffer);
   if (outbuffer[5] & 0x04)
   {
      vga_puts("OFF");
   }
   else
   {
      vga_puts(" ON");
   }
   
  // Innen
   
   vga_gotoxy(TAB2,pos_y);
   vga_command("f,2");
   strcpy_P(buffer,(PGM_P)pgm_read_word(&(WOZI_table[1])));
   vga_puts(buffer);
   vga_putint_right(outbuffer[3]/2);
   
   
   //Zeile 2
   // Radiator
   pos_y++;
   vga_gotoxy(TAB0,pos_y);
   vga_command("f,2");
   strcpy_P(buffer,(PGM_P)pgm_read_word(&(WOZI_table[2])));
   vga_puts(buffer);
   if (outbuffer[5] & 0x04)
   {
      vga_puts("  OK");
   }
   else
   {
      vga_puts("Alarm");
   }
   
   
}

void setBuero(void)
{
   char buffer[12]={};
   // Buero
   vga_command("f,2");
   uint8_t pos_y = BUERO_Y;
   vga_gotoxy(TAB0,pos_y);
   vga_command("f,2");
   strcpy_P(buffer,(PGM_P)pgm_read_word(&(raum_table[3])));
   vga_puts(buffer);

}

void setLabor(void)
{
   char buffer[12]={};
   // Labor
   vga_command("f,2");
   uint8_t pos_y = LABOR_Y;
   vga_gotoxy(TAB0,pos_y);
   vga_command("f,2");
   strcpy_P(buffer,(PGM_P)pgm_read_word(&(raum_table[4])));
   vga_puts(buffer);
   
}

void setOG1(void)
{
   char buffer[12]={};
   // OG 1
   vga_command("f,2");
   uint8_t pos_y = OG1_Y;
   vga_gotoxy(TAB0,pos_y);
   vga_command("f,2");
   strcpy_P(buffer,(PGM_P)pgm_read_word(&(raum_table[5])));
   vga_puts(buffer);
   
}

void setOG2(void)
{
   char buffer[12]={};
   // OG 2
   vga_command("f,2");
   uint8_t pos_y = OG2_Y;
   vga_gotoxy(TAB0,pos_y);
   vga_command("f,2");
   strcpy_P(buffer,(PGM_P)pgm_read_word(&(raum_table[6])));
   vga_puts(buffer);
   
}

void setEstrich(void)
{
   char buffer[12]={};
   // Estrich
   vga_command("f,2");
   uint8_t pos_y = ESTRICH_Y;
   vga_gotoxy(TAB0,pos_y);
   vga_command("f,2");
   strcpy_P(buffer,(PGM_P)pgm_read_word(&(raum_table[7])));
   vga_puts(buffer);
   
   //Zeile 1
   pos_y++;
   // Vorlauf
   vga_gotoxy(TAB0,pos_y);
   vga_command("f,2");
   strcpy_P(buffer,(PGM_P)pgm_read_word(&(ES_table[0])));
   vga_puts(buffer);
   vga_putint_right(outbuffer[9]/2);

   // Boiler O
   vga_gotoxy(TAB1,pos_y);
   vga_command("f,2");
   strcpy_P(buffer,(PGM_P)pgm_read_word(&(ES_table[4])));
   vga_puts(buffer);
   vga_putint_right(outbuffer[4]/2);

   // Pumpe
   vga_gotoxy(TAB2,pos_y);
   vga_command("f,2");
   strcpy_P(buffer,(PGM_P)pgm_read_word(&(ES_table[6])));
   vga_puts(buffer);
  
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
   vga_gotoxy(TAB0,pos_y);
   vga_command("f,2");
   strcpy_P(buffer,(PGM_P)pgm_read_word(&(ES_table[1])));
   vga_puts(buffer);
   vga_putint_right(outbuffer[9]/2);
   
   // Boiler M
   vga_gotoxy(TAB1,pos_y);
   vga_command("f,2");
   strcpy_P(buffer,(PGM_P)pgm_read_word(&(ES_table[3])));
   vga_puts(buffer);
   vga_putint_right(outbuffer[4]/2);

   // Elektro
   vga_gotoxy(TAB2,pos_y);
   vga_command("f,2");
   strcpy_P(buffer,(PGM_P)pgm_read_word(&(ES_table[7])));
   vga_puts(buffer);
   
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
   vga_gotoxy(TAB0,pos_y);
   vga_command("f,2");
   strcpy_P(buffer,(PGM_P)pgm_read_word(&(ES_table[5])));
   vga_puts(buffer);
   vga_putint_right(outbuffer[9]/2);
   
   // Boiler U
   vga_gotoxy(TAB1,pos_y);
   vga_command("f,2");
   strcpy_P(buffer,(PGM_P)pgm_read_word(&(ES_table[2])));
   vga_puts(buffer);
   vga_putint_right(outbuffer[4]/2);
   
   // Alarm
   vga_gotoxy(TAB2,pos_y);
   vga_command("f,2");
   strcpy_P(buffer,(PGM_P)pgm_read_word(&(ES_table[8])));
   vga_puts(buffer);
   
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
   vga_command("f,3");
   uint8_t pos_y = 0;
   vga_gotoxy(TAB0,pos_y);
   vga_command("f,3");

   //strcpy_P(buffer,pgm_read_word(&(Status[0])));
   //PSTR("Testnachricht")
   vga_puts("Status: ");
   pos_y++;
   // EE
   vga_gotoxy(TAB10,pos_y);
   vga_command("f,3");
   strcpy_P(buffer,(PGM_P)pgm_read_word(&(status_table[0])));
   vga_puts(buffer);
   // Read Fehler
   vga_gotoxy(TAB11,pos_y);
   vga_command("f,3");
   strcpy_P(buffer,(PGM_P)pgm_read_word(&(status_table[1])));
   vga_puts(buffer);
   int2hexstring(outbuffer[40], (char*)&data_string);
   vga_puts(data_string);

   // Write fehler
   vga_gotoxy(TAB12,pos_y);
   vga_command("f,3");
   strcpy_P(buffer,(PGM_P)pgm_read_word(&(status_table[2])));
   vga_puts(buffer);
   int2hexstring(outbuffer[41], (char*)&data_string);
   vga_puts(data_string);

     // Err Fehler
   vga_gotoxy(TAB13,pos_y);
   vga_command("f,3");
   strcpy_P(buffer,(PGM_P)pgm_read_word(&(status_table[3])));
   vga_puts(buffer);
   int2hexstring(outbuffer[42], (char*)&data_string);
   vga_puts(data_string);

  
   
   // Belegung
   pos_y++;
   vga_gotoxy(TAB10,pos_y);
   vga_command("f,3");
   strcpy_P(buffer,(PGM_P)pgm_read_word(&(status_table[4])));
   vga_puts(buffer);
   // Belegung Read
   vga_gotoxy(TAB11,pos_y);
   vga_command("f,3");
   strcpy_P(buffer,(PGM_P)pgm_read_word(&(status_table[1])));
   vga_puts(buffer);
   int2hexstring(outbuffer[43], (char*)&data_string);
   vga_puts(data_string);
   // Belegung Write
   vga_gotoxy(TAB12,pos_y);
   vga_command("f,3");
   strcpy_P(buffer,(PGM_P)pgm_read_word(&(status_table[2])));
   vga_puts(buffer);
   
   vga_puthex(outbuffer[44]);
   //int2hexstring(outbuffer[44], (char*)&data_string);
   //vga_puts(data_string);
   //vga_puts((char*)outbuffer[44]);


}



void setRaumData(void)
{
   setHeizung();
   setWerkstatt();
   setWoZi();
   setBuero();
   setLabor();
   setOG1();
   setOG2();
   setEstrich();
   
 }

void setDATATASK(void)
{
   char in_string[4];
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
   uint8_t i;
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
   
   newline();
   vga_puts("Daten vom  Master");
   vga_putch(' ');
   vga_puts("TASK: ");
   char out_string[4];
   int2hexstring(out_startdaten, (char*)&out_string);
   vga_puts(out_string);
   vga_putch(' ');
   newline();
   vga_puts("Startadresse lb: ");
   int2hexstring(out_lbdaten, (char*)&out_string);
   vga_puts(out_string);
   vga_putch(' ');
   vga_puts("hb: ");
   int2hexstring(out_hbdaten, (char*)&out_string);
   vga_puts(out_string);
   vga_putch(' ');
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
   
} // setDATATASK





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
   uint8_t incounter=0;
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
                  incounter++;
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
                  
                  
                  
                  
                  vga_command("f,5");
                  newline();
                  //if (!(in_startdaten == 0xC0))
                  {
                     newline();
                     vga_puts("Daten vom Webserver:");
                     vga_putch(' ');
                     
                     char stringbuffer[32]={};
                     char* getString(const char* str)
                     {
                        strcpy_P(stringbuffer, (char*)str);
                        return stringbuffer;
                     }

                     switch (in_startdaten)
                     {
                        case 0xC0:
                        {
                           char buffer[16]={};
                           //strcpy_P(buffer,getString(task_C0));
                           strcpy_P(buffer,(PGM_P)pgm_read_word(&(task_table_C[0])));
                           //strcpy_P(buffer, (const char*)pgm_read_word(&(task_C0)));
                           //strcpy_P(buffer,(PGM_P)pgm_read_word(&task_C0));
                           vga_puts(buffer);

                      //     vga_puts("DATATASK: ");

                        }break;
                           
                           
                     }// switch in_startdate
                     
                     
                     char in_string[4];
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
                     uint8_t i;
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
                           //if (i)
                           {
                              //vga_putch(' ');
                              //vga_putch(' ');
                           }
                        }
                        {
                           vga_putch(' ');
                           char data_string[4];
                           int2hexstring(inbuffer[i], (char*)&data_string);
                           vga_puts(data_string);
                           
                           
                        }
                     }
                  }
                  
                  //lcd_gotoxy(0,1);
                  //lcd_putc('*');
                  uint8_t i;
                  for (i=0;i<3;i++)
                  {
                     //lcd_puthex(inbuffer[i+32]);
                     //lcd_putc(' ');
                     
                  }
                  
                  newline();
                  vga_puts("Daten vom  Master");
                  vga_putch(' ');
                  vga_puts("TASK: ");
                  char out_string[4];
                  int2hexstring(out_startdaten, (char*)&out_string);
                  vga_puts(out_string);
                  vga_putch(' ');
                  newline();
                  vga_puts("Startadresse lb: ");
                  int2hexstring(out_lbdaten, (char*)&out_string);
                  vga_puts(out_string);
                  vga_putch(' ');
                  vga_puts("hb: ");
                  int2hexstring(out_hbdaten, (char*)&out_string);
                  vga_puts(out_string);
                  vga_putch(' ');
                  vga_putch(' ');
                  
                  for (i=0;i<SPI_BUFSIZE;i++)
                  {
                     if (i%16 ==0)
                     {
                        newline();
                        //if (i)
                        {
                           //vga_putch(' ');
                           //vga_putch(' ');
                        }
                        
                     }
                     {
                        vga_putch(' ');
                        char data_string[4];
                        int2hexstring(outbuffer[i], (char*)&data_string);
                        vga_puts(data_string);
                        
                        
                     }
                  }
                 
                  
                  // Ausgang Master
                  
                  //   Zeit im Titelbalken angeben
                  //uint8_t stunde = (outbuffer[0] & 0x1F); // Stunde, Bit 0-4
                  //uint8_t minute = (outbuffer[1] & 0x3F); // Minute, Bit 0-5
                  uint8_t stunde = (outbuffer[46] ); // Stunde, Bit 0-4
                  uint8_t minute = (outbuffer[47] ); // Minute, Bit 0-5
                  
                  // Tag ausgeben
                  
                  vga_command("f,1");
                  vga_gotoxy(85,0);
                  vga_command("f,1");
                  //vga_putint2(outbuffer[45]);
                  char buffer[4];
                  strcpy_P(buffer,(PGM_P)pgm_read_word(&(tag_table[outbuffer[45]-1])));
                  vga_puts(buffer);

                  // Zeit ausgeben
                  
                  vga_command("f,1");
                  vga_gotoxy(90,0);
                  vga_command("f,1");
                  vga_putint2(stunde);
                  vga_putch(':');
                  vga_putint2(minute);

                  vga_command("f,2");
                  vga_command("p,0,1");
                  vga_command("f,2");
               //   setHeizung(outbuffer[2]/2,outbuffer[3]/2,outbuffer[4],outbuffer[5]);
                  setRaumData();
                  
                  setStatusData();
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
               lcd_gotoxy(19,1);
               lcd_putc('f');
               
               //if (out_startdaten + in_enddaten==0xFF)
               {
                  lcd_gotoxy(19,1);
                  lcd_putc('+');
                  spistatus |= (1<<SUCCESS_BIT); // Bit fuer vollstaendige und korrekte  Uebertragung setzen
                  lcd_gotoxy(19,0);
                  lcd_putc(' ');
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
               
            }
            else
            {
               //lcd_gotoxy(18,3);
               //lcd_putc('e');
               
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
