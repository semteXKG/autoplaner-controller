#ifndef target_selector_h
#define target_selector_h

#include <Arduino.h>
#include <ESP32Encoder.h>
#include <SharedData.h>

class TargetSelector {
    private:
        ESP32Encoder* encoder;
        SharedData* sharedData;
        int prevEncPosition;
        long fastRotationTill = 0, fastRotationSince = 0;
        long currentInputPosition = 0;
        bool hasCorrectInputSource;
        long lastValues[MAX_INPUTS];
        long lastTimestamps[MAX_INPUTS];
        void handleEncoder();
        void handleInputSelectionButton();
        void handleMoveToConversionButton();
        void correctAccidentalInputs();
        boolean evaluateFastMode();
    public:
        TargetSelector(int pin1, int pin2, SharedData* sharedData);
        ~TargetSelector();
        void tick();
};

#endif