#include <DigisparkOLED.h>
// #include <DigiCDC.h>
#include <Wire.h>
// ============================================================================

#include "huffman_bitmaps.h"

void setup()
{
    oled.begin();
    oled.clear();
}

void huffman_bitmap(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, const uint8_t huffman_bitmap[])
{
    uint16_t j = 0;
    uint8_t bit_index = 0;
    uint8_t list_index;
    uint8_t y, x;

    uint8_t val = pgm_read_byte(&huffman_bitmap[j++]);

    for (y = y0; y < y1; y++) {
        // oled.setCursor(((unsigned)(x0-1))%128, y);
        oled.setCursor(x0, y);
        oled.ssd1306_send_data_start();
        for (x = x0; x < x1; x++) {
            // search throught flattened tree
            list_index = 0;
            unsigned char _0 = pgm_read_byte(&huffman_table_list[list_index][0]);
            unsigned char _1 = pgm_read_byte(&huffman_table_list[list_index][1]);
            while(_0 != _1) {
                list_index = val & 1 ? _1 : _0;

                val >>= 1;
                bit_index += 1;

                if(bit_index >= 8) {
                    val = pgm_read_byte(&huffman_bitmap[j++]);
                    bit_index = 0;
                }
                _0 = pgm_read_byte(&huffman_table_list[list_index][0]);
                _1 = pgm_read_byte(&huffman_table_list[list_index][1]);
            }

            oled.ssd1306_send_data_byte(_0);
        }
        oled.ssd1306_send_data_stop();
    }
    // oled.setCursor(0, 0);
}

void loop()
{
    for (int i = 0; i < num_bitmaps; i++) {
        huffman_bitmap(0, 1, 128, 7, huffman_bitmaps[i]);
        delay(100);

    }
}