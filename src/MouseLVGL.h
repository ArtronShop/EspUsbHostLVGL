#ifndef __MOUSE_LVGL_H__
#define __MOUSE_LVGL_H__

#include "EspUsbHost.h"
#include "lvgl.h"

extern const lv_img_dsc_t mouse_cursor_icon;

class MouseLVGL : public EspUsbHost {
    public:
        bool left_press = false;
        int16_t point_x = 0;
        int16_t point_y = 0;

        float dpi = 2.5f;

        void onMouseButtons(hid_mouse_report_t, uint8_t) ;
        void onMouseMove(hid_mouse_report_t) ;

        void begin(float dpi=2.5f) ;
        void useLVGL(const void *mouse_cursor_icon_src=&mouse_cursor_icon) ;
        void loop() ;
        
        void setDPI(float) ;
};

extern MouseLVGL Mouse;
#endif