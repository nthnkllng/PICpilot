/*
 * File:   PWM.c
 *
 * Created on August 21, 2014, 11:40 PM
 */
#include "PWM.h"
#include "OutputCompare.h"
#include "InputCapture.h"
#include "../Common/debug.h"

char initialized = 0;

int pwmArray[NUM_CHANNELS]; //The input
int checkArray[NUM_CHANNELS]; //The output for status updates
float scaleFactorIn[NUM_CHANNELS];
int offsetIn[NUM_CHANNELS];
float scaleFactorOut[NUM_CHANNELS];
int offsetOut[NUM_CHANNELS];

void initPWM(char inputChannels, char outputChannels){
    initIC(inputChannels);
    int i = 0;
    for (i = 0; i < NUM_CHANNELS; i++){
        // Use default scale factor values
        scaleFactorIn[i] = MAX_PWM/(float)(UPPER_PWM - MIDDLE_PWM); // We know that MAX_PWM = (UPPER_PWM - MIDDLE_PWM)/2 for a factor of 1.
        // Use default offset values
        offsetIn[i] = MIDDLE_PWM;
        // Use default scale factor values
        scaleFactorOut[i] = (float)(UPPER_PWM - MIDDLE_PWM)/MAX_PWM; // We know that MAX_PWM = (UPPER_PWM - MIDDLE_PWM) for a factor of 1.
        // Use default offset values
        offsetOut[i] = MIDDLE_PWM;
    }
    initOC(outputChannels);
    initialized = 1;
}

void PWMInputCalibration(unsigned int channel, float signalScaleFactor, unsigned int signalOffset){
    if (channel > 0 && channel <= NUM_CHANNELS){ //Check if channel number is valid
        scaleFactorIn[channel - 1] = signalScaleFactor;
        offsetIn[channel - 1] = signalOffset;
    }
    else { //Invalid Channel
        //Display Error Message
#if DEBUG
        error("Invalid PWM channel");
#endif
    }
}

void PWMOutputCalibration(unsigned int channel, float signalScaleFactor, unsigned int signalOffset){
    if (channel > 0 && channel <= NUM_CHANNELS){ //Check if channel number is valid
        scaleFactorOut[channel - 1] = signalScaleFactor;
        offsetOut[channel - 1] = signalOffset;
    }
    else { //Invalid Channel
        //Display Error Message
#if DEBUG
        error("Invalid PWM channel");
#endif
    }
}


int getPWM(unsigned int channel){
    if (initialized && channel > 0 && channel <= NUM_CHANNELS){ //Is the Input Initialized?
        return (int)((getICValue(channel) - offsetIn[channel - 1]) * scaleFactorIn[channel - 1]);
    }
    else { //Not initialized or invalid channel
        //Display Error Message
#if DEBUG
        error("PWM not initialized or invalid channel specified");
#endif
        //Return 0
        return MIN_PWM;
    }
}
int* getPWMArray(){
    if (initialized){ //Is the Input Initialized?
        unsigned int* icArray = getICValues();
        int i = 0;
        for (i = 0; i < NUM_CHANNELS; i++){
            pwmArray[i] = (int)(((int)(icArray[i]) - offsetIn[i]) * scaleFactorIn[i]);
        }
        return pwmArray;
    }
    else { //Not initialized
        //Display Error Message
#if DEBUG
        error("PWM not initialized");
#endif
        //Return 0
        return 0;

    }

}

void setPWM(unsigned int channel, int pwm){
    if (initialized && channel > 0 && channel <= NUM_CHANNELS){ //Is the Input Initialized?
        checkArray[channel - 1] = pwm;
        setOCValue(channel, (int)(pwm * scaleFactorOut[channel - 1] + offsetOut[channel - 1]));
    }
    else { //Not initialized or invalid channel
        //Display Error Message
#if DEBUG
        error("PWM not initialized or invalid channel specified");
#endif
    }
}
void setPWMArray(int* ocArray){
    if (initialized){ //Is the Input Initialized?
        int i = 0;
        for (i = 0; i < NUM_CHANNELS; i++){
            checkArray[i] = ocArray[i];
            setOCValue(i,(int)(ocArray[i]  * scaleFactorOut[i] + offsetOut[i]));
        }
    }
    else { //Not initialized
        //Display Error Message
#if DEBUG
        error("PWM not initialized.");
#endif
    }
}

int checkPWM(unsigned int channel){
    if (initialized && channel > 0 && channel <= NUM_CHANNELS){ //Is the Input Initialized?
        return checkArray[channel - 1];
    }
    else { //Not initialized or invalid channel
           //Display Error Message
#if DEBUG
        error("PWM not initialized or invalid channel specified");
#endif
        return -1;
    }
}

int* checkPWMArray(){
    if (initialized){ //Is the Input Initialized?
        return (int *)&checkArray;
    }
    else { //Not initialized
           //Display Error Message
#if DEBUG
        error("PWM not initialized.");
#endif
        return 0;
    }
}