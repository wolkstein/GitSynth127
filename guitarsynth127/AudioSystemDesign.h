// MIXER 4
#define ABT_MAX_FREQ 600 
#define MIXER4GAIN_MIN 0.0f
#define MIXER4GAIN_MAX 2.0f

// AudioFilterStateVariable
#define C_FILTER_FREQ_MIN 0
#define C_FILTER_FREQ_MAX 20000
#define C_FILTER_Q_MIN 0.7f
#define C_FILTER_Q_MAX 25.0f
#define C_FILTER_OCT_MIN 0.0f
#define C_FILTER_OCT_MAX 7.0f

// Guitar Sensing
#define G_E_SENS_FALL_MIN 0
#define G_E_SENS_FALL_MAX 1000
#define G_E_SENS_ATTACK_MIN 1
#define G_E_SENS_ATTACK_MAX 3000

// Guitar Pitch Detect
#define G_P_D_TRESHOLD_MIN 0
#define G_P_D_TRESHOLD_MAX 25000
#define G_P_D_QUALITY_MIN 0.0f
#define G_P_D_QUALITY_MAX 0.99f



// GUItool: begin automatically generated code
AudioSynthWaveform       MOD_LFO;        //xy=895,1154
AudioSynthWaveform       VOLUMEN_LFO;    //xy=1048,1223
AudioAnalyzePeak         FOLLOW_MOD_LFO; //xy=1134,1154
AudioInputUSB            USBin;          //xy=1168,326
AudioInputI2S            i2s2;           //xy=1175,497
AudioSynthWaveformDc     VOL_LFO_NOISE_DEPTH; //xy=1219,1398
AudioSynthWaveformDc     VOL_LFO_SUBS_DEPTH; //xy=1220,1359
AudioSynthWaveformDc     VOL_LFO_OSC1_DEPTH; //xy=1222,1281
AudioSynthWaveformDc     VOL_LFO_OSC2_DEPTH; //xy=1222,1321
AudioAnalyzePeak         FOLLOW_VOL_LFO; //xy=1273,1223
AudioSynthWaveform       SubOsc2;        //xy=1327,1013
AudioEffectBitcrusher    bitcrusher1;    //xy=1328,1064
AudioSynthWaveform       SubOsc1;        //xy=1345,963
AudioAnalyzeRMS          lineinRms;      //xy=1369,406
AudioSynthNoiseWhite     noise1;         //xy=1382,1115
AudioSynthWaveform       Osc2;           //xy=1390,912
AudioMixer4              mixerAnalyse;   //xy=1401,686
AudioSynthWaveform       Osc1;           //xy=1455,865
AudioSynthWaveformDc     Filter_LFO_depth; //xy=1545,1412
AudioMixer4              SubOscMixer;    //xy=1557,999
AudioSynthWaveform       Filter_LFO;     //xy=1561,1371
AudioAnalyzeNoteFrequency notefreq;       //xy=1649,633
AudioSynthWaveformDc     filter1envelopeIntensity; //xy=1658,1505
AudioAnalyzeRMS          rmsMixerAnalyseOut; //xy=1688,730
AudioAnalyzeAttack       myAttackDetector; //xy=1716,682
AudioSynthWaveformDc     filter1envelopeGenerator; //xy=1718,1632
AudioSynthWaveformDc     VCAenvelopeGenerator; //xy=1720,1677
AudioSynthWaveformDc     F1F2CutoffInverter_ctrl; //xy=1727,1212
AudioEffectMultiply      VOL_LFO_OSC1_CTR; //xy=1757,822
AudioEffectMultiply      VOL_LFO_OSC2_CTR; //xy=1757,912
AudioEffectMultiply      Filter_LFO_multiply; //xy=1764,1377
AudioEffectMultiply      VOL_LFO_NOISE_CTR; //xy=1770,1097
AudioEffectMultiply      VOL_LFO_SUBS_CTR; //xy=1780,1006
AudioEffectMultiply      multiply1;      //xy=1934,1474
AudioEffectMultiply      F1F2CutoffInverter; //xy=1993,1169
AudioMixer4              OscMainMix;     //xy=2012,977
AudioMixer4              Filter1FrequenzControlMixer; //xy=2036,1294
AudioAnalyzePeak         FCMAnalyse;     //xy=2085,1616
AudioAnalyzePeak         filterAttackAnalyse; //xy=2198,1538
AudioFilterStateVariable filter2;        //xy=2232,982
AudioFilterStateVariable filter1;        //xy=2237,1061
AudioAnalyzePeak         peakFilter1Out; //xy=2393,796
AudioMixer4              FilterMixer;    //xy=2479,976
AudioEffectMultiply      VCAGuitarEnvelope; //xy=2654,1065
AudioMixer4              I2SOutputMixerChannel2; //xy=3040,533
AudioMixer4              I2SOutputMixerChannel1; //xy=3041,451
AudioMixer4              UsbOutputMixerRechts; //xy=3046,333
AudioMixer4              UsbOutputMixerLinks; //xy=3047,260
AudioOutputUSB           USBout;         //xy=3311,292
AudioOutputI2S           i2s1;           //xy=3333,463
AudioConnection          patchCord1(MOD_LFO, FOLLOW_MOD_LFO);
AudioConnection          patchCord2(VOLUMEN_LFO, FOLLOW_VOL_LFO);
AudioConnection          patchCord3(USBin, 0, mixerAnalyse, 0);
AudioConnection          patchCord4(USBin, 0, I2SOutputMixerChannel1, 1);
AudioConnection          patchCord5(USBin, 0, I2SOutputMixerChannel2, 1);
AudioConnection          patchCord6(USBin, 1, I2SOutputMixerChannel1, 2);
AudioConnection          patchCord7(USBin, 1, I2SOutputMixerChannel2, 2);
AudioConnection          patchCord8(i2s2, 0, mixerAnalyse, 2);
AudioConnection          patchCord9(i2s2, 0, lineinRms, 0);
AudioConnection          patchCord10(i2s2, 0, I2SOutputMixerChannel1, 0);
AudioConnection          patchCord11(i2s2, 0, UsbOutputMixerLinks, 0);
AudioConnection          patchCord12(i2s2, 0, I2SOutputMixerChannel2, 0);
AudioConnection          patchCord13(i2s2, 0, UsbOutputMixerRechts, 0);
AudioConnection          patchCord14(VOL_LFO_NOISE_DEPTH, 0, VOL_LFO_NOISE_CTR, 1);
AudioConnection          patchCord15(VOL_LFO_SUBS_DEPTH, 0, VOL_LFO_SUBS_CTR, 1);
AudioConnection          patchCord16(VOL_LFO_OSC1_DEPTH, 0, VOL_LFO_OSC1_CTR, 1);
AudioConnection          patchCord17(VOL_LFO_OSC2_DEPTH, 0, VOL_LFO_OSC2_CTR, 1);
AudioConnection          patchCord18(SubOsc2, 0, SubOscMixer, 1);
AudioConnection          patchCord19(bitcrusher1, 0, SubOscMixer, 2);
AudioConnection          patchCord20(SubOsc1, 0, SubOscMixer, 0);
AudioConnection          patchCord21(noise1, 0, VOL_LFO_NOISE_CTR, 0);
AudioConnection          patchCord22(Osc2, 0, VOL_LFO_OSC2_CTR, 0);
AudioConnection          patchCord23(mixerAnalyse, myAttackDetector);
AudioConnection          patchCord24(mixerAnalyse, rmsMixerAnalyseOut);
AudioConnection          patchCord25(mixerAnalyse, notefreq);
AudioConnection          patchCord26(mixerAnalyse, bitcrusher1);
AudioConnection          patchCord27(Osc1, 0, VOL_LFO_OSC1_CTR, 0);
AudioConnection          patchCord28(Filter_LFO_depth, 0, Filter_LFO_multiply, 1);
AudioConnection          patchCord29(SubOscMixer, 0, VOL_LFO_SUBS_CTR, 0);
AudioConnection          patchCord30(Filter_LFO, 0, Filter_LFO_multiply, 0);
AudioConnection          patchCord31(filter1envelopeIntensity, 0, multiply1, 1);
AudioConnection          patchCord32(filter1envelopeGenerator, 0, multiply1, 0);
AudioConnection          patchCord33(filter1envelopeGenerator, filterAttackAnalyse);
AudioConnection          patchCord34(VCAenvelopeGenerator, 0, VCAGuitarEnvelope, 1);
AudioConnection          patchCord35(VCAenvelopeGenerator, FCMAnalyse);
AudioConnection          patchCord36(F1F2CutoffInverter_ctrl, 0, F1F2CutoffInverter, 1);
AudioConnection          patchCord37(VOL_LFO_OSC1_CTR, 0, OscMainMix, 0);
AudioConnection          patchCord38(VOL_LFO_OSC2_CTR, 0, OscMainMix, 1);
AudioConnection          patchCord39(Filter_LFO_multiply, 0, Filter1FrequenzControlMixer, 2);
AudioConnection          patchCord40(VOL_LFO_NOISE_CTR, 0, OscMainMix, 3);
AudioConnection          patchCord41(VOL_LFO_SUBS_CTR, 0, OscMainMix, 2);
AudioConnection          patchCord42(multiply1, 0, Filter1FrequenzControlMixer, 3);
AudioConnection          patchCord43(F1F2CutoffInverter, 0, filter2, 1);
AudioConnection          patchCord44(OscMainMix, 0, filter1, 0);
AudioConnection          patchCord45(OscMainMix, 0, filter2, 0);
AudioConnection          patchCord46(Filter1FrequenzControlMixer, 0, filter1, 1);
AudioConnection          patchCord47(Filter1FrequenzControlMixer, 0, F1F2CutoffInverter, 0);
AudioConnection          patchCord48(filter2, 0, FilterMixer, 1);
AudioConnection          patchCord49(filter2, 1, FilterMixer, 2);
AudioConnection          patchCord50(filter2, 2, FilterMixer, 3);
AudioConnection          patchCord51(filter1, 0, FilterMixer, 0);
AudioConnection          patchCord52(filter1, 0, peakFilter1Out, 0);
AudioConnection          patchCord53(FilterMixer, 0, VCAGuitarEnvelope, 0);
AudioConnection          patchCord54(VCAGuitarEnvelope, 0, UsbOutputMixerLinks, 3);
AudioConnection          patchCord55(VCAGuitarEnvelope, 0, UsbOutputMixerRechts, 3);
AudioConnection          patchCord56(VCAGuitarEnvelope, 0, I2SOutputMixerChannel1, 3);
AudioConnection          patchCord57(VCAGuitarEnvelope, 0, I2SOutputMixerChannel2, 3);
AudioConnection          patchCord58(I2SOutputMixerChannel2, 0, i2s1, 1);
AudioConnection          patchCord59(I2SOutputMixerChannel1, 0, i2s1, 0);
AudioConnection          patchCord60(UsbOutputMixerRechts, 0, USBout, 1);
AudioConnection          patchCord61(UsbOutputMixerLinks, 0, USBout, 0);
AudioControlSGTL5000     sgtl5000_1;     //xy=3291,544
// GUItool: end automatically generated code




// wolke ohne gui
//AudioSynthMasterclock myMasterClock;
//AudioAnalyzePeak myMasterPeak;
//AudioConnection          clockToPeak(myMasterClock, myMasterPeak);
