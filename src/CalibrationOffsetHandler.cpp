#include <CalibrationOffsetHandler.h>

CalibrationOffsetHandler::CalibrationOffsetHandler(SharedData* sharedData) {
    this->sharedData = sharedData;
}

CalibrationOffsetHandler::~CalibrationOffsetHandler() {
}

void CalibrationOffsetHandler::tick() {
    if(sharedData->getState() == MachineState::SETTINGS_OFFSET_ADJUSTING &&
        sharedData->enterButton->pressed()) {
            int oldOffset = sharedData->getPreviousOffset();
            int diff = sharedData->getOffset() - oldOffset;
            sharedData->setCurrentPosition(sharedData->getCurrentPosition() + diff);
            sharedData->setTargetPosition(sharedData->getCurrentPosition());
            sharedData->setPreviousOffset(sharedData->getOffset());
            sharedData->switchState(MachineState::IDLE);
    }

    if(sharedData->getState() == MachineState::CALIBRATION_NEEDED && sharedData->enterButton->rose()) {
        sharedData->markCalibratinStart();
    }
}

