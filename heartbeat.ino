#include <FastLED.h>
#define NUM_LEDS 40
#define HISTORYSIZE 100

#define DATA_PIN 2

// Define the array of leds
CRGB leds[NUM_LEDS];


int pulsepin = 0;
int ledpin = 13;
int threshold = 400;
int history[HISTORYSIZE];
int historyidx = 0;
int range = 0;
int hue = 0;
int low = 10000;
int high = 0;
int mid;
int last;

void fadeall() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].nscale8(250);
  }
}
void scroll() {
  for (int i = 0; i < NUM_LEDS - 1; i++) {
    leds[i] = leds[i + 1];
  }
}
void setup() {
  pinMode(ledpin, OUTPUT);
  Serial.begin(115200);
  LEDS.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
  LEDS.setBrightness(84);

}

int average(int data)
{
  history[historyidx] = data;
  historyidx++;
  if (historyidx == HISTORYSIZE)
    historyidx = 0;
  low = 10000;
  high = 0;
  for (int i = 0; i < HISTORYSIZE; i++)
  {
    int point = history[i];
    if (point > high)
      high = point;
    if (point < low)
      low = point;
  }
  range = high - low;
  if (range < 40)
    return -1;
  mid = (range / 2) + low;
  return (( data - mid) * 255) / (high - mid);
}

void graph(int data, int mid)
{
  for (int i = 0; i < data ; i += 3)
  {
    if (i > mid)
      Serial.print("+");
    else
      Serial.print("-");
  }
  Serial.println("X");
}

void pulses(int data)
{
  scroll();
  if (data >= 0)
    leds[NUM_LEDS - 1] = CHSV(hue++, 255, 255);
  else
    leds[NUM_LEDS - 1] = CRGB(0, 32, 0);
}

void ramping(int data)
{
  hue++;
  for (int i = 0; i < NUM_LEDS; i++)
  {
    if (i > data)
      leds[i] = CHSV(hue + i, 255, 255);
    else
      leds[i] = CRGB(0, 32, 0);

  }

}

void volume(int data)
{
  hue++;
  for (int i = 0; i < NUM_LEDS; i++)
  {
    if (data >= 0)
      leds[i] = CHSV(hue + i, 255, data);
    else
      leds[NUM_LEDS - 1] = CRGB(0, 4, 0);
  }
}

void loop() {
  int data = analogRead(pulsepin);
  int level = average(data);
  if (level >=0)
    digitalWrite(ledpin, HIGH);
  else
    digitalWrite(ledpin, LOW);
  graph(data, mid);
  pulses(level);
  FastLED.show();

  delay(10);
}





