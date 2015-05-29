


// UART
#define UART_PORT		PORTD
#define UART_PIN		PIND
#define UART_DDR		DDRD
//#define RTS_PIN      0
//#define CTS_PIN      1


// SR
#define SR_PORT               PORTB
#define SR_DDR                DDRB
#define SR_PIN                PINB
#define SR_CLK_PIN            4
#define SR_DATA_PIN				5
#define SR_LOAD_PIN				6

// TWI

//#define TWI_PORT		PORTC
//#define TWI_PIN		PINC
//#define TWI_DDR		DDRC

//#define SDAPIN       4
//#define SCLPIN       5
#define TWI_ERR_BIT				7


#define LOOPLED_PORT	PORTD
#define LOOPLED_DDR	DDRD
#define LOOPLED_PIN	2

// Tastatur
#define TASTE1          19
#define TASTE2          29
#define TASTE3          44
#define TASTE4          67
#define TASTE5          94
#define TASTE6          122
#define TASTE7          155
#define TASTE8          180
#define TASTE9          205
#define TASTEL          220
#define TASTE0          236
#define TASTER          248
#define TASTATURPORT    PORTC
#define TASTATURDDR     DDRC
#define TASTATURPIN		0
#define TASTATURKANAL		0



// Manuell
#define MANUELL_PORT		PORTD
#define MANUELL_DDR		DDRD
#define MANUELL_PIN		PIND

#define MANUELL			7	// Bit 7 von Status
#define MANUELLPIN		6	// Pin 6 von PORT D fuer Anzeige Manuell
#define MANUELLNEU		7	// Pin 7 von Status. Gesetzt wenn neue Schalterposition eingestellt
#define MANUELLTIMEOUT	100 // Loopled-counts bis Manuell zurueckgesetzt wird. 02FF: ca. 100 s

// These prescaler values are for high speed mode, ADHSM = 1 // von Teensy
#if F_CPU == 16000000L
#define ADC_PRESCALER ((1<<ADPS2) | (1<<ADPS1))
#elif F_CPU == 8000000L
#define ADC_PRESCALER ((1<<ADPS2) | (1<<ADPS0))
#elif F_CPU == 7372000L
#define ADC_PRESCALER ((1<<ADPS2) | (1<<ADPS0))

#elif F_CPU == 4000000L
#define ADC_PRESCALER ((1<<ADPS2))
#elif F_CPU == 2000000L
#define ADC_PRESCALER ((1<<ADPS1) | (1<<ADPS0))
#elif F_CPU == 1000000L
#define ADC_PRESCALER ((1<<ADPS1))
#else
#define ADC_PRESCALER ((1<<ADPS0))
#endif





// Strings
// http://www.atmel.com/webdoc/AVRLibcReferenceManual/pgmspace_1pgmspace_strings.html



//
// Wochentag
const char tag_0[] PROGMEM = "MO";
const char tag_1[] PROGMEM = "DI";
const char tag_2[] PROGMEM = "MI";
const char tag_3[] PROGMEM = "DO";
const char tag_4[] PROGMEM = "FR";
const char tag_5[] PROGMEM = "SA";
const char tag_6[] PROGMEM = "SO";

PGM_P const tag_table[] PROGMEM =
{
   tag_0,
   tag_1,
   tag_2,
   tag_3,
   tag_4,
   tag_5,
   tag_6
   
   
};


const char string_0[] PROGMEM = "HEIZUNG";
const char string_1[] PROGMEM = "WERKSTATT";
const char string_2[] PROGMEM = "WOZI";
const char string_3[] PROGMEM = "BUERO";
const char string_4[] PROGMEM = "LABOR";
const char string_5[] PROGMEM = "OG 1";
const char string_6[] PROGMEM = "OG 2";
const char string_7[] PROGMEM = "ESTRICH";

PGM_P const raum_table[] PROGMEM =
{
   string_0,
   string_1,
   string_2,
   string_3,
   string_4,
   string_5,
   string_6,
   string_7

};
const char heizung_0[] PROGMEM = "Vorlauf  :";
const char heizung_1[] PROGMEM = "Ruecklauf:";
const char heizung_2[] PROGMEM = "Aussen   :";
const char heizung_3[] PROGMEM = "Code:";
const char heizung_4[] PROGMEM = "Brenner:";
const char heizung_5[] PROGMEM = "Pos :";
const char heizung_6[] PROGMEM = "Rinne  :";


PGM_P const heizung_table[] PROGMEM =
{
   heizung_0,
   heizung_1,
   heizung_2,
   heizung_3,
   heizung_4,
   heizung_5,
   heizung_6


};


const char pos_0[] PROGMEM = "OFF";
const char pos_1[] PROGMEM = "F/-";
const char pos_2[] PROGMEM = "F/R";
const char pos_3[] PROGMEM = "R/R";
const char pos_4[] PROGMEM = "V/V";
const char pos_5[] PROGMEM = "Hnd";

PGM_P const pos_table[] PROGMEM =
{
   pos_0,
   pos_1,
   pos_2,
   pos_3,
   pos_4,
   pos_5
   
};

const char WS_0[] PROGMEM = "Lampe:";
const char WS_1[] PROGMEM = "Ofen :";
const char WS_2[] PROGMEM = "Temp :";
const char WS_3[] PROGMEM = "ABC  :";
const char WS_4[] PROGMEM = "A. W :";
const char WS_5[] PROGMEM = "A.TK :";

PGM_P const WS_table[] PROGMEM =
{
   WS_0,
   WS_1,
   WS_2,
   WS_3,
   WS_4,
   WS_5
};

const char RAUM_0[] PROGMEM = "Lampe:";
const char RAUM_1[] PROGMEM = "Innen:";
const char RAUM_2[] PROGMEM = "Rad  :";
const char RAUM_3[] PROGMEM = "ABC  :";

PGM_P const RAUM_table[] PROGMEM =
{
   RAUM_0,
   RAUM_1,
   RAUM_2,
   RAUM_3
};



const char ES_0[] PROGMEM = "Vorlauf  :";
const char ES_1[] PROGMEM = "Ruecklauf:";
const char ES_2[] PROGMEM = "Boiler U:";
const char ES_3[] PROGMEM = "Boiler M:";
const char ES_4[] PROGMEM = "Boiler O:";
const char ES_5[] PROGMEM = "Kollektor:";
const char ES_6[] PROGMEM = "Pumpe  :";
const char ES_7[] PROGMEM = "Elektro:";
const char ES_8[] PROGMEM = "Alarm  :";

PGM_P const ES_table[] PROGMEM =
{
   ES_0,
   ES_1,
   ES_2,
   ES_3,
   ES_4,
   ES_5,
   ES_6,
   ES_7,
   ES_8
};

// Status
const char Status[] PROGMEM = "Status";

const char St_0[] PROGMEM = "EE  : ";
const char St_1[] PROGMEM = "Read :";
const char St_2[] PROGMEM = "Write:";
const char St_3[] PROGMEM = "EEProm:";
const char St_4[] PROGMEM = "Raum:";
const char St_5[] PROGMEM = "R: ";
const char St_6[] PROGMEM = "W: ";

PGM_P const status_table[] PROGMEM =
{
   St_0,
   St_1,
   St_2,
   St_3,
   St_4,
   St_5,
   St_6,

};

// Error
const char Err_0[] PROGMEM = "TWI:";
const char Err_1[] PROGMEM = "call :";
const char Err_2[] PROGMEM = "reply:";
const char Err_3[] PROGMEM = "count: ";


const char Err_4[] PROGMEM = "SPI: ";
const char Err_5[] PROGMEM = "o_st :";
const char Err_6[] PROGMEM = "i_end:";
const char Err_7[] PROGMEM = "b_cnt:";


PGM_P const error_table[] PROGMEM =
{
   Err_0,
   Err_1,
   Err_2,
   Err_3,
   Err_4,
   Err_5,
   Err_6,
   Err_7

};

// TASK
const char task_A0[] PROGMEM = "ERR";

const char task_B0[] PROGMEM = "NULL ";
const char task_B1[] PROGMEM = "STATUS ";
const char task_B2[] PROGMEM = "STATUSCONFIRM ";
const char task_B3[] PROGMEM = "B3 ";
const char task_B4[] PROGMEM = "EEPROMREPORT ";
const char task_B5[] PROGMEM = "EEPROMCONFIRM ";
const char task_B6[] PROGMEM = "EEPROMRECEIVE ";
const char task_B7[] PROGMEM = "EEPROMWRITE ";
const char task_B8[] PROGMEM = "EEPROMREAD ";
const char task_B9[] PROGMEM = "EEPROMSEND ";
const char task_BA[] PROGMEM = "EE_READWOCHEA ";
const char task_BB[] PROGMEM = "EE_READWOCHEB ";
const char task_BC[] PROGMEM = "EEPROMREADPWM ";
const char task_BD[] PROGMEM = "BE ";
const char task_BE[] PROGMEM = "BE ";
const char task_BF[] PROGMEM = "RESET ";


const char task_C0[] PROGMEM = "DATA ";
const char task_C1[] PROGMEM = "SOLAR ";
const char task_C2[] PROGMEM = "C2 ";
const char task_C7[] PROGMEM = "MASTERERR ";

PGM_P const task_table_B[] PROGMEM =
{
   task_B0,
   task_B1,
   task_B2,
   task_B3,
   task_B4,
   task_B5,
   task_B6,
   task_B7,
   task_B8,
   task_B9,
   task_BA,
   task_BB,
   task_BC,
   task_BD,
   task_BE,
   task_BF
};
   
PGM_P const task_table_C[] PROGMEM =
{
   task_C0,
   task_C1,
   task_C2,
   task_C7,
};
   
// OSZI

 #define OSZIPORT     PORTD
 #define OSZIDDR      DDRD
 #define OSZIA        0
 #define OSZIB        0


#define OSZIA_HI  OSZIPORT |= (1<< OSZIA)
#define OSZIA_LO  OSZIPORT &= ~(1<< OSZIA)
#define OSZIB_HI  OSZIPORT |= (1<< OSZIB)
#define OSZIB_LO  OSZIPORT &= ~(1<< OSZIB)

uint16_t loopCount0=0;
uint16_t loopCount1=0;
uint16_t loopCount2=0;



volatile uint8_t					Programmstatus=0x00;
uint8_t Tastenwert=0;
volatile uint16_t TastaturCount=0;
uint8_t Tastaturabfrage=0;
volatile uint8_t tastaturstatus=0;

uint8_t Taste = 0;
volatile uint16_t		Manuellcounter=0; // Counter fuer Timeout
uint16_t TastenStatus=0;
uint16_t Tastencount=0;
uint16_t Tastenprellen=0x01F;

volatile uint8_t uartstatus=0;

// Array fuer Daten von Slaves (4 bytes pro Slave
#define SLAVESTATUSRAUM 18 // Raumnummer
#define SLAVESTATUSPOS  19 // Beginn Raumdaten: 4 bytes
volatile uint8_t slavestatus[8][4];
uint16_t startcounter=0x00;

#define CURSORX   2
#define CURSORY   2
#define MAXMENUEBENE 2

#define HOMESCREEN   0
#define RAUMSCREEN   1
uint8_t curr_screen=0;
uint8_t cursorx=0,cursory=0;
uint8_t menuebene=0;
