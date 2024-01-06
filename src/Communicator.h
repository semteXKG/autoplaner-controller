#ifndef communicator_h
#define communicator_h

#include <esp_now.h>
#include <WiFi.h>
#include <SharedData.h>
#include <mutex>

class Communicator {
private:
    SharedData* sharedData;
    BackingData* previousBackingData;
    std::mutex lockMutex;
    boolean isChanged(BackingData* currentData, BackingData* previousData);
    void printDiffChanges(boolean fromRemote, BackingData* newData, BackingData* existingData);
public:
    Communicator(SharedData* sharedData);
    ~Communicator();
    void tick();
    void updateBackingData(BackingData* backingData);
};

#endif