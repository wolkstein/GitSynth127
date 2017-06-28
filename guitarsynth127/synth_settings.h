/*
 * System and preset settings
 */



uint16_t EEPROM_MAIN_SYSTEM_INFO_START = 0;
uint16_t EEPROM_MAIN_SYSTEM_SETTINGS_START = 20;

struct MainSystemInfo
{
  uint32_t revision;
  uint16_t initeeprom; 
};

MainSystemInfo myMainSystemInfo = {
  140617, // check this against eeprom revision change
  7777 // if this is not in eeprom we will write all settings in the eeprom (first time)
};

uint32_t MainSystemRevision = 140617; // change this if SystemSettings are changed

struct SystemSettings
{
  uint16_t Expression_Max;
  uint16_t Expression_Min;
  bool switchLivePresets;
  int8_t LivePresets[30]; // 0 lookup table für die Live Presets
  bool expandSystemSettings;
};

SystemSettings mySystemSettings = {
        740, // Expression_Max
        50,  // Expression_Min
        false, // use livepresets order
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // int8_t LivePresets[30]; // 0 lookup table für die Live Presets
        true    
};



int myMenuWindow = 0; //presets
int menuWindowSize = 141; // count of entrys -1 in "allSettings" 

float osc1hz = 0.0f; 
float osc2hz = 0.0f;
float subosc1hz = 0.0f;
float subosc2hz = 0.0f;

int8_t SequencerExtraNote = 0;

float G_Filter1_Active_Frequenz_Tracking = 1.0f;
float G_Filter2_Active_Frequenz_Tracking = 1.0f;
bool using_Filter1_Active_Frequenz_Tracking = false;
bool using_Filter2_Active_Frequenz_Tracking = false;

struct allSettings
{
  // preset name
  uint8_t            preset; // 1  Pos(0)
  
  int           filter1Freq; // 1 Gui
  float         filter1Reso; // 1 Gui
  float       filter1Octave; // 1 Gui
  
  float    filter_1_2_blend; // 1 Modees  -1 <--> 1  -1 = full filter1 1 = full filter 2, 0 both Gui
  float     f1_f2_crossover_Modulation;  // Gui
  
  byte          filter2Mode; // 1,  1 = Lowpass, 2 = Bandpass, 3 = Highpass Gui
  int           filter2Freq; // 1 Gui
  float         filter2Reso; // 1 Gui
  float       filter2Octave; // 1 Gui

  // envelop detection guitar settings 
  int16_t   sensingTreshold; // 0  benutzt für die alte callback funktion Kommt nicht in die Gui
  int16_t     sensingAttack; // 1 Attack setting Gui
  int16_t       sensingFall; // 1 sensingDamp !! benutzt für die alte callback funktion, steuert jetzt aber immer noch die releasetime. ist cool gui
  int16_t     sensingBounce; // 0 benutzt für die alte callback funktion 
  int16_t  sensingNoteOffCorner; // 0 benutzt im zusammenhang mit der alten callback funktion und wird zur zeit nicht verwendet gui
  // pitch detection line in
  int16_t pitchDetectTreshold; // 1 gui
  float  pitchDetectQuality; // 1  (Pos 10) gui
  
  
  //Syhth section
  // OSC 1                        Pos(11)
  int8_t      osc1_waveform; // 1  werte 0 - 7 0 = WAVEFORM_SINE usw
  float     osc1_pulseWidth; // 1  werte 0.0 - 1.0
  int8_t        osc1_octave; // 1  werte 1 - 7
  int8_t          osc1_note; // 1  werte 1 - 12
  int8_t         osc1_detune; // 1  werte -12 - (+)12  0 is no detune 1 = 4.999 cent detune
  float          osc1_level; // 1  werte 0.0 - 1.0
  int16_t            osc1_phase; // 1  werte 0 - 360

  // OSC 2
  int8_t      osc2_waveform;
  float     osc2_pulseWidth;
  int8_t        osc2_octave; // 1 - 7
  int8_t          osc2_note; // 1 - 12
  int8_t        osc2_detune; // -12 - 12  0 is no detune 1 = 4.999 cent detune
  float          osc2_level;
  int16_t         osc2_phase;
  
  // SubOsc 1 
  int8_t  sub_osc1_waveform;
  float sub_osc1_pulseWidth; 
  int8_t    sub_osc1_octave; // 1 - 7
  int8_t      sub_osc1_note; // 1 - 12
  int8_t    sub_osc1_detune; // -12 - 12  0 is no detune 1 = 4.999 cent detune
  float      sub_osc1_level;
  int16_t     sub_osc1_phase;

  // SubOsc 2 
  int8_t  sub_osc2_waveform;
  float sub_osc2_pulseWidth; 
  int8_t    sub_osc2_octave; // 1 - 7
  int8_t      sub_osc2_note; // 1 - 12
  int8_t    sub_osc2_detune; // -12 - 12  0 is no detune 1 = 4.999 cent detune
  float      sub_osc2_level;
  int16_t    sub_osc2_phase; // oscs giu fertig

  // Noise
  float         noise_level; // gui fertig
  
  int8_t       Crusher_Bits; //this defaults to passthrough.
  int8_t  Crusher_SampleRate; // this defaults to passthrough.
  float     Crusher_Volumen; // gui fertig
  

  //Control matrix envelop, filter, LFOS und ziele

  // MOD LFO
  int8_t     mod_lfo_waveform;
  float    mod_lfo_pulseWidth;
  float           mod_lfo_bpm; 
  int16_t  mod_lfo_freq_bpm_multi; 
  int16_t       mod_lfo_phase;
  bool   mod_lfo_T1_T4_FREQ_intensety_sync;
  float  mod_lfo_T1_OSC1_FREQ_intensety;
  float  mod_lfo_T2_OSC2_FREQ_intensety;
  float  mod_lfo_T3_SUB1_FREQ_intensety;
  float  mod_lfo_T4_SUB2_FREQ_intensety;
  bool   mod_lfo_T1_T4_PULSE_intensety_sync;
  float  mod_lfo_T1_OSC1_PULSE_intensety;
  float  mod_lfo_T2_OSC2_PULSE_intensety;
  float  mod_lfo_T3_SUB1_PULSE_intensety;
  float  mod_lfo_T4_SUB2_PULSE_intensety;
  bool    mod_lfo_attack_sync;
  float   mod_lfo_follow_envelope_attack;
  
  float envelopeFollowerAttackToMODLFOFrequenzy; // 0 - 10.0
  uint16_t mod_lfo_cycles;
  
  // Volumen LFO
  int8_t     vol_lfo_waveform;
  float    vol_lfo_pulseWidth;
  float           vol_lfo_bpm; 
  int16_t  vol_lfo_freq_bpm_multi; 
  int16_t       vol_lfo_phase;
  bool   vol_lfo_T1_T4_intensety_sync;
  float  vol_lfo_T1_OSC1_intensety;
  float  vol_lfo_T2_OSC2_intensety;
  float  vol_lfo_T3_SUBS_intensety;
  float  vol_lfo_T4_NOISE_intensety;
  bool    vol_lfo_attack_sync;
  float   vol_lfo_follow_envelope_attack;
  
  float envelopeFollowerAttackToVOLLFOFrequenzy; // 0 - 10.0
  uint16_t vol_lfo_cycles;
    
  // FilterLFO 
  int8_t  filter_lfo_waveform;
  float filter_lfo_pulseWidth;
  float       filter_lfo_bpm; // bpm
  int16_t filter_lfo_freq_bpm_multi; 
  int16_t    filter_lfo_phase;
  float filter_lfo_intensety;
  bool filter_lfo_attack_sync;
  
  float envelopeFollowerAttackToFilterLFOFrequenzy; // 0 - 10.0
  float envelopeFollowerAttackToFilterLFODepth; // 0 - 10.0
  uint16_t filter_lfo_cycles;
  
  // midi envelope
  int16_t      midi_attack; // 1
  int16_t           midi_hold; // 1
  int16_t         midi_decay; // 1
  int16_t         midi_sustain; // 1  % of midi note on envelope 
  int16_t         midi_release; // 1
  int8_t    midi_pichbandrange;
  
  // filter envelope
  int16_t         filter_delay;
  int16_t         filter_attack; // 1
  int16_t           filter_hold; // 1
  int16_t         filter_decay; // 1
  int16_t         filter_sustain; // 1  % of midi note on envelope 
  int16_t         filter_release; // 1

  float filter1envelopeIntensity; // -1.0 - 1.0 // gui
  bool gitarUseFilterADSR; // gui
  float gitarUseFilterADSRstaticVelocity;
  bool gitarUseEnvelopeADSR; // gui
    
  // Mixer sektion
  //OSC Master Volume
  float   SYNTH_Master_Volume; // 1 Gui
  bool   autoPeakCorrection;

  float instrumentLineInGainAnalog; // 0 werte 0 - 1 das ist später für den digitalen gain poti gedacht
  float instrumentBypassMix; // 0 werte 0 - 1 das ist später für den anlogen gitarren mix weg zum ausgang 1
   
  float  instrumentToSynth;  // 0  werte 0 - 1 ziel ist mixerAnalyse Port 2, gegebennenfalls im menue x2 umrechnen. achtung im momment ist dieser wert bei 1.9. komisch weil der input schon hoch ist im auge behalten!
  float  instrumentToUsb_1;  // 0 werte 0 - 1 ziel ist UsbOutputMixerLinks Port 0
  float  instrumentToUsb_2;  // 0 werte 0 - 1 ziel ist UsbOutputMixerRechts Port 0
  float  usbIn_1_ToSynth;    // 0 werte 0 - 1 ziel ist mixerAnalyse Port 0 
  float  usbIn_1_ToOutput1;  // 0 werte 0 - 1 ziel ist I2SOutputMixerChannel1 Port 1
  float  usbIn_1_ToOutput2;  // 0 werte 0 - 1 ziel ist I2SOutputMixerChannel2 port 1
  float  usbIn_2_ToOutput1;  // 0 werte 0 - 1 ziel ist I2SOutputMixerChannel1 port 2
  float  usbIn_2_ToOutput2;  // 0 werte 0 - 1 ziel ist I2SOutputMixerChannel2 port 2
  float  synthToUsbOutput1;  // 0 werte 0 - 1 ziel ist UsbOutputMixerLinks Port 3
  float  synthToUsbOutput2;  // 0 werte 0 - 1 ziel ist UsbOutputMixerRechts Port 3
  float  synthToOutput1;  // 0 werte 0 - 1 ziel ist I2SOutputMixerChannel1 Port 3
  float  synthToOutput2;  // 0 werte 0 - 1 ziel ist I2SOutputMixerChannel2 Port 3

  bool MasterClockToLfos; // 0 das soll nachher die masterclock zum beispiel durch tappen auf einen taster mit den lfos syncronisieren
  bool MasterClockToMODLfo; // 0 sync Mod Lfo
  bool MasterClockToVOLLfo; // 0 sync Vol Lfo
  bool MasterclockToFreqLFO; // 0 sync Freq Lfo

  bool switchLivePresets; // 0 auswählen ob die presetreihenvolge der Liveauswahl folgt
  int8_t LivePresets[30]; // 0 lookup table für die Live Presets
  bool LivePresetInUse[30]; // 0 schaltet die lookups frei und weiter
  byte midiChannel;
  
  // store the presetname
  char       presetName[10]; // 0 das wird noch für das menü und die option edit preset name verwendet

  // dies kommt in die settings nur zum testen hier
  // 0 = off, 1 - 4 entspricht osc 1 - osc 4
  uint8_t Filter1_Active_Frequenz_Tracking_Target;
  uint8_t Filter2_Active_Frequenz_Tracking_Target;
  int8_t int_Filter1_Active_Frequenz_Tracking;
  int8_t int_Filter2_Active_Frequenz_Tracking;


  uint8_t sequenceTimeSignature; // 7 = 1/4
  uint8_t sequeceSteps; // sequencer steps = 0 means sequencer is off
  int8_t sequenzerNotes[32];
  int8_t expressionPedalFunction;
  int8_t functionBtnFunction;

  bool       freeDataBool1;
  bool       freeDataBool2;
  bool       freeDataBool3;
  bool       freeDataBool4;
  float     freeDataFloat1; // filter lowpass freq. default 8000hz
  float     freeDataFloat2; // filter lowpass resonance default 0.7 
  float     freeDataFloat3; // filter highpass freq. default 80hz
  float     freeDataFloat4; // filter highpass resonance default 0.7 
  int32_t     freeDataInt1; // input gain 0 - 15
  int32_t     freeDataInt2; // Poti Function
  int32_t     freeDataInt3; // Exp Pedal Min-Mapping
  int32_t     freeDataInt4; // Exp pedal Max-Mapping // Da dies in den default settings auf 0 steht rechnen wir über all +127

  uint16_t  delay1_0Time;
  uint16_t  delay1_1Time;
  uint16_t  delay1_2Time;
  uint16_t  delay1_3Time;

  float delay1_EffectDryMixer;
  float delay1_EffectWetMixer;
  float delay1_FeedbackMixInput;
  float delay1_FeedbackMixOutput;

  float chorus1_EffectDryMixer;
  float chorus1_EffectWetMixer;
  int16_t chorus1_length;
  int16_t chorus1_n_chorus;

     // short *delayline,int d_length,int delay_offset,int d_depth,float delay_rate)
  float flange1_EffectDryMixer;
  float flange1_EffectWetMixer;
  int16_t flange1_d_length;
  int16_t flange1_delay_offset;
  int16_t flange1_d_depth;
  float flange1_delay_rate;
} ;

// Init Defaults
allSettings mySettings = {
                           1,      //presets               | on
                           1500,   // Filter1 Freq         | on
                           3.0f,  // Filter1 Reso         | on
                           6.3f,    // Filter1 Octave       | on
                           
                          -0.5f, //float    filter_1_2_blend; // 1 Modees  -1 <--> 1  -1 = full filter1 1 = full filter 2, 0 both
                          1.0f, //bool     f1_f2_crossover_Modulation; -1 crosover, 1 gleichgeschaltet
                          
                          3,    //byte          filter2Mode; // 1,  1 = Lowpass, 2 = Bandpass, 3 = Highpass
                          1500, //int           filter2Freq; // 1
                          3.0f, //         filter2Reso; // 1
                          6.3f,  //       filter2Octave; // 1
        
                           3600,   // sensingTreshold      | off 
                           1,      // sensingAttack        | on
                           503,    // sensingFall/Damp     | on
                           50,     // sensingBounce        | off
                           653,   // sensingNoteOffCorner | off
                           90,    // pitchDetectTreshold  | on
                           0.826f,    // pitchDetectQuality   | on

                           //Syhth section
                          // OSC 1 Pos(11)
                              1, // 1  werte 0 - 7 0 = WAVEFORM_SINE usw
                            0.5f, // 1  werte 0.0 - 1.0
                              0, // 1  werte 1 - 7 //164,814
                              0, // 1  werte 0 - 12
                              0, // 1  werte -12 - (+)12  0 is no detune 1 = 4.999 cent detune
                            0.3f, // 1  werte 0.0 - 1.0
                            150, // 1  werte 0 - 360
      
                            // OSC 2
                              3, // 1  werte 0 - 7 0 = WAVEFORM_SINE usw
                            0.5f, // 1  werte 0.0 - 1.0
                              1, // 1  werte 1 - 7 //
                              0, // 1  werte 0 - 12
                              0, // 1  werte -12 - (+)12  0 is no detune 1 = 4.999 cent detune
                            0.0, // 1  werte 0.0 - 1.0
                             90, // 1  werte 0 - 360
                                
                          // SubOsc 1 
                              0, // 1  werte 0 - 7 0 = WAVEFORM_SINE usw
                            0.5f, // 1  werte 0.0 - 1.0
                             -1, // 1  werte 1 - 7 //82,4069
                              0, // 1  werte 0 - 12
                              0, // 1  werte -12 - (+)12  0 is no detune 1 = 4.999 cent detune
                            0.0f, // 1  werte 0.0 - 1.0
                             30, // 1  werte 0 - 360
    
                           // SubOsc 2 
                              0, // 1  werte 0 - 7 0 = WAVEFORM_SINE usw
                            0.5f, // 1  werte 0.0 - 1.0
                             -1, // 1  werte -3 - 3 //82,4069
                              0, // 1  werte -12 - 12
                              0, // 1  werte -12 - (+)12  0 is no detune 1 = 4.999 cent detune
                            0.0f, // 1  werte 0.0 - 1.0
                            270, // 1  werte 0 - 360
    
                             // Noise
                            0.0,
                             // Bitcrusher in
                             16, // 1 werte 16 -2
                              0, // 1 werte 44100, 22050, 11025 .. absolute min is 345
                            0.0f, // 1 werte 0.0 -1.0

                            //Control Matrix
                            // MOD LFO
                              0, // 1 int8_t     mod_lfo_waveform;
                            0.0f, // 1 float    mod_lfo_pulseWidth;
                           90.0f, // 1           mod_lfo_bpm; 
                              7, // 1 int16_t  mod_lfo_freq_bpm_multi; 
                              0, // 1 int16_t       mod_lfo_phase;
                          false, // 1 bool   mod_lfo_T1_T4_FREQ_intensety_sync;
                            0.0f, // 1 float  mod_lfo_T1_OSC1_RREQ_intensety;
                            0.0f, // 1 float  mod_lfo_T2_OSC2_FREQ_intensety;
                            0.0f, // 1  float  mod_lfo_T3_SUB1_FREQ_intensety;
                            0.0f, // 1  float  mod_lfo_T4_SUB2_FREQ_intensety;
                          false, // 1  bool   mod_lfo_T1_T4_PULSE_intensety_sync;
                            0.0f, // 1  float  mod_lfo_T1_OSC1_PULSE_intensety;
                            0.0f, // 1  float  mod_lfo_T2_OSC2_PULSE_intensety;
                            0.0f, // 1  float  mod_lfo_T3_SUB1_PULSE_intensety;
                            0.0f, // 1  float  mod_lfo_T4_SUB2_PULSE_intensety;
                          false, // 1   bool    mod_lfo_attack_sync;
                            0.0f, // 1  float   mod_lfo_follow_envelope_attack;
                              
                            0.0f, // 1  float envelopeFollowerAttackToMODLFOFrequenzy; // 0 - 10.0
                               0, // 1 mod lfo cycles
                               
                            // Volumen LFO
                              0, // 1  werte 0 - 7 0 = WAVEFORM_SINE usw
                            0.5f, // 1  werte 0.0 - 1.0     float    vol_lfo_pulseWidth;
                           90.0f, // 1  werte Frequenzy in Bbm       float           vol_lfo_bpm; 
                              7, // 1 werte für bpm multiplying      int16_t  vol_lfo_freq_bpm_multi; 
                              0, // 1  werte 0 - 360     int16_t       vol_lfo_phase;
                          false, // 1 war und falsch, wenn gesetzt folgen T2 T3 und T4 den werten von T1      bool   vol_lfo_T1_T4_intensety_sync;
                            0.0f, //     float  vol_lfo_T1_OSC1_intensety;
                            0.0f, // 1        float  vol_lfo_T2_OSC2_intensety;
                            0.0f, // 1       float  vol_lfo_T3_SUBS_intensety;
                            0.0f, // 1       float  vol_lfo_T4_NOISE_intensety;
                          false, // 1       vol_lfo_attack_sync
                            0.0f, // 1 vol_lfo_follow_envelope_attack
                               
                            0.0f, // 1  werte -10.0 - 10.0 , werte kleiner 1.0 werden als hz * 1/x = 0.y verwendet, positive als hz * x = y.  attack envelop to VolLfo frequency float envelopeFollowerAttackToVOLLFOFrequenzy; // 0 - 10.0
                               0, // Volumen lfo cycles
                            
                            // Filter LFO
                              0, // 1  werte 0 - 7 0 = WAVEFORM_SINE usw
                            0.5f, // 1  werte 0.0 - 1.0
                           90.0f, // 1  werte Frequenzy in Bbm
                              7, // 1 werte für bpm multiplying 
                              0, // 1  werte 0 - 360 
                            0.0f, // 1  werte -1.0 - 1.0 filter lfo depth to filter
                            false, // 1 falsch war Filter-lfo sync to attack
                                                      
                            0.0f, // 1  werte -10.0 - 10.0 , werte kleiner 1.0 werden als hz * 1/x = 0.y verwendet, positive als hz * x = y.  attack envelop to FilterLfo frequency 
                            0.0f, // 1 envelopeFollowerAttackToFilterLFODepth
                              0, // Filter lfo cycles
                            
                            // midi envelope
                            1, // 1 werte 1 ms
                           30, // 1 ms
                           300, // 1 ms
                           70, // 1  % max 100
                           300, // 1 ms
                           12, // 1  half tones
                            // filter envelope
                             1,  // int16_t         filter_delay;
                             1, //  int16_t         filter_attack; // 1
                             30, // int16_t           filter_hold; // 1
                            300, // int16_t         filter_decay; // 1
                             70, //  int16_t         filter_sustain; // 1  % of midi note on envelope 
                            300, //        filter_release; // 1                           

                            0.0f, // 1  werte -1.0 - 1.0 attack envelop to filter
                            true, // gitarre nutzt filteradsr
                            0.0f,
                            true, // gitarre nutzt evelope adsr
                            
                            // MIXER SEKTION OSC Master Volumen
                            0.2f, // 1 OSC Master Volumen OSCMixer input 1-4
                          false,  // auto correct peaks and lower master volumen

                             0.0f, // 0 float instrumentLineInGainAnalog; // 0 werte 0 - 1 das ist später für den digitalen gain poti gedacht
                           127.0f, // 0float instrumentBypassMix; // 0 werte 0 - 1 das ist später für den anlogen gitarren mix weg zum ausgang 1
   
                            0.95f, // zz auf 1.9 also mal zwei nehmen  float  instrumentToSynth;  // 0  werte 0 - 1 ziel ist mixerAnalyse Port 2, gegebennenfalls im menue x2 umrechnen. achtung im momment ist dieser wert bei 1.9. komisch weil der input schon hoch ist im auge behalten!
                            0.9f,  // float  instrumentToUsb_1;  // 0 werte 0 - 1 ziel ist UsbOutputMixerLinks Port 0
                            0.0f,  // float  instrumentToUsb_2;  // 0 werte 0 - 1 ziel ist UsbOutputMixerRechts Port 0
                            0.0f, // float  usbIn_1_ToSynth;    // 0 werte 0 - 1 ziel ist mixerAnalyse Port 0 
                            0.0f,  //float  usbIn_1_ToOutput1;  // 0 werte 0 - 1 ziel ist I2SOutputMixerChannel1 Port 1
                            0.0f,  //float  usbIn_1_ToOutput2;  // 0 werte 0 - 1 ziel ist I2SOutputMixerChannel2 port 1
                            0.9f,  //float  usbIn_2_ToOutput1;  // 0 werte 0 - 1 ziel ist I2SOutputMixerChannel1 port 2
                            0.9f,  //float  usbIn_2_ToOutput2;  // 0 werte 0 - 1 ziel ist I2SOutputMixerChannel2 port 2
                            0.9f,  //float  synthToUsbOutput1;  // 0 werte 0 - 1 ziel ist UsbOutputMixerLinks Port 3
                            0.9f,  //float  synthToUsbOutput2;  // 0 werte 0 - 1 ziel ist UsbOutputMixerRechts Port 3
                            0.9f,  //float  synthToOutput1;  // 0 werte 0 - 1 ziel ist I2SOutputMixerChannel1 Port 3
                            0.9f,  //float  synthToOutput2;  // 0 werte 0 - 1 ziel ist I2SOutputMixerChannel2 Port 3

                            true, // bool MasterClockToLfos; // 0 das soll nachher die masterclock zum beispiel durch tappen auf einen taster mit den lfos syncronisieren
                           false, // bool MasterClockToMODLfo; // 0 sync Mod Lfo
                           false, // bool MasterClockToVOLLfo; // 0 sync Vol Lfo
                           false, // bool MasterclockToFreqLFO; // 0 sync Freq Lfo

                           false, // bool switchLivePresets; // 0 auswählen ob die presetreihenvolge der Liveauswahl folgt
                           {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29}, // int8_t LivePresets[30]; // 0 lookup table für die Live Presets
                           {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //bool LivePresetInUse[30]; // 0 schaltet die lookups frei und weiter
                          
                              1, // true
                    "Default  ", //store the presetname not used in menu structure
                              0, // Filter1 activ tracking target
                              0, // Filter2 active tracking target
                              0, // Filter 1 tracking divider as int
                              0, // Filter 2 Tracking divider as int


                              7, // Sequencer Time Signature 7 = 1/4
                              0, // sequencer steps = 0 means sequencer is off
                              {  // sequencer notes
                                0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0     
                              },
                              0, //int8_t expressionPedalFunction;
                              0, //int8_t functionBtnFunction;
                                      
                          false, //  bool       freeDataBool1;
                          false, //  bool       freeDataBool2;
                          false, //  bool       freeDataBool3;
                          false, //  bool       freeDataBool4;
                        8000.0f, //  float     freeDataFloat1;
                           0.71f, //  float     freeDataFloat2;  
                          80.0f, //  float     freeDataFloat3;
                           0.71f, //  float     freeDataFloat4;
                              5, //  int32_t     freeDataInt1;
                              0, // int32_t     freeDataInt2; poti function
                              0, // int32_t     freeDataInt3;
                              0,  // int32_t     freeDataInt4;


                              0, //   uint16_t  delay1_0Time;
                              0, //   uint16_t  delay1_1Time;
                              0, //   uint16_t  delay1_2Time;
                              0, //   uint16_t  delay1_3Time;
                              
                           1.0f, //  float delay1_EffectDryMixer;
                           0.0f, //  float delay1_EffectWetMixer;
                           0.0f, //  float delay1_FeedbackMixInput;
                           0.0f, //  float delay1_FeedbackMixOutput;
                              
                           1.0f, //  float chorus1_EffectDryMixer;
                           0.0f, //  float chorus1_EffectWetMixer;
                              0, //  int16_t chorus1_length;
                              0, //  int16_t chorus1_n_chorus;
                              
                           1.0f, //  float flange1_EffectDryMixer;
                           0.0f, //  float flange1_EffectWetMixer;
                              0, //  int16_t flange1_d_length;
                              0, //  int16_t flange1_delay_offset;
                              0, //  int16_t flange1_d_depth;
                           0.0f //  float flange1_delay_rate;                           
                          };


allSettings mySettings_DataMigration = mySettings;
                         
//             10 sektion 1, 29 synths,crusher, control matrix,midi envelope, osc mastervolumen, presetname
bool hidingSettings[143] = {
                             true,
                             true,
                             true,
                             true,
                             true, //presets               | on
                             true, // Filter1 Freq         | on
                             true, // Filter1 Reso         | on
                             true, // Filter1 Octave       | on
                             true,
                             true,
                             true,
                             true,
                             true,
                             true,
                             false,// sensingTreshold      | off 
                             true, // sensingAttack        | on
                             true, // sensingFall          | on
                             false,// sensingBounce        | off
                             true,// sensingNoteOffCorner | off
                             true, // pitchDetectTreshold  | on
                             true,  // pitchDetectQuality   | on
                             // synts
                             true,
                             true,
                             true,
                             true,                            
                             true,
                             true,
                             true,
                             true,
                             true,
                             true,
                             true,
                             true,
                             true,
                             true,
                             true,
                             true,                            
                             true, 
                             true,
                             true,
                             true,
                             true,
                             true,
                             true,
                             true,
                             true,
                             true,
                             true,
                             true,
                             true,
                             // bitcrusher
                             true,
                             true,
                             true,
                             //Control Matrix
                             // MOD LFO
                             true,
                             true,
                             true,
                             true,                            
                             true,
                             true,
                             true,
                             true,
                             true,
                             true,
                             true,
                             true,
                             true,
                             true,
                             true,
                             true,                            
                             true, 
                             true,
                                     
                             //Vol LFO
                             true, 
                             true,
                             true,
                             true,
                             true,
                             true,
                             true,
                             true,
                             true,
                             true,
                             true,
                             true,
                             true,
                             //Filter lfo
                             true,
                             true,
                             true,
                             true,
                             true,
                             true,
                             true,
                             true,
                             true,
                             // midi envelope
                             true,
                             true,
                             true,
                             true,
                             true,
                             true,
                             // filter envelope
                             true,  // int16_t         filter_delay;
                             true, //  int16_t         filter_attack; // 1
                             true, // int16_t           filter_hold; // 1
                             true, // int16_t         filter_decay; // 1
                             true, //  int16_t         filter_sustain; // 1  % of midi note on envelope 
                             true, //        filter_release; // 1  
                             // OSC Master volumen
                             true,
                             true,

                             true, // midichannel hier
                             true,
                             true,
                             true,
                             true,
                             true,
                             true,
                             true,
                             true,
                             true,
                             true,
                             true,
                             true,
                             true,
                             true,
                             true,
                             true,
                             true,
                             true,
                             true,
                             true,
                             true,
                             true,
                             true,
                             true,
                             true,
                             true,
                             true,
                             true,
                             true,
                             // system
                             true, // EXP- Min            (hidingSettings[137])
                             true, // Exp- Max            (hidingSettings[138])
                             true, // Use Live-Presets    (hidingSettings[139])
                             true, // edit Live-Presets        (hidingSettings[140])
                             true, // expand System menue
                             false // last unused value                                         
                           };
                           
bool hideSettings = true;// einstellen ob unbenutzte gezeigt werden sollen

const char* FunctionBtnLookup[4] =
{
    "OFF           ",
    "Freeze Note   ",
    "Mute Synth T-U",
    "Mute Synth T-D"
};

const char* expPedalFunctionLookup[23] =
{
    "OFF           ",
    "F1 CutOff     ",
    "F1 Reso       ",
    "F1 Octave     ",
    "F2 CutOff     ",
    "F2 Reso       ",
    "F2 Octave     ",  
    "Filter Mix    ",
    "Env F-Cut     ",
    "ADSR Attack   ",
    "ADSR Hold     ",
    "ADSR Decay    ",
    "ADSR Sustain  ",
    "ADSR Release  ",
    "F-ADSR Delay  ",   
    "F-ADSR Attack ",
    "F-ADSR Hold   ",
    "F-ADSR Decay  ",
    "F-ADSR Sustain",
    "F-ADSR Release",
    "Input Gain    ",
    "Pitch Up      ",
    "Pitch down    "
};

const int waveIntsLook[6] =
{
    WAVEFORM_SINE,
    WAVEFORM_SAWTOOTH,
    WAVEFORM_SAWTOOTH_REVERSE,
    WAVEFORM_SQUARE,
    WAVEFORM_TRIANGLE,
    WAVEFORM_PULSE
};

const float bitCrusherSampleRate[7] =
{
  44100.0f,
  22050.0f,
  11025.0f,
  5512.5f,
  2765.25f,
  1378.125f,
  689.0625f,
};

const char* waveNamesLook[6] =
{
    "SINE ",
    "SAW  ",
    "SAw-R",
    "SQARE",
    "TRIAN",
    "PULSE"                 
};
const char* LFOwaveNamesLook[12] =
{
    "SINE ",
    "SAW  ",
    "SAw-R",
    "SQARE",
    "TRIAN",
    "PULSE",
    "RA-UP",
    "R-DOW",
    "ST-UP",
    "S-DOW",
    "HUI  ",
    "HOI  "             
};
const float beatMultiLookup[21] = 
{
  // Punkt. | Gerade | Triolige
    0.166f,     0.25f,     0.425f,  //  1
    0.333f,     0.5f,      0.75f,   // 1/2
    0.667f,     1.0f,      1.5f,    // 1/4
    1.333f,     2.0f,      3.0f,    // 1/8
    2.667f,     4.0f,      6.0f,    // 1/16
    5.333f,     8.0f,     12.0f,    // 1/32
   10.667f,    16.0f,     24.0f    // 1/32
};

const char* beatMultiNamesLookup[18+3] =
{
    "*   1 ", "    1 ", "T   1 ",
    "* 1/2 ", "  1/2 ", "T 1/2 ",
    "* 1/4 ", "  1/4 ", "T 1/4 ",     
    "* 1/8 ", "  1/8 ", "T 1/8 ",
    "* 1/16", "  1/16", "T 1/16",
    "* 1/32", "  1/32", "T 1/32",
    "* 1/64", "  1/64", "T 1/64"   
};

// midi helper

const float tune_frequencies2_PGM[128] =
{
    8.1758f,    8.6620f,    9.1770f,    9.7227f,    10.3009f,    10.9134f,    11.5623f,    12.2499f,
    12.9783f,   13.7500f,   14.5676f,   15.4339f,   16.3516f,    17.3239f,    18.3540f,    19.4454f,
    20.6017f,   21.8268f,   23.1247f,   24.4997f,   25.9565f,    27.5000f,    29.1352f,    30.8677f,
    32.7032f,   34.6478f,   36.7081f,   38.8909f,   41.2034f,    43.6535f,    46.2493f,    48.9994f,
    51.9131f,   55.0000f,   58.2705f,   61.7354f,   65.4064f,    69.2957f,    73.4162f,    77.7817f,
    82.4069f,   87.3071f,   92.4986f,   97.9989f,   103.8262f,   110.0000f,   116.5409f,   123.4708f,
    130.8128f,  138.5913f,  146.8324f,  155.5635f,  164.8138f,   174.6141f,   184.9972f,   195.9977f,
    207.6523f,  220.0000f,  233.0819f,  246.9417f,  261.6256f,   277.1826f,   293.6648f,   311.1270f,
    329.6276f,  349.2282f,  369.9944f,  391.9954f,  415.3047f,   440.0000f,   466.1638f,   493.8833f,
    523.2511f,  554.3653f,  587.3295f,  622.2540f,  659.2551f,   698.4565f,   739.9888f,   783.9909f,
    830.6094f,  880.0000f,  932.3275f,  987.7666f,  1046.5023f,  1108.7305f,  1174.6591f,  1244.5079f,
    1318.5102f, 1396.9129f, 1479.9777f, 1567.9817f, 1661.2188f,  1760.0000f,  1864.6550f,  1975.5332f,
    2093.0045f, 2217.4610f, 2349.3181f, 2489.0159f, 2637.0205f,  2793.8259f,  2959.9554f,  3135.9635f,
    3322.4376f, 3520.0000f, 3729.3101f, 3951.0664f, 4186.0090f,  4434.9221f,  4698.6363f,  4978.0317f,
    5274.0409f, 5587.6517f, 5919.9108f, 6271.9270f, 6644.8752f,  7040.0000f,  7458.6202f,  7902.1328f,
    8372.0181f, 8869.8442f, 9397.2726f, 9956.0635f, 10548.0818f, 11175.3034f, 11839.8215f, 12543.8540f
};

const float velocity2amplitude[128] = {
0.001f,
0.01778f,0.01966f,0.02164f,0.02371f,0.02588f,0.02814f,0.03049f,0.03294f,0.03549f,0.03812f,
0.04086f,0.04369f,0.04661f,0.04963f,0.05274f,0.05594f,0.05924f,0.06264f,0.06613f,0.06972f,
0.07340f,0.07717f,0.08104f,0.08500f,0.08906f,0.09321f,0.09746f,0.10180f,0.10624f,0.11077f,
0.11539f,0.12011f,0.12493f,0.12984f,0.13484f,0.13994f,0.14513f,0.15042f,0.15581f,0.16128f,
0.16685f,0.17252f,0.17828f,0.18414f,0.19009f,0.19613f,0.20227f,0.20851f,0.21484f,0.22126f,
0.22778f,0.23439f,0.24110f,0.24790f,0.25480f,0.26179f,0.26887f,0.27605f,0.28333f,0.29070f,
0.29816f,0.30572f,0.31337f,0.32112f,0.32896f,0.33690f,0.34493f,0.35306f,0.36128f,0.36960f,
0.37801f,0.38651f,0.39511f,0.40381f,0.41260f,0.42148f,0.43046f,0.43953f,0.44870f,0.45796f,
0.46732f,0.47677f,0.48631f,0.49595f,0.50569f,0.51552f,0.52544f,0.53546f,0.54557f,0.55578f,
0.56609f,0.57648f,0.58697f,0.59756f,0.60824f,0.61902f,0.62989f,0.64085f,0.65191f,0.66307f,
0.67432f,0.68566f,0.69710f,0.70863f,0.72026f,0.73198f,0.74380f,0.75571f,0.76771f,0.77981f,
0.79201f,0.80430f,0.81668f,0.82916f,0.84174f,0.85440f,0.86717f,0.88003f,0.89298f,0.90602f,
0.91917f,0.93240f,0.94573f,0.95916f,0.97268f,0.98629f,1.00000f
};

float get_frequency( float myfrequency, int octave , int note, int cent){

// 
  // oktaven ++
  if(octave > 0){
    for(int i = octave; i > 0; i--){
      myfrequency *= 2.0f;
    }
  }

  // oktaven --
  if(octave < 0){
    for(int i = octave; i < 0; i++){
      myfrequency /= 2.0f;
    }
  }
  
  // halbtöne ++  
  if(note > 0) {
    for(int i = note; i > 0; i--){
       myfrequency *= 1.059463f;
    }
  }

  // halbtöne --
  if(note < 0){
    for(int i = note; i < 0; i++){
      myfrequency *= 0.943874f;
    }
  }

  // cent ++  
  if(cent > 0) {
    for(int i = cent; i > 0; i--){
       myfrequency *= 1.0048251f;
    }
  }

  // halbtöne --
  if(cent < 0){
    for(int i = cent; i < 0; i++){
      myfrequency *= 0.9951981;
    }
  }

   return myfrequency; 
}

float bpmToHz(float bpm){
  return bpm/60.0;
  
}



void calculateFilterMixer(){

  float gainF1 = 0.0f;
  float gainF2LowPass = 0.0f;
  float gainF2BandPass = 0.0f;
  float gainF2HighPass = 0.0f;

// beide auf 50 und dann 
  float myKopfKnackerLeft = 0.0f;
  float myKopfKnackerRight = 0.0f;
  
  if( mySettings.filter_1_2_blend == 0.0){ // neutral
    myKopfKnackerLeft = 0.5;
    myKopfKnackerRight = 0.5;
  }
  if( mySettings.filter_1_2_blend < 0.0){ 
      myKopfKnackerLeft = 0.5 + abs(mySettings.filter_1_2_blend);
      myKopfKnackerRight = 0.5 - abs(mySettings.filter_1_2_blend); 
  }
  if( mySettings.filter_1_2_blend > 0.0){ 
      myKopfKnackerLeft = 0.5 - abs(mySettings.filter_1_2_blend);
      myKopfKnackerRight = 0.5 + abs(mySettings.filter_1_2_blend); 
  }
  myKopfKnackerLeft *=1.5;
  myKopfKnackerRight *=1.5;
  if(myKopfKnackerLeft > 1.0)myKopfKnackerLeft = 1.0;
  if(myKopfKnackerRight > 1.0)myKopfKnackerRight = 1.0;
  
 
  if(mySettings.filter2Mode == 1){// Lowpass
    gainF2LowPass = myKopfKnackerRight;
    gainF2BandPass = 0.0;
    gainF2HighPass = 0.0;
  }
  if(mySettings.filter2Mode == 2){// Bandpass
    gainF2LowPass = 0.0;
    gainF2BandPass= myKopfKnackerRight;
    gainF2HighPass = 0.0;
  }
  if(mySettings.filter2Mode == 3){// Highpass
    gainF2LowPass = 0.0;
    gainF2BandPass = 0.0;
    gainF2HighPass = myKopfKnackerRight;
  }
  
  gainF1 = myKopfKnackerLeft;
  
  FilterMixer.gain(0, gainF1);
  FilterMixer.gain(1, gainF2LowPass);
  FilterMixer.gain(2, gainF2BandPass);
  FilterMixer.gain(3, gainF2HighPass);
  
  if(DEB_MENUE) Serial.printf(" F1 %f <> %f F2 || Value: %f | Filter1 in 0: %f | Filter2 Lowpass in 1: %f | Filter2 Bandpass in 2: %f | Filter2 Highpass: %f \n",
                              myKopfKnackerLeft ,myKopfKnackerRight , mySettings.filter_1_2_blend,
                              gainF1,gainF2LowPass,gainF2BandPass,gainF2HighPass);
}


