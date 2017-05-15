
//################# init
void initMidiReceiving(){
  usbMIDI.setHandleNoteOn(OnNoteOn);
  usbMIDI.setHandleNoteOff(OnNoteOff);
  usbMIDI.setHandleControlChange(OnControlChange);
  usbMIDI.setHandleProgramChange(OnProgramChange);
  usbMIDI.setHandlePitchChange(OnPitchChange);
  usbMIDI.setHandleRealTimeSystem(HandleRealtimeMessage);
}


//################ implementation

void OnProgramChange(byte channel, byte program){
  if(DEBUG_MIDI_INPUT) Serial.printf("%Program Change on Channel: %d, Program: % ",channel, program ); 
  current_preset = program;
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

void HandleRealtimeMessage(uint8_t realtimebyte){

  switch(realtimebyte){

    case 0xF8: // hex 248 System Real Time, Timing Clock 24 per quarter note
      if(mySettings.MasterClockToLfos || mySettings.MasterClockToMODLfo || mySettings.MasterClockToVOLLfo || mySettings.MasterclockToFreqLFO){
        MidiTimeClockCount += 1;
        MiditimeClockTime = millis();
        MidismothedDiffVal = float(MiditimeClockTime - MidioldTimeClockMilli); 
        MidismothedDiffValFiltered = (MidismothedDiffVal * (1 - 0.91)) + (MidismothedDiffValFiltered  *  0.91);
        MidismothedDiffValFilteredSummed += MidismothedDiffValFiltered;
        if(MidiTimeClockCount >24){
  
          MidiCool = float(round( MidismothedDiffValFilteredSummed ));
          MidiBpm = 60000.0 / MidiCool;
          if(DEBUG_MIDI_INPUT) Serial.printf("%f %f cool int %f, Bpm %f\n", MidismothedDiffVal,MidismothedDiffValFiltered,MidiCool,MidiBpm );
          //if(DEBUG_MIDI_INPUT) Serial.printf("%d %d\n", realtimebyte,timeClockTime-oldTimeClockMilli );      
  
          MidiTimeClockCount = 1;
          MidismothedDiffValFilteredSummed = 0.0;
          
          mySystemBpm = MidiBpm;
          mysystemFreqMicrosTime = MidiCool * 1000;
          sequencerSpeedTime = int(mysystemFreqMicrosTime / beatMultiLookup[mySettings.sequenceTimeSignature]);
          systemOnMidiClock = true;
        }
        MidioldTimeClockMilli = MiditimeClockTime;
      }
      
    break;
    
    default:
      if(DEBUG_MIDI_INPUT){
        Serial.printf("R-T-M: %d currently not implemented\n",realtimebyte );
      }       
 
  }
  
}

// Note On Off / Gui Request Preset and Sequencer notes
//Sequencer Notes need note 126 and 0 with 1 velocety bevor each Sequencer note
bool getSeqNote126 = false;
bool getSeqNote1 = false;
void OnNoteOn(byte channel, byte note, byte velocity){
  if(!channeltest(channel)) return;  
  if(DEBUG_MIDI_INPUT){
    Serial.printf("Receive Midi Note On, NOTE: %d | VEL: %d\n",note, velocity ); 
  }
  if((note==127 || note==0) && velocity == 1)
    Midi_Request_Settings(note);
  else
  { 
   if(DEBUG_MIDI_INPUT){
      Serial.printf("Request Sequence 126: %d 1: %d\n",getSeqNote126, getSeqNote1 ); 
   }
      
   if(getSeqNote126 && getSeqNote1){//
     // midi note 0- 31 enthält die pitch offsets
     if(note < 32){
      mySettings.sequenzerNotes[note] = velocity-25;
      if(DEBUG_MIDI_INPUT){
        Serial.printf("Receive a Sequencer note, NOTE: %d | VEL: %d | mean Add note offset value: %d at position: %d \n",note, velocity,velocity-25, note ); 
      }
     }

     // note 32 enthält die steps
     if(note == 32){
      mySettings.sequeceSteps = velocity - 1;
      if(DEBUG_MIDI_INPUT){
        Serial.printf("Receive a Sequencer Steps: %d | and settings value of mySettings.sequeceSteps = %d \n", velocity, mySettings.sequeceSteps ); 
      }     
     }
     // note 33 enthält die time signature
     if(note == 33){
      int8_t valuee = velocity - 1;
      if(valuee > 20) valuee = 20;
      mySettings.sequenceTimeSignature = valuee;
      sequencerSpeedTime = int(mysystemFreqMicrosTime / beatMultiLookup[mySettings.sequenceTimeSignature]);
      if(DEBUG_MIDI_INPUT){
        Serial.printf("Receive a Sequencer Time signature in raw: %d | and mySettings.sequenceTimeSignature = %d | beat multi = %f \n",velocity,mySettings.sequenceTimeSignature,beatMultiLookup[mySettings.sequenceTimeSignature]); 
      }
     }
     
     getSeqNote126 = false;
     getSeqNote1 = false;
     StopAnyPlayingNote();

   }
   else
   {
    StartStopMidiNote( true, millis(), int(velocity), int(note), 0 );    
   }
   
   if(note == 126 && velocity == 1) getSeqNote126 = true;
   if(note == 1 && velocity == 1) getSeqNote1 = true; 
  }
}


void OnNoteOff(byte channel, byte note, byte velocity){
  if(!channeltest(channel)) return;
  if(DEBUG_MIDI_INPUT){
    Serial.printf("Receive Midi Note Off, NOTE: %d | VEL: %d\n",note, velocity );  
  }
  
  StartStopMidiNote( false, millis(), int(velocity), int(note), 0);
}

void OnPitchChange(byte channel, int pitch){
  if(!channeltest(channel)) return;
  if(DEBUG_MIDI_INPUT){
    Serial.printf("Receive Midi Pitch Bend: %d\n",pitch - 8192 );  
  }
  myMidiNote.midiPitch = pitch - 8192; 
}



void OnControlChange(byte channel, byte control, byte value){
   if(!channeltest(channel) || GuiaskForPreset) return; 
   if(DEBUG_MIDI_INPUT){
    Serial.printf("Receive Midi CC: %d | VAL: %d | CHANNEL: %d ||",control, value, channel );
   }
   int loghelperValInt=0;
   float loghelperValFloat=0.0f;
   switch(control){

    case 0: // Expression Pedal Function

        mySettings.expressionPedalFunction = value;
        
        if(DEBUG_MIDI_INPUT){
          Serial.printf("Expression Pedal Function is set to Valu %d\n", mySettings.expressionPedalFunction);
        }        
    break;

    case 1:      
        mySettings.freeDataInt2 = value;
        
        if(DEBUG_MIDI_INPUT){
          Serial.printf("Poti Function is set to Valu %d\n", mySettings.freeDataInt2);
        } 
    break;
    
    case 2:
        if(value > 3) value = 3;
        mySettings.functionBtnFunction = value;

        if(value == 2){
          for(int i=0;i<4;i++){
            OscMainMix.gain(i, 0.0);
          }
        }
        else
        {
          for(int i=0;i<4;i++){
              OscMainMix.gain(i, mySettings.SYNTH_Master_Volume);
          }
        }
        

        if(DEBUG_MIDI_INPUT){
          Serial.printf("Function BTN Function is set to Valu %d\n", mySettings.functionBtnFunction);
        } 
    break;
    
    case 3:
        loghelperValInt=map(int(value),0,127,0,15);
        mySettings.freeDataInt1 = loghelperValInt;
        sgtl5000_1.lineInLevel(mySettings.freeDataInt1);
        
        if(DEBUG_MIDI_INPUT){
          Serial.printf("Gitar-InputGain %d\n", mySettings.freeDataInt1);
        }        
    break;

//    case 3:  
//      loghelperValInt=map(int(value),0,127,0,255);
//      mySettings.instrumentBypassMix = float(loghelperValInt);
//
//      setMCP42Value(byte(mySettings.instrumentBypassMix));
//      if(DEBUG_MIDI_INPUT){
//        Serial.printf("Gitar-Synth Mix %f\n", mySettings.instrumentBypassMix);
//      }         
//    break;

    // ############################## MIXER SECTION ############################################
    case 4: // Mixer Instrument Input To Synth Frequency - and Envelope - tracking, Volumen

        loghelperValInt=map(int(value),0,127,0,1000);
        loghelperValFloat=float(loghelperValInt)/1000.0;
        loghelperValFloat= pow(loghelperValFloat,5);
        
        mySettings.instrumentToSynth = loghelperValFloat;

        mixerAnalyse.gain(2, mySettings.instrumentToSynth * 2.0);
        
        if(DEBUG_MIDI_INPUT){
          Serial.printf("MIXER Instrument Input To Synth Frequency - and Envelope - tracking changing now to %f| and back to lin: %d\n",mySettings.instrumentToSynth, map( int(powf(mySettings.instrumentToSynth, 0.2)*1000)+5 ,0 , 1000, 0, 127) );
        }        
    break;

    case 5: // Mixer Usb 1 Input To Synth Frequency - and Envelope - tracking, Volumen

        loghelperValInt=map(int(value),0,127,0,1000);
        loghelperValFloat=float(loghelperValInt)/1000.0;
        loghelperValFloat= pow(loghelperValFloat,5);
        
        mySettings.usbIn_1_ToSynth = loghelperValFloat;

        mixerAnalyse.gain(0, mySettings.usbIn_1_ToSynth);
        
        if(DEBUG_MIDI_INPUT){
          Serial.printf("MIXER USB Input 1 To Synth Frequency - and Envelope - tracking changing now to %f| and back to lin: %d\n",mySettings.usbIn_1_ToSynth, map( int(powf(mySettings.usbIn_1_ToSynth, 0.2)*1000)+5 ,0 , 1000, 0, 127) );
        }        
    break;
    
    case 6: // Synth to output 1

        loghelperValInt=map(int(value),0,127,0,1000);
        loghelperValFloat=float(loghelperValInt)/1000.0;
        loghelperValFloat= pow(loghelperValFloat,5);
        
        mySettings.synthToOutput1 = loghelperValFloat;

         I2SOutputMixerChannel1.gain(3, mySettings.synthToOutput1);
        
        if(DEBUG_MIDI_INPUT){
          Serial.printf("MIXER Synth to Output 1 changing now to %f| and back to lin: %d\n",mySettings.synthToOutput1, map( int(powf(mySettings.synthToOutput1, 0.2)*1000)+5 ,0 , 1000, 0, 127) );
        }        
    break;

    case 7: // Synth to output 2

        loghelperValInt=map(int(value),0,127,0,1000);
        loghelperValFloat=float(loghelperValInt)/1000.0;
        loghelperValFloat= pow(loghelperValFloat,5);
        
        mySettings.synthToOutput2 = loghelperValFloat;

        I2SOutputMixerChannel2.gain(3, mySettings.synthToOutput2);
        
        if(DEBUG_MIDI_INPUT){
          Serial.printf("MIXER Synth to Outut 2 changing now to %f| and back to lin: %d\n",mySettings.synthToOutput2, map( int(powf(mySettings.synthToOutput2, 0.2)*1000)+5 ,0 , 1000, 0, 127) );
        }        
    break;
    
    case 8: // Synth to Usb output 1

        loghelperValInt=map(int(value),0,127,0,1000);
        loghelperValFloat=float(loghelperValInt)/1000.0;
        loghelperValFloat= pow(loghelperValFloat,5);
        
        mySettings.synthToUsbOutput1 = loghelperValFloat;

        UsbOutputMixerLinks.gain(3, mySettings.synthToUsbOutput1); 
        
        if(DEBUG_MIDI_INPUT){
          Serial.printf("MIXER Synth to Usb Output 1 changing now to %f| and back to lin: %d\n",mySettings.synthToUsbOutput1, map( int(powf(mySettings.synthToUsbOutput1, 0.2)*1000)+5 ,0 , 1000, 0, 127) );
        }        
    break;

    case 9: // Synth to Usb output 2

        loghelperValInt=map(int(value),0,127,0,1000);
        loghelperValFloat=float(loghelperValInt)/1000.0;
        loghelperValFloat= pow(loghelperValFloat,5);
        
        mySettings.synthToUsbOutput2 = loghelperValFloat;

        UsbOutputMixerRechts.gain(3, mySettings.synthToUsbOutput2);
        
        if(DEBUG_MIDI_INPUT){
          Serial.printf("MIXER Synth to Usb Outut 2 changing now to %f| and back to lin: %d\n",mySettings.synthToUsbOutput2, map( int(powf(mySettings.synthToUsbOutput2, 0.2)*1000)+5 ,0 , 1000, 0, 127) );
        }        
    break;

    case 10: // Instrument to Usb output 1

        loghelperValInt=map(int(value),0,127,0,1000);
        loghelperValFloat=float(loghelperValInt)/1000.0;
        loghelperValFloat= pow(loghelperValFloat,5);
        
        mySettings.instrumentToUsb_1 = loghelperValFloat;

        UsbOutputMixerLinks.gain(0, mySettings.instrumentToUsb_1); 
        
        if(DEBUG_MIDI_INPUT){
          Serial.printf("MIXER Instrument to Usb Output 1 changing now to %f| and back to lin: %d\n",mySettings.instrumentToUsb_1, map( int(powf(mySettings.instrumentToUsb_1, 0.2)*1000)+5 ,0 , 1000, 0, 127) );
        }        
    break;

    case 11: // Instrument to Usb output 2

        loghelperValInt=map(int(value),0,127,0,1000);
        loghelperValFloat=float(loghelperValInt)/1000.0;
        loghelperValFloat= pow(loghelperValFloat,5);
        
        mySettings.instrumentToUsb_2 = loghelperValFloat;

        UsbOutputMixerRechts.gain(0, mySettings.instrumentToUsb_2);
        
        if(DEBUG_MIDI_INPUT){
          Serial.printf("MIXER Instrument to Usb Outut 2 changing now to %f| and back to lin: %d\n",mySettings.instrumentToUsb_2, map( int(powf(mySettings.instrumentToUsb_2, 0.2)*1000)+5 ,0 , 1000, 0, 127) );
        }        
    break;

    // ################## Usb Input to Output Matrix ######################################

    case 12: // Usb Input 1 to output 1

        loghelperValInt=map(int(value),0,127,0,1000);
        loghelperValFloat=float(loghelperValInt)/1000.0;
        loghelperValFloat= pow(loghelperValFloat,5);
        
        mySettings.usbIn_1_ToOutput1 = loghelperValFloat;

        I2SOutputMixerChannel1.gain(1, mySettings.usbIn_1_ToOutput1); 
        
        if(DEBUG_MIDI_INPUT){
          Serial.printf("MIXER Usb Input 1 Outut 1 changing now to %f| and back to lin: %d\n",mySettings.usbIn_1_ToOutput1, map( int(powf(mySettings.usbIn_1_ToOutput1, 0.2)*1000)+5 ,0 , 1000, 0, 127) );
        }        
    break;

    case 13: //  Usb Input 1 to output 2

        loghelperValInt=map(int(value),0,127,0,1000);
        loghelperValFloat=float(loghelperValInt)/1000.0;
        loghelperValFloat= pow(loghelperValFloat,5);
        
        mySettings.usbIn_1_ToOutput2 = loghelperValFloat;

        I2SOutputMixerChannel2.gain(1, mySettings.usbIn_1_ToOutput2);
        
        if(DEBUG_MIDI_INPUT){
          Serial.printf("MIXER Usb Input 1 Outut 2 changing now to %f| and back to lin: %d\n",mySettings.usbIn_1_ToOutput2, map( int(powf(mySettings.usbIn_1_ToOutput2, 0.2)*1000)+5 ,0 , 1000, 0, 127) );
        }        
    break;


    case 14: // Usb Input 2 to output 1

        loghelperValInt=map(int(value),0,127,0,1000);
        loghelperValFloat=float(loghelperValInt)/1000.0;
        loghelperValFloat= pow(loghelperValFloat,5);
        
        mySettings.usbIn_2_ToOutput1 = loghelperValFloat;

        I2SOutputMixerChannel1.gain(2, mySettings.usbIn_2_ToOutput1);
        
        if(DEBUG_MIDI_INPUT){
          Serial.printf("MIXER Usb Input 2 Outut 1 changing now to %f| and back to lin: %d\n",mySettings.usbIn_2_ToOutput1, map( int(powf(mySettings.usbIn_2_ToOutput1, 0.2)*1000)+5 ,0 , 1000, 0, 127) );
        }        
    break;

    case 15: //  Usb Input 2 to output 2

        loghelperValInt=map(int(value),0,127,0,1000);
        loghelperValFloat=float(loghelperValInt)/1000.0;
        loghelperValFloat= pow(loghelperValFloat,5);
        
        mySettings.usbIn_2_ToOutput2 = loghelperValFloat;

        I2SOutputMixerChannel2.gain(2, mySettings.usbIn_2_ToOutput2);
        
        if(DEBUG_MIDI_INPUT){
          Serial.printf("MIXER Usb Input 2 Outut 2 changing now to %f| and back to lin: %d\n",mySettings.usbIn_2_ToOutput2, map( int(powf(mySettings.usbIn_2_ToOutput2, 0.2)*1000)+5 ,0 , 1000, 0, 127) );
        }        
    break;

    case 16: // D-F-F Filter1
        if(value > 4){ 
          if(DEBUG_MIDI_INPUT) Serial.printf("Filter 1 D-F-F to high value: %d\n",value);
        }
        else
        { 
          if(using_Filter1_Active_Frequenz_Tracking && value == 0){ // we disable active frequency tracking
            filter1.frequency(mySettings.filter1Freq);
          }
             
          mySettings.Filter1_Active_Frequenz_Tracking_Target = value;
          
          if(DEBUG_MIDI_INPUT){
            Serial.printf("Filter 1 D-F-F changing now to %d\n", mySettings.Filter1_Active_Frequenz_Tracking_Target);
          }
          if(mySettings.Filter1_Active_Frequenz_Tracking_Target > 0)
            using_Filter1_Active_Frequenz_Tracking = true;
          else
            using_Filter1_Active_Frequenz_Tracking = false; 
        }            
    break;

    case 17: // D-F-F Filter2
        if(value > 4){ 
          if(DEBUG_MIDI_INPUT) Serial.printf("Filter 2 D-F-F to high value: %d\n",value);
        }
        else
        { 
          if(using_Filter2_Active_Frequenz_Tracking && value == 0){ // we disable active frequency tracking
            filter2.frequency(mySettings.filter2Freq);
          }
          
          mySettings.Filter2_Active_Frequenz_Tracking_Target = value;
          
          if(DEBUG_MIDI_INPUT){
            Serial.printf("Filter 2 D-F-F  changing now to %d\n", mySettings.Filter2_Active_Frequenz_Tracking_Target);
          }
          if(mySettings.Filter2_Active_Frequenz_Tracking_Target > 0)
            using_Filter2_Active_Frequenz_Tracking = true;
          else
            using_Filter2_Active_Frequenz_Tracking = false;
            
        }            
    break;

    case 18: // freq LFO cycles
          mySettings.filter_lfo_cycles = value;
          
          if(DEBUG_MIDI_INPUT){
            Serial.printf("Filter LFO Cycles  changing now to %d\n", mySettings.filter_lfo_cycles);
          }         
    break;


    case 19: // mod LFO Cycles
          mySettings.mod_lfo_cycles = value;
          
          if(DEBUG_MIDI_INPUT){
            Serial.printf("MOD LFO Cycles changing now to %d\n", mySettings.mod_lfo_cycles);
          }
    break;






    
    
    case 20: // controll 14 = osc 1 - 4 mixer volumen
        
        loghelperValInt=map(int(value),0,127,0,1000);
        loghelperValFloat=float(loghelperValInt)/1000.0;
        loghelperValFloat= pow(loghelperValFloat,5);
        
        mySettings.SYNTH_Master_Volume = loghelperValFloat;
        for(int i=0;i<4;i++){
          OscMainMix.gain(i, loghelperValFloat);//0.1 Ausgang zum filter
        }
        
        if(DEBUG_MIDI_INPUT){
          Serial.printf("POST OSC Mixer Output Volume changing now to %f| and back to lin: %d\n",loghelperValFloat, map( int(powf(loghelperValFloat, 0.2)*1000)+5 ,0 , 1000, 0, 127) );
        }

            
    break;

    case 21: // controll 21 = filter frequency

        if(mySettings.Filter1_Active_Frequenz_Tracking_Target>0){

          loghelperValInt = map(int(value),0,127,-60,60);
          
          mySettings.int_Filter1_Active_Frequenz_Tracking = loghelperValInt;
        
          G_Filter1_Active_Frequenz_Tracking = calculateDynamicFilterFrequency(mySettings.int_Filter1_Active_Frequenz_Tracking);
          
          if(DEBUG_MIDI_INPUT){
            Serial.printf("Control Filter1 Cutoff og D-F-F Frequency changing now to OSC Frequency x %fz | raw %f \n", G_Filter1_Active_Frequenz_Tracking);
          }          
     
        }
        else
        {
        
          loghelperValInt=map(int(value),0,127,0,1000);
          loghelperValFloat=float(loghelperValInt)/1000.0;
          
          loghelperValFloat= (pow(loghelperValFloat,5)*20000)+10.0;
          
          mySettings.filter1Freq =int(loghelperValFloat);  
            
          filter1.frequency(mySettings.filter1Freq);
          if(DEBUG_MIDI_INPUT){
            Serial.printf("Control Filter1 Cutoff Frequency changing now to %dHz | raw %f \n",mySettings.filter1Freq,loghelperValFloat);
          }
        }    
    break;
   
    case 22: // controll 22 = filter resonance
        
        loghelperValFloat=float(value)*100.0;
        loghelperValInt=map(int(loghelperValFloat),0,12700,710,25000);       
        mySettings.filter1Reso =  float(loghelperValInt/1000.0);
        filter1.resonance(mySettings.filter1Reso);
        if(DEBUG_MIDI_INPUT){
          Serial.printf("Filter1 Resonance changing now to %f | and back to midi:%d \n",mySettings.filter1Reso, map(int(mySettings.filter1Reso * 1000.0)+5,710,25000,0,127));
        }
            
    break;

    case 23: // controll 23 = filter octave
        
        loghelperValFloat=float(value)*100.0;
        loghelperValInt=map(int(loghelperValFloat),0,12700,0,7000);       
        mySettings.filter1Octave =  float(loghelperValInt/1000.0);
        filter1.octaveControl(mySettings.filter1Octave);
        if(DEBUG_MIDI_INPUT){
          Serial.printf("Filter1 Octave changing now to %f| and back to midi: %d\n",mySettings.filter1Octave, map(int(mySettings.filter1Octave * 1000.0)+5,0,7000,0,127));
        }
            
    break;
    
    case 24: // controll 24 = filter2 frequency
         if(mySettings.Filter2_Active_Frequenz_Tracking_Target>0){

          loghelperValInt = map(int(value),0,127,-60,60);
          
          mySettings.int_Filter2_Active_Frequenz_Tracking = loghelperValInt;
        
          G_Filter2_Active_Frequenz_Tracking = calculateDynamicFilterFrequency(mySettings.int_Filter2_Active_Frequenz_Tracking);
          
          if(DEBUG_MIDI_INPUT){
            Serial.printf("Control Filter2 Cutoff og D-F-F Frequency changing now to OSC Frequency x %fz | raw %f \n", G_Filter2_Active_Frequenz_Tracking);
          }          
     
        }
        else
        {       
          loghelperValInt=map(int(value),0,127,0,1000);
          loghelperValFloat=float(loghelperValInt)/1000.0;        
          loghelperValFloat= (pow(loghelperValFloat,5)*20000)+10.0;
          mySettings.filter2Freq =int(loghelperValFloat);  
      
          filter2.frequency(mySettings.filter2Freq);
          if(DEBUG_MIDI_INPUT){
            Serial.printf("Control Filter2 Cutoff Frequency changing now to %dHz| raw %f|and back to lin: %d\n",mySettings.filter2Freq, powf( ((loghelperValFloat-10) / 20000.0 ), 0.2)*1000.0, map(int( powf( ((loghelperValFloat-10) / 20000.0 ), 0.2)*1000.0  )+5 ,0 , 1000, 0, 127) );
          }
        }
            
    break; 
    
    case 25: // controll 25 = filter2 resonance
        
        loghelperValFloat=float(value)*100.0;
        loghelperValInt=map(int(loghelperValFloat),0,12700,710,25000);       
        mySettings.filter2Reso =  float(loghelperValInt/1000.0);
        filter2.resonance(mySettings.filter2Reso);
        if(DEBUG_MIDI_INPUT){
          Serial.printf("Filter2 Resonance changing now to %f\n",mySettings.filter2Reso);
        }
            
    break;

    case 26: // controll 26 = filter2 octave
        
        loghelperValFloat=float(value)*100.0;
        loghelperValInt=map(int(loghelperValFloat),0,12700,0,7000);       
        mySettings.filter2Octave =  float(loghelperValInt/1000.0);
        filter2.octaveControl(mySettings.filter2Octave);
        if(DEBUG_MIDI_INPUT){
          Serial.printf("Filter2 Octave changing now to %f\n",mySettings.filter2Octave);
        }
            
    break;

    case 27:
    
        if(value<1 || value >3){
          if(DEBUG_MIDI_INPUT) Serial.printf("Filter2 Mode to low or to high value: %d\n",value);
        }
        else
        {
          mySettings.filter2Mode = value;
          calculateFilterMixer();
          if(DEBUG_MIDI_INPUT) Serial.printf("Filter2 Mode set to: %d\n",value);
        }
        
    break;

    case 28: // filter 1 2 blend
        loghelperValInt= map(value*100, 0,12700, -500, 500);
        loghelperValFloat = float(loghelperValInt) / 1000.0;
        mySettings.filter_1_2_blend = loghelperValFloat;
        calculateFilterMixer();
        if(DEBUG_MIDI_INPUT) Serial.printf("Filter 1-2 Mixer set to: %f| and back to midi: %d\n",loghelperValFloat, map(int(loghelperValFloat*1000.0)+5,-500,500,0,127));
        
    break;

    case 29: // filter 1 2 crossover envlope
        if(value >1){
          if(DEBUG_MIDI_INPUT) Serial.printf("Filter1-2  crossover Mod to high value: %d\n",value);
        }
        else
        {
          if(value == 0) mySettings.f1_f2_crossover_Modulation = 1.0;
          if(value == 1) mySettings.f1_f2_crossover_Modulation = -1.0;
          F1F2CutoffInverter_ctrl.amplitude(mySettings.f1_f2_crossover_Modulation,2);
          
          if(DEBUG_MIDI_INPUT) Serial.printf("Filter1-2 Crossover Mod is set to: %f\n",mySettings.f1_f2_crossover_Modulation);
        }       
    break;
        
    case 30: // filter 1 2 guitarsensing attack
        
        loghelperValInt=map(int(value),0,127,0,1000);
        loghelperValFloat=float(loghelperValInt)/1000.0;
        loghelperValFloat= pow(loghelperValFloat,5)*3000;
        mySettings.sensingAttack = int(loghelperValFloat)+1;
        
        if(DEBUG_MIDI_INPUT){
          // back
          loghelperValInt = map(int(powf((mySettings.sensingAttack-1)/3000.0 ,0.2)*1000)+6,1,1000,0,127);         
          Serial.printf("Git Sensing Attack %d | und zuruck: %d\n", mySettings.sensingAttack, loghelperValInt );
        }
             
    break;
    
    case 31: // filter 1 2 guitarsensing release
        
        loghelperValInt=map(int(value),0,127,0,1000);
        mySettings.sensingFall = loghelperValInt;
        myAttackDetector.setTresAndFall(mySettings.sensingTreshold, mySettings.sensingFall);
        if(DEBUG_MIDI_INPUT) Serial.printf("Git Sensing Release %d | und zuruck: %d\n",mySettings.sensingFall , map(mySettings.sensingFall+5,0,1000,0,127));
             
    break;

    case 32: // filter 1 2 guitarsensing N-O-C
        
        loghelperValInt=map(int(value),0,127,0,1000);
        loghelperValFloat=float(loghelperValInt)/1000.0;
        loghelperValFloat= pow(loghelperValFloat,5)*8000;
        mySettings.sensingNoteOffCorner = int(loghelperValFloat);
     
        if(DEBUG_MIDI_INPUT){
          // back
          loghelperValInt = map(int(powf((mySettings.sensingNoteOffCorner)/8000.0 ,0.2)*1000)+6,1,1000,0,127);     
          Serial.printf("Git Sensing Noc %d | und zuruck: %d\n", mySettings.sensingNoteOffCorner, loghelperValInt );
        }
             
    break;
    
    case 33: // filter 1 2 guitarsensing treshold
        
        loghelperValInt=map(int(value),0,127,0,1000);
        loghelperValFloat=float(loghelperValInt)/1000.0;
        loghelperValFloat= pow(loghelperValFloat,5)*25000;
        mySettings.pitchDetectTreshold = int(loghelperValFloat);
         
        if(DEBUG_MIDI_INPUT){
          // back
          loghelperValInt = map(int(powf((mySettings.pitchDetectTreshold)/25000.0 ,0.2)*1000)+6,1,1000,0,127);
          Serial.printf("Git Sensing treshold %d | und zuruck: %d\n", mySettings.pitchDetectTreshold, loghelperValInt );
        }             
    break;

    case 34: // filter 1 2 guitarsensing quality
        
        loghelperValInt=map(int(value),0,127,0,990);
        loghelperValFloat = float(loghelperValInt)/1000.0;
        mySettings.pitchDetectQuality = loghelperValFloat;
        
        if(DEBUG_MIDI_INPUT) Serial.printf("Git Sensing Pitch Qualuty %f | und zuruck: %d\n",mySettings.pitchDetectQuality , map(int(mySettings.pitchDetectQuality*1000.0)+5,0,990,0,127));
             
    break;

    // OSC 1
    case 35: // OSC 1 Volumen
        
        loghelperValInt=map(int(value),0,127,0,1000);
        loghelperValFloat=float(loghelperValInt)/1000.0;
        loghelperValFloat= pow(loghelperValFloat,5);
        
        mySettings.osc1_level = loghelperValFloat;
        Osc1.amplitude(mySettings.osc1_level);
        
        if(DEBUG_MIDI_INPUT){
          Serial.printf("POST OSC 1 Volume changing now to %f| and back to lin: %d\n",loghelperValFloat, map( int(powf(loghelperValFloat, 0.2)*1000)+5 ,0 , 1000, 0, 127) );
        }
             
    break;

    case 36: // OSC1 WaveForm
    
        if(value >15){ // OSC 1 + 10 abetarys
          if(DEBUG_MIDI_INPUT) Serial.printf("OSC1 WaveForm to high value: %d\n",value);
        }
        else
        {
          mySettings.osc1_waveform = value;

          if(mySettings.osc1_waveform > 5) selectABTWaveTableOSC1(mySettings.osc1_waveform -6);
          if(mySettings.osc1_waveform < 6) Osc1.begin(mySettings.osc1_level, osc1hz , waveIntsLook[mySettings.osc1_waveform] );
                
          if(DEBUG_MIDI_INPUT) Serial.printf("OSC2 WaveForm set to: %d\n",value);
        }
        
    break;
    
    case 37: // OSC 1 Pulse Wide
        
        loghelperValInt=map(int(value),0,127,10,1000);
        loghelperValFloat=float(loghelperValInt)/1000.0;
        
        mySettings.osc1_pulseWidth = loghelperValFloat;
        Osc1.pulseWidth(mySettings.osc1_pulseWidth);
        if(DEBUG_MIDI_INPUT){
          Serial.printf("OSC 1 Pulsewide changing now to %f| and back to lin: %d\n",loghelperValFloat, map( int(mySettings.osc1_pulseWidth*1000)+5 ,10 , 1000, 0, 127) );
        }
             
    break;
    
    case 38: // OSC 1 Octave
        if(value > 8){ // OSC 1 Octave
          if(DEBUG_MIDI_INPUT) Serial.printf("OSC1 Octave to high value: %d\n",value);
        }
        else
        {     
          mySettings.osc1_octave = value - 3;
          
          if(DEBUG_MIDI_INPUT){
            Serial.printf("OSC 1 Octave changing now to %d\n", mySettings.osc1_octave);
          }
        }
    break;
       
    case 39: // OSC 1 Note
        if(value > 24){ // OSC 1 Note
          if(DEBUG_MIDI_INPUT) Serial.printf("OSC1 Note to high value: %d\n",value);
        }
        else
        {     
          mySettings.osc1_note = value - 12;
          
          if(DEBUG_MIDI_INPUT){
            Serial.printf("OSC 1 Note changing now to %d\n", mySettings.osc1_note);
          }
        }            
    break;

    case 40: // OSC 1 Cents
        if(value > 24){ // OSC 1 Detune cents
          if(DEBUG_MIDI_INPUT) Serial.printf("OSC1 Detune to high value: %d\n",value);
        }
        else
        {     
          mySettings.osc1_detune = value - 12;
          
          if(DEBUG_MIDI_INPUT){
            Serial.printf("OSC 1 Detune changing now to %d\n", mySettings.osc1_detune);
          }
        }            
    break;

    case 41: // OSC 1 Phase
    
        loghelperValInt=map(int(value),0,127,0,360);
        mySettings.osc1_phase = loghelperValInt;
        Osc1.phase(mySettings.osc1_phase);
        
        if(DEBUG_MIDI_INPUT){
          Serial.printf("OSC 1 Phase changing now to %d | und zurueck: %d\n", mySettings.osc1_phase, map(mySettings.osc1_phase + 1,0,360,0,127));
        }
            
    break;


    // OSC 2

    case 42: // OSC 2 Volumen
        
        loghelperValInt=map(int(value),0,127,0,1000);
        loghelperValFloat=float(loghelperValInt)/1000.0;
        loghelperValFloat= pow(loghelperValFloat,5);
        
        mySettings.osc2_level = loghelperValFloat;
        Osc2.amplitude(mySettings.osc2_level);
        
        if(DEBUG_MIDI_INPUT){
          Serial.printf("POST OSC 2 Volume changing now to %f| and back to lin: %d\n",loghelperValFloat, map( int(powf(loghelperValFloat, 0.2)*1000)+5 ,0 , 1000, 0, 127) );
        }
             
    break;

    case 43: // OSC2 WaveForm
    
        if(value >5){ // OSC 2 
          if(DEBUG_MIDI_INPUT) Serial.printf("OSC2 WaveForm to high value: %d\n",value);
        }
        else
        {
          mySettings.osc2_waveform = value;

          //if(mySettings.osc2_waveform > 5) selectABTWaveTableOSC1(1, mySettings.osc2_waveform -6);
          if(mySettings.osc2_waveform < 6) Osc2.begin(mySettings.osc2_level, osc2hz , waveIntsLook[mySettings.osc2_waveform] );
                
          if(DEBUG_MIDI_INPUT) Serial.printf("Osc2 WaveForm set to: %d\n",value);
        }
        
    break;
    case 44: // OSC 2 Pulse Wide
        
        loghelperValInt=map(int(value),0,127,10,1000);
        loghelperValFloat=float(loghelperValInt)/1000.0;
        
        mySettings.osc2_pulseWidth = loghelperValFloat;
        Osc2.pulseWidth(mySettings.osc2_pulseWidth);
        if(DEBUG_MIDI_INPUT){
          Serial.printf("OSC 2 Pulsewide changing now to %f| and back to lin: %d\n",loghelperValFloat, map( int(mySettings.osc2_pulseWidth*1000)+5 ,10 , 1000, 0, 127) );
        }
             
    break;
    
    case 45: // OSC 2 Octave
        if(value > 8){ // OSC 1 Octave
          if(DEBUG_MIDI_INPUT) Serial.printf("OSC2 Octave to high value: %d\n",value);
        }
        else
        {     
          mySettings.osc2_octave = value - 3;
          
          if(DEBUG_MIDI_INPUT){
            Serial.printf("OSC 2 Octave changing now to %d\n", mySettings.osc2_octave);
          }
        }

    break;
        
    case 46: // OSC 2 Note
        if(value > 24){ // OSC 1 Note
          if(DEBUG_MIDI_INPUT) Serial.printf("OSC2 Note to high value: %d\n",value);
        }
        else
        {     
          mySettings.osc2_note = value - 12;
          
          if(DEBUG_MIDI_INPUT){
            Serial.printf("OSC 2 Note changing now to %d\n", mySettings.osc2_note);
          }
        }            
    break;

    case 47: // OSC 2 Cents
        if(value > 24){ // OSC 1 Detune cents
          if(DEBUG_MIDI_INPUT) Serial.printf("OSC2 Detune to high value: %d\n",value);
        }
        else
        {     
          mySettings.osc2_detune = value - 12;
          
          if(DEBUG_MIDI_INPUT){
            Serial.printf("OSC 2 Detune changing now to %d\n", mySettings.osc2_detune);
          }
        }            
    break;

    case 48: // OSC 2 Phase
    
        loghelperValInt=map(int(value),0,127,0,360);
        mySettings.osc2_phase = loghelperValInt;
        Osc2.phase(mySettings.osc2_phase);
        
        if(DEBUG_MIDI_INPUT){
          Serial.printf("OSC 2 Phase changing now to %d | und zurueck: %d\n", mySettings.osc2_phase, map(mySettings.osc2_phase + 1,0,360,0,127));
        }

    break;

        // SUB OSC 1

    case 49: // SUB_OSC 1 Volumen
        
        loghelperValInt=map(int(value),0,127,0,1000);
        loghelperValFloat=float(loghelperValInt)/1000.0;
        loghelperValFloat= pow(loghelperValFloat,5);
        
        mySettings.sub_osc1_level = loghelperValFloat;
        SubOsc1.amplitude(mySettings.sub_osc1_level);
        
        if(DEBUG_MIDI_INPUT){
          Serial.printf("POST SUB_OSC 1 Volume changing now to %f| and back to lin: %d\n",loghelperValFloat, map( int(powf(loghelperValFloat, 0.2)*1000)+5 ,0 , 1000, 0, 127) );
        }
             
    break;

    case 50: // SUB_OSC1 WaveForm
    
        if(value >15){ // SUB_OSC 1 + 10 abetarys
          if(DEBUG_MIDI_INPUT) Serial.printf("SUB_OSC1 WaveForm to high value: %d\n",value);
        }
        else
        {
          mySettings.sub_osc1_waveform = value;

          if(mySettings.sub_osc1_waveform > 5) selectABTWaveTableOSC1(mySettings.sub_osc1_waveform -6);
          if(mySettings.sub_osc1_waveform < 6) SubOsc1.begin(mySettings.sub_osc1_level, subosc1hz , waveIntsLook[mySettings.sub_osc1_waveform] );
                
          if(DEBUG_MIDI_INPUT) Serial.printf("SubOSC2 WaveForm set to: %d\n",value);
        }
        
    break;
    
    case 51: // SUB_OSC 1 Pulse Wide
        
        loghelperValInt=map(int(value),0,127,10,1000);
        loghelperValFloat=float(loghelperValInt)/1000.0;
        
        mySettings.sub_osc1_pulseWidth = loghelperValFloat;
        SubOsc1.pulseWidth(mySettings.sub_osc1_pulseWidth);
        if(DEBUG_MIDI_INPUT){
          Serial.printf("SUB_OSC 1 Pulsewide changing now to %f| and back to lin: %d\n",loghelperValFloat, map( int(mySettings.sub_osc1_pulseWidth*1000)+5 ,10 , 1000, 0, 127) );
        }
             
    break;
    
    case 52: // SUB_OSC 1 Octave
        if(value > 8){ // SUB_OSC 1 Octave
          if(DEBUG_MIDI_INPUT) Serial.printf("SUB_OSC1 Octave to high value: %d\n",value);
        }
        else
        {     
          mySettings.sub_osc1_octave = value - 3;
          
          if(DEBUG_MIDI_INPUT){
            Serial.printf("SUB_OSC 1 Octave changing now to %d\n", mySettings.sub_osc1_octave);
          }
        }
    break;
        
    case 53: // SUB_OSC 1 Note
        if(value > 24){ // SUB_OSC 1 Note
          if(DEBUG_MIDI_INPUT) Serial.printf("SUB_OSC1 Note to high value: %d\n",value);
        }
        else
        {     
          mySettings.sub_osc1_note = value - 12;
          
          if(DEBUG_MIDI_INPUT){
            Serial.printf("SUB_OSC 1 Note changing now to %d\n", mySettings.sub_osc1_note);
          }
        }            
    break;

    case 54: // SUB_OSC 1 Cents
        if(value > 24){ // SUB_OSC 1 Detune cents
          if(DEBUG_MIDI_INPUT) Serial.printf("SUB_OSC1 Detune to high value: %d\n",value);
        }
        else
        {     
          mySettings.sub_osc1_detune = value - 12;
          
          if(DEBUG_MIDI_INPUT){
            Serial.printf("SUB_OSC 1 Detune changing now to %d\n", mySettings.sub_osc1_detune);
          }
        }            
    break;

    case 55: // SUB_OSC 1 Phase
    
        loghelperValInt=map(int(value),0,127,0,360);
        mySettings.sub_osc1_phase = loghelperValInt;
        SubOsc1.phase(mySettings.sub_osc1_phase);
        
        if(DEBUG_MIDI_INPUT){
          Serial.printf("SUB_OSC 1 Phase changing now to %d | und zurueck: %d\n", mySettings.sub_osc1_phase, map(mySettings.sub_osc1_phase + 1,0,360,0,127));
        }
            
    break;


    // SUB_OSC 2

    case 56: // SUB_OSC 2 Volumen
        
        loghelperValInt=map(int(value),0,127,0,1000);
        loghelperValFloat=float(loghelperValInt)/1000.0;
        loghelperValFloat= pow(loghelperValFloat,5);
        
        mySettings.sub_osc2_level = loghelperValFloat;
        SubOsc2.amplitude(mySettings.sub_osc2_level);
        
        if(DEBUG_MIDI_INPUT){
          Serial.printf("POST SUB_OSC 2 Volume changing now to %f| and back to lin: %d\n",loghelperValFloat, map( int(powf(loghelperValFloat, 0.2)*1000)+5 ,0 , 1000, 0, 127) );
        }
             
    break;

    case 57: // SUB_OSC2 WaveForm
    
        if(value >5){ // SUB_OSC 2 
          if(DEBUG_MIDI_INPUT) Serial.printf("SUB_OSC2 WaveForm to high value: %d\n",value);
        }
        else
        {
          mySettings.sub_osc2_waveform = value;

          //if(mySettings.sub_osc2_waveform > 5) selectABTWaveTableOSC1(1, mySettings.sub_osc2_waveform -6);
          if(mySettings.sub_osc2_waveform < 6) SubOsc2.begin(mySettings.sub_osc2_level, subosc2hz , waveIntsLook[mySettings.sub_osc2_waveform] );
                
          if(DEBUG_MIDI_INPUT) Serial.printf("Sub Osc2 WaveForm set to: %d\n",value);
        }
        
    break;
    case 58: // SUB_OSC 2 Pulse Wide
        
        loghelperValInt=map(int(value),0,127,10,1000);
        loghelperValFloat=float(loghelperValInt)/1000.0;
        
        mySettings.sub_osc2_pulseWidth = loghelperValFloat;
        SubOsc2.pulseWidth(mySettings.sub_osc2_pulseWidth);
        if(DEBUG_MIDI_INPUT){
          Serial.printf("SUB_OSC 2 Pulsewide changing now to %f| and back to lin: %d\n",loghelperValFloat, map( int(mySettings.sub_osc2_pulseWidth*1000)+5 ,10 , 1000, 0, 127) );
        }
             
    break;
    
    case 59: // SUB_OSC 2 Octave
        if(value > 8){ // SUB_OSC 1 Octave
          if(DEBUG_MIDI_INPUT) Serial.printf("SUB_OSC2 Octave to high value: %d\n",value);
        }
        else
        {     
          mySettings.sub_osc2_octave = value - 3;
          
          if(DEBUG_MIDI_INPUT){
            Serial.printf("SUB_OSC 2 Octave changing now to %d\n", mySettings.sub_osc2_octave);
          }
        }
    break;
        
    case 60: // SUB_OSC 2 Note
        if(value > 24){ // SUB_OSC 1 Note
          if(DEBUG_MIDI_INPUT) Serial.printf("SUB_OSC2 Note to high value: %d\n",value);
        }
        else
        {     
          mySettings.sub_osc2_note = value - 12;
          
          if(DEBUG_MIDI_INPUT){
            Serial.printf("SUB_OSC 2 Note changing now to %d\n", mySettings.sub_osc2_note);
          }
        }            
    break;

    case 61: // SUB_OSC 2 Cents
        if(value > 24){ // SUB_OSC 1 Detune cents
          if(DEBUG_MIDI_INPUT) Serial.printf("SUB_OSC2 Detune to high value: %d\n",value);
        }
        else
        {     
          mySettings.sub_osc2_detune = value - 12;
          
          if(DEBUG_MIDI_INPUT){
            Serial.printf("SUB_OSC 2 Detune changing now to %d\n", mySettings.sub_osc2_detune);
          }
        }            
    break;

    case 62: // SUB_OSC 2 Phase
    
        loghelperValInt=map(int(value),0,127,0,360);
        mySettings.sub_osc2_phase = loghelperValInt;
        SubOsc2.phase(mySettings.sub_osc2_phase);
        
        if(DEBUG_MIDI_INPUT){
          Serial.printf("SUB_OSC 2 Phase changing now to %d | und zurueck: %d\n", mySettings.sub_osc2_phase, map(mySettings.sub_osc2_phase + 1,0,360,0,127));
        }
            
    break;  

    case 63: // Noise Volumen
        
        loghelperValInt=map(int(value),0,127,0,1000);
        loghelperValFloat=float(loghelperValInt)/1000.0;
        loghelperValFloat= pow(loghelperValFloat,5);
        
        mySettings.noise_level = loghelperValFloat;
        noise1.amplitude(mySettings.noise_level);
        
        if(DEBUG_MIDI_INPUT){
          Serial.printf("Noise  Volume changing now to %f| and back to lin: %d\n",loghelperValFloat, map( int(powf(loghelperValFloat, 0.2)*1000)+5 ,0 , 1000, 0, 127) );
        }
             
    break;
    
    case 64: // Bit Crusher Volumen
        
        loghelperValInt=map(int(value),0,127,0,1000);
        loghelperValFloat=float(loghelperValInt)/1000.0;
        loghelperValFloat= pow(loghelperValFloat,5);
        
        mySettings.Crusher_Volumen = loghelperValFloat;
        SubOscMixer.gain(2, mySettings.Crusher_Volumen);
        
        if(DEBUG_MIDI_INPUT){
          Serial.printf("Bit Crusher Volume changing now to %f| and back to lin: %d\n",loghelperValFloat, map( int(powf(loghelperValFloat, 0.2)*1000)+5 ,0 , 1000, 0, 127) );
        }
             
    break;

    
    case 65: // Bit Crusher Bits
        if(value > 15){
          if(DEBUG_MIDI_INPUT) Serial.printf("Bit Crusher Bits to high value: %d\n",value);
        }
        else
        {          
          mySettings.Crusher_Bits = value+1;     
          bitcrusher1.bits(mySettings.Crusher_Bits);    
          
          if(DEBUG_MIDI_INPUT){
            Serial.printf("Bit Crusher Bits changing now to %d \n",value +1 );
          }
        }
             
    break;

    case 66: // Bit Crusher Rate
        if(value > 6){
          if(DEBUG_MIDI_INPUT) Serial.printf("Bit Crusher Sample rate to high value: %d\n",value);
        }
        else
        {  

          mySettings.Crusher_SampleRate = value;
          bitcrusher1.sampleRate(bitCrusherSampleRate[mySettings.Crusher_SampleRate]);               
         
          if(DEBUG_MIDI_INPUT){
            Serial.printf("Bit Crusher Bits at index %d, change to rate: %f\n",value, bitCrusherSampleRate[mySettings.Crusher_SampleRate]);
          }
        }
             
    break;

    // Filter LFO
    case 67: // Filter WaveForm
    
        if(value >11){ // 
          if(DEBUG_MIDI_INPUT) Serial.printf("Filter LFO WaveForm to high value: %d\n",value);
        }
        else
        {
          mySettings.filter_lfo_waveform = value;
          if(mySettings.filter_lfo_waveform > 5) selectABTWaveTableFilterLFO( bpmToHz(mySettings.filter_lfo_bpm), mySettings.filter_lfo_waveform -6);
          if(mySettings.filter_lfo_waveform < 6) Filter_LFO.begin(1.0, bpmToHz(mySettings.filter_lfo_bpm) * beatMultiLookup[mySettings.filter_lfo_freq_bpm_multi] , waveIntsLook[mySettings.filter_lfo_waveform], mySettings.filter_lfo_cycles); 
                
          if(DEBUG_MIDI_INPUT) Serial.printf("Filter LFO WaveForm set to: %d\n",value);
        }
        
   break;
    
   case 68: // Filter Lfo PulseWide
     
        loghelperValInt=map(int(value),0,127,10,1000);
        loghelperValFloat=float(loghelperValInt)/1000.0;
        
        mySettings.filter_lfo_pulseWidth = loghelperValFloat;
        Filter_LFO.pulseWidth(mySettings.filter_lfo_pulseWidth);
        if(DEBUG_MIDI_INPUT){
          Serial.printf("Filter LFO 2 Pulsewide changing now to %f| and back to lin: %d\n",loghelperValFloat, map( int(mySettings.filter_lfo_pulseWidth*1000)+5 ,10 , 1000, 0, 127) );
        } 
   break;
   
   case 69: // Filter Lfo Bpm
    
        if(mySettings.MasterClockToLfos || mySettings.MasterclockToFreqLFO){
          if(DEBUG_MIDI_INPUT) Serial.printf("Filter BPM on CLOCK SYNC! BREAK\n"); 
          break;
        }
        
        loghelperValInt=map(int(value),0,127,0,360);
        mySettings.filter_lfo_bpm = float(loghelperValInt);
        
        if(DEBUG_MIDI_INPUT) Serial.printf("Filter LFO BPM set to: %f| und zuruck: %d\n", mySettings.filter_lfo_bpm, map(mySettings.filter_lfo_bpm + 2,0,360,0,127));    
    
   break;


   case 70: // Filter Lfo Bpm multip
     
        if(value >20){
          if(DEBUG_MIDI_INPUT) Serial.printf("Filter LFO BPM Multiplier to high value: %d\n",value);
        }
        else
        {
           mySettings.filter_lfo_freq_bpm_multi = value;
        }
        
        if(DEBUG_MIDI_INPUT) Serial.printf("Filter LFO BPM Multip set to: %s| midi val %d\n",beatMultiNamesLookup[mySettings.filter_lfo_freq_bpm_multi], mySettings.filter_lfo_freq_bpm_multi ); 
   break;

    case 71: // Filter LFO Phase
    
        loghelperValInt=map(int(value),0,127,0,360);
        mySettings.filter_lfo_phase = loghelperValInt;
        Filter_LFO.phase(mySettings.filter_lfo_phase);
        
        if(DEBUG_MIDI_INPUT){
          Serial.printf("Filter Lfo Phase changing now to %d | und zurueck: %d\n", mySettings.filter_lfo_phase, map(mySettings.filter_lfo_phase + 1,0,360,0,127));
        }
            
    break;
    
     case 72: // Filter lfo sync to attack
        
        if(value >1){
          if(DEBUG_MIDI_INPUT) Serial.printf("Filter LFO Sync To Attack to high value: %d\n",value);
        }
        else
        {
          if(value == 0) mySettings.filter_lfo_attack_sync = false;
          if(value == 1) mySettings.filter_lfo_attack_sync = true;
          
          if(DEBUG_MIDI_INPUT) Serial.printf("Filter LFO Sync to Attack is set to: %d\n",mySettings.filter_lfo_attack_sync);
        }
     break;

     case 73: // Filter Lfo Depth
     
         loghelperValInt=map(int(value),0,127,-1000,1000);
         loghelperValFloat=float(loghelperValInt)/1000.0;
            
         mySettings.filter_lfo_intensety = loghelperValFloat;
      
         Filter_LFO_depth.amplitude(mySettings.filter_lfo_intensety);
         if(value == 63 || value == 64) Filter_LFO_depth.amplitude(0.0);// Set the lfo to zero because we become no clear zero with floats
         
         if(DEBUG_MIDI_INPUT) Serial.printf("Filter LFO Depth is set to: %f | und zuruck %d\n",mySettings.filter_lfo_intensety, map( int(mySettings.filter_lfo_intensety*1000)+5 ,-1000 , 1000, 0, 127) );

         
     break;      

     
     case 74: // Attack envelop to FilterLFO Frequency
         
         loghelperValInt=map(int(value),0,127, 0,1000);
         loghelperValFloat=float(loghelperValInt)/100.0;
         mySettings.envelopeFollowerAttackToFilterLFOFrequenzy = loghelperValFloat;

         if(DEBUG_MIDI_INPUT) Serial.printf("Filter LFO Evelope Attack to Freq. is set to: %f | und zuruck %d\n",mySettings.envelopeFollowerAttackToFilterLFOFrequenzy, map( int(mySettings.envelopeFollowerAttackToFilterLFOFrequenzy*100)+5 , 0 , 1000, 0, 127) );
     break;

      case 75: // Filter Lfo Attack envelope to Depth
     
         loghelperValInt=map(int(value),0,127,-1000,1000);
         loghelperValFloat=float(loghelperValInt)/1000.0;
            
         mySettings.envelopeFollowerAttackToFilterLFODepth = loghelperValFloat;
      
         //if(value == 63 || value == 64) Filter_LFO_depth.amplitude(0.0);// Set the lfo to zero because we become no clear zero with floats
         
         if(DEBUG_MIDI_INPUT) Serial.printf("Filter LFO Attack envelope to Depth is set to: %f | und zuruck %d\n",mySettings.envelopeFollowerAttackToFilterLFODepth, map( int(mySettings.envelopeFollowerAttackToFilterLFODepth*1000)+5 ,-1000 , 1000, 0, 127) );

         
     break;

     case 76: // Attack envelope to Filter cutoff
     
         loghelperValInt=map(int(value),0,127,-1000,1000);
         loghelperValFloat=float(loghelperValInt)/1000.0;
            
         mySettings.filter1envelopeIntensity = loghelperValFloat;
         filter1envelopeIntensity.amplitude(mySettings.filter1envelopeIntensity);
         if(value == 63 || value == 64) filter1envelopeIntensity.amplitude(0.0); // Set to zero because we become no clear zero with floats
         
         if(DEBUG_MIDI_INPUT) Serial.printf(" Attack envelope to Filter Cutoff is set to: %f | und zuruck %d\n",mySettings.filter1envelopeIntensity, map( int(mySettings.filter1envelopeIntensity*1000)+5 ,-1000 , 1000, 0, 127) );
                
     break; 
    

     case 77: // gittare use filteradsr
          if(value >1){
            if(DEBUG_MIDI_INPUT) Serial.printf("Guitar use Filter DAHDSR to high value: %d\n",value);
          }
          else
          {
            if(value == 0) mySettings.gitarUseFilterADSR = false;
            if(value == 1) mySettings.gitarUseFilterADSR = true;
            
            if(DEBUG_MIDI_INPUT) Serial.printf("Guitar use Filter DAHDSR is set to: %d\n",mySettings.gitarUseFilterADSR);
          }
     break;
   
     case 78:      // gittare use envelop adsr
          if(value >1){
            if(DEBUG_MIDI_INPUT) Serial.printf("Guitar use Envelop AHDSR to high value: %d\n",value);
          }
          else
          {
            if(value == 0) mySettings.gitarUseEnvelopeADSR = false;
            if(value == 1) mySettings.gitarUseEnvelopeADSR = true;
            
            if(DEBUG_MIDI_INPUT) Serial.printf("Guitar use Envelope AHDSR is set to: %d\n",mySettings.gitarUseEnvelopeADSR);
          }
     break;

    // envelope ahdsr
    case 79: // envelope attack
        
        loghelperValInt=map(int(value),0,127,0,1000);
        loghelperValFloat=float(loghelperValInt)/1000.0;
        loghelperValFloat= pow(loghelperValFloat,3.0)*9999.0;
        mySettings.midi_attack = int(loghelperValFloat)+1;
        
        if(DEBUG_MIDI_INPUT){
          // back
          loghelperValInt = map(int(powf((mySettings.midi_attack-1)/9999.0 ,1.0/3.0)*1000)+7,1,1000,0,127);         
          Serial.printf("Envelope Attack %d | und zuruck: %d\n", mySettings.midi_attack, loghelperValInt );
        }
             
    break;   

    case 80: // envelope hold
        
        loghelperValInt=map(int(value),0,127,0,1000);
        loghelperValFloat=float(loghelperValInt)/1000.0;
        loghelperValFloat= pow(loghelperValFloat,3.0)*9999.0;
        mySettings.midi_hold = int(loghelperValFloat)+1;
        
        if(DEBUG_MIDI_INPUT){
          // back
          loghelperValInt = map(int(powf((mySettings.midi_hold-1)/9999.0 ,1.0/3.0)*1000)+7,1,1000,0,127);         
          Serial.printf("Envelope Hold %d | und zuruck: %d\n", mySettings.midi_hold, loghelperValInt );
        }
             
    break; 

    case 81: // envelope decay
        
        loghelperValInt=map(int(value),0,127,0,1000);
        loghelperValFloat=float(loghelperValInt)/1000.0;
        loghelperValFloat= pow(loghelperValFloat,3.0)*9999.0;
        mySettings.midi_decay = int(loghelperValFloat)+1;
        
        if(DEBUG_MIDI_INPUT){
          // back
          loghelperValInt = map(int(powf((mySettings.midi_decay-1)/9999.0 ,1.0/3.0)*1000)+7,1,1000,0,127);         
          Serial.printf("Envelope Decay %d | und zuruck: %d\n", mySettings.midi_decay, loghelperValInt );
        }
             
    break;

    case 82: // envelope sustain
        if(value >100){
         if(DEBUG_MIDI_INPUT) Serial.printf("Envelop AHDSR Sustain to high value: %d\n",value);
        }
        else
        {       
  
          mySettings.midi_sustain = value;
          
          if(DEBUG_MIDI_INPUT){        
            Serial.printf("Envelope Sustain %d | und zuruck: %d\n", mySettings.midi_sustain, value);
          }
        }
             
    break; 

    case 83: // envelope release
        
        loghelperValInt=map(int(value),0,127,0,1000);
        loghelperValFloat=float(loghelperValInt)/1000.0;
        loghelperValFloat= pow(loghelperValFloat,3.0)*9999.0;
        mySettings.midi_release = int(loghelperValFloat)+1;
        
        if(DEBUG_MIDI_INPUT){
          // back
          loghelperValInt = map(int(powf((mySettings.midi_release-1)/9999.0 ,1.0/3.0)*1000)+7,1,1000,0,127);         
          Serial.printf("Envelope release %d | und zuruck: %d\n", mySettings.midi_release, loghelperValInt );
        }
             
    break;

    // ###### filter dahdsr ####################################################
    case 84: // ADSR filter delay
        
        loghelperValInt=map(int(value),0,127,0,1000);
        loghelperValFloat=float(loghelperValInt)/1000.0;
        loghelperValFloat= pow(loghelperValFloat,3.0)*9999.0;
        mySettings.filter_delay = int(loghelperValFloat)+1;
        
        if(DEBUG_MIDI_INPUT){
          // back
          loghelperValInt = map(int(powf((mySettings.filter_delay-1)/9999.0 ,1.0/3.0)*1000)+7,1,1000,0,127);         
          Serial.printf("ADSR Filter Delay %d | und zuruck: %d\n", mySettings.filter_delay, loghelperValInt );
        }
             
    break; 

    
    case 85: // ADSR filter attack
        
        loghelperValInt=map(int(value),0,127,0,1000);
        loghelperValFloat=float(loghelperValInt)/1000.0;
        loghelperValFloat= pow(loghelperValFloat,3.0)*9999.0;
        mySettings.filter_attack = int(loghelperValFloat)+1;
        
        if(DEBUG_MIDI_INPUT){
          // back
          loghelperValInt = map(int(powf((mySettings.filter_attack-1)/9999.0 ,1.0/3.0)*1000)+7,1,1000,0,127);         
          Serial.printf("ADSR Filter Attack %d | und zuruck: %d\n", mySettings.filter_attack, loghelperValInt );
        }
             
    break;   

    case 86: // ADSR Filter hold
        
        loghelperValInt=map(int(value),0,127,0,1000);
        loghelperValFloat=float(loghelperValInt)/1000.0;
        loghelperValFloat= pow(loghelperValFloat,3.0)*9999.0;
        mySettings.filter_hold = int(loghelperValFloat)+1;
        
        if(DEBUG_MIDI_INPUT){
          // back
          loghelperValInt = map(int(powf((mySettings.filter_hold-1)/9999.0 ,1.0/3.0)*1000)+7,1,1000,0,127);         
          Serial.printf("ADSR Filter Hold %d | und zuruck: %d\n", mySettings.filter_hold, loghelperValInt );
        }
             
    break; 

    case 87: // ADSR Filter decay
        
        loghelperValInt=map(int(value),0,127,0,1000);
        loghelperValFloat=float(loghelperValInt)/1000.0;
        loghelperValFloat= pow(loghelperValFloat,3.0)*9999.0;
        mySettings.filter_decay = int(loghelperValFloat)+1;
        
        if(DEBUG_MIDI_INPUT){
          // back
          loghelperValInt = map(int(powf((mySettings.filter_decay-1)/9999.0 ,1.0/3.0)*1000)+7,1,1000,0,127);         
          Serial.printf("ADSR Filter Decay %d | und zuruck: %d\n", mySettings.filter_decay, loghelperValInt );
        }
             
    break;

    case 88: // ADSR Filter sustain
        if(value >100){
         if(DEBUG_MIDI_INPUT) Serial.printf(" AHDSR Filter Sustain to high value: %d\n",value);
        }
        else
        {       
  
          mySettings.filter_sustain = value;
          
          if(DEBUG_MIDI_INPUT){        
            Serial.printf("ADSR Filter Sustain %d | und zuruck: %d\n", mySettings.filter_sustain, value);
          }
        }
             
    break; 

    case 89: // ADSR Filter release
        
        loghelperValInt=map(int(value),0,127,0,1000);
        loghelperValFloat=float(loghelperValInt)/1000.0;
        loghelperValFloat= pow(loghelperValFloat,3.0)*9999.0;
        mySettings.filter_release = int(loghelperValFloat)+1;
        
        if(DEBUG_MIDI_INPUT){
          // back
          loghelperValInt = map(int(powf((mySettings.filter_release-1)/9999.0 ,1.0/3.0)*1000)+7,1,1000,0,127);         
          Serial.printf("ADSR filter release %d | und zuruck: %d\n", mySettings.filter_release, loghelperValInt );
        }
             
    break;

    // ###########################################  MOD LFO ####################################################
    
    // Mod LFO
    case 90: // Mod Lfo WaveForm
    
        if(value >11){ // SUB_OSC 2 
          if(DEBUG_MIDI_INPUT) Serial.printf("Mod LFO WaveForm to high value: %d\n",value);
        }
        else
        {
          mySettings.mod_lfo_waveform = value;

          if(mySettings.mod_lfo_waveform > 5) selectABTWaveTableModLFO( bpmToHz(mySettings.mod_lfo_bpm), mySettings.mod_lfo_waveform -6);
          if(mySettings.mod_lfo_waveform < 6) MOD_LFO.begin(0.5, bpmToHz(mySettings.mod_lfo_bpm) * beatMultiLookup[mySettings.mod_lfo_freq_bpm_multi] , waveIntsLook[mySettings.mod_lfo_waveform], mySettings.mod_lfo_cycles);
                
          if(DEBUG_MIDI_INPUT) Serial.printf("Mod LFO WaveForm set to: %d\n",value);
        }
        
   break;
    
   case 91: // Mod Lfo PulseWide
     
        loghelperValInt=map(int(value),0,127,10,1000);
        loghelperValFloat=float(loghelperValInt)/1000.0;
        
        mySettings.mod_lfo_pulseWidth = loghelperValFloat;
        MOD_LFO.pulseWidth(mySettings.mod_lfo_pulseWidth);
        if(DEBUG_MIDI_INPUT){
          Serial.printf("Mod LFO  Pulsewide changing now to %f| and back to lin: %d\n",loghelperValFloat, map( int(mySettings.mod_lfo_pulseWidth*1000)+5 ,10 , 1000, 0, 127) );
        } 
   break;
   
   case 92: // Mod Lfo Bpm
    
        if(mySettings.MasterClockToLfos || mySettings.MasterClockToMODLfo){
          if(DEBUG_MIDI_INPUT) Serial.printf("Mod Lfo BPM on CLOCK SYNC! BREAK\n"); 
          break;
        }
        
        loghelperValInt=map(int(value),0,127,0,360);
        mySettings.mod_lfo_bpm = float(loghelperValInt);
        
        if(DEBUG_MIDI_INPUT) Serial.printf("Mod LFO BPM set to: %f| und zuruck: %d\n", mySettings.mod_lfo_bpm, map(mySettings.mod_lfo_bpm + 2,0,360,0,127));    
    
   break;


   case 93: // Mod Lfo Bpm multip
     
        if(value >20){
          if(DEBUG_MIDI_INPUT) Serial.printf("Mod LFO BPM Multiplier to high value: %d\n",value);
        }
        else
        {
           mySettings.mod_lfo_freq_bpm_multi = value;
        }
        
        if(DEBUG_MIDI_INPUT) Serial.printf("Mod LFO BPM Multip set to: %s| midi val %d\n",beatMultiNamesLookup[mySettings.mod_lfo_freq_bpm_multi], mySettings.mod_lfo_freq_bpm_multi ); 
   break;

    case 94: // Mod LFO Phase
    
        loghelperValInt=map(int(value),0,127,0,360);
        mySettings.mod_lfo_phase = loghelperValInt;
        MOD_LFO.phase(mySettings.mod_lfo_phase);
        
        if(DEBUG_MIDI_INPUT){
          Serial.printf("Mod Lfo Phase changing now to %d | und zurueck: %d\n", mySettings.mod_lfo_phase, map(mySettings.mod_lfo_phase + 1,0,360,0,127));
        }
            
    break;
    
     case 95: // Mod lfo sync to attack
        
        if(value >1){
          if(DEBUG_MIDI_INPUT) Serial.printf("Mod LFO Sync To Attack to high value: %d\n",value);
        }
        else
        {
          if(value == 0) mySettings.mod_lfo_attack_sync = false;
          if(value == 1) mySettings.mod_lfo_attack_sync = true;
          
          if(DEBUG_MIDI_INPUT) Serial.printf("Mod LFO Sync to Attack is set to: %d\n",mySettings.mod_lfo_attack_sync);
        }
     break;

     case 96: // Attack envelop to ModLFO Frequency
         
         loghelperValInt=map(int(value),0,127, 0,1000);
         loghelperValFloat=float(loghelperValInt)/100.0;
         mySettings.envelopeFollowerAttackToMODLFOFrequenzy = loghelperValFloat;

         if(DEBUG_MIDI_INPUT) Serial.printf("Mod LFO Evelope Attack to Freq. is set to: %f | und zuruck %d\n",mySettings.envelopeFollowerAttackToMODLFOFrequenzy, map( int(mySettings.envelopeFollowerAttackToMODLFOFrequenzy*100)+5 , 0 , 1000, 0, 127) );
     break;

      case 97: // Mod Lfo Attack envelope to Depth
     
         loghelperValInt=map(int(value),0,127,-1000,1000);
         loghelperValFloat=float(loghelperValInt)/1000.0;
            
         mySettings.mod_lfo_follow_envelope_attack = loghelperValFloat;
      
         //if(value == 63 || value == 64) Filter_LFO_depth.amplitude(0.0);// Set the lfo to zero because we become no clear zero with floats
         
         if(DEBUG_MIDI_INPUT) Serial.printf("Mod LFO Attack envelope to Depth is set to: %f | und zuruck %d\n",mySettings.mod_lfo_follow_envelope_attack, map( int(mySettings.mod_lfo_follow_envelope_attack*1000)+5 ,-1000 , 1000, 0, 127) );

         
     break;


     // gittare filteradsr static velocity
     case 98: 
         loghelperValInt=map(int(value),0,127, 0,1000);
         loghelperValFloat=float(loghelperValInt)/1000.0;

         mySettings.gitarUseFilterADSRstaticVelocity = loghelperValFloat;

         if(DEBUG_MIDI_INPUT) Serial.printf("Filter ADSR use Static velocity: %f | and back %d\n", mySettings.gitarUseFilterADSRstaticVelocity,map( int(mySettings.gitarUseFilterADSRstaticVelocity*1000)+5 ,0 , 1000, 0, 127));

     break;


     // Mod LFO Target Freq OSC1 
     case 99: 
          loghelperValInt=map(int(value),0,127, 0,1000);
          loghelperValFloat=float(loghelperValInt)/1000.0;

          mySettings.mod_lfo_T1_OSC1_FREQ_intensety = loghelperValFloat;

          if(mySettings.mod_lfo_T1_T4_FREQ_intensety_sync){
            mySettings.mod_lfo_T2_OSC2_FREQ_intensety = mySettings.mod_lfo_T1_OSC1_FREQ_intensety;
            mySettings.mod_lfo_T3_SUB1_FREQ_intensety = mySettings.mod_lfo_T1_OSC1_FREQ_intensety;
            mySettings.mod_lfo_T4_SUB2_FREQ_intensety = mySettings.mod_lfo_T1_OSC1_FREQ_intensety;
          } 

          if(DEBUG_MIDI_INPUT) Serial.printf("Mod LFO Target Freq OSC1: %f | and back %d\n", mySettings.mod_lfo_T1_OSC1_FREQ_intensety,map( int(mySettings.mod_lfo_T1_OSC1_FREQ_intensety*1000)+5 ,0 , 1000, 0, 127));

     break;


     // Mod LFO Target Freq OSC2
     case 100: 
          loghelperValInt=map(int(value),0,127, 0,1000);
          loghelperValFloat=float(loghelperValInt)/1000.0;

          mySettings.mod_lfo_T2_OSC2_FREQ_intensety = loghelperValFloat;
       
          if(mySettings.mod_lfo_T1_T4_FREQ_intensety_sync){
            mySettings.mod_lfo_T1_OSC1_FREQ_intensety = mySettings.mod_lfo_T2_OSC2_FREQ_intensety;
            mySettings.mod_lfo_T3_SUB1_FREQ_intensety = mySettings.mod_lfo_T2_OSC2_FREQ_intensety;
            mySettings.mod_lfo_T4_SUB2_FREQ_intensety = abs(mySettings.mod_lfo_T2_OSC2_FREQ_intensety);
          } 


          if(DEBUG_MIDI_INPUT) Serial.printf("Mod LFO Target Freq OSC2: %f | and back %d\n", mySettings.mod_lfo_T2_OSC2_FREQ_intensety,map( int(mySettings.mod_lfo_T2_OSC2_FREQ_intensety*1000)+5 ,0 , 1000, 0, 127));

     break;


     // Mod LFO Target Freq SUB OSC1 
     case 101: 
          loghelperValInt=map(int(value),0,127, 0,1000);
          loghelperValFloat=float(loghelperValInt)/1000.0;

          mySettings.mod_lfo_T3_SUB1_FREQ_intensety = loghelperValFloat;

          if(mySettings.mod_lfo_T1_T4_FREQ_intensety_sync){
            mySettings.mod_lfo_T1_OSC1_FREQ_intensety = mySettings.mod_lfo_T3_SUB1_FREQ_intensety;
            mySettings.mod_lfo_T2_OSC2_FREQ_intensety = mySettings.mod_lfo_T3_SUB1_FREQ_intensety;
            mySettings.mod_lfo_T4_SUB2_FREQ_intensety = abs(mySettings.mod_lfo_T3_SUB1_FREQ_intensety);
          }
           
          if(DEBUG_MIDI_INPUT) Serial.printf("Mod LFO Target Freq SUB OSC1: %f | and back %d\n", mySettings.mod_lfo_T3_SUB1_FREQ_intensety,map( int(mySettings.mod_lfo_T3_SUB1_FREQ_intensety*1000)+5 ,0 , 1000, 0, 127));

     break;


     // Mod LFO Target Freq Sub OSC2 
     case 102: 
          loghelperValInt=map(int(value),0,127, 0,1000);
          loghelperValFloat=float(loghelperValInt)/1000.0;

          mySettings.mod_lfo_T4_SUB2_FREQ_intensety = loghelperValFloat;

          if(mySettings.mod_lfo_T1_T4_FREQ_intensety_sync){
            mySettings.mod_lfo_T1_OSC1_FREQ_intensety = mySettings.mod_lfo_T4_SUB2_FREQ_intensety;
            mySettings.mod_lfo_T2_OSC2_FREQ_intensety = mySettings.mod_lfo_T4_SUB2_FREQ_intensety;
            mySettings.mod_lfo_T3_SUB1_FREQ_intensety = abs(mySettings.mod_lfo_T4_SUB2_FREQ_intensety);
          } 

          if(DEBUG_MIDI_INPUT) Serial.printf("Mod LFO Target Freq SUB OSC2: %f | and back %d\n", mySettings.mod_lfo_T4_SUB2_FREQ_intensety,map( int(mySettings.mod_lfo_T4_SUB2_FREQ_intensety*1000)+5 ,0 , 1000, 0, 127));

     break;     


     case 103: // MOD lfo Intensity T1 - T4 Sync
          if(value >1){
            if(DEBUG_MIDI_INPUT) Serial.printf("MOD lfo Intensity T1 - T4 Sync to high value: %d\n",value);
          }
          else
          {
            if(value == 0) mySettings.mod_lfo_T1_T4_FREQ_intensety_sync = false;
            if(value == 1) mySettings.mod_lfo_T1_T4_FREQ_intensety_sync = true;
            
            if(DEBUG_MIDI_INPUT) Serial.printf("MOD lfo Intensity T1 - T4 Sync is set to: %d\n",mySettings.mod_lfo_T1_T4_FREQ_intensety_sync);
          }
          
     break; 


     case 104: // MOD lfo Intensity T5 - T8 Sync
          if(value >1){
            if(DEBUG_MIDI_INPUT) Serial.printf("MOD lfo Intensity T5 - T8 Sync to high value: %d\n",value);
          }
          else
          {
            if(value == 0) mySettings.mod_lfo_T1_T4_PULSE_intensety_sync = false;
            if(value == 1) mySettings.mod_lfo_T1_T4_PULSE_intensety_sync = true;
            
            if(DEBUG_MIDI_INPUT) Serial.printf("MOD lfo Intensity T5 - T8 Sync is set to: %d\n",mySettings.mod_lfo_T1_T4_PULSE_intensety_sync);
          }
          
     break; 

     case 105: // Mod Lfo Depth T1 OSC1
          loghelperValInt=map(int(value),0,127, -1000,1000);
          loghelperValFloat=float(loghelperValInt)/1000.0;

          mySettings.mod_lfo_T1_OSC1_PULSE_intensety = loghelperValFloat;
          
          if(mySettings.mod_lfo_T1_T4_PULSE_intensety_sync){
            mySettings.mod_lfo_T2_OSC2_PULSE_intensety = mySettings.mod_lfo_T1_OSC1_PULSE_intensety;
            mySettings.mod_lfo_T3_SUB1_PULSE_intensety = mySettings.mod_lfo_T1_OSC1_PULSE_intensety;
            mySettings.mod_lfo_T4_SUB2_PULSE_intensety = mySettings.mod_lfo_T1_OSC1_PULSE_intensety;
          }


          if(DEBUG_MIDI_INPUT) Serial.printf("Mod LFO Target Pulse OSC1: %f | and back %d\n", mySettings.mod_lfo_T1_OSC1_PULSE_intensety, map( int(mySettings.mod_lfo_T1_OSC1_PULSE_intensety*1000)+5 ,-1000 , 1000, 0, 127));
     
     break;

     case 106: // Mod Lfo Depth T2 OSC2
          loghelperValInt=map(int(value),0,127, -1000,1000);
          loghelperValFloat=float(loghelperValInt)/1000.0;

          mySettings.mod_lfo_T2_OSC2_PULSE_intensety = loghelperValFloat;
          
          if(mySettings.mod_lfo_T1_T4_PULSE_intensety_sync){
            mySettings.mod_lfo_T1_OSC1_PULSE_intensety = mySettings.mod_lfo_T2_OSC2_PULSE_intensety;
            mySettings.mod_lfo_T3_SUB1_PULSE_intensety = mySettings.mod_lfo_T2_OSC2_PULSE_intensety;
            mySettings.mod_lfo_T4_SUB2_PULSE_intensety = mySettings.mod_lfo_T2_OSC2_PULSE_intensety;
          }

          if(DEBUG_MIDI_INPUT) Serial.printf("Mod LFO Target Pulse OSC2: %f | and back %d\n", mySettings.mod_lfo_T2_OSC2_PULSE_intensety, map( int(mySettings.mod_lfo_T2_OSC2_PULSE_intensety*1000)+5 ,-1000 , 1000, 0, 127));
     
     break;

     case 107: // Mod Lfo Depth T3 Sub OSC1
          loghelperValInt=map(int(value),0,127, -1000,1000);
          loghelperValFloat=float(loghelperValInt)/1000.0;

          mySettings.mod_lfo_T3_SUB1_PULSE_intensety = loghelperValFloat;
          
          if(mySettings.mod_lfo_T1_T4_PULSE_intensety_sync){
            mySettings.mod_lfo_T1_OSC1_PULSE_intensety = mySettings.mod_lfo_T3_SUB1_PULSE_intensety;
            mySettings.mod_lfo_T2_OSC2_PULSE_intensety = mySettings.mod_lfo_T3_SUB1_PULSE_intensety;
            mySettings.mod_lfo_T4_SUB2_PULSE_intensety = abs(mySettings.mod_lfo_T3_SUB1_PULSE_intensety);
          }

          if(DEBUG_MIDI_INPUT) Serial.printf("Mod LFO Target Pulse OSC2: %f | and back %d\n", mySettings.mod_lfo_T3_SUB1_PULSE_intensety, map( int(mySettings.mod_lfo_T3_SUB1_PULSE_intensety*1000)+5 ,-1000 , 1000, 0, 127));
     
     break;
     
     case 108: // Mod Lfo Depth T4 Sub OSC2
          loghelperValInt=map(int(value),0,127, -1000,1000);
          loghelperValFloat=float(loghelperValInt)/1000.0;

          mySettings.mod_lfo_T4_SUB2_PULSE_intensety = loghelperValFloat;
          
          if(mySettings.mod_lfo_T1_T4_PULSE_intensety_sync){
            mySettings.mod_lfo_T1_OSC1_PULSE_intensety = mySettings.mod_lfo_T4_SUB2_PULSE_intensety;
            mySettings.mod_lfo_T2_OSC2_PULSE_intensety = mySettings.mod_lfo_T4_SUB2_PULSE_intensety;
            mySettings.mod_lfo_T3_SUB1_PULSE_intensety = mySettings.mod_lfo_T4_SUB2_PULSE_intensety;
          }

          if(DEBUG_MIDI_INPUT) Serial.printf("Mod LFO Target Pulse OSC2: %f | and back %d\n", mySettings.mod_lfo_T4_SUB2_PULSE_intensety, map( int(mySettings.mod_lfo_T4_SUB2_PULSE_intensety*1000)+5 ,-1000 , 1000, 0, 127));
     
     break;


   // ###########################################  Vol LFO ####################################################
    
    // Vol LFO
    case 109: // Vol Lfo WaveForm
    
        if(value >11){ // SUB_OSC 2 
          if(DEBUG_MIDI_INPUT) Serial.printf("Vol LFO WaveForm to high value: %d\n",value);
        }
        else
        {
          mySettings.vol_lfo_waveform = value;

          if(mySettings.vol_lfo_waveform > 5) selectABTWaveTableVolumenLFO( bpmToHz(mySettings.vol_lfo_bpm), mySettings.vol_lfo_waveform -6);
          if(mySettings.vol_lfo_waveform < 6) VOLUMEN_LFO.begin(0.5, bpmToHz(mySettings.vol_lfo_bpm) * beatMultiLookup[mySettings.vol_lfo_freq_bpm_multi] , waveIntsLook[mySettings.vol_lfo_waveform], mySettings.vol_lfo_cycles);     
          
          if(DEBUG_MIDI_INPUT) Serial.printf("Vol LFO WaveForm set to: %d\n",value);
        }
        
   break;
    
   case 110: // Vol Lfo PulseWide
     
        loghelperValInt=map(int(value),0,127,10,1000);
        loghelperValFloat=float(loghelperValInt)/1000.0;
        
        mySettings.vol_lfo_pulseWidth = loghelperValFloat;
        VOLUMEN_LFO.pulseWidth(mySettings.vol_lfo_pulseWidth);
        if(DEBUG_MIDI_INPUT){
          Serial.printf("Vol LFO  Pulsewide changing now to %f| and back to lin: %d\n",loghelperValFloat, map( int(mySettings.vol_lfo_pulseWidth*1000)+5 ,10 , 1000, 0, 127) );
        } 
   break;
   
   case 111: // Vol Lfo Bpm
    
        if(mySettings.MasterClockToLfos || mySettings.MasterClockToVOLLfo){
          if(DEBUG_MIDI_INPUT) Serial.printf("Vol BPM on CLOCK SYNC! BREAK\n"); 
          break;
        }
        
        loghelperValInt=map(int(value),0,127,0,360);
        mySettings.vol_lfo_bpm = float(loghelperValInt);
        
        if(DEBUG_MIDI_INPUT) Serial.printf("Vol LFO BPM set to: %f| und zuruck: %d\n", mySettings.vol_lfo_bpm, map(mySettings.vol_lfo_bpm + 2,0,360,0,127));    
    
   break;


   case 112: // Vol Lfo Bpm multip
     
        if(value >20){
          if(DEBUG_MIDI_INPUT) Serial.printf("Vol LFO BPM Multiplier to high value: %d\n",value);
        }
        else
        {
           mySettings.vol_lfo_freq_bpm_multi = value;
        }
        
        if(DEBUG_MIDI_INPUT) Serial.printf("Vol LFO BPM Multip set to: %s| midi val %d\n",beatMultiNamesLookup[mySettings.vol_lfo_freq_bpm_multi], mySettings.vol_lfo_freq_bpm_multi ); 
   break;

    case 113: // Vol LFO Phase
    
        loghelperValInt=map(int(value),0,127,0,360);
        mySettings.vol_lfo_phase = loghelperValInt;
        VOLUMEN_LFO.phase(mySettings.vol_lfo_phase);
        
        if(DEBUG_MIDI_INPUT){
          Serial.printf("Vol Lfo Phase changing now to %d | und zurueck: %d\n", mySettings.vol_lfo_phase, map(mySettings.vol_lfo_phase + 1,0,360,0,127));
        }
            
    break;
    
     case 114: // Vol lfo sync to attack
        
        if(value >1){
          if(DEBUG_MIDI_INPUT) Serial.printf("Vol LFO Sync To Attack to high value: %d\n",value);
        }
        else
        {
          if(value == 0) mySettings.vol_lfo_attack_sync = false;
          if(value == 1) mySettings.vol_lfo_attack_sync = true;
          
          if(DEBUG_MIDI_INPUT) Serial.printf("Vol LFO Sync to Attack is set to: %d\n",mySettings.vol_lfo_attack_sync);
        }
     break;

     case 115: // Attack envelop to VolLFO Frequency
         
         loghelperValInt=map(int(value),0,127, 0,1000);
         loghelperValFloat=float(loghelperValInt)/100.0;
         mySettings.envelopeFollowerAttackToVOLLFOFrequenzy = loghelperValFloat;

         if(DEBUG_MIDI_INPUT) Serial.printf("Vol LFO Evelope Attack to Freq. is set to: %f | und zuruck %d\n",mySettings.envelopeFollowerAttackToVOLLFOFrequenzy, map( int(mySettings.envelopeFollowerAttackToVOLLFOFrequenzy*100)+5 , 0 , 1000, 0, 127) );
     break;

      case 116: // Vol Lfo Attack envelope to Depth
     
         loghelperValInt=map(int(value),0,127,-1000,1000);
         loghelperValFloat=float(loghelperValInt)/1000.0;
            
         mySettings.vol_lfo_follow_envelope_attack = loghelperValFloat;
      
         //if(value == 63 || value == 64) Filter_LFO_depth.amplitude(0.0);// Set the lfo to zero because we become no clear zero with floats
         
         if(DEBUG_MIDI_INPUT) Serial.printf("Vol LFO Attack envelope to Depth is set to: %f | und zuruck %d\n",mySettings.vol_lfo_follow_envelope_attack, map( int(mySettings.vol_lfo_follow_envelope_attack*1000)+5 ,-1000 , 1000, 0, 127) );

         
     break;

     // ##################################################

     case 117: // Vol lfo Intensity T1 - T1 Sync
          if(value >1){
            if(DEBUG_MIDI_INPUT) Serial.printf("Vol lfo Intensity T1 - T4 Sync to high value: %d\n",value);
          }
          else
          {
            if(value == 0) mySettings.vol_lfo_T1_T4_intensety_sync = false;
            if(value == 1) mySettings.vol_lfo_T1_T4_intensety_sync = true;
            
            if(DEBUG_MIDI_INPUT) Serial.printf("Vol lfo Intensity T1 - T4 Sync is set to: %d\n",mySettings.vol_lfo_T1_T4_intensety_sync);
          }
          
     break; 

     case 118: // Vol Lfo Depth T1 OSC1
          loghelperValInt=map(int(value),0,127, -1000,1000);
          loghelperValFloat=float(loghelperValInt)/1000.0;

          mySettings.vol_lfo_T1_OSC1_intensety = loghelperValFloat;
          
    
          if(mySettings.vol_lfo_T1_T4_intensety_sync){
            mySettings.vol_lfo_T2_OSC2_intensety = mySettings.vol_lfo_T1_OSC1_intensety;
            mySettings.vol_lfo_T3_SUBS_intensety = mySettings.vol_lfo_T1_OSC1_intensety;
            mySettings.vol_lfo_T4_NOISE_intensety = abs(mySettings.vol_lfo_T1_OSC1_intensety);
          }

          if(DEBUG_MIDI_INPUT) Serial.printf("Vol LFO Target OSC1: %f | and back %d\n", mySettings.vol_lfo_T1_OSC1_intensety, map( int(mySettings.vol_lfo_T1_OSC1_intensety*1000)+5 ,-1000 , 1000, 0, 127));
     
     break;

     case 119: // Vol Lfo Depth T2 OSC2
          loghelperValInt=map(int(value),0,127, -1000,1000);
          loghelperValFloat=float(loghelperValInt)/1000.0;

          mySettings.vol_lfo_T2_OSC2_intensety = loghelperValFloat;
              
          if(mySettings.vol_lfo_T1_T4_intensety_sync){
            mySettings.vol_lfo_T1_OSC1_intensety = mySettings.vol_lfo_T2_OSC2_intensety;
            mySettings.vol_lfo_T3_SUBS_intensety = mySettings.vol_lfo_T2_OSC2_intensety;
            mySettings.vol_lfo_T4_NOISE_intensety = abs(mySettings.vol_lfo_T2_OSC2_intensety);
          }

          if(DEBUG_MIDI_INPUT) Serial.printf("Vol LFO Target OSC2: %f | and back %d\n", mySettings.vol_lfo_T2_OSC2_intensety, map( int(mySettings.vol_lfo_T2_OSC2_intensety*1000)+5 ,-1000 , 1000, 0, 127));
     
     break;

     case 120: // Vol Lfo Depth T3 Sub OSCc
          loghelperValInt=map(int(value),0,127, -1000,1000);
          loghelperValFloat=float(loghelperValInt)/1000.0;

          mySettings.vol_lfo_T3_SUBS_intensety = loghelperValFloat;
          
          if(mySettings.vol_lfo_T1_T4_intensety_sync){
            mySettings.vol_lfo_T1_OSC1_intensety = mySettings.vol_lfo_T3_SUBS_intensety;
            mySettings.vol_lfo_T2_OSC2_intensety = mySettings.vol_lfo_T3_SUBS_intensety;
            mySettings.vol_lfo_T4_NOISE_intensety = abs(mySettings.vol_lfo_T3_SUBS_intensety);
          }

          if(DEBUG_MIDI_INPUT) Serial.printf("Vol LFO Target SUBS: %f | and back %d\n", mySettings.vol_lfo_T3_SUBS_intensety, map( int(mySettings.vol_lfo_T3_SUBS_intensety*1000)+5 ,-1000 , 1000, 0, 127));
     
     break;
     
     case 121: // Vol Lfo Depth T4 Sub Noise
          loghelperValInt=map(int(value),0,127, 0,1000);
          loghelperValFloat=float(loghelperValInt)/1000.0;

          mySettings.vol_lfo_T4_NOISE_intensety = loghelperValFloat;
          if(mySettings.vol_lfo_T1_T4_intensety_sync){
            mySettings.vol_lfo_T1_OSC1_intensety = mySettings.vol_lfo_T4_NOISE_intensety;
            mySettings.vol_lfo_T2_OSC2_intensety = mySettings.vol_lfo_T4_NOISE_intensety;
            mySettings.vol_lfo_T3_SUBS_intensety = mySettings.vol_lfo_T4_NOISE_intensety;
          }         

          if(DEBUG_MIDI_INPUT) Serial.printf("Vol LFO Target Noise: %f | and back %d\n", mySettings.vol_lfo_T4_NOISE_intensety, map( int(mySettings.vol_lfo_T4_NOISE_intensety*1000)+5 ,0, 1000, 0, 127));
     
     break;

     case 122: // Midi Pitchband range
     
        if(value >23){ // SUB_OSC 2 
          if(DEBUG_MIDI_INPUT) Serial.printf("Vol LFO WaveForm to high value: %d\n",value);
        }
        else
        {
          mySettings.midi_pichbandrange = value + 1;

                
          if(DEBUG_MIDI_INPUT) Serial.printf("Midi Pitch Band Range set to: %d\n", mySettings.midi_pichbandrange  );
        }    

     break;


     case 123: // Peak Auto Correction
          if(value >1){
            if(DEBUG_MIDI_INPUT) Serial.printf("Peak Auto correction to high value: %d\n",value);
          }
          else
          {
            if(value == 0) mySettings.autoPeakCorrection = false;
            if(value == 1) mySettings.autoPeakCorrection = true;
            
            if(DEBUG_MIDI_INPUT) Serial.printf("Peak Auto correction is set to: %d\n",mySettings.autoPeakCorrection);
          }
     break;


     case 124: // Master Clock To All LFOs
          if(value >1){
            if(DEBUG_MIDI_INPUT) Serial.printf("Master Clock To All LFOs to high value: %d\n",value);
          }
          else
          {
            if(value == 0) mySettings.MasterClockToLfos = false;
            if(value == 1) mySettings.MasterClockToLfos = true;
            
            if(DEBUG_MIDI_INPUT) Serial.printf("Master Clock To All LFOs is set to: %d\n",mySettings.MasterClockToLfos);
          }
     break;


     case 125: // Master Clock To MOD LFO
          if(value >1){
            if(DEBUG_MIDI_INPUT) Serial.printf("Master Clock To MOD LFO to high value: %d\n",value);
          }
          else
          {
            if(value == 0) mySettings.MasterClockToMODLfo = false;
            if(value == 1) mySettings.MasterClockToMODLfo = true;
            
            if(DEBUG_MIDI_INPUT) Serial.printf("Master Clock To MOD LFO is set to: %d\n",mySettings.MasterClockToMODLfo);
          }
     break;


     case 126: // Master Clock To Vol LFO
          if(value >1){
            if(DEBUG_MIDI_INPUT) Serial.printf("Master Clock To Vol LFO to high value: %d\n",value);
          }
          else
          {
            if(value == 0) mySettings.MasterClockToVOLLfo = false;
            if(value == 1) mySettings.MasterClockToVOLLfo = true;
            
            if(DEBUG_MIDI_INPUT) Serial.printf("Master Clock To VOL LFO is set to: %d\n",mySettings.MasterClockToVOLLfo);
          }
     break;

     case 127: // Master Clock To Freq LFO
          if(value >1){
            if(DEBUG_MIDI_INPUT) Serial.printf("Master Clock To Freq LFO to high value: %d\n",value);
          }
          else
          {
            if(value == 0) mySettings.MasterclockToFreqLFO = false;
            if(value == 1) mySettings.MasterclockToFreqLFO = true;
            
            if(DEBUG_MIDI_INPUT) Serial.printf("Master Clock To FREQ. LFO is set to: %d\n",mySettings.MasterclockToFreqLFO);
          }
     break;


        
    // ###############################  DEFAULT #################################
    default:
       if(DEBUG_MIDI_INPUT){
        Serial.printf("CC %d currently not implemented\n",control );
       }       
   }

   // ################################ END of CC input #########################
}

bool channeltest(byte channel){
  if(channel == mySettings.midiChannel) return true;
  else return false;
}



// ################################## compute adsr and midi notes to hz
void StopAnyPlayingNote(){
        myMidiNote.adsrState = 5;
        myFilterNote.adsrState = 6;
        StopSequencer();  
}

// adsr states 0=finished, 1 attack, 2 hold, 3 decay 4 sustain,  5 release 6 in suistain 7 in release
void StartStopMidiNote( bool onOff, unsigned long startTime, int velocity, int note, int pitch){
    
    float myNoteinHz = tune_frequencies2_PGM[note];
    
    if(!onOff){
      if(myNoteinHz == myMidiNote.midiNote){
        myMidiNote.adsrState = 5;  // // mono synth trick, release only if not an other note is played
        myFilterNote.adsrState = 6;
        // SequencerTest
        StopSequencer();
      }

    }
    
    if(onOff){
      // LFo Sync to key Attack
      //// test !!! kann knacken.... geht wohl doch nach test!
      //Filter_LFO.phase(mySettings.filter_lfo_phase);

      float filterlfobpm = mySettings.filter_lfo_bpm;
      float volumenlfobpm = mySettings.vol_lfo_bpm;
      float modlfobpm = mySettings.mod_lfo_bpm;
        if(mySettings.MasterClockToLfos){// alle gleichzeitig syncronisieren
           filterlfobpm = mySystemBpm;
           volumenlfobpm = mySystemBpm;
           modlfobpm = mySystemBpm;
        }
        if(mySettings.MasterClockToMODLfo){//  
          modlfobpm = mySystemBpm;    
        }
        if(mySettings.MasterClockToVOLLfo){
          volumenlfobpm = mySystemBpm;
        }
        if(mySettings.MasterclockToFreqLFO){
           filterlfobpm = mySystemBpm;    
        }
      
      if (mySettings.filter_lfo_attack_sync){
        if(mySettings.filter_lfo_waveform > 5) selectABTWaveTableFilterLFO( bpmToHz(filterlfobpm), mySettings.filter_lfo_waveform -6);
        if(mySettings.filter_lfo_waveform < 6) Filter_LFO.begin(1.0, bpmToHz(filterlfobpm) * beatMultiLookup[mySettings.filter_lfo_freq_bpm_multi] , waveIntsLook[mySettings.filter_lfo_waveform], mySettings.filter_lfo_cycles);
      }
      if (mySettings.vol_lfo_attack_sync){
        if(mySettings.vol_lfo_waveform > 5) selectABTWaveTableVolumenLFO( bpmToHz(volumenlfobpm), mySettings.vol_lfo_waveform -6);
        if(mySettings.vol_lfo_waveform < 6) VOLUMEN_LFO.begin(0.5, bpmToHz(volumenlfobpm) * beatMultiLookup[mySettings.vol_lfo_freq_bpm_multi] , waveIntsLook[mySettings.vol_lfo_waveform], mySettings.vol_lfo_cycles);
      }
      if (mySettings.mod_lfo_attack_sync){
        if(mySettings.mod_lfo_waveform > 5) selectABTWaveTableModLFO( bpmToHz(modlfobpm), mySettings.mod_lfo_waveform -6);
        if(mySettings.mod_lfo_waveform < 6) MOD_LFO.begin(0.5, bpmToHz(modlfobpm) * beatMultiLookup[mySettings.mod_lfo_freq_bpm_multi] , waveIntsLook[mySettings.mod_lfo_waveform], mySettings.mod_lfo_cycles);
      }

      myMidiNote.adsrState = 1;  // release
      myFilterNote.adsrState = 1;
      myMidiNote.attackTime = startTime;
      myFilterNote.delayTime = startTime;
      myFilterNote.isGuitar = false;
      myMidiNote.midiVelocety = velocity2amplitude[velocity];
      myFilterNote.filterVelocety = velocity2amplitude[velocity];
      
      myMidiNote.midiNote = myNoteinHz;
      myMidiNote.midiPitch = pitch;

      // led blink
      myLedStatus.TRACKING_LED_MILLIS_ON = startTime;
      myLedStatus.TRACKING_LED_MILLIS = 50;
      // taptempo on keyboard
      if(taptempoOnGuitarOrKeyboard) checkTapTempo( micros() );
      // sequencer test;
      StartSequencer();
    }

}


void updateMidiNote(unsigned long milli){
  
  // nichts zu tun
  if(myMidiNote.adsrState == 0 ){
    if(DEB_MIDI_NOTE_ENVELOPE)
      //Serial.printf("%d ms, adsrState is %d \n",milli,myMidiNote.adsrState );
    return;
  }

  // hier die noten berrechnung und oscs trimmen inclusive aller parameter
  // solange state release durch ist und det state null erreicht ist
  
  // attack hold decay sustain release
  //case note on attack
  if(myMidiNote.adsrState == 1 ){
    if(DEB_MIDI_NOTE_ENVELOPE) Serial.printf("Update Midi: Note On ATTACK next state is hold \\\\ \n");
    myMidiNote.adsrState = 2;
    VCAenvelopeGenerator.amplitude(0.0,1);// eperimental
    VCAenvelopeGenerator.amplitude(myMidiNote.midiVelocety, mySettings.midi_attack);
    //filter1envelopeGenerator.amplitude(myMidiNote.midiVelocety, mySettings.midi_attack);
  }
  // reach state hold
  if(myMidiNote.adsrState == 2 && milli > myMidiNote.attackTime + mySettings.midi_attack ){
    if(DEB_MIDI_NOTE_ENVELOPE) Serial.printf("Update Midi: Note reach stage HOLD, next state is decay >>>>>>>>>>>>>>>>\n");
    myMidiNote.adsrState = 3;
    myMidiNote.holdTime = milli;
    // tun nichts mit dem envelope
  }
  // reach state decay
  if(myMidiNote.adsrState == 3 && milli > myMidiNote.holdTime + mySettings.midi_hold ){
    if(DEB_MIDI_NOTE_ENVELOPE) Serial.printf("Update Midi: Note reach stage DECAY next state is sustain //\n");
    myMidiNote.adsrState = 4;
    myMidiNote.decayTime = milli;
    VCAenvelopeGenerator.amplitude(myMidiNote.midiVelocety / 100.0 * float(mySettings.midi_sustain), mySettings.midi_decay);
    //filter1envelopeGenerator.amplitude(myMidiNote.midiVelocety / 100.0 * float(mySettings.midi_sustain), mySettings.midi_decay);
  }
  // reach state sustain
  if(myMidiNote.adsrState == 4 && milli > myMidiNote.decayTime + mySettings.midi_decay ){
    if(DEB_MIDI_NOTE_ENVELOPE) Serial.printf("Update Midi: Note reach stage SUSTAIN || \n");
    myMidiNote.adsrState = 6; // verbleiben im sustain bis release aufgerufen wird
    //tun nichts hier
  }

  // start release
  if(myMidiNote.adsrState == 5 ){
    if(DEB_MIDI_NOTE_ENVELOPE) Serial.printf("Update Midi: Note Off , start release // \n");
    myMidiNote.adsrState = 7; // verbleiben im sustain bis release aufgerufen wird
    myMidiNote.releaseTime = milli;
    VCAenvelopeGenerator.amplitude(0.0,  mySettings.midi_release);
    //filter1envelopeGenerator.amplitude(0.0, mySettings.midi_release);
  }  
  // release finished
  if(myMidiNote.adsrState == 7 && milli > myMidiNote.releaseTime +  mySettings.midi_release){
    if(DEB_MIDI_NOTE_ENVELOPE) Serial.printf("Update Midi: Note Off , release finished | \n");
    myMidiNote.adsrState = 0; // verbleiben im sustain bis release aufgerufen wird
  }
   
}




void updateFilterNote(unsigned long milli){
  
  // nichts zu tun
  if(myFilterNote.adsrState == 0 ){
    if(DEB_FILTER_NOTE_ENVELOPE)
      //Serial.printf("%d ms, adsrState is %d \n",milli,myMidiNote.adsrState );
    return;
  }

  // hier die noten berrechnung und oscs trimmen inclusive aller parameter
  // solange state release durch ist und det state null erreicht ist
  
  // attack hold decay sustain release
  //case note on attack
  if(myFilterNote.adsrState == 1 ){
    if(DEB_FILTER_NOTE_ENVELOPE) Serial.printf("Update FIlter ADSR: Note On DELAY next state is attack || filter velocity: %f at time: %d\n", myFilterNote.filterVelocety,milli);
    if(!myFilterNote.isGuitar) myFilterNote.adsrState = 2; 
    //filter1envelopeGenerator.amplitude(0.0, 30); // zu kleine rampe erzeugt pop geräusch 
    //filter1envelopeGenerator.amplitude(0.0, 0.1); // scheinbar habe ich zuvor die einstellungen nicht richtig interpretiert, keine pop gerausche bei richtiger attack einstellung, und eine langer release zeit beinflusst nicht mehr den erneuten attack
  }  
  
  if(myFilterNote.adsrState == 2 && milli > myFilterNote.delayTime + mySettings.filter_delay ){
    if(DEB_FILTER_NOTE_ENVELOPE) Serial.printf("Update FIlter ADSR: Note On ATTACK next state is hold \\\\ \n");
    myFilterNote.adsrState = 3;
    myFilterNote.attackTime = milli;
    filter1envelopeGenerator.amplitude(myFilterNote.filterVelocety, mySettings.filter_attack);
  }
  // reach state hold
  if(myFilterNote.adsrState == 3 && milli > myFilterNote.attackTime + mySettings.filter_attack ){
    if(DEB_FILTER_NOTE_ENVELOPE) Serial.printf("Update Filter ADSR: Note reach stage HOLD, next state is decay >>>>>>>>>>>>>>>>\n");
    myFilterNote.adsrState = 4;
    myFilterNote.holdTime = milli;
    // tun nichts mit dem envelope
  }
  // reach state decay
  if(myFilterNote.adsrState == 4 && milli > myFilterNote.holdTime + mySettings.filter_hold ){
    if(DEB_FILTER_NOTE_ENVELOPE) Serial.printf("Update Filter ADSR: Note reach stage DECAY next state is sustain //\n");
    myFilterNote.adsrState = 5;
    myFilterNote.decayTime = milli;
    filter1envelopeGenerator.amplitude(myFilterNote.filterVelocety / 100.0 * float(mySettings.filter_sustain), mySettings.filter_decay);
  }
  // reach state sustain
  if(myFilterNote.adsrState == 5 && milli > myFilterNote.decayTime + mySettings.filter_decay ){
    if(DEB_FILTER_NOTE_ENVELOPE) Serial.printf("Update Filter ADSR: Note reach stage SUSTAIN || \n");
    myFilterNote.adsrState = 7; // verbleiben im sustain bis release aufgerufen wird
    //tun nichts hier
  }

  // start release
  if(myFilterNote.adsrState == 6 ){
    if(DEB_FILTER_NOTE_ENVELOPE) Serial.printf("Update Filter ADSR: Note Off , start release // \n");
    myFilterNote.adsrState = 8; // verbleiben im sustain bis release aufgerufen wird
    myFilterNote.releaseTime = milli;
    filter1envelopeGenerator.amplitude(0.0, mySettings.filter_release);
  }  
  // release finished
  if(myFilterNote.adsrState == 8 && milli > myFilterNote.releaseTime +  mySettings.filter_release){
    if(DEB_FILTER_NOTE_ENVELOPE) Serial.printf("Update Filter ADSR: Note Off , release finished | \n");
    myFilterNote.adsrState = 0; // verbleiben im sustain bis release aufgerufen wird
  }
   
}


void updateGuitarNote(unsigned long milli){
  
  // nichts zu tun
  if(myGuitarNote.adsrState == 0 ){
    if(DEB_MIDI_NOTE_ENVELOPE)
      //Serial.printf("%d ms, adsrState is %d \n",milli,myGuitarNote.adsrState );
    return;
  }

  // hier die noten berrechnung und oscs trimmen inclusive aller parameter
  // solange state release durch ist und det state null erreicht ist
  
  // attack hold decay sustain release
  //case note on attack
  if(myGuitarNote.adsrState == 1){
    //myGuitarNote.adsrState = 2;
    VCAenvelopeGenerator.amplitude(myGuitarNote.midiVelocety, mySettings.midi_attack);
    if(DEB_MIDIGUITAR_NOTE_ENVELOPE) Serial.printf("Update MidiGuitar: Note On ATTACK next state is hold \\\\ state: %f | attacktime: %d\n",myGuitarNote.midiVelocety, myGuitarNote.attackTime);
    //filter1envelopeGenerator.amplitude(myGuitarNote.midiVelocety, mySettings.midi_attack);
  }
  // reach state hold
  if(myGuitarNote.adsrState == 2 && milli > myGuitarNote.attackTime + mySettings.midi_attack ){
    myGuitarNote.adsrState = 3;
    myGuitarNote.holdTime = milli;
    if(DEB_MIDIGUITAR_NOTE_ENVELOPE) Serial.printf("Update MidiGuitar: Note reach stage HOLD, next state is decay >>>>>>>>>>>>>>>> state: %d, milli: %d, die zeit: %d\n",myGuitarNote.adsrState,milli,myGuitarNote.attackTime + mySettings.midi_attack );
    // tun nichts mit dem envelope
  }
  // reach state decay
  if(myGuitarNote.adsrState == 3 && milli > myGuitarNote.holdTime + mySettings.midi_hold ){
    myGuitarNote.adsrState = 4;
    myGuitarNote.decayTime = milli;
    VCAenvelopeGenerator.amplitude(myGuitarNote.midiVelocety / 100.0 * float(mySettings.midi_sustain), mySettings.midi_decay);
    if(DEB_MIDIGUITAR_NOTE_ENVELOPE) Serial.printf("Update MidiGuitar: Note reach stage DECAY next state is sustain // state: %d\n",myGuitarNote.adsrState);
    
  }
  // reach state sustain
  if(myGuitarNote.adsrState == 4 && milli > myGuitarNote.decayTime + mySettings.midi_decay ){
    myGuitarNote.adsrState = 6; // verbleiben im sustain bis release aufgerufen wird
    //tun nichts hier
    if(DEB_MIDIGUITAR_NOTE_ENVELOPE) Serial.printf("Update MidiGuitar: Note reach stage SUSTAIN || state: %d\n",myGuitarNote.adsrState);

  }

  // start release
  if(myGuitarNote.adsrState == 5 ){
    myGuitarNote.adsrState = 7; // verbleiben im sustain bis release aufgerufen wird
    myGuitarNote.releaseTime = milli;
    VCAenvelopeGenerator.amplitude(0.0,  mySettings.midi_release);
    //filter1envelopeGenerator.amplitude(0.0, mySettings.midi_release);
    if(DEB_MIDIGUITAR_NOTE_ENVELOPE) Serial.printf("Update MidiGuitar: Note Off , start release // state: %d\n",myGuitarNote.adsrState);

  }  
  // release finished
  
  if(myGuitarNote.adsrState == 7 /*&& milli > myGuitarNote.releaseTime +  mySettings.midi_release*/){
    myGuitarNote.adsrState = 0; // verbleiben im sustain bis release aufgerufen wird
    if(DEB_MIDIGUITAR_NOTE_ENVELOPE) Serial.printf("Update MidiGuitar: Note Off , release finished | state: %d \n",myGuitarNote.adsrState);

  }
   
}


