#include <FastLED.h>
#define NUM_LEDS (40*144/60)
#define DATA_PIN 2
CRGB leds[NUM_LEDS];
int  hue = 0;
int pushButton = 4;

#define DEBOUNCETIME 4
#define MAXPULSE 50

void shiftLeft(CRGB data)
{
  int i ;
  for ( i = (NUM_LEDS / 2) - 1; i > 0; i--)
    leds[i] = leds[i - 1];
  leds[i] = data;
}

void shiftRight(CRGB data)
{
  int i;
  for (i = ( NUM_LEDS / 2); i < NUM_LEDS - 1; i++)
    leds[i] = leds[i + 1];
  leds[i] = data;
}

void shiftin(CRGB color)
{
  shiftLeft(color);
  shiftRight(color);
  shiftLeft(color);
  shiftRight(color);

  FastLED.show();
}

void animateStrip(int data)
{
  hue += 5;
  CRGB color = CRGB(0, 32, 0);
  if (data > 0)
    color = CHSV(hue, 255, 255);
  shiftin(color);
}


void setup() {
  LEDS.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
  LEDS.setBrightness(255);

  // initialize serial communication at 9600 bits per second:

  Serial.begin(9600);

  // make the pushbutton's pin an input:

  pinMode(pushButton, INPUT);

}


int debounce(int data)
{
  static int last ;
  static int count;
  static int state;
  if (data == last)
  {
    count++;
    if (count > DEBOUNCETIME)
    {
       state = data;
    }
    if(count>MAXPULSE)
    {
      count=MAXPULSE;
      return 0;
    }
  }
  else
    count = 0;
  last = data;
  return state;
}

void loop() {

  // read the input pin:

  int buttonState = digitalRead(pushButton);
  buttonState = debounce(buttonState);
  animateStrip(buttonState);
  Serial.println(buttonState);

  delay(10);        // delay in between reads for stability

}
