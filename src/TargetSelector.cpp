#include <TargetSelector.h>

TargetSelector::TargetSelector(int pin1, int pin2, SharedData* sharedData) {
    ESP32Encoder::useInternalWeakPullResistors=UP;
	this->encoder = new ESP32Encoder();	
  	encoder->attachFullQuad(pin1, pin2);
	encoder->clearCount();
    encoder->setFilter(1023);

    this->sharedData = sharedData;
	this->prevEncPosition = 0;
}

TargetSelector::~TargetSelector() {
    // destructor
}

void TargetSelector::handleEncoder() {
	sharedData->setLastRotation(0);
    // Loop and read the count
	int newPos = round((double) encoder->getCount() / 4.0);

	if(newPos == prevEncPosition) {
		return;
	}

	int32_t delta = newPos - prevEncPosition;	
	sharedData->setLastRotation(delta);
	prevEncPosition = newPos;	

	if(this->sharedData->getState() != MachineState::IDLE && this->sharedData->getState() != MachineState::SETTINGS_OFFSET_ADJUSTING) {
		return;
	}

	lastTimestamps[currentInputPosition % MAX_INPUTS] = millis();

	boolean fastMode = evaluateFastMode();
	
	long factor;
	if (sharedData->speedButton->isPressed()) {
		factor =  INCREMENT_SLOW_IN_DENOM;
	} else if (fastMode) {
		factor = INCREMENT_NORMAL_IN_DENOM * 2;
	} else {
		factor = INCREMENT_NORMAL_IN_DENOM;
	}

	long totalDelta = factor * delta;

	lastValues[currentInputPosition % MAX_INPUTS] = totalDelta;
	currentInputPosition++;

	if(sharedData->getState() == MachineState::IDLE) {
		sharedData->setTargetPosition(sharedData->getTargetPosition() + totalDelta);
	} else if (sharedData->getState() == MachineState::SETTINGS_OFFSET_ADJUSTING) {
		sharedData->setOffset(sharedData->getOffset() + totalDelta);
	}

	sharedData->scheduleDisplayUpdate();
}

long TargetSelector::correctFractionValues(long targetPosition, long totalDelta, boolean isSLowEnabled) {
	if (isSLowEnabled) {
		return totalDelta;
	}
	int remainder = targetPosition % DENOMINATION;
	if(remainder == 0) {
		return totalDelta;
	}
	return totalDelta > 0 ? 
				totalDelta + (DENOMINATION - remainder) : 
				totalDelta - remainder;
}

boolean TargetSelector::evaluateFastMode() {
	int lastIndex = (currentInputPosition-3) % MAX_INPUTS;
	int currentIndex = (currentInputPosition) % MAX_INPUTS;
	if(lastIndex < 0) {
		lastIndex += MAX_INPUT;
	}
	
	long currentElement = lastTimestamps[currentIndex];
	long prevElement = lastTimestamps[lastIndex];
	return currentElement - prevElement < 200;
}

void TargetSelector::handleInputSelectionButton() {
	if (sharedData->getState() == IDLE) {
		if (sharedData->enterButton->rose() && hasCorrectInputSource) {
			correctAccidentalInputs();
			if (sharedData->getTargetPosition() != sharedData->getCurrentPosition()) {
				sharedData->setLastDistance(sharedData->getTargetPosition() - sharedData->getCurrentPosition());
				sharedData->switchState(MachineState::PREP_UNLOCK);
			} else {
				sharedData->setTargetPosition(sharedData->getCurrentPosition() + sharedData->getLastDistance());
			}
			sharedData->scheduleDisplayUpdate();	
		}
		if(sharedData->enterButton->fell()) {
			hasCorrectInputSource = true;
		}
	} else {
		hasCorrectInputSource = false;
	}
	
}

void TargetSelector::correctAccidentalInputs() {
	double correction = 0;
	long currentTimestamp = millis();
	for (int i = currentInputPosition - 1; i >= currentInputPosition - MAX_INPUTS; i--) {
		Serial.print("CHecking index: ");
		Serial.println(i);
		if (currentTimestamp - IGNORE_ACCIDENTAL_INPUT_DELAY_MS < lastTimestamps[i % MAX_INPUTS]) {
			Serial.print("removed accidental input " );
			Serial.println(lastValues[i % MAX_INPUTS]);
			correction += lastValues[i % MAX_INPUTS] * -1;
		} else {
			break;
		}
	}
	sharedData->setTargetPosition(sharedData->getTargetPosition() + correction);
}

void TargetSelector::handleMoveToConversionButton() {
	if (sharedData->getState() == IDLE && sharedData->moveToConversion->isPressed() && sharedData->moveToConversion->currentDuration() > TIMEOUT_MOVE_TO_CONVERSION) {
		sharedData->setTargetPosition(IDLE_POSITION_IN_DENOM);
		sharedData->switchState(PREP_UNLOCK);
	}
}

void TargetSelector::tick() {
	handleEncoder();
	handleInputSelectionButton();
	handleMoveToConversionButton();
}
