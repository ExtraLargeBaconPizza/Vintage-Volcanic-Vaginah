/*
    Board: NodeMCU-ESP32s
	Upload Speed: 921600
	Pinout Reference: https://esphome.io/devices/nodemcu_esp32.html
    Ws2815 Data Sheet: https://www.led-stuebchen.de/download/WS2815.pdf

    Notes:
    - Cannot use loop in addLeds function because the pin number must be a runtime constant. Gross, I know.
    - Ensure esp32 is grounded to psu, even if powered by seperate usb
    - Ws2815 are unidirectional. You must connect to the female end of the strip to a 4-pin  male JST-SM from the board
    - Parrallel output: - writing to each pixel takes 30µs, so FastLED.show() can take while because it has to iterate over all pixels and strips in series (ie one at a time)
                        - parrallel will write to strips in parrallel and must be enabled
                        - reddit explination: https://www.reddit.com/r/FastLED/comments/aqlb94/troubleshooting_slow_performance_tied_to/
                        - https://github.com/FastLED/FastLED/wiki/Parallel-Output
                        - solution: https://www.reddit.com/r/FastLED/comments/klw88g/are_there_magical_words_to_summon_parallel_output/
    - Backup data line: - Do not wire backup data line to seperate pins. This can cause noise over longer wire lengths, which can result in random flickering
                        - Either wire both data lines to the same pin or wire the backup data line directly to the ground (as close to strip as possible)
                        - https://www.reddit.com/r/WLED/comments/yw10qa/ws2815_backup_data_line_question/
*/

#include <FastLED.h>

#include "Eruption.h"
#include "LavaFlow.h"
#include "LedStripControls.h"
#include "VintageVolcanicVaginah.h"

void setup()
{
    SetupLedStrips();
}

void loop()
{
    UpdateLavaFlow();
    UpdateEruption();
    UpdateLedStrips();

    delay(GetSpeedDelay());
}