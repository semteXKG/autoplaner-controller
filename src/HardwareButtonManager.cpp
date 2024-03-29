#include <HardwareButtonManager.h>

HardwareButtonManager::HardwareButtonManager(gpio_num_t enterButtonPin, gpio_num_t speedButtonPin, gpio_num_t moveToConversionButtonPin, SharedData* sharedData) {
    this->sharedData = sharedData;

    Bounce2::Button* enterButton = new Bounce2::Button();
    enterButton->setPressedState(LOW);
    enterButton->interval(50);
    enterButton->attach(enterButtonPin, INPUT_PULLUP);

    Bounce2::Button* speedButton = new Bounce2::Button();
    speedButton->setPressedState(LOW);
    speedButton->interval(50);
    speedButton->attach(speedButtonPin, INPUT_PULLUP);

    Bounce2::Button* moveToConv = new Bounce2::Button();
    moveToConv->interval(50);
    moveToConv->setPressedState(LOW);
    moveToConv->attach(moveToConversionButtonPin, INPUT_PULLUP);

    sharedData->enterButton = enterButton;
    sharedData->speedButton = speedButton;
    sharedData->moveToConversion = moveToConv;
}

HardwareButtonManager::~HardwareButtonManager() {
}

void HardwareButtonManager::tick() {
    sharedData->speedButton->update();
    sharedData->enterButton->update();
    sharedData->moveToConversion->update();
}
