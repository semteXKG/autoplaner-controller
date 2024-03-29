#include <SharedData.h>
#include <Arduino.h>

SharedData::SharedData() {
    backingData = (BackingData*) malloc(sizeof(BackingData));
    backingData->calibrationDone = false;
    backingData->calibrationStart = false;
    backingData->locked = 0;
    backingData->offset = 0;
    backingData->currentPosition = MIN_POSITION;
    backingData->targetPosition = backingData->currentPosition;
    backingData->state = MachineState::CALIBRATION_NEEDED;
}

SharedData::~SharedData() {

}

long SharedData::getTargetPosition() {
    return backingData->targetPosition;
}

void SharedData::setTargetPosition(long targetPosition) {
    backingData->targetPosition = targetPosition;
    if(backingData->targetPosition < MIN_POSITION) {
		backingData->targetPosition = MIN_POSITION;
	} else if (backingData->targetPosition > MAX_POSITION) {
		backingData->targetPosition = MAX_POSITION;
	}
}

long SharedData::getCurrentPosition() {
    return backingData->currentPosition;
}

void SharedData::setCurrentPosition(long currentPosition) {
    backingData->currentPosition = currentPosition;
}

void SharedData::setPosition(long position) {
    setCurrentPosition(position);
    setTargetPosition(position);
}

void SharedData::setOffset(int offset) {
    backingData->offset = offset;
}

int SharedData::getOffset() {
    return backingData->offset;
}

void SharedData::setPreviousOffset(int prevOffset) {
    previousOffset = prevOffset;
}

int SharedData::getPreviousOffset() {
    return previousOffset;
}

long SharedData::getLastDistance() {
    return lastDistance;
}

void SharedData::setLastDistance(long lastDistance) {
    this->lastDistance = lastDistance;
}

void SharedData::setMenuEntries(char* upper, char* mid, char* low) {
    menuEntries[0] = upper;
    menuEntries[1] = mid;
    menuEntries[2] = low;
}

char** SharedData::getMenuEntries() {
    return menuEntries;
}

void SharedData::setLastRotation(int lastRotation) {
    this->lastRotation = lastRotation;
}

int SharedData::getLastRotation() {
    return lastRotation;
}

bool SharedData::isLocked() {
    return backingData->locked;
}

void SharedData::setLocked(bool locked) {
    backingData->locked = locked;
    scheduleDisplayUpdate();
}

void SharedData::markCalibrationDone() {
    backingData->calibrationDone = true;
}

void SharedData::markCalibratinStart() {
    backingData->calibrationStart = true;
}

void SharedData::scheduleDisplayUpdate() {
    if(this->nextDisplayUpdate == -1) {
        this->nextDisplayUpdate = millis() + DISPLAY_UPDATE_DELAY;
    }
}

bool SharedData::shouldUpdateDisplay() {
    if (this->nextDisplayUpdate != -1 && millis() > this->nextDisplayUpdate) {
        this->nextDisplayUpdate = -1;
        return true;
    }
    return false;
}

MachineState SharedData::getState() {
    return backingData->state;
}

void SharedData::switchState(MachineState state) {
    if(state == IDLE && !backingData->calibrationDone) {
        Serial.print("Cannot go idle without calibration ");
        state = MachineState::CALIBRATION_NEEDED;
    }
    
    backingData->state = state;
    scheduleDisplayUpdate();
}

BackingData* SharedData::getBackingData() {
    return backingData;
}

void SharedData::setBackingData(BackingData* newBackingData) {
    BackingData* currentBackingData = backingData;
    this->backingData = newBackingData;
    this->previousOffset = backingData->offset;
    free(currentBackingData);
}