const int pinW = 3;
const int pinR = 9;
const int pinG = 6;
const int pinB = 5;
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
float outHue = 0.45;
float outSaturation = 1.0;
float outBrightness = 0;

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

float hueToRgb(float p, float q, float t) {
	if (t < (float)0)
			t += (float)1;
	if (t > (float)1)
			t -= (float)1;
	if (t < (float)1/(float)6)
			return p + (q - p) * (float)6 * t;
	if (t < (float)1/(float)2)
			return q;
	if (t < (float)2/(float)3)
			return p + (q - p) * ((float)2.0/ (float)3.0 - t) * (float)6;
	return p;
}
void hslToRgb(float hue, float saturation, float lightness, int rgb[]) {
	float r, g, b;

	if (saturation == (float)0) {
			r = g = b = lightness; // achromatic
	} else {
			float q = lightness < 0.5 ? lightness * (1 + saturation) : lightness + saturation - lightness * saturation;
			float p = 2 * lightness - q;
			r = hueToRgb(p, q, hue + (float)1/(float)3);
			g = hueToRgb(p, q, hue);
			b = hueToRgb(p, q, hue - (float)1/(float)3);
	}
	rgb[0] = (int) (r * 255);
	rgb[1] = (int) (g * 255);
	rgb[2] = (int) (b * 255);
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
			if (deltaIdleTime > 2000 && currentPir == HIGH) {
				currentState = FadeIn;
			}
			break;
		case FadeIn:
			if (outBrightness >= 0.5) {
				currentState = On;
				onStateTime = millis();
			}
			break;
		case On:
			unsigned long deltaOnTime;
			deltaOnTime = millis()-onStateTime;
			if (deltaOnTime > 2000 && currentPir == LOW) {
				currentState = FadeOut;
			}
			break;
		case FadeOut:
			if (outBrightness <= 0.45) {
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
			outBrightness += 0.005;
			break;
		case On:
			break;
		case FadeOut:
			outBrightness -= 0.01;
			//if (outBrightness < 0 ) {
			//	outBrightness = 0;
			//}
			break;
	}
	
	if (lastState == FadeIn || lastState == FadeOut) {
		int nRGB[3] = {0,0,0};
		hslToRgb(outHue, outSaturation, outBrightness, nRGB);
		Serial.print(nRGB[0]);
		Serial.print(" ");
		Serial.print(nRGB[1]);
		Serial.print(" ");
		Serial.print(nRGB[2]);
		Serial.print(" \n");
		//Serial.println(outBrightness);
		analogWrite(pinR, nRGB[0]);
		analogWrite(pinG, nRGB[1]);
		analogWrite(pinB, nRGB[2]);
	}
}

void setup() {
	Serial.begin(9600);
	pinMode(pinPir, INPUT);
}

void loop() {
	checkStates();
	actStages();
	delay(50);
}
