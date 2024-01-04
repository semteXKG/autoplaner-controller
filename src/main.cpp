#include <Arduino.h>
#include <U8g2lib.h>
#include <TargetSelector.h>
#include <Display.h>
#include <SharedData.h>
#include <HardwareButtonManager.h>
#include <MenuHandler.h>
#include <ArduinoNvs.h>
#include <Communicator.h>

#define DEBUG ""

static const gpio_num_t SPEED_BUTTON = GPIO_NUM_12;
static const gpio_num_t GO_BUTTON = GPIO_NUM_14;
static const gpio_num_t MOVE_TO_CONVERSION_BUTTON = GPIO_NUM_16;

static const gpio_num_t HEIGHT_STEPPER_PULSE = GPIO_NUM_33;
static const gpio_num_t HEIGHT_STEPPER_DIR = GPIO_NUM_32;

static const gpio_num_t LOCK_STEPPER_PULSE = GPIO_NUM_15;
static const gpio_num_t LOCK_STEPPER_DIR = GPIO_NUM_2;


Display* display;
TargetSelector* inputManager;
SharedData* sharedData;
HardwareButtonManager* buttonManager;
MenuHandler* menuHandler;
Communicator* communicator;

void setup() {
	Serial.begin(115200);
	
	if (!NVS.begin("planer")) {
		ESP.restart();
	}

	sharedData = new SharedData();
	inputManager = new TargetSelector(18, 19, sharedData);
	buttonManager = new HardwareButtonManager(GO_BUTTON, SPEED_BUTTON, MOVE_TO_CONVERSION_BUTTON, sharedData);
	display = new Display(sharedData);
	menuHandler = new MenuHandler(sharedData);
	communicator = new Communicator(sharedData);
	sharedData->scheduleDisplayUpdate();
//	sharedData->switchState(MachineState::CALIBRATION_NEEDED);
	sharedData->switchState(MachineState::IDLE);
}


void loop() {
	buttonManager->tick();
	inputManager->tick();
	display->tick();
	menuHandler->tick();
	communicator->tick();
}