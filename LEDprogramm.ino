
  // Steuerung für unsere Esszimmerlampe
    
    #include <FastLED.h>
    #define NUM_LEDS 136
    #define DATA_PIN 3
    #define BRIGHTNESS  255
    #define COOLING  55
    #define SPARKING 120
    CRGB leds[NUM_LEDS];
    int Received=0;
    bool gReverseDirection = false;


void setup() {
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
    FastLED.setCorrection(TypicalLEDStrip);
    FastLED.setDither(BRIGHTNESS < 255);
    FastLED.setBrightness(BRIGHTNESS);
    delay(500);
    for (int j=0;j<256;j++)
    {
    for (int i=0;i<121;i++)
    {
    leds[i].r = 0; 
    leds[i].g = 0; 
    leds[i].b = j;
    }
    for (int i=120;i<137;i++)
    {
    leds[i].r = j; 
    leds[i].g = j; 
    leds[i].b = 255;
    }
    FastLED.show();
    delay (5);
    }
  Serial.begin(9600);

    
}

void loop() {
// if(Received == 0)
// {
     if(Serial.available())
 { 
    Received = Serial.read();
 }   
// }
 
 
if (Received == '1')
  {
    for (int i=120;i<137;i++)
    {
    leds[i].r = 255; 
    leds[i].g = 255; 
    leds[i].b = 255;
    }
    FastLED.show();
    delay (50);
    Received=0;
  }

  if (Received == '2')
  {
    for (int i=120;i<137;i++)
    {
    leds[i].r = 200; 
    leds[i].g = 155; 
    leds[i].b = 55;
    }
    FastLED.show();
    delay (50);
    Received=0;
  }
  
  if (Received == '3')
  {
    for (int i=120;i<137;i++)
    {
    leds[i].r = 0; 
    leds[i].g = 0; 
    leds[i].b = 0;
    }
    FastLED.show();
    delay (50);
    Received=0;
  }
  
  if (Received == '4')
  {
    for (int i=0;i<120;i++)
    {
    leds[i].r = 255; 
    leds[i].g = 0; 
    leds[i].b = 0;
    }
    FastLED.show();
    delay (50);
    Received=0;
  }

if (Received == '5')
  {
    for (int i=0;i<120;i++)
    {
    leds[i].r = 0; 
    leds[i].g = 255; 
    leds[i].b = 0;
    }
    FastLED.show();
    delay (50);
    Received=0;
  }

if (Received == '6')
  {
    for (int i=0;i<120;i++)
    {
    leds[i].r = 0; 
    leds[i].g = 0; 
    leds[i].b = 255;
    }
    FastLED.show();
    delay (50);
    Received=0;
  }

if (Received == '7')
  {
    for (int i=0;i<120;i++)
    {
    leds[i].r = 255; 
    leds[i].g = 255; 
    leds[i].b = 0;
    }
    FastLED.show();
    delay (50);
    Received=0;
  }

if (Received == '8')
  {
    for (int i=0;i<120;i++)
    {
    leds[i].r = 0; 
    leds[i].g = 0; 
    leds[i].b = 0;
    }
    FastLED.show();
    delay (50);
    Received=0;
  }

if (Received == '9')
  {
   Received=0;
    do {
  pride();
  FastLED.show(); 
  if(Serial.available())
  { 
    Received = Serial.read();  
  } 
  
  } while (Received == 0);

    
  }
  


if (Received == 'a')
{
  Received=0; 
  do {
  Fire2012(); // run simulation frame
  
  FastLED.show(); // display this frame
  FastLED.delay(16);
  if(Serial.available())
  { 
    Received = Serial.read();  
  } 
}   while (Received == 0);
}

}

void pride() 
{
  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;
 
  uint8_t sat8 = beatsin88( 87, 220, 250);
  uint8_t brightdepth = beatsin88( 341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88( 203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16;//gHue * 256;
  uint16_t hueinc16 = beatsin88(113, 1, 3000);
  
  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis ;
  sLastMillis  = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88( 400, 5,9);
  uint16_t brightnesstheta16 = sPseudotime;
  
  for( uint16_t i = 0 ; i < NUM_LEDS; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;

    brightnesstheta16  += brightnessthetainc16;
    uint16_t b16 = sin16( brightnesstheta16  ) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);
    
    CRGB newcolor = CHSV( hue8, sat8, bri8);
    
    uint16_t pixelnumber = i;
    pixelnumber = (NUM_LEDS-1) - pixelnumber;
    
    nblend( leds[pixelnumber], newcolor, 64);
  }
}


void Fire2012()
{
// Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS; j++) {
      CRGB color = HeatColor( heat[j]);
      int pixelnumber;
      if( gReverseDirection ) {
        pixelnumber = (NUM_LEDS-1) - j;
      } else {
        pixelnumber = j;
      }
      leds[pixelnumber] = color;
    }
}
