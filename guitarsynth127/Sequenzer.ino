

unsigned long oldSequenceMicros = 0;
byte currenteSequenceStep = 0;
bool runsequece = false;

void checkSeqence(unsigned long sequenceMicros){
  if(!runsequece) return; // sequencer off 
  if(sequenceMicros > oldSequenceMicros + sequencerSpeedTime ){
     doSequence(sequenceMicros);
  }
}

void doSequence(unsigned long sequenceMicros){
  oldSequenceMicros = sequenceMicros;// timing the sequenzer

  // setSequencer extra note
  SequencerExtraNote = mySettings.sequenzerNotes[currenteSequenceStep];
  
  currenteSequenceStep++;
  if(currenteSequenceStep > mySettings.sequeceSteps - 1) currenteSequenceStep = 0;
  
  if(DEB_SEQUENZER) Serial.printf("time: %d, timeDelta: %d, At step: %d\n",sequenceMicros/1000, mysystemFreqMicrosTime, currenteSequenceStep);  
}

// ! SEQUENCER IN MIDI StartStopMidiNote() zeile 1974
void StartSequencer(){
  if( mySettings.sequeceSteps == 0) return; // 0 =  sequencer is off 
  currenteSequenceStep = 0; // sequenzer at first position
  sequencerSpeedTime = int(mysystemFreqMicrosTime / beatMultiLookup[mySettings.sequenceTimeSignature]);
  
  oldSequenceMicros = micros() - sequencerSpeedTime;
  runsequece = true;
  if(DEB_SEQUENZER) Serial.println("Starte Sequencer");
}

void StopSequencer(){
  //if( mySettings.sequeceSteps == 0) return; // 0 =  sequencer is off 
  runsequece = false;
  SequencerExtraNote = 0;
  if(DEB_SEQUENZER) Serial.println("Stoppe Sequencer");
}
