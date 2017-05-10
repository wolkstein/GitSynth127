//const char* FunctionBtnLookup[3] =
//{
//    "OFF           ",
//    "Freeze Note   ",
//    "Mute Synth T-U",
//    "Mute Synth T-D"
//};

unsigned long thisolddebugmillies = 0;

void checkFunctionBtnStatus(unsigned long milli){
   
  if (ToggleSwitchFunction.update()) {
    if (ToggleSwitchFunction.fallingEdge()) {
      ToggleSwitchIsToggled = true;
      if(DEB_TOGGLE_SWITCH) Serial.printf("Toggle Switch schaut nach oben, bool ToggleSwitchIsToggled = %d\n",ToggleSwitchIsToggled);
    }
    if (ToggleSwitchFunction.risingEdge()) {
      ToggleSwitchIsToggled = false;
      if(DEB_TOGGLE_SWITCH) Serial.printf("Toggle Switch schaut nach unten, bool ToggleSwitchIsToggled = %d\n",ToggleSwitchIsToggled);
    }

  }
   
  if (FunctioSwitchFunction.update()) {
    if (FunctioSwitchFunction.fallingEdge()) {
      FbtIsPressed = true;
    }
    if (FunctioSwitchFunction.risingEdge()) {
      FbtIsPressed = false;
    }
  }

  switch(mySettings.functionBtnFunction) {
    case 0:
      FrozenNote = false;
    break;
    
    case 1:
      if(FbtIsPressed) FrozenNote = false;
      else FrozenNote = true;
    break;

    case 2:
      FrozenNote = false;
      if(FunctioSwitchFunction.fallingEdge()){
        if(DEB_FUNCTION_BTN) Serial.println("Runter Druck");
        for(int i=0;i<4;i++){
          OscMainMix.gain(i, mySettings.SYNTH_Master_Volume);
        }
      }
      if(FunctioSwitchFunction.risingEdge()){
        if(DEB_FUNCTION_BTN) Serial.println("Hoch Druck");
        for(int i=0;i<4;i++){
          OscMainMix.gain(i, 0.0);
        }
      }
    break;

    case 3:
       FrozenNote = false;
       if(FunctioSwitchFunction.fallingEdge()){
        if(DEB_FUNCTION_BTN) Serial.println("Runter Druck");
        for(int i=0;i<4;i++){
          OscMainMix.gain(i, 0.0);
        }
      }
      if(FunctioSwitchFunction.risingEdge()){
        if(DEB_FUNCTION_BTN) Serial.println("Hoch Druck");
        for(int i=0;i<4;i++){
          OscMainMix.gain(i, mySettings.SYNTH_Master_Volume);
        }
      }       
    break;
     
  }

  if(DEB_FUNCTION_BTN && milli > thisolddebugmillies + 1000){
    Serial.printf("Function BTN use: %s, FrozenNote is: %d\n",FunctionBtnLookup[mySettings.functionBtnFunction], FrozenNote);
    thisolddebugmillies = milli;
  }
}

