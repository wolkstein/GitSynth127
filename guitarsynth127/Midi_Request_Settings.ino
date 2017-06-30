


bool get127 = false;
bool get0 = false;
void Midi_Request_Settings(byte note){
  if(note == 127) get127 = true;
  if(note == 0) get0 = true;

  if(get127 && get0){
    GuiaskForPreset = true;
    askForPresetmillis = millis();
    if(DEBUG_MIDI_REQUEST_SETTINGS) Serial.printf("MIDI_REQUEST for SETTINGS at: %d ms, Midi CC in blocked %d \n", askForPresetmillis,GuiaskForPreset );
    get127 = false;
    get0 = false;

    
    int midiValueLocal = 0;

    // Expression Pedal
    usbMIDI.sendControlChange(0, mySettings.expressionPedalFunction, mySettings.midiChannel);
    
    // Poti Function
    usbMIDI.sendControlChange(1, mySettings.freeDataInt2, mySettings.midiChannel);
     
    // Function BTN Function
    usbMIDI.sendControlChange(2, mySettings.functionBtnFunction, mySettings.midiChannel);   
    
    // Guitar Input Gain
    midiValueLocal = map(mySettings.freeDataInt1, 0, 15, 0, 130);
    if(midiValueLocal >127) midiValueLocal = 127;
    usbMIDI.sendControlChange(3, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);
    
    // ########################## ANALOG MIXER #############################
    //
    // ########## Expression Pedal + Foot Switch    ########################

    
    // ########################## Digital MIXER ############################
    // Mixer Instrument Input To Synth Frequency - and Envelope - tracking, 0-127
    midiValueLocal = map( int(powf(mySettings.instrumentToSynth, 0.2)*1000)+5 ,0 , 1000, 0, 127); // volumen conversation
    usbMIDI.sendControlChange(4, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);

    // Mixer Usb Input 1 To Synth Frequency - and Envelope - tracking, 0-127
    midiValueLocal = map( int(powf(mySettings.usbIn_1_ToSynth, 0.2)*1000)+5 ,0 , 1000, 0, 127); // volumen conversation
    usbMIDI.sendControlChange(5, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);

    // Mixer Synth To Output 1, 0-127
    midiValueLocal = map( int(powf(mySettings.synthToOutput1, 0.2)*1000)+5 ,0 , 1000, 0, 127); // volumen conversation
    usbMIDI.sendControlChange(6, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);

    // Mixer Synth To Output 2, 0-127
    midiValueLocal = map( int(powf(mySettings.synthToOutput2, 0.2)*1000)+5 ,0 , 1000, 0, 127); // volumen conversation
    usbMIDI.sendControlChange(7, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);

    // Mixer Synth To Usb Output 1, 0-127
    midiValueLocal = map( int(powf(mySettings.synthToUsbOutput1, 0.2)*1000)+5 ,0 , 1000, 0, 127); // volumen conversation
    usbMIDI.sendControlChange(8, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);

    // Mixer Synth To Usb Output 2, 0-127
    midiValueLocal = map( int(powf(mySettings.synthToUsbOutput2, 0.2)*1000)+5 ,0 , 1000, 0, 127); // volumen conversation
    usbMIDI.sendControlChange(9, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);

    // Mixer Instrument To Usb-OutPut 1, 0-127
    midiValueLocal = map( int(powf(mySettings.instrumentToUsb_1, 0.2)*1000)+5 ,0 , 1000, 0, 127); // volumen conversation
    usbMIDI.sendControlChange(10, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);

    // Mixer Instrument To Usb-OutPut 2, 0-127
    midiValueLocal = map( int(powf(mySettings.instrumentToUsb_2, 0.2)*1000)+5 ,0 , 1000, 0, 127); // volumen conversation
    usbMIDI.sendControlChange(11, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);

    // Mixer Usb Input 1 To Output 1, 0-127
    midiValueLocal = map( int(powf(mySettings.usbIn_1_ToOutput1, 0.2)*1000)+5 ,0 , 1000, 0, 127); // volumen conversation
    usbMIDI.sendControlChange(12, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);

    // Mixer Usb Input 1 To Output 2, 0-127
    midiValueLocal = map( int(powf(mySettings.usbIn_1_ToOutput2, 0.2)*1000)+5 ,0 , 1000, 0, 127); // volumen conversation
    usbMIDI.sendControlChange(13, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);

    // Mixer Usb Input 2 To Output 1, 0-127
    midiValueLocal = map( int(powf(mySettings.usbIn_2_ToOutput1, 0.2)*1000)+5 ,0 , 1000, 0, 127); // volumen conversation
    usbMIDI.sendControlChange(14, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);

    // Mixer Usb Input 2 To Output 2, 0-127
    midiValueLocal = map( int(powf(mySettings.usbIn_2_ToOutput2, 0.2)*1000)+5 ,0 , 1000, 0, 127); // volumen conversation
    usbMIDI.sendControlChange(15, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);

    // Filter 1 D-F-F
    midiValueLocal =  mySettings.Filter1_Active_Frequenz_Tracking_Target;
    usbMIDI.sendControlChange(16, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);   

    // Filter 2 D-F-F
    midiValueLocal =  mySettings.Filter2_Active_Frequenz_Tracking_Target;
    usbMIDI.sendControlChange(17, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);


    // Filter LFO cycles
    midiValueLocal =  mySettings.filter_lfo_cycles;
    usbMIDI.sendControlChange(18, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);   

    // MOD LFO 2 cycles
    midiValueLocal =  mySettings.mod_lfo_cycles;
    usbMIDI.sendControlChange(19, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);




     
    
    // 1: OSC Volumen CC 20 gui fertig
    midiValueLocal = map( int(powf(mySettings.SYNTH_Master_Volume, 0.2)*1000)+5 ,0 , 1000, 0, 127); // volumen conversation
    usbMIDI.sendControlChange(20, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);

    // 2: filter1 frequency CC 21 gui fertig
    if(mySettings.Filter1_Active_Frequenz_Tracking_Target>0){
      midiValueLocal = map( mySettings.int_Filter1_Active_Frequenz_Tracking , -60 , 60, 0, 127);
    }
    else
    {
      midiValueLocal = map( int(powf(((mySettings.filter1Freq-10)/20000.0), 0.2)*1000.0)+5 ,0 , 1000, 0, 127); // frequency conversation
    }
    usbMIDI.sendControlChange(21, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);
    
    // 3: filter1 resonance CC 22 gui fertig
    midiValueLocal = map(int(mySettings.filter1Reso * 1000.0)+5,710,25000,0,127); // frequency conversation
    usbMIDI.sendControlChange(22, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);

    // 4: filter1 octave CC 23 gui fertig
    midiValueLocal = map(int(mySettings.filter1Octave * 1000.0)+5,0,7000,0,127);
    usbMIDI.sendControlChange(23, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);

    // 5: filter2 frequency CC 24 gui fertig
    if(mySettings.Filter2_Active_Frequenz_Tracking_Target>0){
      midiValueLocal = map( mySettings.int_Filter2_Active_Frequenz_Tracking , -60 , 60, 0, 127);
    }
    else
    {
      midiValueLocal = map( int(powf(((mySettings.filter2Freq-10)/20000.0), 0.2)*1000.0)+5 ,0 , 1000, 0, 127); // frequency conversation
    }
    usbMIDI.sendControlChange(24, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);
    
    // 6: filter2 resonance CC 25 gui fertig
    midiValueLocal = map(int(mySettings.filter2Reso * 1000.0)+5,710,25000,0,127); // frequency conversation
    usbMIDI.sendControlChange(25, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);  

    // 7: filter2 octave CC 26 gui fertig
    midiValueLocal = map(int(mySettings.filter2Octave * 1000.0)+5,0,7000,0,127);
    usbMIDI.sendControlChange(26, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);

    // 8: Filter 2 Mode CC 27 gui fertig
    usbMIDI.sendControlChange(27, mySettings.filter2Mode, mySettings.midiChannel);

    // 9: Filter 1 - 2 Blend gui fertig
    midiValueLocal =map(int(mySettings.filter_1_2_blend*1000.0)+5,-500,500,0,127);
    usbMIDI.sendControlChange(28, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);
    
    // 10: Filter 1 - 2 crossover mod gui fertig
    midiValueLocal = 0;
    if( mySettings.f1_f2_crossover_Modulation < 0.0) midiValueLocal = 1; 
    usbMIDI.sendControlChange(29, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);
    
    //delay(2); // hmm gui ist zu langsam, queu?
    // 11: Guitar Sensing Attack gui fertig
    midiValueLocal = map(int(powf((mySettings.sensingAttack-1)/3000.0 ,0.2)*1000)+6,1,1000,0,127);
    usbMIDI.sendControlChange(30, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);
   
    // 12: Guitar Sensing Release gui fertig
    midiValueLocal = map(mySettings.sensingFall+5,0,1000,0,127);
    usbMIDI.sendControlChange(31, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);  

    // 13: Guitar sensing Noc gui fertig
    midiValueLocal = map(int(powf((mySettings.sensingNoteOffCorner)/8000.0 ,0.2)*1000)+6,1,1000,0,127);
    usbMIDI.sendControlChange(32, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);

    // 14: Guitar Sensing Pitch detect treshold gui fertig 
    midiValueLocal = map(int(powf((mySettings.pitchDetectTreshold)/25000.0 ,0.2)*1000)+6,1,1000,0,127);
    usbMIDI.sendControlChange(33, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);
    
    // 15: Guitar Sensing Pitch detect quality gui fertig
    midiValueLocal = map(int(mySettings.pitchDetectQuality*1000.0)+5,0,990,0,127);
    usbMIDI.sendControlChange(34, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);

    // 16: OSC1 Volumen gui fertig
    midiValueLocal = map( int(powf(mySettings.osc1_level, 0.2)*1000)+5 ,0 , 1000, 0, 127);
    usbMIDI.sendControlChange(35, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);
   
    // 17: OSC 1 Waveform mit AKWFS gui fertig
    midiValueLocal =  mySettings.osc1_waveform;
    if( mySettings.osc1_waveform > 15) midiValueLocal = 15;
    usbMIDI.sendControlChange(36, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);

    // 18: OSC 1 PulseWide gui fertig
    midiValueLocal =  map( int(mySettings.osc1_pulseWidth*1000)+5 ,10 , 1000, 0, 127);
    usbMIDI.sendControlChange(37, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);
    
    // 19: OSC 1 Octave gui fertig
    midiValueLocal =  mySettings.osc1_octave;
    if( mySettings.osc1_octave > 8) midiValueLocal = 8;
    usbMIDI.sendControlChange(38, midiValueLocal + 3, mySettings.midiChannel);

    // 20: OSC 1 Note gui fertig
    midiValueLocal =  mySettings.osc1_note;
    if( mySettings.osc1_note > 24) midiValueLocal = 24;
    usbMIDI.sendControlChange(39, midiValueLocal + 12, mySettings.midiChannel);

    // 21: OSC 1 Detune gui fertig
    midiValueLocal =  mySettings.osc1_detune;
    if( mySettings.osc1_detune > 24) midiValueLocal = 24;
    usbMIDI.sendControlChange(40, midiValueLocal + 12, mySettings.midiChannel);

    // 22: OSC 1 Phase gui fertig
    midiValueLocal =  map(mySettings.osc1_phase + 1,0,360,0,127);
    usbMIDI.sendControlChange(41, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);

    // OSC 2
     
    // 23: OSC2 Volumen gui fertig
    midiValueLocal = map( int(powf(mySettings.osc2_level, 0.2)*1000)+5 ,0 , 1000, 0, 127);
    usbMIDI.sendControlChange(42, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);
   
    // 17: OSC 2 Waveform mit AKWFS gui fertig
    midiValueLocal =  mySettings.osc2_waveform;
    if( mySettings.osc2_waveform > 15) midiValueLocal = 15;
    usbMIDI.sendControlChange(43, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);

    // 18: OSC 2 PulseWide gui fertig
    midiValueLocal =  map( int(mySettings.osc2_pulseWidth*1000)+5 ,10 , 1000, 0, 127);
    usbMIDI.sendControlChange(44, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);
    
    // 19: OSC 2 Octave gui fertig
    midiValueLocal =  mySettings.osc2_octave;
    if( mySettings.osc2_octave > 8) midiValueLocal = 8;
    usbMIDI.sendControlChange(45, midiValueLocal + 3, mySettings.midiChannel);

    // 20: OSC 2 Note gui fertig
    midiValueLocal =  mySettings.osc2_note;
    if( mySettings.osc2_note > 24) midiValueLocal = 24;
    usbMIDI.sendControlChange(46, midiValueLocal + 12, mySettings.midiChannel);

    // 21: OSC 2 Detune gui fertig
    midiValueLocal =  mySettings.osc2_detune;
    if( mySettings.osc2_detune > 24) midiValueLocal = 24;
    usbMIDI.sendControlChange(47, midiValueLocal + 12, mySettings.midiChannel);

    // 22: OSC 2 Phase gui fertig
    midiValueLocal =  map(mySettings.osc2_phase + 1,0,360,0,127);
    usbMIDI.sendControlChange(48, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);


    // SUB OSC S
    // 16: SUB OSC 1 Volumen gui fertig
    midiValueLocal = map( int(powf(mySettings.sub_osc1_level, 0.2)*1000)+5 ,0 , 1000, 0, 127);
    usbMIDI.sendControlChange(49, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);
   
    // 17: SUB OSC 1 Waveform mit AKWFS gui fertig
    midiValueLocal =  mySettings.sub_osc1_waveform;
    if( mySettings.sub_osc1_waveform > 15) midiValueLocal = 15;
    usbMIDI.sendControlChange(50, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);

    // 18: SUB OSC 1 PulseWide gui fertig
    midiValueLocal =  map( int(mySettings.sub_osc1_pulseWidth*1000)+5 ,10 , 1000, 0, 127);
    usbMIDI.sendControlChange(51, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);
    
    // 19: SUB OSC 1 Octave gui fertig
    midiValueLocal =  mySettings.sub_osc1_octave;
    if( mySettings.sub_osc1_octave > 8) midiValueLocal = 8;
    usbMIDI.sendControlChange(52, midiValueLocal + 3, mySettings.midiChannel);

    // 20: SUB OSC 1 Note gui fertig
    midiValueLocal =  mySettings.sub_osc1_note;
    if( mySettings.sub_osc1_note > 24) midiValueLocal = 24;
    usbMIDI.sendControlChange(53, midiValueLocal + 12, mySettings.midiChannel);

    // 21: SUB OSC 1 Detune gui fertig
    midiValueLocal =  mySettings.sub_osc1_detune;
    if( mySettings.sub_osc1_detune > 24) midiValueLocal = 24;
    usbMIDI.sendControlChange(54, midiValueLocal + 12, mySettings.midiChannel);

    // 22: SUB OSC 1 Phase gui fertig
    midiValueLocal =  map(mySettings.sub_osc1_phase + 1,0,360,0,127);
    usbMIDI.sendControlChange(55, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);

    // SUB OSC 2
     
    // 23: SUB OSC 2 Volumen gui fertig
    midiValueLocal = map( int(powf(mySettings.sub_osc2_level, 0.2)*1000)+5 ,0 , 1000, 0, 127);
    usbMIDI.sendControlChange(56, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);
   
    // 17: SUB OSC 2 Waveform mit AKWFS gui fertig
    midiValueLocal =  mySettings.sub_osc2_waveform;
    if( mySettings.sub_osc2_waveform > 15) midiValueLocal = 15;
    usbMIDI.sendControlChange(57, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);

    // 18: SUB OSC 2 PulseWide gui fertig
    midiValueLocal =  map( int(mySettings.sub_osc2_pulseWidth*1000)+5 ,10 , 1000, 0, 127);
    usbMIDI.sendControlChange(58, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);
    
    // 19: SUB OSC 2 Octave gui fertig
    midiValueLocal =  mySettings.sub_osc2_octave;
    if( mySettings.sub_osc2_octave > 8) midiValueLocal = 8;
    usbMIDI.sendControlChange(59, midiValueLocal + 3, mySettings.midiChannel);

    // 20: SUB OSC 2 Note gui fertig
    midiValueLocal =  mySettings.sub_osc2_note;
    if( mySettings.sub_osc2_note > 24) midiValueLocal = 24;
    usbMIDI.sendControlChange(60, midiValueLocal + 12, mySettings.midiChannel);

    // 21: SUB OSC 2 Detune gui fertig
    midiValueLocal =  mySettings.sub_osc2_detune;
    if( mySettings.sub_osc2_detune > 24) midiValueLocal = 24;
    usbMIDI.sendControlChange(61, midiValueLocal + 12, mySettings.midiChannel);

    // 22: SUB OSC 2 Phase gui fertig
    midiValueLocal =  map(mySettings.sub_osc2_phase + 1,0,360,0,127);
    usbMIDI.sendControlChange(62, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);

    // noise volumen gui fertig
    midiValueLocal = map( int(powf(mySettings.noise_level, 0.2)*1000)+5 ,0 , 1000, 0, 127);
    usbMIDI.sendControlChange(63, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);

    // bit crusher volumen gui fertig
    midiValueLocal = map( int(powf(mySettings.Crusher_Volumen, 0.2)*1000)+5 ,0 , 1000, 0, 127);
    usbMIDI.sendControlChange(64, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);
    
    // bit crusher bits gui fertig
    midiValueLocal = mySettings.Crusher_Bits -1;
    usbMIDI.sendControlChange(65, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);
    
    // bit crusher sample rate gui fertig
    midiValueLocal = mySettings.Crusher_SampleRate;
    usbMIDI.sendControlChange(66, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);
    
    // Filter Lfo WaveForm gui fertig
    midiValueLocal = mySettings.filter_lfo_waveform;
    usbMIDI.sendControlChange(67, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);

    // Filter LFO PulseWide gui fertig
    midiValueLocal = map( int(mySettings.filter_lfo_pulseWidth*1000)+5 ,10 , 1000, 0, 127);
    usbMIDI.sendControlChange(68, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);    

    // Filter LFO BPM  gui fertig
    midiValueLocal = map(mySettings.filter_lfo_bpm + 2,0,360,0,127);
    usbMIDI.sendControlChange(69, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);

    // Filter LFO BPM Multiplier gui fertig
    midiValueLocal = mySettings.filter_lfo_freq_bpm_multi;
    usbMIDI.sendControlChange(70, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);

    // Filter LFO Phase gui fertig
    midiValueLocal =  map(mySettings.filter_lfo_phase + 1,0,360,0,127);
    usbMIDI.sendControlChange(71, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);
   
    // Filter Lfo Sync To Attack mod gui fertig
    midiValueLocal = 0;
    if( mySettings.filter_lfo_attack_sync) midiValueLocal = 1; 
    usbMIDI.sendControlChange(72, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);
    
    // Filter LFO Depth gui fertig
    midiValueLocal = map( int(mySettings.filter_lfo_intensety*1000)+5 ,-1000 , 1000, 0, 127);
    usbMIDI.sendControlChange(73, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);

    // Filter LFO envelope Attack to filter freq. gui fertig
    midiValueLocal = map( int(mySettings.envelopeFollowerAttackToFilterLFOFrequenzy*100)+5 , 0 , 1000, 0, 127);
    usbMIDI.sendControlChange(74, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);

    // Filter LFO envelope Attack to filter depth. gui fertig
    midiValueLocal = map( int(mySettings.envelopeFollowerAttackToFilterLFODepth*1000)+5 ,-1000 , 1000, 0, 127);
    usbMIDI.sendControlChange(75, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);

    // Attack Envelope to filter gui fertig
    midiValueLocal = map( int(mySettings.filter1envelopeIntensity*1000)+5 ,-1000 , 1000, 0, 127);
    usbMIDI.sendControlChange(76, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);

    // guitar use filteradsr gui fertig
    midiValueLocal = 0;
    if( mySettings.gitarUseFilterADSR) midiValueLocal = 1; 
    usbMIDI.sendControlChange(77, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);

    // guitar use filteradsr gui fertig
    midiValueLocal = 0;
    if( mySettings.gitarUseEnvelopeADSR) midiValueLocal = 1; 
    usbMIDI.sendControlChange(78, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);  

     // Envelope Attack gui fertig
    midiValueLocal = map(int(powf((mySettings.midi_attack-1)/9999.0 ,1.0/3.0)*1000)+7,1,1000,0,127);
    usbMIDI.sendControlChange(79, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);
  
    // Envelope Hold gui fertig
    midiValueLocal = map(int(powf((mySettings.midi_hold-1)/9999.0 ,1.0/3.0)*1000)+7,1,1000,0,127);
    usbMIDI.sendControlChange(80, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);
 
    // Envelope Decay gui fertig
    midiValueLocal = map(int(powf((mySettings.midi_decay-1)/9999.0 ,1.0/3.0)*1000)+7,1,1000,0,127);
    usbMIDI.sendControlChange(81, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);
    
    // Envelope Sustain gui fertig
    midiValueLocal = mySettings.midi_sustain;
    if(midiValueLocal > 100) midiValueLocal = 100;
    usbMIDI.sendControlChange(82, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);

    // Envelope Release gui fertig
    midiValueLocal = map(int(powf((mySettings.midi_release-1)/9999.0 ,1.0/3.0)*1000)+7,1,1000,0,127);
    usbMIDI.sendControlChange(83, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);

    // ###############   ADSR Filter
    // Filter Delay gui fertig
    midiValueLocal = map(int(powf((mySettings.filter_delay-1)/9999.0 ,1.0/3.0)*1000)+7,1,1000,0,127);
    usbMIDI.sendControlChange(84, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);  
    
    // Filter Attack gui fertig
    midiValueLocal = map(int(powf((mySettings.filter_attack-1)/9999.0 ,1.0/3.0)*1000)+7,1,1000,0,127);
    usbMIDI.sendControlChange(85, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);
  
    // Filter Hold gui fertig
    midiValueLocal = map(int(powf((mySettings.filter_hold-1)/9999.0 ,1.0/3.0)*1000)+7,1,1000,0,127);
    usbMIDI.sendControlChange(86, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);
 
    // Filter Decay gui fertig
    midiValueLocal = map(int(powf((mySettings.filter_decay-1)/9999.0 ,1.0/3.0)*1000)+7,1,1000,0,127);
    usbMIDI.sendControlChange(87, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);

    // Filter Sustain gui fertig
    midiValueLocal = mySettings.filter_sustain;
    if(midiValueLocal > 100) midiValueLocal = 100;
    usbMIDI.sendControlChange(88, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);

    // Filter Release gui fertig
    midiValueLocal = map(int(powf((mySettings.filter_release-1)/9999.0 ,1.0/3.0)*1000)+7,1,1000,0,127);
    usbMIDI.sendControlChange(89, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);  

    // MOD LFO #########################################
    // Mod Lfo WaveForm gui fertig
    midiValueLocal = mySettings.mod_lfo_waveform;
    usbMIDI.sendControlChange(90, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);

    // Mod LFO PulseWide gui fertig
    midiValueLocal = map( int(mySettings.mod_lfo_pulseWidth*1000)+5 ,10 , 1000, 0, 127);
    usbMIDI.sendControlChange(91, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);    

    // Mod LFO BPM gui fertig
    midiValueLocal = map(mySettings.mod_lfo_bpm + 2,0,360,0,127);
    usbMIDI.sendControlChange(92, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);

    // Mod LFO BPM Multiplier gui fertig
    midiValueLocal = mySettings.mod_lfo_freq_bpm_multi;
    usbMIDI.sendControlChange(93, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);

    // Mod LFO Phase gui fertig
    midiValueLocal =  map(mySettings.mod_lfo_phase + 1,0,360,0,127);
    usbMIDI.sendControlChange(94, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);
   
    //Mod Lfo Sync To Attack gui fertig
    midiValueLocal = 0;
    if( mySettings.mod_lfo_attack_sync) midiValueLocal = 1; 
    usbMIDI.sendControlChange(95, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);
 
    // Mod LFO envelope Attack to filter freq. gui fertig
    midiValueLocal = map( int(mySettings.envelopeFollowerAttackToMODLFOFrequenzy*100)+5 , 0 , 1000, 0, 127);
    usbMIDI.sendControlChange(96, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);

    // Mod LFO envelope Attack to filter depth. gui fertig
    midiValueLocal = map( int(mySettings.mod_lfo_follow_envelope_attack*1000)+5 ,-1000 , 1000, 0, 127);
    usbMIDI.sendControlChange(97, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);  
    
    // Filter ADSR Static Envelope gui fertig
    midiValueLocal =  map( int(mySettings.gitarUseFilterADSRstaticVelocity*1000)+5 ,0 , 1000, 0, 127);
    usbMIDI.sendControlChange(98, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);

    // Mod LFO Target OSC1 Freq gui fertig
    midiValueLocal = map( int(mySettings.mod_lfo_T1_OSC1_FREQ_intensety*1000)+5 ,0 , 1000, 0, 127);
    usbMIDI.sendControlChange(99, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);
 
    // Mod LFO Target OSC2 Freq gui fertig
    midiValueLocal = map( int(mySettings.mod_lfo_T2_OSC2_FREQ_intensety*1000)+5 ,0 , 1000, 0, 127);
    usbMIDI.sendControlChange(100, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);   

    // Mod LFO Target SUB OSC1 Freq gui fertig
    midiValueLocal = map( int(mySettings.mod_lfo_T3_SUB1_FREQ_intensety*1000)+5 ,0 , 1000, 0, 127);
    usbMIDI.sendControlChange(101, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);
 
    // Mod LFO Target SUB OSC2 Freq gui fertig
    midiValueLocal = map( int(mySettings.mod_lfo_T4_SUB2_FREQ_intensety*1000)+5 ,0 , 1000, 0, 127);
    usbMIDI.sendControlChange(102, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);
    
    //Mod Lfo Intensity T1 - T4 Sync gui fertig
    midiValueLocal = 0;
    if( mySettings.mod_lfo_T1_T4_FREQ_intensety_sync) midiValueLocal = 1; 
    usbMIDI.sendControlChange(103, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);    
    
    //Mod Lfo Intensity T5 - T8 Sync gui fertig
    midiValueLocal = 0;
    if( mySettings.mod_lfo_T1_T4_PULSE_intensety_sync) midiValueLocal = 1; 
    usbMIDI.sendControlChange(104, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel); 


    // Mod LFO Target OSC1 Pulse gui fertig
    midiValueLocal = map( int(mySettings.mod_lfo_T1_OSC1_PULSE_intensety*1000)+5 ,-1000 , 1000, 0, 127);
    usbMIDI.sendControlChange(105, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);
 
    // Mod LFO Target OSC2 Pulse gui fertig
    midiValueLocal = map( int(mySettings.mod_lfo_T2_OSC2_PULSE_intensety*1000)+5 ,-1000 , 1000, 0, 127);
    usbMIDI.sendControlChange(106, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);   

    // Mod LFO Target SUB OSC1 Pulse gui fertig
    midiValueLocal = map( int(mySettings.mod_lfo_T3_SUB1_PULSE_intensety*1000)+5 ,-1000 , 1000, 0, 127);
    usbMIDI.sendControlChange(107, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);
 
    // Mod LFO Target SUB OSC2 Pulse gui fertig
    midiValueLocal = map( int(mySettings.mod_lfo_T4_SUB2_PULSE_intensety*1000)+5 ,-1000 , 1000, 0, 127);
    usbMIDI.sendControlChange(108, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);


    // Vol LFO #########################################
    // Vol Lfo WaveForm gui fertig
    midiValueLocal = mySettings.vol_lfo_waveform;
    usbMIDI.sendControlChange(109, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);

    // Vol LFO PulseWide gui fertig
    midiValueLocal = map( int(mySettings.vol_lfo_pulseWidth*1000)+5 ,10 , 1000, 0, 127);
    usbMIDI.sendControlChange(110, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);    

    // Vol LFO BPM gui fertig
    midiValueLocal = map(mySettings.vol_lfo_bpm + 2,0,360,0,127);
    usbMIDI.sendControlChange(111, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);

    // Vol LFO BPM Multiplier gui fertig
    midiValueLocal = mySettings.vol_lfo_freq_bpm_multi;
    usbMIDI.sendControlChange(112, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);

    // Vol LFO Phase gui fertig
    midiValueLocal =  map(mySettings.vol_lfo_phase + 1,0,360,0,127);
    usbMIDI.sendControlChange(113, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);
   
    //Vol Lfo Sync To Attack gui fertig
    midiValueLocal = 0;
    if( mySettings.vol_lfo_attack_sync) midiValueLocal = 1; 
    usbMIDI.sendControlChange(114, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);
 
    // Vol LFO envelope Attack to filter freq. gui fertig
    midiValueLocal = map( int(mySettings.envelopeFollowerAttackToVOLLFOFrequenzy*100)+5 , 0 , 1000, 0, 127);
    usbMIDI.sendControlChange(115, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);

    // Vol LFO envelope Attack to filter depth. gui fertig
    midiValueLocal = map( int(mySettings.vol_lfo_follow_envelope_attack*1000)+5 ,-1000 , 1000, 0, 127);
    usbMIDI.sendControlChange(116, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel); 

    //Vol Lfo Intensity T1 - T4 Sync gui fertig
    midiValueLocal = 0;
    if( mySettings.vol_lfo_T1_T4_intensety_sync) midiValueLocal = 1; 
    usbMIDI.sendControlChange(117, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel); 


    // Vol LFO Target OSC1 gui fertig
    midiValueLocal = map( int(mySettings.vol_lfo_T1_OSC1_intensety*1000)+5 ,-1000 , 1000, 0, 127);
    usbMIDI.sendControlChange(118, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);
 
    // Vol LFO Target OSC2 gui fertig
    midiValueLocal = map( int(mySettings.vol_lfo_T2_OSC2_intensety*1000)+5 ,-1000 , 1000, 0, 127);
    usbMIDI.sendControlChange(119, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);   

    // Vol LFO Target SUBS gui fertig
    midiValueLocal = map( int(mySettings.vol_lfo_T3_SUBS_intensety*1000)+5 ,-1000 , 1000, 0, 127);
    usbMIDI.sendControlChange(120, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);
 
    // Vol LFO Target Noise gui fertig
    midiValueLocal = map( int(mySettings.vol_lfo_T4_NOISE_intensety*1000)+5 ,0 , 1000, 0, 127);
    usbMIDI.sendControlChange(121, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);

    // Midi pitchbend Range
    midiValueLocal = mySettings.midi_pichbandrange -1;
    usbMIDI.sendControlChange(122, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);

    // peak Auto correction 
    midiValueLocal = 0;
    if( mySettings.autoPeakCorrection) midiValueLocal = 1; 
    usbMIDI.sendControlChange(123, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);        

     // Master Clock To All LFOs
    midiValueLocal = 0;
    if( mySettings.MasterClockToLfos) midiValueLocal = 1; 
    usbMIDI.sendControlChange(124, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);    
    
    // Master Clock To MOD LFO
    midiValueLocal = 0;
    if( mySettings.MasterClockToMODLfo) midiValueLocal = 1; 
    usbMIDI.sendControlChange(125, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);     
    
     // Master Clock To VOL LFO
    midiValueLocal = 0;
    if( mySettings.MasterClockToVOLLfo) midiValueLocal = 1; 
    usbMIDI.sendControlChange(126, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);    
    
     // Master Clock To FREQ LFO 
    midiValueLocal = 0;
    if( mySettings.MasterclockToFreqLFO) midiValueLocal = 1; 
    usbMIDI.sendControlChange(127, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel);

 
    // ######################## channel + 1 sends ########################
   
    // 1: lowpass frequency CC 1 channel +1
    midiValueLocal = map( int(powf(((mySettings.freeDataFloat1 - 10)/20000.0), 0.2)*1000.0)+5 ,0 , 1000, 0, 127); // frequency conversation    
    usbMIDI.sendControlChange(1, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel + 1);
    
    // 2: lowpass resonance CC 2 channel + 1
    midiValueLocal = map(int(mySettings.freeDataFloat2 * 1000.0)+5,710,25000,0,127); // frequency conversation
    usbMIDI.sendControlChange(2, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel + 1);

    // 3: highpass frequency CC 3 channel + 1
    midiValueLocal = map( int(powf(((mySettings.freeDataFloat3 - 10)/20000.0), 0.2)*1000.0)+5 ,0 , 1000, 0, 127); // frequency conversation
    usbMIDI.sendControlChange(3, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel + 1);
    
    // 4: highpass resonance CC 2 channel + 1
    midiValueLocal = map(int(mySettings.freeDataFloat4 * 1000.0)+5,710,25000,0,127); // frequency conversation
    usbMIDI.sendControlChange(4, checkmidiValueLocal(midiValueLocal), mySettings.midiChannel + 1); 

    
    for(int i = 0; i<32;i++){
      usbMIDI.sendNoteOn(125, 1, mySettings.midiChannel);
      usbMIDI.sendNoteOn(3, 1, mySettings.midiChannel);
      usbMIDI.sendNoteOn(i, mySettings.sequenzerNotes[i] + 25, mySettings.midiChannel);
    }
    usbMIDI.sendNoteOn(125, 1, mySettings.midiChannel);
    usbMIDI.sendNoteOn(3, 1, mySettings.midiChannel);
    usbMIDI.sendNoteOn(32, mySettings.sequeceSteps + 1, mySettings.midiChannel);
    
    usbMIDI.sendNoteOn(125, 1, mySettings.midiChannel);
    usbMIDI.sendNoteOn(3, 1, mySettings.midiChannel);
    usbMIDI.sendNoteOn(33, mySettings.sequenceTimeSignature + 1, mySettings.midiChannel);    
 }
}

int checkmidiValueLocal(int value){
  if(value <= 0) value = 0;
  if(value >= 127) value = 127;
  return value;
}

