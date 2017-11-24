const int pinW = 3;
const int pinR = 5;
const int pinG = 6;
const int pinB = 9;

int valueSensorPir = 0; 

void setup() {
	Serial.begin(9600);
}

void loop() {
	analogWrite(analogOutPin, outputValue);
	Serial.println(valueSensorPir);

	delay(2);
}
