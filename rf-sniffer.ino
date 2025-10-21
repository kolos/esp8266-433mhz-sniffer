// ESP8266 433MHz Sniffer with burst filtering + decode
// RXB6 DATA -> D2 (GPIO4) on ESP8266

#define RF_PIN 4
#define MIN_PULSE 150
#define SYNC_GAP 4500
#define MAX_PULSES 200
#define MIN_BURST_LEN 30
#define REPEAT_REQUIRED 2

volatile unsigned long lastChange = 0;
volatile unsigned int pulseWidth = 0;
volatile bool newPulse = false;

unsigned int burst[MAX_PULSES];
int burstLen = 0;

unsigned int lastBurst[MAX_PULSES];
int lastBurstLen = 0;
int repeatCount = 0;

void ICACHE_RAM_ATTR handleInterrupt() {
  unsigned long now = micros();
  unsigned int pw = now - lastChange;
  lastChange = now;

  if (pw > MIN_PULSE) {
    pulseWidth = pw;
    newPulse = true;
  }
}

bool burstsEqual(unsigned int *a, int lenA, unsigned int *b, int lenB) {
  if (lenA != lenB) return false;
  for (int i = 0; i < lenA; i++) {
    int diff = abs((int)a[i] - (int)b[i]);
    if (diff > 200) return false; // jitter tolerance
  }
  return true;
}

void decodeBurst(unsigned int *data, int len) {
  Serial.print("BITS: ");
  for (int i = 1; i < len; i++) { // skip sync gap
    unsigned int pw = data[i];
    if (pw > 200 && pw < 500) {
      Serial.print("0");
    } else if (pw >= 700 && pw < 950) {
      Serial.print("1");
    } else {
      Serial.print("_");
    }
  }
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  pinMode(RF_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(RF_PIN), handleInterrupt, CHANGE);

  Serial.println("433MHz Sniffer with filtered decode...");
}

void loop() {
  static unsigned long lastPulseTime = 0;

  if (newPulse) {
    noInterrupts();
    unsigned int pw = pulseWidth;
    newPulse = false;
    interrupts();

    if (burstLen < MAX_PULSES) {
      burst[burstLen++] = pw;
    }
    lastPulseTime = micros();
  }

  if (burstLen > 0 && (micros() - lastPulseTime > SYNC_GAP)) {
    if (burstLen >= MIN_BURST_LEN) {
      if (burstsEqual(burst, burstLen, lastBurst, lastBurstLen)) {
        repeatCount++;
        if (repeatCount >= REPEAT_REQUIRED) {
          // Print raw burst
          Serial.print("RAW: ");
          for (int i = 0; i < burstLen; i++) {
            Serial.print(burst[i]);
            if (i < burstLen - 1) Serial.print(",");
          }
          Serial.println();
          // Decode into bits
          decodeBurst(burst, burstLen);
          Serial.println();
          repeatCount = 0;
        }
      } else {
        memcpy(lastBurst, burst, burstLen * sizeof(unsigned int));
        lastBurstLen = burstLen;
        repeatCount = 1;
      }
    }
    burstLen = 0;
  }
}
