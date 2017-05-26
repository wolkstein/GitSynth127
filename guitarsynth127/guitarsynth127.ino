/*
   GitSynth by Michael Wolkstein copyright 2017


   noten 1*1 http://www.sengpielaudio.com/Rechner-notennamen.htm

   Temperierte Stimmung                              32 bit float 6-8 digits
   note um einen halbton erhöhen ist notenfrequenz x 1,059463 0943592952645618252949463
   note um einen halbton niedriger ist notenfrequenz x 0,943874 31268169349664191315666757

   100 cent = 1 halbtonschritt
   1 cent hoch   1.000577 7895
   1 cent runter 0,999422 5441

   4.16666 cent = 100/24 cent schritte = 1.00241

   wir unterteilen in
   oktave +- 12(24) halbtonschritte +- 12(24) 4.16666 cent schritten

   entspricht im ausdruck
                  |   oktaven erhöhen                   |+| halbtonschritte
   neue frequenz = frequenz x ((oktavenZumErhöhen x 2)/1) + halbtonschritte + cents

  in usb_desc.h dies ändern
  #define MANUFACTURER_NAME  {'T','e','e','n','s','y','d','u','i','n','o'}
  #define MANUFACTURER_NAME_LEN 11
  //  #define PRODUCT_NAME    {'T','e','e','n','s','y',' ','M','I','D','I','/','A','u','d','i','o'}
  //  #define PRODUCT_NAME_LEN  17
  #define PRODUCT_NAME    {'G','I','T','S','y','n','t','h','1','2','7'}
  #define PRODUCT_NAME_LEN  11


*/

// wichtig
// meine anderungen bei analyze_notefreq.h
// die default werte für AUDIO_GUITARTUNER_BLOCKS 24
// wir wollen aber nur 9 um schneller zu sein
// also nun in analyze_notefreq.h #define AUDIO_GUITARTUNER_BLOCKS  9 eintragen

// analyze_peak.h zusatz
/*
  float readFull(void) {
    __disable_irq();
    int min = min_sample;
    int max = max_sample;
    min_sample = 32767;
    max_sample = -32768;
    __enable_irq();

    if (min > max) max = min;
    return (float)max / 32767.0f;
  }
*/


// debugging
#define DEB_PROCESSOR_AND_TIME false
#define DEB_GIUTAR_ATTACK_DETECTION false
#define DEB_PITCH_DETECTION false
#define DEB_MENUE false
#define DEB_PEAKS false
#define DEB_ENVELOPE_FOLLOWER false
#define DEBUG_MIDI_INPUT true
#define DEBUG_MIDI_REQUEST_SETTINGS false
#define DEB_MIDI_NOTE_ENVELOPE false
#define DEB_MIDIGUITAR_NOTE_ENVELOPE false
#define DEB_FILTER_NOTE_ENVELOPE false
#define DEB_SD false
#define DEB_LFO false
#define DEB_TAPTEMPO false
#define DEBUG_EXPRESSION_PEDAL false
#define DEBUG_EXPRESSION_PEDAL_RAW false
#define DEB_SEQUENZER false
#define DEB_ANALOG_READ false
#define DEB_FUNCTION_BTN false
#define DEB_TOGGLE_SWITCH false
#define DEB_EXTRA_PITCH false
#define DEB_EEPROM false

#include <SerialFlash.h>
#include <Audio.h>
#include <Wire.h>
//#include <SPI.h>
#include <SD.h>
//#include <EDB.h>
#include "EDB.h"
#include <LiquidCrystalFast.h>
#include <Bounce.h>
#include <EEPROM.h>
#include "AudioSystemDesign.h"
#include "synth_settings.h"
#include "AKWF_SOUND_FONTS.h"
#include "lfo_shapes.h"


// new v3 notefreq coefficients
int16_t fir_44117_HZ_coefficients[22] =
{
    0, 3, 6, -11, -71, 21,
    352, -15, -1202, -6, 5011, 8209,
    5011, -6, -1202, -15, 352, 21,
    -71, -11, 6, 3
};

int16_t fir_22059_HZ_coefficients[20] =
{
    0, 1, -6, -54, 18, 326,
    -14, -1178, -6, 5001, 8209, 5001,
    -6, -1178, -14, 326, 18, -54,
    -6, 1
};
// end coefficients

bool ToggleSwitchIsToggled = false;
bool FbtIsPressed = false;
bool FrozenNote = false;

// ######## Digital poti #############
// cs = 29, SCK = 14, SI(MOSI) = 7, SHDN + RS sind high
//#define MCP42XXXX_CS 29
//#define MCP42XXXX_MOSI_PIN  7
//#define MCP42XXXX_SCK_PIN   14

//###############################################
//Wave table OSC1
struct myOSCWaveTable {
  int16_t const* table;
};

#define OSC_TABLE_NUM 10 // 

myOSCWaveTable akwfOSCWaveTables[OSC_TABLE_NUM] {
  AKWF_bitreduced_0001,
  AKWF_bitreduced_0002,
  AKWF_hvoice_0003,
  AKWF_hvoice_0004,
  AKWF_theremin_0005,
  AKWF_theremin_0006,
  AKWF_theremin_0007,
  AKWF_theremin_0008,
  AKWF_eorgan_0001,
  AKWF_eorgan_0021
};


void selectABTWaveTableOSC1( uint16_t table) {
  const struct myOSCWaveTable* waveTable  = &akwfOSCWaveTables[table];
  int16_t* wave = const_cast<short int*>(waveTable->table);
  Osc1.begin(0.5, 440, WAVEFORM_ARBITRARY);
  Osc1.arbitraryWaveform(wave, ABT_MAX_FREQ);
}
//###############################################

//###############################################
//Wave table LFO
struct myLFOWaveTable {
  int16_t const* table;
};

#define LFO_TABLE_NUM 6 // 

myLFOWaveTable myLFOWaveTables[LFO_TABLE_NUM] {
  ramp_up,
  ramp_down,
  step_up,
  step_down,
  rutsch_hoch,
  rutsch_runter
};


void selectABTWaveTableFilterLFO(float hz, uint16_t table) {
  const struct myLFOWaveTable* waveTable  = &myLFOWaveTables[table];
  int16_t* wave = const_cast<short int*>(waveTable->table);
  Filter_LFO.begin(1.0, hz * beatMultiLookup[mySettings.filter_lfo_freq_bpm_multi] , WAVEFORM_ARBITRARY, mySettings.filter_lfo_cycles);
  Filter_LFO.arbitraryWaveform(wave, ABT_MAX_FREQ);
}

void selectABTWaveTableVolumenLFO(float hz, uint16_t table) {
  const struct myLFOWaveTable* waveTable  = &myLFOWaveTables[table];
  int16_t* wave = const_cast<short int*>(waveTable->table);
  VOLUMEN_LFO.begin(0.5, hz * beatMultiLookup[mySettings.vol_lfo_freq_bpm_multi], WAVEFORM_ARBITRARY, mySettings.vol_lfo_cycles);
  VOLUMEN_LFO.arbitraryWaveform(wave, ABT_MAX_FREQ);
}

void selectABTWaveTableModLFO(float hz, uint16_t table) {
  const struct myLFOWaveTable* waveTable  = &myLFOWaveTables[table];
  int16_t* wave = const_cast<short int*>(waveTable->table);
  MOD_LFO.begin(0.5, hz * beatMultiLookup[mySettings.mod_lfo_freq_bpm_multi] , WAVEFORM_ARBITRARY, mySettings.mod_lfo_cycles);
  MOD_LFO.arbitraryWaveform(wave, ABT_MAX_FREQ);
}

//###############################################




//-----------------------SD EDB--------------------------------------------------------------
const int chipSelect = BUILTIN_SDCARD;
const char* db_name = "/PRESETS/preset.db"; // preset.db
File dbFile;

void mywriter (unsigned long address, byte data) {
  //digitalWrite(13, HIGH);
  dbFile.seek(address);
  dbFile.write(data);
  dbFile.flush();
  //digitalWrite(13, LOW);
}

byte myreader (unsigned long address) {
  //digitalWrite(13, HIGH);
  dbFile.seek(address);
  byte b = dbFile.read();
  //digitalWrite(13, LOW);
  return b;
}

EDB db(&mywriter, &myreader);

// -------------------------- Buttond

Bounce incrementButton = Bounce(1, 50);  // increment valu
Bounce decrementButton = Bounce(0, 50);  // decrement value
Bounce menuButtonUp = Bounce(3, 50);  // Menu Button
Bounce menuButtonDown = Bounce(2, 50);  // Menu Button
Bounce noteOnOffandOKButton = Bounce(4, 20);  //
Bounce tapTempoBtn = Bounce(5, 100); //tap tempo

Bounce PresetUpBtn = Bounce(28,50);
Bounce PresetDownBtn = Bounce(27,50);

Bounce ToggleSwitchFunction = Bounce(29, 50);
Bounce FunctioSwitchFunction = Bounce(30, 50);

// button test
int incrementButtoncounter = 0;
byte previousState0 = HIGH;         // what state was the button last time
unsigned long countAt0 = 0;         // when count changed


byte previousState1 = HIGH;         // what state was the button last time
unsigned long countAt1 = 0;         // when count changed
// button test ende

byte previousState2 = HIGH;         // what state was the button last time
unsigned long countAt2 = 0;         // when count changed
bool longPressFinishedBtn2 = true;  //

byte previousState3 = HIGH;         // what state was the button last time
unsigned long countAt3 = 0;         // when count changed
bool taptempoOnGuitarOrKeyboard = false;

bool menuExtrButton = false;
uint8_t menuExtrButtonPos = 0;
int8_t menuExtra_I_Pos = 0;

//LiquidCrystalFast lcd(RS, RW, Enable, D4, D5, D6, D7)
LiquidCrystalFast lcd(39, 38, 37, 36, 35, 34, 33);


#define CLIPLEDPIN 24
#define TAPTEMPOLEDPIN 25
#define TRACKINGLEDPIN 26

#define ExPedalRead 32
#define PotiRead 31

uint16_t ExPedalValue = 0;
uint16_t oldExPedalValue = 0;
uint16_t PotiValue = 0;
uint16_t oldPotiValue = 0;

// adsr states 0=finished, 1 attack, 2 hold, 3 decay 4 sustain, 5 release 6 in suistain 7 in release
struct MidiNote
{
  int             adsrState;
  unsigned long           attackTime;
  unsigned long             holdTime;
  unsigned long            decayTime;
  unsigned long          releaseTime;
  float        midiVelocety;
  float            midiNote;
  float         oldmidiNote;
  int             midiPitch;
};
MidiNote myMidiNote = { 0, 0, 0, 0, 0, 0.0, 0.0f, 0.0f, 0};

struct GuitarNote
{
  int             adsrState;
  unsigned long           attackTime;
  unsigned long             holdTime;
  unsigned long            decayTime;
  unsigned long          releaseTime;
  float        midiVelocety;
  float            midiNote;
  float         oldmidiNote;
  int             midiPitch;
};
GuitarNote myGuitarNote = { 0, 0, 0, 0, 0, 0.0, 0.0f, 0.0f, 0};

struct FilterNote
{
  int             adsrState;
  unsigned long           delayTime;
  unsigned long           attackTime;
  unsigned long             holdTime;
  unsigned long            decayTime;
  unsigned long          releaseTime;
  float               filterVelocety;
  bool                      isGuitar;
};
FilterNote myFilterNote = { 0, 0, 0, 0, 0, 0, 0.0f, false};

struct LedStatus
{
  unsigned long CLIP_LED_MILLIS_ON;
  uint16_t CLIP_LED_MILLIS;
  unsigned long TAPTEMPO_LED_MILLIS_ON;
  uint16_t TAPTEMPO_LED_MILLIS;
  unsigned long TRACKING_LED_MILLIS_ON;
  uint16_t TRACKING_LED_MILLIS;
};
LedStatus myLedStatus = {0, 200, 0, 200, 0, 200};

bool key_input = false;

float peak = 0.0f;
float note;
float noteForFrozenNote;
float prob;
float highPeak = 0.0f;


// zeitmessung und prozessormessung
unsigned long old_MicroTime = 0;
int timecycles = 0;
unsigned long longestRound = 0;
unsigned long shortestround = 1000000;
unsigned long oldlongshortVal = 0;

uint8_t current_preset = 1;
const int myInputLineIn = AUDIO_INPUT_LINEIN;

// midi clock vars use in midi and bottom of this file void checksystemonmidiclock()
bool systemOnMidiClock = false;
uint8_t MidiTimeClockCount = 1;
unsigned long MidioldTimeClockMilli = 0;
float MidismothedDiffVal = 0.0f;
float MidismothedDiffValFiltered = 0.0f;
float MidismothedDiffValFilteredSummed = 0.0f;
float MidiCool = 0.0f;
float MidiBpm = 90.0f;
unsigned long  MiditimeClockTime = 0;

bool GuiaskForPreset = false;
bool getSequencerSequence = false;

unsigned long askForPresetmillis = 0; // werden zu anfang in Midi_Request_settings gesetzt
unsigned long getSequencerSequenceMillis = 0;

uint16_t ExPedalValueFiltered =0;
uint16_t PotiValueFiltered =0;



void setup() {

  // ############ DIGITAL POTI #############
//  pinMode (MCP42XXXX_CS, OUTPUT);
//  digitalWrite(MCP42XXXX_CS, HIGH);
//  SPI.setMOSI(MCP42XXXX_MOSI_PIN);
//  SPI.setSCK(MCP42XXXX_SCK_PIN);
//  SPI.begin();

  // POTI TEST
//  digitalWrite(MCP42XXXX_CS, LOW);
//  SPI.transfer(B00010001);     // B00010001 für poti 0 on MCP42XXXX
//  SPI.transfer(125);  //  0-255 value
//  digitalWrite(MCP42XXXX_CS, HIGH);

  //############# Buttons ##################
  pinMode(0, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(27, INPUT_PULLUP);
  pinMode(28, INPUT_PULLUP);
  pinMode(29, INPUT_PULLUP); 
  pinMode(30, INPUT_PULLUP);
   
  // ############ LED S #####################
  pinMode(CLIPLEDPIN, OUTPUT);
  digitalWrite(CLIPLEDPIN, LOW);
  pinMode(TAPTEMPOLEDPIN, OUTPUT);
  digitalWrite(TAPTEMPOLEDPIN, LOW);
  pinMode(TRACKINGLEDPIN, OUTPUT);
  digitalWrite(TRACKINGLEDPIN, LOW);

  //############# Audio Settings #############
  AudioMemory(200);
  sgtl5000_1.enable();
  sgtl5000_1.inputSelect(myInputLineIn);

  sgtl5000_1.adcHighPassFilterDisable();// gut eleminiert input rauschen!!



  sgtl5000_1.audioProcessorDisable();
  //sgtl5000_1.audioPostProcessorEnable();//test für lowpassfilter 
  //sgtl5000_1.enhanceBassEnable(); //!! use as lowpassfilter
  
  sgtl5000_1.volume(0.7); // am ende des setups die lautstärke auf level setzen
  //sgtl5000_1.lineInLevel(10 , 10);//( left, right) 5 default

  sgtl5000_1.lineOutLevel(13);// 29 default rauscht sehr stark auf 13

  //############## LCD ##########################
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Init System");

      // get exp pedal position
  unsigned long potimillis = millis();
  while(millis() < potimillis + 1000){ 
    ExPedalValueFiltered =  (float(analogRead(ExPedalRead)) * (1.0 - 0.8)) + (float(ExPedalValueFiltered) *  0.8);
    PotiValueFiltered = (float(analogRead(PotiRead)) * (1.0 - 0.99)) + (float(PotiValueFiltered) *  0.99);
  }

  //eeprom test, init and migration if revision change
    if(DEB_EEPROM){
      Serial.println("EEpromTest");
      Serial.println(sizeof(myMainSystemInfo)); // get size of struct  sizeof(mySettings);
      Serial.println(sizeof(mySystemSettings)); // get size of struct  sizeof(mySettings);
      Serial.println(EEPROM.length());
    }

    EEPROM.get( EEPROM_MAIN_SYSTEM_INFO_START, myMainSystemInfo);
    if(myMainSystemInfo.initeeprom != 7777){
      if(DEB_EEPROM) Serial.println("init eeprom first time");
      myMainSystemInfo.revision = MainSystemRevision;
      myMainSystemInfo.initeeprom = 7777;
      EEPROM.put(EEPROM_MAIN_SYSTEM_INFO_START, myMainSystemInfo);
      EEPROM.put(EEPROM_MAIN_SYSTEM_SETTINGS_START, mySystemSettings);
    }
    if(myMainSystemInfo.revision != MainSystemRevision){
      if(DEB_EEPROM) Serial.println("need eeprom update, eeprom rev has changed");
      // here code to migrate the System settings if needed
      
    }
    
    EEPROM.get( EEPROM_MAIN_SYSTEM_SETTINGS_START, mySystemSettings);
    if(DEB_EEPROM){
      Serial.printf("EEprom revision: %d\n", myMainSystemInfo.revision);
      Serial.printf("System Settings ExpMax: %d\n", mySystemSettings.Expression_Max);
      Serial.printf("System Settings ExpMin: %d\n", mySystemSettings.Expression_Min);
    }
  



  // ###### MIDI Receiving Messages
  initMidiReceiving();
  // end ###### MIDI Receiving Messages

  // ####### set all "mySettings" audio settings
  init_sd_edb();

  //clear and create new table
  //edb_deleteAll();  // only one time for an new sd card or changing data structure
  //edb_createRecords(127); // only one time for an new sd card

  edb_selectPreset(1);

  setSettings(true);
  calculateFilterMixer();

  //################ Misk #######################
  selectMenuItemAndPrintLcd(0);
  // Audio library isr allways gets priority

  myAttackDetector.set(mySettings.sensingTreshold, mySettings.sensingFall, mySettings.sensingBounce, mypeakcallback);
  myAttackDetector.setCallbackActive(true);
  
  delay(100);
  askForPresetmillis = millis(); // werden zu anfang in Midi_Request_settings gesetzt
  
}


// L-in, OSCM, F-Out
bool anyFilterPeak[3] = { false, false, false};
bool thePeak = false;
float filterCtrPeak = 0.0f;
float lineInRMSVal = 0.0f;
float allOscsRMSVal = 0.0f;
bool peakDetectAndPrintLcd[2] = {false, false};
unsigned long bouncepeakDetectAndPrintLcd = 0;
unsigned long oldmilliTime = 0;

unsigned long old_MidinoteUpdateTime = 0;

float valueFCMAnalyse = 0.0f;
float valueFilterAttackAnalyse = 0.0f;
float valueFOLLOW_VOL_LFO = 0.0f;
float valueFOLLOW_VOL_LFO_invertOSC1 = 0.0f;
float valueFOLLOW_VOL_LFO_invertOSC2 = 0.0f;
float valueFOLLOW_VOL_LFO_invertSUBS = 0.0f;

float offsetInverterLVOvolOSC1 = 1.0f;
float offsetInverterLVOvolOSC2 = 1.0f;
float offsetInverterLVOvolSUBS = 1.0f;

float valueFOLLOW_MOD_LFO_invertOSC1 = 0.0f;
float valueFOLLOW_MOD_LFO_invertOSC2 = 0.0f;
float valueFOLLOW_MOD_LFO_invertSUB1 = 0.0f;
float valueFOLLOW_MOD_LFO_invertSUB2 = 0.0f;

float offsetInverterLVOmodOSC1 = 1.0f;
float offsetInverterLVOmodOSC2 = 1.0f;
float offsetInverterLVOmodSUB1 = 1.0f;
float offsetInverterLVOmodSUB2 = 1.0f;

bool PULSE_TRANSITION_OSC1 = true;
bool PULSE_TRANSITION_OSC2 = true;
bool PULSE_TRANSITION_SUB1 = true;
bool PULSE_TRANSITION_SUB2 = true;

unsigned long pupskraft = 0;

float MOD_lfo_OSC1_extrapitch = 1.0f;
float MOD_lfo_OSC2_extrapitch = 1.0f;
float MOD_lfo_SUB1_extrapitch = 1.0f;
float MOD_lfo_SUB2_extrapitch = 1.0f;

unsigned long masteclockMicros = 0;
unsigned long taptempoOld = 0;
unsigned long taptempoIncrement = 0;
double mySystemBpm = 90.0;
float mySystemFrequenzy = mySystemBpm / 60.0;
unsigned long mysystemFreqMicrosTime = 666667;
unsigned long sequencerSpeedTime = int(mysystemFreqMicrosTime / beatMultiLookup[mySettings.sequenceTimeSignature]);
unsigned long masterclockLEDBlinkerOldTime = 0;
unsigned long expressionpedaloldtime = 0;
unsigned long potioldtime = 0;

int myTapCounter = 0;

unsigned long oldShowBpmmillis = 0;
bool starblink = true;


float EnvelopLongSustainVelecety = 0.0;
float FilterLongSustainVelecety = 0.0;
bool reachEnvelopLongSustainVelecetyLevel = false;
bool reachFilterLongSustainVelecetyLevel = false;




void loop() {

  // zeit zu beginn des loops in millis
  unsigned long milli = millis();// time from current loop
  
  checkFunctionBtnStatus(milli);

  if (GuiaskForPreset && milli > askForPresetmillis + 1500) {
    if (DEBUG_MIDI_REQUEST_SETTINGS) Serial.printf("midi cc in released -------------------\n", GuiaskForPreset);
    GuiaskForPreset = false;
  }

  usbMIDI.read();

  // Read expressionpedal and poti

  if (mySettings.freeDataInt2 > 0 && milli > potioldtime + 20){// == nur für debug zwecke
    PotiValue = analogRead(PotiRead);
    
    PotiValueFiltered = (float(PotiValue) * (1.0 - 0.8)) + (float(PotiValueFiltered) *  0.8);
    
    if(myMenuWindow == 130){
      if(PotiValueFiltered != oldPotiValue){
        if(DEB_ANALOG_READ) Serial.printf("Poti: %d, filter: %d\n", PotiValue, PotiValueFiltered);
        if(ToggleSwitchIsToggled) setAnalogPoti(PotiValue);
        oldPotiValue = PotiValueFiltered;      
      }
    }
    else
    {
      if(PotiValueFiltered != oldPotiValue){
        setAnalogPoti(PotiValue);
        oldPotiValue = PotiValueFiltered;
      }
    }

    potioldtime = milli;
  }

  if (mySettings.expressionPedalFunction > 0 && milli > expressionpedaloldtime + 20){
    ExPedalValue = analogRead(ExPedalRead);

    ExPedalValueFiltered = (float(ExPedalValue) * (1.0 - 0.8)) + (float(ExPedalValueFiltered) *  0.8);

    
    if(myMenuWindow == 127){
      if(ExPedalValueFiltered != oldExPedalValue){
        if(DEB_ANALOG_READ) Serial.printf("EXP-PEDAL: %d, filter: %d\n", ExPedalValue, ExPedalValueFiltered);
        if(ToggleSwitchIsToggled) setExpressionPedal(ExPedalValue);
        oldExPedalValue = ExPedalValueFiltered;      
      }
    }
    else
    {
      if(ExPedalValueFiltered != oldExPedalValue){
        setExpressionPedal(ExPedalValue);
        oldExPedalValue = ExPedalValueFiltered;
      }
    }
    expressionpedaloldtime = milli;
  }
  

  //Serial.printf("Type : %d, Data1: %d, Data2: %d, SYS EX: %d\n",usbMIDI.getType(),usbMIDI.getData1(),usbMIDI.getData1(),usbMIDI.getSysExArray());

  updateMidiNote(milli);
  updateFilterNote(milli);
  updateGuitarNote(milli);

  // test masterclock war so, zu ungenau
  //if(myMasterPeak.available()){
  //Serial.println(myMasterPeak.readFull());
  //}


  // get input peak from Mixer-Analys Output connection
  if (rmsMixerAnalyseOut.available()) {
    peak = rmsMixerAnalyseOut.read();
  }
  if (lineinRms.available()) {
    lineInRMSVal = lineinRms.read();
    if (lineInRMSVal > 0.95) anyFilterPeak[0] = true;
    else anyFilterPeak[0] = false;
  }
  if (peakFilter1Out.available()) {
    filterCtrPeak = peakFilter1Out.read();
    if (filterCtrPeak > 0.99) {
      anyFilterPeak[2] = true;

      if (mySettings.autoPeakCorrection) {
        for (int i = 0; i < 4; i++) {
          if (mySettings.SYNTH_Master_Volume > 0.01 ) mySettings.SYNTH_Master_Volume -= 0.005;
          OscMainMix.gain(i, mySettings.SYNTH_Master_Volume);//0.1 Ausgang zum filter
        }
      }

    }
    else anyFilterPeak[2] = false;
  }

  //###############  lcd show peak and Limiter function
  if (anyFilterPeak[0] || anyFilterPeak[1] || anyFilterPeak[2] ) { //lcd.setCursor(0, 0); //0.58 0.7
    peakDetectAndPrintLcd[0] = true;
  }
  else {
    peakDetectAndPrintLcd[0] = false;
  }

  if (peakDetectAndPrintLcd[0]) {
    myLedStatus.CLIP_LED_MILLIS_ON = milli; // blink red led

    if (!peakDetectAndPrintLcd[1] && milli > bouncepeakDetectAndPrintLcd + 600) {
      peakDetectAndPrintLcd[1] = true;
      //Serial.println("lcd an");
      bouncepeakDetectAndPrintLcd = milli;
      if (anyFilterPeak[0]) {
        lcd.setCursor(0, 0);
        lcd.printf("!LIP!: %4.2f\n", lineInRMSVal);
      }
      if (anyFilterPeak[1]) {
        lcd.setCursor(0, 0);
        lcd.printf("!OMP!: %f4.2\n", allOscsRMSVal);
      }
      if (anyFilterPeak[2]) {
        lcd.setCursor(0, 0);
        lcd.printf("!PFP!: %4.2f\n", filterCtrPeak);
      }
    }
  }
  if (!peakDetectAndPrintLcd[0]) {
    if (peakDetectAndPrintLcd[1] && milli > bouncepeakDetectAndPrintLcd + 2000) {
      peakDetectAndPrintLcd[1] = false;
      //Serial.println("lcd aus");
      selectMenuItemAndPrintLcd(0);
      bouncepeakDetectAndPrintLcd = milli;
    }
  }

  if (DEB_PEAKS) {
    Serial.printf("I2S2 Links: %f  || MIX ANALYSE %f || POST OSC MIXER %f || FILTER PEAK %f ||| Limiter is set: %d Master is: %f\n", lineInRMSVal, peak, allOscsRMSVal, filterCtrPeak, mySettings.autoPeakCorrection, mySettings.SYNTH_Master_Volume);
  }


  //###############################  LFO frequenzcontroll ##################################

  if (FCMAnalyse.available()) {

    valueFCMAnalyse = FCMAnalyse.read();
    float mybpmToHz = bpmToHz(mySettings.filter_lfo_bpm);
    if (mySettings.MasterClockToLfos || mySettings.MasterclockToFreqLFO) mybpmToHz = bpmToHz(mySystemBpm);
    float  mybeatMultiLookup = beatMultiLookup[mySettings.filter_lfo_freq_bpm_multi];
    Filter_LFO.frequency((mybpmToHz * mybeatMultiLookup) + (valueFCMAnalyse * mySettings.envelopeFollowerAttackToFilterLFOFrequenzy * mybpmToHz * mybeatMultiLookup * 3));
  }

  if (filterAttackAnalyse.available()) {
    valueFilterAttackAnalyse = filterAttackAnalyse.read();

    // filter lfo attack envelope to filter lfo depth
    float envelope_mix = 1.0f;

    // check mySettings.filter_lfo_intensety ~ 0.0
    float local_filter_lfo_intensety = 0.0f;
    if (mySettings.filter_lfo_intensety > 0.0095 || mySettings.filter_lfo_intensety < -0.0095) {
      local_filter_lfo_intensety = mySettings.filter_lfo_intensety;
    }
    else
    {
      local_filter_lfo_intensety = 0.0;
    }

    if (mySettings.envelopeFollowerAttackToFilterLFODepth > 0.0095 || mySettings.envelopeFollowerAttackToFilterLFODepth < -0.0095) {

      envelope_mix = mySettings.envelopeFollowerAttackToFilterLFODepth * valueFilterAttackAnalyse;

      envelope_mix += local_filter_lfo_intensety;
      if ( envelope_mix >= 1.0) envelope_mix = 1.0;
      if ( envelope_mix <= -1.0) envelope_mix = -1.0;
      //Serial.printf("was geht mit dem filter? %f\n", envelope_mix);
      Filter_LFO_depth.amplitude(envelope_mix);
    }
    else
    {
      Filter_LFO_depth.amplitude(local_filter_lfo_intensety);
      //Serial.printf("was geht mit dem filter? %f\n", local_filter_lfo_intensety);
    }
  }


  //###############################################LFO Volumencontroll
  if (FOLLOW_VOL_LFO.available()) {

    valueFOLLOW_VOL_LFO = FOLLOW_VOL_LFO.readFull(); // werte -1- 1  ---> 0.0 - 1.0

    // envelope_mix
    // valueFCMAnalyse * vol_lfo_follow_envelope_attack
    float envelope_mix = 1.0f;
    if (mySettings.vol_lfo_follow_envelope_attack > 0.009 || mySettings.vol_lfo_follow_envelope_attack < -0.009) {

      if (mySettings.vol_lfo_follow_envelope_attack > 0.0)
        envelope_mix = mySettings.vol_lfo_follow_envelope_attack * valueFilterAttackAnalyse;
      else
        envelope_mix = 1 - (mySettings.vol_lfo_follow_envelope_attack * valueFilterAttackAnalyse * -1);
    }

    //osc1
    valueFOLLOW_VOL_LFO_invertOSC1 = valueFOLLOW_VOL_LFO;
    offsetInverterLVOvolOSC1 = 1.0;
    if (mySettings.vol_lfo_T1_OSC1_intensety < 0.005) {
      valueFOLLOW_VOL_LFO_invertOSC1 = valueFOLLOW_VOL_LFO * -1.0;
      offsetInverterLVOvolOSC1 = -1.0;
    }
    VOL_LFO_OSC1_DEPTH.amplitude(1 - ((valueFOLLOW_VOL_LFO_invertOSC1 + 0.5 ) * (mySettings.vol_lfo_T1_OSC1_intensety * offsetInverterLVOvolOSC1 * envelope_mix )) , 2);
    //   Serial.printf("Time: %d | Full Read: %f | Mit offset: %f | Mit Mischer %f | Value: %f\n",milli, valueFOLLOW_VOL_LFO, valueFOLLOW_VOL_LFO + 0.5,
    //                 1 - ((valueFOLLOW_VOL_LFO_invertOSC1 + 0.5 ) * (mySettings.vol_lfo_T1_OSC1_intensety * offsetInverterLVOvolOSC1 * valueFCMAnalyse)),mySettings.vol_lfo_T1_OSC1_intensety );

    //osc2
    valueFOLLOW_VOL_LFO_invertOSC2 = valueFOLLOW_VOL_LFO;
    offsetInverterLVOvolOSC2 = 1.0;
    if (mySettings.vol_lfo_T2_OSC2_intensety < 0.005) {
      valueFOLLOW_VOL_LFO_invertOSC2 = valueFOLLOW_VOL_LFO * -1.0;
      offsetInverterLVOvolOSC2 = -1.0;
    }
    VOL_LFO_OSC2_DEPTH.amplitude(1 - ((valueFOLLOW_VOL_LFO_invertOSC2 + 0.5 ) * (mySettings.vol_lfo_T2_OSC2_intensety * offsetInverterLVOvolOSC2 * envelope_mix )) , 2);

    //SUB2
    valueFOLLOW_VOL_LFO_invertSUBS = valueFOLLOW_VOL_LFO;
    offsetInverterLVOvolSUBS = 1.0;
    if (mySettings.vol_lfo_T3_SUBS_intensety < 0.005) {
      valueFOLLOW_VOL_LFO_invertSUBS = valueFOLLOW_VOL_LFO * -1.0;
      offsetInverterLVOvolSUBS = -1.0;
    }
    VOL_LFO_SUBS_DEPTH.amplitude(1 - ((valueFOLLOW_VOL_LFO_invertSUBS + 0.5 ) * (mySettings.vol_lfo_T3_SUBS_intensety * offsetInverterLVOvolSUBS * envelope_mix )) , 2);
    //Serial.printf("Time: %d | Full Read: %f | Mit offset: %f | Mit Mischer %f | Value: %f\n",milli, valueFOLLOW_VOL_LFO, valueFOLLOW_VOL_LFO + 0.5,
    //              1 - ((valueFOLLOW_VOL_LFO_invertSUBS + 0.5 ) * (mySettings.vol_lfo_T3_SUBS_intensety * offsetInverterLVOvolSUBS )),mySettings.vol_lfo_T3_SUBS_intensety );
    // Noise
    VOL_LFO_NOISE_DEPTH.amplitude(1 - ((valueFOLLOW_VOL_LFO + 0.5 ) * mySettings.vol_lfo_T4_NOISE_intensety * envelope_mix ) , 2);



    float mybpmToHz = bpmToHz(mySettings.vol_lfo_bpm);
    if (mySettings.MasterClockToLfos || mySettings.MasterClockToVOLLfo) mybpmToHz = bpmToHz(mySystemBpm);
    float  mybeatMultiLookup = beatMultiLookup[mySettings.vol_lfo_freq_bpm_multi];
    VOLUMEN_LFO.frequency((mybpmToHz * mybeatMultiLookup) + (valueFCMAnalyse * mySettings.envelopeFollowerAttackToVOLLFOFrequenzy * mybpmToHz * mybeatMultiLookup * 3));
  }

  // ############################ MOD LFO PITCH #####################################

  //    MOD_lfo_OSC1_extrapitch = 1.0;
  //    MOD_lfo_OSC2_extrapitch = 1.0;
  //    MOD_lfo_SUB1_extrapitch = 1.0;
  //    MOD_lfo_SUB2_extrapitch = 1.0;

  if (FOLLOW_MOD_LFO.available()) {
    // OSC 1
    float local_MOD_LFO_val = FOLLOW_MOD_LFO.readFull();

    float envelope_mix = 1.0f;
    float localValueFilterAttackAnalyse = valueFilterAttackAnalyse;
    if (mySettings.mod_lfo_follow_envelope_attack > 0.0095 || mySettings.mod_lfo_follow_envelope_attack < -0.0095) {

      if (mySettings.mod_lfo_follow_envelope_attack > 0.0)
        envelope_mix = mySettings.mod_lfo_follow_envelope_attack * localValueFilterAttackAnalyse;
      else
        envelope_mix = 1 - (mySettings.mod_lfo_follow_envelope_attack * localValueFilterAttackAnalyse * -1);
    }



    if (local_MOD_LFO_val >= 0) {
      MOD_lfo_OSC1_extrapitch = ( local_MOD_LFO_val * envelope_mix ) * (mySettings.mod_lfo_T1_OSC1_FREQ_intensety * 2.0 ) + 1 ;
    }
    else
    {
      MOD_lfo_OSC1_extrapitch = ( local_MOD_LFO_val * envelope_mix ) * (mySettings.mod_lfo_T1_OSC1_FREQ_intensety) + 1;
    }
    if (DEB_LFO) Serial.printf("Time: %d | settings val: %f | RAW MOD LFO: %f| FREQ-Dev: %f | envelope mix: %f | value filter analyse %f| raw:%f\n", milli, mySettings.mod_lfo_T1_OSC1_FREQ_intensety, local_MOD_LFO_val, MOD_lfo_OSC1_extrapitch , envelope_mix, localValueFilterAttackAnalyse, valueFilterAttackAnalyse );



    // OSC 2
    if (local_MOD_LFO_val >= 0) {
      MOD_lfo_OSC2_extrapitch = (local_MOD_LFO_val  * envelope_mix ) * (mySettings.mod_lfo_T2_OSC2_FREQ_intensety * 2.0 ) + 1 ;
    }
    else
    {
      MOD_lfo_OSC2_extrapitch = (local_MOD_LFO_val  * envelope_mix ) * (mySettings.mod_lfo_T2_OSC2_FREQ_intensety) + 1;
    }
    //if(DEB_LFO) Serial.printf("Time: %d | settings val: %f | RAW MOD LFO: %f| FREQ-Dev: %f\n",milli,mySettings.mod_lfo_T2_OSC2_FREQ_intensety,local_MOD_LFO_val,MOD_lfo_OSC2_extrapitch);



    // SUB 1
    if (local_MOD_LFO_val >= 0) {
      MOD_lfo_SUB1_extrapitch = (local_MOD_LFO_val  * envelope_mix ) * (mySettings.mod_lfo_T3_SUB1_FREQ_intensety * 2.0 ) + 1 ;
    }
    else
    {
      MOD_lfo_SUB1_extrapitch = (local_MOD_LFO_val  * envelope_mix ) * (mySettings.mod_lfo_T3_SUB1_FREQ_intensety) + 1;
    }

    //if(DEB_LFO) Serial.printf("Time: %d | settings val: %f | RAW MOD LFO: %f| FREQ-Dev: %f\n",milli,mySettings.mod_lfo_T3_SUB1_FREQ_intensety,local_MOD_LFO_val,MOD_lfo_SUB1_extrapitch);



    // SUB 2
    if (local_MOD_LFO_val >= 0) {
      MOD_lfo_SUB2_extrapitch = (local_MOD_LFO_val  * envelope_mix ) * (mySettings.mod_lfo_T4_SUB2_FREQ_intensety * 2.0 ) + 1 ;
    }
    else
    {
      MOD_lfo_SUB2_extrapitch = (local_MOD_LFO_val  * envelope_mix ) * (mySettings.mod_lfo_T4_SUB2_FREQ_intensety) + 1;
    }

    //if(DEB_LFO) Serial.printf("Time: %d | settings val: %f | RAW MOD LFO: %f| FREQ-Dev: %f\n",milli,mySettings.mod_lfo_T1_OSC1_FREQ_intensety,local_MOD_LFO_val,MOD_lfo_OSC1_extrapitch);


    
    if(DEB_EXTRA_PITCH){
      Serial.printf("OSC1 extra Pitch: %f,OSC2 extra Pitch: %f,SubOSC1 extra Pitch: %f,SubOSC1 extra Pitch: %f\n", MOD_lfo_OSC1_extrapitch, MOD_lfo_OSC2_extrapitch, MOD_lfo_SUB1_extrapitch, MOD_lfo_SUB2_extrapitch);
    }

    
    // ############ mod lfo pulse width

    //osc1
    if (mySettings.mod_lfo_T1_OSC1_PULSE_intensety > 0.01 || mySettings.mod_lfo_T1_OSC1_PULSE_intensety < -0.01  ) {
      valueFOLLOW_MOD_LFO_invertOSC1 = local_MOD_LFO_val;
      offsetInverterLVOmodOSC1 = 1.0;
      if (mySettings.mod_lfo_T1_OSC1_PULSE_intensety < - 0.005) {
        valueFOLLOW_MOD_LFO_invertOSC1 = local_MOD_LFO_val * -1.0;
        offsetInverterLVOmodOSC1 = -1.0;
      }
      float OSC1PULSE = mySettings.osc1_pulseWidth;
      OSC1PULSE = abs((valueFOLLOW_MOD_LFO_invertOSC1 + 0.6) * mySettings.mod_lfo_T1_OSC1_PULSE_intensety * 0.9 * offsetInverterLVOmodOSC1 * envelope_mix ) + mySettings.osc1_pulseWidth;
      if (OSC1PULSE <= 0.01) OSC1PULSE = 0.01;
      Osc1.pulseWidth(OSC1PULSE);
      //Serial.printf("Time: %d | settings val: %f | RAW MOD LFO: %f| Plus Inverter: %f | OSC1PHASE: %f\n",milli,mySettings.mod_lfo_T1_OSC1_PULSE_intensety,local_MOD_LFO_val,valueFOLLOW_MOD_LFO_invertOSC1,OSC1PULSE);
      PULSE_TRANSITION_OSC1 = true;
    }
    else
    {
      if (PULSE_TRANSITION_OSC1) {
        if (DEBUG_MIDI_INPUT) Serial.println("PULSE off"); // check midi death zone around value 63 - 64
        PULSE_TRANSITION_OSC1 = false;
        Osc1.pulseWidth(mySettings.osc1_pulseWidth);
      }
    }


    //osc2
    if (mySettings.mod_lfo_T2_OSC2_PULSE_intensety > 0.01 || mySettings.mod_lfo_T2_OSC2_PULSE_intensety < -0.01  ) {
      valueFOLLOW_MOD_LFO_invertOSC2 = local_MOD_LFO_val;
      offsetInverterLVOmodOSC2 = 1.0;
      if (mySettings.mod_lfo_T2_OSC2_PULSE_intensety < - 0.005) {
        valueFOLLOW_MOD_LFO_invertOSC2 = local_MOD_LFO_val * -1.0;
        offsetInverterLVOmodOSC2 = -1.0;
      }
      float OSC2PULSE = mySettings.osc2_pulseWidth;
      OSC2PULSE = abs((valueFOLLOW_MOD_LFO_invertOSC2 + 0.6) * mySettings.mod_lfo_T2_OSC2_PULSE_intensety * 0.9 * offsetInverterLVOmodOSC2 * envelope_mix) + mySettings.osc2_pulseWidth;
      if (OSC2PULSE <= 0.01) OSC2PULSE = 0.01;
      Osc2.pulseWidth(OSC2PULSE);
      PULSE_TRANSITION_OSC2 = true;
    }
    else
    {
      if (PULSE_TRANSITION_OSC2) {
        //Serial.println("PHASE off");
        PULSE_TRANSITION_OSC2 = false;
        Osc2.pulseWidth(mySettings.osc2_pulseWidth);
      }
    }

    //SUB1
    if (mySettings.mod_lfo_T3_SUB1_PULSE_intensety > 0.01 || mySettings.mod_lfo_T3_SUB1_PULSE_intensety < -0.01  ) {
      valueFOLLOW_MOD_LFO_invertSUB1 = local_MOD_LFO_val;
      offsetInverterLVOmodSUB1 = 1.0;
      if (mySettings.mod_lfo_T3_SUB1_PULSE_intensety < - 0.005) {
        valueFOLLOW_MOD_LFO_invertSUB1 = local_MOD_LFO_val * -1.0;
        offsetInverterLVOmodSUB1 = -1.0;
      }
      float SUB1PULSE = mySettings.sub_osc1_pulseWidth;
      SUB1PULSE = abs((valueFOLLOW_MOD_LFO_invertSUB1 + 0.6) * mySettings.mod_lfo_T3_SUB1_PULSE_intensety * 0.9 * offsetInverterLVOmodSUB1 * envelope_mix) + mySettings.sub_osc1_pulseWidth;
      if (SUB1PULSE <= 0.01) SUB1PULSE = 0.01;
      SubOsc1.pulseWidth(SUB1PULSE);
      PULSE_TRANSITION_SUB1 = true;
    }
    else
    {
      if (PULSE_TRANSITION_SUB1) {
        //Serial.println("PHASE off");
        PULSE_TRANSITION_SUB1 = false;
        SubOsc1.pulseWidth(mySettings.sub_osc1_pulseWidth);
      }
    }

    //SUB2
    if (mySettings.mod_lfo_T4_SUB2_PULSE_intensety > 0.01 || mySettings.mod_lfo_T4_SUB2_PULSE_intensety < -0.01  ) {
      valueFOLLOW_MOD_LFO_invertSUB2 = local_MOD_LFO_val;
      offsetInverterLVOmodSUB2 = 1.0;
      if (mySettings.mod_lfo_T4_SUB2_PULSE_intensety < - 0.005) {
        valueFOLLOW_MOD_LFO_invertSUB2 = local_MOD_LFO_val * -1.0;
        offsetInverterLVOmodSUB2 = -1.0;
      }
      float SUB2PULSE = mySettings.sub_osc2_pulseWidth;
      SUB2PULSE = abs((valueFOLLOW_MOD_LFO_invertSUB2 + 0.6) * mySettings.mod_lfo_T4_SUB2_PULSE_intensety * 0.9 * offsetInverterLVOmodSUB2 * envelope_mix) + mySettings.sub_osc2_pulseWidth;
      if (SUB2PULSE <= 0.01) SUB2PULSE = 0.01;
      SubOsc2.pulseWidth(SUB2PULSE);
      PULSE_TRANSITION_SUB2 = true;
    }
    else
    {
      if (PULSE_TRANSITION_SUB2) {
        //Serial.println("PHASE off");
        PULSE_TRANSITION_SUB2 = false;
        SubOsc2.pulseWidth(mySettings.sub_osc2_pulseWidth);
      }
    }


    //   VOL_LFO_OSC1_DEPTH.amplitude(1 - ((valueFOLLOW_VOL_LFO_invertOSC1 + 0.5 ) * (mySettings.vol_lfo_T1_OSC1_intensety * offsetInverterLVOvolOSC1 * envelope_mix )) ,2);
    //   Serial.printf("Time: %d | Full Read: %f | Mit offset: %f | Mit Mischer %f | Value: %f\n",milli, valueFOLLOW_VOL_LFO, valueFOLLOW_VOL_LFO + 0.5,
    //                 1 - ((valueFOLLOW_VOL_LFO_invertOSC1 + 0.5 ) * (mySettings.vol_lfo_T1_OSC1_intensety * offsetInverterLVOvolOSC1 * valueFCMAnalyse)),mySettings.vol_lfo_T1_OSC1_intensety );


    // ################ mod lfo freq follow envelope
    float mybpmToHz = bpmToHz(mySettings.mod_lfo_bpm);
    if (mySettings.MasterClockToLfos || mySettings.MasterClockToMODLfo) mybpmToHz = bpmToHz(mySystemBpm);
    float  mybeatMultiLookup = beatMultiLookup[mySettings.mod_lfo_freq_bpm_multi];
    MOD_LFO.frequency((mybpmToHz * mybeatMultiLookup) + (valueFCMAnalyse * mySettings.envelopeFollowerAttackToMODLFOFrequenzy * mybpmToHz * mybeatMultiLookup * 3));
  }// mod lfo end ########################




  // ############################### instrument envelope detection ##################
  // read back fundamental frequency
  int32_t cpeak = myAttackDetector.peakInfo(); // 32767 =1.0 0= 0.0 gain
  float ampli = float(cpeak / 32767.0);
  float dcAmpli = float((cpeak - mySettings.pitchDetectTreshold) / 32767.0);
  if (dcAmpli < 0.0) dcAmpli = 0.0;

  // ############################### pitch detection ##################
  // auto controll pitch and attack detect analyse gain from guitar input
  // find maximum
  bool must_lower_analyse_gain = false;
  if (cpeak > 32700) must_lower_analyse_gain = true;
  
  if (notefreq.available() && cpeak > mySettings.pitchDetectTreshold && myMidiNote.adsrState == 0 && !FrozenNote) { // wir folgen der gitarre
 
    // -8191  0 8191
    // center values for exact pitching
    if (myMidiNote.midiPitch < -8191) myMidiNote.midiPitch = -8191;
    if (myMidiNote.midiPitch > 8191) myMidiNote.midiPitch = 8191;

    // 24 = 2400 cent
    // 1 = 100 cent
    int pitchstepsincent = map(myMidiNote.midiPitch, -8191, 8191, -mySettings.midi_pichbandrange * 100, mySettings.midi_pichbandrange * 100);
    int myextraoctacve = pitchstepsincent / 1200;
    int myextranote =  (pitchstepsincent - myextraoctacve * 1200) / 100;
    int myextracent =  ((pitchstepsincent - (myextraoctacve * 1200)) - myextranote * 100) / 8.3;
  
//    if(FrozenNote) note = note;
//    else note = notefreq.read();
    note = notefreq.read();
    
    prob = notefreq.probability();
    //Serial.printf("Freq: %f | Quali: %f | cpeak: %d\n",note,prob,cpeak);
    // pichdetection to synth

    if (!mySettings.gitarUseEnvelopeADSR && !taptempoOnGuitarOrKeyboard) {
      // Tracking Led On
      myLedStatus.TRACKING_LED_MILLIS_ON = milli;
      myLedStatus.TRACKING_LED_MILLIS = 3;
    }

    if (DEB_PITCH_DETECTION) Serial.printf("%d ,Corner passed with: %d | Gain to high: %d| Freq: %f | Quali: %f |\n", milli, cpeak, must_lower_analyse_gain, note, prob );

    //long ttt=micros();
    osc1hz = get_frequency(note, mySettings.osc1_octave + myextraoctacve, mySettings.osc1_note + myextranote + SequencerExtraNote, mySettings.osc1_detune + myextracent) * MOD_lfo_OSC1_extrapitch;
    //Serial.println(osc1hz);
    osc2hz = get_frequency(note, mySettings.osc2_octave + myextraoctacve, mySettings.osc2_note + myextranote + SequencerExtraNote, mySettings.osc2_detune + myextracent) * MOD_lfo_OSC2_extrapitch;
    subosc1hz = get_frequency(note, mySettings.sub_osc1_octave + myextraoctacve, mySettings.sub_osc1_note + myextranote + SequencerExtraNote, mySettings.sub_osc1_detune + myextracent) * MOD_lfo_SUB1_extrapitch;
    subosc2hz = get_frequency(note, mySettings.sub_osc2_octave + myextraoctacve, mySettings.sub_osc2_note + myextranote + SequencerExtraNote, mySettings.sub_osc2_detune + myextracent) * MOD_lfo_SUB2_extrapitch;

    //Serial.println(micros()-ttt);

    //AudioNoInterrupts();// ?????
    if (prob >= mySettings.pitchDetectQuality) {
      Osc1.frequency(osc1hz);
      Osc2.frequency(osc2hz);
      SubOsc1.frequency(subosc1hz);
      SubOsc2.frequency(subosc2hz);
    }
    //AudioInterrupts();

  }
  else
  {  // Frozen note
    if(FrozenNote){
      //long ttt=micros();
      
      // -8191  0 8191
      // center values for exact pitching
      if (myMidiNote.midiPitch < -8191) myMidiNote.midiPitch = -8191;
      if (myMidiNote.midiPitch > 8191) myMidiNote.midiPitch = 8191;
  
      // 24 = 2400 cent
      // 1 = 100 cent
      int pitchstepsincent = map(myMidiNote.midiPitch, -8191, 8191, -mySettings.midi_pichbandrange * 100, mySettings.midi_pichbandrange * 100);
      int myextraoctacve = pitchstepsincent / 1200;
      int myextranote =  (pitchstepsincent - myextraoctacve * 1200) / 100;
      int myextracent =  ((pitchstepsincent - (myextraoctacve * 1200)) - myextranote * 100) / 8.3;
        
      osc1hz = get_frequency(note, mySettings.osc1_octave + myextraoctacve, mySettings.osc1_note + myextranote + SequencerExtraNote, mySettings.osc1_detune + myextracent) * MOD_lfo_OSC1_extrapitch;
      //Serial.println(osc1hz);
      osc2hz = get_frequency(note, mySettings.osc2_octave + myextraoctacve, mySettings.osc2_note + myextranote + SequencerExtraNote, mySettings.osc2_detune + myextracent) * MOD_lfo_OSC2_extrapitch;
      subosc1hz = get_frequency(note, mySettings.sub_osc1_octave + myextraoctacve, mySettings.sub_osc1_note + myextranote + SequencerExtraNote, mySettings.sub_osc1_detune + myextracent) * MOD_lfo_SUB1_extrapitch;
      subosc2hz = get_frequency(note, mySettings.sub_osc2_octave + myextraoctacve, mySettings.sub_osc2_note + myextranote + SequencerExtraNote, mySettings.sub_osc2_detune + myextracent) * MOD_lfo_SUB2_extrapitch;

      Osc1.frequency(osc1hz);
      Osc2.frequency(osc2hz);
      SubOsc1.frequency(subosc1hz);
      SubOsc2.frequency(subosc2hz);

      
    }
  }

  // MIDI NOTEN #####################
  if (myMidiNote.adsrState != 0 && milli > old_MidinoteUpdateTime + 20) { // hier die keyboard midi berechnungen für die oscs


    // -8191  0 8191
    // center values for exact pitching
    if (myMidiNote.midiPitch < -8191) myMidiNote.midiPitch = -8191;
    if (myMidiNote.midiPitch > 8191) myMidiNote.midiPitch = 8191;

    // 24 = 2400 cent
    // 1 = 100 cent
    int pitchstepsincent = map(myMidiNote.midiPitch, -8191, 8191, -mySettings.midi_pichbandrange * 100, mySettings.midi_pichbandrange * 100);
    int myextraoctacve = pitchstepsincent / 1200;
    int myextranote =  (pitchstepsincent - myextraoctacve * 1200) / 100;
    int myextracent =  ((pitchstepsincent - (myextraoctacve * 1200)) - myextranote * 100) / 8.3;


    if (DEB_PITCH_DETECTION) Serial.printf("%d || Midi Note will synthesized with %f hz , and Pitch in octave note cent: %d | %d | %d\n", milli, myMidiNote.midiNote, myextraoctacve, myextranote, myextracent );

    osc1hz = get_frequency(myMidiNote.midiNote, mySettings.osc1_octave + myextraoctacve, mySettings.osc1_note + myextranote + SequencerExtraNote, mySettings.osc1_detune + myextracent) * MOD_lfo_OSC1_extrapitch;
    osc2hz = get_frequency(myMidiNote.midiNote, mySettings.osc2_octave + myextraoctacve, mySettings.osc2_note + myextranote + SequencerExtraNote, mySettings.osc2_detune + myextracent) * MOD_lfo_OSC2_extrapitch;
    subosc1hz = get_frequency(myMidiNote.midiNote, mySettings.sub_osc1_octave + myextraoctacve, mySettings.sub_osc1_note + myextranote + SequencerExtraNote, mySettings.sub_osc1_detune + myextracent) * MOD_lfo_SUB1_extrapitch;
    subosc2hz = get_frequency(myMidiNote.midiNote, mySettings.sub_osc2_octave + myextraoctacve, mySettings.sub_osc2_note + myextranote + SequencerExtraNote, mySettings.sub_osc2_detune + myextracent) * MOD_lfo_SUB2_extrapitch;

    //AudioNoInterrupts(); //???
    Osc1.frequency(osc1hz);
    Osc2.frequency(osc2hz);
    SubOsc1.frequency(subosc1hz);
    SubOsc2.frequency(subosc2hz);
    //AudioInterrupts();
    old_MidinoteUpdateTime = milli;

  }

  // ################## Envelope vca und filter ######################
  if (milli >= oldmilliTime + 2) {
    if (dcAmpli <= 0.008) dcAmpli = 0.0; // macht das sinn so?
    // ############ envelope ###################
    // amp envelope
    if (mySettings.gitarUseEnvelopeADSR && !FrozenNote) {
      if (dcAmpli > EnvelopLongSustainVelecety && dcAmpli > 0.05) {
        if(!reachEnvelopLongSustainVelecetyLevel && milli >= myGuitarNote.attackTime + 10){
          //Serial.printf("env GO, %d\n",milli);
          if(mySettings.midi_attack >100)
            VCAenvelopeGenerator.amplitude(0.0, 0.0);
          else
            VCAenvelopeGenerator.amplitude(0.0, 0.03);
        }
        reachEnvelopLongSustainVelecetyLevel = true;
        EnvelopLongSustainVelecety = dcAmpli;
        myGuitarNote.midiVelocety = dcAmpli;
        myGuitarNote.adsrState = 1;
        myGuitarNote.attackTime = milli;
        StartSequencer();
        // Tracking Led On
        myLedStatus.TRACKING_LED_MILLIS_ON = milli;
        myLedStatus.TRACKING_LED_MILLIS = 50;
      }
      else
      {
        if (reachEnvelopLongSustainVelecetyLevel) {
          reachEnvelopLongSustainVelecetyLevel = false;
          myGuitarNote.adsrState = 2;
        }
      }
    }
    else {
      if(!FrozenNote) EnvelopLongSustainVelecety = dcAmpli;
    }


    // ############ Filter envelope ###################
    if (mySettings.gitarUseFilterADSR && !FrozenNote) {
      if (dcAmpli /*- 0.1*/ > FilterLongSustainVelecety && dcAmpli > 0.05) { // schaltschwelle 0.1 ist ein test
        //Serial.println(milli);
        if(!reachFilterLongSustainVelecetyLevel && milli >= myFilterNote.delayTime + 10){
          //Serial.printf("Filter GO, %d\n",milli);
          filter1envelopeGenerator.amplitude(0.0, 0.0);
        }
        reachFilterLongSustainVelecetyLevel = true;
        FilterLongSustainVelecety = dcAmpli;
        if (mySettings.gitarUseFilterADSRstaticVelocity >= 0.01) myFilterNote.filterVelocety = mySettings.gitarUseFilterADSRstaticVelocity;
        else myFilterNote.filterVelocety = dcAmpli;
        myFilterNote.adsrState = 1;
        myFilterNote.delayTime = milli;
        myFilterNote.isGuitar = true;
      }
      else
      {
        if (reachFilterLongSustainVelecetyLevel) {
          reachFilterLongSustainVelecetyLevel = false;
          myFilterNote.adsrState = 2;
        }
      }
    }
    else
      if(!FrozenNote) FilterLongSustainVelecety = dcAmpli;

    if (myMidiNote.adsrState == 0) { // wen midi notes spielen kein envelope von gitarre

      if (!mySettings.gitarUseEnvelopeADSR)
        VCAenvelopeGenerator.amplitude(EnvelopLongSustainVelecety, mySettings.sensingAttack); // die rampzeit kann hier aähnlich adsr Attack benutzt werden. darf nicht <1ms werden und max ist 999ms
      // filter envelope  der input kann konfiguriert werden zb inverted envelop und intensität, hier muss noch geklärt werden ob weitere multiplyer notig sind
      if (!mySettings.gitarUseFilterADSR) filter1envelopeGenerator.amplitude(dcAmpli/*EnvelopLongSustainVelecety*/, mySettings.sensingAttack);

    }

    if (DEB_ENVELOPE_FOLLOWER) Serial.printf("Probe: %f|| Time: %d ||AMPLI: %f || DcAmpli: %f| Static velocety: %f\n", prob, milli, ampli, dcAmpli, EnvelopLongSustainVelecety);
    oldmilliTime = milli;
  }

  //  ################### Filter adsr note of detection in loop, this is possible because 30 ms + - are not importent here

  if (cpeak < mySettings.sensingNoteOffCorner && !key_input && !FrozenNote) {
    if (mySettings.gitarUseEnvelopeADSR && ((myGuitarNote.adsrState == 6 ) || (myGuitarNote.adsrState == 1 ) || (myGuitarNote.adsrState == 2 ) || (myGuitarNote.adsrState == 3 )  )) {
      myGuitarNote.adsrState = 5;
      EnvelopLongSustainVelecety = 0.0;
      FilterLongSustainVelecety = 0.0;
      StopSequencer();
      //Serial.println(myGuitarNote.adsrState);
    }
    if (myFilterNote.adsrState == 7 && myMidiNote.adsrState == 0) { // filterNote 7 ist in systain status das ist die logische folgerung wenn gitarre gespielt wird und der filter state relase eingeschlatet werden soll
      if (DEB_GIUTAR_ATTACK_DETECTION) Serial.println(myFilterNote.adsrState);
      myFilterNote.adsrState = 6; // schalten auf release weil cpeak unterhalb der grenze liegt und fertig

    }
  }

  //############################ Dynamic Filter Frequencys #####################
  if (mySettings.Filter1_Active_Frequenz_Tracking_Target > 0) {

    switch (mySettings.Filter1_Active_Frequenz_Tracking_Target) {
      case 1: // osc 1
        filter1.frequency(osc1hz * G_Filter1_Active_Frequenz_Tracking);
        break;

      case 2: // osc 2
        filter1.frequency(osc2hz * G_Filter1_Active_Frequenz_Tracking);
        break;

      case 3: // sub osc 1
        filter1.frequency(subosc1hz * G_Filter1_Active_Frequenz_Tracking);
        break;

      case 4: // sub osc 2
        filter1.frequency(subosc2hz * G_Filter1_Active_Frequenz_Tracking);
        break;
    }
  }

  if (mySettings.Filter2_Active_Frequenz_Tracking_Target > 0) {

    switch (mySettings.Filter2_Active_Frequenz_Tracking_Target) {
      case 1: // osc 1
        filter2.frequency(osc1hz * G_Filter2_Active_Frequenz_Tracking);
        break;

      case 2: // osc 2
        filter2.frequency(osc2hz * G_Filter2_Active_Frequenz_Tracking);
        break;

      case 3: // sub osc 1
        filter2.frequency(subosc1hz * G_Filter2_Active_Frequenz_Tracking);
        break;

      case 4: // sub osc 2
        filter2.frequency(subosc2hz * G_Filter2_Active_Frequenz_Tracking);
        break;
    }
  }

  // ########################### buttons   #########################
  // teste etwas zu speichern
  if (noteOnOffandOKButton.update()) {
    if (noteOnOffandOKButton.fallingEdge()) {

      if (menuExtrButton) {
        menuExtrButtonPos += 1;
        if (menuExtrButtonPos > 8) menuExtrButtonPos = 0;

        //Serial.printf("Menu Extrafunktion, Poiter at: %d\n",menuExtrButtonPos);
        selectMenuItemAndPrintLcd(0);
      }


      countAt2 = millis();
      previousState2 = LOW;
      longPressFinishedBtn2 = false;
    }
    if (noteOnOffandOKButton.risingEdge()) {
      if (!menuExtrButton) {
        //Serial.println("Kann note spielen");
        //Serial.println(myMidiNote.adsrState);
        // erst nach 8 sekunden betriebszeit erlaubt
        if (milli > 8000) {
          if(myMenuWindow == 133 || myMenuWindow == 134){
            if(DEB_EEPROM) Serial.println("write eeprom settings");
            EEPROM.put(EEPROM_MAIN_SYSTEM_SETTINGS_START, mySystemSettings);
            lcd.setCursor(0, 1);
            lcd.printf("Save System");          
          }
          else{
            if (myMidiNote.adsrState == 0) StartStopMidiNote( true, milli, 100, 50, 0);
            else StopAnyPlayingNote();
          }
        }
      }
      countAt2 = millis();
      previousState2 = HIGH;
      //Serial.printf("Knopf Nr.: %d  | Löse Zeit: %d ms | State: %d\n",1,countAt2,previousState2);
    }
  }
  else
  {
    if (millis() > countAt2 + 2000 && previousState2 == LOW && !longPressFinishedBtn2) {
      //Serial.printf("Longpress save Preset\n");
      previousState2 = HIGH;
      longPressFinishedBtn2 = true;
      lcd.clear();
      lcd.blink();
      lcd.setCursor(0, 0);
      lcd.printf(" Save Preset:%d", mySettings.preset);
      lcd.setCursor(0, 0);
      edb_updateOneRecord(current_preset);
      lcd.noBlink();
      selectMenuItemAndPrintLcd(0); // zurück ins menu
      if (myMidiNote.adsrState == 0) StartStopMidiNote( true, milli, 0, 50, 0);


    }
  }


  // knopftest später in einer schleife auszuführen um knöpfe x-y zu bedienen
  if (menuButtonUp.update()) {
    if (menuButtonUp.fallingEdge()) {
      myMenuWindow += 1;
      if (myMenuWindow > menuWindowSize) myMenuWindow = 0; //presets is first menu, sensingBounce is currenzly the last entry
      while (!hidingSettings[myMenuWindow] && hideSettings) {
        myMenuWindow += 1;
        if (myMenuWindow > menuWindowSize) myMenuWindow = 0;
      }
      selectMenuItemAndPrintLcd(0);
      //Serial.println(myMenuWindow);
    }
  }

  if (menuButtonDown.update()) {
    if (menuButtonDown.fallingEdge()) {
      myMenuWindow -= 1;
      if (myMenuWindow < 0) myMenuWindow = menuWindowSize; //presets is first menu, sensingBounce is currenzly the last entry
      while (!hidingSettings[myMenuWindow] && hideSettings) {
        myMenuWindow -= 1;
        if (myMenuWindow < 0) myMenuWindow = menuWindowSize;
      }
      selectMenuItemAndPrintLcd(0);
      //Serial.println(myMenuWindow);
    }
  }


  // Preset FussTaster
  
  if (PresetUpBtn.update()) {
    if (PresetUpBtn.fallingEdge()) {
      Serial.printf("Preset Button Up: %d, Program: %S ",current_preset + 1); 
      current_preset +=1;
      if(current_preset > 127)current_preset = 1; // 127 im fertig zustand zum testen 5
      if(current_preset < 1)current_preset = 127;
    
      AudioNoInterrupts();
      edb_selectPreset(current_preset);
      AudioInterrupts();
      setSettings(false);
      calculateFilterMixer();
      myMenuWindow = 0;
      selectMenuItemAndPrintLcd(0);

    }
  }

  if (PresetDownBtn.update()) {
    if (PresetDownBtn.fallingEdge()) {
      
      Serial.printf("Preset Button Down: %d, Program: %S ",current_preset - 1); 
      current_preset -=1;
      if(current_preset > 127)current_preset = 1; // 127 im fertig zustand zum testen 5
      if(current_preset < 1)current_preset = 127;
    
      AudioNoInterrupts();
      edb_selectPreset(current_preset);
      AudioInterrupts();
      setSettings(false);
      calculateFilterMixer();
      myMenuWindow = 0;
      selectMenuItemAndPrintLcd(0);

    }
  }


  if (incrementButton.update()) {
    if (incrementButton.fallingEdge()) {
      incrementButtoncounter += 1;
      countAt0 = millis();
      previousState0 = LOW;
      //Serial.printf("Knopf Nr.: %d | Drück Zeit: %d ms | State: %d\n",1,countAt,previousState);
      selectMenuItemAndPrintLcd(1);
    }
    if (incrementButton.risingEdge()) {
      incrementButtoncounter = 0;
      countAt0 = millis();
      previousState0 = HIGH;
      //Serial.printf("Knopf Nr.: %d  | Löse Zeit: %d ms | State: %d\n",1,countAt,previousState);
      //selectMenuItemAndPrintLcd();
    }

  }
  else
  {
    if (millis() > countAt0 + 1000 && previousState0 == LOW) {
      countAt0 += 100;
      incrementButtoncounter += 1;
      //selectMenuItemAndPrintLcd();
      selectMenuItemAndPrintLcd(1);
      //Serial.printf("Longpress \n");
      //}
    }
  }

  if (decrementButton.update()) {
    if (decrementButton.fallingEdge()) {
      countAt1 = millis();
      previousState1 = LOW;
      //Serial.printf("Knopf Nr.: %d | Drück Zeit: %d ms | State: %d\n",1,countAt,previousState);
      selectMenuItemAndPrintLcd(-1);
    }
    if (decrementButton.risingEdge()) {
      countAt1 = millis();
      previousState1 = HIGH;
      //Serial.printf("Knopf Nr.: %d  | Löse Zeit: %d ms | State: %d\n",1,countAt,previousState);
      //selectMenuItemAndPrintLcd();
    }

  }
  else
  {
    if (millis() > countAt1 + 1000 && previousState1 == LOW) {
      countAt1 += 100;
      //selectMenuItemAndPrintLcd();
      selectMenuItemAndPrintLcd(-1);
      //Serial.printf("Longpress \n");
      //}
    }
  }

  // ####################################################
  /*
     Zeitmessungen:

     1. einfachste ausführung mit gutem trakking. treshold = 0,34, falldown = 0,017
     ohne usb "Zeitmessung: 386877ms | Runden: 58895 | Langsamste Rundenzeit: 135us | Schnellste Rundenzeit: 8us | Durchschnitt Zeit: 8us"
     mit usb "Zeitmessung: 562890ms | Runden: 34124 | Langsamste Rundenzeit: 1320us | Schnellste Rundenzeit: 8us | Durchschnitt Zeit: 14us"
     Langsamste Rundenzeit: 252us | Schnellste Rundenzeit: 9us | Durchschnitt Zeit: 9us |CPU: Osc1=1,2  Notfreq=0,0  all=4.88,6.93    Memory: 1,4
          runtergetaktet bei 96 mhz Zeitmessung: 341111ms | Runden: 9021 | Langsamste Rundenzeit: 2222us | Schnellste Rundenzeit: 16us | Durchschnitt Zeit: 55us |CPU: Filter Processor: 4, 66.23, Max: 71.44 | Memory: 16, Max: 37
    CYCLE LFO runtergetaktet bei 96 mhz Zeitmessung: 232859ms | Runden: 7854 | Langsamste Rundenzeit: 2136us | Schnellste Rundenzeit: 17us | Durchschnitt Zeit: 63us |CPU: Filter Processor: 4, 69.13, Max: 73.64 | Memory: 19, Max: 37

     Time: 42811ms | R: 13979 | Slow T: 1574us | Fast T: 17us | avr T: 35us |CPU: F1 Load: 4, 52.09, Max: 53.92 | Mem: 8, Max: 20, Ex-Ped: 157, Pot: 0


     teensy 3.6
     pre midi
     Zeitmessung: 572384ms | Runden: 100455 | Langsamste Rundenzeit: 774us | Schnellste Rundenzeit: 3us | Durchschnitt Zeit: 4us |CPU: 24.76, Max: 27.43 | Memory: 5, Max: 18

  */
  // Loop timemessung und prozessorauslastung und masterclock
  unsigned long microtime = micros();

  // ##################  masterclock ######################

  //  long masteclockMicros = 0;
  //  long taptempoOld = 0;
  //  double mySystemBpm = 90.0;
  //  int myTapCounter = 0;

  if (tapTempoBtn.update()) {
    if (tapTempoBtn.fallingEdge()) {
      checkTapTempo(microtime);
      countAt3 = millis();
      previousState3 = LOW;
      myLedStatus.TRACKING_LED_MILLIS_ON = millis();
      myLedStatus.TRACKING_LED_MILLIS = 50;
    }
    if (tapTempoBtn.risingEdge()) {
      countAt3 = millis();
      previousState3 = HIGH;
    }

  }
  else
  {
    if (millis() > countAt3 + 2000 && previousState3 == LOW) {
      countAt3 += 200;

      if (!taptempoOnGuitarOrKeyboard) {
        taptempoOnGuitarOrKeyboard = true;
        myLedStatus.TRACKING_LED_MILLIS_ON = millis();
        myLedStatus.TRACKING_LED_MILLIS = 600;
        myLedStatus.TAPTEMPO_LED_MILLIS_ON = millis() + 100;
        myLedStatus.TAPTEMPO_LED_MILLIS = 700;
        myLedStatus.CLIP_LED_MILLIS_ON = millis() + 200;
        myLedStatus.CLIP_LED_MILLIS = 800;

        //Serial.printf("longpress , on guitar keys: %d", taptempoOnGuitarOrKeyboard);
      }

    }
    else if (millis() < countAt3 + 100 && previousState3 == HIGH && taptempoOnGuitarOrKeyboard) {
      taptempoOnGuitarOrKeyboard = false;
      //Serial.printf("longpress ende!!, on guitar keys: %d?n", taptempoOnGuitarOrKeyboard);
    }
  }

  checkSeqence(microtime); // fire sequencer



  // ##################  masterclock  ende ################

  if (longestRound < microtime - oldlongshortVal) longestRound = microtime - oldlongshortVal;
  if (shortestround > microtime - oldlongshortVal) shortestround = microtime - oldlongshortVal;

  timecycles += 1;

  if (microtime >= old_MicroTime + 500000) { // 500000 = 500 ms
    if (DEB_PROCESSOR_AND_TIME) Serial.printf("Time: %dms | R: %d | Slow T: %dus | Fast T: %dus | avr T: %dus |", millis(), timecycles, longestRound, shortestround, 500000 / timecycles);
    timecycles = 0;
    old_MicroTime = microtime;
    shortestround = 1000000;
    longestRound = 0;

    if (DEB_PROCESSOR_AND_TIME) {
      Serial.print("CPU: ");
      /*    Serial.print("Osc1:");
          Serial.print(Osc1.processorUsage());
          Serial.print(",");
          Serial.print(Osc1.processorUsageMax());
          Serial.print("  ");
          Serial.print("Notfreq: ");
          Serial.print(notefreq.processorUsage());
          Serial.print(",");
          Serial.print(notefreq.processorUsageMax());
          Serial.print("  ");filter1
          Serial.print("All: ");*/
      Serial.print("F1 Load: ");
      Serial.print(filter1.processorUsage());
      Serial.print(", ");
      Serial.print(AudioProcessorUsage());
      Serial.print(", Max: ");
      Serial.print(AudioProcessorUsageMax());
      Serial.print(" | ");
      Serial.print("Mem: ");
      Serial.print(AudioMemoryUsage());
      Serial.print(", Max: ");
      Serial.print(AudioMemoryUsageMax());
      Serial.print(", Ex-Ped: ");
      Serial.print(ExPedalValue);
      Serial.print(", Pot: ");
      Serial.println(PotiValue);
    }
  }
  oldlongshortVal = microtime;
  // ####################################################
  //Serial.println(thistime);

  masterclockLEDBlinker(milli, false);
  checkMyLedBlinker(milli);
  checksystemonmidiclock();
  showSystemBpmOnLCD(milli);
}



void mypeakcallback( bool test) { // currently not used
  if(FrozenNote) return;
  if (DEB_GIUTAR_ATTACK_DETECTION)
    Serial.printf("|| lllllllllllllllllllllllllllllllllllllll------------------> Hit Note-On| %d \n", test);

  unsigned long thisMilli = millis();

  if (mySettings.gitarUseEnvelopeADSR) {
    EnvelopLongSustainVelecety = 0.0;
  }

  if (mySettings.gitarUseFilterADSR) {
    // das hier ist neu
    FilterLongSustainVelecety = 0.0;
    // das ist alt und wird zz ersetzt
    //myFilterNote.adsrState = 1;
    //myFilterNote.delayTime = thisMilli;
    //myFilterNote.filterVelocety = 0.8; // noch nachbessern depth regler hier
  }
  //// test !!! kann knacken.... geht wohl doch nach test!
  //Filter_LFO.phase(mySettings.filter_lfo_phase);
  float filterlfobpm = mySettings.filter_lfo_bpm;
  float volumenlfobpm = mySettings.vol_lfo_bpm;
  float modlfobpm = mySettings.mod_lfo_bpm;
  if (mySettings.MasterClockToLfos) { // alle gleichzeitig syncronisieren
    filterlfobpm = mySystemBpm;
    volumenlfobpm = mySystemBpm;
    modlfobpm = mySystemBpm;
  }
  if (mySettings.MasterClockToMODLfo) { //
    modlfobpm = mySystemBpm;
  }
  if (mySettings.MasterClockToVOLLfo) {
    volumenlfobpm = mySystemBpm;
  }
  if (mySettings.MasterclockToFreqLFO) {
    filterlfobpm = mySystemBpm;
  }

  if (mySettings.filter_lfo_attack_sync) {
    if (mySettings.filter_lfo_waveform > 5) selectABTWaveTableFilterLFO( bpmToHz(filterlfobpm), mySettings.filter_lfo_waveform - 6);
    if (mySettings.filter_lfo_waveform < 6) Filter_LFO.begin(1.0, bpmToHz(filterlfobpm) * beatMultiLookup[mySettings.filter_lfo_freq_bpm_multi] , waveIntsLook[mySettings.filter_lfo_waveform], mySettings.filter_lfo_cycles);
  }
  if (mySettings.vol_lfo_attack_sync) {
    if (mySettings.vol_lfo_waveform > 5) selectABTWaveTableVolumenLFO( bpmToHz(volumenlfobpm), mySettings.vol_lfo_waveform - 6);
    if (mySettings.vol_lfo_waveform < 6) VOLUMEN_LFO.begin(0.5, bpmToHz(volumenlfobpm) * beatMultiLookup[mySettings.vol_lfo_freq_bpm_multi] , waveIntsLook[mySettings.vol_lfo_waveform], mySettings.vol_lfo_cycles);
  }
  if (mySettings.mod_lfo_attack_sync) {
    if (mySettings.mod_lfo_waveform > 5) selectABTWaveTableModLFO( bpmToHz(modlfobpm), mySettings.mod_lfo_waveform - 6);
    if (mySettings.mod_lfo_waveform < 6) MOD_LFO.begin(0.5, bpmToHz(modlfobpm) * beatMultiLookup[mySettings.mod_lfo_freq_bpm_multi] , waveIntsLook[mySettings.mod_lfo_waveform], mySettings.mod_lfo_cycles);
  }
  //// test


  // led blink on taptempo
  if (taptempoOnGuitarOrKeyboard) {
    checkTapTempo( micros() );
    myLedStatus.TRACKING_LED_MILLIS_ON = thisMilli;
    myLedStatus.TRACKING_LED_MILLIS = 50;
  }
  // das blinkt einmal hell im gitarUseEnvelope Mode
  if (!mySettings.gitarUseEnvelopeADSR && !taptempoOnGuitarOrKeyboard) {
    myLedStatus.TRACKING_LED_MILLIS_ON = thisMilli;
    myLedStatus.TRACKING_LED_MILLIS = 50;
  }


}

void checkMyLedBlinker(unsigned long milli) {
  if (milli >= myLedStatus.CLIP_LED_MILLIS_ON && milli <= myLedStatus.CLIP_LED_MILLIS_ON + myLedStatus.CLIP_LED_MILLIS)
    digitalWrite(CLIPLEDPIN, HIGH);
  else digitalWrite(CLIPLEDPIN, LOW);

  if (milli >= myLedStatus.TAPTEMPO_LED_MILLIS_ON && milli <= myLedStatus.TAPTEMPO_LED_MILLIS_ON + myLedStatus.TAPTEMPO_LED_MILLIS)
    digitalWrite(TAPTEMPOLEDPIN, HIGH);
  else digitalWrite(TAPTEMPOLEDPIN, LOW);

  if (milli >= myLedStatus.TRACKING_LED_MILLIS_ON && milli <= myLedStatus.TRACKING_LED_MILLIS_ON + myLedStatus.TRACKING_LED_MILLIS)
    digitalWrite(TRACKINGLEDPIN, HIGH);
  else digitalWrite(TRACKINGLEDPIN, LOW);
}

// ################# masterclock helper ##################
void checkTapTempo(unsigned long micro) {
  // ignore first tap by old bpm time diff + 1 second(example 90bpm)
  unsigned long tapTempoIgnoreTime = (unsigned long)((60000.0 / mySystemBpm + 1000) * 1000);
  //Serial.printf("vor check, ignore time: %d | timediff %d | my bpm %f\n",tapTempoIgnoreTime, microtime - taptempoOld,mySystemBpm );
  if (  micro - taptempoOld > tapTempoIgnoreTime) {
    myTapCounter = 0;
    taptempoIncrement = 0;
    if (DEB_TAPTEMPO) Serial.printf("LOOOOOOOSCHE incrementer: %d\n", taptempoIncrement);
  }
  myTapCounter += 1;
  if (myTapCounter > 2) {
    taptempoIncrement += micro - taptempoOld;
    mySystemBpm = 60000000.0 / double(taptempoIncrement / (myTapCounter - 2));
    mysystemFreqMicrosTime = double(taptempoIncrement / (myTapCounter - 2));
    sequencerSpeedTime = int(mysystemFreqMicrosTime / beatMultiLookup[mySettings.sequenceTimeSignature]);

    if (myTapCounter == 2 || myTapCounter % 4 == 0) {
      masterclockLEDBlinker(micro / 1000, true);
    }
    else masterclockLEDBlinker(micro / 1000, false);

    masterclockInternalSyncToLFOs(); // sync lfos
    if (DEB_TAPTEMPO) Serial.printf("ab jetzt counten: %d | incrementval: %d | timediff %d | my bpm %f | system microtime %d\n", myTapCounter, taptempoIncrement, micro - taptempoOld, mySystemBpm, mysystemFreqMicrosTime );
  }
  taptempoOld = micro;
}

// master clock syncronisation und led output

void masterclockLEDBlinker(unsigned long milli, bool directinput) {

  if (milli >= masterclockLEDBlinkerOldTime + mysystemFreqMicrosTime / 1000 || directinput) {
    myLedStatus.TAPTEMPO_LED_MILLIS_ON = milli;
    myLedStatus.TAPTEMPO_LED_MILLIS = 50;
    masterclockLEDBlinkerOldTime = milli;
  }
}

void masterclockInternalSyncToLFOs() {

  if (mySettings.MasterClockToLfos) { // alle gleichzeitig syncronisieren
    if (DEB_TAPTEMPO) Serial.printf("sync All LFos to: %f bpm:\n", mySystemBpm);

    if (mySettings.filter_lfo_waveform > 5) selectABTWaveTableFilterLFO(bpmToHz(mySystemBpm), mySettings.filter_lfo_waveform - 6);
    if (mySettings.filter_lfo_waveform < 6) Filter_LFO.begin(1.0, bpmToHz(mySystemBpm) * beatMultiLookup[mySettings.filter_lfo_freq_bpm_multi] , waveIntsLook[mySettings.filter_lfo_waveform], mySettings.filter_lfo_cycles);

    if (mySettings.vol_lfo_waveform > 5) selectABTWaveTableVolumenLFO( bpmToHz(mySystemBpm), mySettings.vol_lfo_waveform - 6);
    if (mySettings.vol_lfo_waveform < 6) VOLUMEN_LFO.begin(0.5, bpmToHz(mySystemBpm) * beatMultiLookup[mySettings.vol_lfo_freq_bpm_multi] , waveIntsLook[mySettings.vol_lfo_waveform], mySettings.vol_lfo_cycles);

    if (mySettings.mod_lfo_waveform > 5) selectABTWaveTableModLFO( bpmToHz(mySystemBpm), mySettings.mod_lfo_waveform - 6);
    if (mySettings.mod_lfo_waveform < 6) MOD_LFO.begin(0.5, bpmToHz(mySystemBpm) * beatMultiLookup[mySettings.mod_lfo_freq_bpm_multi] , waveIntsLook[mySettings.mod_lfo_waveform], mySettings.mod_lfo_cycles);
  }
  if (mySettings.MasterClockToMODLfo) { //
    if (DEB_TAPTEMPO) Serial.printf("sync Mod LFo to: %f bpm:\n", mySystemBpm);
    if (mySettings.mod_lfo_waveform > 5) selectABTWaveTableModLFO( bpmToHz(mySystemBpm), mySettings.mod_lfo_waveform - 6);
    if (mySettings.mod_lfo_waveform < 6) MOD_LFO.begin(0.5, bpmToHz(mySystemBpm) * beatMultiLookup[mySettings.mod_lfo_freq_bpm_multi] , waveIntsLook[mySettings.mod_lfo_waveform], mySettings.mod_lfo_cycles);
  }
  if (mySettings.MasterClockToVOLLfo) {
    if (DEB_TAPTEMPO) Serial.printf("sync Vol LFo to: %f bpm:\n", mySystemBpm);
    if (mySettings.vol_lfo_waveform > 5) selectABTWaveTableVolumenLFO( bpmToHz(mySystemBpm), mySettings.vol_lfo_waveform - 6);
    if (mySettings.vol_lfo_waveform < 6) VOLUMEN_LFO.begin(0.5, bpmToHz(mySystemBpm) * beatMultiLookup[mySettings.vol_lfo_freq_bpm_multi] , waveIntsLook[mySettings.vol_lfo_waveform], mySettings.vol_lfo_cycles);
  }
  if (mySettings.MasterclockToFreqLFO) {
    if (DEB_TAPTEMPO) Serial.printf("sync Filter LFo to: %f bpm:\n", mySystemBpm);
    if (mySettings.filter_lfo_waveform > 5) selectABTWaveTableFilterLFO( bpmToHz(mySystemBpm), mySettings.filter_lfo_waveform - 6);
    if (mySettings.filter_lfo_waveform < 6) Filter_LFO.begin(1.0, bpmToHz(mySystemBpm) * beatMultiLookup[mySettings.filter_lfo_freq_bpm_multi] , waveIntsLook[mySettings.filter_lfo_waveform], mySettings.filter_lfo_cycles);
  }

}


void checksystemonmidiclock() {
  if (millis() - 500 > MidioldTimeClockMilli && systemOnMidiClock) {
    systemOnMidiClock = false;
    if (DEBUG_MIDI_INPUT) Serial.println("Midiklock off");
  }
}



void showSystemBpmOnLCD(unsigned long milli) {
  if (milli >= oldShowBpmmillis + 1000) {
    if(myMenuWindow == 0){
      lcd.setCursor(7, 0);
      if (systemOnMidiClock && starblink) {
        lcd.printf(" | %4.1f*", mySystemBpm);
      }
      else
      {
        lcd.printf(" | %4.1f ", mySystemBpm);
      }
    }
    if(myMenuWindow == 133 || myMenuWindow == 134){
      lcd.setCursor(5, 1);
      lcd.printf(" | %d ", analogRead(ExPedalRead));
    }
    
    starblink = !starblink;
    oldShowBpmmillis = milli;
  }

}



void setSettings(bool fromsetup) {

  AudioNoInterrupts();
  setMCP42Value(byte(mySettings.instrumentBypassMix));
  // D-F-F INit
  G_Filter1_Active_Frequenz_Tracking = calculateDynamicFilterFrequency(mySettings.int_Filter1_Active_Frequenz_Tracking);
  G_Filter2_Active_Frequenz_Tracking = calculateDynamicFilterFrequency(mySettings.int_Filter2_Active_Frequenz_Tracking);
  //Serial.printf("Filter1 D-F-F: %d | Filter2 D-F-F: %d| Filter 1 DFF value %f| Filter 2 DFF value %f \n", mySettings.Filter1_Active_Frequenz_Tracking_Target, mySettings.Filter2_Active_Frequenz_Tracking_Target, G_Filter1_Active_Frequenz_Tracking, G_Filter2_Active_Frequenz_Tracking);
  if (mySettings.Filter1_Active_Frequenz_Tracking_Target > 0)
    using_Filter1_Active_Frequenz_Tracking = true;
  if (mySettings.Filter2_Active_Frequenz_Tracking_Target > 0)
    using_Filter2_Active_Frequenz_Tracking = true;


  // test masterclock war zu ungenau!
  //myMasterClock.begin(60.0);


   // Configure higpass and lowpass

   // data migration my existing presets on sd card
//   mySettings.freeDataFloat1 = 8000.0f;
//   mySettings.freeDataFloat2 = 0.7f;
//   mySettings.freeDataFloat3 = 80.0f;
//   mySettings.freeDataFloat4 = 0.7f;
 
   lowpass.frequency(mySettings.freeDataFloat1);
   lowpass.resonance(mySettings.freeDataFloat2);
   highpass.frequency(mySettings.freeDataFloat3);
   highpass.resonance(mySettings.freeDataFloat4);


  //################# frequenc tracking ##############
  // original teensy audio notefreq lib
  //if (fromsetup) notefreq.begin(.15);
  // new v3 library from colin duffy
  if (fromsetup) notefreq.begin(.15, fir_22059_HZ_coefficients, sizeof(fir_22059_HZ_coefficients), 2);
  //if (fromsetup) notefreq.begin(.15, fir_44117_HZ_coefficients, sizeof(fir_44117_HZ_coefficients), 2);
  //##################### Mixers Matrix ###################

  // ### UsbMixerLinks Und Rechts ###
  // I2S Eingang0 (Gittareneingang) Direkt Mix zum usb ausgang 0(L)
  // I2S Eingang0 (Gittareneingang) Direkt Mix zum usb ausgang 0(R)
  UsbOutputMixerLinks.gain(0, mySettings.instrumentToUsb_1);// Gitarre 0 -> UsbOut 0(L)
  UsbOutputMixerRechts.gain(0, mySettings.instrumentToUsb_2);// Gitarre 0 -> UsbOut 1(R)

  // VCAGuitarEnvelope (Synth) Direkt Mix zum usb ausgang 0(L)
  // VCAGuitarEnvelope (Synth) Direkt Mix zum usb ausgang 0(R)
  UsbOutputMixerLinks.gain(3, mySettings.synthToUsbOutput1);//  Synth -> UsbOut 0(L) war 0.7
  UsbOutputMixerRechts.gain(3, mySettings.synthToUsbOutput2);// Synth -> UsbOut 1(R) war 0.7

  // Unbelegte UsbMixer Eingänge sind default auf 0.0
  UsbOutputMixerLinks.gain(1, 0.0);
  UsbOutputMixerRechts.gain(1, 0.0);
  UsbOutputMixerLinks.gain(2, 0.0);
  UsbOutputMixerRechts.gain(2, 0.0);

  // ### I2S1 Ausgangsmixer Links und rechts ###
  // Diese Ausgangsmatrix erlaubt Den Instrumenteneingang(I2S Eingang0) direkt zum ausgang1 und 2 zu mixen !! mach ich analog
  I2SOutputMixerChannel1.gain(0, 0.0); // InstrumentMix (I2S Eingang0) -> I2SOut 0 (links)
  I2SOutputMixerChannel2.gain(0, 0.0); // InstrumentMix (I2S Eingang0) -> I2SOut 1 (rechts)

  // USB eingang MONO STEREO MATRIX, nutzbar um usb audio seperat an die zwei ausgänge zu verteilen
  I2SOutputMixerChannel1.gain(1, mySettings.usbIn_1_ToOutput1); // USBin 0(Links) -> i2s 0 (Links)
  I2SOutputMixerChannel1.gain(2, mySettings.usbIn_2_ToOutput1); // USBin 1(Rechts) -> i2s 0 (Links)
  I2SOutputMixerChannel2.gain(1, mySettings.usbIn_1_ToOutput2); // USBin 0(Links) -> i2s 1 (Rechts)
  I2SOutputMixerChannel2.gain(2, mySettings.usbIn_2_ToOutput2); // USBin 1(Rechts) -> i2s 1 (Rechts)

  // Synth nach I2S out
  I2SOutputMixerChannel1.gain(3, mySettings.synthToOutput1);//0.9
  I2SOutputMixerChannel2.gain(3, mySettings.synthToOutput2);


  // dieser mixer stellt den pegel vom usb in 0 zum analyse mixer ein
  // zurzeit nur von USBin. später noch vom i2s eingang
  mixerAnalyse.gain(0, mySettings.usbIn_1_ToSynth); // war 0.95
  mixerAnalyse.gain(2, mySettings.instrumentToSynth * 2.0); // war 1.9 Achtung, der gain ist so hoch weil ohne richgtige line vorstufe!! sonst 0.9


  // Main OSC MIXER
  OscMainMix.gain(0, mySettings.SYNTH_Master_Volume);
  OscMainMix.gain(1, mySettings.SYNTH_Master_Volume);
  OscMainMix.gain(2, mySettings.SYNTH_Master_Volume);//0.6 kommt vom subosc mixer
  OscMainMix.gain(3, mySettings.SYNTH_Master_Volume);//0.1

  SubOscMixer.gain(0, 0.7); // subosc 1
  SubOscMixer.gain(1, 0.7); // subosc 2
  SubOscMixer.gain(2, int(mySettings.Crusher_Volumen)); // bitcrusher

  Filter1FrequenzControlMixer.gain(2, 1.0); // input from LFO1 ! unklar
  Filter1FrequenzControlMixer.gain(3, 1.0); // input from filter1envelopeGenerator

  filter1envelopeIntensity.amplitude(mySettings.filter1envelopeIntensity); // das kommt in die settings für gitarren anschlag zu filter envelop -1 nach +1
  Filter_LFO_depth.amplitude(mySettings.filter_lfo_intensety);


  VOL_LFO_OSC1_DEPTH.amplitude(0.5);
  VOL_LFO_OSC2_DEPTH.amplitude(0.5);
  VOL_LFO_SUBS_DEPTH.amplitude(0.5);
  VOL_LFO_NOISE_DEPTH.amplitude(0.5);


  //################# OSC s ##########################
  // defaults
  osc1hz = get_frequency(160, mySettings.osc1_octave, mySettings.osc1_note, mySettings.osc1_detune);
  osc2hz = get_frequency(320, mySettings.osc2_octave, mySettings.osc2_note, mySettings.osc2_detune);
  subosc1hz = get_frequency(80, mySettings.sub_osc1_octave, mySettings.sub_osc1_note, mySettings.sub_osc1_detune);
  subosc2hz = get_frequency(80, mySettings.sub_osc2_octave, mySettings.sub_osc2_note, mySettings.sub_osc2_detune);
  //Serial.println(mySettings.sub_osc2_note);

  if (mySettings.osc1_waveform > 5) selectABTWaveTableOSC1(mySettings.osc1_waveform - 6);
  if (mySettings.osc1_waveform < 6) Osc1.begin(mySettings.osc1_level, osc1hz , waveIntsLook[mySettings.osc1_waveform] );
  Osc1.pulseWidth(mySettings.osc1_pulseWidth);

  SubOsc1.begin(mySettings.sub_osc1_level, subosc1hz , waveIntsLook[mySettings.sub_osc1_waveform] );
  SubOsc1.pulseWidth(mySettings.sub_osc1_pulseWidth);

  Osc2.begin(mySettings.osc2_level, osc2hz , waveIntsLook[mySettings.osc2_waveform]);
  Osc2.pulseWidth(mySettings.osc2_pulseWidth);

  SubOsc2.begin(mySettings.sub_osc2_level, subosc2hz , waveIntsLook[mySettings.sub_osc2_waveform] );
  SubOsc2.pulseWidth(mySettings.sub_osc2_pulseWidth);

  Osc1.phase(mySettings.osc1_phase);
  Osc2.phase(mySettings.osc2_phase);
  SubOsc1.phase(mySettings.sub_osc1_phase);
  SubOsc2.phase(mySettings.sub_osc2_phase);
  noise1.amplitude(mySettings.noise_level);
  bitcrusher1.bits(mySettings.Crusher_Bits);
  bitcrusher1.sampleRate(bitCrusherSampleRate[mySettings.Crusher_SampleRate]);
  //Serial.println(mySettings.Crusher_SampleRate);



  if (mySettings.filter_lfo_waveform > 5) selectABTWaveTableFilterLFO( bpmToHz(mySettings.filter_lfo_bpm), mySettings.filter_lfo_waveform - 6);
  if (mySettings.filter_lfo_waveform < 6) Filter_LFO.begin(1.0, bpmToHz(mySettings.filter_lfo_bpm) * beatMultiLookup[mySettings.filter_lfo_freq_bpm_multi] , waveIntsLook[mySettings.filter_lfo_waveform], mySettings.filter_lfo_cycles);
  Filter_LFO.pulseWidth(mySettings.filter_lfo_pulseWidth);
  Filter_LFO.phase(mySettings.filter_lfo_phase);

  if (mySettings.vol_lfo_waveform > 5) selectABTWaveTableVolumenLFO( bpmToHz(mySettings.vol_lfo_bpm), mySettings.vol_lfo_waveform - 6);
  if (mySettings.vol_lfo_waveform < 6) VOLUMEN_LFO.begin(0.5, bpmToHz(mySettings.vol_lfo_bpm) * beatMultiLookup[mySettings.vol_lfo_freq_bpm_multi] , waveIntsLook[mySettings.vol_lfo_waveform], mySettings.vol_lfo_cycles);
  VOLUMEN_LFO.pulseWidth(mySettings.vol_lfo_pulseWidth);
  VOLUMEN_LFO.phase(mySettings.vol_lfo_phase);

  if (mySettings.mod_lfo_waveform > 5) selectABTWaveTableModLFO( bpmToHz(mySettings.mod_lfo_bpm), mySettings.mod_lfo_waveform - 6);
  if (mySettings.mod_lfo_waveform < 6) MOD_LFO.begin(0.5, bpmToHz(mySettings.mod_lfo_bpm) * beatMultiLookup[mySettings.mod_lfo_freq_bpm_multi] , waveIntsLook[mySettings.mod_lfo_waveform], mySettings.mod_lfo_cycles);
  MOD_LFO.pulseWidth(mySettings.mod_lfo_pulseWidth);
  MOD_LFO.phase(mySettings.mod_lfo_phase);


  //################ Filter #######################
  // filter1 defaults
  filter1.frequency(mySettings.filter1Freq);
  filter1.resonance(mySettings.filter1Reso);
  filter1.octaveControl(mySettings.filter1Octave);

  filter2.frequency(mySettings.filter2Freq);
  filter2.resonance(mySettings.filter2Reso);
  filter2.octaveControl(mySettings.filter2Octave);

  FilterMixer.gain(0, 0.0);
  FilterMixer.gain(1, 0.0);
  FilterMixer.gain(2, 0.0);
  FilterMixer.gain(3, 0.0);

  F1F2CutoffInverter_ctrl.amplitude(mySettings.f1_f2_crossover_Modulation, 2);
  
  sgtl5000_1.lineInLevel(mySettings.freeDataInt1);
  
  AudioInterrupts();
  // ############### Pins ########################
}

// ############### dynamic Filter Frequency ##############
// -3 - 3 = off, + Values -> 60  return positiv Float value between 1.0f - 20.0f
// - Values -> -60 return a positiv Float value between 1.0f - 0.05f

// hmm 0.3 -20 scheint sinvller mit einer kleinenern abstufung um 1

float calculateDynamicFilterFrequency(int value) {
  float returnVal = 1.0f;
  float valF =  float(value);
  if (value < 0) {
    returnVal = 1.0f - pow((abs(valF) / 60.0f), 5);
    if (returnVal < 0.15) returnVal = 0.05;
  }
  if (value > 0) returnVal = 1.0f + (pow((valF / 60.0f), 5) * 50);

  return returnVal;
}


void setMCP42Value(byte value) {

  // hmm, bytes sollten eigentlich bei 0 und 255 überlaufen
  // trotzdem mal sichern :)
  if (value > 255) value = 255;
  if (value < 0) value = 0;
  return;
  
//  digitalWrite(MCP42XXXX_CS, LOW);
//  SPI.transfer(B00010001);     // B00010001 für poti 0 on MCP42XXXX
//  SPI.transfer(value);  //  0-255 value
//  digitalWrite(MCP42XXXX_CS, HIGH);

}

