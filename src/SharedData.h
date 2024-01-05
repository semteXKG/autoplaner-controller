#ifndef shareddata_h
#define shareddata_h

#include <Constants.h>
#include <Bounce2.h>

struct BackingData
{
    long targetPosition;
    long currentPosition;
    int offset;
    bool locked;
    bool calibrationDone;
    bool calibrationStart;
    MachineState state;
};


class SharedData {
    private:
        BackingData* backingData;
        char* menuEntries[3];
        int rapidInputReceived = 0;
        long nextDisplayUpdate = -1;
        int lastRotation = 0;
        long lastDistance = 0;
        int previousOffset;
    public:
        SharedData();
        ~SharedData();
        Bounce2::Button* enterButton;
        Bounce2::Button* speedButton;
        Bounce2::Button* moveToConversion;
        void setTargetPosition(long targetPosition);
        long getTargetPosition();
        void setCurrentPosition(long currentPosition);
        long getCurrentPosition();
        void setPosition(long position);
        void setOffset(int offset);
        int getOffset();
        void setPreviousOffset(int offset);
        int getPreviousOffset();
        long getLastDistance();
        void setLastDistance(long lastDistance);
        void setMenuEntries(char* upper, char* mid, char* low);
        char** getMenuEntries();
        void setLastRotation(int rotation);
        int getLastRotation();
        bool isLocked();
        void setLocked(bool locked);
        void markCalibrationDone();
        void markCalibratinStart();
        void scheduleDisplayUpdate();
        bool shouldUpdateDisplay();
        void switchState(MachineState state);
        MachineState getState();
        BackingData* getBackingData();
        void setBackingData(BackingData* backingData);
};

#endif