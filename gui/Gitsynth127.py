#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import time
import rtmidi
import math
from rtmidi.midiconstants import * ##BANK_SELECT_LSB, BANK_SELECT_MSB, CHANNEL_PRESSURE, CONTROLLER_CHANGE, NOTE_ON, NOTE_OFF, PROGRAM_CHANGE
from rtmidi.midiutil import *

from PyQt4.QtGui import *
from PyQt4 import QtCore, QtGui
from PyQt4.QtCore import QSize, SIGNAL, QEvent

from Gitsynth_ui import Ui_GitSynthV127


global gInIsConnected
global gOutIsConnected
global gMyMidiOutputPort
global gMyMidiInputPort


################## Midi Input Thread ######################
class midiInputThread(QtCore.QThread):
    def __init__(self, main_app):
        QtCore.QThread.__init__(self)
        self.main_app = main_app

    def treadMapValInt(self, x, in_min, in_max, out_min, out_max):
        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min

    def run(self):
        #print("In Thread")
        global gMyMidiOutputPort
        global gMyMidiInputPort
        global gInIsConnected
        global gOutIsConnected

        myMidiNotecheck125 = False
        myMidiNotecheck3 = False

        timer = time.time()
        lastTimecheckedForMidiconnections = timer
        self.main_app.label_Messages.setText("Midi: %s" % gInIsConnected)
        while True:
            time.sleep(0.002)
            GitSynth127OutPort = -1
            GitSynth127InPort = -1
            if time.time() > lastTimecheckedForMidiconnections + 2.0: # all this seconds check for midi conection
                lastTimecheckedForMidiconnections = time.time()
                for i, port in enumerate(midiOut.get_ports()):
                    if "GITSynth127" or "QmidiNet" in str(port):
                        GitSynth127OutPort = i
                        if gMyMidiOutputPort == False:
                            midiOut.open_port(GitSynth127OutPort)
                            gMyMidiOutputPort = True
                            gOutIsConnected = midiOut.get_ports()[GitSynth127OutPort]
                            self.main_app.label_Messages.setText("Midi: %s" % gOutIsConnected)

                if GitSynth127OutPort == -1:
                    gMyMidiOutputPort = False
                    midiOut.close_port()
                    gInIsConnected = "No Midi Device found"
                    self.main_app.label_Messages.setText("Midi: %s" % gInIsConnected)



                for i, port in enumerate(midiIn.get_ports()):
                    if "GITSynth127" or "QmidiNet" in str(port):
                        GitSynth127InPort = i
                        if gMyMidiInputPort == False:
                            midiIn.open_port(GitSynth127InPort)
                            gMyMidiInputPort = True

                if GitSynth127InPort == -1:
                    gMyMidiInputPort = False
                    midiIn.close_port()






            msg = midiIn.get_message()
            if msg:
                message, deltatime = msg
                timer += deltatime
                #print(msg)
                #print("@%0.6f %r" % (timer, message))
                if message[0] == 144:
                    if myMidiNotecheck125 and myMidiNotecheck3:
                        #print("Checked good! Note: %d, Velo: %d" %(message[1],message[2]))
                        # 33 = time signature
                        if message[1] == 33:
                            self.main_app.comboBox_SequencerBeat.setCurrentIndex(message[2] - 1 )
                        # 32 = sequencer steps
                        if message[1] == 32:
                            self.main_app.spinBox_sequencerSteps.setValue(message[2] - 1 )
                        # 0 -31 = offsets
                        if message[1] == 0:
                            self.main_app.spinBox_Seq0.setValue(message[2] - 25 )
                        if message[1] == 1:
                            self.main_app.spinBox_Seq1.setValue(message[2] - 25 )
                        if message[1] == 2:
                            self.main_app.spinBox_Seq2.setValue(message[2] - 25 )
                        if message[1] == 3:
                            self.main_app.spinBox_Seq3.setValue(message[2] - 25 )
                        if message[1] == 4:
                            self.main_app.spinBox_Seq4.setValue(message[2] - 25 )
                        if message[1] == 5:
                            self.main_app.spinBox_Seq5.setValue(message[2] - 25 )
                        if message[1] == 6:
                            self.main_app.spinBox_Seq6.setValue(message[2] - 25 )
                        if message[1] == 7:
                            self.main_app.spinBox_Seq7.setValue(message[2] - 25 )
                        if message[1] == 8:
                            self.main_app.spinBox_Seq8.setValue(message[2] - 25 )
                        if message[1] == 9:
                            self.main_app.spinBox_Seq9.setValue(message[2] - 25 )
                        if message[1] == 10:
                            self.main_app.spinBox_Seq10.setValue(message[2] - 25 )
                        if message[1] == 11:
                            self.main_app.spinBox_Seq11.setValue(message[2] - 25 )
                        if message[1] == 12:
                            self.main_app.spinBox_Seq12.setValue(message[2] - 25 )
                        if message[1] == 13:
                            self.main_app.spinBox_Seq13.setValue(message[2] - 25 )
                        if message[1] == 14:
                            self.main_app.spinBox_Seq14.setValue(message[2] - 25 )
                        if message[1] == 15:
                            self.main_app.spinBox_Seq15.setValue(message[2] - 25 )
                        if message[1] == 16:
                            self.main_app.spinBox_Seq16.setValue(message[2] - 25 )
                        if message[1] == 17:
                            self.main_app.spinBox_Seq17.setValue(message[2] - 25 )
                        if message[1] == 18:
                            self.main_app.spinBox_Seq18.setValue(message[2] - 25 )
                        if message[1] == 19:
                            self.main_app.spinBox_Seq19.setValue(message[2] - 25 )
                        if message[1] == 20:
                            self.main_app.spinBox_Seq20.setValue(message[2] - 25 )
                        if message[1] == 21:
                            self.main_app.spinBox_Seq21.setValue(message[2] - 25 )
                        if message[1] == 22:
                            self.main_app.spinBox_Seq22.setValue(message[2] - 25 )
                        if message[1] == 23:
                            self.main_app.spinBox_Seq23.setValue(message[2] - 25 )
                        if message[1] == 24:
                            self.main_app.spinBox_Seq24.setValue(message[2] - 25 )
                        if message[1] == 25:
                            self.main_app.spinBox_Seq25.setValue(message[2] - 25 )
                        if message[1] == 26:
                            self.main_app.spinBox_Seq26.setValue(message[2] - 25 )
                        if message[1] == 27:
                            self.main_app.spinBox_Seq27.setValue(message[2] - 25 )
                        if message[1] == 28:
                            self.main_app.spinBox_Seq28.setValue(message[2] - 25 )
                        if message[1] == 29:
                            self.main_app.spinBox_Seq29.setValue(message[2] - 25 )
                        if message[1] == 30:
                            self.main_app.spinBox_Seq30.setValue(message[2] - 25 )
                        if message[1] == 31:
                            self.main_app.spinBox_Seq31.setValue(message[2] - 25 )

                        myMidiNotecheck125 = False
                        myMidiNotecheck3 = False
                    else:
                        if message[1] == 125:
                            myMidiNotecheck125 = True
                        if message[1] == 3:
                            myMidiNotecheck3 = True

                ############ CC channel 2 #######################
                if message[0] == 177: # message is cc on channel 1, 176 = ch 1 - 191 = cc on ch 16
                    #print("Channel Two")
                    if message[1] == 1: # lowpass freq
                        self.main_app.dial_highcut_filter.setValue(message[2])

                    if message[1] == 2: # lowpass reso
                        self.main_app.dial_hcq.setValue(message[2])

                    if message[1] == 3: # lowpass freq
                        self.main_app.dial_lowcutfilter.setValue(message[2])

                    if message[1] == 4: # lowpass reso
                        self.main_app.dial_lcq.setValue(message[2])

                    ############# delay ###########
                    if message[1] == 5: # delay dry
                        self.main_app.Delay_Dry_Dail.setValue(message[2])

                    if message[1] == 6: # delay wet
                        self.main_app.Delay_Wet_Dail.setValue(message[2])

                    if message[1] == 7: # delay time ms
                        self.main_app.Delay_TimeMs_SpinBox.setValue(self.treadMapValInt(message[2],0,127,0,1800))

                    if message[1] == 8: # delay time signature
                        self.main_app.comboBox_Delay_Time_Signature.setCurrentIndex(message[2])

                    if message[1] == 9: # Delay Gain
                        self.main_app.Delay_Gain_Dail.setValue(message[2])

                    if message[1] == 10: # Delay Reso
                        self.main_app.Delay_FeedBack_Dail.setValue(message[2])
                    ############## delay end ######


                ############ CC channel 1 ########################
                if message[0] == 176: # message is cc on channel 1, 176 = ch 1 - 191 = cc on ch 16
                    #print(msg)

                    ##### MIXER #############################################
                    if message[1] == 0: # Expression Pedal
                        self.main_app.ExpPressionPedalComboBox.setCurrentIndex(message[2])

                    if message[1] == 1: # Poti
                        self.main_app.PotiComboBox.setCurrentIndex(message[2])

                    if message[1] == 2: # Function Btn
                        self.main_app.functionBtn_Combobox.setCurrentIndex(message[2])

                    if message[1] == 3: # Input Gain
                        self.main_app.InputGain_Dail.setValue(message[2])

                    if message[1] == 4: # Mixer Instrument Input To Synth Frequency - and Envelope - tracking, 0-127
                        self.main_app.verticalSlider_InstrumentToSynth.setValue(message[2])

                    if message[1] == 5: # Mixer Usb Input 1 To Synth Frequency - and Envelope - tracking, 0-127
                        self.main_app.verticalSlider_Usb1ToSynth.setValue(message[2])

                    if message[1] == 6: # Mixer Synth To Output 1, 0-127
                        self.main_app.verticalSlider_SynthToOut1.setValue(message[2])

                    if message[1] == 7: # Mixer Synth To Output 2, 0-127
                        self.main_app.verticalSlider_SynthToOut2.setValue(message[2])

                    if message[1] == 8: # Mixer Synth To Usb Output 1, 0-127
                        self.main_app.verticalSlider_SynthToUsbOut1.setValue(message[2])

                    if message[1] == 9: # Mixer Synth To Usb Output 2, 0-127
                        self.main_app.verticalSlider_SynthToUsbOut2.setValue(message[2])

                    if message[1] == 10: # Mixer Instrument To Usb-OutPut 1, 0-127
                        self.main_app.verticalSlider_InstrumentToUsbOut1.setValue(message[2])

                    if message[1] == 11: # Mixer Instrument To Usb-OutPut 2, 0-127
                        self.main_app.verticalSlider_InstrumentToUsbOut2.setValue(message[2])

                    if message[1] == 12: # Mixer Usb Input 1 To Output 1, 0-127
                        self.main_app.verticalSlider_UsbIn1ToOut1.setValue(message[2])

                    if message[1] == 13: # Mixer Usb Input 1 To Output 2, 0-127
                        self.main_app.verticalSlider_UsbIn2ToOut1.setValue(message[2])

                    if message[1] == 14: # Mixer Usb Input 2 To Output 1, 0-127
                        self.main_app.verticalSlider_UsbIn1ToOut2.setValue(message[2])

                    if message[1] == 15: # Mixer Usb Input 2 To Output 2, 0-127
                        self.main_app.verticalSlider_UsbIn2ToOut2.setValue(message[2])

                    if message[1] == 16: #  Filter 1 DFF, 0-127
                        self.main_app.comboBox_activeTrackingFilter1.setCurrentIndex(message[2])

                    if message[1] == 17: # Filter 2 DFF, 0-127
                        self.main_app.comboBox_activeTrackingFilter2.setCurrentIndex(message[2])

                    if message[1] == 18: # Filter LFO Cycles, 0-127
                        self.main_app.comboBox_FilterLfoCycles.setCurrentIndex(message[2])

                    if message[1] == 19: # Mod LFO Cycles, 0-127
                        self.main_app.comboBox_ModLfoCycles.setCurrentIndex(message[2])



                    if message[1] == 20: # OSC Main Volumen
                        self.main_app.dial_OSCS_Main_Mixer.setValue(message[2])

                    if message[1] == 21: # Filter 1 Freq
                        self.main_app.dial_f1_f.setValue(message[2])

                    if message[1] == 22: # Filter 1 Reso
                        self.main_app.dial_f1_r.setValue(message[2])

                    if message[1] == 23: # Filter 1 Octave
                        self.main_app.dial_f1_o.setValue(message[2])

                    if message[1] == 24: # Filter 2 Freq
                        self.main_app.dial_f2_f.setValue(message[2])

                    if message[1] == 25: # Filter 2 Reso
                        self.main_app.dial_f2_r.setValue(message[2])

                    if message[1] == 26: # Filter 2 Octave
                        self.main_app.dial_f2_o.setValue(message[2])

                    if message[1] == 27: # Filter 2 Mode
                        self.main_app.comboBox_f2_type.setCurrentIndex(message[2]-1)

                    if message[1] == 28: # Filter 1 - 2 Blend
                        self.main_app.horizontalSlider_Filter_Mixer.setValue(message[2])

                    if message[1] == 29: # Filter 1 - 2 crossover
                        if message[2] == 0:
                            self.main_app.checkBoxF1_F2crossOver.setChecked(False)
                        if message[2] == 1:
                            self.main_app.checkBoxF1_F2crossOver.setChecked(True)

                    if message[1] == 30: # Guitar Sensing Attack
                         self.main_app.verticalSliderGitarSensingAttack.setValue(message[2])

                    if message[1] == 31: # Guitar Sensing Release
                         self.main_app.dial_23GitSensRelease.setValue(message[2])

                    if message[1] == 32: # Guitar Sensing NOC
                         self.main_app.dial_24GuitarSensNoc.setValue(message[2])

                    if message[1] == 33: # Guitar Sensing Pitch detect treshold
                         self.main_app.dial_26PitchDetectTreshold.setValue(message[2])

                    if message[1] == 34: # Guitar Sensing Pitch detect quality
                         self.main_app.dial_25PitchTedectQuality.setValue(message[2])

                    if message[1] == 35: # OSC1 Volumen
                         self.main_app.dialOSC1Volume.setValue(message[2])

                    if message[1] == 36: # OSC1 WaveForm
                         self.main_app.comboBox_OSC1Waveform.setCurrentIndex(message[2])

                    if message[1] == 37: # OSC1 PulseWide
                         self.main_app.dial_5OSC1_PulseWide.setValue(message[2])

                    if message[1] == 38: # OSC1 Octave
                         self.main_app.dial_2OSC1Octave.setValue(message[2])

                    if message[1] == 39: # OSC1 Note
                         self.main_app.dial_3OSC1Note.setValue(message[2])

                    if message[1] == 40: # OSC1 Detune
                         self.main_app.dial_4OSC1Cents.setValue(message[2])

                    if message[1] == 41: # OSC1 Phase
                         self.main_app.dial_5OSC1_Phase.setValue(message[2])

                    if message[1] == 42: # OSC2 Volumen
                      self.main_app.dialOSC2Volume.setValue(message[2])

                    if message[1] == 43: # OSC2 WaveForm
                      self.main_app.comboBox_OSC2Waveform.setCurrentIndex(message[2])

                    if message[1] == 44: # OSC2 PulseWide
                      self.main_app.dial_5OSC2_PulseWide.setValue(message[2])

                    if message[1] == 45: # OSC2 Octave
                      self.main_app.dial_2OSC2Octave.setValue(message[2])

                    if message[1] == 46: # OSC2 Note
                      self.main_app.dial_3OSC2Note.setValue(message[2])

                    if message[1] == 47: # OSC2 Detune
                      self.main_app.dial_4OSC2Cents.setValue(message[2])

                    if message[1] == 48: # OSC2 Phase
                      self.main_app.dial_5OSC2_Phase.setValue(message[2])

                    if message[1] == 49: # SUBOSC1 Volumen
                         self.main_app.dialSUBOSC1Volume.setValue(message[2])

                    if message[1] == 50: # SUBOSC1 WaveForm
                         self.main_app.comboBox_SUBOSC1Waveform.setCurrentIndex(message[2])

                    if message[1] == 51: # SUBOSC1 PulseWide
                         self.main_app.dial_5SUBOSC1_PulseWide.setValue(message[2])

                    if message[1] == 52: # SUBOSC1 Octave
                         self.main_app.dial_2SUBOSC1Octave.setValue(message[2])

                    if message[1] == 53: # SUBOSC1 Note
                         self.main_app.dial_3SUBOSC1Note.setValue(message[2])

                    if message[1] == 54: # SUBOSC1 Detune
                         self.main_app.dial_4SUBOSC1Cents.setValue(message[2])

                    if message[1] == 55: # SUBOSC1 Phase
                         self.main_app.dial_5SUBOSC1_Phase.setValue(message[2])

                    if message[1] == 56: # SUBOSC2 Volumen
                      self.main_app.dialSUBOSC2Volume.setValue(message[2])

                    if message[1] == 57: # SUBOSC2 WaveForm
                      self.main_app.comboBox_SUBOSC2Waveform.setCurrentIndex(message[2])

                    if message[1] == 58: # SUBOSC2 PulseWide
                      self.main_app.dial_5SUBOSC2_PulseWide.setValue(message[2])

                    if message[1] == 59: # SUBOSC2 Octave
                      self.main_app.dial_2SUBOSC2Octave.setValue(message[2])

                    if message[1] == 60: # SUBOSC2 Note
                      self.main_app.dial_3SUBOSC2Note.setValue(message[2])

                    if message[1] == 61: # SUBOSC2 Detune
                      self.main_app.dial_4SUBOSC2Cents.setValue(message[2])

                    if message[1] == 62: # SUBOSC2 Phase
                      self.main_app.dial_5SUBOSC2_Phase.setValue(message[2])

                    if message[1] == 63: # NOISE Volumen
                      self.main_app.dail_NoiseVolumen.setValue(message[2])

                    if message[1] == 64: # NOISE Volumen
                      self.main_app.dial_3BitcrusherVolumen.setValue(message[2])

                    if message[1] == 65: # Bitcrusher Bits
                      self.main_app.comboBoxBicrusherBits.setCurrentIndex(message[2])

                    if message[1] == 66: # Bitcrusher Sample Rate
                        self.main_app.comboBox_2BitcrusherSampleRate.setCurrentIndex(int(message[2]))

                    if message[1] == 67: # Filter Lfo WaveForm
                        self.main_app.comboBox_5FilterLFOWafeForm.setCurrentIndex(int(message[2]))

                    if message[1] == 68: # Filter LFO PulseWide
                        self.main_app.dial_21FilterLFOPulseWide.setValue(message[2])

                    if message[1] == 69: # Filter LFO BPM
                        self.main_app.dialFilterLFOBPM.setValue(message[2])

                    if message[1] == 70: # Filter LFO BPM Multip
                        self.main_app.comboBox_6FilterLFOBpmMultip.setCurrentIndex(message[2])

                    if message[1] == 71: # Filter LFO Phase
                        self.main_app.dial_FilterLFO_Phase.setValue(message[2])

                    if message[1] == 72: # Filter Lfo Sync To Attack
                        if message[2] == 0:
                            self.main_app.checkBoxFilterLfoSyncToAttack.setChecked(False)
                        if message[2] == 1:
                            self.main_app.checkBoxFilterLfoSyncToAttack.setChecked(True)

                    if message[1] == 73: # Filter LFO Depth
                        self.main_app.dial_FilterLfoDepth.setValue(message[2])

                    if message[1] == 74: # Filter LFO Attack envelope to freq.
                            self.main_app.dial_FilterLfoATTCKtoFreq.setValue(message[2])

                    if message[1] == 75: # Filter LFO Attack envelope to depth.
                            self.main_app.dial_FilterLfoATTCKtoDepth.setValue(message[2])

                    if message[1] == 76: # Envelope Attack to Filter Cutoff.
                            self.main_app.dialAttackEnvelopeFilterCut.setValue(message[2])

                    if message[1] == 77: # Guitar use Filter DAHDSR
                        if message[2] == 0:
                            self.main_app.checkBox_GuitarUseFilterDAHDSR.setChecked(False)
                        if message[2] == 1:
                            self.main_app.checkBox_GuitarUseFilterDAHDSR.setChecked(True)

                    if message[1] == 78: # Guitar use Envelope AHDSR
                        if message[2] == 0:
                            self.main_app.checkBoxGuitarUseEnvelopeAHDSR.setChecked(False)
                        if message[2] == 1:
                            self.main_app.checkBoxGuitarUseEnvelopeAHDSR.setChecked(True)

                    if message[1] == 79: # Envelope Attack.
                        self.main_app.verticalSlider_envelopeAttack.setValue(message[2])

                    if message[1] == 80: # Envelope Hold.
                        self.main_app.verticalSlider_envelopeHold.setValue(message[2])

                    if message[1] == 81: # Envelope Decay.
                        self.main_app.verticalSlider_envelopeDecay.setValue(message[2])

                    if message[1] == 82: # Envelope sustain.
                        self.main_app.verticalSlider_envelopeSustain.setValue(message[2])

                    if message[1] == 83: # Envelope release.
                        self.main_app.verticalSlider_envelopeRelease.setValue(message[2])


                    if message[1] == 84: # Filter Delay
                        self.main_app.verticalSlider_FilterDelay.setValue(message[2])

                    if message[1] == 85: # Filter Attack
                        self.main_app.verticalSlider_FilterAttack.setValue(message[2])

                    if message[1] == 86: # Filter Hold
                        self.main_app.verticalSlider_FilterHold.setValue(message[2])

                    if message[1] == 87: # Filter Decay
                        self.main_app.verticalSlider_FilterDecay.setValue(message[2])

                    if message[1] == 88: # Filter Sustain
                        self.main_app.verticalSlider_FilterSustain.setValue(message[2])

                    if message[1] == 89: # Filter Release
                        self.main_app.verticalSlider_FilterRelease.setValue(message[2])


                    if message[1] == 90: # MOD Lfo WaveForm
                        self.main_app.comboBox_ModLFOWafeForm.setCurrentIndex(int(message[2]))

                    if message[1] == 91: # MOD LFO PulseWide
                        self.main_app.dial_ModLFOPulseWide.setValue(message[2])

                    if message[1] == 92: # MOD LFO BPM
                        self.main_app.dialModLFOBPM.setValue(message[2])

                    if message[1] == 93: # MOD LFO BPM Multip
                        self.main_app.comboBoxModLfoMulti.setCurrentIndex(message[2])

                    if message[1] == 94: # MOD LFO Phase
                        self.main_app.dial_ModLFO_Phase.setValue(message[2])

                    if message[1] == 95: # MOD Lfo Sync To Attack
                        if message[2] == 0:
                            self.main_app.checkBoxModLfoSyncToAttack.setChecked(False)
                        if message[2] == 1:
                            self.main_app.checkBoxModLfoSyncToAttack.setChecked(True)


                    if message[1] == 96: # Mod LFO Attack envelope to freq.
                            self.main_app.dial_ModLfoATTCKtoFreq.setValue(message[2])

                    if message[1] == 97: # Mod LFO Attack envelope to depth.
                            self.main_app.dial_ModLfoATTCKtoDepth.setValue(message[2])

                    if message[1] == 98: # Filter use Static envelope
                            self.main_app.dial_GuitarFilterStaticVelocity.setValue(message[2])

                    if message[1] == 99: # MOD LFO Frequency Target OSC1
                            self.main_app.verticalSlider_ModLfoTargetFrequencyToOSC1.setValue(message[2])

                    if message[1] == 100: # MOD LFO Frequency Target OSC2
                            self.main_app.verticalSlider_ModLfoTargetFrequencyToOSC2.setValue(message[2])

                    if message[1] == 101: # MOD LFO Frequency Target OSC1
                            self.main_app.verticalSlider_ModLfoTargetFrequencyToSubOSC1.setValue(message[2])

                    if message[1] == 102: # MOD LFO Frequency Target OSC2
                            self.main_app.verticalSlider_ModLfoTargetFrequencyToSubOSC2.setValue(message[2])

                    if message[1] == 103: # MOD Lfo Frequency Target 1-4 Sync
                        if message[2] == 0:
                            self.main_app.checkBoxModLfoSyncAllFreqencyTargets.setChecked(False)
                        if message[2] == 1:
                            self.main_app.checkBoxModLfoSyncAllFreqencyTargets.setChecked(True)

                    if message[1] == 104: # MOD Lfo Frequency Target 5-8 Sync
                        if message[2] == 0:
                            self.main_app.checkBoxModLfoSyncAllPulseTargets.setChecked(False)
                        if message[2] == 1:
                            self.main_app.checkBoxModLfoSyncAllPulseTargets.setChecked(True)


                    if message[1] == 105: # MOD LFO Pulse Target OSC1
                            self.main_app.verticalSlider_ModLfoTargetPulseToOSC1.setValue(message[2])

                    if message[1] == 106: # MOD LFO Pulse Target OSC2
                            self.main_app.verticalSlider_ModLfoTargetPulseToOSC2.setValue(message[2])

                    if message[1] == 107: # MOD LFO Pulse Target OSC1
                            self.main_app.verticalSlider_ModLfoTargetPulseToSubOSC1.setValue(message[2])

                    if message[1] == 108: # MOD LFO Pulse Target OSC2
                            self.main_app.verticalSlider_ModLfoTargetPulseToSubOSC2.setValue(message[2])

                    ############## Vol LFO #####################

                    if message[1] == 109: # Vol Lfo WaveForm
                        self.main_app.comboBox_VolLFOWafeForm.setCurrentIndex(int(message[2]))

                    if message[1] == 110: # Vol LFO PulseWide
                        self.main_app.dial_VolLFOPulseWide.setValue(message[2])

                    if message[1] == 111: # Vol LFO BPM
                        self.main_app.dialVolLFOBPM.setValue(message[2])

                    if message[1] == 112: # Vol LFO BPM Multip
                        self.main_app.comboBoxVolLfoMulti.setCurrentIndex(message[2])

                    if message[1] == 113: # Vol LFO Phase
                        self.main_app.dial_VolLFO_Phase.setValue(message[2])

                    if message[1] == 114: # Vol Lfo Sync To Attack
                        if message[2] == 0:
                            self.main_app.checkBoxVolLfoSyncToAttack.setChecked(False)
                        if message[2] == 1:
                            self.main_app.checkBoxVolLfoSyncToAttack.setChecked(True)

                    if message[1] == 115: # Vol LFO Attack envelope to freq.
                            self.main_app.dial_VolLfoATTCKtoFreq.setValue(message[2])

                    if message[1] == 116: # Vol LFO Attack envelope to depth.
                            self.main_app.dial_VolLfoATTCKtoDepth.setValue(message[2])

                    if message[1] == 117: # Vol Lfo Target 1-4 Sync
                        if message[2] == 0:
                            self.main_app.checkBoxVolLfoSyncAllTargets.setChecked(False)
                        if message[2] == 1:
                            self.main_app.checkBoxVolLfoSyncAllTargets.setChecked(True)

                    if message[1] == 118: # VOL LFO Target OSC1
                            self.main_app.verticalSlider_VolLfoTargetToOSC1.setValue(message[2])

                    if message[1] == 119: # VOL LFO Target OSC2
                            self.main_app.verticalSlider_VolLfoTargetToOSC2.setValue(message[2])

                    if message[1] == 120: # VOL LFO Target SUBS
                            self.main_app.verticalSlider_VolLfoTargetToAllSubs.setValue(message[2])

                    if message[1] == 121: # VOL LFO Target NOISE
                            self.main_app.verticalSlider_VolLfoTargetToNoise.setValue(message[2])

                    if message[1] == 122: # Midi Pitch Bend Range
                        self.main_app.comboBox_MidiPitchBendRange.setCurrentIndex(message[2])

                    if message[1] == 123: # Auto Peak Correction
                        if message[2] == 0:
                            self.main_app.checkBoxPeakCutoff.setChecked(False)
                        if message[2] == 1:
                            self.main_app.checkBoxPeakCutoff.setChecked(True)

                    if message[1] == 124: # Master clock Sync All LFOs
                        if message[2] == 0:
                            self.main_app.SYNC_ALL_LFOS.setChecked(False)
                        if message[2] == 1:
                            self.main_app.SYNC_ALL_LFOS.setChecked(True)

                    if message[1] == 125: # Master clock Sync MOD LFO
                        if message[2] == 0:
                            self.main_app.SYNCModLFO.setChecked(False)
                        if message[2] == 1:
                            self.main_app.SYNCModLFO.setChecked(True)

                    if message[1] == 126: # Master clock Sync Vol LFO
                        if message[2] == 0:
                            self.main_app.SYNCVolumenLFO.setChecked(False)
                        if message[2] == 1:
                            self.main_app.SYNCVolumenLFO.setChecked(True)

                    if message[1] == 127: # Master clock Sync Freq LFO
                        if message[2] == 0:
                            self.main_app.SYNCFLFO.setChecked(False)
                        if message[2] == 1:
                            self.main_app.SYNCFLFO.setChecked(True)



############################ MainWindow Class, SIMPLE as possible!! ##################
class MyForm(QtGui.QMainWindow):
    def __init__(self, parent=None):
        QtGui.QWidget.__init__(self, parent)
        self.ui = Ui_GitSynthV127()
        self.ui.setupUi(self)

        self.ui.weRequestSettings = False

        myLocalFilterLfoCycles = 0
        myLocalFilterLfoWaveform = 0
        myLocalModLfoCycles = 0
        myLocalModLfoWaveform = 0

        ##### midi input thread starten
        self.thisMidiInputThread = midiInputThread(self.ui)
        self.thisMidiInputThread.start()

        # self.ui.tabWidget.setStyleSheet("""
        #    QWidget
        #    {
        #     background-color: rgba(50,50,50, 250);
        #     }
        #     """)

        #self.ui.tabSynth.setStyleSheet("""
        #    QWidget
        #    {
        #     background-image: url("img/metaltwo.jpg");
        #     }
        #     """)

        # Background Image
        #palette	= QPalette()
        #palette.setBrush(QPalette.Background,QBrush(QPixmap("img/mainBackground.jpg")))
        #self.setPalette(palette)

        ####### Request/Send Settings##################
        self.ui.pushButtonRequest.pressed.connect(self.requestMidiSettings)
        self.ui.pushButtonRequest.setCheckable( False )
        self.ui.pushButton_TransmitAll.pressed.connect(self.sendAllMidiSettings)
        self.ui.pushButton_TransmitAll.setCheckable( False )

        self.ui.pushButton_sendSequence.pressed.connect(self.sendSequence)
        self.ui.pushButton_sendSequence.setCheckable( False )

        ####### Midi Note on/off ################################
        self.ui.pushButton_PlayTestNote.pressed.connect(self.togglePlayNote)
        self.ui.pushButton_PlayTestNote.setCheckable( True )
        #self.ui.pushButton_PlayTestNote.setChecked( False )

        ########## Programm change ##############################
        self.ui.spinBox_Program.valueChanged.connect(self.setProgramChange)

        ##### MIXER #############################################
        self.ui.InputGain_Dail.valueChanged.connect(self.setInputGain)
        self.ui.verticalSlider_InstrumentToSynth.valueChanged.connect(self.setInstrumentToSynth)
        self.ui.verticalSlider_Usb1ToSynth.valueChanged.connect(self.setUsb1ToSynth)
        self.ui.verticalSlider_SynthToOut1.valueChanged.connect(self.setSynthToOut1)
        self.ui.verticalSlider_SynthToOut2.valueChanged.connect(self.setSynthToOut2)
        self.ui.verticalSlider_SynthToUsbOut1.valueChanged.connect(self.setSynthToUsbOut1)
        self.ui.verticalSlider_SynthToUsbOut2.valueChanged.connect(self.setSynthToUsbOut2)

        self.ui.verticalSlider_InstrumentToUsbOut1.valueChanged.connect(self.setInstrumentToUsbOut1)
        self.ui.verticalSlider_InstrumentToUsbOut2.valueChanged.connect(self.setInstrumentToUsbOut2)
        self.ui.verticalSlider_UsbIn1ToOut1.valueChanged.connect(self.setUsbIn1ToOut1)
        self.ui.verticalSlider_UsbIn2ToOut1.valueChanged.connect(self.setUsbIn2ToOut1)
        self.ui.verticalSlider_UsbIn1ToOut2.valueChanged.connect(self.setUsbIn1ToOut2)
        self.ui.verticalSlider_UsbIn2ToOut2.valueChanged.connect(self.setUsbIn2ToOut2)



        # OSC Main Gain
        self.ui.dial_OSCS_Main_Mixer.valueChanged.connect(self.dailOscMainGainEvent)

        # Filter 1
        self.ui.dial_f1_f.valueChanged.connect(self.dailFilter1FreqEvent)
        self.ui.dial_f1_r.valueChanged.connect(self.dailFilter1ResonaceEvent)
        self.ui.dial_f1_o.valueChanged.connect(self.dailFilter1octaveEvent)

        # Filter 2
        self.ui.dial_f2_f.valueChanged.connect(self.dailFilter2FreqEvent)
        self.ui.dial_f2_r.valueChanged.connect(self.dailFilter2ResonaceEvent)
        self.ui.dial_f2_o.valueChanged.connect(self.dailFilter2octaveEvent)
        self.ui.comboBox_f2_type.currentIndexChanged.connect(self.setFilter2Mode)
        self.ui.horizontalSlider_Filter_Mixer.valueChanged.connect(self.setFilter1to2Blend)
        self.ui.checkBoxF1_F2crossOver.stateChanged.connect(self.setFilter1to2Crossover)

        # highcut
        self.ui.dial_highcut_filter.valueChanged.connect(self.dailHighCutFreqEvent)
        self.ui.dial_hcq.valueChanged.connect(self.dailHcqEvent)
        # lowcut
        self.ui.dial_lowcutfilter.valueChanged.connect(self.dailLowCutFreqEvent)
        self.ui.dial_lcq.valueChanged.connect(self.dailLcqEvent)

        # delay
        self.ui.Delay_Dry_Dail.valueChanged.connect(self.DelayDryMixEvent)
        self.ui.Delay_Wet_Dail.valueChanged.connect(self.DelayWetMixEvent)
        self.ui.Delay_TimeMs_SpinBox.valueChanged.connect(self.DelayTimeMsEvent)
        self.ui.comboBox_Delay_Time_Signature.currentIndexChanged.connect(self.DelayTimeSigEvent)
        self.ui.Delay_Gain_Dail.valueChanged.connect(self.DelayGainEvent)
        self.ui.Delay_FeedBack_Dail.valueChanged.connect(self.DelayFeedbackEvent)

        ############## delay end ######
        #Guitar Sensing
        self.ui.verticalSliderGitarSensingAttack.valueChanged.connect(self.setGuitarSensattack)
        self.ui.dial_23GitSensRelease.valueChanged.connect(self.setGuitarSensRelease)
        self.ui.dial_24GuitarSensNoc.valueChanged.connect(self.setGuitarSensNOC)
        self.ui.dial_26PitchDetectTreshold.valueChanged.connect(self.setGuitarSensPitchDetectTreshold)
        self.ui.dial_25PitchTedectQuality.valueChanged.connect(self.setGuitarSensPitchDetectQuality)
        self.ui.pushButtonGuitarSenseDefaults.pressed.connect(self.resetGuitarSenseDefaults)

        #OSC1
        self.ui.dialOSC1Volume.valueChanged.connect(self.setOSC1Volumen)
        self.ui.comboBox_OSC1Waveform.currentIndexChanged.connect(self.setOSC1Waveform)
        self.ui.dial_5OSC1_PulseWide.valueChanged.connect(self.setOSC1PulseWide)
        self.ui.dial_2OSC1Octave.valueChanged.connect(self.setOSC1Octave)
        self.ui.dial_3OSC1Note.valueChanged.connect(self.setOSC1Note)
        self.ui.dial_4OSC1Cents.valueChanged.connect(self.setOSC1Detune)
        self.ui.dial_5OSC1_Phase.valueChanged.connect(self.setOSC1Phase)

        #OSC2
        self.ui.dialOSC2Volume.valueChanged.connect(self.setOSC2Volumen)
        self.ui.comboBox_OSC2Waveform.currentIndexChanged.connect(self.setOSC2Waveform)
        self.ui.dial_5OSC2_PulseWide.valueChanged.connect(self.setOSC2PulseWide)
        self.ui.dial_2OSC2Octave.valueChanged.connect(self.setOSC2Octave)
        self.ui.dial_3OSC2Note.valueChanged.connect(self.setOSC2Note)
        self.ui.dial_4OSC2Cents.valueChanged.connect(self.setOSC2Detune)
        self.ui.dial_5OSC2_Phase.valueChanged.connect(self.setOSC2Phase)

        #SUBOSC1
        self.ui.dialSUBOSC1Volume.valueChanged.connect(self.setSUBOSC1Volumen)
        self.ui.comboBox_SUBOSC1Waveform.currentIndexChanged.connect(self.setSUBOSC1Waveform)
        self.ui.dial_5SUBOSC1_PulseWide.valueChanged.connect(self.setSUBOSC1PulseWide)
        self.ui.dial_2SUBOSC1Octave.valueChanged.connect(self.setSUBOSC1Octave)
        self.ui.dial_3SUBOSC1Note.valueChanged.connect(self.setSUBOSC1Note)
        self.ui.dial_4SUBOSC1Cents.valueChanged.connect(self.setSUBOSC1Detune)
        self.ui.dial_5SUBOSC1_Phase.valueChanged.connect(self.setSUBOSC1Phase)

        #SUBOSC2
        self.ui.dialSUBOSC2Volume.valueChanged.connect(self.setSUBOSC2Volumen)
        self.ui.comboBox_SUBOSC2Waveform.currentIndexChanged.connect(self.setSUBOSC2Waveform)
        self.ui.dial_5SUBOSC2_PulseWide.valueChanged.connect(self.setSUBOSC2PulseWide)
        self.ui.dial_2SUBOSC2Octave.valueChanged.connect(self.setSUBOSC2Octave)
        self.ui.dial_3SUBOSC2Note.valueChanged.connect(self.setSUBOSC2Note)
        self.ui.dial_4SUBOSC2Cents.valueChanged.connect(self.setSUBOSC2Detune)
        self.ui.dial_5SUBOSC2_Phase.valueChanged.connect(self.setSUBOSC2Phase)

        # Noise Volumen
        self.ui.dail_NoiseVolumen.valueChanged.connect(self.setNoiseVolumen)

        # Bit Crusher Volumen
        self.ui.dial_3BitcrusherVolumen.valueChanged.connect(self.setBitCrusherVolumen)
        self.ui.comboBoxBicrusherBits.currentIndexChanged.connect(self.setBitcrusherBits)
        self.ui.comboBox_2BitcrusherSampleRate.currentIndexChanged.connect(self.setBitcrusherRate)

        # Filter Lfo
        self.ui.comboBox_5FilterLFOWafeForm.currentIndexChanged.connect(self.setFilterLFOWaveForm)
        self.ui.dial_21FilterLFOPulseWide.valueChanged.connect(self.setFilterLFOPulseWide)
        self.ui.dialFilterLFOBPM.valueChanged.connect(self.setFilterLFOBpm)
        self.ui.comboBox_6FilterLFOBpmMultip.currentIndexChanged.connect(self.setFilterLfoBpmMulti)
        self.ui.dial_FilterLFO_Phase.valueChanged.connect(self.setFilterLfoPhase)
        self.ui.checkBoxFilterLfoSyncToAttack.stateChanged.connect(self.setFilterLfoSyncToAttack)
        self.ui.dial_FilterLfoDepth.valueChanged.connect(self.setFilterLfoDepth)
        self.ui.dial_FilterLfoATTCKtoFreq.valueChanged.connect(self.setFilterLfoAttackToFreq)
        self.ui.dial_FilterLfoATTCKtoDepth.valueChanged.connect(self.setFilterAttackenvelopeToFilterDepth)
        self.ui.dialAttackEnvelopeFilterCut.valueChanged.connect(self.setEnvelopeAttackenvelopeToFilterCuttoff)
        self.ui.checkBox_GuitarUseFilterDAHDSR.stateChanged.connect(self.setGuitarUseFilterDAHDSR)
        self.ui.checkBoxGuitarUseEnvelopeAHDSR.stateChanged.connect(self.setGuitarUseEnvelopeAHDSR)

        # Envelope AHDSR
        self.ui.verticalSlider_envelopeAttack.valueChanged.connect(self.setEnvelopeAttack)
        self.ui.verticalSlider_envelopeHold.valueChanged.connect(self.setEnvelopeHold)
        self.ui.verticalSlider_envelopeDecay.valueChanged.connect(self.setEnvelopeDecay)
        self.ui.verticalSlider_envelopeSustain.valueChanged.connect(self.setEnvelopeSustain)
        self.ui.verticalSlider_envelopeRelease.valueChanged.connect(self.setEnvelopeRelease)

        # Filter ADSR
        self.ui.verticalSlider_FilterDelay.valueChanged.connect(self.setFilterDelay)
        self.ui.verticalSlider_FilterAttack.valueChanged.connect(self.setFilterAttack)
        self.ui.verticalSlider_FilterHold.valueChanged.connect(self.setFilterHold)
        self.ui.verticalSlider_FilterDecay.valueChanged.connect(self.setFilterDecay)
        self.ui.verticalSlider_FilterSustain.valueChanged.connect(self.setFilterSustain)
        self.ui.verticalSlider_FilterRelease.valueChanged.connect(self.setFilterRelease)
        self.ui.dial_GuitarFilterStaticVelocity.valueChanged.connect(self.setGuitarStaticEnvelopeToFilter)

        # Mod LFO
        self.ui.comboBox_ModLFOWafeForm.currentIndexChanged.connect(self.setModLFOWaveForm)
        self.ui.dial_ModLFOPulseWide.valueChanged.connect(self.setModLFOPulseWide)
        self.ui.dialModLFOBPM.valueChanged.connect(self.setModLFOBpm)
        self.ui.comboBoxModLfoMulti.currentIndexChanged.connect(self.setModLfoBpmMulti)
        self.ui.dial_ModLFO_Phase.valueChanged.connect(self.setModLfoPhase)
        self.ui.checkBoxModLfoSyncToAttack.stateChanged.connect(self.setModLfoSyncToAttack)
        self.ui.dial_ModLfoATTCKtoFreq.valueChanged.connect(self.setModLfoAttackToFreq)
        self.ui.dial_ModLfoATTCKtoDepth.valueChanged.connect(self.setModAttackenvelopeToFilterDepth)      
        self.ui.verticalSlider_ModLfoTargetFrequencyToOSC1.valueChanged.connect(self.setModLfoTargetFreqOSC1)
        self.ui.verticalSlider_ModLfoTargetFrequencyToOSC2.valueChanged.connect(self.setModLfoTargetFreqOSC2)
        self.ui.verticalSlider_ModLfoTargetFrequencyToSubOSC1.valueChanged.connect(self.setModLfoTargetFreqSubOSC1)
        self.ui.verticalSlider_ModLfoTargetFrequencyToSubOSC2.valueChanged.connect(self.setModLfoTargetFreqSubOSC2)
        self.ui.checkBoxModLfoSyncAllFreqencyTargets.stateChanged.connect(self.setModLfoTarget1to4FreqSync)
        self.ui.checkBoxModLfoSyncAllPulseTargets.stateChanged.connect(self.setModLfoTarget5to8PulseSync)
        self.ui.verticalSlider_ModLfoTargetPulseToOSC1.valueChanged.connect(self.setModLfoTargetPulseOSC1)
        self.ui.verticalSlider_ModLfoTargetPulseToOSC2.valueChanged.connect(self.setModLfoTargetPulseOSC2)
        self.ui.verticalSlider_ModLfoTargetPulseToSubOSC1.valueChanged.connect(self.setModLfoTargetPulseSubOSC1)
        self.ui.verticalSlider_ModLfoTargetPulseToSubOSC2.valueChanged.connect(self.setModLfoTargetPulseSubOSC2)


        ### VOL LFO ##################
        self.ui.comboBox_VolLFOWafeForm.currentIndexChanged.connect(self.setVolLFOWaveForm)
        self.ui.dial_VolLFOPulseWide.valueChanged.connect(self.setVolLFOPulseWide)
        self.ui.dialVolLFOBPM.valueChanged.connect(self.setVolLFOBpm)
        self.ui.comboBoxVolLfoMulti.currentIndexChanged.connect(self.setVolLfoBpmMulti)
        self.ui.dial_VolLFO_Phase.valueChanged.connect(self.setVolLfoPhase)
        self.ui.checkBoxVolLfoSyncToAttack.stateChanged.connect(self.setVolLfoSyncToAttack)
        self.ui.dial_VolLfoATTCKtoFreq.valueChanged.connect(self.setVolLfoAttackToFreq)
        self.ui.dial_VolLfoATTCKtoDepth.valueChanged.connect(self.setVolAttackenvelopeToFilterDepth)
        self.ui.checkBoxVolLfoSyncAllTargets.stateChanged.connect(self.setVolLfoTarget1to4Sync)
        self.ui.verticalSlider_VolLfoTargetToOSC1.valueChanged.connect(self.setVolLfoTargetOSC1)
        self.ui.verticalSlider_VolLfoTargetToOSC2.valueChanged.connect(self.setVolLfoTargetOSC2)
        self.ui.verticalSlider_VolLfoTargetToAllSubs.valueChanged.connect(self.setVolLfoTargetAllSubs)
        self.ui.verticalSlider_VolLfoTargetToNoise.valueChanged.connect(self.setVolLfoTargetNoise)

        ##### MISK SETTINGS #################
        self.ui.comboBox_MidiPitchBendRange.currentIndexChanged.connect(self.setMidiPitchBendRange)
        self.ui.checkBoxPeakCutoff.stateChanged.connect(self.setAutoPeakCorrection)
        self.ui.SYNC_ALL_LFOS.stateChanged.connect(self.setAllLfosOnSync)
        self.ui.SYNCModLFO.stateChanged.connect(self.setModLfosOnSync)
        self.ui.SYNCVolumenLFO.stateChanged.connect(self.setVolLfosOnSync)
        self.ui.SYNCFLFO.stateChanged.connect(self.setFreqLfosOnSync)

        self.ui.comboBox_activeTrackingFilter1.currentIndexChanged.connect(self.setFilter1DFF)
        self.ui.comboBox_activeTrackingFilter2.currentIndexChanged.connect(self.setFilter2DFF)

        self.ui.comboBox_FilterLfoCycles.currentIndexChanged.connect(self.setFilterLfoCycles)
        self.ui.comboBox_ModLfoCycles.currentIndexChanged.connect(self.setModLfoCycles)

        self.ui.ExpPressionPedalComboBox.currentIndexChanged.connect(self.setExpressionPedal)
        self.ui.PotiComboBox.currentIndexChanged.connect(self.setPoti)
        self.ui.functionBtn_Combobox.currentIndexChanged.connect(self.setFunctionBtn)


        ##### Send Settings #################

    def requestMidiSettings(self):
        print ("Request Btn pressed, we interrupt midi_output while settings come in: %d" % self.ui.weRequestSettings)
        control = [NOTE_ON, 127, 1]
        midiOut.send_message(control)
        control = [NOTE_ON, 0, 1]
        midiOut.send_message(control)
        self.ui.weRequestSettings = True

    ############# Toggle Note #########################
    def togglePlayNote(self):
        noteIsChecked = self.ui.pushButton_PlayTestNote.isChecked()
        control = [NOTE_OFF, int(self.ui.spinBox_NoteValue.value()),0]
        if noteIsChecked == 0:
            self.ui.spinBox_NoteValue.value()
            control = [NOTE_ON, int(self.ui.spinBox_NoteValue.value()), 127]

        if noteIsChecked == 1:
            self.ui.spinBox_NoteValue.value()
            control = [NOTE_OFF, int(self.ui.spinBox_NoteValue.value()),0]

        midiOut.send_message(control)


###############################################################################################################################
    def sendAllMidiSettings(self):
        #print("Big Function:)")
        #def setAllLfosOnSync(self):


        # Dff Filter1
        myVal = self.ui.comboBox_activeTrackingFilter1.currentIndex()
        control = [CONTROLLER_CHANGE, 16, myVal] # Filter 1 DFF
        midiOut.send_message(control)

        # Dff Filter2
        myVal = self.ui.comboBox_activeTrackingFilter2.currentIndex()
        control = [CONTROLLER_CHANGE, 17, myVal] # Filter 2 DFF
        midiOut.send_message(control)


        # all lfos
        myVal = self.ui.SYNC_ALL_LFOS.checkState()
        if myVal == 2:
            myVal = 1
        control = [CONTROLLER_CHANGE, 124, myVal] # Master Clock To all LFOs
        midiOut.send_message(control)

        #def setModLfosOnSync(self):
        myVal = self.ui.SYNCModLFO.checkState()

        if myVal == 2:
            myVal = 1

        control = [CONTROLLER_CHANGE, 125, myVal] # Master Clock To Mod LFOs
        midiOut.send_message(control)


        #def setVolLfosOnSync(self):
        myVal = self.ui.SYNCVolumenLFO.checkState()

        if myVal == 2:
            myVal = 1

        control = [CONTROLLER_CHANGE, 126, myVal] # Master Clock To Vol LFOs
        midiOut.send_message(control)


        #def setFreqLfosOnSync(self):
        myVal = self.ui.SYNCFLFO.checkState()

        if myVal == 2:
            myVal = 1

        control = [CONTROLLER_CHANGE, 127, myVal] # Master Clock To Freq LFOs
        midiOut.send_message(control)
        myVal = self.ui.verticalSlider_InstrumentToSynth.value()
        control = [CONTROLLER_CHANGE, 4, myVal] #
        midiOut.send_message(control)

        self.ui.lcdNumber_InstrumentToSynth.display(int(self.mapValInt( myVal,0,127,0,100 )))


        myVal = self.ui.verticalSlider_Usb1ToSynth.value()
        control = [CONTROLLER_CHANGE, 5, myVal] #
        midiOut.send_message(control)

        self.ui.lcdNumber_Usb1ToSynth.display(int(self.mapValInt( myVal,0,127,0,100 )))


        myVal = self.ui.verticalSlider_SynthToOut1.value()
        control = [CONTROLLER_CHANGE, 6, myVal] #
        midiOut.send_message(control)

        self.ui.lcdNumber_SynthToOut1.display(int(self.mapValInt( myVal,0,127,0,100 )))

        #def setSynthToOut2(self):
        myVal = self.ui.verticalSlider_SynthToOut2.value()
        control = [CONTROLLER_CHANGE, 7, myVal] #
        midiOut.send_message(control)

        self.ui.lcdNumber_SynthToOut2.display(int(self.mapValInt( myVal,0,127,0,100 )))

        #def setSynthToUsbOut1(self):
        myVal = self.ui.verticalSlider_SynthToUsbOut1.value()
        control = [CONTROLLER_CHANGE, 8, myVal] #
        midiOut.send_message(control)

        self.ui.lcdNumber_SynthToUsbOut1.display(int(self.mapValInt( myVal,0,127,0,100 )))

        #def setSynthToUsbOut2(self):
        myVal = self.ui.verticalSlider_SynthToUsbOut2.value()
        control = [CONTROLLER_CHANGE, 9, myVal] #
        midiOut.send_message(control)

        self.ui.lcdNumber_SynthToUsbOut2.display(int(self.mapValInt( myVal,0,127,0,100 )))

        #def setInstrumentToUsbOut1(self):
        myVal = self.ui.verticalSlider_InstrumentToUsbOut1.value()
        control = [CONTROLLER_CHANGE, 10, myVal] #
        midiOut.send_message(control)

        self.ui.lcdNumber_InstrumentToUsbOut1.display(int(self.mapValInt( myVal,0,127,0,100 )))

        #def setInstrumentToUsbOut2(self):
        myVal = self.ui.verticalSlider_InstrumentToUsbOut2.value()
        control = [CONTROLLER_CHANGE, 11, myVal] #
        midiOut.send_message(control)

        self.ui.lcdNumber_InstrumentToUsbOut2.display(int(self.mapValInt( myVal,0,127,0,100 )))

        #def setUsbIn1ToOut1(self):
        myVal = self.ui.verticalSlider_UsbIn1ToOut1.value()
        control = [CONTROLLER_CHANGE, 12, myVal] #
        midiOut.send_message(control)

        self.ui.lcdNumber_UsbIn1ToOut1.display(int(self.mapValInt( myVal,0,127,0,100 )))


        time.sleep(0.05)

        #def setUsbIn2ToOut1(self):
        myVal = self.ui.verticalSlider_UsbIn2ToOut1.value()
        control = [CONTROLLER_CHANGE, 13, myVal] #
        midiOut.send_message(control)

        self.ui.lcdNumber_UsbIn2ToOut1.display(int(self.mapValInt( myVal,0,127,0,100 )))

        #def setUsbIn1ToOut2(self):
        myVal = self.ui.verticalSlider_UsbIn1ToOut2.value()
        control = [CONTROLLER_CHANGE, 14, myVal] #
        midiOut.send_message(control)

        self.ui.lcdNumber_UsbIn1ToOut2.display(int(self.mapValInt( myVal,0,127,0,100 )))

        #def setUsbIn2ToOut2(self):
        myVal = self.ui.verticalSlider_UsbIn2ToOut2.value()
        control = [CONTROLLER_CHANGE, 15, myVal] #
        midiOut.send_message(control)

        self.ui.lcdNumber_UsbIn2ToOut2.display(int(self.mapValInt( myVal,0,127,0,100 )))



        ################ LFO Cycles ###############
        #def setFilterLfoCycles(self):
        myVal = self.ui.comboBox_FilterLfoCycles.currentIndex()
        control = [CONTROLLER_CHANGE, 18, myVal] # Filter LFO Cycles
        midiOut.send_message(control)

        #def setModLfoCycles(self):
        myVal = self.ui.comboBox_ModLfoCycles.currentIndex()
        control = [CONTROLLER_CHANGE, 19, myVal] # Mod LFO Cycles
        midiOut.send_message(control)



        ################ OSC Gain #################
        #def dailOscMainGainEvent(self):
        myVal = self.ui.dial_OSCS_Main_Mixer.value()
        control = [CONTROLLER_CHANGE, 20, myVal] # filter 1 frequency
        midiOut.send_message(control)


        ################ Filter HighCut ###########
        #def dailHighCutFreqEvent(self):
        #print("pups")
        myVal = self.ui.dial_highcut_filter.value()
        control = [(CONTROLLER_CHANGE & 0xf0) | ((2) - 1 & 0xf), 1, myVal]
        midiOut.send_message(control)

        loghelperValInt = self.mapValInt( myVal,0,127,0,1000 )
        loghelperValFloat=float(loghelperValInt)/1000.0
        loghelperValFloat= (math.pow(loghelperValFloat,5)*20000)+10.0
        self.ui.label_value_highcut.setText("%.0f" % loghelperValFloat)

        #def dailHcqEvent(self):
        #print("pups2")
        myVal = self.ui.dial_hcq.value()
        control = [(CONTROLLER_CHANGE & 0xf0) | ((2) - 1 & 0xf), 2, myVal]
        midiOut.send_message(control)

        loghelperValInt = self.mapValInt( myVal*100,0,12700,710,25000 )
        loghelperValFloat = loghelperValInt/1000.0

        self.ui.label_value_hcq.setText("%.2f" % loghelperValFloat)

        ################ Filter LowCut ###########
        #def dailLowCutFreqEvent(self):
        #print("pups3")
        myVal = self.ui.dial_lowcutfilter.value()
        control = [(CONTROLLER_CHANGE & 0xf0) | ((2) - 1 & 0xf), 3, myVal]
        midiOut.send_message(control)

        loghelperValInt = self.mapValInt( myVal,0,127,0,1000 )
        loghelperValFloat=float(loghelperValInt)/1000.0
        loghelperValFloat= (math.pow(loghelperValFloat,5)*20000)+10.0
        self.ui.label_value_lowcut.setText("%.0f" % loghelperValFloat)

        #def dailLcqEvent(self):
        #print("pups4")
        myVal = self.ui.dial_lcq.value()
        control = [(CONTROLLER_CHANGE & 0xf0) | ((2) - 1 & 0xf), 4, myVal]
        midiOut.send_message(control)

        loghelperValInt = self.mapValInt( myVal*100,0,12700,710,25000 )
        loghelperValFloat = loghelperValInt/1000.0

        self.ui.label_value_lcq.setText("%.2f" % loghelperValFloat)


        ############ Delay ############################

        #def DelayDryMixEvent(self):
        myVal = self.ui.Delay_Dry_Dail.value()
        control = [(CONTROLLER_CHANGE & 0xf0) | ((2) - 1 & 0xf), 5, myVal]
        midiOut.send_message(control)

        loghelperValInt = self.mapValInt(myVal,0,127,0,100)

        self.ui.Delay1_Dry_Lable.setText("%d" % loghelperValInt)

        #def DelayWetMixEvent(self):
        myVal = self.ui.Delay_Wet_Dail.value()
        control = [(CONTROLLER_CHANGE & 0xf0) | ((2) - 1 & 0xf), 6, myVal]
        midiOut.send_message(control)

        loghelperValInt = self.mapValInt(myVal,0,127,0,100)

        self.ui.Delay1_Wet_Lable.setText("%d" % loghelperValInt)

        #def DelayTimeMsEvent(self):
        myVal = self.ui.Delay_TimeMs_SpinBox.value()
        loghelperValInt = self.mapValInt(myVal,0,1800,0,127)
        control = [(CONTROLLER_CHANGE & 0xf0) | ((2) - 1 & 0xf), 7, loghelperValInt]
        midiOut.send_message(control)

        #def DelayTimeSigEvent(self):
        myVal = self.ui.comboBox_Delay_Time_Signature.currentIndex()
        control = [(CONTROLLER_CHANGE & 0xf0) | ((2) - 1 & 0xf), 8, myVal]
        midiOut.send_message(control)

        #def DelayGainEvent(self):
        myVal = self.ui.Delay_Gain_Dail.value()
        control = [(CONTROLLER_CHANGE & 0xf0) | ((2) - 1 & 0xf), 9, myVal]
        midiOut.send_message(control)

        loghelperValInt = self.mapValInt(myVal,0,127,0,100)

        self.ui.Delay1_Gain_Lable.setText("%d" % loghelperValInt)

        #def DelayFeedbackEvent(self):
        myVal = self.ui.Delay_FeedBack_Dail.value()
        control = [(CONTROLLER_CHANGE & 0xf0) | ((2) - 1 & 0xf), 10, myVal]
        midiOut.send_message(control)

        loghelperValInt = self.mapValInt(myVal,0,127,0,120)

        self.ui.Delay1_Feetback_Lable.setText("%d" % loghelperValInt)



        ################ FILTER 1 #################
        # Filter 1 Frequency (CC 21)
        #def dailFilter1FreqEvent(self):
        #print "radstand %d" % self.ui.dial_f1_f.value()
        myVal = self.ui.dial_f1_f.value()
        control = [CONTROLLER_CHANGE, 21, myVal] # filter 1 frequency
        midiOut.send_message(control)

        if self.ui.comboBox_activeTrackingFilter1.currentIndex() > 0:
            loghelperValInt = self.mapValInt( myVal,0,127,-60,60)
            self.ui.lcdNumber.display(int(loghelperValInt))
        else:
            loghelperValInt = self.mapValInt( myVal,0,127,0,1000 )
            loghelperValFloat=float(loghelperValInt)/1000.0
            loghelperValFloat= (math.pow(loghelperValFloat,5)*20000)+10.0
            self.ui.lcdNumber.display(int(loghelperValFloat))

        #def dailFilter1ResonaceEvent(self):
        myVal = self.ui.dial_f1_r.value()
        #print "radstand %d" % self.ui.dial_f1_f.value()
        control = [CONTROLLER_CHANGE, 22, myVal] # filter 1 frequency
        midiOut.send_message(control)

        loghelperValInt = self.mapValInt( myVal*100,0,12700,710,25000 )
        loghelperValFloat = loghelperValInt/1000.0

        self.ui.lcdNumber_2.display(loghelperValFloat)

        #def dailFilter1octaveEvent(self):
        myVal = self.ui.dial_f1_o.value()
        control = [CONTROLLER_CHANGE, 23, myVal] # filter 1 frequency
        midiOut.send_message(control)

        loghelperValInt = self.mapValInt( myVal*100,0,12700,0,7000 )
        loghelperValFloat = loghelperValInt/1000.0

        self.ui.lcdNumber_3.display(loghelperValFloat)


        ################ FILTER 2 #################
        # Filter 2 Frequency (CC 24)
        #def dailFilter2FreqEvent(self):
        myVal = self.ui.dial_f2_f.value()
        control = [CONTROLLER_CHANGE, 24, myVal] # filter 2 frequency
        midiOut.send_message(control)

        if self.ui.comboBox_activeTrackingFilter2.currentIndex() > 0:
            loghelperValInt = self.mapValInt( myVal,0,127,-60,60)
            self.ui.lcdNumber_4.display(int(loghelperValInt))
        else:
            loghelperValInt = self.mapValInt( myVal,0,127,0,1000 )
            loghelperValFloat=float(loghelperValInt)/1000.0
            loghelperValFloat= (math.pow(loghelperValFloat,5)*20000)+10.0
            self.ui.lcdNumber_4.display(int(loghelperValFloat))


        #def dailFilter2ResonaceEvent(self):
        myVal = self.ui.dial_f2_r.value()
        #print "radstand %d" % self.ui.dial_f1_f.value()
        control = [CONTROLLER_CHANGE, 25, myVal] # filter 2 reso
        midiOut.send_message(control)

        loghelperValInt = self.mapValInt( myVal*100,0,12700,710,25000 )
        loghelperValFloat = loghelperValInt/1000.0

        self.ui.lcdNumber_5.display(loghelperValFloat)

        #def dailFilter2octaveEvent(self):
        myVal = self.ui.dial_f2_o.value()
        control = [CONTROLLER_CHANGE, 26, self.ui.dial_f2_o.value()] # filter 2 octave
        midiOut.send_message(control)

        loghelperValInt = self.mapValInt( myVal*100,0,12700,0,7000 )
        loghelperValFloat = loghelperValInt/1000.0

        self.ui.lcdNumber_6.display(loghelperValFloat)

        #def setFilter2Mode(self):

        myVal = self.ui.comboBox_f2_type.currentIndex() +1
        #print (myVal)
        control = [CONTROLLER_CHANGE, 27, myVal] # filter 2 Mode
        midiOut.send_message(control)

        #def setFilter1to2Blend(self):

        myVal =  self.ui.horizontalSlider_Filter_Mixer.value()
        #print("blend %d" % myVal)
        control = [CONTROLLER_CHANGE, 28, myVal] # filter 2 Mode
        midiOut.send_message(control)

        loghelperValInt = self.mapValInt( myVal*100,0,12700,-500,500)
        #print("blend mitte %d" % loghelperValInt)
        myKopfKnackerLeft = 0
        myKopfKnackerRight = 0
        if loghelperValInt < 0:
            myKopfKnackerLeft = 500 + math.fabs(loghelperValInt)
            myKopfKnackerRight = 500 - math.fabs(loghelperValInt)

        if loghelperValInt > 0:
            myKopfKnackerLeft = 500 - math.fabs(loghelperValInt)
            myKopfKnackerRight = 500 + math.fabs(loghelperValInt)

        self.ui.lcdNumber_7.display(int(myKopfKnackerLeft/10))
        self.ui.lcdNumber_8.display(int(myKopfKnackerRight/10))
        #print("blend letzte links %d" % myKopfKnackerLeft)
        #print("blend letzte rechts %d" % myKopfKnackerRight)

        #def setFilter1to2Crossover(self):
        myVal = self.ui.checkBoxF1_F2crossOver.checkState()
        #print("crossover %d" % myVal)
        if myVal == 2:
            myVal = 1

        control = [CONTROLLER_CHANGE, 29, myVal] # filter 2 Mode
        midiOut.send_message(control)

        ################### Guitar Sensing ##########################

        time.sleep(0.05)

        #def setGuitarSensattack(self):
        myVal = self.ui.verticalSliderGitarSensingAttack.value()
        control = [CONTROLLER_CHANGE, 30, myVal] # Guitar Sensing Attack
        midiOut.send_message(control)

        loghelperValInt = self.mapValInt(myVal,0,127,0,1000)
        loghelperValFloat=float(loghelperValInt)/1000.0
        loghelperValFloat= math.pow(loghelperValFloat,5)*3000
        self.ui.label_GitSensAttack.setText("%d" % int(loghelperValFloat+1.0))


        #def setGuitarSensRelease(self):
        myVal = self.ui.dial_23GitSensRelease.value()
        control = [CONTROLLER_CHANGE, 31, myVal] # Guitar Sensing Release
        midiOut.send_message(control)

        loghelperValInt = self.mapValInt(myVal,0,127,0,100)
        self.ui.lcdNumber_13.display(loghelperValInt)


        #def setGuitarSensNOC(self):
        myVal = self.ui.dial_24GuitarSensNoc.value()
        control = [CONTROLLER_CHANGE, 32, myVal] # Guitar Sensing NOC
        midiOut.send_message(control)

        loghelperValInt = self.mapValInt(myVal,0,127,0,1000)
        loghelperValFloat=float(loghelperValInt)/1000.0
        loghelperValFloat= math.pow(loghelperValFloat,5)*8000
        self.ui.lcdNumber_14_GuitSensNoc.display(int(loghelperValFloat))

        #def setGuitarSensPitchDetectTreshold(self):
        myVal = self.ui.dial_26PitchDetectTreshold.value()
        control = [CONTROLLER_CHANGE, 33, myVal] # Guitar Sensing Pitch Detect treshold
        midiOut.send_message(control)

        loghelperValInt = self.mapValInt(myVal,0,127,0,1000)
        loghelperValFloat=float(loghelperValInt)/1000.0
        loghelperValFloat= math.pow(loghelperValFloat,5)*25000
        self.ui.lcdNumber_15PitchDetectTreshold.display(int(loghelperValFloat))

        #def setGuitarSensPitchDetectQuality(self):
        myVal = self.ui.dial_25PitchTedectQuality.value()
        control = [CONTROLLER_CHANGE, 34, myVal] # Guitar Sensing Pitch Quality
        midiOut.send_message(control)

        loghelperValInt = self.mapValInt(myVal,0,127,0,99)
        self.ui.lcdNumber_16PitchTedectQuality.display(loghelperValInt)

        #def resetGuitarSenseDefaults(self):

        # muss noch überlegt werden ob git sens attack auch default gesetzt wird
        #self.main_app.verticalSliderGitarSensingAttack.setValue(0) #1ms
        #control = [CONTROLLER_CHANGE, 30, 0]
        #midiOut.send_message(control)

        # Guitar Sensing Release
        self.ui.dial_23GitSensRelease.setValue(64) #503
        control = [CONTROLLER_CHANGE, 31, 64]
        midiOut.send_message(control)

        # Guitar Sensing NOC
        self.ui.dial_24GuitarSensNoc.setValue(77) #653
        control = [CONTROLLER_CHANGE, 32, 77]
        midiOut.send_message(control)

        # Guitar Sensing Pitch detect treshold
        self.ui.dial_26PitchDetectTreshold.setValue(57) #90
        control = [CONTROLLER_CHANGE, 33, 57]
        midiOut.send_message(control)

        # Guitar Sensing Pitch detect quality
        self.ui.dial_25PitchTedectQuality.setValue(106) # 0.826
        control = [CONTROLLER_CHANGE, 34, 106]
        midiOut.send_message(control)

        ################### OSC 1 #################################################
        #def setOSC1Volumen(self):
        myVal = self.ui.dialOSC1Volume.value()
        control = [CONTROLLER_CHANGE, 35, myVal] # OSC1 Volumen
        midiOut.send_message(control)
        loghelperValInt = self.mapValInt(myVal,0,127,0,100)
        self.ui.lcdNumber_9OSC1Volume.display(loghelperValInt)

        #def setOSC1Waveform(self):
        myVal = self.ui.comboBox_OSC1Waveform.currentIndex()
        control = [CONTROLLER_CHANGE, 36, myVal] # OSC1 Waveform
        midiOut.send_message(control)

        #def setOSC1PulseWide(self):
        myVal = self.ui.dial_5OSC1_PulseWide.value()
        control = [CONTROLLER_CHANGE, 37, myVal] # OSC1 PulseWide
        midiOut.send_message(control)
        loghelperValInt = self.mapValInt(myVal,0,127,1,100)
        self.ui.label_23OSC1_PulseWide.setText("%d" % loghelperValInt)

        #def setOSC1Octave(self):
        myVal = self.ui.dial_2OSC1Octave.value()
        control = [CONTROLLER_CHANGE, 38, myVal] # OSC1 Octave
        midiOut.send_message(control)

        self.ui.label_16OSC1Octave.setText("%d" % (myVal - 3))

        #def setOSC1Note(self):
        myVal = self.ui.dial_3OSC1Note.value()
        control = [CONTROLLER_CHANGE, 39, myVal] # OSC1 Octave
        midiOut.send_message(control)

        self.ui.label_17OSC1Note.setText("%d" % (myVal - 12))

        #def setOSC1Detune(self):
        myVal = self.ui.dial_4OSC1Cents.value()
        control = [CONTROLLER_CHANGE, 40, myVal] # OSC1 Detune cents
        midiOut.send_message(control)

        self.ui.label_18OSC1Cents.setText("%d" % (self.mapValInt(myVal,0,24,-100,100)))

        #def setOSC1Phase(self):
        myVal = self.ui.dial_5OSC1_Phase.value()
        control = [CONTROLLER_CHANGE, 41, myVal] # OSC1 Phase
        midiOut.send_message(control)

        myLableVal = self.mapValInt(myVal,0,127,0,360)
        self.ui.label_18OSC1Phase.setText("%d" % (myLableVal))

        time.sleep(0.05)
        ################### OSC 2 #################################################
        #def setOSC2Volumen(self):
        myVal = self.ui.dialOSC2Volume.value()
        control = [CONTROLLER_CHANGE, 42, myVal] # OSC2 Volumen
        midiOut.send_message(control)
        loghelperValInt = self.mapValInt(myVal,0,127,0,100)
        self.ui.lcdNumber_9OSC2Volume.display(loghelperValInt)

        #def setOSC2Waveform(self):
        myVal = self.ui.comboBox_OSC2Waveform.currentIndex()
        control = [CONTROLLER_CHANGE, 43, myVal] # OSC2 Waveform
        midiOut.send_message(control)

        #def setOSC2PulseWide(self):
        myVal = self.ui.dial_5OSC2_PulseWide.value()
        control = [CONTROLLER_CHANGE, 44, myVal] # OSC2 PulseWide
        midiOut.send_message(control)
        loghelperValInt = self.mapValInt(myVal,0,127,1,100)
        self.ui.label_23OSC2_PulseWide.setText("%d" % loghelperValInt)

        #def setOSC2Octave(self):
        myVal = self.ui.dial_2OSC2Octave.value()
        control = [CONTROLLER_CHANGE, 45, myVal] # OSC2 Octave
        midiOut.send_message(control)

        self.ui.label_16OSC2Octave.setText("%d" % (myVal - 3))

        #def setOSC2Note(self):
        myVal = self.ui.dial_3OSC2Note.value()
        control = [CONTROLLER_CHANGE, 46, myVal] # OSC2 Octave
        midiOut.send_message(control)

        self.ui.label_17OSC2Note.setText("%d" % (myVal - 12))

        #def setOSC2Detune(self):
        myVal = self.ui.dial_4OSC2Cents.value()
        control = [CONTROLLER_CHANGE, 47, myVal] # OSC2 Detune cents
        midiOut.send_message(control)

        self.ui.label_18OSC2Cents.setText("%d" % (self.mapValInt(myVal,0,24,-100,100)))

        #def setOSC2Phase(self):
        myVal = self.ui.dial_5OSC2_Phase.value()
        control = [CONTROLLER_CHANGE, 48, myVal] # OSC2 Phase
        midiOut.send_message(control)

        myLableVal = self.mapValInt(myVal,0,127,0,360)
        self.ui.label_18OSC2Phase.setText("%d" % (myLableVal))


        ################### SUB OSC 1 #################################################
        #def setSUBOSC1Volumen(self):
        myVal = self.ui.dialSUBOSC1Volume.value()
        control = [CONTROLLER_CHANGE, 49, myVal] # SUBOSC1 Volumen
        midiOut.send_message(control)
        loghelperValInt = self.mapValInt(myVal,0,127,0,100)
        self.ui.lcdNumber_9SUBOSC1Volume.display(loghelperValInt)

        #def setSUBOSC1Waveform(self):
        myVal = self.ui.comboBox_SUBOSC1Waveform.currentIndex()
        control = [CONTROLLER_CHANGE, 50, myVal] # SUBOSC1 Waveform
        midiOut.send_message(control)

        #def setSUBOSC1PulseWide(self):
        myVal = self.ui.dial_5SUBOSC1_PulseWide.value()
        control = [CONTROLLER_CHANGE, 51, myVal] # SUBOSC1 PulseWide
        midiOut.send_message(control)
        loghelperValInt = self.mapValInt(myVal,0,127,1,100)
        self.ui.label_23SUBOSC1_PulseWide.setText("%d" % loghelperValInt)

        #def setSUBOSC1Octave(self):
        myVal = self.ui.dial_2SUBOSC1Octave.value()
        control = [CONTROLLER_CHANGE, 52, myVal] # SUBOSC1 Octave
        midiOut.send_message(control)

        self.ui.label_16SUBOSC1Octave.setText("%d" % (myVal - 3))

        #def setSUBOSC1Note(self):
        myVal = self.ui.dial_3SUBOSC1Note.value()
        control = [CONTROLLER_CHANGE, 53, myVal] # SUBOSC1 Octave
        midiOut.send_message(control)

        self.ui.label_17SUBOSC1Note.setText("%d" % (myVal - 12))

        #def setSUBOSC1Detune(self):
        myVal = self.ui.dial_4SUBOSC1Cents.value()
        control = [CONTROLLER_CHANGE, 54, myVal] # SUBOSC1 Detune cents
        midiOut.send_message(control)

        self.ui.label_18SUBOSC1Cents.setText("%d" % (self.mapValInt(myVal,0,24,-100,100)))

        #def setSUBOSC1Phase(self):
        myVal = self.ui.dial_5SUBOSC1_Phase.value()
        control = [CONTROLLER_CHANGE, 55, myVal] # SUBOSC1 Phase
        midiOut.send_message(control)

        myLableVal = self.mapValInt(myVal,0,127,0,360)
        self.ui.label_18SUBOSC1Phase.setText("%d" % (myLableVal))

        ################### SUB OSC 2 #################################################
        #def setSUBOSC2Volumen(self):
        myVal = self.ui.dialSUBOSC2Volume.value()
        control = [CONTROLLER_CHANGE, 56, myVal] # SUBOSC2 Volumen
        midiOut.send_message(control)
        loghelperValInt = self.mapValInt(myVal,0,127,0,100)
        self.ui.lcdNumber_9SUBOSC2Volume.display(loghelperValInt)

        #def setSUBOSC2Waveform(self):
        myVal = self.ui.comboBox_SUBOSC2Waveform.currentIndex()
        control = [CONTROLLER_CHANGE, 57, myVal] # SUBOSC2 Waveform
        midiOut.send_message(control)

        #def setSUBOSC2PulseWide(self):
        myVal = self.ui.dial_5SUBOSC2_PulseWide.value()
        control = [CONTROLLER_CHANGE, 58, myVal] # SUBOSC2 PulseWide
        midiOut.send_message(control)
        loghelperValInt = self.mapValInt(myVal,0,127,1,100)
        self.ui.label_23SUBOSC2_PulseWide.setText("%d" % loghelperValInt)

        #def setSUBOSC2Octave(self):
        myVal = self.ui.dial_2SUBOSC2Octave.value()
        control = [CONTROLLER_CHANGE, 59, myVal] # SUBOSC2 Octave
        midiOut.send_message(control)

        self.ui.label_16SUBOSC2Octave.setText("%d" % (myVal - 3))

        #def setSUBOSC2Note(self):
        myVal = self.ui.dial_3SUBOSC2Note.value()
        control = [CONTROLLER_CHANGE, 60, myVal] # SUBOSC2 Note
        midiOut.send_message(control)

        self.ui.label_17SUBOSC2Note.setText("%d" % (myVal - 12))

        #def setSUBOSC2Detune(self):
        myVal = self.ui.dial_4SUBOSC2Cents.value()
        control = [CONTROLLER_CHANGE, 61, myVal] # SUBOSC2 Detune cents
        midiOut.send_message(control)

        self.ui.label_18SUBOSC2Cents.setText("%d" % (self.mapValInt(myVal,0,24,-100,100)))

        #def setSUBOSC2Phase(self):
        myVal = self.ui.dial_5SUBOSC2_Phase.value()
        control = [CONTROLLER_CHANGE, 62, myVal] # SUBOSC2 Phase
        midiOut.send_message(control)

        myLableVal = self.mapValInt(myVal,0,127,0,360)
        self.ui.label_18SUBOSC2Phase.setText("%d" % (myLableVal))

        time.sleep(0.05)
        #################### Noise Volumen #########################################

        #def setNoiseVolumen(self):
        myVal = self.ui.dail_NoiseVolumen.value()

        control = [CONTROLLER_CHANGE, 63, myVal] # Noise Volumen
        midiOut.send_message(control)
        loghelperValInt = self.mapValInt(myVal,0,127,0,100)
        self.ui.label_41NoiseVolumen.setText("%d" % (loghelperValInt))

        #################### Bit Crusher ###########################################

        #def setBitCrusherVolumen(self):
        myVal = self.ui.dial_3BitcrusherVolumen.value()

        control = [CONTROLLER_CHANGE, 64, myVal] # Bit Crusher Volumen
        midiOut.send_message(control)
        loghelperValInt = self.mapValInt(myVal,0,127,0,100)
        self.ui.label_43BitcrusherVolumen.setText("%d" % (loghelperValInt))

        #def setBitcrusherBits(self):
        myVal = self.ui.comboBoxBicrusherBits.currentIndex()

        control = [CONTROLLER_CHANGE, 65, myVal] # Bit Crusher Bits
        midiOut.send_message(control)

        #def setBitcrusherRate(self):
        myVal = self.ui.comboBox_2BitcrusherSampleRate.currentIndex()

        control = [CONTROLLER_CHANGE, 66, myVal] # Bit Crusher Rate
        midiOut.send_message(control)

        ###################### Filter LFO ##########################################

        #def setFilterLFOWaveForm(self):
        myVal = self.ui.comboBox_5FilterLFOWafeForm.currentIndex()
        control = [CONTROLLER_CHANGE, 67, myVal] # Filter LFO waveForm
        midiOut.send_message(control)

        #def setFilterLFOPulseWide(self):
        myVal = self.ui.dial_21FilterLFOPulseWide.value()
        control = [CONTROLLER_CHANGE, 68, myVal] # Filter LFO PulseWide
        midiOut.send_message(control)
        loghelperValInt = self.mapValInt(myVal,0,127,1,100)
        self.ui.label_68FilterLfoPulseWide.setText("%d" % loghelperValInt)

        #def setFilterLFOBpm(self):
        myVal = self.ui.dialFilterLFOBPM.value()
        control = [CONTROLLER_CHANGE, 69, myVal ] # Filter LFO BPM
        midiOut.send_message(control)
        self.ui.label_18FilterLFOBPM.setText("%d" % self.mapValInt(myVal,0,127,0,360))

        #def setFilterLfoBpmMulti(self):
        myVal = self.ui.comboBox_6FilterLFOBpmMultip.currentIndex()
        control = [CONTROLLER_CHANGE, 70, myVal] # Filter LFO BPM Multi
        midiOut.send_message(control)

        #def setFilterLfoPhase(self):
        myVal = self.ui.dial_FilterLFO_Phase.value()
        control = [CONTROLLER_CHANGE, 71, myVal] # Filter LFO Phase
        midiOut.send_message(control)

        myLableVal = self.mapValInt(myVal,0,127,0,360)
        self.ui.label_Filter_LFO_Phase.setText("%d" % (myLableVal))

        #def setFilterLfoSyncToAttack(self):
        myVal = self.ui.checkBoxFilterLfoSyncToAttack.checkState()
        #print("sync filter to attack %d" % myVal)
        if myVal == 2:
            myVal = 1

        control = [CONTROLLER_CHANGE, 72, myVal] # filter 2 Mode
        midiOut.send_message(control)


        #def setFilterLfoDepth(self):
        myVal = self.ui.dial_FilterLfoDepth.value()
        control = [CONTROLLER_CHANGE, 73, myVal] # Filter LFO Depth
        midiOut.send_message(control)

        myLableVal = self.mapValInt(myVal,0,127,-100,100)
        if myVal == 63 or myVal == 64:
            self.ui.label_FilterLfoDepth.setText("%d" % (0))
        else:
            self.ui.label_FilterLfoDepth.setText("%d" % (myLableVal))

        #def setFilterLfoAttackToFreq(self):
        myVal = self.ui.dial_FilterLfoATTCKtoFreq.value()
        control = [CONTROLLER_CHANGE, 74, myVal] # Filter LFO Attack Envelope to Freq.
        midiOut.send_message(control)

        self.ui.label_FilterLfoAttackToFreq.setText("%d" % (self.mapValInt(myVal,0,127,0,30)))

        #def setFilterAttackenvelopeToFilterDepth(self):
        myVal = self.ui.dial_FilterLfoATTCKtoDepth.value()
        control = [CONTROLLER_CHANGE, 75, myVal] # Filter LFO Attack Envelope to Depth.
        midiOut.send_message(control)
        if myVal ==  63 or myVal == 64:
            self.ui.label_FilterLfoAttackToDepth.setText("%d" % (0))
        else:
            self.ui.label_FilterLfoAttackToDepth.setText("%d" % (self.mapValInt(myVal,0,127,-100,100)))

        #def setEnvelopeAttackenvelopeToFilterCuttoff(self):
        myVal = self.ui.dialAttackEnvelopeFilterCut.value()
        control = [CONTROLLER_CHANGE, 76, myVal] # Filter LFO Attack Envelope to Freq.
        midiOut.send_message(control)

        if myVal ==  63 or myVal == 64:
            self.ui.labelEnvelopeFilterCut.setText("%d" % (0))
        else:
            self.ui.labelEnvelopeFilterCut.setText("%d" % (self.mapValInt(myVal,0,127,-100,100)))

        #def setGuitarUseFilterDAHDSR(self):
        myVal = self.ui.checkBox_GuitarUseFilterDAHDSR.checkState()
        if myVal == 2:
            myVal = 1

        control = [CONTROLLER_CHANGE, 77, myVal] # Guitar use filter DAHDSR
        midiOut.send_message(control)

        #def setGuitarUseEnvelopeAHDSR(self):
        myVal = self.ui.checkBoxGuitarUseEnvelopeAHDSR.checkState()
        if myVal == 2:
            myVal = 1

        control = [CONTROLLER_CHANGE, 78, myVal] # Guitar use Envelope DAHDSR
        midiOut.send_message(control)

        time.sleep(0.05)
        #################### Envelope AHDSR #######################################
        #def setEnvelopeAttack(self):
        myVal = self.ui.verticalSlider_envelopeAttack.value()
        control = [CONTROLLER_CHANGE, 79, myVal] # Attack Envelope
        midiOut.send_message(control)

        loghelperValInt = self.mapValInt(myVal,0,127,0,1000)
        loghelperValFloat=float(loghelperValInt)/1000.0
        loghelperValFloat= math.pow(loghelperValFloat,3)*10000
        if loghelperValFloat+1 > 10000:
            loghelperValFloat = 9999

        self.ui.label_EnvelopeAttack.setText("%d" % int(loghelperValFloat+1))

        #def setEnvelopeHold(self):
        myVal = self.ui.verticalSlider_envelopeHold.value()
        control = [CONTROLLER_CHANGE, 80, myVal] # Hold Envelope
        midiOut.send_message(control)

        loghelperValInt = self.mapValInt(myVal,0,127,0,1000)
        loghelperValFloat=float(loghelperValInt)/1000.0
        loghelperValFloat= math.pow(loghelperValFloat,3)*10000
        if loghelperValFloat+1 > 10000:
            loghelperValFloat = 9999

        self.ui.label_EnvelopeHold.setText("%d" % int(loghelperValFloat+1))

        #def setEnvelopeDecay(self):
        myVal = self.ui.verticalSlider_envelopeDecay.value()
        control = [CONTROLLER_CHANGE, 81, myVal] # Decay Envelope
        midiOut.send_message(control)

        loghelperValInt = self.mapValInt(myVal,0,127,0,1000)
        loghelperValFloat=float(loghelperValInt)/1000.0
        loghelperValFloat= math.pow(loghelperValFloat,3)*10000
        if loghelperValFloat+1 > 10000:
            loghelperValFloat = 9999

        self.ui.label_EnvelopeDecay.setText("%d" % int(loghelperValFloat+1))

        #def setEnvelopeSustain(self):
        myVal = self.ui.verticalSlider_envelopeSustain.value()
        control = [CONTROLLER_CHANGE, 82, myVal] # sustain envelope
        midiOut.send_message(control)

        self.ui.label_EnvelopeSustain.setText("%d" % int(myVal))

        #def setEnvelopeRelease(self):
        myVal = self.ui.verticalSlider_envelopeRelease.value()
        control = [CONTROLLER_CHANGE, 83, myVal] # release Envelope
        midiOut.send_message(control)

        loghelperValInt = self.mapValInt(myVal,0,127,0,1000)
        loghelperValFloat=float(loghelperValInt)/1000.0
        loghelperValFloat= math.pow(loghelperValFloat,3)*10000
        if loghelperValFloat+1 > 10000:
            loghelperValFloat = 9999

        self.ui.label_EnvelopeRelease.setText("%d" % int(loghelperValFloat+1))

        #################### Filter ADSR ##########################################

        #def setFilterDelay(self):
        myVal = self.ui.verticalSlider_FilterDelay.value()
        control = [CONTROLLER_CHANGE, 84, myVal] # Filter Delay
        midiOut.send_message(control)

        loghelperValInt = self.mapValInt(myVal,0,127,0,1000)
        loghelperValFloat=float(loghelperValInt)/1000.0
        loghelperValFloat= math.pow(loghelperValFloat,3)*10000
        if loghelperValFloat+1 > 10000:
            loghelperValFloat = 9999

        self.ui.label_FilterDelay.setText("%d" % int(loghelperValFloat+1))

        #def setFilterAttack(self):
        myVal = self.ui.verticalSlider_FilterAttack.value()
        control = [CONTROLLER_CHANGE, 85, myVal] # Filter Attack
        midiOut.send_message(control)

        loghelperValInt = self.mapValInt(myVal,0,127,0,1000)
        loghelperValFloat=float(loghelperValInt)/1000.0
        loghelperValFloat= math.pow(loghelperValFloat,3)*10000
        if loghelperValFloat+1 > 10000:
            loghelperValFloat = 9999

        self.ui.label_FilterAttack.setText("%d" % int(loghelperValFloat+1))

        #def setFilterHold(self):
        myVal = self.ui.verticalSlider_FilterHold.value()
        control = [CONTROLLER_CHANGE, 86, myVal] # Filter Hold
        midiOut.send_message(control)

        loghelperValInt = self.mapValInt(myVal,0,127,0,1000)
        loghelperValFloat=float(loghelperValInt)/1000.0
        loghelperValFloat= math.pow(loghelperValFloat,3)*10000
        if loghelperValFloat+1 > 10000:
            loghelperValFloat = 9999

        self.ui.label_FilterHold.setText("%d" % int(loghelperValFloat+1))

        #def setFilterDecay(self):
        myVal = self.ui.verticalSlider_FilterDecay.value()
        control = [CONTROLLER_CHANGE, 87, myVal] # Filter Decay
        midiOut.send_message(control)

        loghelperValInt = self.mapValInt(myVal,0,127,0,1000)
        loghelperValFloat=float(loghelperValInt)/1000.0
        loghelperValFloat= math.pow(loghelperValFloat,3)*10000
        if loghelperValFloat+1 > 10000:
            loghelperValFloat = 9999

        self.ui.label_FilterDecay.setText("%d" % int(loghelperValFloat+1))

        #def setFilterSustain(self):
        myVal = self.ui.verticalSlider_FilterSustain.value()
        control = [CONTROLLER_CHANGE, 88, myVal] # Filter Sustain
        midiOut.send_message(control)

        self.ui.label_FilterSustain.setText("%d" % int(myVal))

        #def setFilterRelease(self):
        myVal = self.ui.verticalSlider_FilterRelease.value()
        control = [CONTROLLER_CHANGE, 89, myVal] # Filter Release
        midiOut.send_message(control)

        loghelperValInt = self.mapValInt(myVal,0,127,0,1000)
        loghelperValFloat=float(loghelperValInt)/1000.0
        loghelperValFloat= math.pow(loghelperValFloat,3)*10000
        if loghelperValFloat+1 > 10000:
            loghelperValFloat = 9999

        self.ui.label_FilterRelease.setText("%d" % int(loghelperValFloat+1))

        #def setGuitarStaticEnvelopeToFilter(self):
        myVal = self.ui.dial_GuitarFilterStaticVelocity.value()
        control = [CONTROLLER_CHANGE, 98, myVal] # Static Guitar Filter Envelope !! wir benutzen cc 98 hier wir sind nicht in der reihenvolge
        midiOut.send_message(control)

        if myVal == 0:
            self.ui.label_StaticOrDynamicEnvelope.setText("Dynamic Envelope")
            self.ui.label_GuitarFilterStaticVelocety.setText("-")
        else:
            self.ui.label_StaticOrDynamicEnvelope.setText("Static Envelope")
            self.ui.label_GuitarFilterStaticVelocety.setText("%d" % int(self.mapValInt(myVal,0,127,0,100)) )

        time.sleep(0.05)
        #################### MOD LFO ##############################################

        #def setModLFOWaveForm(self):
        myVal = self.ui.comboBox_ModLFOWafeForm.currentIndex()
        control = [CONTROLLER_CHANGE, 90, myVal] # MOD LFO waveForm
        midiOut.send_message(control)

        #def setModLFOPulseWide(self):
        myVal = self.ui.dial_ModLFOPulseWide.value()
        control = [CONTROLLER_CHANGE, 91, myVal] # MOD LFO PulseWide
        midiOut.send_message(control)
        loghelperValInt = self.mapValInt(myVal,0,127,1,100)
        self.ui.label_ModLfoPulseWide.setText("%d" % loghelperValInt)

        time.sleep(0.05)
        #def setModLFOBpm(self):
        myVal = self.ui.dialModLFOBPM.value()
        control = [CONTROLLER_CHANGE, 92, myVal ] # MOD LFO BPM
        midiOut.send_message(control)
        self.ui.label_ModLFOBPM.setText("%d" % self.mapValInt(myVal,0,127,0,360))

        #def setModLfoBpmMulti(self):
        myVal = self.ui.comboBoxModLfoMulti.currentIndex()
        control = [CONTROLLER_CHANGE, 93, myVal] # MOD LFO BPM Multi
        midiOut.send_message(control)

        #def setModLfoPhase(self):
        myVal = self.ui.dial_ModLFO_Phase.value()
        control = [CONTROLLER_CHANGE, 94, myVal] # MOD LFO Phase
        midiOut.send_message(control)

        myLableVal = self.mapValInt(myVal,0,127,0,360)
        self.ui.label_Mod_LFO_Phase.setText("%d" % (myLableVal))

        #def setModLfoSyncToAttack(self):
        myVal = self.ui.checkBoxModLfoSyncToAttack.checkState()
        #print("sync MOD to attack %d" % myVal)
        if myVal == 2:
            myVal = 1

        control = [CONTROLLER_CHANGE, 95, myVal] # MOD 2 Mode
        midiOut.send_message(control)

        #def setModLfoAttackToFreq(self):
        myVal = self.ui.dial_ModLfoATTCKtoFreq.value()
        control = [CONTROLLER_CHANGE, 96, myVal] # MOD LFO Attack Envelope to Freq.
        midiOut.send_message(control)

        self.ui.label_ModLfoAttackToFreq.setText("%d" % (self.mapValInt(myVal,0,127,0,30)))

        #def setModAttackenvelopeToFilterDepth(self):
        myVal = self.ui.dial_ModLfoATTCKtoDepth.value()
        control = [CONTROLLER_CHANGE, 97, myVal] # MOD LFO Attack Envelope to Depth.
        midiOut.send_message(control)
        if myVal ==  63 or myVal == 64:
            self.ui.label_ModLfoAttackToDepth.setText("%d" % (0))
        else:
            self.ui.label_ModLfoAttackToDepth.setText("%d" % (self.mapValInt(myVal,0,127,-100,100)))



        #def setModLfoTargetFreqOSC1(self):
        myVal = self.ui.verticalSlider_ModLfoTargetFrequencyToOSC1.value()
        control = [CONTROLLER_CHANGE, 99, myVal] # Mod LFO T1 Freq
        midiOut.send_message(control)

        mytacktackval = float(self.mapValInt( myVal, 0, 127, 0, 1000))/1000.0

        local_notes = 0.0
        local_cents = 0.0

        local_notes = 12*mytacktackval;
        local_cents = math.fmod( local_notes,1 )*100;

        self.ui.label_ModLfoTargetFrequencyToOSC1.setText("%d, %d" % (int(local_notes), int(local_cents)) )

        #def setModLfoTargetFreqOSC2(self):
        myVal = self.ui.verticalSlider_ModLfoTargetFrequencyToOSC2.value()
        control = [CONTROLLER_CHANGE, 100, myVal] # MOD LFO T2 Freq
        midiOut.send_message(control)

        mytacktackval = float(self.mapValInt( myVal, 0, 127, 0, 1000))/1000.0

        local_notes = 0.0
        local_cents = 0.0

        local_notes = 12*mytacktackval;
        local_cents = math.fmod( local_notes,1 )*100;

        self.ui.label_ModLfoTargetFrequencyToOSC2.setText("%d, %d" % (int(local_notes), int(local_cents)) )


        #def setModLfoTargetFreqSubOSC1(self):
        myVal = self.ui.verticalSlider_ModLfoTargetFrequencyToSubOSC1.value()
        control = [CONTROLLER_CHANGE, 101, myVal] # MOD LFO T3 Freq
        midiOut.send_message(control)

        mytacktackval = float(self.mapValInt( myVal, 0, 127, 0, 1000))/1000.0

        local_notes = 0.0
        local_cents = 0.0

        local_notes = 12*mytacktackval;
        local_cents = math.fmod( local_notes,1 )*100;

        self.ui.label_ModLfoTargetFrequencyToSubOSC1.setText("%d, %d" % (int(local_notes), int(local_cents)) )


        #def setModLfoTargetFreqSubOSC2(self):
        myVal = self.ui.verticalSlider_ModLfoTargetFrequencyToSubOSC2.value()
        control = [CONTROLLER_CHANGE, 102, myVal] # MOD LFO T4 Freq
        midiOut.send_message(control)

        mytacktackval = float(self.mapValInt( myVal, 0, 127, 0, 1000))/1000.0

        local_notes = 0.0
        local_cents = 0.0

        local_notes = 12*mytacktackval;
        local_cents = math.fmod( local_notes,1 )*100;

        self.ui.label_ModLfoTargetFrequencyToSubOSC2.setText("%d, %d" % (int(local_notes), int(local_cents)) )

        #def setModLfoTarget1to4FreqSync(self):
        myVal = self.ui.checkBoxModLfoSyncAllFreqencyTargets.checkState()
        if myVal == 2:
            myVal = 1
        control = [CONTROLLER_CHANGE, 103, myVal] # MOD LFO Sync T1-t4
        midiOut.send_message(control)

        #def setModLfoTarget5to8PulseSync(self):
        myVal = self.ui.checkBoxModLfoSyncAllPulseTargets.checkState()
        if myVal == 2:
            myVal = 1
        control = [CONTROLLER_CHANGE, 104, myVal] # MOD LFO Sync T5-t8
        midiOut.send_message(control)


        #def setModLfoTargetPulseOSC1(self):
        myVal = self.ui.verticalSlider_ModLfoTargetPulseToOSC1.value()
        control = [CONTROLLER_CHANGE, 105, myVal] # Mod LFO T1 Pulse
        midiOut.send_message(control)

        myTextVal = int(self.mapValInt(myVal, 0, 127, -100, 100))
        if myVal == 63 or myVal == 64:
            myTextVal = 0
        self.ui.label_ModLfoTargetPulseToOSC1.setText("%d" % myTextVal)

        #def setModLfoTargetPulseOSC2(self):
        myVal = self.ui.verticalSlider_ModLfoTargetPulseToOSC2.value()
        control = [CONTROLLER_CHANGE, 106, myVal] # MOD LFO T2 Pulse
        midiOut.send_message(control)

        myTextVal = int(self.mapValInt(myVal, 0, 127, -100, 100))
        if myVal == 63 or myVal == 64:
            myTextVal = 0
        self.ui.label_ModLfoTargetPulseToOSC2.setText("%d" % myTextVal)


        #def setModLfoTargetPulseSubOSC1(self):
        myVal = self.ui.verticalSlider_ModLfoTargetPulseToSubOSC1.value()
        control = [CONTROLLER_CHANGE, 107, myVal] # MOD LFO T3 Pulse
        midiOut.send_message(control)

        myTextVal = int(self.mapValInt(myVal, 0, 127, -100, 100))
        if myVal == 63 or myVal == 64:
            myTextVal = 0
        self.ui.label_ModLfoTargetPulseToSubOSC1.setText("%d" % myTextVal)


        #def setModLfoTargetPulseSubOSC2(self):
        myVal = self.ui.verticalSlider_ModLfoTargetPulseToSubOSC2.value()
        control = [CONTROLLER_CHANGE, 108, myVal] # MOD LFO T4 Pulse
        midiOut.send_message(control)

        myTextVal = int(self.mapValInt(myVal, 0, 127, -100, 100))
        if myVal == 63 or myVal == 64:
            myTextVal = 0
        self.ui.label_ModLfoTargetPulseToSubOSC2.setText("%d" % myTextVal)


        ##### Vol LFO #############################################################

        #def setVolLFOWaveForm(self):
        myVal = self.ui.comboBox_VolLFOWafeForm.currentIndex()
        control = [CONTROLLER_CHANGE, 109, myVal] # VOL LFO waveForm
        midiOut.send_message(control)

        #def setVolLFOPulseWide(self):
        myVal = self.ui.dial_VolLFOPulseWide.value()
        control = [CONTROLLER_CHANGE, 110, myVal] # VOL LFO PulseWide
        midiOut.send_message(control)
        loghelperValInt = self.mapValInt(myVal,0,127,1,100)
        self.ui.label_VolLfoPulseWide.setText("%d" % loghelperValInt)

        #def setVolLFOBpm(self):
        myVal = self.ui.dialVolLFOBPM.value()
        control = [CONTROLLER_CHANGE, 111, myVal ] # VOL LFO BPM
        midiOut.send_message(control)
        self.ui.label_VolLFOBPM.setText("%d" % self.mapValInt(myVal,0,127,0,360))

        #def setVolLfoBpmMulti(self):
        myVal = self.ui.comboBoxVolLfoMulti.currentIndex()
        control = [CONTROLLER_CHANGE, 112, myVal] # VOL LFO BPM Multi
        midiOut.send_message(control)

        #def setVolLfoPhase(self):
        myVal = self.ui.dial_VolLFO_Phase.value()
        control = [CONTROLLER_CHANGE, 113, myVal] # VOL LFO Phase
        midiOut.send_message(control)

        myLableVal = self.mapValInt(myVal,0,127,0,360)
        self.ui.label_Vol_LFO_Phase.setText("%d" % (myLableVal))

        #def setVolLfoSyncToAttack(self):
        myVal = self.ui.checkBoxVolLfoSyncToAttack.checkState()
        #print("sync VOL to attack %d" % myVal)
        if myVal == 2:
            myVal = 1

        control = [CONTROLLER_CHANGE, 114, myVal] # VOL 2 Mode
        midiOut.send_message(control)

        #def setVolLfoAttackToFreq(self):
        myVal = self.ui.dial_VolLfoATTCKtoFreq.value()
        control = [CONTROLLER_CHANGE, 115, myVal] # VOL LFO Attack Envelope to Freq.
        midiOut.send_message(control)

        self.ui.label_VolLfoAttackToFreq.setText("%d" % (self.mapValInt(myVal,0,127,0,30)))

        #def setVolAttackenvelopeToFilterDepth(self):
        myVal = self.ui.dial_VolLfoATTCKtoDepth.value()
        control = [CONTROLLER_CHANGE, 116, myVal] # VOL LFO Attack Envelope to Depth.
        midiOut.send_message(control)
        if myVal ==  63 or myVal == 64:
            self.ui.label_VolLfoAttackToDepth.setText("%d" % (0))
        else:
            self.ui.label_VolLfoAttackToDepth.setText("%d" % (self.mapValInt(myVal,0,127,-100,100)))

        #def setVolLfoTarget1to4Sync(self):
        myVal = self.ui.checkBoxVolLfoSyncAllTargets.checkState()
        if myVal == 2:
            myVal = 1
        control = [CONTROLLER_CHANGE, 117, myVal] # VOL LFO Sync T1-t4
        midiOut.send_message(control)

        time.sleep(0.05)

        #def setVolLfoTargetOSC1(self):
        myVal = self.ui.verticalSlider_VolLfoTargetToOSC1.value()
        control = [CONTROLLER_CHANGE, 118, myVal] # Vol LFO T1
        midiOut.send_message(control)

        myTextVal = int(self.mapValInt(myVal, 0, 127, -100, 100))
        if myVal == 63 or myVal == 64:
            myTextVal = 0
        self.ui.label_VolLfoTargetToOSC1.setText("%d" % myTextVal)

        #def setVolLfoTargetOSC2(self):
        myVal = self.ui.verticalSlider_VolLfoTargetToOSC2.value()
        control = [CONTROLLER_CHANGE, 119, myVal] # Vol LFO T2
        midiOut.send_message(control)

        myTextVal = int(self.mapValInt(myVal, 0, 127, -100, 100))
        if myVal == 63 or myVal == 64:
            myTextVal = 0
        self.ui.label_VolLfoTargetToOSC2.setText("%d" % myTextVal)


        #def setVolLfoTargetAllSubs(self):
        myVal = self.ui.verticalSlider_VolLfoTargetToAllSubs.value()
        control = [CONTROLLER_CHANGE, 120, myVal] # Vol LFO T3
        midiOut.send_message(control)

        myTextVal = int(self.mapValInt(myVal, 0, 127, -100, 100))
        if myVal == 63 or myVal == 64:
            myTextVal = 0
        self.ui.label_VolLfoTargetToAllSubs.setText("%d" % myTextVal)


        #def setVolLfoTargetNoise(self):
        myVal = self.ui.verticalSlider_VolLfoTargetToNoise.value()
        control = [CONTROLLER_CHANGE, 121, myVal] # MOD LFO T4 Pulse
        midiOut.send_message(control)

        myTextVal = int(self.mapValInt(myVal, 0, 127, 0, 100))
        if myVal == 63 or myVal == 64:
            myTextVal = 0
        self.ui.label_VolLfoTargetToNoise.setText("%d" % myTextVal)

        #def setExpressionPedal(self):
        myVal = self.ui.ExpPressionPedalComboBox.currentIndex()
        control = [CONTROLLER_CHANGE, 0, myVal]
        midiOut.send_message(control)

        #def setPoti(self):
        myVal = self.ui.PotiComboBox.currentIndex()
        control = [CONTROLLER_CHANGE, 1, myVal]
        midiOut.send_message(control)

        #def setFunctionBtn(self):
        myVal = self.ui.functionBtn_Combobox.currentIndex()
        control = [CONTROLLER_CHANGE, 2, myVal]
        midiOut.send_message(control)

        #def setInputGain(self):
        myVal = self.ui.InputGain_Dail.value()
        control = [CONTROLLER_CHANGE, 3, myVal] # input gain
        midiOut.send_message(control)
        self.ui.InputGain_Lcd.display(int(self.mapValInt( myVal,0,127,0,15)))

        ##### MISK SETTINGS #################

        #def setMidiPitchBendRange(self):
        myVal = self.ui.comboBox_MidiPitchBendRange.currentIndex()
        control = [CONTROLLER_CHANGE, 122, myVal] # Midi Pitch Bend Range
        midiOut.send_message(control)

        #def setAutoPeakCorrection(self):
        myVal = self.ui.checkBoxPeakCutoff.checkState()
        if myVal == 2:
            myVal = 1

        control = [CONTROLLER_CHANGE, 123, myVal] # Auto peak Correction
        midiOut.send_message(control)


        self.sendSequence();



###############################################################################################################################
################################################################################################################################

    ###### Program Change###################################
    def setProgramChange(self):
        myVal = self.ui.spinBox_Program.value()
        control = [PROGRAM_CHANGE, myVal] #
        midiOut.send_message(control)

    #### Expression Pedal ###################################
    def setExpressionPedal(self):
        myVal = self.ui.ExpPressionPedalComboBox.currentIndex()
        control = [CONTROLLER_CHANGE, 0, myVal]
        midiOut.send_message(control)

    #### Poti ###################################
    def setPoti(self):
        myVal = self.ui.PotiComboBox.currentIndex()
        control = [CONTROLLER_CHANGE, 1, myVal]
        midiOut.send_message(control)

    #### Function button ###################################
    def setFunctionBtn(self):
        myVal = self.ui.functionBtn_Combobox.currentIndex()
        control = [CONTROLLER_CHANGE, 2, myVal]
        midiOut.send_message(control)


    ##### MIXER #############################################
    def setInputGain(self):
        myVal = self.ui.InputGain_Dail.value()
        control = [CONTROLLER_CHANGE, 3, myVal] # input gain
        midiOut.send_message(control)
        self.ui.InputGain_Lcd.display(int(self.mapValInt( myVal,0,127,0,15)))

    def setInstrumentToSynth(self):
        myVal = self.ui.verticalSlider_InstrumentToSynth.value()
        control = [CONTROLLER_CHANGE, 4, myVal] #
        midiOut.send_message(control)

        self.ui.lcdNumber_InstrumentToSynth.display(int(self.mapValInt( myVal,0,127,0,100 )))

    def setUsb1ToSynth(self):
        myVal = self.ui.verticalSlider_Usb1ToSynth.value()
        control = [CONTROLLER_CHANGE, 5, myVal] #
        midiOut.send_message(control)

        self.ui.lcdNumber_Usb1ToSynth.display(int(self.mapValInt( myVal,0,127,0,100 )))

    def setSynthToOut1(self):
        myVal = self.ui.verticalSlider_SynthToOut1.value()
        control = [CONTROLLER_CHANGE, 6, myVal] #
        midiOut.send_message(control)

        self.ui.lcdNumber_SynthToOut1.display(int(self.mapValInt( myVal,0,127,0,100 )))

    def setSynthToOut2(self):
        myVal = self.ui.verticalSlider_SynthToOut2.value()
        control = [CONTROLLER_CHANGE, 7, myVal] #
        midiOut.send_message(control)

        self.ui.lcdNumber_SynthToOut2.display(int(self.mapValInt( myVal,0,127,0,100 )))

    def setSynthToUsbOut1(self):
        myVal = self.ui.verticalSlider_SynthToUsbOut1.value()
        control = [CONTROLLER_CHANGE, 8, myVal] #
        midiOut.send_message(control)

        self.ui.lcdNumber_SynthToUsbOut1.display(int(self.mapValInt( myVal,0,127,0,100 )))

    def setSynthToUsbOut2(self):
        myVal = self.ui.verticalSlider_SynthToUsbOut2.value()
        control = [CONTROLLER_CHANGE, 9, myVal] #
        midiOut.send_message(control)

        self.ui.lcdNumber_SynthToUsbOut2.display(int(self.mapValInt( myVal,0,127,0,100 )))

    def setInstrumentToUsbOut1(self):
        myVal = self.ui.verticalSlider_InstrumentToUsbOut1.value()
        control = [CONTROLLER_CHANGE, 10, myVal] #
        midiOut.send_message(control)

        self.ui.lcdNumber_InstrumentToUsbOut1.display(int(self.mapValInt( myVal,0,127,0,100 )))

    def setInstrumentToUsbOut2(self):
        myVal = self.ui.verticalSlider_InstrumentToUsbOut2.value()
        control = [CONTROLLER_CHANGE, 11, myVal] #
        midiOut.send_message(control)

        self.ui.lcdNumber_InstrumentToUsbOut2.display(int(self.mapValInt( myVal,0,127,0,100 )))

    def setUsbIn1ToOut1(self):
        myVal = self.ui.verticalSlider_UsbIn1ToOut1.value()
        control = [CONTROLLER_CHANGE, 12, myVal] #
        midiOut.send_message(control)

        self.ui.lcdNumber_UsbIn1ToOut1.display(int(self.mapValInt( myVal,0,127,0,100 )))

    def setUsbIn2ToOut1(self):
        myVal = self.ui.verticalSlider_UsbIn2ToOut1.value()
        control = [CONTROLLER_CHANGE, 13, myVal] #
        midiOut.send_message(control)

        self.ui.lcdNumber_UsbIn2ToOut1.display(int(self.mapValInt( myVal,0,127,0,100 )))

    def setUsbIn1ToOut2(self):
        myVal = self.ui.verticalSlider_UsbIn1ToOut2.value()
        control = [CONTROLLER_CHANGE, 14, myVal] #
        midiOut.send_message(control)

        self.ui.lcdNumber_UsbIn1ToOut2.display(int(self.mapValInt( myVal,0,127,0,100 )))

    def setUsbIn2ToOut2(self):
        myVal = self.ui.verticalSlider_UsbIn2ToOut2.value()
        control = [CONTROLLER_CHANGE, 15, myVal] #
        midiOut.send_message(control)

        self.ui.lcdNumber_UsbIn2ToOut2.display(int(self.mapValInt( myVal,0,127,0,100 )))


    ################ D-F-F ####################
    def setFilter1DFF(self):
        myVal = self.ui.comboBox_activeTrackingFilter1.currentIndex()
        control = [CONTROLLER_CHANGE, 16, myVal] # Filter 1 DFF
        midiOut.send_message(control)
        #self.requestMidiSettings()

    def setFilter2DFF(self):
        myVal = self.ui.comboBox_activeTrackingFilter2.currentIndex()
        control = [CONTROLLER_CHANGE, 17, myVal] # Filter 2 DFF
        midiOut.send_message(control)

    ################ LFO Cycles ###############

    def setFilterLfoCycles(self):
        myVal = self.ui.comboBox_FilterLfoCycles.currentIndex()
        control = [CONTROLLER_CHANGE, 18, myVal] # Filter LFO Cycles
        midiOut.send_message(control)

    def setModLfoCycles(self):
        myVal = self.ui.comboBox_ModLfoCycles.currentIndex()
        control = [CONTROLLER_CHANGE, 19, myVal] # Mod LFO Cycles
        midiOut.send_message(control)


    ################ OSC Gain #################
    def dailOscMainGainEvent(self):
        myVal = self.ui.dial_OSCS_Main_Mixer.value()
        control = [CONTROLLER_CHANGE, 20, myVal] # filter 1 frequency
        midiOut.send_message(control)


    ################ Filter HighCut ###########
    def dailHighCutFreqEvent(self):
        #print("pups")
        myVal = self.ui.dial_highcut_filter.value()
        control = [(CONTROLLER_CHANGE & 0xf0) | ((2) - 1 & 0xf), 1, myVal]
        midiOut.send_message(control)

        loghelperValInt = self.mapValInt( myVal,0,127,0,1000 )
        loghelperValFloat=float(loghelperValInt)/1000.0
        loghelperValFloat= (math.pow(loghelperValFloat,5)*20000)+10.0
        self.ui.label_value_highcut.setText("%.0f" % loghelperValFloat)

    def dailHcqEvent(self):
        #print("pups2")
        myVal = self.ui.dial_hcq.value()
        control = [(CONTROLLER_CHANGE & 0xf0) | ((2) - 1 & 0xf), 2, myVal]
        midiOut.send_message(control)

        loghelperValInt = self.mapValInt( myVal*100,0,12700,710,25000 )
        loghelperValFloat = loghelperValInt/1000.0

        self.ui.label_value_hcq.setText("%.2f" % loghelperValFloat)

    ################ Filter LowCut ###########
    def dailLowCutFreqEvent(self):
        #print("pups3")
        myVal = self.ui.dial_lowcutfilter.value()
        control = [(CONTROLLER_CHANGE & 0xf0) | ((2) - 1 & 0xf), 3, myVal]
        midiOut.send_message(control)

        loghelperValInt = self.mapValInt( myVal,0,127,0,1000 )
        loghelperValFloat=float(loghelperValInt)/1000.0
        loghelperValFloat= (math.pow(loghelperValFloat,5)*20000)+10.0
        self.ui.label_value_lowcut.setText("%.0f" % loghelperValFloat)

    def dailLcqEvent(self):
        #print("pups4")
        myVal = self.ui.dial_lcq.value()
        control = [(CONTROLLER_CHANGE & 0xf0) | ((2) - 1 & 0xf), 4, myVal]
        midiOut.send_message(control)

        loghelperValInt = self.mapValInt( myVal*100,0,12700,710,25000 )
        loghelperValFloat = loghelperValInt/1000.0

        self.ui.label_value_lcq.setText("%.2f" % loghelperValFloat)




    ################## Delay #####################

    def DelayDryMixEvent(self):
        myVal = self.ui.Delay_Dry_Dail.value()
        control = [(CONTROLLER_CHANGE & 0xf0) | ((2) - 1 & 0xf), 5, myVal]
        midiOut.send_message(control)

        loghelperValInt = self.mapValInt(myVal,0,127,0,100)

        self.ui.Delay1_Dry_Lable.setText("%d" % loghelperValInt)

    def DelayWetMixEvent(self):
        myVal = self.ui.Delay_Wet_Dail.value()
        control = [(CONTROLLER_CHANGE & 0xf0) | ((2) - 1 & 0xf), 6, myVal]
        midiOut.send_message(control)

        loghelperValInt = self.mapValInt(myVal,0,127,0,100)

        self.ui.Delay1_Wet_Lable.setText("%d" % loghelperValInt)

    def DelayTimeMsEvent(self):
        myVal = self.ui.Delay_TimeMs_SpinBox.value()
        loghelperValInt = self.mapValInt(myVal,0,1800,0,127)
        control = [(CONTROLLER_CHANGE & 0xf0) | ((2) - 1 & 0xf), 7, loghelperValInt]
        midiOut.send_message(control)

    def DelayTimeSigEvent(self):
        myVal = self.ui.comboBox_Delay_Time_Signature.currentIndex()
        control = [(CONTROLLER_CHANGE & 0xf0) | ((2) - 1 & 0xf), 8, myVal]
        midiOut.send_message(control)

    def DelayGainEvent(self):
        myVal = self.ui.Delay_Gain_Dail.value()
        control = [(CONTROLLER_CHANGE & 0xf0) | ((2) - 1 & 0xf), 9, myVal]
        midiOut.send_message(control)

        loghelperValInt = self.mapValInt(myVal,0,127,0,100)

        self.ui.Delay1_Gain_Lable.setText("%d" % loghelperValInt)

    def DelayFeedbackEvent(self):
        myVal = self.ui.Delay_FeedBack_Dail.value()
        control = [(CONTROLLER_CHANGE & 0xf0) | ((2) - 1 & 0xf), 10, myVal]
        midiOut.send_message(control)

        loghelperValInt = self.mapValInt(myVal,0,127,0,120)

        self.ui.Delay1_Feetback_Lable.setText("%d" % loghelperValInt)



    ################ FILTER 1 #################
    # Filter 1 Frequency (CC 21)
    def dailFilter1FreqEvent(self):
        #print "radstand %d" % self.ui.dial_f1_f.value()
        myVal = self.ui.dial_f1_f.value()
        control = [CONTROLLER_CHANGE, 21, myVal] # filter 1 frequency
        midiOut.send_message(control)

        if self.ui.comboBox_activeTrackingFilter1.currentIndex() > 0:
            loghelperValInt = self.mapValInt( myVal,0,127,-60,60)
            self.ui.lcdNumber.display(int(loghelperValInt))
        else:
            loghelperValInt = self.mapValInt( myVal,0,127,0,1000 )
            loghelperValFloat=float(loghelperValInt)/1000.0
            loghelperValFloat= (math.pow(loghelperValFloat,5)*20000)+10.0
            self.ui.lcdNumber.display(int(loghelperValFloat))

    def dailFilter1ResonaceEvent(self):
        myVal = self.ui.dial_f1_r.value()
        #print "radstand %d" % self.ui.dial_f1_f.value()
        control = [CONTROLLER_CHANGE, 22, myVal] # filter 1 frequency
        midiOut.send_message(control)

        loghelperValInt = self.mapValInt( myVal*100,0,12700,710,25000 )
        loghelperValFloat = loghelperValInt/1000.0

        self.ui.lcdNumber_2.display(loghelperValFloat)

    def dailFilter1octaveEvent(self):
        myVal = self.ui.dial_f1_o.value()
        control = [CONTROLLER_CHANGE, 23, myVal] # filter 1 frequency
        midiOut.send_message(control)

        loghelperValInt = self.mapValInt( myVal*100,0,12700,0,7000 )
        loghelperValFloat = loghelperValInt/1000.0

        self.ui.lcdNumber_3.display(loghelperValFloat)


    ################ FILTER 2 #################
    # Filter 2 Frequency (CC 24)
    def dailFilter2FreqEvent(self):
        myVal = self.ui.dial_f2_f.value()
        control = [CONTROLLER_CHANGE, 24, myVal] # filter 2 frequency
        midiOut.send_message(control)

        if self.ui.comboBox_activeTrackingFilter2.currentIndex() > 0:
            loghelperValInt = self.mapValInt( myVal,0,127,-60,60)
            self.ui.lcdNumber_4.display(int(loghelperValInt))
        else:
            loghelperValInt = self.mapValInt( myVal,0,127,0,1000 )
            loghelperValFloat=float(loghelperValInt)/1000.0
            loghelperValFloat= (math.pow(loghelperValFloat,5)*20000)+10.0
            self.ui.lcdNumber_4.display(int(loghelperValFloat))

    def dailFilter2ResonaceEvent(self):
        myVal = self.ui.dial_f2_r.value()
        #print "radstand %d" % self.ui.dial_f1_f.value()
        control = [CONTROLLER_CHANGE, 25, myVal] # filter 2 reso
        midiOut.send_message(control)

        loghelperValInt = self.mapValInt( myVal*100,0,12700,710,25000 )
        loghelperValFloat = loghelperValInt/1000.0

        self.ui.lcdNumber_5.display(loghelperValFloat)

    def dailFilter2octaveEvent(self):
        myVal = self.ui.dial_f2_o.value()
        control = [CONTROLLER_CHANGE, 26, self.ui.dial_f2_o.value()] # filter 2 octave
        midiOut.send_message(control)

        loghelperValInt = self.mapValInt( myVal*100,0,12700,0,7000 )
        loghelperValFloat = loghelperValInt/1000.0

        self.ui.lcdNumber_6.display(loghelperValFloat)

    def setFilter2Mode(self):

         myVal = self.ui.comboBox_f2_type.currentIndex() +1
         #print (myVal)
         control = [CONTROLLER_CHANGE, 27, myVal] # filter 2 Mode
         midiOut.send_message(control)

    def setFilter1to2Blend(self):

        myVal =  self.ui.horizontalSlider_Filter_Mixer.value()
        #print("blend %d" % myVal)
        control = [CONTROLLER_CHANGE, 28, myVal] # filter 2 Mode
        midiOut.send_message(control)

        loghelperValInt = self.mapValInt( myVal*100,0,12700,-500,500)
        #print("blend mitte %d" % loghelperValInt)
        myKopfKnackerLeft = 0
        myKopfKnackerRight = 0
        if loghelperValInt < 0:
            myKopfKnackerLeft = 500 + math.fabs(loghelperValInt)
            myKopfKnackerRight = 500 - math.fabs(loghelperValInt)

        if loghelperValInt > 0:
            myKopfKnackerLeft = 500 - math.fabs(loghelperValInt)
            myKopfKnackerRight = 500 + math.fabs(loghelperValInt)

        self.ui.lcdNumber_7.display(int(myKopfKnackerLeft/10))
        self.ui.lcdNumber_8.display(int(myKopfKnackerRight/10))
        #print("blend letzte links %d" % myKopfKnackerLeft)
        #print("blend letzte rechts %d" % myKopfKnackerRight)

    def setFilter1to2Crossover(self):
        myVal = self.ui.checkBoxF1_F2crossOver.checkState()
        #print("crossover %d" % myVal)
        if myVal == 2:
            myVal = 1

        control = [CONTROLLER_CHANGE, 29, myVal] # filter 2 Mode
        midiOut.send_message(control)

    ################### Guitar Sensing ##########################

    def setGuitarSensattack(self):
        myVal = self.ui.verticalSliderGitarSensingAttack.value()
        control = [CONTROLLER_CHANGE, 30, myVal] # Guitar Sensing Attack
        midiOut.send_message(control)

        loghelperValInt = self.mapValInt(myVal,0,127,0,1000)
        loghelperValFloat=float(loghelperValInt)/1000.0
        loghelperValFloat= math.pow(loghelperValFloat,5)*3000
        self.ui.label_GitSensAttack.setText("%d" % int(loghelperValFloat+1.0))


    def setGuitarSensRelease(self):
        myVal = self.ui.dial_23GitSensRelease.value()
        control = [CONTROLLER_CHANGE, 31, myVal] # Guitar Sensing Release
        midiOut.send_message(control)

        loghelperValInt = self.mapValInt(myVal,0,127,0,100)
        self.ui.lcdNumber_13.display(loghelperValInt)


    def setGuitarSensNOC(self):
        myVal = self.ui.dial_24GuitarSensNoc.value()
        control = [CONTROLLER_CHANGE, 32, myVal] # Guitar Sensing NOC
        midiOut.send_message(control)

        loghelperValInt = self.mapValInt(myVal,0,127,0,1000)
        loghelperValFloat=float(loghelperValInt)/1000.0
        loghelperValFloat= math.pow(loghelperValFloat,5)*8000
        self.ui.lcdNumber_14_GuitSensNoc.display(int(loghelperValFloat))

    def setGuitarSensPitchDetectTreshold(self):
        myVal = self.ui.dial_26PitchDetectTreshold.value()
        control = [CONTROLLER_CHANGE, 33, myVal] # Guitar Sensing Pitch Detect treshold
        midiOut.send_message(control)

        loghelperValInt = self.mapValInt(myVal,0,127,0,1000)
        loghelperValFloat=float(loghelperValInt)/1000.0
        loghelperValFloat= math.pow(loghelperValFloat,5)*25000
        self.ui.lcdNumber_15PitchDetectTreshold.display(int(loghelperValFloat))

    def setGuitarSensPitchDetectQuality(self):
        myVal = self.ui.dial_25PitchTedectQuality.value()
        control = [CONTROLLER_CHANGE, 34, myVal] # Guitar Sensing Pitch Quality
        midiOut.send_message(control)

        loghelperValInt = self.mapValInt(myVal,0,127,0,99)
        self.ui.lcdNumber_16PitchTedectQuality.display(loghelperValInt)

    def resetGuitarSenseDefaults(self):

        # muss noch überlegt werden ob git sens attack auch default gesetzt wird
        #self.main_app.verticalSliderGitarSensingAttack.setValue(0) #1ms
        #control = [CONTROLLER_CHANGE, 30, 0]
        #midiOut.send_message(control)

        # Guitar Sensing Release
        self.ui.dial_23GitSensRelease.setValue(64) #503
        control = [CONTROLLER_CHANGE, 31, 64]
        midiOut.send_message(control)

        # Guitar Sensing NOC
        self.ui.dial_24GuitarSensNoc.setValue(77) #653
        control = [CONTROLLER_CHANGE, 32, 77]
        midiOut.send_message(control)

        # Guitar Sensing Pitch detect treshold
        self.ui.dial_26PitchDetectTreshold.setValue(57) #90
        control = [CONTROLLER_CHANGE, 33, 57]
        midiOut.send_message(control)

        # Guitar Sensing Pitch detect quality
        self.ui.dial_25PitchTedectQuality.setValue(106) # 0.826
        control = [CONTROLLER_CHANGE, 34, 106]
        midiOut.send_message(control)

    ################### OSC 1 #################################################
    def setOSC1Volumen(self):
        myVal = self.ui.dialOSC1Volume.value()
        control = [CONTROLLER_CHANGE, 35, myVal] # OSC1 Volumen
        midiOut.send_message(control)
        loghelperValInt = self.mapValInt(myVal,0,127,0,100)
        self.ui.lcdNumber_9OSC1Volume.display(loghelperValInt)

    def setOSC1Waveform(self):
        myVal = self.ui.comboBox_OSC1Waveform.currentIndex()
        control = [CONTROLLER_CHANGE, 36, myVal] # OSC1 Waveform
        midiOut.send_message(control)

    def setOSC1PulseWide(self):
         myVal = self.ui.dial_5OSC1_PulseWide.value()
         control = [CONTROLLER_CHANGE, 37, myVal] # OSC1 PulseWide
         midiOut.send_message(control)
         loghelperValInt = self.mapValInt(myVal,0,127,1,100)
         self.ui.label_23OSC1_PulseWide.setText("%d" % loghelperValInt)

    def setOSC1Octave(self):
        myVal = self.ui.dial_2OSC1Octave.value()
        control = [CONTROLLER_CHANGE, 38, myVal] # OSC1 Octave
        midiOut.send_message(control)

        self.ui.label_16OSC1Octave.setText("%d" % (myVal - 3))

    def setOSC1Note(self):
        myVal = self.ui.dial_3OSC1Note.value()
        control = [CONTROLLER_CHANGE, 39, myVal] # OSC1 Octave
        midiOut.send_message(control)

        self.ui.label_17OSC1Note.setText("%d" % (myVal - 12))

    def setOSC1Detune(self):
        myVal = self.ui.dial_4OSC1Cents.value()
        control = [CONTROLLER_CHANGE, 40, myVal] # OSC1 Detune cents
        midiOut.send_message(control)

        self.ui.label_18OSC1Cents.setText("%d" % (self.mapValInt(myVal,0,24,-100,100)))

    def setOSC1Phase(self):
        myVal = self.ui.dial_5OSC1_Phase.value()
        control = [CONTROLLER_CHANGE, 41, myVal] # OSC1 Phase
        midiOut.send_message(control)

        myLableVal = self.mapValInt(myVal,0,127,0,360)
        self.ui.label_18OSC1Phase.setText("%d" % (myLableVal))

    ################### OSC 2 #################################################
    def setOSC2Volumen(self):
        myVal = self.ui.dialOSC2Volume.value()
        control = [CONTROLLER_CHANGE, 42, myVal] # OSC2 Volumen
        midiOut.send_message(control)
        loghelperValInt = self.mapValInt(myVal,0,127,0,100)
        self.ui.lcdNumber_9OSC2Volume.display(loghelperValInt)

    def setOSC2Waveform(self):
        myVal = self.ui.comboBox_OSC2Waveform.currentIndex()
        control = [CONTROLLER_CHANGE, 43, myVal] # OSC2 Waveform
        midiOut.send_message(control)

    def setOSC2PulseWide(self):
         myVal = self.ui.dial_5OSC2_PulseWide.value()
         control = [CONTROLLER_CHANGE, 44, myVal] # OSC2 PulseWide
         midiOut.send_message(control)
         loghelperValInt = self.mapValInt(myVal,0,127,1,100)
         self.ui.label_23OSC2_PulseWide.setText("%d" % loghelperValInt)

    def setOSC2Octave(self):
        myVal = self.ui.dial_2OSC2Octave.value()
        control = [CONTROLLER_CHANGE, 45, myVal] # OSC2 Octave
        midiOut.send_message(control)

        self.ui.label_16OSC2Octave.setText("%d" % (myVal - 3))

    def setOSC2Note(self):
        myVal = self.ui.dial_3OSC2Note.value()
        control = [CONTROLLER_CHANGE, 46, myVal] # OSC2 Octave
        midiOut.send_message(control)

        self.ui.label_17OSC2Note.setText("%d" % (myVal - 12))

    def setOSC2Detune(self):
        myVal = self.ui.dial_4OSC2Cents.value()
        control = [CONTROLLER_CHANGE, 47, myVal] # OSC2 Detune cents
        midiOut.send_message(control)

        self.ui.label_18OSC2Cents.setText("%d" % (self.mapValInt(myVal,0,24,-100,100)))

    def setOSC2Phase(self):
        myVal = self.ui.dial_5OSC2_Phase.value()
        control = [CONTROLLER_CHANGE, 48, myVal] # OSC2 Phase
        midiOut.send_message(control)

        myLableVal = self.mapValInt(myVal,0,127,0,360)
        self.ui.label_18OSC2Phase.setText("%d" % (myLableVal))


    ################### SUB OSC 1 #################################################
    def setSUBOSC1Volumen(self):
        myVal = self.ui.dialSUBOSC1Volume.value()
        control = [CONTROLLER_CHANGE, 49, myVal] # SUBOSC1 Volumen
        midiOut.send_message(control)
        loghelperValInt = self.mapValInt(myVal,0,127,0,100)
        self.ui.lcdNumber_9SUBOSC1Volume.display(loghelperValInt)

    def setSUBOSC1Waveform(self):
        myVal = self.ui.comboBox_SUBOSC1Waveform.currentIndex()
        control = [CONTROLLER_CHANGE, 50, myVal] # SUBOSC1 Waveform
        midiOut.send_message(control)

    def setSUBOSC1PulseWide(self):
         myVal = self.ui.dial_5SUBOSC1_PulseWide.value()
         control = [CONTROLLER_CHANGE, 51, myVal] # SUBOSC1 PulseWide
         midiOut.send_message(control)
         loghelperValInt = self.mapValInt(myVal,0,127,1,100)
         self.ui.label_23SUBOSC1_PulseWide.setText("%d" % loghelperValInt)

    def setSUBOSC1Octave(self):
        myVal = self.ui.dial_2SUBOSC1Octave.value()
        control = [CONTROLLER_CHANGE, 52, myVal] # SUBOSC1 Octave
        midiOut.send_message(control)

        self.ui.label_16SUBOSC1Octave.setText("%d" % (myVal - 3))

    def setSUBOSC1Note(self):
        myVal = self.ui.dial_3SUBOSC1Note.value()
        control = [CONTROLLER_CHANGE, 53, myVal] # SUBOSC1 Octave
        midiOut.send_message(control)

        self.ui.label_17SUBOSC1Note.setText("%d" % (myVal - 12))

    def setSUBOSC1Detune(self):
        myVal = self.ui.dial_4SUBOSC1Cents.value()
        control = [CONTROLLER_CHANGE, 54, myVal] # SUBOSC1 Detune cents
        midiOut.send_message(control)

        self.ui.label_18SUBOSC1Cents.setText("%d" % (self.mapValInt(myVal,0,24,-100,100)))

    def setSUBOSC1Phase(self):
        myVal = self.ui.dial_5SUBOSC1_Phase.value()
        control = [CONTROLLER_CHANGE, 55, myVal] # SUBOSC1 Phase
        midiOut.send_message(control)

        myLableVal = self.mapValInt(myVal,0,127,0,360)
        self.ui.label_18SUBOSC1Phase.setText("%d" % (myLableVal))

    ################### SUB OSC 2 #################################################
    def setSUBOSC2Volumen(self):
        myVal = self.ui.dialSUBOSC2Volume.value()
        control = [CONTROLLER_CHANGE, 56, myVal] # SUBOSC2 Volumen
        midiOut.send_message(control)
        loghelperValInt = self.mapValInt(myVal,0,127,0,100)
        self.ui.lcdNumber_9SUBOSC2Volume.display(loghelperValInt)

    def setSUBOSC2Waveform(self):
        myVal = self.ui.comboBox_SUBOSC2Waveform.currentIndex()
        control = [CONTROLLER_CHANGE, 57, myVal] # SUBOSC2 Waveform
        midiOut.send_message(control)

    def setSUBOSC2PulseWide(self):
         myVal = self.ui.dial_5SUBOSC2_PulseWide.value()
         control = [CONTROLLER_CHANGE, 58, myVal] # SUBOSC2 PulseWide
         midiOut.send_message(control)
         loghelperValInt = self.mapValInt(myVal,0,127,1,100)
         self.ui.label_23SUBOSC2_PulseWide.setText("%d" % loghelperValInt)

    def setSUBOSC2Octave(self):
        myVal = self.ui.dial_2SUBOSC2Octave.value()
        control = [CONTROLLER_CHANGE, 59, myVal] # SUBOSC2 Octave
        midiOut.send_message(control)

        self.ui.label_16SUBOSC2Octave.setText("%d" % (myVal - 3))

    def setSUBOSC2Note(self):
        myVal = self.ui.dial_3SUBOSC2Note.value()
        control = [CONTROLLER_CHANGE, 60, myVal] # SUBOSC2 Note
        midiOut.send_message(control)

        self.ui.label_17SUBOSC2Note.setText("%d" % (myVal - 12))

    def setSUBOSC2Detune(self):
        myVal = self.ui.dial_4SUBOSC2Cents.value()
        control = [CONTROLLER_CHANGE, 61, myVal] # SUBOSC2 Detune cents
        midiOut.send_message(control)

        self.ui.label_18SUBOSC2Cents.setText("%d" % (self.mapValInt(myVal,0,24,-100,100)))

    def setSUBOSC2Phase(self):
        myVal = self.ui.dial_5SUBOSC2_Phase.value()
        control = [CONTROLLER_CHANGE, 62, myVal] # SUBOSC2 Phase
        midiOut.send_message(control)

        myLableVal = self.mapValInt(myVal,0,127,0,360)
        self.ui.label_18SUBOSC2Phase.setText("%d" % (myLableVal))

    #################### Noise Volumen #########################################

    def setNoiseVolumen(self):
        myVal = self.ui.dail_NoiseVolumen.value()

        control = [CONTROLLER_CHANGE, 63, myVal] # Noise Volumen
        midiOut.send_message(control)
        loghelperValInt = self.mapValInt(myVal,0,127,0,100)
        self.ui.label_41NoiseVolumen.setText("%d" % (loghelperValInt))

    #################### Bit Crusher ###########################################

    def setBitCrusherVolumen(self):
        myVal = self.ui.dial_3BitcrusherVolumen.value()

        control = [CONTROLLER_CHANGE, 64, myVal] # Bit Crusher Volumen
        midiOut.send_message(control)
        loghelperValInt = self.mapValInt(myVal,0,127,0,100)
        self.ui.label_43BitcrusherVolumen.setText("%d" % (loghelperValInt))

    def setBitcrusherBits(self):
        myVal = self.ui.comboBoxBicrusherBits.currentIndex()

        control = [CONTROLLER_CHANGE, 65, myVal] # Bit Crusher Bits
        midiOut.send_message(control)

    def setBitcrusherRate(self):
        myVal = self.ui.comboBox_2BitcrusherSampleRate.currentIndex()

        control = [CONTROLLER_CHANGE, 66, myVal] # Bit Crusher Rate
        midiOut.send_message(control)

    ###################### Filter LFO ##########################################

    def setFilterLFOWaveForm(self):
        myVal = self.ui.comboBox_5FilterLFOWafeForm.currentIndex()
        control = [CONTROLLER_CHANGE, 67, myVal] # Filter LFO waveForm
        midiOut.send_message(control)

        #myLocalFilterLfoWaveform = myVal
        if myVal < 6:
            self.ui.comboBox_FilterLfoCycles.setVisible(False)
        else:
            self.ui.comboBox_FilterLfoCycles.setVisible(True)

    def setFilterLFOPulseWide(self):
        myVal = self.ui.dial_21FilterLFOPulseWide.value()
        control = [CONTROLLER_CHANGE, 68, myVal] # Filter LFO PulseWide
        midiOut.send_message(control)
        loghelperValInt = self.mapValInt(myVal,0,127,1,100)
        self.ui.label_68FilterLfoPulseWide.setText("%d" % loghelperValInt)

    def setFilterLFOBpm(self):
        myVal = self.ui.dialFilterLFOBPM.value()
        control = [CONTROLLER_CHANGE, 69, myVal ] # Filter LFO BPM
        midiOut.send_message(control)
        self.ui.label_18FilterLFOBPM.setText("%d" % self.mapValInt(myVal,0,127,0,360))

    def setFilterLfoBpmMulti(self):
        myVal = self.ui.comboBox_6FilterLFOBpmMultip.currentIndex()
        control = [CONTROLLER_CHANGE, 70, myVal] # Filter LFO BPM Multi
        midiOut.send_message(control)

    def setFilterLfoPhase(self):
        myVal = self.ui.dial_FilterLFO_Phase.value()
        control = [CONTROLLER_CHANGE, 71, myVal] # Filter LFO Phase
        midiOut.send_message(control)

        myLableVal = self.mapValInt(myVal,0,127,0,360)
        self.ui.label_Filter_LFO_Phase.setText("%d" % (myLableVal))

    def setFilterLfoSyncToAttack(self):
        myVal = self.ui.checkBoxFilterLfoSyncToAttack.checkState()
        #print("sync filter to attack %d" % myVal)
        if myVal == 2:
            myVal = 1

        control = [CONTROLLER_CHANGE, 72, myVal] # filter 2 Mode
        midiOut.send_message(control)


    def setFilterLfoDepth(self):
        myVal = self.ui.dial_FilterLfoDepth.value()
        control = [CONTROLLER_CHANGE, 73, myVal] # Filter LFO Depth
        midiOut.send_message(control)

        myLableVal = self.mapValInt(myVal,0,127,-100,100)
        if myVal == 63 or myVal == 64:
            self.ui.label_FilterLfoDepth.setText("%d" % (0))
        else:
            self.ui.label_FilterLfoDepth.setText("%d" % (myLableVal))

    def setFilterLfoAttackToFreq(self):
        myVal = self.ui.dial_FilterLfoATTCKtoFreq.value()
        control = [CONTROLLER_CHANGE, 74, myVal] # Filter LFO Attack Envelope to Freq.
        midiOut.send_message(control)

        self.ui.label_FilterLfoAttackToFreq.setText("%d" % (self.mapValInt(myVal,0,127,0,30)))

    def setFilterAttackenvelopeToFilterDepth(self):
        myVal = self.ui.dial_FilterLfoATTCKtoDepth.value()
        control = [CONTROLLER_CHANGE, 75, myVal] # Filter LFO Attack Envelope to Depth.
        midiOut.send_message(control)
        if myVal ==  63 or myVal == 64:
            self.ui.label_FilterLfoAttackToDepth.setText("%d" % (0))
        else:
            self.ui.label_FilterLfoAttackToDepth.setText("%d" % (self.mapValInt(myVal,0,127,-100,100)))

    def setEnvelopeAttackenvelopeToFilterCuttoff(self):
        myVal = self.ui.dialAttackEnvelopeFilterCut.value()
        control = [CONTROLLER_CHANGE, 76, myVal] # Filter LFO Attack Envelope to Freq.
        midiOut.send_message(control)

        if myVal ==  63 or myVal == 64:
            self.ui.labelEnvelopeFilterCut.setText("%d" % (0))
        else:
            self.ui.labelEnvelopeFilterCut.setText("%d" % (self.mapValInt(myVal,0,127,-100,100)))

    def setGuitarUseFilterDAHDSR(self):
        myVal = self.ui.checkBox_GuitarUseFilterDAHDSR.checkState()
        if myVal == 2:
            myVal = 1

        control = [CONTROLLER_CHANGE, 77, myVal] # Guitar use filter DAHDSR
        midiOut.send_message(control)

    def setGuitarUseEnvelopeAHDSR(self):
        myVal = self.ui.checkBoxGuitarUseEnvelopeAHDSR.checkState()
        if myVal == 2:
            myVal = 1

        control = [CONTROLLER_CHANGE, 78, myVal] # Guitar use Envelope DAHDSR
        midiOut.send_message(control)

    #################### Envelope AHDSR #######################################
    def setEnvelopeAttack(self):
        myVal = self.ui.verticalSlider_envelopeAttack.value()
        control = [CONTROLLER_CHANGE, 79, myVal] # Attack Envelope
        midiOut.send_message(control)

        loghelperValInt = self.mapValInt(myVal,0,127,0,1000)
        loghelperValFloat=float(loghelperValInt)/1000.0
        loghelperValFloat= math.pow(loghelperValFloat,3)*10000
        if loghelperValFloat+1 > 10000:
            loghelperValFloat = 9999

        self.ui.label_EnvelopeAttack.setText("%d" % int(loghelperValFloat+1))

    def setEnvelopeHold(self):
        myVal = self.ui.verticalSlider_envelopeHold.value()
        control = [CONTROLLER_CHANGE, 80, myVal] # Hold Envelope
        midiOut.send_message(control)

        loghelperValInt = self.mapValInt(myVal,0,127,0,1000)
        loghelperValFloat=float(loghelperValInt)/1000.0
        loghelperValFloat= math.pow(loghelperValFloat,3)*10000
        if loghelperValFloat+1 > 10000:
            loghelperValFloat = 9999

        self.ui.label_EnvelopeHold.setText("%d" % int(loghelperValFloat+1))

    def setEnvelopeDecay(self):
        myVal = self.ui.verticalSlider_envelopeDecay.value()
        control = [CONTROLLER_CHANGE, 81, myVal] # Decay Envelope
        midiOut.send_message(control)

        loghelperValInt = self.mapValInt(myVal,0,127,0,1000)
        loghelperValFloat=float(loghelperValInt)/1000.0
        loghelperValFloat= math.pow(loghelperValFloat,3)*10000
        if loghelperValFloat+1 > 10000:
            loghelperValFloat = 9999

        self.ui.label_EnvelopeDecay.setText("%d" % int(loghelperValFloat+1))

    def setEnvelopeSustain(self):
        myVal = self.ui.verticalSlider_envelopeSustain.value()
        control = [CONTROLLER_CHANGE, 82, myVal] # sustain envelope
        midiOut.send_message(control)

        self.ui.label_EnvelopeSustain.setText("%d" % int(myVal))

    def setEnvelopeRelease(self):
        myVal = self.ui.verticalSlider_envelopeRelease.value()
        control = [CONTROLLER_CHANGE, 83, myVal] # release Envelope
        midiOut.send_message(control)

        loghelperValInt = self.mapValInt(myVal,0,127,0,1000)
        loghelperValFloat=float(loghelperValInt)/1000.0
        loghelperValFloat= math.pow(loghelperValFloat,3)*10000
        if loghelperValFloat+1 > 10000:
            loghelperValFloat = 9999

        self.ui.label_EnvelopeRelease.setText("%d" % int(loghelperValFloat+1))

    #################### Filter ADSR ##########################################

    def setFilterDelay(self):
        myVal = self.ui.verticalSlider_FilterDelay.value()
        control = [CONTROLLER_CHANGE, 84, myVal] # Filter Delay
        midiOut.send_message(control)

        loghelperValInt = self.mapValInt(myVal,0,127,0,1000)
        loghelperValFloat=float(loghelperValInt)/1000.0
        loghelperValFloat= math.pow(loghelperValFloat,3)*10000
        if loghelperValFloat+1 > 10000:
            loghelperValFloat = 9999

        self.ui.label_FilterDelay.setText("%d" % int(loghelperValFloat+1))

    def setFilterAttack(self):
        myVal = self.ui.verticalSlider_FilterAttack.value()
        control = [CONTROLLER_CHANGE, 85, myVal] # Filter Attack
        midiOut.send_message(control)

        loghelperValInt = self.mapValInt(myVal,0,127,0,1000)
        loghelperValFloat=float(loghelperValInt)/1000.0
        loghelperValFloat= math.pow(loghelperValFloat,3)*10000
        if loghelperValFloat+1 > 10000:
            loghelperValFloat = 9999

        self.ui.label_FilterAttack.setText("%d" % int(loghelperValFloat+1))

    def setFilterHold(self):
        myVal = self.ui.verticalSlider_FilterHold.value()
        control = [CONTROLLER_CHANGE, 86, myVal] # Filter Hold
        midiOut.send_message(control)

        loghelperValInt = self.mapValInt(myVal,0,127,0,1000)
        loghelperValFloat=float(loghelperValInt)/1000.0
        loghelperValFloat= math.pow(loghelperValFloat,3)*10000
        if loghelperValFloat+1 > 10000:
            loghelperValFloat = 9999

        self.ui.label_FilterHold.setText("%d" % int(loghelperValFloat+1))

    def setFilterDecay(self):
        myVal = self.ui.verticalSlider_FilterDecay.value()
        control = [CONTROLLER_CHANGE, 87, myVal] # Filter Decay
        midiOut.send_message(control)

        loghelperValInt = self.mapValInt(myVal,0,127,0,1000)
        loghelperValFloat=float(loghelperValInt)/1000.0
        loghelperValFloat= math.pow(loghelperValFloat,3)*10000
        if loghelperValFloat+1 > 10000:
            loghelperValFloat = 9999

        self.ui.label_FilterDecay.setText("%d" % int(loghelperValFloat+1))

    def setFilterSustain(self):
        myVal = self.ui.verticalSlider_FilterSustain.value()
        control = [CONTROLLER_CHANGE, 88, myVal] # Filter Sustain
        midiOut.send_message(control)

        self.ui.label_FilterSustain.setText("%d" % int(myVal))

    def setFilterRelease(self):
        myVal = self.ui.verticalSlider_FilterRelease.value()
        control = [CONTROLLER_CHANGE, 89, myVal] # Filter Release
        midiOut.send_message(control)

        loghelperValInt = self.mapValInt(myVal,0,127,0,1000)
        loghelperValFloat=float(loghelperValInt)/1000.0
        loghelperValFloat= math.pow(loghelperValFloat,3)*10000
        if loghelperValFloat+1 > 10000:
            loghelperValFloat = 9999

        self.ui.label_FilterRelease.setText("%d" % int(loghelperValFloat+1))

    def setGuitarStaticEnvelopeToFilter(self):
        myVal = self.ui.dial_GuitarFilterStaticVelocity.value()
        control = [CONTROLLER_CHANGE, 98, myVal] # Static Guitar Filter Envelope !! wir benutzen cc 98 hier wir sind nicht in der reihenvolge
        midiOut.send_message(control)

        if myVal == 0:
            self.ui.label_StaticOrDynamicEnvelope.setText("Dynamic Envelope")
            self.ui.label_GuitarFilterStaticVelocety.setText("-")
        else:
            self.ui.label_StaticOrDynamicEnvelope.setText("Static Envelope")
            self.ui.label_GuitarFilterStaticVelocety.setText("%d" % int(self.mapValInt(myVal,0,127,0,100)) )


    #################### MOD LFO ##############################################

    def setModLFOWaveForm(self):
        myVal = self.ui.comboBox_ModLFOWafeForm.currentIndex()
        control = [CONTROLLER_CHANGE, 90, myVal] # MOD LFO waveForm
        midiOut.send_message(control)

        if myVal < 6:
            self.ui.comboBox_ModLfoCycles.setVisible(False)
        else:
            self.ui.comboBox_ModLfoCycles.setVisible(True)

    def setModLFOPulseWide(self):
        myVal = self.ui.dial_ModLFOPulseWide.value()
        control = [CONTROLLER_CHANGE, 91, myVal] # MOD LFO PulseWide
        midiOut.send_message(control)
        loghelperValInt = self.mapValInt(myVal,0,127,1,100)
        self.ui.label_ModLfoPulseWide.setText("%d" % loghelperValInt)

    def setModLFOBpm(self):
        myVal = self.ui.dialModLFOBPM.value()
        control = [CONTROLLER_CHANGE, 92, myVal ] # MOD LFO BPM
        midiOut.send_message(control)
        self.ui.label_ModLFOBPM.setText("%d" % self.mapValInt(myVal,0,127,0,360))

    def setModLfoBpmMulti(self):
        myVal = self.ui.comboBoxModLfoMulti.currentIndex()
        control = [CONTROLLER_CHANGE, 93, myVal] # MOD LFO BPM Multi
        midiOut.send_message(control)

    def setModLfoPhase(self):
        myVal = self.ui.dial_ModLFO_Phase.value()
        control = [CONTROLLER_CHANGE, 94, myVal] # MOD LFO Phase
        midiOut.send_message(control)

        myLableVal = self.mapValInt(myVal,0,127,0,360)
        self.ui.label_Mod_LFO_Phase.setText("%d" % (myLableVal))

    def setModLfoSyncToAttack(self):
        myVal = self.ui.checkBoxModLfoSyncToAttack.checkState()
        #print("sync MOD to attack %d" % myVal)
        if myVal == 2:
            myVal = 1

        control = [CONTROLLER_CHANGE, 95, myVal] # MOD 2 Mode
        midiOut.send_message(control)

    def setModLfoAttackToFreq(self):
        myVal = self.ui.dial_ModLfoATTCKtoFreq.value()
        control = [CONTROLLER_CHANGE, 96, myVal] # MOD LFO Attack Envelope to Freq.
        midiOut.send_message(control)

        self.ui.label_ModLfoAttackToFreq.setText("%d" % (self.mapValInt(myVal,0,127,0,30)))

    def setModAttackenvelopeToFilterDepth(self):
        myVal = self.ui.dial_ModLfoATTCKtoDepth.value()
        control = [CONTROLLER_CHANGE, 97, myVal] # MOD LFO Attack Envelope to Depth.
        midiOut.send_message(control)
        if myVal ==  63 or myVal == 64:
            self.ui.label_ModLfoAttackToDepth.setText("%d" % (0))
        else:
            self.ui.label_ModLfoAttackToDepth.setText("%d" % (self.mapValInt(myVal,0,127,-100,100)))



    def setModLfoTargetFreqOSC1(self):
        myVal = self.ui.verticalSlider_ModLfoTargetFrequencyToOSC1.value()
        control = [CONTROLLER_CHANGE, 99, myVal] # Mod LFO T1 Freq
        midiOut.send_message(control)

        mytacktackval = float(self.mapValInt( myVal, 0, 127, 0, 1000))/1000.0

        local_notes = 0.0
        local_cents = 0.0

        local_notes = 12*mytacktackval;
        local_cents = math.fmod( local_notes,1 )*100;

        self.ui.label_ModLfoTargetFrequencyToOSC1.setText("%d, %d" % (int(local_notes), int(local_cents)) )

    def setModLfoTargetFreqOSC2(self):
        myVal = self.ui.verticalSlider_ModLfoTargetFrequencyToOSC2.value()
        control = [CONTROLLER_CHANGE, 100, myVal] # MOD LFO T2 Freq
        midiOut.send_message(control)

        mytacktackval = float(self.mapValInt( myVal, 0, 127, 0, 1000))/1000.0

        local_notes = 0.0
        local_cents = 0.0

        local_notes = 12*mytacktackval;
        local_cents = math.fmod( local_notes,1 )*100;

        self.ui.label_ModLfoTargetFrequencyToOSC2.setText("%d, %d" % (int(local_notes), int(local_cents)) )


    def setModLfoTargetFreqSubOSC1(self):
        myVal = self.ui.verticalSlider_ModLfoTargetFrequencyToSubOSC1.value()
        control = [CONTROLLER_CHANGE, 101, myVal] # MOD LFO T3 Freq
        midiOut.send_message(control)

        mytacktackval = float(self.mapValInt( myVal, 0, 127, 0, 1000))/1000.0

        local_notes = 0.0
        local_cents = 0.0

        local_notes = 12*mytacktackval;
        local_cents = math.fmod( local_notes,1 )*100;

        self.ui.label_ModLfoTargetFrequencyToSubOSC1.setText("%d, %d" % (int(local_notes), int(local_cents)) )


    def setModLfoTargetFreqSubOSC2(self):
        myVal = self.ui.verticalSlider_ModLfoTargetFrequencyToSubOSC2.value()
        control = [CONTROLLER_CHANGE, 102, myVal] # MOD LFO T4 Freq
        midiOut.send_message(control)

        mytacktackval = float(self.mapValInt( myVal, 0, 127, 0, 1000))/1000.0

        local_notes = 0.0
        local_cents = 0.0

        local_notes = 12*mytacktackval;
        local_cents = math.fmod( local_notes,1 )*100;

        self.ui.label_ModLfoTargetFrequencyToSubOSC2.setText("%d, %d" % (int(local_notes), int(local_cents)) )

    def setModLfoTarget1to4FreqSync(self):
        myVal = self.ui.checkBoxModLfoSyncAllFreqencyTargets.checkState()
        if myVal == 2:
            myVal = 1
        control = [CONTROLLER_CHANGE, 103, myVal] # MOD LFO Sync T1-t4
        midiOut.send_message(control)

    def setModLfoTarget5to8PulseSync(self):
        myVal = self.ui.checkBoxModLfoSyncAllPulseTargets.checkState()
        if myVal == 2:
            myVal = 1
        control = [CONTROLLER_CHANGE, 104, myVal] # MOD LFO Sync T5-t8
        midiOut.send_message(control)




    def setModLfoTargetPulseOSC1(self):
        myVal = self.ui.verticalSlider_ModLfoTargetPulseToOSC1.value()
        control = [CONTROLLER_CHANGE, 105, myVal] # Mod LFO T1 Pulse
        midiOut.send_message(control)

        myTextVal = int(self.mapValInt(myVal, 0, 127, -100, 100))
        if myVal == 63 or myVal == 64:
            myTextVal = 0
        self.ui.label_ModLfoTargetPulseToOSC1.setText("%d" % myTextVal)

    def setModLfoTargetPulseOSC2(self):
        myVal = self.ui.verticalSlider_ModLfoTargetPulseToOSC2.value()
        control = [CONTROLLER_CHANGE, 106, myVal] # MOD LFO T2 Pulse
        midiOut.send_message(control)

        myTextVal = int(self.mapValInt(myVal, 0, 127, -100, 100))
        if myVal == 63 or myVal == 64:
            myTextVal = 0
        self.ui.label_ModLfoTargetPulseToOSC2.setText("%d" % myTextVal)


    def setModLfoTargetPulseSubOSC1(self):
        myVal = self.ui.verticalSlider_ModLfoTargetPulseToSubOSC1.value()
        control = [CONTROLLER_CHANGE, 107, myVal] # MOD LFO T3 Pulse
        midiOut.send_message(control)

        myTextVal = int(self.mapValInt(myVal, 0, 127, -100, 100))
        if myVal == 63 or myVal == 64:
            myTextVal = 0
        self.ui.label_ModLfoTargetPulseToSubOSC1.setText("%d" % myTextVal)


    def setModLfoTargetPulseSubOSC2(self):
        myVal = self.ui.verticalSlider_ModLfoTargetPulseToSubOSC2.value()
        control = [CONTROLLER_CHANGE, 108, myVal] # MOD LFO T4 Pulse
        midiOut.send_message(control)

        myTextVal = int(self.mapValInt(myVal, 0, 127, -100, 100))
        if myVal == 63 or myVal == 64:
            myTextVal = 0
        self.ui.label_ModLfoTargetPulseToSubOSC2.setText("%d" % myTextVal)


    ##### Vol LFO #############################################################

    def setVolLFOWaveForm(self):
        myVal = self.ui.comboBox_VolLFOWafeForm.currentIndex()
        control = [CONTROLLER_CHANGE, 109, myVal] # VOL LFO waveForm
        midiOut.send_message(control)

    def setVolLFOPulseWide(self):
        myVal = self.ui.dial_VolLFOPulseWide.value()
        control = [CONTROLLER_CHANGE, 110, myVal] # VOL LFO PulseWide
        midiOut.send_message(control)
        loghelperValInt = self.mapValInt(myVal,0,127,1,100)
        self.ui.label_VolLfoPulseWide.setText("%d" % loghelperValInt)

    def setVolLFOBpm(self):
        myVal = self.ui.dialVolLFOBPM.value()
        control = [CONTROLLER_CHANGE, 111, myVal ] # VOL LFO BPM
        midiOut.send_message(control)
        self.ui.label_VolLFOBPM.setText("%d" % self.mapValInt(myVal,0,127,0,360))

    def setVolLfoBpmMulti(self):
        myVal = self.ui.comboBoxVolLfoMulti.currentIndex()
        control = [CONTROLLER_CHANGE, 112, myVal] # VOL LFO BPM Multi
        midiOut.send_message(control)

    def setVolLfoPhase(self):
        myVal = self.ui.dial_VolLFO_Phase.value()
        control = [CONTROLLER_CHANGE, 113, myVal] # VOL LFO Phase
        midiOut.send_message(control)

        myLableVal = self.mapValInt(myVal,0,127,0,360)
        self.ui.label_Vol_LFO_Phase.setText("%d" % (myLableVal))

    def setVolLfoSyncToAttack(self):
        myVal = self.ui.checkBoxVolLfoSyncToAttack.checkState()
        #print("sync VOL to attack %d" % myVal)
        if myVal == 2:
            myVal = 1

        control = [CONTROLLER_CHANGE, 114, myVal] # VOL 2 Mode
        midiOut.send_message(control)

    def setVolLfoAttackToFreq(self):
        myVal = self.ui.dial_VolLfoATTCKtoFreq.value()
        control = [CONTROLLER_CHANGE, 115, myVal] # VOL LFO Attack Envelope to Freq.
        midiOut.send_message(control)

        self.ui.label_VolLfoAttackToFreq.setText("%d" % (self.mapValInt(myVal,0,127,0,30)))

    def setVolAttackenvelopeToFilterDepth(self):
        myVal = self.ui.dial_VolLfoATTCKtoDepth.value()
        control = [CONTROLLER_CHANGE, 116, myVal] # VOL LFO Attack Envelope to Depth.
        midiOut.send_message(control)
        if myVal ==  63 or myVal == 64:
            self.ui.label_VolLfoAttackToDepth.setText("%d" % (0))
        else:
            self.ui.label_VolLfoAttackToDepth.setText("%d" % (self.mapValInt(myVal,0,127,-100,100)))

    def setVolLfoTarget1to4Sync(self):
        myVal = self.ui.checkBoxVolLfoSyncAllTargets.checkState()
        if myVal == 2:
            myVal = 1
        control = [CONTROLLER_CHANGE, 117, myVal] # VOL LFO Sync T1-t4
        midiOut.send_message(control)


    def setVolLfoTargetOSC1(self):
        myVal = self.ui.verticalSlider_VolLfoTargetToOSC1.value()
        control = [CONTROLLER_CHANGE, 118, myVal] # Vol LFO T1
        midiOut.send_message(control)

        myTextVal = int(self.mapValInt(myVal, 0, 127, -100, 100))
        if myVal == 63 or myVal == 64:
            myTextVal = 0
        self.ui.label_VolLfoTargetToOSC1.setText("%d" % myTextVal)

    def setVolLfoTargetOSC2(self):
        myVal = self.ui.verticalSlider_VolLfoTargetToOSC2.value()
        control = [CONTROLLER_CHANGE, 119, myVal] # Vol LFO T2
        midiOut.send_message(control)

        myTextVal = int(self.mapValInt(myVal, 0, 127, -100, 100))
        if myVal == 63 or myVal == 64:
            myTextVal = 0
        self.ui.label_VolLfoTargetToOSC2.setText("%d" % myTextVal)


    def setVolLfoTargetAllSubs(self):
        myVal = self.ui.verticalSlider_VolLfoTargetToAllSubs.value()
        control = [CONTROLLER_CHANGE, 120, myVal] # Vol LFO T3
        midiOut.send_message(control)

        myTextVal = int(self.mapValInt(myVal, 0, 127, -100, 100))
        if myVal == 63 or myVal == 64:
            myTextVal = 0
        self.ui.label_VolLfoTargetToAllSubs.setText("%d" % myTextVal)


    def setVolLfoTargetNoise(self):
        myVal = self.ui.verticalSlider_VolLfoTargetToNoise.value()
        control = [CONTROLLER_CHANGE, 121, myVal] # MOD LFO T4 Pulse
        midiOut.send_message(control)

        myTextVal = int(self.mapValInt(myVal, 0, 127, 0, 100))
        if myVal == 63 or myVal == 64:
            myTextVal = 0
        self.ui.label_VolLfoTargetToNoise.setText("%d" % myTextVal)




##### MISK SETTINGS #################

    def setMidiPitchBendRange(self):
        myVal = self.ui.comboBox_MidiPitchBendRange.currentIndex()
        control = [CONTROLLER_CHANGE, 122, myVal] # Midi Pitch Bend Range
        midiOut.send_message(control)

    def setAutoPeakCorrection(self):
        myVal = self.ui.checkBoxPeakCutoff.checkState()
        if myVal == 2:
            myVal = 1

        control = [CONTROLLER_CHANGE, 123, myVal] # Auto peak Correction
        midiOut.send_message(control)


    def setAllLfosOnSync(self):
        myVal = self.ui.SYNC_ALL_LFOS.checkState()
        if myVal > 0 :
            self.ui.SYNCFLFO.setCheckable(False)
            self.ui.SYNCVolumenLFO.setCheckable(False)
            self.ui.SYNCModLFO.setCheckable(False)
        else :
            self.ui.SYNCFLFO.setCheckable(True)
            self.ui.SYNCVolumenLFO.setCheckable(True)
            self.ui.SYNCModLFO.setCheckable(True)


        if myVal == 2:
            myVal = 1

        control = [CONTROLLER_CHANGE, 124, myVal] # Master Clock To all LFOs
        midiOut.send_message(control)

    def setModLfosOnSync(self):
        myVal = self.ui.SYNCModLFO.checkState()

        if myVal == 2:
            myVal = 1

        control = [CONTROLLER_CHANGE, 125, myVal] # Master Clock To Mod LFOs
        midiOut.send_message(control)


    def setVolLfosOnSync(self):
        myVal = self.ui.SYNCVolumenLFO.checkState()

        if myVal == 2:
            myVal = 1

        control = [CONTROLLER_CHANGE, 126, myVal] # Master Clock To Vol LFOs
        midiOut.send_message(control)


    def setFreqLfosOnSync(self):
        myVal = self.ui.SYNCFLFO.checkState()

        if myVal == 2:
            myVal = 1

        control = [CONTROLLER_CHANGE, 127, myVal] # Master Clock To Freq LFOs
        midiOut.send_message(control)



    ################### Sequencer #############################################
    def sendSequence(self):
        # value steps
        valueSteps = self.ui.spinBox_sequencerSteps.value() + 1 # +1 because 0 produce note off

        control = [NOTE_ON, 126, 1]
        midiOut.send_message(control)
        control = [NOTE_ON, 1, 1]
        midiOut.send_message(control)

        control = [NOTE_ON, 32, valueSteps]
        midiOut.send_message(control)

        #value time signatur
        valueTimeSignature = self.ui.comboBox_SequencerBeat.currentIndex() + 1
        control = [NOTE_ON, 126, 1]
        midiOut.send_message(control)
        control = [NOTE_ON, 1, 1]
        midiOut.send_message(control)

        control = [NOTE_ON, 33, valueTimeSignature]
        midiOut.send_message(control)

        ################ Step Values #############
        # value sequencer step 0
        valuestep0 = self.ui.spinBox_Seq0.value() +25
        control = [NOTE_ON, 126, 1]
        midiOut.send_message(control)
        control = [NOTE_ON, 1, 1]
        midiOut.send_message(control)

        control = [NOTE_ON, 0, valuestep0]
        midiOut.send_message(control)

        # value sequencer step 1
        valuestep1 = self.ui.spinBox_Seq1.value() +25
        control = [NOTE_ON, 126, 1]
        midiOut.send_message(control)
        control = [NOTE_ON, 1, 1]
        midiOut.send_message(control)

        control = [NOTE_ON, 1, valuestep1]
        midiOut.send_message(control)

        # value sequencer step 2
        valuestep2 = self.ui.spinBox_Seq2.value() +25
        control = [NOTE_ON, 126, 1]
        midiOut.send_message(control)
        control = [NOTE_ON, 1, 1]
        midiOut.send_message(control)

        control = [NOTE_ON, 2, valuestep2]
        midiOut.send_message(control)

        # value sequencer step 3
        valuestep3 = self.ui.spinBox_Seq3.value() +25
        control = [NOTE_ON, 126, 1]
        midiOut.send_message(control)
        control = [NOTE_ON, 1, 1]
        midiOut.send_message(control)

        control = [NOTE_ON, 3, valuestep3]
        midiOut.send_message(control)


        # value sequencer step 4
        valuestep4 = self.ui.spinBox_Seq4.value() +25
        control = [NOTE_ON, 126, 1]
        midiOut.send_message(control)
        control = [NOTE_ON, 1, 1]
        midiOut.send_message(control)

        control = [NOTE_ON, 4, valuestep4]
        midiOut.send_message(control)

        # value sequencer step 5
        valuestep5 = self.ui.spinBox_Seq5.value() +25
        control = [NOTE_ON, 126, 1]
        midiOut.send_message(control)
        control = [NOTE_ON, 1, 1]
        midiOut.send_message(control)

        control = [NOTE_ON, 5, valuestep5]
        midiOut.send_message(control)

        # value sequencer step 6
        valuestep6 = self.ui.spinBox_Seq6.value() +25
        control = [NOTE_ON, 126, 1]
        midiOut.send_message(control)
        control = [NOTE_ON, 1, 1]
        midiOut.send_message(control)

        control = [NOTE_ON, 6, valuestep6]
        midiOut.send_message(control)

        # value sequencer step 7
        valuestep7 = self.ui.spinBox_Seq7.value() +25
        control = [NOTE_ON, 126, 1]
        midiOut.send_message(control)
        control = [NOTE_ON, 1, 1]
        midiOut.send_message(control)

        control = [NOTE_ON, 7, valuestep7]
        midiOut.send_message(control)



        # value sequencer step 8
        valuestep8 = self.ui.spinBox_Seq8.value() +25
        control = [NOTE_ON, 126, 1]
        midiOut.send_message(control)
        control = [NOTE_ON, 1, 1]
        midiOut.send_message(control)

        control = [NOTE_ON, 8, valuestep8]
        midiOut.send_message(control)

        # value sequencer step 9
        valuestep9 = self.ui.spinBox_Seq9.value() +25
        control = [NOTE_ON, 126, 1]
        midiOut.send_message(control)
        control = [NOTE_ON, 1, 1]
        midiOut.send_message(control)

        control = [NOTE_ON, 9, valuestep9]
        midiOut.send_message(control)

        # value sequencer step 10
        valuestep10 = self.ui.spinBox_Seq10.value() +25
        control = [NOTE_ON, 126, 1]
        midiOut.send_message(control)
        control = [NOTE_ON, 1, 1]
        midiOut.send_message(control)

        control = [NOTE_ON, 10, valuestep10]
        midiOut.send_message(control)

        # value sequencer step 11
        valuestep11 = self.ui.spinBox_Seq11.value() +25
        control = [NOTE_ON, 126, 1]
        midiOut.send_message(control)
        control = [NOTE_ON, 1, 1]
        midiOut.send_message(control)

        control = [NOTE_ON, 11, valuestep11]
        midiOut.send_message(control)


        # value sequencer step 12
        valuestep12 = self.ui.spinBox_Seq12.value() +25
        control = [NOTE_ON, 126, 1]
        midiOut.send_message(control)
        control = [NOTE_ON, 1, 1]
        midiOut.send_message(control)

        control = [NOTE_ON, 12, valuestep12]
        midiOut.send_message(control)

        # value sequencer step 13
        valuestep13 = self.ui.spinBox_Seq13.value() +25
        control = [NOTE_ON, 126, 1]
        midiOut.send_message(control)
        control = [NOTE_ON, 1, 1]
        midiOut.send_message(control)

        control = [NOTE_ON, 13, valuestep13]
        midiOut.send_message(control)

        # value sequencer step 14
        valuestep14 = self.ui.spinBox_Seq14.value() +25
        control = [NOTE_ON, 126, 1]
        midiOut.send_message(control)
        control = [NOTE_ON, 1, 1]
        midiOut.send_message(control)

        control = [NOTE_ON, 14, valuestep14]
        midiOut.send_message(control)

        # value sequencer step 15
        valuestep15 = self.ui.spinBox_Seq15.value() +25
        control = [NOTE_ON, 126, 1]
        midiOut.send_message(control)
        control = [NOTE_ON, 1, 1]
        midiOut.send_message(control)

        control = [NOTE_ON, 15, valuestep15]
        midiOut.send_message(control)



        # value sequencer step 16
        valuestep16 = self.ui.spinBox_Seq16.value() +25
        control = [NOTE_ON, 126, 1]
        midiOut.send_message(control)
        control = [NOTE_ON, 1, 1]
        midiOut.send_message(control)

        control = [NOTE_ON, 16, valuestep16]
        midiOut.send_message(control)


        # value sequencer step 17
        valuestep17 = self.ui.spinBox_Seq17.value() +25
        control = [NOTE_ON, 126, 1]
        midiOut.send_message(control)
        control = [NOTE_ON, 1, 1]
        midiOut.send_message(control)

        control = [NOTE_ON, 17, valuestep17]
        midiOut.send_message(control)

        # value sequencer step 18
        valuestep18 = self.ui.spinBox_Seq18.value() +25
        control = [NOTE_ON, 126, 1]
        midiOut.send_message(control)
        control = [NOTE_ON, 1, 1]
        midiOut.send_message(control)

        control = [NOTE_ON, 18, valuestep18]
        midiOut.send_message(control)

        # value sequencer step 19
        valuestep19 = self.ui.spinBox_Seq19.value() +25
        control = [NOTE_ON, 126, 1]
        midiOut.send_message(control)
        control = [NOTE_ON, 1, 1]
        midiOut.send_message(control)

        control = [NOTE_ON, 19, valuestep19]
        midiOut.send_message(control)

        # value sequencer step 20
        valuestep20 = self.ui.spinBox_Seq20.value() +25
        control = [NOTE_ON, 126, 1]
        midiOut.send_message(control)
        control = [NOTE_ON, 1, 1]
        midiOut.send_message(control)

        control = [NOTE_ON, 20, valuestep20]
        midiOut.send_message(control)


        # value sequencer step 21
        valuestep21 = self.ui.spinBox_Seq21.value() +25
        control = [NOTE_ON, 126, 1]
        midiOut.send_message(control)
        control = [NOTE_ON, 1, 1]
        midiOut.send_message(control)

        control = [NOTE_ON, 21, valuestep21]
        midiOut.send_message(control)

        # value sequencer step 22
        valuestep22 = self.ui.spinBox_Seq22.value() +25
        control = [NOTE_ON, 126, 1]
        midiOut.send_message(control)
        control = [NOTE_ON, 1, 1]
        midiOut.send_message(control)

        control = [NOTE_ON, 22, valuestep22]
        midiOut.send_message(control)

        # value sequencer step 23
        valuestep23 = self.ui.spinBox_Seq23.value() +25
        control = [NOTE_ON, 126, 1]
        midiOut.send_message(control)
        control = [NOTE_ON, 1, 1]
        midiOut.send_message(control)

        control = [NOTE_ON, 23, valuestep23]
        midiOut.send_message(control)

        # value sequencer step 24
        valuestep24 = self.ui.spinBox_Seq24.value() +25
        control = [NOTE_ON, 126, 1]
        midiOut.send_message(control)
        control = [NOTE_ON, 1, 1]
        midiOut.send_message(control)

        control = [NOTE_ON, 24, valuestep24]
        midiOut.send_message(control)



        # value sequencer step 25
        valuestep25 = self.ui.spinBox_Seq25.value() +25
        control = [NOTE_ON, 126, 1]
        midiOut.send_message(control)
        control = [NOTE_ON, 1, 1]
        midiOut.send_message(control)

        control = [NOTE_ON, 25, valuestep25]
        midiOut.send_message(control)

        # value sequencer step 26
        valuestep26 = self.ui.spinBox_Seq26.value() +25
        control = [NOTE_ON, 126, 1]
        midiOut.send_message(control)
        control = [NOTE_ON, 1, 1]
        midiOut.send_message(control)

        control = [NOTE_ON, 26, valuestep26]
        midiOut.send_message(control)

        # value sequencer step 27
        valuestep27 = self.ui.spinBox_Seq27.value() +25
        control = [NOTE_ON, 126, 1]
        midiOut.send_message(control)
        control = [NOTE_ON, 1, 1]
        midiOut.send_message(control)

        control = [NOTE_ON, 27, valuestep27]
        midiOut.send_message(control)

        # value sequencer step 28
        valuestep28 = self.ui.spinBox_Seq28.value() +25
        control = [NOTE_ON, 126, 1]
        midiOut.send_message(control)
        control = [NOTE_ON, 1, 1]
        midiOut.send_message(control)

        control = [NOTE_ON, 28, valuestep28]
        midiOut.send_message(control)


        # value sequencer step 29
        valuestep29 = self.ui.spinBox_Seq29.value() +25
        control = [NOTE_ON, 126, 1]
        midiOut.send_message(control)
        control = [NOTE_ON, 1, 1]
        midiOut.send_message(control)

        control = [NOTE_ON, 29, valuestep29]
        midiOut.send_message(control)

        # value sequencer step 30
        valuestep30 = self.ui.spinBox_Seq30.value() +25
        control = [NOTE_ON, 126, 1]
        midiOut.send_message(control)
        control = [NOTE_ON, 1, 1]
        midiOut.send_message(control)

        control = [NOTE_ON, 30, valuestep30]
        midiOut.send_message(control)

        # value sequencer step 31
        valuestep31 = self.ui.spinBox_Seq31.value() +25
        control = [NOTE_ON, 126, 1]
        midiOut.send_message(control)
        control = [NOTE_ON, 1, 1]
        midiOut.send_message(control)

        control = [NOTE_ON, 31, valuestep31]
        midiOut.send_message(control)



    #################### Helper ################################################

    def mapValInt(self, x, in_min, in_max, out_min, out_max):
        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min

    ################ Exit #####
    def closeEvent(self, event):
        self.thisMidiInputThread.exit()
        print("closing App")


# ----------------------- MAIN --------------------------------------
if __name__ == "__main__":
    ### init midi output

    global gInIsConnected
    gInIsConnected = "No Midi Device found"
    global gOutIsConnected
    gOutIsConnected = "No Midi Device found"

    global gMyMidiOutputPort
    gMyMidiOutputPort = False
    global gMyMidiInputPort
    gMyMidiInputPort = False

    print ("Available MIDI devices:")
    midiOut = rtmidi.MidiOut()
    myOutDeviceId = -1
    for i, port in enumerate(midiOut.get_ports()):
        print("%d: %s" % (i, port))
        if "GITSynth127" in str(port):
        #if "KMidimon" in str(port):
            print("Found Gitsynth127 on ID: %d" % i)
            myOutDeviceId = i

    if myOutDeviceId == -1:
        print("Gitsynth not Found")
    else:
        print( "Connecting to device: %s" % midiOut.get_ports()[myOutDeviceId])
        midiOut.open_port(myOutDeviceId)
        gOutIsConnected = midiOut.get_ports()[myOutDeviceId]
        gMyMidiOutputPort = True

    ### init midi input
    midiIn = rtmidi.MidiIn()
    myInDeviceId = -1
    for i, port in enumerate(midiIn.get_ports()):
        print("%d: %s" % (i, port))
        if "GITSynth127" in str(port):
        #if "KMidimon" in str(port):
            print("Found input from Gitsynth127 on ID: %d" % i )
            myInDeviceId = i

    if myInDeviceId == -1:
        print ("Gitsynth not Found")
    else:
        print( "Connecting to device: %s" % midiIn.get_ports()[myInDeviceId])
        midiIn.open_port(myInDeviceId)
        gInIsConnected = midiIn.get_ports()[myInDeviceId]
        gMyMidiInputPort = True



    app = QtGui.QApplication(sys.argv)
    myapp = MyForm()
    myapp.show()
    sys.exit(app.exec_())
