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
AudioSynthWaveform       MOD_LFO;        //xy=513.3333129882812,1237.333324432373
AudioSynthWaveform       VOLUMEN_LFO;    //xy=666.3333129882812,1306.333324432373
AudioAnalyzePeak         FOLLOW_MOD_LFO; //xy=752.3333129882812,1237.333324432373
AudioInputUSB            USBin;          //xy=786.3333129882812,409.33332443237305
AudioInputI2S            i2s2;           //xy=793.3333129882812,580.333324432373
AudioSynthWaveformDc     VOL_LFO_NOISE_DEPTH; //xy=837.3333129882812,1481.333324432373
AudioSynthWaveformDc     VOL_LFO_SUBS_DEPTH; //xy=838.3333129882812,1442.333324432373
AudioSynthWaveformDc     VOL_LFO_OSC1_DEPTH; //xy=840.3333129882812,1364.333324432373
AudioSynthWaveformDc     VOL_LFO_OSC2_DEPTH; //xy=840.3333129882812,1404.333324432373
AudioAnalyzePeak         FOLLOW_VOL_LFO; //xy=891.3333129882812,1306.333324432373
AudioSynthWaveform       SubOsc2;        //xy=945.3333129882812,1096.333324432373
AudioEffectBitcrusher    bitcrusher1;    //xy=946.3333129882812,1147.333324432373
AudioSynthWaveform       SubOsc1;        //xy=963.3333129882812,1046.333324432373
AudioAnalyzeRMS          lineinRms;      //xy=987.3333129882812,489.33332443237305
AudioSynthNoiseWhite     noise1;         //xy=1000.3333129882812,1198.333324432373
AudioSynthWaveform       Osc2;           //xy=1008.3333129882812,995.333324432373
AudioMixer4              mixerAnalyse;   //xy=1019.3333129882812,769.333324432373
AudioSynthWaveform       Osc1;           //xy=1073.3333129882812,948.333324432373
AudioSynthWaveformDc     Filter_LFO_depth; //xy=1163.3333129882812,1495.333324432373
AudioMixer4              SubOscMixer;    //xy=1175.3333129882812,1082.333324432373
AudioSynthWaveform       Filter_LFO;     //xy=1179.3333129882812,1454.333324432373
AudioAnalyzeNoteFrequency notefreq;       //xy=1267.3333129882812,716.333324432373
AudioSynthWaveformDc     filter1envelopeIntensity; //xy=1276.3333129882812,1588.333324432373
AudioAnalyzeRMS          rmsMixerAnalyseOut; //xy=1306.3333129882812,813.333324432373
AudioAnalyzeAttack       myAttackDetector; //xy=1334.3333129882812,765.333324432373
AudioSynthWaveformDc     filter1envelopeGenerator; //xy=1336.3333129882812,1715.333324432373
AudioSynthWaveformDc     VCAenvelopeGenerator; //xy=1338.3333129882812,1760.333324432373
AudioSynthWaveformDc     F1F2CutoffInverter_ctrl; //xy=1345.3333129882812,1295.333324432373
AudioEffectMultiply      VOL_LFO_OSC1_CTR; //xy=1375.3333129882812,905.333324432373
AudioEffectMultiply      VOL_LFO_OSC2_CTR; //xy=1375.3333129882812,995.333324432373
AudioEffectMultiply      Filter_LFO_multiply; //xy=1382.3333129882812,1460.333324432373
AudioEffectMultiply      VOL_LFO_NOISE_CTR; //xy=1388.3333129882812,1180.333324432373
AudioEffectMultiply      VOL_LFO_SUBS_CTR; //xy=1398.3333129882812,1089.333324432373
AudioEffectMultiply      multiply1;      //xy=1552.3333129882812,1557.333324432373
AudioEffectMultiply      F1F2CutoffInverter; //xy=1611.3333129882812,1252.333324432373
AudioMixer4              OscMainMix;     //xy=1630.3333129882812,1060.333324432373
AudioMixer4              Filter1FrequenzControlMixer; //xy=1654.3333129882812,1377.333324432373
AudioAnalyzePeak         FCMAnalyse;     //xy=1703.3333129882812,1699.333324432373
AudioAnalyzePeak         filterAttackAnalyse; //xy=1816.3333129882812,1621.333324432373
AudioFilterStateVariable filter2;        //xy=1850.3333129882812,1065.333324432373
AudioFilterStateVariable filter1;        //xy=1855.3333129882812,1144.333324432373
AudioAnalyzePeak         peakFilter1Out; //xy=2011.3333129882812,879.333324432373
AudioMixer4              FilterMixer;    //xy=2097.3333129882812,1059.333324432373
AudioFilterStateVariable lowpass;        //xy=2243.3333129882812,1143.333324432373
AudioFilterStateVariable highpass;       //xy=2359.3333129882812,996.333324432373
AudioAnalyzePeak         unusedPeak;     //xy=2478.3333129882812,880.333324432373
AudioAnalyzePeak         unusedpeak2;    //xy=2531.3333129882812,921.333324432373
AudioEffectMultiply      VCAGuitarEnvelope; //xy=2547.3333129882812,1109.333324432373
AudioMixer4              I2SOutputMixerChannel2; //xy=2658.3333129882812,616.333324432373
AudioMixer4              I2SOutputMixerChannel1; //xy=2659.3333129882812,534.333324432373
AudioMixer4              UsbOutputMixerRechts; //xy=2664.3333129882812,416.33332443237305
AudioMixer4              UsbOutputMixerLinks; //xy=2665.3333129882812,343.33332443237305
AudioOutputUSB           USBout;         //xy=2929.3333129882812,375.33332443237305
AudioOutputI2S           i2s1;           //xy=2951.3333129882812,546.333324432373
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
AudioConnection          patchCord53(FilterMixer, 0, lowpass, 0);
AudioConnection          patchCord54(lowpass, 0, highpass, 0);
AudioConnection          patchCord55(highpass, 0, unusedPeak, 0);
AudioConnection          patchCord56(highpass, 1, unusedpeak2, 0);
AudioConnection          patchCord57(highpass, 2, VCAGuitarEnvelope, 0);
AudioConnection          patchCord58(VCAGuitarEnvelope, 0, I2SOutputMixerChannel2, 3);
AudioConnection          patchCord59(VCAGuitarEnvelope, 0, I2SOutputMixerChannel1, 3);
AudioConnection          patchCord60(VCAGuitarEnvelope, 0, UsbOutputMixerRechts, 3);
AudioConnection          patchCord61(VCAGuitarEnvelope, 0, UsbOutputMixerLinks, 3);
AudioConnection          patchCord62(I2SOutputMixerChannel2, 0, i2s1, 1);
AudioConnection          patchCord63(I2SOutputMixerChannel1, 0, i2s1, 0);
AudioConnection          patchCord64(UsbOutputMixerRechts, 0, USBout, 1);
AudioConnection          patchCord65(UsbOutputMixerLinks, 0, USBout, 0);
AudioControlSGTL5000     sgtl5000_1;     //xy=2909.3333129882812,627.333324432373


