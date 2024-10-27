#include "MouseLVGL.h"
#include "lvgl.h"

static const char * TAG = "MouseLVGL";

void MouseLVGL::onMouseButtons(hid_mouse_report_t report, uint8_t last_buttons) {
    left_press = report.buttons & MOUSE_BUTTON_LEFT;
};

void MouseLVGL::onMouseMove(hid_mouse_report_t report) {
    ESP_LOGV(TAG, "buttons=0x%02x(%c%c%c%c%c), x=%d, y=%d, wheel=%d\n",
                  report.buttons,
                  (report.buttons & MOUSE_BUTTON_LEFT) ? 'L' : ' ',
                  (report.buttons & MOUSE_BUTTON_RIGHT) ? 'R' : ' ',
                  (report.buttons & MOUSE_BUTTON_MIDDLE) ? 'M' : ' ',
                  (report.buttons & MOUSE_BUTTON_BACKWARD) ? 'B' : ' ',
                  (report.buttons & MOUSE_BUTTON_FORWARD) ? 'F' : ' ',
                  report.x,
                  report.y,
                  report.wheel);

    point_x += report.x * this->dpi;
    point_x = constrain(point_x, 0, 480);

    point_y += report.y * this->dpi;
    point_y = constrain(point_y, 0, 320);
};

static void mouse_read(lv_indev_drv_t *indev_driver, lv_indev_data_t * data) {
    MouseLVGL * mouse = (MouseLVGL *) indev_driver->user_data;

    data->point.x = mouse->point_x;
    data->point.y = mouse->point_y;
    data->state = mouse->left_press ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL;
}

void MouseLVGL::begin(float dpi) {
    this->dpi = dpi;
    EspUsbHost::begin();
}

void MouseLVGL::useLVGL(const void *mouse_cursor_icon_src) {
    static lv_indev_drv_t indev_mouse_drv;
    lv_indev_drv_init(&indev_mouse_drv);
    indev_mouse_drv.type = LV_INDEV_TYPE_POINTER;
    indev_mouse_drv.read_cb = mouse_read;
    indev_mouse_drv.user_data = this;
    lv_indev_t * indev_mouse = lv_indev_drv_register(&indev_mouse_drv);
    lv_obj_t *mouse_cursor_img = lv_img_create(lv_scr_act());
    lv_img_set_src(mouse_cursor_img, mouse_cursor_icon_src);
    lv_indev_set_cursor(indev_mouse, mouse_cursor_img);
}

void MouseLVGL::loop() {
    this->task();
}

MouseLVGL Mouse;
