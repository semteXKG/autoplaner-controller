#ifndef calibration_offset_handler
#define calibration_offset_handler

#include <Arduino.h>
#include <SharedData.h>

class CalibrationOffsetHandler {
private:
    SharedData* sharedData;
public:
    CalibrationOffsetHandler(SharedData* sharedData);
    ~CalibrationOffsetHandler();
    void tick();
};

#endif