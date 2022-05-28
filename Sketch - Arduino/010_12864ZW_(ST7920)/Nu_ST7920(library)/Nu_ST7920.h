#ifndef Nu_ST7920_h
#define Nu_ST7920_h
#include <Arduino.h>
#include <inttypes.h>
#include <SPI.h>
class ZW12864 {
  public:
    ZW12864(uint8_t slave, uint8_t rst);
    void Send_command(uint8_t  Data);
    void Send_data(uint8_t Data);
    void Init(void);
    void Send_symbol_text(uint8_t y, uint8_t x, char *message);
    void Graphic_mode(bool enable);
    const char* FloatToChar (float f, signed char width);
    void Send_Float_buffer(uint8_t y, uint8_t x);
    void Send_Float_buffer_minus(uint8_t y, uint8_t x);
	void Draw_bitmap( uint8_t *bitmap);
	void Update(void);
	void Clean_Frame_buffer(void);
	void Draw_pixel(uint8_t x, uint8_t y);
	void Clean_pixel(uint8_t x, uint8_t y);
  private:
    uint8_t Slave_Select_Pin;
    uint8_t RST_Pin;
    char Float_buffer[10] = {0,};
    char Float_buffer_minus[10] = {0,};
	byte Frame_buffer[1024] = {0,}; 
	uint8_t ZW12864_width = 128; 
    uint8_t ZW12864_height = 64; 
};
#endif
