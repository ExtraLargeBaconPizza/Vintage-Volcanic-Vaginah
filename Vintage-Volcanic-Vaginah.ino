    #include <Adafruit_NeoPixel.h>

    #define PIN 6
    #define NUM_LEDS 300

    Adafruit_NeoPixel _strip;
    
    int _currentDropIndex;
    int _currentDropPosition;

    void setup()
    {
        _strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

        _strip.begin();

        clearAll();

        _strip.show();
    }

    void loop() 
    {
        updateDrop();

        _strip.show();
    }

    void updateDrop()
    {
        if (_currentDropIndex == 0)
        {
            clearAll();

            return;
        }

        if (_currentDropPosition == _currentDropIndex)
        {
            _currentDropIndex--;
            _currentDropPosition = 0;
        }
        else
        {
            setPixel (_currentDropPosition, 0, 0, 255);

            if ( _currentDropPosition > 0)
            {
                setPixel (_currentDropPosition - 1, 0, 0, 0);
            }
        }
        
        _currentDropPosition++;
    }

    void clearAll()
    {
        for (int i = 0; i < NUM_LEDS; i++)
        {
            setPixel (i, 0, 0, 0);
        }
        
        _currentDropIndex = NUM_LEDS;
        _currentDropPosition = 6;
    }

    void joeBigGuns()
    {
        for (int i = 0; i < NUM_LEDS; i++)
        {
            setPixel (i, 255, 255, 255);
        }
    }

    void setPixel (int Pixel, byte red, byte green, byte blue) 
    {
        _strip.setPixelColor(Pixel, _strip.Color(red, green, blue));
    }