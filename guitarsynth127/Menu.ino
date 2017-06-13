

int mylastAlphabetPosition = 14; // hoeste position = 72
char asciLookupTable[] = {
  48,49,50,51,52,53,
  54,55,56,57,60,61,
  62,63,65,66,67,68,
  69,70,71,72,73,74,
  75,76,77,78,79,80,
  81,82,83,84,85,86,
  87,88,89,90,95,97,
  98,99,100,101,102,103,
  104,105,106,107,108,109,
  110,111,112,113,114,115,
  116,117,118,119,120,121,
  122,32,33,42,43,45,46};


void selectMenuItemAndPrintLcd(int updown) {
   if(DEB_MENUE) Serial.printf("MenuIncrementer: %d Windowselect: %d\n",updown,myMenuWindow);
   float my_F_incrementer = 0.0f;
   int my_I_incrementer = 0;
   float percentval = 0.0f;
   float myKopfKnackerLeft = 0.0f; // für uberblendungs kram nutzbar
   float myKopfKnackerRight = 0.0f;
   lcd.clear();
   lcd.noCursor();
   lcd.setCursor(0, 0);

   switch(myMenuWindow) {
    case 0:
      if( mySystemSettings.switchLivePresets){
        lcd.printf("Live-%d",live_preset_position+1);
      }
      else
      {
        lcd.print("Presets"); 
      }
      lcd.setCursor(0, 1);

      current_preset += updown;
      if(current_preset > 127)current_preset = 1; // 127 im fertig zustand zum testen 5
      if(current_preset < 1)current_preset = 127;
      if(updown!=0){
        AudioNoInterrupts();
        edb_selectPreset(current_preset);
        AudioInterrupts();
        setSettings(false);
        calculateFilterMixer();
      }
      lcd.setCursor(0, 1);
      lcd.printf("%d %s",mySettings.preset, mySettings.presetName ); 
      
      menuExtrButton = false;
      myMidiNote.midiPitch = 0;

     break;


     case 1: // highcut filter freq

      my_I_incrementer = updown * 50; // 50 steps
      if(mySettings.freeDataFloat1 < 250) my_I_incrementer =  updown *10;
      if(mySettings.freeDataFloat1 > 1000) my_I_incrementer =  updown *100;
      if(checkIntValuesValide(my_I_incrementer, C_FILTER_FREQ_MIN+10, C_FILTER_FREQ_MAX, (int)mySettings.freeDataFloat1 + my_I_incrementer))
        mySettings.freeDataFloat1 += my_I_incrementer;
        
      if(updown!=0) lowpass.frequency(mySettings.freeDataFloat1);
      

      lcd.print("HighCut Cutoff");
      lcd.setCursor(0, 1);
      lcd.print(mySettings.freeDataFloat1);
      lcd.print("Hz");     
      menuExtrButton = false;
      
     break;
      
     case 2: // highCut Q
      my_F_incrementer = float(updown/100.0);//0.01 steps
      mySettings.freeDataFloat2 = getFloatValuesValide(my_F_incrementer, C_FILTER_Q_MIN, C_FILTER_Q_MAX, mySettings.freeDataFloat2);
       
      if(updown!=0) lowpass.resonance(mySettings.freeDataFloat2);
      lcd.print("HighCut Reso.");
      lcd.setCursor(0, 1);
      lcd.print(mySettings.freeDataFloat2);
      //lcd.print("");
      menuExtrButton = false;
     break;


     case 3: // highcut filter freq

      my_I_incrementer = updown * 50; // 50 steps
      if(mySettings.freeDataFloat3 < 250) my_I_incrementer =  updown *10;
      if(mySettings.freeDataFloat3 > 1000) my_I_incrementer =  updown *100;
      if(checkIntValuesValide(my_I_incrementer, C_FILTER_FREQ_MIN+10, C_FILTER_FREQ_MAX, (int)mySettings.freeDataFloat3 + my_I_incrementer))
        mySettings.freeDataFloat3 += my_I_incrementer;
        
      if(updown!=0) highpass.frequency(mySettings.freeDataFloat3);
      

      lcd.print("LowCut Cutoff");
      lcd.setCursor(0, 1);
      lcd.print(mySettings.freeDataFloat3);
      lcd.print("Hz");     
      menuExtrButton = false;
      
     break;
      
     case 4: // LowCut Q
      my_F_incrementer = float(updown/100.0);//0.01 steps
      mySettings.freeDataFloat4 = getFloatValuesValide(my_F_incrementer, C_FILTER_Q_MIN, C_FILTER_Q_MAX, mySettings.freeDataFloat4);
       
      if(updown!=0) highpass.resonance(mySettings.freeDataFloat4);
      lcd.print("LowCut Reso.");
      lcd.setCursor(0, 1);
      lcd.print(mySettings.freeDataFloat4);
      //lcd.print("");
      menuExtrButton = false;
     break;

     case 4 + 1: // filter1 freq
      if(mySettings.Filter1_Active_Frequenz_Tracking_Target>0){
        my_I_incrementer = updown;
        
        if(checkIntValuesValide(my_I_incrementer, -60, 60, mySettings.int_Filter1_Active_Frequenz_Tracking + my_I_incrementer))
          mySettings.int_Filter1_Active_Frequenz_Tracking += my_I_incrementer;

        if(updown!=0) G_Filter1_Active_Frequenz_Tracking = calculateDynamicFilterFrequency(mySettings.int_Filter1_Active_Frequenz_Tracking);
         
        lcd.print("Filter1 D-F-F");
        lcd.setCursor(0, 1);
        //if( mySettings.int_Filter1_Active_Frequenz_Tracking < -3 || mySettings.int_Filter1_Active_Frequenz_Tracking > 3)
        lcd.printf("on OSC F x %4.2f", G_Filter1_Active_Frequenz_Tracking);

        using_Filter1_Active_Frequenz_Tracking = true;

      }
      else
      {
        my_I_incrementer = updown * 50; // 50 steps
        if(mySettings.filter1Freq < 250) my_I_incrementer =  updown *10;
        if(mySettings.filter1Freq > 1000) my_I_incrementer =  updown *100;
        if(checkIntValuesValide(my_I_incrementer, C_FILTER_FREQ_MIN+10, C_FILTER_FREQ_MAX, mySettings.filter1Freq + my_I_incrementer))
          mySettings.filter1Freq += my_I_incrementer;
          
        if(updown!=0) filter1.frequency(mySettings.filter1Freq);
        

        lcd.print("Filter1 Cutoff");
        lcd.setCursor(0, 1);
        lcd.print(mySettings.filter1Freq);
        lcd.print("Hz");
        using_Filter1_Active_Frequenz_Tracking = false;
      }
      
      menuExtrButton = false;
     break;
      
     case 4 + 2: // filter1 Q
      my_F_incrementer = float(updown/100.0);//0.01 steps
      mySettings.filter1Reso = getFloatValuesValide(my_F_incrementer, C_FILTER_Q_MIN, C_FILTER_Q_MAX, mySettings.filter1Reso);
       
      if(updown!=0) filter1.resonance(mySettings.filter1Reso);
      lcd.print("Filter1 Resonanc");
      lcd.setCursor(0, 1);
      lcd.print(mySettings.filter1Reso);
      //lcd.print("");
      menuExtrButton = false;  break;
 
     case 4 + 3: // filter1 Octave 
      my_F_incrementer = float(updown/20.0);//0.05 steps
      mySettings.filter1Octave = getFloatValuesValide( my_F_incrementer, C_FILTER_OCT_MIN, C_FILTER_OCT_MAX, mySettings.filter1Octave);
 
      if(updown!=0) filter1.octaveControl(mySettings.filter1Octave);
      lcd.print("Filter1 Bandwidth");
      lcd.setCursor(0, 1);
      lcd.print(abs(mySettings.filter1Octave));
      lcd.print(" Octaves");
      menuExtrButton = false;  break;
      
     case 4 + 4: // filter_1_2_blend; // 1 Modees  -1 <--> 1  -1 = full filter1 1 = full filter 2, 0 both
      my_F_incrementer = float(updown/100.0);//0.01 steps
      mySettings.filter_1_2_blend = getFloatValuesValide(my_F_incrementer, -0.5, 0.5, mySettings.filter_1_2_blend);
      
      if(updown!=0) calculateFilterMixer();
      
      lcd.print("F1 <-> F2 Blend");
      lcd.setCursor(0, 1);

      // blendenberechnung

      
      if( mySettings.filter_1_2_blend < 0.001 && mySettings.filter_1_2_blend > -0.001){ // neutral
        myKopfKnackerLeft = 0.5;
        myKopfKnackerRight = 0.5;
      }
      if( mySettings.filter_1_2_blend < -0.001){ 
          myKopfKnackerLeft = 0.5 + abs(mySettings.filter_1_2_blend);
          myKopfKnackerRight = 0.5 - abs(mySettings.filter_1_2_blend); 
      }
      if( mySettings.filter_1_2_blend > 0.001){ 
          myKopfKnackerLeft = 0.5 - abs(mySettings.filter_1_2_blend);
          myKopfKnackerRight = 0.5 + abs(mySettings.filter_1_2_blend); 
      }     
      lcd.printf("F1 %d<->%d F2",int(myKopfKnackerLeft*100.0), int(myKopfKnackerRight*100.0) );
      menuExtrButton = false;  break;
      
     case 4 + 5: // f1_f2_crossover_Modulation
      if(updown > 0) mySettings.f1_f2_crossover_Modulation = -1.0;
      if(updown < 0) mySettings.f1_f2_crossover_Modulation = 1.0;

      if(updown!=0) F1F2CutoffInverter_ctrl.amplitude(mySettings.f1_f2_crossover_Modulation,2);
  
      
      lcd.print("F-Lfo & F-Env");
      lcd.setCursor(0, 1);
      if(mySettings.f1_f2_crossover_Modulation == 1.0) lcd.print("F1 = F2");
      if(mySettings.f1_f2_crossover_Modulation == -1.0) lcd.print("F1 != F2");
      menuExtrButton = false;  break;

     case 4 + 6: // Filter2 Mode:  1,  1 = Lowpass, 2 = Bandpass, 3 = Highpass
      my_I_incrementer = updown;
      if(checkIntValuesValide(my_I_incrementer, 1, 3, mySettings.filter2Mode + my_I_incrementer))
        mySettings.filter2Mode += my_I_incrementer;

      if(updown!=0) calculateFilterMixer();
      
      lcd.print("Filter2 Mode");
      lcd.setCursor(0, 1);
      if(mySettings.filter2Mode == 1) lcd.print("Lowpass");
      if(mySettings.filter2Mode == 2) lcd.print("Bandpass");
      if(mySettings.filter2Mode == 3) lcd.print("Highpass");
      menuExtrButton = false;  break;
     
      // filter 2

     case 4 + 7: // filter2 freq
       if(mySettings.Filter2_Active_Frequenz_Tracking_Target>0){
        my_I_incrementer = updown;
        
        if(checkIntValuesValide(my_I_incrementer, -60, 60, mySettings.int_Filter2_Active_Frequenz_Tracking + my_I_incrementer))
          mySettings.int_Filter2_Active_Frequenz_Tracking += my_I_incrementer;

        if(updown!=0) G_Filter2_Active_Frequenz_Tracking = calculateDynamicFilterFrequency(mySettings.int_Filter2_Active_Frequenz_Tracking);
         
        lcd.print("Filter2 D-F-F");
        lcd.setCursor(0, 1);
        //if( mySettings.int_Filter2_Active_Frequenz_Tracking < -3 || mySettings.int_Filter2_Active_Frequenz_Tracking > 3)
        lcd.printf("on OSC F x %4.2f", G_Filter2_Active_Frequenz_Tracking);
        using_Filter2_Active_Frequenz_Tracking = true;      
      }
      else
      {
        my_I_incrementer = updown * 50; // 50 steps
        if(mySettings.filter2Freq < 250) my_I_incrementer =  updown *10;
        if(mySettings.filter2Freq > 1000) my_I_incrementer =  updown *100;
        if(checkIntValuesValide(my_I_incrementer, C_FILTER_FREQ_MIN, C_FILTER_FREQ_MAX, mySettings.filter2Freq + my_I_incrementer))
          mySettings.filter2Freq += my_I_incrementer;
  
        if(updown!=0) filter2.frequency(mySettings.filter2Freq);
        lcd.print("Filter2 Cutoff");
        lcd.setCursor(0, 1);
        lcd.print(mySettings.filter2Freq);
        lcd.print("Hz");
        using_Filter2_Active_Frequenz_Tracking = false;
      }
      menuExtrButton = false;
     break;
      
     case 4 + 8: // filter2 Q
      my_F_incrementer = float(updown/100.0);//0.01 steps
      mySettings.filter2Reso = getFloatValuesValide(my_F_incrementer, C_FILTER_Q_MIN, C_FILTER_Q_MAX, mySettings.filter2Reso);
       
      if(updown!=0) filter2.resonance(mySettings.filter2Reso);
      lcd.print("Filter2 Resonanc");
      lcd.setCursor(0, 1);
      lcd.print(mySettings.filter2Reso);
      //lcd.print("");
      menuExtrButton = false;  break;
 
     case 4 + 9: // filter2 Octave 
      my_F_incrementer = float(updown/20.0);//0.05 steps
      mySettings.filter2Octave = getFloatValuesValide( my_F_incrementer, C_FILTER_OCT_MIN, C_FILTER_OCT_MAX, mySettings.filter2Octave);
 
      if(updown!=0) filter2.octaveControl(mySettings.filter2Octave);
      lcd.print("Filter2 Bandwidt");
      lcd.setCursor(0, 1);
      lcd.print(abs(mySettings.filter2Octave));
      lcd.print(" Octaves");
      menuExtrButton = false;  break;


// filter ende


    case 4 + 10: // off
      mySettings.sensingTreshold += updown *10;
      myAttackDetector.setTresAndFall(mySettings.sensingTreshold, mySettings.sensingFall);
      lcd.print("Gitsens Treshold");
      lcd.setCursor(0, 1);
      lcd.print(mySettings.sensingTreshold);
      //lcd.print("");
      menuExtrButton = false;  break;

    case 4 + 11: // Guitar Sensing Attack on
      my_I_incrementer = updown;
      if(mySettings.sensingAttack>30) my_I_incrementer = updown*5;
      if(mySettings.sensingAttack>80) my_I_incrementer = updown*10;
      
      if(checkIntValuesValide(my_I_incrementer, G_E_SENS_ATTACK_MIN, G_E_SENS_ATTACK_MAX, mySettings.sensingAttack + my_I_incrementer))
        mySettings.sensingAttack += my_I_incrementer;
        
      lcd.print("Gitsens Attack");
      lcd.setCursor(0, 1);
      lcd.print(mySettings.sensingAttack);
      lcd.print("ms");
      menuExtrButton = false;  break;
      
     case 4 + 12: // Guitar Sensing Fall on
      my_I_incrementer = updown * 5;
      if(checkIntValuesValide(my_I_incrementer, G_E_SENS_FALL_MIN, G_E_SENS_FALL_MAX, mySettings.sensingFall + my_I_incrementer))
        mySettings.sensingFall += my_I_incrementer;
        
      myAttackDetector.setTresAndFall(mySettings.sensingTreshold, mySettings.sensingFall);
      lcd.print("Gitsens Release");
      lcd.setCursor(0, 1);
      percentval = mySettings.sensingFall/10.0;
      lcd.print( percentval, 1);
      lcd.print("%");
      menuExtrButton = false;  break;
      
     case 4 + 13: // off
      mySettings.sensingBounce += updown;
      myAttackDetector.setBounceTime(mySettings.sensingBounce);
      lcd.print("Gitsens Bonce T.");
      lcd.setCursor(0, 1);
      lcd.print(mySettings.sensingBounce);
      //lcd.print("    ");
      menuExtrButton = false;  break;

     case 4 + 14: // off
      my_I_incrementer = updown * 10; // 10 steps
      if(checkIntValuesValide(my_I_incrementer, 0, 8000, mySettings.sensingNoteOffCorner + my_I_incrementer))
        mySettings.sensingNoteOffCorner += my_I_incrementer;
      if(mySettings.sensingNoteOffCorner < 0) mySettings.sensingNoteOffCorner = 0;
      if(mySettings.sensingNoteOffCorner > 8000) mySettings.sensingNoteOffCorner = 8000;
     
      lcd.print("Gitsens NOC");
      lcd.setCursor(0, 1);
      lcd.print(mySettings.sensingNoteOffCorner);
      //lcd.print("    ");
      menuExtrButton = false;  break;

     case 4 + 15: // on PITCH detect tershold
      my_I_incrementer = updown * 10; // 10 steps
      if(checkIntValuesValide(my_I_incrementer, G_P_D_TRESHOLD_MIN, G_P_D_TRESHOLD_MAX, mySettings.pitchDetectTreshold + my_I_incrementer))
        mySettings.pitchDetectTreshold += my_I_incrementer;
      
      if(mySettings.pitchDetectTreshold < 0) mySettings.pitchDetectTreshold = 0;
      if(mySettings.pitchDetectTreshold > 15000) mySettings.pitchDetectTreshold = 15000;
       
     
      lcd.print("Pitch Detect Trs");
      lcd.setCursor(0, 1);
      lcd.print(mySettings.pitchDetectTreshold);
      //lcd.print("    ");
      menuExtrButton = false;  break;

     case 4 + 16: // on PITCH detect quality
      my_F_incrementer = float(updown/100.0);//0.01 steps
      mySettings.pitchDetectQuality = getFloatValuesValide(my_F_incrementer, G_P_D_QUALITY_MIN, G_P_D_QUALITY_MAX, mySettings.pitchDetectQuality);
      
      lcd.print("Pitch Detect Q.");
      lcd.setCursor(0, 1);
      lcd.print(int(mySettings.pitchDetectQuality*100));
      lcd.print("%");
      menuExtrButton = false;  break;

// SYNTHS
     case 4 + 17: // OSC1 level
      my_F_incrementer = float(updown/100.0);//0.1 steps
      mySettings.osc1_level = getFloatValuesValide(my_F_incrementer, 0.0, 1.0, mySettings.osc1_level);   
      
      if(updown!=0) Osc1.amplitude(mySettings.osc1_level);
       
      lcd.print("OSC 1 Volumen");
      lcd.setCursor(0, 1);
      //lcd.print(int(mySettings.osc1_level*100));
      lcd.print(int(powf(mySettings.osc1_level, 0.2)*100));
      lcd.print("%");
      menuExtrButton = false;
      break;
      
     case 4 + 18: // OSC1 waveform
      my_I_incrementer = updown; // 10 steps
      if(checkIntValuesValide(my_I_incrementer, 0, 22, mySettings.osc1_waveform + my_I_incrementer))
        mySettings.osc1_waveform += my_I_incrementer;
        
      if(updown!=0 && mySettings.osc1_waveform > 5) selectABTWaveTableOSC1( mySettings.osc1_waveform -6);
      if(updown!=0 && mySettings.osc1_waveform < 6) Osc1.begin(mySettings.osc1_level, osc1hz , waveIntsLook[mySettings.osc1_waveform] );
      
      lcd.print("OSC 1 Waveform");
      lcd.setCursor(0, 1);
      if(mySettings.osc1_waveform > 5)
        lcd.printf("AKWF %d",mySettings.osc1_waveform -6);
      else
        lcd.print(waveNamesLook[mySettings.osc1_waveform]);
        //lcd.print("    ");
      menuExtrButton = false;  break;
      
     case 4 + 19: // OSC1 pulsewidth
      my_F_incrementer = float(updown/100.0);//0.01 steps
      mySettings.osc1_pulseWidth = getFloatValuesValide(my_F_incrementer, 0.01, 1.0, mySettings.osc1_pulseWidth);
      
      if(updown!=0) Osc1.pulseWidth(mySettings.osc1_pulseWidth);
      //Serial.println(mySettings.osc1_pulseWidth);
       
      lcd.print("OSC 1 Pulsewidth");
      lcd.setCursor(0, 1);
      if(mySettings.osc1_waveform == 5){
        lcd.print(int(mySettings.osc1_pulseWidth*100));
        lcd.print("%");
      }
      else{
        lcd.print("unused");
        //lcd.print("     ");
      }
      menuExtrButton = false;  break;
      
     case 4 + 20: // OSC1 octave
      my_I_incrementer = updown;
      if(checkIntValuesValide(my_I_incrementer, -3, 5, mySettings.osc1_octave + my_I_incrementer))
        mySettings.osc1_octave += my_I_incrementer;

      lcd.print("OSC 1 Octave +|-");
      lcd.setCursor(0, 1);
      lcd.print(mySettings.osc1_octave);
      lcd.print(" Octaves");
      menuExtrButton = false;  break;

     case 4 + 21: // OSC1 note
      my_I_incrementer = updown;
      if(checkIntValuesValide(my_I_incrementer, -12, 12, mySettings.osc1_note + my_I_incrementer))
        mySettings.osc1_note += my_I_incrementer;

      lcd.print("OSC 1 Note +|-");
      lcd.setCursor(0, 1);
      lcd.print(mySettings.osc1_note);
      lcd.print(" Notes");
      menuExtrButton = false;  break;

     case 4 + 22: // OSC1 detune
      my_I_incrementer = updown;
      if(checkIntValuesValide(my_I_incrementer, -12, 12, mySettings.osc1_detune + my_I_incrementer))
        mySettings.osc1_detune+= my_I_incrementer;

      lcd.print("OSC 1 Detune +|-");
      lcd.setCursor(0, 1);
      lcd.print(mySettings.osc1_detune);
      lcd.print(" x 4,99 Cents");
      menuExtrButton = false;  break;

     case 4 + 23: // OSC1 Phase
      my_I_incrementer = updown;
      if(checkIntValuesValide(my_I_incrementer, 0, 360, mySettings.osc1_phase + my_I_incrementer))
        mySettings.osc1_phase+= my_I_incrementer;
      
      if(updown!=0) Osc1.phase(mySettings.osc1_phase);
      lcd.print("OSC 1 Phase");
      lcd.setCursor(0, 1);
      lcd.print(mySettings.osc1_phase);
      lcd.print(" Deg.");
      menuExtrButton = false;  break;


     // osc2 #####################################################
     case 4 + 24: // OSC2 level
      my_F_incrementer = float(updown/100.0);//0.1 steps
      mySettings.osc2_level = getFloatValuesValide(my_F_incrementer, 0.0, 1.0, mySettings.osc2_level);

      if(updown!=0) Osc2.amplitude(mySettings.osc2_level);
       
      lcd.print("OSC 2 Volumen");
      lcd.setCursor(0, 1);
      //lcd.print(int(mySettings.osc2_level*100));
      lcd.print(int(powf(mySettings.osc2_level, 0.2)*100));
      lcd.print("%");
      menuExtrButton = false;
      break;
      
     case 4 + 25: // OSC2 waveform
      my_I_incrementer = updown; // 10 steps
      if(checkIntValuesValide(my_I_incrementer, 0, 5, mySettings.osc2_waveform + my_I_incrementer))
        mySettings.osc2_waveform += my_I_incrementer;
        
      //if(updown!=0 && mySettings.osc2_waveform > 5) selectABTWaveTableOSC1(1, mySettings.osc2_waveform -6);
      if(updown!=0 /*&& mySettings.osc2_waveform < 6*/) Osc2.begin(mySettings.osc2_level, osc2hz , waveIntsLook[mySettings.osc2_waveform] );
      //if(updown!=0) Osc2.begin( mySettings.osc2_level, osc2hz , waveIntsLook[mySettings.osc2_waveform]  );

      lcd.print("OSC 2 Waveform");
      lcd.setCursor(0, 1);
      if(mySettings.osc2_waveform > 5)
        lcd.printf("AKWF %d",mySettings.osc2_waveform -6);
      else
        lcd.print(waveNamesLook[mySettings.osc2_waveform]);
      menuExtrButton = false;
      break;
      
     case 4 + 26: // OSC2 pulsewidth
      my_F_incrementer = float(updown/100.0);//0.01 steps
      mySettings.osc2_pulseWidth = getFloatValuesValide(my_F_incrementer, 0.1, 1.0, mySettings.osc2_pulseWidth);
      
      if(updown!=0) Osc2.pulseWidth(mySettings.osc2_pulseWidth);
       
      lcd.print("OSC 2 Pulsewidth");
      lcd.setCursor(0, 1);
      if(mySettings.osc2_waveform == 5){
        lcd.print(int(mySettings.osc2_pulseWidth*100));
        lcd.print("%");
      }
      else{
        lcd.print("unused");
        //lcd.print("     ");
      }
      
      menuExtrButton = false;  break;
      
     case 4 + 27: // OSC2 octave
      my_I_incrementer = updown;
      if(checkIntValuesValide(my_I_incrementer, -3, 5, mySettings.osc2_octave + my_I_incrementer))
        mySettings.osc2_octave += my_I_incrementer;

      lcd.print("OSC 2 Octave +|-");
      lcd.setCursor(0, 1);
      lcd.print(mySettings.osc2_octave);
      lcd.print(" Octaves");
      menuExtrButton = false;  break;

     case 4 + 28: // OSC2 note
      my_I_incrementer = updown;
      if(checkIntValuesValide(my_I_incrementer, -12, 12, mySettings.osc2_note + my_I_incrementer))
        mySettings.osc2_note += my_I_incrementer;

      lcd.print("OSC 2 Note +|-");
      lcd.setCursor(0, 1);
      lcd.print(mySettings.osc2_note);
      lcd.print(" Notes");
      menuExtrButton = false;  break;

     case 4 + 29: // OSC1 note
      my_I_incrementer = updown;
      if(checkIntValuesValide(my_I_incrementer, -12, 12, mySettings.osc2_detune + my_I_incrementer))
        mySettings.osc2_detune+= my_I_incrementer;

      lcd.print("OSC 2 Detune +|-");
      lcd.setCursor(0, 1);
      lcd.print(mySettings.osc2_detune);
      lcd.print(" x 4,99 Cents");
      menuExtrButton = false;  break;

     case 4 + 30: // OSC1 Phase
      my_I_incrementer = updown;
      if(checkIntValuesValide(my_I_incrementer, 0, 360, mySettings.osc2_phase + my_I_incrementer))
        mySettings.osc2_phase+= my_I_incrementer;
      
      if(updown!=0) Osc2.phase(mySettings.osc2_phase);
      lcd.print("OSC 2 Phase");
      lcd.setCursor(0, 1);
      lcd.print(mySettings.osc2_phase);
      lcd.print(" Deg.");
      menuExtrButton = false;  break;



      // Sub OSC1

     case 4 + 31: // SubOSC1 level
      my_F_incrementer = float(updown/100.0);//0.1 steps
      mySettings.sub_osc1_level = getFloatValuesValide(my_F_incrementer, 0.0, 1.0, mySettings.sub_osc1_level);
      
      if(updown!=0) SubOsc1.amplitude(mySettings.sub_osc1_level);
       
      lcd.print("SubOSC 1 Volumen");
      lcd.setCursor(0, 1);
      //lcd.print(int(mySettings.sub_osc1_level*100));
      lcd.print(int(powf(mySettings.sub_osc1_level, 0.2)*100));
      lcd.print("%");
      menuExtrButton = false;
      break;
      
     case 4 + 32: // SubOSC1 waveform
      my_I_incrementer = updown; // 10 steps
      if(checkIntValuesValide(my_I_incrementer, 0, 5, mySettings.sub_osc1_waveform + my_I_incrementer))
        mySettings.sub_osc1_waveform += my_I_incrementer;
      
      //if(updown!=0 && mySettings.sub_osc1_waveform > 5) selectABTWaveTableOSC1(1, mySettings.sub_osc1_waveform -6);
      if(updown!=0 /*&& mySettings.sub_osc1_waveform < 6*/) SubOsc1.begin(mySettings.sub_osc1_level, subosc1hz , waveIntsLook[mySettings.sub_osc1_waveform] );

      lcd.print("SubOSC1 Waveform");
      lcd.setCursor(0, 1);
      if(mySettings.sub_osc1_waveform > 5)
        lcd.printf("AKWF %d",mySettings.sub_osc1_waveform -6);
      else
        lcd.print(waveNamesLook[mySettings.sub_osc1_waveform]);
      menuExtrButton = false;
      break;
      
     case 4 + 33: // SubOSC1 pulsewidth
      my_F_incrementer = float(updown/100.0);//0.01 steps
      mySettings.sub_osc1_pulseWidth = getFloatValuesValide(my_F_incrementer, 0.1, 1.0, mySettings.sub_osc1_pulseWidth);
      
      if(updown!=0) SubOsc1.pulseWidth(mySettings.sub_osc1_pulseWidth);
       
      lcd.print("SubOSC1 Pulsew.");
      lcd.setCursor(0, 1);
      if(mySettings.sub_osc1_waveform == 5){
        lcd.print(int(mySettings.sub_osc1_pulseWidth*100));
        lcd.print("%");
      }
      else{
        lcd.print("unused");
        //lcd.print("     ");
      }
      menuExtrButton = false;  break;
      
     case 4 + 34: // SubOSC1 octave
      my_I_incrementer = updown;
      if(checkIntValuesValide(my_I_incrementer, -3, 5, mySettings.sub_osc1_octave + my_I_incrementer))
        mySettings.sub_osc1_octave += my_I_incrementer;

      lcd.print("SubOSC1 Oct. +|-");
      lcd.setCursor(0, 1);
      lcd.print(mySettings.sub_osc1_octave);
      lcd.print(" Octaves");
      menuExtrButton = false;  break;

     case 4 + 35: // SubOSC1 note
      my_I_incrementer = updown;
      if(checkIntValuesValide(my_I_incrementer, -12, 12, mySettings.sub_osc1_note + my_I_incrementer))
        mySettings.sub_osc1_note += my_I_incrementer;

      lcd.print("SubOSC1 Note +|-");
      lcd.setCursor(0, 1);
      lcd.print(mySettings.sub_osc1_note);
      lcd.print(" Notes");
      menuExtrButton = false;  break;

     case 4 + 36: // SubOSC1 note
      my_I_incrementer = updown;
      if(checkIntValuesValide(my_I_incrementer, -12, 12, mySettings.sub_osc1_detune + my_I_incrementer))
        mySettings.sub_osc1_detune+= my_I_incrementer;

      lcd.print("SubOSC1 Det. +|-");
      lcd.setCursor(0, 1);
      lcd.print(mySettings.sub_osc1_detune);
      lcd.print(" x 4,99 Cents");
      menuExtrButton = false;  break;
 
     case 4 + 37: // SubOSC1 Phase
      my_I_incrementer = updown;
      if(checkIntValuesValide(my_I_incrementer, 0, 360, mySettings.sub_osc1_phase + my_I_incrementer))
        mySettings.sub_osc1_phase+= my_I_incrementer;
      
      if(updown!=0) SubOsc1.phase(mySettings.sub_osc1_phase);
      lcd.print("SubOSC 1 Phase");
      lcd.setCursor(0, 1);
      lcd.print(mySettings.sub_osc1_phase);
      lcd.print(" Deg.");
      menuExtrButton = false;
     break;

      // Sub OSC2

     case 4 + 38: // SubOSC2 level
      my_F_incrementer = float(updown/100.0);//0.1 steps
      mySettings.sub_osc2_level = getFloatValuesValide(my_F_incrementer, 0.0, 1.0, mySettings.sub_osc2_level);
      
      if(updown!=0) SubOsc2.amplitude(mySettings.sub_osc2_level);
       
      lcd.print("SubOSC 2 Volumen");
      lcd.setCursor(0, 1);
      //lcd.print(int(mySettings.sub_osc2_level*100));
      lcd.print(int(powf(mySettings.sub_osc2_level, 0.2)*100));
      lcd.print("%");
      menuExtrButton = false;
     break;
     
     case 4 + 39: // SubOSC2 waveform
      my_I_incrementer = updown; // 10 steps
      if(checkIntValuesValide(my_I_incrementer, 0, 5, mySettings.sub_osc2_waveform + my_I_incrementer))
        mySettings.sub_osc2_waveform += my_I_incrementer;

      //if(updown!=0 && mySettings.sub_osc2_waveform > 5) selectABTWaveTableOSC1(1, mySettings.sub_osc2_waveform -6);
      if(updown!=0 /*&& mySettings.sub_osc2_waveform < 6*/) SubOsc2.begin(mySettings.sub_osc2_level, subosc2hz , waveIntsLook[mySettings.sub_osc2_waveform] );

      lcd.print("SubOSC2 Waveform");
      lcd.setCursor(0, 1);
      if(mySettings.sub_osc2_waveform > 5)
        lcd.printf("AKWF %d",mySettings.sub_osc2_waveform -6);
      else
        lcd.print(waveNamesLook[mySettings.sub_osc2_waveform]);
      menuExtrButton = false;
      break;
      
     case 4 + 40: // SubOSC2 pulsewidth
      my_F_incrementer = float(updown/100.0);//0.01 steps
      mySettings.sub_osc2_pulseWidth = getFloatValuesValide(my_F_incrementer, 0.1, 1.0, mySettings.sub_osc2_pulseWidth);
      
      if(updown!=0) SubOsc2.pulseWidth(mySettings.sub_osc2_pulseWidth);
       
      lcd.print("SubOSC2 Pulsew.");
      lcd.setCursor(0, 1);
      if(mySettings.sub_osc2_waveform == 5){
        lcd.print(int(mySettings.sub_osc2_pulseWidth*100));
        lcd.print("%");
      }
      else{
        lcd.print("unused");
        //lcd.print("     ");
      }
      menuExtrButton = false;  break;
      
     case 4 + 41: // SubOSC2 octave
      my_I_incrementer = updown;
      if(checkIntValuesValide(my_I_incrementer, -3, 5, mySettings.sub_osc2_octave + my_I_incrementer))
        mySettings.sub_osc2_octave += my_I_incrementer;

      lcd.print("SubOSC2 Oct. +|-");
      lcd.setCursor(0, 1);
      lcd.print(mySettings.sub_osc2_octave);
      lcd.print(" Octaves");
      menuExtrButton = false;  break;

     case 4 + 42: // SubOSC2 note
      my_I_incrementer = updown;
      if(checkIntValuesValide(my_I_incrementer, -12, 12, mySettings.sub_osc2_note + my_I_incrementer))
        mySettings.sub_osc2_note += my_I_incrementer;

      lcd.print("SubOSC2 Note +|-");
      lcd.setCursor(0, 1);
      lcd.print(mySettings.sub_osc2_note);
      lcd.print(" Notes");
      menuExtrButton = false;  break;

     case 4 + 43: // SubOSC2 cent
      my_I_incrementer = updown;
      if(checkIntValuesValide(my_I_incrementer, -12, 12, mySettings.sub_osc2_detune + my_I_incrementer))
        mySettings.sub_osc2_detune+= my_I_incrementer;

      lcd.print("SubOSC2 Det. +|-");
      lcd.setCursor(0, 1);
      lcd.print(mySettings.sub_osc2_detune);
      lcd.print(" x 4,99 Cents");
      menuExtrButton = false;  break;
 
     case 4 + 44: // SubOSC2 Phase
      my_I_incrementer = updown;
      if(checkIntValuesValide(my_I_incrementer, 0, 360, mySettings.sub_osc2_phase + my_I_incrementer))
        mySettings.sub_osc2_phase+= my_I_incrementer;
      
      if(updown!=0) SubOsc2.phase(mySettings.sub_osc2_phase);
      lcd.print("SubOSC 2 Phase");
      lcd.setCursor(0, 1);
      lcd.print(mySettings.sub_osc2_phase);
      lcd.print(" Deg.");
      menuExtrButton = false;  break;
     
     case 4 + 45: // Noise level
      my_F_incrementer = float(updown/100.0);//0.1 steps
      mySettings.noise_level = getFloatValuesValide(my_F_incrementer, 0.0, 1.0, mySettings.noise_level);
      
      if(updown!=0) noise1.amplitude(mySettings.noise_level);
       
      lcd.print("W-Noise Volumen");
      lcd.setCursor(0, 1);
      //lcd.print(int(mySettings.noise_level*100));
      lcd.print(int(powf(mySettings.noise_level, 0.2)*100));
      lcd.print("%");
      menuExtrButton = false;  break;

     //  Bitcrusher

     case 4 + 46: // Bitcrusher Volumen
      my_F_incrementer = float(updown/100.0);//0.1 steps
      mySettings.Crusher_Volumen = getFloatValuesValide(my_F_incrementer, 0.0, 1.0, mySettings.Crusher_Volumen);
      
      if(updown!=0) SubOscMixer.gain(2, mySettings.Crusher_Volumen);
       
      lcd.print("Git Crusher Gain");
      lcd.setCursor(0, 1);
      //lcd.print(int(mySettings.Crusher_Volumen*100));
      lcd.print(int(powf(mySettings.Crusher_Volumen, 0.2)*100));
      lcd.print("%");
      menuExtrButton = false;
      break;
      
     case 4 + 47: // Bitcrusher Bits
      my_I_incrementer = updown;
      if(checkIntValuesValide(my_I_incrementer, 1, 16, mySettings.Crusher_Bits + my_I_incrementer))
        mySettings.Crusher_Bits+= my_I_incrementer;
      
      if(updown!=0) bitcrusher1.bits(mySettings.Crusher_Bits);
      lcd.print("Git Crusher Bits");
      lcd.setCursor(0, 1);
      lcd.print(mySettings.Crusher_Bits);
      lcd.print(" Bits");
      menuExtrButton = false;  break; 
          
     case 4 + 48: // Bitcrusher Samplerate
     
      my_I_incrementer = updown;
      if(checkIntValuesValide(my_I_incrementer, 0, 6, mySettings.Crusher_SampleRate + my_I_incrementer))
        mySettings.Crusher_SampleRate+= my_I_incrementer;
      if(DEB_MENUE) Serial.printf("samplerate lookup %d: entspricht: %f\n",mySettings.Crusher_SampleRate, bitCrusherSampleRate[mySettings.Crusher_SampleRate]);
      
      if(updown!=0) bitcrusher1.sampleRate(bitCrusherSampleRate[mySettings.Crusher_SampleRate]);
      lcd.print("Git Crusher Rate");
      lcd.setCursor(0, 1);
      lcd.print(int(bitCrusherSampleRate[mySettings.Crusher_SampleRate]));
      lcd.print("Hz");
      menuExtrButton = false;
      break;
     
     // Control Matrix ##############################      
     
     // MOD LFO
    case 4 + 49: // MOD LFO waveform
      my_I_incrementer = updown; // 10 steps
      if(checkIntValuesValide(my_I_incrementer, 0, 11, mySettings.mod_lfo_waveform + my_I_incrementer))
        mySettings.mod_lfo_waveform += my_I_incrementer;
      if(updown!=0 && mySettings.mod_lfo_waveform > 5) selectABTWaveTableModLFO( bpmToHz(mySettings.mod_lfo_bpm), mySettings.mod_lfo_waveform -6);
      if(updown!=0 && mySettings.mod_lfo_waveform < 6) MOD_LFO.begin(0.5, bpmToHz(mySettings.mod_lfo_bpm) * beatMultiLookup[mySettings.mod_lfo_freq_bpm_multi] , waveIntsLook[mySettings.mod_lfo_waveform], mySettings.mod_lfo_cycles);
      
      lcd.print("MX M-LFO Wavefo");
      lcd.setCursor(0, 1);
      lcd.print(LFOwaveNamesLook[mySettings.mod_lfo_waveform]);
      //lcd.print("    ");
      menuExtrButton = false;  break;
      
     case 4 + 50: // MOD LFO pulsewidth
      my_F_incrementer = float(updown/100.0);//0.01 steps
      mySettings.mod_lfo_pulseWidth = getFloatValuesValide(my_F_incrementer, 0.1, 1.0, mySettings.mod_lfo_pulseWidth);
      
      if(updown!=0) MOD_LFO.pulseWidth(mySettings.mod_lfo_pulseWidth);
       
      lcd.print("MX M-LFO Pulsew.");
      lcd.setCursor(0, 1);
      if(mySettings.mod_lfo_waveform == 5){
        lcd.print(int(mySettings.mod_lfo_pulseWidth*100));
        lcd.print("%");
      }
      else{
        lcd.print("unused");
        //lcd.print("     ");
      }
      menuExtrButton = false;  break;
      
     case 4 + 51: // Mod Lfo Bpm
      my_F_incrementer = float(updown/10.0);//0.1 steps

      if(mySettings.MasterClockToLfos || mySettings.MasterClockToMODLfo){
        lcd.print("MX M-LFO BPM ");
        lcd.setCursor(0, 1);
        lcd.print("on CLOCK SYNC");
        menuExtrButton = false;
      break;       
      }
      
      if(updown!=0 ) mySettings.mod_lfo_bpm = getFloatValuesValide(my_F_incrementer, 0.0, 360.0, mySettings.mod_lfo_bpm);

      lcd.print("MX M-LFO BPM ");
      lcd.setCursor(0, 1);
      lcd.printf("%4.2f",mySettings.mod_lfo_bpm);
      lcd.print(" BPM");
      menuExtrButton = false;
      break;

     case 4 + 52: // Mod Lfo Bpm multip
      my_I_incrementer = updown;//
      if(checkIntValuesValide(my_I_incrementer, 0, 20, mySettings.mod_lfo_freq_bpm_multi + my_I_incrementer))
        mySettings.mod_lfo_freq_bpm_multi += my_I_incrementer;
           
      lcd.print("MX M-LFO BPM x");
      lcd.setCursor(0, 1);
      if(mySettings.MasterClockToLfos || mySettings.MasterClockToMODLfo){
         lcd.printf("%4.1f %s",mySystemBpm, beatMultiNamesLookup[mySettings.mod_lfo_freq_bpm_multi]);
      }
      else
        lcd.printf("%4.1f %s",mySettings.mod_lfo_bpm, beatMultiNamesLookup[mySettings.mod_lfo_freq_bpm_multi]);
      menuExtrButton = false;  break;

     case 4 + 53: // Mod LFO Phase
      my_I_incrementer = updown;
      if(checkIntValuesValide(my_I_incrementer, 0, 360, mySettings.mod_lfo_phase + my_I_incrementer))
        mySettings.mod_lfo_phase+= my_I_incrementer;
      
      if(updown!=0) MOD_LFO.phase(mySettings.mod_lfo_phase);
      lcd.print("MX M-LFO Phase");
      lcd.setCursor(0, 1);
      lcd.print(mySettings.mod_lfo_phase);
      lcd.print(" Deg.");
      menuExtrButton = false;  break;

     case 4 + 54: // MOD lfo Intensity T1 - T4 Sync
      lcd.print("MX M-LFO F1-F4 S");
      lcd.setCursor(0, 1);
      if(updown > 0){
        mySettings.mod_lfo_T1_T4_FREQ_intensety_sync = true;
      }
      if(updown < 0){
        mySettings.mod_lfo_T1_T4_FREQ_intensety_sync = false;
      }
      
      if(mySettings.mod_lfo_T1_T4_FREQ_intensety_sync) lcd.print("On");
      else lcd.print("Off");
      menuExtrButton = false;
     break; 

     case 4 + 55:
      my_F_incrementer = float(updown/600.0);
      
      mySettings.mod_lfo_T1_OSC1_FREQ_intensety = getFloatValuesValide(my_F_incrementer, 0.0, 1.0, mySettings.mod_lfo_T1_OSC1_FREQ_intensety);

      if(mySettings.mod_lfo_T1_T4_FREQ_intensety_sync){
        mySettings.mod_lfo_T2_OSC2_FREQ_intensety = mySettings.mod_lfo_T1_OSC1_FREQ_intensety;
        mySettings.mod_lfo_T3_SUB1_FREQ_intensety = mySettings.mod_lfo_T1_OSC1_FREQ_intensety;
        mySettings.mod_lfo_T4_SUB2_FREQ_intensety = abs(mySettings.mod_lfo_T1_OSC1_FREQ_intensety);
      } 
      
      lcd.print("MX M-LFO->OSC1-F");
      lcd.setCursor(0, 1);
      if(mySettings.mod_lfo_T1_OSC1_FREQ_intensety > 0.001 ){

        // die berrechnung
        float local_notes = 0.0f;
        float local_cents = 0.0f;       

        local_notes = 12*mySettings.mod_lfo_T1_OSC1_FREQ_intensety;
        local_cents = fmod( local_notes,1 )*100;
              
        lcd.printf("%d N, %d C",int(local_notes), int(local_cents)); //!! hier die magic für den betrachter. das heist werte von 1.0x -2.0 oder 0.9x -0 in noten und cents darstellen
      }
      else
      {
         lcd.print("Off");
      }
      menuExtrButton = false;  break;

     case 4 + 56: // MOD LFO OSC2
      my_F_incrementer = float(updown/600.0);
      
      mySettings.mod_lfo_T2_OSC2_FREQ_intensety = getFloatValuesValide(my_F_incrementer, 0.0, 1.0, mySettings.mod_lfo_T2_OSC2_FREQ_intensety);

      if(mySettings.mod_lfo_T1_T4_FREQ_intensety_sync){
        mySettings.mod_lfo_T1_OSC1_FREQ_intensety = mySettings.mod_lfo_T2_OSC2_FREQ_intensety;
        mySettings.mod_lfo_T3_SUB1_FREQ_intensety = mySettings.mod_lfo_T2_OSC2_FREQ_intensety;
        mySettings.mod_lfo_T4_SUB2_FREQ_intensety = abs(mySettings.mod_lfo_T2_OSC2_FREQ_intensety);
      } 
      
      lcd.print("MX M-LFO->OSC2-F");
      lcd.setCursor(0, 1);
      if(mySettings.mod_lfo_T2_OSC2_FREQ_intensety > 0.001 ){

        // die berrechnung
        float local_notes = 0.0f;
        float local_cents = 0.0f;       

        local_notes = 12*mySettings.mod_lfo_T2_OSC2_FREQ_intensety;
        local_cents = fmod( local_notes,1 )*100;
              
        lcd.printf("%d N, %d C",int(local_notes), int(local_cents)); //!! hier die magic für den betrachter. das heist werte von 1.0x -2.0 oder 0.9x -0 in noten und cents darstellen
      }
      else
      {
         lcd.print("Off");
      }
      menuExtrButton = false;  break;



     case 4 + 57: // SUB1
      my_F_incrementer = float(updown/600.0);
      
      mySettings.mod_lfo_T3_SUB1_FREQ_intensety = getFloatValuesValide(my_F_incrementer, 0.0, 1.0, mySettings.mod_lfo_T3_SUB1_FREQ_intensety);

      if(mySettings.mod_lfo_T1_T4_FREQ_intensety_sync){
        mySettings.mod_lfo_T1_OSC1_FREQ_intensety = mySettings.mod_lfo_T3_SUB1_FREQ_intensety;
        mySettings.mod_lfo_T2_OSC2_FREQ_intensety = mySettings.mod_lfo_T3_SUB1_FREQ_intensety;
        mySettings.mod_lfo_T4_SUB2_FREQ_intensety = abs(mySettings.mod_lfo_T3_SUB1_FREQ_intensety);
      } 
      
      lcd.print("MX M-LFO->SUB1-F");
      lcd.setCursor(0, 1);
      if(mySettings.mod_lfo_T3_SUB1_FREQ_intensety > 0.001 ){

        // die berrechnung
        float local_notes = 0.0f;
        float local_cents = 0.0f;       

        local_notes = 12*mySettings.mod_lfo_T3_SUB1_FREQ_intensety;
        local_cents = fmod( local_notes,1 )*100;
              
        lcd.printf("%d N, %d C",int(local_notes), int(local_cents)); //!! hier die magic für den betrachter. das heist werte von 1.0x -2.0 oder 0.9x -0 in noten und cents darstellen
      }
      else
      {
         lcd.print("Off");
      }
      menuExtrButton = false;  break;

     case 4 + 58: // MOD LFO SUB2
      my_F_incrementer = float(updown/600.0);
      
      mySettings.mod_lfo_T4_SUB2_FREQ_intensety = getFloatValuesValide(my_F_incrementer, 0.0, 1.0, mySettings.mod_lfo_T4_SUB2_FREQ_intensety);

      if(mySettings.mod_lfo_T1_T4_FREQ_intensety_sync){
        mySettings.mod_lfo_T1_OSC1_FREQ_intensety = mySettings.mod_lfo_T4_SUB2_FREQ_intensety;
        mySettings.mod_lfo_T2_OSC2_FREQ_intensety = mySettings.mod_lfo_T4_SUB2_FREQ_intensety;
        mySettings.mod_lfo_T3_SUB1_FREQ_intensety = abs(mySettings.mod_lfo_T4_SUB2_FREQ_intensety);
      } 
      
      lcd.print("MX M-LFO->SUB2-F");
      lcd.setCursor(0, 1);
      if(mySettings.mod_lfo_T4_SUB2_FREQ_intensety > 0.001 ){

        // die berrechnung
        float local_notes = 0.0f;
        float local_cents = 0.0f;       

        local_notes = 12*mySettings.mod_lfo_T4_SUB2_FREQ_intensety;
        local_cents = fmod( local_notes,1 )*100;
              
        lcd.printf("%d N, %d C",int(local_notes), int(local_cents)); //!! hier die magic für den betrachter. das heist werte von 1.0x -2.0 oder 0.9x -0 in noten und cents darstellen
      }
      else
      {
         lcd.print("Off");
      }
      menuExtrButton = false;  break;



     /// Mod LFO PULSE
     
     case 4 + 59: // MOD lfo Pulse Intensity T1 - T4 Sync
      lcd.print("MX M-LFO P1-P4 S");
      lcd.setCursor(0, 1);
      if(updown > 0){
        mySettings.mod_lfo_T1_T4_PULSE_intensety_sync = true;
      }
      if(updown < 0){
        mySettings.mod_lfo_T1_T4_PULSE_intensety_sync = false;
      }
      
      if(mySettings.mod_lfo_T1_T4_PULSE_intensety_sync) lcd.print("On");
      else lcd.print("Off");
      menuExtrButton = false;  break; 

     case 4 + 60: // Mod Lfo Depth T1 OSC1
      my_F_incrementer = float(updown/100.0);//0.1 steps
      mySettings.mod_lfo_T1_OSC1_PULSE_intensety = getFloatValuesValide(my_F_incrementer, -1.0, 1.0, mySettings.mod_lfo_T1_OSC1_PULSE_intensety);

      if(mySettings.mod_lfo_T1_T4_PULSE_intensety_sync){
        mySettings.mod_lfo_T2_OSC2_PULSE_intensety = mySettings.mod_lfo_T1_OSC1_PULSE_intensety;
        mySettings.mod_lfo_T3_SUB1_PULSE_intensety = mySettings.mod_lfo_T1_OSC1_PULSE_intensety;
        mySettings.mod_lfo_T4_SUB2_PULSE_intensety = mySettings.mod_lfo_T1_OSC1_PULSE_intensety;
      }
       
      lcd.print("MX M-LFO->OSC1-P");
      lcd.setCursor(0, 1);
      if(mySettings.mod_lfo_T1_OSC1_PULSE_intensety > 0.01 || mySettings.mod_lfo_T1_OSC1_PULSE_intensety < -0.01  ){
       lcd.print(mySettings.mod_lfo_T1_OSC1_PULSE_intensety);
      }
      else lcd.print("Off");
      menuExtrButton = false;
     break;

     case 4 + 61: // Vol Lfo Depth T2 OSC2
      my_F_incrementer = float(updown/100.0);//0.1 steps
      mySettings.mod_lfo_T2_OSC2_PULSE_intensety = getFloatValuesValide(my_F_incrementer, -1.0, 1.0, mySettings.mod_lfo_T2_OSC2_PULSE_intensety);

      if(mySettings.mod_lfo_T1_T4_PULSE_intensety_sync){
        mySettings.mod_lfo_T1_OSC1_PULSE_intensety = mySettings.mod_lfo_T2_OSC2_PULSE_intensety;
        mySettings.mod_lfo_T3_SUB1_PULSE_intensety = mySettings.mod_lfo_T2_OSC2_PULSE_intensety;
        mySettings.mod_lfo_T4_SUB2_PULSE_intensety = mySettings.mod_lfo_T2_OSC2_PULSE_intensety;
      }
             
      lcd.print("MX M-LFO->OSC2-P");
      lcd.setCursor(0, 1);
      if(mySettings.mod_lfo_T2_OSC2_PULSE_intensety > 0.01 || mySettings.mod_lfo_T2_OSC2_PULSE_intensety < -0.01  ){
        lcd.print(mySettings.mod_lfo_T2_OSC2_PULSE_intensety);
      }
      else lcd.print("Off");
      menuExtrButton = false;  break;
      
     case 4 + 62: // Mod Lfo Depth T3 SUB1
      my_F_incrementer = float(updown/100.0);//0.1 steps
      mySettings.mod_lfo_T3_SUB1_PULSE_intensety = getFloatValuesValide(my_F_incrementer, -1.0, 1.0, mySettings.mod_lfo_T3_SUB1_PULSE_intensety);

      if(mySettings.mod_lfo_T1_T4_PULSE_intensety_sync){
        mySettings.mod_lfo_T1_OSC1_PULSE_intensety = mySettings.mod_lfo_T3_SUB1_PULSE_intensety;
        mySettings.mod_lfo_T2_OSC2_PULSE_intensety = mySettings.mod_lfo_T3_SUB1_PULSE_intensety;
        mySettings.mod_lfo_T4_SUB2_PULSE_intensety = abs(mySettings.mod_lfo_T3_SUB1_PULSE_intensety);
      }
             
      lcd.print("MX M-LFO->SUB1-P");
      lcd.setCursor(0, 1);
      if(mySettings.mod_lfo_T3_SUB1_PULSE_intensety > 0.01 || mySettings.mod_lfo_T3_SUB1_PULSE_intensety < -0.01  )
        lcd.print(mySettings.mod_lfo_T3_SUB1_PULSE_intensety);
      else lcd.print("Off");
      menuExtrButton = false;  break;
      
     case 4 + 63: // Mod Lfo Depth T4 SUB2
      my_F_incrementer = float(updown/100.0);//0.1 steps
      mySettings.mod_lfo_T4_SUB2_PULSE_intensety = getFloatValuesValide(my_F_incrementer, 0.0, 1.0, mySettings.mod_lfo_T4_SUB2_PULSE_intensety);

      if(mySettings.mod_lfo_T1_T4_PULSE_intensety_sync){
        mySettings.mod_lfo_T1_OSC1_PULSE_intensety = mySettings.mod_lfo_T4_SUB2_PULSE_intensety;
        mySettings.mod_lfo_T2_OSC2_PULSE_intensety = mySettings.mod_lfo_T4_SUB2_PULSE_intensety;
        mySettings.mod_lfo_T3_SUB1_PULSE_intensety = mySettings.mod_lfo_T4_SUB2_PULSE_intensety;
      }
             
      lcd.print("MX M-LFO->SUB2-P");
      lcd.setCursor(0, 1);
      if(mySettings.mod_lfo_T4_SUB2_PULSE_intensety > 0.01 || mySettings.mod_lfo_T4_SUB2_PULSE_intensety < -0.01  )
        lcd.print(mySettings.mod_lfo_T4_SUB2_PULSE_intensety);
      else lcd.print("Off");
      menuExtrButton = false;  break;


     
      // auch noch für mod lfo
     case 4 + 64: // Mod lfo sync to attack
      lcd.print("MX M-LFO Syn2At");
      lcd.setCursor(0, 1);
      if(updown > 0){
        mySettings.mod_lfo_attack_sync = true;
      }
      if(updown < 0){
        mySettings.mod_lfo_attack_sync = false;
      }
      
      if(mySettings.mod_lfo_attack_sync) lcd.print("On");
      else lcd.print("Off");
      menuExtrButton = false;
     break;     

     case 4 + 65:// Attack envelop to MODLFO Depth
      my_F_incrementer = float(updown/100.0);//0.01 steps
      mySettings.mod_lfo_follow_envelope_attack = getFloatValuesValide(my_F_incrementer, -1.0, 1.0, mySettings.mod_lfo_follow_envelope_attack);
          
      lcd.print("MX AtEn>M-LFO-In");
      lcd.setCursor(0, 1);
      lcd.print(mySettings.mod_lfo_follow_envelope_attack * 100.0);
      lcd.printf("%");
      menuExtrButton = false;
     break; 
     
     case 4 + 66:// Attack envelop to VolLFO Frequency
      my_F_incrementer = float(updown/10.0);//0.01 steps
      mySettings.envelopeFollowerAttackToMODLFOFrequenzy = getFloatValuesValide(my_F_incrementer, 0.0, 10.0, mySettings.envelopeFollowerAttackToMODLFOFrequenzy);
          
      lcd.print("MX AtEn>M-LFO-FR");
      lcd.setCursor(0, 1);
      lcd.print(mySettings.envelopeFollowerAttackToMODLFOFrequenzy * 3);
      lcd.printf("");
      menuExtrButton = false;
     break; 




      // Vol_LFO ####################################################
     case 4 + 67: // Vol LFO waveform
      my_I_incrementer = updown; // 10 steps
      if(checkIntValuesValide(my_I_incrementer, 0, 11, mySettings.vol_lfo_waveform + my_I_incrementer))
        mySettings.vol_lfo_waveform += my_I_incrementer;

      if(updown!=0 && mySettings.vol_lfo_waveform > 5) selectABTWaveTableVolumenLFO( bpmToHz(mySettings.vol_lfo_bpm), mySettings.vol_lfo_waveform -6);
      if(updown!=0 && mySettings.vol_lfo_waveform < 6) VOLUMEN_LFO.begin( 0.5, bpmToHz(mySettings.vol_lfo_bpm) * beatMultiLookup[mySettings.vol_lfo_freq_bpm_multi] , waveIntsLook[mySettings.vol_lfo_waveform], mySettings.vol_lfo_cycles);
      
      lcd.print("MX V-LFO Wavefo");
      lcd.setCursor(0, 1);
      lcd.print(LFOwaveNamesLook[mySettings.vol_lfo_waveform]);
      //lcd.print("    ");
      menuExtrButton = false;  break;
      
     case 4 + 68: // Vol LFO pulsewidth
      my_F_incrementer = float(updown/100.0);//0.01 steps
      mySettings.vol_lfo_pulseWidth = getFloatValuesValide(my_F_incrementer, 0.1, 1.0, mySettings.vol_lfo_pulseWidth);
      
      if(updown!=0) VOLUMEN_LFO.pulseWidth(mySettings.vol_lfo_pulseWidth);
       
      lcd.print("MX V-LFO Pulsew.");
      lcd.setCursor(0, 1);
      if(mySettings.vol_lfo_waveform == 5){
        lcd.print(int(mySettings.vol_lfo_pulseWidth*100));
        lcd.print("%");
      }
      else{
        lcd.print("unused");
        //lcd.print("     ");
      }
      menuExtrButton = false;  break;
      
     case 4 + 69: // Vol Lfo Bpm
      my_F_incrementer = float(updown/10.0);//0.1 steps
      if(mySettings.MasterClockToLfos || mySettings.MasterClockToVOLLfo){
        lcd.print("MX V-LFO BPM ");
        lcd.setCursor(0, 1);
        lcd.print("on CLOCK SYNC");
        menuExtrButton = false;
      break;       
      }
      mySettings.vol_lfo_bpm = getFloatValuesValide(my_F_incrementer, 0.0, 360.0, mySettings.vol_lfo_bpm);

      lcd.print("MX V-LFO BPM ");
      lcd.setCursor(0, 1);
      lcd.printf("%4.2f",mySettings.vol_lfo_bpm);
      lcd.print(" BPM");
      menuExtrButton = false;  break;

     case 4 + 70: // Vol Lfo Bpm multip
      my_I_incrementer = updown;//
      if(checkIntValuesValide(my_I_incrementer, 0, 20, mySettings.vol_lfo_freq_bpm_multi + my_I_incrementer))
        mySettings.vol_lfo_freq_bpm_multi += my_I_incrementer;
           
      lcd.print("MX V-LFO BPM x");
      lcd.setCursor(0, 1);
      if(mySettings.MasterClockToLfos || mySettings.MasterClockToVOLLfo){
        lcd.printf("%4.1f %s",mySystemBpm, beatMultiNamesLookup[mySettings.vol_lfo_freq_bpm_multi]);
      }
      else
        lcd.printf("%4.1f %s",mySettings.vol_lfo_bpm, beatMultiNamesLookup[mySettings.vol_lfo_freq_bpm_multi]);
      menuExtrButton = false;  break;

     case 4 + 71: // Vol LFO Phase
      my_I_incrementer = updown;
      if(checkIntValuesValide(my_I_incrementer, 0, 360, mySettings.vol_lfo_phase + my_I_incrementer))
        mySettings.vol_lfo_phase+= my_I_incrementer;
      
      if(updown!=0) VOLUMEN_LFO.phase(mySettings.vol_lfo_phase);
      lcd.print("MX V-LFO Phase");
      lcd.setCursor(0, 1);
      lcd.print(mySettings.vol_lfo_phase);
      lcd.print(" Deg.");
      menuExtrButton = false;  break;

     case 4 + 72: // Vol lfo Intensity T1 - T4 Sync
      lcd.print("MX V-LFO T1-T4 S");
      lcd.setCursor(0, 1);
      if(updown > 0){
        mySettings.vol_lfo_T1_T4_intensety_sync = true;
      }
      if(updown < 0){
        mySettings.vol_lfo_T1_T4_intensety_sync = false;
      }
      
      if(mySettings.vol_lfo_T1_T4_intensety_sync) lcd.print("On");
      else lcd.print("Off");
      menuExtrButton = false;  break; 

     case 4 + 73: // Vol Lfo Depth T1 OSC1
      my_F_incrementer = float(updown/100.0);//0.1 steps
      mySettings.vol_lfo_T1_OSC1_intensety = getFloatValuesValide(my_F_incrementer, -1.0, 1.0, mySettings.vol_lfo_T1_OSC1_intensety);

      if(mySettings.vol_lfo_T1_T4_intensety_sync){
        mySettings.vol_lfo_T2_OSC2_intensety = mySettings.vol_lfo_T1_OSC1_intensety;
        mySettings.vol_lfo_T3_SUBS_intensety = mySettings.vol_lfo_T1_OSC1_intensety;
        mySettings.vol_lfo_T4_NOISE_intensety = abs(mySettings.vol_lfo_T1_OSC1_intensety);
      }
       
      lcd.print("MX V-LFO->OSC1");
      lcd.setCursor(0, 1);
      if(mySettings.vol_lfo_T1_OSC1_intensety > 0.005 || mySettings.vol_lfo_T1_OSC1_intensety < -0.005  )
        lcd.print(mySettings.vol_lfo_T1_OSC1_intensety);
      else lcd.print("OFF");
      menuExtrButton = false;  break;

     case 4 + 74: // Vol Lfo Depth T2 OSC2
      my_F_incrementer = float(updown/100.0);//0.1 steps
      mySettings.vol_lfo_T2_OSC2_intensety = getFloatValuesValide(my_F_incrementer, -1.0, 1.0, mySettings.vol_lfo_T2_OSC2_intensety);

      if(mySettings.vol_lfo_T1_T4_intensety_sync){
        mySettings.vol_lfo_T1_OSC1_intensety = mySettings.vol_lfo_T2_OSC2_intensety;
        mySettings.vol_lfo_T3_SUBS_intensety = mySettings.vol_lfo_T2_OSC2_intensety;
        mySettings.vol_lfo_T4_NOISE_intensety = abs(mySettings.vol_lfo_T2_OSC2_intensety);
      }
             
      lcd.print("MX V-LFO->OSC2");
      lcd.setCursor(0, 1);
      if(mySettings.vol_lfo_T2_OSC2_intensety > 0.005 || mySettings.vol_lfo_T2_OSC2_intensety < -0.005  )
       lcd.print(mySettings.vol_lfo_T2_OSC2_intensety);
      else lcd.print("Off");
      menuExtrButton = false;  break;
      
     case 4 + 75: // Vol Lfo Depth T3 SUBS
      my_F_incrementer = float(updown/100.0);//0.1 steps
      mySettings.vol_lfo_T3_SUBS_intensety = getFloatValuesValide(my_F_incrementer, -1.0, 1.0, mySettings.vol_lfo_T3_SUBS_intensety);

      if(mySettings.vol_lfo_T1_T4_intensety_sync){
        mySettings.vol_lfo_T1_OSC1_intensety = mySettings.vol_lfo_T3_SUBS_intensety;
        mySettings.vol_lfo_T2_OSC2_intensety = mySettings.vol_lfo_T3_SUBS_intensety;
        mySettings.vol_lfo_T4_NOISE_intensety = abs(mySettings.vol_lfo_T3_SUBS_intensety);
      }
             
      lcd.print("MX V-LFO->SUBOSC");
      lcd.setCursor(0, 1);
      if(mySettings.vol_lfo_T3_SUBS_intensety > 0.005 || mySettings.vol_lfo_T3_SUBS_intensety < -0.005  )
        lcd.print(mySettings.vol_lfo_T3_SUBS_intensety);
      else lcd.print("Off");
      menuExtrButton = false;  break;
      
     case 4 + 76: // Vol Lfo Depth T4 NOISE
      my_F_incrementer = float(updown/100.0);//0.1 steps
      mySettings.vol_lfo_T4_NOISE_intensety = getFloatValuesValide(my_F_incrementer, 0.0, 1.0, mySettings.vol_lfo_T4_NOISE_intensety);

      if(mySettings.vol_lfo_T1_T4_intensety_sync){
        mySettings.vol_lfo_T1_OSC1_intensety = mySettings.vol_lfo_T4_NOISE_intensety;
        mySettings.vol_lfo_T2_OSC2_intensety = mySettings.vol_lfo_T4_NOISE_intensety;
        mySettings.vol_lfo_T3_SUBS_intensety = mySettings.vol_lfo_T4_NOISE_intensety;
      }
             
      lcd.print("MX V-LFO->NOISE");
      lcd.setCursor(0, 1);
      if(mySettings.vol_lfo_T4_NOISE_intensety > 0.005 )
        lcd.print(mySettings.vol_lfo_T4_NOISE_intensety);
      else lcd.print("Off");
      menuExtrButton = false;  break;
      
     case 4 + 77: // Vol lfo sync to attack
      lcd.print("MX V-LFO Syn2At");
      lcd.setCursor(0, 1);
      if(updown > 0){
        mySettings.vol_lfo_attack_sync = true;
      }
      if(updown < 0){
        mySettings.vol_lfo_attack_sync = false;
      }
      
      if(mySettings.vol_lfo_attack_sync) lcd.print("On");
      else lcd.print("Off");
      menuExtrButton = false;  break;     

     case 4 + 78:// Attack envelop to VolLFO Depth
      my_F_incrementer = float(updown/100.0);//0.01 steps
      mySettings.vol_lfo_follow_envelope_attack = getFloatValuesValide(my_F_incrementer, -1.0, 1.0, mySettings.vol_lfo_follow_envelope_attack);
          
      lcd.print("MX AtEn>V-LFO-In");
      lcd.setCursor(0, 1);
      lcd.print(mySettings.vol_lfo_follow_envelope_attack * 100.0);
      lcd.printf("%");
      menuExtrButton = false;
     break; 
     
     case 4 + 79:// Attack envelop to VolLFO Frequency
      my_F_incrementer = float(updown/10.0);//0.01 steps
      mySettings.envelopeFollowerAttackToVOLLFOFrequenzy = getFloatValuesValide(my_F_incrementer, 0.0, 10.0, mySettings.envelopeFollowerAttackToVOLLFOFrequenzy);
          
      lcd.print("MX AtEn>V-LFO-FR");
      lcd.setCursor(0, 1);
      lcd.print(mySettings.envelopeFollowerAttackToVOLLFOFrequenzy * 3);
      lcd.printf("");
      menuExtrButton = false;
     break; 


     // ##############################################################################################################
     // FilterLFO
     case 4 + 80: // Filter LFO waveform
      my_I_incrementer = updown; // 10 steps
      if(checkIntValuesValide(my_I_incrementer, 0, 11, mySettings.filter_lfo_waveform + my_I_incrementer))
        mySettings.filter_lfo_waveform += my_I_incrementer;

      if(updown!=0 && mySettings.filter_lfo_waveform > 5) selectABTWaveTableFilterLFO( bpmToHz(mySettings.filter_lfo_bpm), mySettings.filter_lfo_waveform -6);
      if(updown!=0 && mySettings.filter_lfo_waveform < 6) Filter_LFO.begin(1.0, bpmToHz(mySettings.filter_lfo_bpm) * beatMultiLookup[mySettings.filter_lfo_freq_bpm_multi] , waveIntsLook[mySettings.filter_lfo_waveform], mySettings.filter_lfo_cycles);     
     
      lcd.print("MX F-LFO Wavefo");
      lcd.setCursor(0, 1);
      lcd.print(LFOwaveNamesLook[mySettings.filter_lfo_waveform]);
      menuExtrButton = false;
     break;
  
      
     case 4 + 81: // Filter LFO pulsewidth
      my_F_incrementer = float(updown/100.0);//0.01 steps
      mySettings.filter_lfo_pulseWidth = getFloatValuesValide(my_F_incrementer, 0.1, 1.0, mySettings.filter_lfo_pulseWidth);
      
      if(updown!=0) Filter_LFO.pulseWidth(mySettings.filter_lfo_pulseWidth);
       
      lcd.print("MX F-LFO Pulsew.");
      lcd.setCursor(0, 1);
      if(mySettings.filter_lfo_waveform == 5){
        lcd.print(int(mySettings.filter_lfo_pulseWidth*100));
        lcd.print("%");
      }
      else{
        lcd.print("unused");
        //lcd.print("     ");
      }
      menuExtrButton = false;
     break;
      
     case 4 + 82: // Filter Lfo Bpm
      my_F_incrementer = float(updown/10.0);//0.1 steps
      if(mySettings.MasterClockToLfos || mySettings.MasterclockToFreqLFO){
        lcd.print("MX F-LFO BPM ");
        lcd.setCursor(0, 1);
        lcd.print("on CLOCK SYNC");
        menuExtrButton = false;
      break;       
      }
      mySettings.filter_lfo_bpm = getFloatValuesValide(my_F_incrementer, 0.0, 360.0, mySettings.filter_lfo_bpm);

      lcd.print("MX F-LFO BPM ");
      lcd.setCursor(0, 1);
      lcd.printf("%4.2f",mySettings.filter_lfo_bpm);
      lcd.print(" BPM");
      menuExtrButton = false;
     break;

     case 4 + 83: // Filter Lfo Bpm multip
      my_I_incrementer = updown;//
      if(checkIntValuesValide(my_I_incrementer, 0, 20, mySettings.filter_lfo_freq_bpm_multi + my_I_incrementer))
        mySettings.filter_lfo_freq_bpm_multi += my_I_incrementer;
           
      lcd.print("MX F-LFO BPM x");
      lcd.setCursor(0, 1);
      if(mySettings.MasterClockToLfos || mySettings.MasterclockToFreqLFO){
        lcd.printf("%4.1f %s",mySystemBpm, beatMultiNamesLookup[mySettings.filter_lfo_freq_bpm_multi]);
      }
      else
        lcd.printf("%4.1f %s",mySettings.filter_lfo_bpm, beatMultiNamesLookup[mySettings.filter_lfo_freq_bpm_multi]);
      menuExtrButton = false;
     break;

     case 4 + 84: // Filter LFO Phase
      my_I_incrementer = updown;
      if(checkIntValuesValide(my_I_incrementer, 0, 360, mySettings.filter_lfo_phase + my_I_incrementer))
        mySettings.filter_lfo_phase+= my_I_incrementer;
      
      if(updown!=0) Filter_LFO.phase(mySettings.filter_lfo_phase);
      lcd.print("MX F-LFO Phase");
      lcd.setCursor(0, 1);
      lcd.print(mySettings.filter_lfo_phase);
      lcd.print(" Deg.");
      menuExtrButton = false;
     break;

    
     case 4 + 85: // Filter lfo sync to attack
      lcd.print("MX F-LFO Syn2At");
      lcd.setCursor(0, 1);
      if(updown > 0){
        mySettings.filter_lfo_attack_sync = true;
      }
      if(updown < 0){
        mySettings.filter_lfo_attack_sync = false;
      }
      
      if(mySettings.filter_lfo_attack_sync) lcd.print("On");
      else lcd.print("Off");
      menuExtrButton = false;
     break; 

     case 4 + 86: // Filter Lfo Depth
      my_F_incrementer = float(updown/100.0);//0.1 steps
      mySettings.filter_lfo_intensety = getFloatValuesValide(my_F_incrementer, -1.0, 1.0, mySettings.filter_lfo_intensety);
      
      Filter_LFO_depth.amplitude(mySettings.filter_lfo_intensety);
       
      lcd.print("MX F-LFO->F1");
      lcd.setCursor(0, 1);
      lcd.print(int(mySettings.filter_lfo_intensety*100));
      lcd.print("%");
      menuExtrButton = false;
     break;
      
     // Attack envelop to FilterLFO Frequency
     case 4 + 87:
      my_F_incrementer = float(updown/10.0);//0.01 steps
      mySettings.envelopeFollowerAttackToFilterLFOFrequenzy = getFloatValuesValide(my_F_incrementer, 0.0, 10.0, mySettings.envelopeFollowerAttackToFilterLFOFrequenzy);
          
      lcd.print("MX AtEn>F-LFO-FR");
      lcd.setCursor(0, 1);
      lcd.print(mySettings.envelopeFollowerAttackToFilterLFOFrequenzy * 3);
      lcd.printf("");
      menuExtrButton = false;
     break;           
    
     case 4 + 88:// Attack envelop to Filter LFO Depth
      my_F_incrementer = float(updown/100.0);//0.01 steps
      mySettings.envelopeFollowerAttackToFilterLFODepth = getFloatValuesValide(my_F_incrementer, -1.0, 1.0, mySettings.envelopeFollowerAttackToFilterLFODepth);
          
      lcd.print("MX AtEn>F-LFO-In");
      lcd.setCursor(0, 1);
      lcd.print(mySettings.envelopeFollowerAttackToFilterLFODepth * 100.0);
      lcd.printf("%");
      menuExtrButton = false;
     break;


      // ############################## Midi Envelope
     case 4 + 89: // attack
      my_I_incrementer = updown;
      if(mySettings.midi_attack>30) my_I_incrementer = updown*5;
      if(mySettings.midi_attack>80) my_I_incrementer = updown*10;
      
      if(checkIntValuesValide(my_I_incrementer, 1, 10000, mySettings.midi_attack + my_I_incrementer))
        mySettings.midi_attack += my_I_incrementer;
     
      lcd.print("ADSR Env. Attack");
      lcd.setCursor(0, 1);
      lcd.print(mySettings.midi_attack);
      lcd.print("ms");
      menuExtrButton = false;  break;
      
     case 4 + 90: // hold
      my_I_incrementer = updown;
      if(mySettings.midi_hold>30) my_I_incrementer = updown*5;
      if(mySettings.midi_hold>80) my_I_incrementer = updown*10;
      
      if(checkIntValuesValide(my_I_incrementer, 1, 10000, mySettings.midi_hold + my_I_incrementer))
        mySettings.midi_hold += my_I_incrementer;
     
      lcd.print("ADSR Env. Hold");
      lcd.setCursor(0, 1);
      lcd.print(mySettings.midi_hold);
      lcd.print("ms");
      menuExtrButton = false;  break;
     
     case 4 + 91: // decay
      my_I_incrementer = updown;
      if(mySettings.midi_decay>30) my_I_incrementer = updown*5;
      if(mySettings.midi_decay>80) my_I_incrementer = updown*10;
      
      if(checkIntValuesValide(my_I_incrementer, 1, 10000, mySettings.midi_decay + my_I_incrementer))
        mySettings.midi_decay += my_I_incrementer;
     
      lcd.print("ADSR Env. Decay");
      lcd.setCursor(0, 1);
      lcd.print(mySettings.midi_decay);
      lcd.print("ms");
      menuExtrButton = false;  break;

     case 4 + 92: // sustain
      my_I_incrementer = updown;
      if(checkIntValuesValide(my_I_incrementer, 0, 100, mySettings.midi_sustain + my_I_incrementer))
        mySettings.midi_sustain += my_I_incrementer;
     
      lcd.print("ADSR Env.Sustain");
      lcd.setCursor(0, 1);
      lcd.print(mySettings.midi_sustain);
      lcd.print(" %");
      menuExtrButton = false;  break;

     case 4 + 93: // release
      my_I_incrementer = updown;
      if(mySettings.midi_release>30) my_I_incrementer = updown*5;
      if(mySettings.midi_release>80) my_I_incrementer = updown*10;
      
      if(checkIntValuesValide(my_I_incrementer, 1, 10000, mySettings.midi_release + my_I_incrementer))
        mySettings.midi_release += my_I_incrementer;
     
      lcd.print("ADSR Env.Release");
      lcd.setCursor(0, 1);
      lcd.print(mySettings.midi_release);
      lcd.print("ms");
      menuExtrButton = false;  break;

      // ############################## Filter Adsr Envelope
 
     case 4 + 94: // delay
      my_I_incrementer = updown;
      if(mySettings.filter_delay>30) my_I_incrementer = updown*5;
      if(mySettings.filter_delay>80) my_I_incrementer = updown*10;
      
      if(checkIntValuesValide(my_I_incrementer, 1, 10000, mySettings.filter_delay + my_I_incrementer))
        mySettings.filter_delay += my_I_incrementer;
     
      lcd.print("Filt Env Delay");
      lcd.setCursor(0, 1);
      lcd.print(mySettings.filter_delay);
      lcd.print("ms");
      menuExtrButton = false;  break;
           
     case 4 + 95: // attack
      my_I_incrementer = updown;
      if(mySettings.filter_attack>30) my_I_incrementer = updown*5;
      if(mySettings.filter_attack>80) my_I_incrementer = updown*10;
      
      if(checkIntValuesValide(my_I_incrementer, 1, 10000, mySettings.filter_attack + my_I_incrementer))
        mySettings.filter_attack += my_I_incrementer;
     
      lcd.print("Filt Env Attack");
      lcd.setCursor(0, 1);
      lcd.print(mySettings.filter_attack);
      lcd.print("ms");
      menuExtrButton = false;  break;
      
     case 4 + 96: // hold
      my_I_incrementer = updown;
      if(mySettings.filter_hold>30) my_I_incrementer = updown*5;
      if(mySettings.filter_hold>80) my_I_incrementer = updown*10;
      
      if(checkIntValuesValide(my_I_incrementer, 1, 10000, mySettings.filter_hold + my_I_incrementer))
        mySettings.filter_hold += my_I_incrementer;
     
      lcd.print("Filt Env Hold");
      lcd.setCursor(0, 1);
      lcd.print(mySettings.filter_hold);
      lcd.print("ms");
      menuExtrButton = false;  break;
     
     case 4 + 97: // decay
      my_I_incrementer = updown;
      if(mySettings.filter_decay>30) my_I_incrementer = updown*5;
      if(mySettings.filter_decay>80) my_I_incrementer = updown*10;
      
      if(checkIntValuesValide(my_I_incrementer, 1, 10000, mySettings.filter_decay + my_I_incrementer))
        mySettings.filter_decay += my_I_incrementer;
     
      lcd.print("Filt Env Decay");
      lcd.setCursor(0, 1);
      lcd.print(mySettings.filter_decay);
      lcd.print("ms");
      menuExtrButton = false;  break;

     case 4 + 98: // sustain
      my_I_incrementer = updown;
      if(checkIntValuesValide(my_I_incrementer, 0, 100, mySettings.filter_sustain + my_I_incrementer))
        mySettings.filter_sustain += my_I_incrementer;
     
      lcd.print("Filt Env Sustain");
      lcd.setCursor(0, 1);
      lcd.print(mySettings.filter_sustain);
      lcd.print(" %");
      menuExtrButton = false;  break;

     case 4 + 99: // release
      my_I_incrementer = updown;
      if(mySettings.filter_release>30) my_I_incrementer = updown*5;
      if(mySettings.filter_release>80) my_I_incrementer = updown*10;
      
      if(checkIntValuesValide(my_I_incrementer, 1, 10000, mySettings.filter_release + my_I_incrementer))
        mySettings.filter_release += my_I_incrementer;
     
      lcd.print("Filt Env Release");
      lcd.setCursor(0, 1);
      lcd.print(mySettings.filter_release);
      lcd.print("ms");
      menuExtrButton = false;  break;
      // ############# ende filter adsr

     // Attack envelop to Filter1
     case 4 + 100:
      my_F_incrementer = float(updown/100.0);//0.1 steps
      mySettings.filter1envelopeIntensity =getFloatValuesValide(my_F_incrementer, -1.0, 1.0, mySettings.filter1envelopeIntensity);
      
      filter1envelopeIntensity.amplitude(mySettings.filter1envelopeIntensity);
       
      lcd.print("MX AtEn>F-CUT");
      lcd.setCursor(0, 1);
      lcd.print(int(mySettings.filter1envelopeIntensity*100));
      lcd.print("%");
      menuExtrButton = false;  break; 

     // gittare use filteradsr
     case 4 + 101: 
      lcd.print("Git. Filter Env");
      lcd.setCursor(0, 1);
      if(updown > 0){
        mySettings.gitarUseFilterADSR = true;
      }
      if(updown < 0){
        mySettings.gitarUseFilterADSR = false;
      }
      
      if(mySettings.gitarUseFilterADSR) lcd.print("Filter ADSR");
      else lcd.print("Filter v. Attack");

      menuExtrButton = false;
     break;
     
     // gittare filteradsr static velocity
     case 4 + 102: 
      lcd.print("Git u.Static Vel");
      lcd.setCursor(0, 1);
      my_F_incrementer = float(updown/100.0);//0.1 steps
      mySettings.gitarUseFilterADSRstaticVelocity = getFloatValuesValide(my_F_incrementer, 0.0, 1.0, mySettings.gitarUseFilterADSRstaticVelocity);

      
      if(mySettings.gitarUseFilterADSRstaticVelocity >= 0.01) lcd.printf("Filter Vel: %4.2f",mySettings.gitarUseFilterADSRstaticVelocity );
      else lcd.print("Filter Vel: Off");

      menuExtrButton = false;
     break;
        
     // gittare use envelop adsr
     case 4 + 103: 
      lcd.print("Git use Env ADSR");
      lcd.setCursor(0, 1);
      if(updown > 0){
        mySettings.gitarUseEnvelopeADSR = true;
      }
      if(updown < 0){
        mySettings.gitarUseEnvelopeADSR = false;
      }
      
      if(mySettings.gitarUseEnvelopeADSR) lcd.print("Envelope ADSR");
      else lcd.print("Envel. v. Attack");

      menuExtrButton = false;
     break;
      
     case 4 + 104: // Midi Pitchband range
      my_I_incrementer = updown;
      
      if(checkIntValuesValide(my_I_incrementer, 1, 24, mySettings.midi_pichbandrange + my_I_incrementer))
        mySettings.midi_pichbandrange += my_I_incrementer;
     
      lcd.print("Midi Pitch Bend");
      lcd.setCursor(0, 1);
      lcd.print(mySettings.midi_pichbandrange);
      lcd.print(" Notes");
      menuExtrButton = false;
     break;

      // ######################### masterclock sync
     case 4 + 105: // sync all together
      lcd.print("Syc A-LFOS CLOCK");
      lcd.setCursor(0, 1);
      if(updown > 0){
        mySettings.MasterClockToLfos = true;
        masterclockInternalSyncToLFOs();
      }
      if(updown < 0){
        mySettings.MasterClockToLfos = false;
      }
      
      if(mySettings.MasterClockToLfos) lcd.print("SYNC ALL-LFO On");
      else lcd.print("SYNC ALL-LFO Off");

      menuExtrButton = false;
      break;

     case 4 + 106:
      lcd.print("CLOCK Syc M-LFO");
      lcd.setCursor(0, 1);
      if(updown > 0){
        mySettings.MasterClockToMODLfo = true;
        masterclockInternalSyncToLFOs();
      }
      if(updown < 0){
        mySettings.MasterClockToMODLfo = false;
      }
      
      if(mySettings.MasterClockToMODLfo) lcd.print("SYNC MOD-LFO On");
      else lcd.print("SYNC MOD-LFO Off");

      menuExtrButton = false;
      break;

     case 4 + 107:
      lcd.print("CLOCK Syc V-LFO");
      lcd.setCursor(0, 1);
      if(updown > 0){
        mySettings.MasterClockToVOLLfo = true;
        masterclockInternalSyncToLFOs();
      }
      if(updown < 0){
        mySettings.MasterClockToVOLLfo = false;
      }
      
      if(mySettings.MasterClockToVOLLfo) lcd.print("SYNC VOL-LFO On");
      else lcd.print("SYNC VOL-LFO Off");

      menuExtrButton = false;
      break;
      
     case 4 + 108:
      lcd.print("CLOCK Syc F-LFO");
      lcd.setCursor(0, 1);
      if(updown > 0){
        mySettings.MasterclockToFreqLFO = true;
        masterclockInternalSyncToLFOs();
      }
      if(updown < 0){
        mySettings.MasterclockToFreqLFO = false;
      }
      
      if(mySettings.MasterclockToFreqLFO) lcd.print("SYNC FIL-LFO On");
      else lcd.print("SYNC FIL-LFO Off");

      menuExtrButton = false;
      break;
         
      // ############################## OSC MAster Volumen
     case 4 + 109: // OSC Master Volumen
      
      my_F_incrementer = float(updown)/100.0;//0.1 steps
      
      mySettings.SYNTH_Master_Volume = getFloatValuesValide(my_F_incrementer, 0.0, 1.0, mySettings.SYNTH_Master_Volume);

      if(updown !=0){
        for(int i=0;i<4;i++){
          OscMainMix.gain(i, mySettings.SYNTH_Master_Volume);//0.1 Ausgang zum filter
        } 
      }

  
      lcd.print("OSC Master Vol");
      lcd.setCursor(0, 1);
      //lcd.print(int(mySettings.SYNTH_Master_Volume*100));
      lcd.print(int(powf(mySettings.SYNTH_Master_Volume, 0.2)*100));
      lcd.print("%");
      menuExtrButton = false;
      break;

     case 4 + 110:
      lcd.print("Peak OSC Master");
      lcd.setCursor(0, 1);
      if(updown > 0){
        mySettings.autoPeakCorrection = true;
      }
      if(updown < 0){
        mySettings.autoPeakCorrection = false;
      }
      
      if(mySettings.autoPeakCorrection) lcd.print("Limit On");
      else lcd.print("Limit Off");

      menuExtrButton = false;
     break;

      // ############################## MIXER SECTION ############################################
     case 4 + 111: // Mixer Instrument Input To Synth Frequency - and Envelope - tracking, Volumen
      
      my_F_incrementer = float(updown)/100.0;//0.1 steps
      
      mySettings.instrumentToSynth = getFloatValuesValide(my_F_incrementer, 0.0, 1.0, mySettings.instrumentToSynth);

      if(updown !=0) mixerAnalyse.gain(2, mySettings.instrumentToSynth * 2.0); // x2 wegen dem niedrigen pegel. für tests ausreichend, später unbedingt erarbeiten

  
      lcd.print("MIX Instr->Synth");
      lcd.setCursor(0, 1);
      //lcd.print(int(mySettings.SYNTH_Master_Volume*100));
      lcd.print(int(powf(mySettings.instrumentToSynth, 0.2)*100));
      lcd.print("%");
      menuExtrButton = false;
     break;
 
     case 4 + 112: // Mixer Usb Input 1 To Synth Frequency - and Envelope - tracking, Volumen
      
      my_F_incrementer = float(updown)/100.0;//0.1 steps
      
      mySettings.usbIn_1_ToSynth = getFloatValuesValide(my_F_incrementer, 0.0, 1.0, mySettings.usbIn_1_ToSynth);

      if(updown !=0) mixerAnalyse.gain(0, mySettings.usbIn_1_ToSynth);

  
      lcd.print("MIX UsbI1->Synth");
      lcd.setCursor(0, 1);
      //lcd.print(int(mySettings.SYNTH_Master_Volume*100));
      lcd.print(int(powf(mySettings.usbIn_1_ToSynth, 0.2)*100));
      lcd.print("%");
      menuExtrButton = false;
     break;

     case 4 + 113: // Synth to output 1
      
      my_F_incrementer = float(updown)/100.0;//0.1 steps
      
      mySettings.synthToOutput1 = getFloatValuesValide(my_F_incrementer, 0.0, 1.0, mySettings.synthToOutput1);

      if(updown !=0) I2SOutputMixerChannel1.gain(3, mySettings.synthToOutput1);

  
      lcd.print("MIX Synth->Out1");
      lcd.setCursor(0, 1);
      //lcd.print(int(mySettings.SYNTH_Master_Volume*100));
      lcd.print(int(powf(mySettings.synthToOutput1, 0.2)*100));
      lcd.print("%");
      menuExtrButton = false;
     break;
 
     case 4 + 114: // Synth to output 2
      
      my_F_incrementer = float(updown)/100.0;//0.1 steps
      
      mySettings.synthToOutput2 = getFloatValuesValide(my_F_incrementer, 0.0, 1.0, mySettings.synthToOutput2);

      if(updown !=0)  I2SOutputMixerChannel2.gain(3, mySettings.synthToOutput2);

  
      lcd.print("MIX Synth->Out2");
      lcd.setCursor(0, 1);
      //lcd.print(int(mySettings.SYNTH_Master_Volume*100));
      lcd.print(int(powf(mySettings.synthToOutput2, 0.2)*100));
      lcd.print("%");
      menuExtrButton = false;
     break;

     case 4 + 115: // Synth to Usb output 1
      
      my_F_incrementer = float(updown)/100.0;//0.1 steps
      
      mySettings.synthToUsbOutput1 = getFloatValuesValide(my_F_incrementer, 0.0, 1.0, mySettings.synthToUsbOutput1);

      if(updown !=0) UsbOutputMixerLinks.gain(3, mySettings.synthToUsbOutput1);

  
      lcd.print("MIX Synt->Usb O1");
      lcd.setCursor(0, 1);
      //lcd.print(int(mySettings.SYNTH_Master_Volume*100));
      lcd.print(int(powf(mySettings.synthToUsbOutput1, 0.2)*100));
      lcd.print("%");
      menuExtrButton = false;
     break;
 
     case 4 + 116: // Synth to Usb output 2
      
      my_F_incrementer = float(updown)/100.0;//0.1 steps
      
      mySettings.synthToUsbOutput2 = getFloatValuesValide(my_F_incrementer, 0.0, 1.0, mySettings.synthToUsbOutput2);

      if(updown !=0) UsbOutputMixerRechts.gain(3, mySettings.synthToUsbOutput2);

  
      lcd.print("MIX Synt->Usb O2");
      lcd.setCursor(0, 1);
      //lcd.print(int(mySettings.SYNTH_Master_Volume*100));
      lcd.print(int(powf(mySettings.synthToUsbOutput2, 0.2)*100));
      lcd.print("%");
      menuExtrButton = false;
     break;


     case 4 + 117: // Instrument to Usb output 1
      
      my_F_incrementer = float(updown)/100.0;//0.1 steps
      
      mySettings.instrumentToUsb_1 = getFloatValuesValide(my_F_incrementer, 0.0, 1.0, mySettings.instrumentToUsb_1);

      if(updown !=0) UsbOutputMixerLinks.gain(0, mySettings.instrumentToUsb_1);

  
      lcd.print("MIX Inst->Usb O1");
      lcd.setCursor(0, 1);
      //lcd.print(int(mySettings.SYNTH_Master_Volume*100));
      lcd.print(int(powf(mySettings.instrumentToUsb_1, 0.2)*100));
      lcd.print("%");
      menuExtrButton = false;
     break;
 
     case 4 + 118: // Instrument to Usb output 2
      
      my_F_incrementer = float(updown)/100.0;//0.1 steps
      
      mySettings.instrumentToUsb_2 = getFloatValuesValide(my_F_incrementer, 0.0, 1.0, mySettings.instrumentToUsb_2);

      if(updown !=0) UsbOutputMixerRechts.gain(0, mySettings.instrumentToUsb_2);

  
      lcd.print("MIX Inst->Usb O2");
      lcd.setCursor(0, 1);
      //lcd.print(int(mySettings.SYNTH_Master_Volume*100));
      lcd.print(int(powf(mySettings.instrumentToUsb_2, 0.2)*100));
      lcd.print("%");
      menuExtrButton = false;
     break;

     // ############## Usb Input Output Matrix #######################################


     case 4 + 119: // Usb Input 1 to output 1
      
      my_F_incrementer = float(updown)/100.0;//0.1 steps
      
      mySettings.usbIn_1_ToOutput1 = getFloatValuesValide(my_F_incrementer, 0.0, 1.0, mySettings.usbIn_1_ToOutput1);

      if(updown !=0) I2SOutputMixerChannel1.gain(1, mySettings.usbIn_1_ToOutput1);
      
  
      lcd.print("MIX UsbIn1->Out1");
      lcd.setCursor(0, 1);
      //lcd.print(int(mySettings.SYNTH_Master_Volume*100));
      lcd.print(int(powf(mySettings.usbIn_1_ToOutput1, 0.2)*100));
      lcd.print("%");
      menuExtrButton = false;
     break;
 
     case 4 + 120: // Usb Input 1 to output 2
      
      my_F_incrementer = float(updown)/100.0;//0.1 steps
      
      mySettings.usbIn_1_ToOutput2 = getFloatValuesValide(my_F_incrementer, 0.0, 1.0, mySettings.usbIn_1_ToOutput2);

      if(updown !=0) I2SOutputMixerChannel2.gain(1, mySettings.usbIn_1_ToOutput2);

  
      lcd.print("MIX UsbIn1->Out2");
      lcd.setCursor(0, 1);
      //lcd.print(int(mySettings.SYNTH_Master_Volume*100));
      lcd.print(int(powf(mySettings.usbIn_1_ToOutput2, 0.2)*100));
      lcd.print("%");
      menuExtrButton = false;
     break;


     case 4 + 121: // Usb Input 2 to output 1
      
      my_F_incrementer = float(updown)/100.0;//0.1 steps
      
      mySettings.usbIn_2_ToOutput1 = getFloatValuesValide(my_F_incrementer, 0.0, 1.0, mySettings.usbIn_2_ToOutput1);

      if(updown !=0) I2SOutputMixerChannel1.gain(2, mySettings.usbIn_2_ToOutput1);
      
  
      lcd.print("MIX UsbIn2->Out1");
      lcd.setCursor(0, 1);
      //lcd.print(int(mySettings.SYNTH_Master_Volume*100));
      lcd.print(int(powf(mySettings.usbIn_2_ToOutput1, 0.2)*100));
      lcd.print("%");
      menuExtrButton = false;
     break;
 
     case 4 + 122: // Usb Input 2 to output 2
      
      my_F_incrementer = float(updown)/100.0;//0.1 steps
      
      mySettings.usbIn_2_ToOutput2 = getFloatValuesValide(my_F_incrementer, 0.0, 1.0, mySettings.usbIn_2_ToOutput2);

      if(updown !=0) I2SOutputMixerChannel2.gain(2, mySettings.usbIn_2_ToOutput2);

  
      lcd.print("MIX UsbIn2->Out2");
      lcd.setCursor(0, 1);
      //lcd.print(int(mySettings.SYNTH_Master_Volume*100));
      lcd.print(int(powf(mySettings.usbIn_2_ToOutput2, 0.2)*100));
      lcd.print("%");
      menuExtrButton = false;
     break;


     // ################### Misk ########################################
     case 4 + 123: // Filter1 Dynamic Frequency
      my_I_incrementer = updown;
      
      if(checkIntValuesValide(my_I_incrementer, 0, 4, mySettings.Filter1_Active_Frequenz_Tracking_Target + my_I_incrementer))
        mySettings.Filter1_Active_Frequenz_Tracking_Target += my_I_incrementer;

      if(using_Filter1_Active_Frequenz_Tracking && mySettings.Filter1_Active_Frequenz_Tracking_Target == 0){ // we disable active frequency tracking
            filter1.frequency(mySettings.filter1Freq);
      }
     
      lcd.print("Dyn-Filter1-Freq");
      lcd.setCursor(0, 1);
      if(mySettings.Filter1_Active_Frequenz_Tracking_Target == 0) lcd.print("OFF");
      if(mySettings.Filter1_Active_Frequenz_Tracking_Target == 1) lcd.print("OSC 1");
      if(mySettings.Filter1_Active_Frequenz_Tracking_Target == 2) lcd.print("OSC 2");
      if(mySettings.Filter1_Active_Frequenz_Tracking_Target == 3) lcd.print("Sub OSC 1");
      if(mySettings.Filter1_Active_Frequenz_Tracking_Target == 4) lcd.print("Sub OSC 2");
      menuExtrButton = false;
      
      if(mySettings.Filter1_Active_Frequenz_Tracking_Target > 0)
        using_Filter1_Active_Frequenz_Tracking = true;
      else
        using_Filter1_Active_Frequenz_Tracking = false;
         
     break;
     
     case 4 + 124: // Filter2 Dynamic Frequency
      my_I_incrementer = updown;
      
      if(checkIntValuesValide(my_I_incrementer, 0, 4, mySettings.Filter2_Active_Frequenz_Tracking_Target + my_I_incrementer))
        mySettings.Filter2_Active_Frequenz_Tracking_Target += my_I_incrementer;


      if(using_Filter2_Active_Frequenz_Tracking && mySettings.Filter2_Active_Frequenz_Tracking_Target == 0){ // we disable active frequency tracking
            filter2.frequency(mySettings.filter2Freq);
      }
     
      lcd.print("Dyn-Filter2-Freq");
      lcd.setCursor(0, 1);
      if(mySettings.Filter2_Active_Frequenz_Tracking_Target == 0) lcd.print("OFF");
      if(mySettings.Filter2_Active_Frequenz_Tracking_Target == 1) lcd.print("OSC 1");
      if(mySettings.Filter2_Active_Frequenz_Tracking_Target == 2) lcd.print("OSC 2");
      if(mySettings.Filter2_Active_Frequenz_Tracking_Target == 3) lcd.print("Sub OSC 1");
      if(mySettings.Filter2_Active_Frequenz_Tracking_Target == 4) lcd.print("Sub OSC 2");
      menuExtrButton = false;
      if(mySettings.Filter2_Active_Frequenz_Tracking_Target > 0)
        using_Filter2_Active_Frequenz_Tracking = true;
      else
        using_Filter2_Active_Frequenz_Tracking = false;       
     break;
            
     case 4 + 125: // Midi channel
      my_I_incrementer = updown;
      
      if(checkIntValuesValide(my_I_incrementer, 1, 16, mySettings.midiChannel + my_I_incrementer))
        mySettings.midiChannel += my_I_incrementer;
     
      lcd.print("Midi Channel");
      lcd.setCursor(0, 1);
      lcd.print(mySettings.midiChannel);
      menuExtrButton = false;
      break;


     case 4 + 126: // Function Btn Function
     
      my_I_incrementer = updown;
      if(checkIntValuesValide(my_I_incrementer, 0, 3, mySettings.functionBtnFunction + my_I_incrementer))
        mySettings.functionBtnFunction += my_I_incrementer;
     
      lcd.print("FunctionBtn Ctr");
      lcd.setCursor(0, 1);
      lcd.print(FunctionBtnLookup[mySettings.functionBtnFunction]);
      menuExtrButton = false;
      // regenerate mixer if muted from mute functions
      if(mySettings.functionBtnFunction == 2){
        for(int i=0;i<4;i++){
            OscMainMix.gain(i, 0);
        }        
      }
      else
      {
        for(int i=0;i<4;i++){
            OscMainMix.gain(i, mySettings.SYNTH_Master_Volume);
        }        
      }
     break;

     case 4 + 127: // Expression Pedal Function
     
      my_I_incrementer = updown;
      if(checkIntValuesValide(my_I_incrementer, 0, 22, mySettings.expressionPedalFunction + my_I_incrementer))
        mySettings.expressionPedalFunction += my_I_incrementer;
     
      lcd.print("ExpPedal Func");
      lcd.setCursor(0, 1);
      lcd.print(expPedalFunctionLookup[mySettings.expressionPedalFunction]);
      menuExtrButton = false;   
     break;

     case 4 + 128: // EXp  Pedal Min
       my_I_incrementer = updown;
      if(checkIntValuesValide(my_I_incrementer, 0, 127, mySettings.freeDataInt3 + my_I_incrementer))
        mySettings.freeDataInt3 += my_I_incrementer;
     
      lcd.print("ExpPedal MIN");
      lcd.setCursor(0, 1);
      lcd.print(mySettings.freeDataInt3);
      menuExtrButton = false;      
     break;

     case 4 + 129: // EXp  Pedal Max
        my_I_incrementer = updown;
      if(checkIntValuesValide(my_I_incrementer, -127, 0, mySettings.freeDataInt4 + my_I_incrementer))
        mySettings.freeDataInt4 += my_I_incrementer;
     
      lcd.print("ExpPedal MAX");
      lcd.setCursor(0, 1);
      lcd.print(mySettings.freeDataInt4 + 127);// !!mySettings.expressionPedalFunction enthält bisher nur 20 einträge 
      menuExtrButton = false;     
     break;

     case 4 + 130: // Poti Function
     
      my_I_incrementer = updown;
      if(checkIntValuesValide(my_I_incrementer, 0, 22, mySettings.freeDataInt2 + my_I_incrementer))
        mySettings.freeDataInt2 += my_I_incrementer;
     
      lcd.print("Poti Func");
      lcd.setCursor(0, 1);
      lcd.print(expPedalFunctionLookup[mySettings.freeDataInt2]);// !!mySettings.expressionPedalFunction enthält bisher nur 22 einträge 
      menuExtrButton = false;   
     break;
     
//     case 4 + 127: // Bypass Mix da das mcp42xx scheiße arbeitet erst mal nicht hier
//     
//      my_I_incrementer = updown;
//      if (checkIntValuesValide(my_I_incrementer, 0, 255, (int)mySettings.instrumentBypassMix + my_I_incrementer)){
//        
//        mySettings.instrumentBypassMix = my_I_incrementer + (int)mySettings.instrumentBypassMix;
//      }
//      
//      if(updown !=0) setMCP42Value(byte(mySettings.instrumentBypassMix));
//      
//      lcd.print("Git<->Synth Mix");
//      lcd.setCursor(0, 1);
//
//      
//      myKopfKnackerLeft = 0.0 + map((int)mySettings.instrumentBypassMix,0,255,100,0);// gitarre map
//      myKopfKnackerRight = 0.0 + map((int)mySettings.instrumentBypassMix,0,255,0,100);// gitarre map; 
//         
//      lcd.printf(" %d <-> %d",int(myKopfKnackerLeft), int(myKopfKnackerRight) );
//      menuExtrButton = false;   
//     break;

     case 4 + 131: // input gain
     
      my_I_incrementer = updown;
      if (checkIntValuesValide(my_I_incrementer, 0, 15, mySettings.freeDataInt1 + my_I_incrementer)){
        
        mySettings.freeDataInt1 += my_I_incrementer;
      }
      
      if(updown !=0) sgtl5000_1.lineInLevel(mySettings.freeDataInt1);//( left, right) 5 default);
      
      lcd.print("Input Gain");
      lcd.setCursor(0, 1);
 
         
      lcd.printf("%d ", mySettings.freeDataInt1 );
      menuExtrButton = false;   
     break;  
      
     case 4 + 132: // edit preset name
      menuExtrButtonLenght = 8;
      menuExtrButton = true;
      menuExtra_I_Pos += updown;
      if(menuExtra_I_Pos > 71) menuExtra_I_Pos = 0;
      if(menuExtra_I_Pos < 0) menuExtra_I_Pos = 71;
          
           
      lcd.print("Edit Preset Name");
 
      lcd.setCursor(0, 1);

      lcd.print(mySettings.presetName);
      lcd.setCursor(menuExtrButtonPos,1);
      //Serial.println(menuExtrButtonPos);
     
      lcd.print(asciLookupTable[menuExtra_I_Pos]);

      // für den cursor
      lcd.setCursor(menuExtrButtonPos,1);
      lcd.cursor();
      
      if(updown !=0) mySettings.presetName[menuExtrButtonPos] = asciLookupTable[menuExtra_I_Pos];
      
     break;

     case 4 + 133: // System EXP Pedal Min
     
      my_I_incrementer = updown;
      if (checkIntValuesValide(my_I_incrementer, 0, 1000, mySystemSettings.Expression_Min + my_I_incrementer)){
        
        mySystemSettings.Expression_Min += my_I_incrementer;
      }
      
      
      lcd.print("SYSTEM EXP-P MIN");
      lcd.setCursor(0, 1);
 
         
      lcd.printf("%d ", mySystemSettings.Expression_Min);
      menuExtrButton = false;   
     break; 
     
     case 4 + 134: // System EXP Pedal Max
     
      my_I_incrementer = updown;
      if (checkIntValuesValide(my_I_incrementer, 0, 1000, mySystemSettings.Expression_Max + my_I_incrementer)){
        
        mySystemSettings.Expression_Max += my_I_incrementer;
      }
      
      
      lcd.print("SYSTEM EXP-P MAX");
      lcd.setCursor(0, 1);
 
         
      lcd.printf("%d ", mySystemSettings.Expression_Max);
      menuExtrButton = false;   
     break;

     case 4 + 135: // System Use Live-Presets
      lcd.print("SYS Live-Preset");
      lcd.setCursor(0, 1);
      
      if(updown > 0){
        for(int i = 0;i<30;i++){
          if( mySystemSettings.LivePresets[i] >0){
            mySystemSettings.switchLivePresets = true;
            break;
          }
        }
      }
      
      if(updown < 0) mySystemSettings.switchLivePresets = false;
      
      
      if( mySystemSettings.switchLivePresets){
        lcd.printf("ON");
      }
      else {
        lcd.printf("OFF");
      }
      menuExtrButton = false;    
     break;
     
     case 4 + 136: // System edit Live-Presets
      lcd.print("SYS Edit Live-Pr");

      menuExtrButtonLenght = 29;
      menuExtrButton = true;

      my_I_incrementer = updown;
      if (checkIntValuesValide(my_I_incrementer, 0, 127, mySystemSettings.LivePresets[menuExtrButtonPos] + my_I_incrementer)){
        
        mySystemSettings.LivePresets[menuExtrButtonPos] += my_I_incrementer;
      }
      //Serial.printf("Pos:, %d ,Value %d\n",menuExtrButtonPos, mySystemSettings.LivePresets[menuExtrButtonPos]);
      
      lcd.setCursor(0, 1);
      if(mySystemSettings.LivePresets[menuExtrButtonPos] == 0 ){
        lcd.printf("Pos:%d, Pres:OFF", menuExtrButtonPos + 1); 
      }
      else
      {
        lcd.printf("Pos:%d, Pres:%d", menuExtrButtonPos + 1,  mySystemSettings.LivePresets[menuExtrButtonPos]);
      }
     break;
     
     case 4 + 137: // expand System menue
      lcd.print("SYS Menu: ");
      if(updown > 0) mySystemSettings.expandSystemSettings = true;
      if(updown < 0) mySystemSettings.expandSystemSettings = false;
      //lcd.setCursor(0, 1);
      
      if(mySystemSettings.expandSystemSettings){
        hidingSettings[137] = true; 
        hidingSettings[138] = true;
        hidingSettings[139] = true; 
        hidingSettings[140] = true;  
        lcd.printf("ON");
      }
      else{
        hidingSettings[137] = false; 
        hidingSettings[138] = false;
        hidingSettings[139] = false; 
        hidingSettings[140] = false; 
        lcd.printf("OFF");
      }
      lcd.setCursor(0, 1);
      lcd.printf("Or save System");
      menuExtrButton = false;
     break;     
   }
  return;
}

bool checkIntValuesValide(int incrementer, int mini, int maxi, int val){
  if(val <= mini - abs(incrementer) || val >= maxi + abs(incrementer)) return false; 
  else return true;
}

float getFloatValuesValide(float incrementer, float mini, float maxi, float val){
  float newall = val + incrementer;
  if(newall > maxi) return maxi;
  if(newall < mini) return mini;
  return newall;
}






