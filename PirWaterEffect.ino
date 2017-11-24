const int pinW = 3;
const int pinR = 5;
const int pinG = 6;
const int pinB = 9;
const int pinPir = 12;

int valueSensorPir = 0;
enum WaterState {
	Idle,
	FadeIn,
	On,
	FadeOut
};
WaterState lastState = Idle;
WaterState currentState = Idle;
int brightness = 0;

String waterStateToString(WaterState in) {
	switch (in) {
		case Idle:
			return("Idle");
			break;
		case FadeIn:
			return("FadeIn");
			break;
		case On:
			return("On");
			break;
		case FadeOut:
			return("FadeOut");
			break;
	}
}

void checkStates() {
	valueSensorPir = digitalRead(pinPir);
	switch (lastState) {
		case Idle:
			if (valueSensorPir == HIGH) {
				currentState = FadeIn;
			}
			break;
		case FadeIn:
			if (brightness > 10) {
				currentState = On;
			}
			break;
		case On:
			if (valueSensorPir == LOW) {
				currentState = FadeOut;
			}
			break;
		case FadeOut:
			if (brightness <= 0) {
				currentState = Idle;
			}
			break;
	}
	if (lastState != currentState) {
		Serial.println(waterStateToString(currentState));
	}
	lastState = currentState;
}

void actStages() {
	switch (lastState) {
		case Idle:
			break;
		case FadeIn:
			brightness += 1;
      Serial.println(brightness);
			break;
		case On:
			break;
		case FadeOut:
			brightness -= 1;
      Serial.println(brightness);
			break;
	}
}

void setup() {
	Serial.begin(9600);
	pinMode(pinPir, INPUT);
}

void loop() {
	checkStates();
	actStages();
	//analogWrite(pinW, brightness);

	delay(100);
}
