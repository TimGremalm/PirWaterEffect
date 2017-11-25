const int pinW = 3;
const int pinR = 5;
const int pinG = 6;
const int pinB = 9;
const int pinPir = 12;

int lastPir = 0;
int currentPir = 0;
enum WaterState {
	Idle,
	FadeIn,
	On,
	FadeOut
};
WaterState lastState = Idle;
WaterState currentState = Idle;
unsigned long onStateTime = 0;
unsigned long idleStateTime = 0;
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
	currentPir = digitalRead(pinPir);
	if (lastPir != currentPir) {
		if (currentPir == HIGH) {
			Serial.println("Pir On");
		} else {
			Serial.println("Pir Off");
		}
	}
	lastPir = currentPir;

	switch (lastState) {
		case Idle:
			unsigned long deltaIdleTime;
			deltaIdleTime = millis()-idleStateTime;
			if (deltaIdleTime > 5000 && currentPir == HIGH) {
				currentState = FadeIn;
			}
			break;
		case FadeIn:
			if (brightness >= 10) {
				currentState = On;
				onStateTime = millis();
			}
			break;
		case On:
			unsigned long deltaOnTime;
			deltaOnTime = millis()-onStateTime;
			if (deltaOnTime > 5000 && currentPir == LOW) {
				currentState = FadeOut;
			}
			break;
		case FadeOut:
			if (brightness <= 0) {
				currentState = Idle;
				idleStateTime = millis();
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
