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
#define DELAY1_MAX_DELAY_TIME 1800.0f

/*
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



// mit delay


// GUItool: begin automatically generated code
AudioSynthWaveform       MOD_LFO;        //xy=947.5,1316.5
AudioSynthWaveform       VOLUMEN_LFO;    //xy=1100.5,1385.5
AudioAnalyzePeak         FOLLOW_MOD_LFO; //xy=1186.5,1316.5
AudioInputUSB            USBin;          //xy=1220.5,488.5
AudioInputI2S            i2s2;           //xy=1227.5,659.5
AudioSynthWaveformDc     VOL_LFO_NOISE_DEPTH; //xy=1271.5,1560.5
AudioSynthWaveformDc     VOL_LFO_SUBS_DEPTH; //xy=1272.5,1521.5
AudioSynthWaveformDc     VOL_LFO_OSC1_DEPTH; //xy=1274.5,1443.5
AudioSynthWaveformDc     VOL_LFO_OSC2_DEPTH; //xy=1274.5,1483.5
AudioAnalyzePeak         FOLLOW_VOL_LFO; //xy=1325.5,1385.5
AudioSynthWaveform       SubOsc2;        //xy=1379.5,1175.5
AudioEffectBitcrusher    bitcrusher1;    //xy=1380.5,1226.5
AudioSynthWaveform       SubOsc1;        //xy=1397.5,1125.5
AudioAnalyzeRMS          lineinRms;      //xy=1421.5,568.5
AudioSynthNoiseWhite     noise1;         //xy=1434.5,1277.5
AudioSynthWaveform       Osc2;           //xy=1442.5,1074.5
AudioMixer4              mixerAnalyse;   //xy=1453.5,848.5
AudioSynthWaveform       Osc1;           //xy=1507.5,1027.5
AudioSynthWaveformDc     Filter_LFO_depth; //xy=1597.5,1574.5
AudioMixer4              SubOscMixer;    //xy=1609.5,1161.5
AudioSynthWaveform       Filter_LFO;     //xy=1613.5,1533.5
AudioAnalyzeNoteFrequency notefreq;       //xy=1701.5,795.5
AudioSynthWaveformDc     filter1envelopeIntensity; //xy=1710.5,1667.5
AudioAnalyzeRMS          rmsMixerAnalyseOut; //xy=1740.5,892.5
AudioAnalyzeAttack       myAttackDetector; //xy=1768.5,844.5
AudioSynthWaveformDc     filter1envelopeGenerator; //xy=1770.5,1794.5
AudioSynthWaveformDc     VCAenvelopeGenerator; //xy=1772.5,1839.5
AudioSynthWaveformDc     F1F2CutoffInverter_ctrl; //xy=1779.5,1374.5
AudioEffectMultiply      VOL_LFO_OSC1_CTR; //xy=1809.5,984.5
AudioEffectMultiply      VOL_LFO_OSC2_CTR; //xy=1809.5,1074.5
AudioEffectMultiply      Filter_LFO_multiply; //xy=1816.5,1539.5
AudioEffectMultiply      VOL_LFO_NOISE_CTR; //xy=1822.5,1259.5
AudioEffectMultiply      VOL_LFO_SUBS_CTR; //xy=1832.5,1168.5
AudioEffectMultiply      multiply1;      //xy=1986.5,1636.5
AudioEffectMultiply      F1F2CutoffInverter; //xy=2045.5,1331.5
AudioMixer4              OscMainMix;     //xy=2064.5,1139.5
AudioMixer4              Filter1FrequenzControlMixer; //xy=2088.5,1456.5
AudioAnalyzePeak         FCMAnalyse;     //xy=2137.5,1778.5
AudioAnalyzePeak         filterAttackAnalyse; //xy=2250.5,1700.5
AudioFilterStateVariable filter2;        //xy=2284.5,1144.5
AudioFilterStateVariable filter1;        //xy=2289.5,1223.5
AudioAnalyzePeak         peakFilter1Out; //xy=2445.5,958.5
AudioMixer4              FilterMixer;    //xy=2531.5,1138.5
AudioFilterStateVariable lowpass;        //xy=2677.5,1222.5
AudioFilterStateVariable highpass;       //xy=2793.5,1075.5
AudioAnalyzePeak         unusedPeak;     //xy=2912.5,959.5
AudioAnalyzePeak         unusedpeak2;    //xy=2965.5,1000.5
AudioEffectMultiply      VCAGuitarEnvelope; //xy=2981.5,1188.5
AudioMixer4              I2SOutputMixerChannel2; //xy=3092.5,695.5
AudioMixer4              I2SOutputMixerChannel1; //xy=3093.5,613.5
AudioMixer4              UsbOutputMixerRechts; //xy=3098.5,495.5
AudioMixer4              UsbOutputMixerLinks; //xy=3099.5,422.5
AudioMixer4              delayfeed;         //xy=3120,1340
AudioEffectDelay         delay1;         //xy=3126,1581
AudioMixer4              delaymixer;         //xy=3259,1205
AudioOutputUSB           USBout;         //xy=3363.5,454.5
AudioOutputI2S           i2s1;           //xy=3385.5,625.5
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
AudioConnection          patchCord58(VCAGuitarEnvelope, 0, delaymixer, 0);
AudioConnection          patchCord59(VCAGuitarEnvelope, 0, delayfeed, 0);
AudioConnection          patchCord60(I2SOutputMixerChannel2, 0, i2s1, 1);
AudioConnection          patchCord61(I2SOutputMixerChannel1, 0, i2s1, 0);
AudioConnection          patchCord62(UsbOutputMixerRechts, 0, USBout, 1);
AudioConnection          patchCord63(UsbOutputMixerLinks, 0, USBout, 0);
AudioConnection          patchCord64(delayfeed, delay1);
AudioConnection          patchCord65(delayfeed, 0, delaymixer, 1);
AudioConnection          patchCord66(delay1, 0, delayfeed, 1);
AudioConnection          patchCord67(delaymixer, 0, I2SOutputMixerChannel2, 3);
AudioConnection          patchCord68(delaymixer, 0, I2SOutputMixerChannel1, 3);
AudioConnection          patchCord69(delaymixer, 0, UsbOutputMixerRechts, 3);
AudioConnection          patchCord70(delaymixer, 0, UsbOutputMixerLinks, 3);
AudioControlSGTL5000     sgtl5000_1;     //xy=3343.5,706.5
// GUItool: end automatically generated code

*/



// GUItool: begin automatically generated code
AudioSynthWaveform       MOD_LFO;        //xy=947.5,1316.5
AudioSynthWaveform       VOLUMEN_LFO;    //xy=1100.5,1385.5
AudioAnalyzePeak         FOLLOW_MOD_LFO; //xy=1186.5,1316.5
AudioInputUSB            USBin;          //xy=1220.5,488.5
AudioInputI2S            i2s2;           //xy=1227.5,659.5
AudioSynthWaveformDc     VOL_LFO_NOISE_DEPTH; //xy=1271.5,1560.5
AudioSynthWaveformDc     VOL_LFO_SUBS_DEPTH; //xy=1272.5,1521.5
AudioSynthWaveformDc     VOL_LFO_OSC1_DEPTH; //xy=1274.5,1443.5
AudioSynthWaveformDc     VOL_LFO_OSC2_DEPTH; //xy=1274.5,1483.5
AudioAnalyzePeak         FOLLOW_VOL_LFO; //xy=1325.5,1385.5
AudioSynthWaveform       SubOsc2;        //xy=1379.5,1175.5
AudioEffectBitcrusher    bitcrusher1;    //xy=1380.5,1226.5
AudioSynthWaveform       SubOsc1;        //xy=1397.5,1125.5
AudioAnalyzeRMS          lineinRms;      //xy=1421.5,568.5
AudioSynthNoiseWhite     noise1;         //xy=1434.5,1277.5
AudioSynthWaveform       Osc2;           //xy=1442.5,1074.5
AudioMixer4              mixerAnalyse;   //xy=1453.5,848.5
AudioSynthWaveform       Osc1;           //xy=1507.5,1027.5
AudioSynthWaveformDc     Filter_LFO_depth; //xy=1597.5,1574.5
AudioMixer4              SubOscMixer;    //xy=1609.5,1161.5
AudioSynthWaveform       Filter_LFO;     //xy=1613.5,1533.5
AudioAnalyzeNoteFrequency notefreq;       //xy=1701.5,795.5
AudioSynthWaveformDc     filter1envelopeIntensity; //xy=1710.5,1667.5
AudioAnalyzeRMS          rmsMixerAnalyseOut; //xy=1740.5,892.5
AudioAnalyzeAttack       myAttackDetector; //xy=1768.5,844.5
AudioSynthWaveformDc     filter1envelopeGenerator; //xy=1770.5,1794.5
AudioSynthWaveformDc     VCAenvelopeGenerator; //xy=1772.5,1839.5
AudioSynthWaveformDc     F1F2CutoffInverter_ctrl; //xy=1779.5,1374.5
AudioEffectMultiply      VOL_LFO_OSC1_CTR; //xy=1809.5,984.5
AudioEffectMultiply      VOL_LFO_OSC2_CTR; //xy=1809.5,1074.5
AudioEffectMultiply      Filter_LFO_multiply; //xy=1816.5,1539.5
AudioEffectMultiply      VOL_LFO_NOISE_CTR; //xy=1822.5,1259.5
AudioEffectMultiply      VOL_LFO_SUBS_CTR; //xy=1832.5,1168.5
AudioEffectMultiply      multiply1;      //xy=1986.5,1636.5
AudioEffectMultiply      F1F2CutoffInverter; //xy=2045.5,1331.5
AudioMixer4              OscMainMix;     //xy=2064.5,1139.5
AudioMixer4              Filter1FrequenzControlMixer; //xy=2088.5,1456.5
AudioAnalyzePeak         FCMAnalyse;     //xy=2137.5,1778.5
AudioAnalyzePeak         filterAttackAnalyse; //xy=2250.5,1700.5
AudioFilterStateVariable filter2;        //xy=2284.5,1144.5
AudioFilterStateVariable filter1;        //xy=2289.5,1223.5
AudioAnalyzePeak         peakFilter1Out; //xy=2445.5,958.5
AudioMixer4              FilterMixer;    //xy=2531.5,1138.5
AudioFilterStateVariable lowpass;        //xy=2677.5,1222.5
AudioFilterStateVariable highpass;       //xy=2793.5,1075.5
AudioAnalyzePeak         unusedPeak;     //xy=2912.5,959.5
AudioAnalyzePeak         unusedpeak2;    //xy=2965.5,1000.5
AudioEffectMultiply      VCAGuitarEnvelope; //xy=2981.5,1188.5
AudioMixer4              I2SOutputMixerChannel2; //xy=3092.5,695.5
AudioMixer4              I2SOutputMixerChannel1; //xy=3093.5,613.5
AudioMixer4              UsbOutputMixerRechts; //xy=3098.5,495.5
AudioMixer4              UsbOutputMixerLinks; //xy=3099.5,422.5
AudioMixer4              delayfeed;         //xy=3120,1340
AudioEffectDelay         delay1;         //xy=3126,1581
AudioMixer4              delaymixer;         //xy=3269,1208
AudioOutputUSB           USBout;         //xy=3363.5,454.5
AudioOutputI2S           i2s1;           //xy=3385.5,625.5
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
AudioConnection          patchCord58(VCAGuitarEnvelope, 0, delaymixer, 0);
AudioConnection          patchCord59(VCAGuitarEnvelope, 0, delayfeed, 0);
AudioConnection          patchCord60(I2SOutputMixerChannel2, 0, i2s1, 1);
AudioConnection          patchCord61(I2SOutputMixerChannel1, 0, i2s1, 0);
AudioConnection          patchCord62(UsbOutputMixerRechts, 0, USBout, 1);
AudioConnection          patchCord63(UsbOutputMixerLinks, 0, USBout, 0);
AudioConnection          patchCord64(delayfeed, delay1);
AudioConnection          patchCord65(delayfeed, 0, delaymixer, 1);
AudioConnection          patchCord66(delay1, 0, delayfeed, 1);
AudioConnection          patchCord67(delay1, 0, delaymixer, 2);
AudioConnection          patchCord68(delaymixer, 0, I2SOutputMixerChannel2, 3);
AudioConnection          patchCord69(delaymixer, 0, I2SOutputMixerChannel1, 3);
AudioConnection          patchCord70(delaymixer, 0, UsbOutputMixerRechts, 3);
AudioConnection          patchCord71(delaymixer, 0, UsbOutputMixerLinks, 3);
AudioControlSGTL5000     sgtl5000_1;     //xy=3343.5,706.5
// GUItool: end automatically generated code

