#include <Wire.h>
#include <HijelHID_BLEMouse.h>

#define MPU_ADDR 0x68
#define LEFT_BTN 3
#define RIGHT_BTN 10

#define MOUSE_LEFT MouseButton::Left
#define MOUSE_RIGHT MouseButton::Right

HijelBLEMouse mouse("MPU_Mouse", "Hijel");

int16_t gx, gy, gz;

float smoothed_dx = 0;
float smoothed_dy = 0;

unsigned long lastPrint = 0;

void setup() {
    Serial.begin(115200);

    Wire.begin(8, 9);

    pinMode(LEFT_BTN, INPUT_PULLUP);
    pinMode(RIGHT_BTN, INPUT_PULLUP);

    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x6B);
    Wire.write(0);
    Wire.endTransmission();

    mouse.setBatteryLevel(100);
    mouse.begin();

    Serial.println("INIT DONE");
}

void loop() {

    if (!mouse.isPaired()) {
        if (millis() - lastPrint > 1000) {
            Serial.println("BLE NOT PAIRED");
            lastPrint = millis();
        }
        delay(100);
        return;
    }

    if (millis() - lastPrint > 1000) {
        Serial.println("BLE CONNECTED");
        lastPrint = millis();
    }

    bool leftPressed = (digitalRead(LEFT_BTN) == LOW);
    bool rightPressed = (digitalRead(RIGHT_BTN) == LOW);

    if (leftPressed)
        mouse.press(MOUSE_LEFT);
    else
        mouse.release(MOUSE_LEFT);

    if (rightPressed)
        mouse.press(MOUSE_RIGHT);
    else
        mouse.release(MOUSE_RIGHT);

    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x43);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_ADDR, 6);

    if (Wire.available() == 6) {
        gx = Wire.read() << 8 | Wire.read();
        gy = Wire.read() << 8 | Wire.read();
        gz = Wire.read() << 8 | Wire.read();
    } else {
        Serial.println("MPU READ ERROR");
        return;
    }

    float sensitivity = 0.003;

    float dx = -gy * sensitivity;
    float dy = -gx * sensitivity;

    if (abs(dx) < 0.5) dx = 0;
    if (abs(dy) < 0.5) dy = 0;

    smoothed_dx = smoothed_dx * 0.7 + dx * 0.3;
    smoothed_dy = smoothed_dy * 0.7 + dy * 0.3;

    int move_x = constrain((int)smoothed_dx, -20, 20);
    int move_y = constrain((int)smoothed_dy, -20, 20);

    mouse.move(move_x, move_y);

    if (millis() - lastPrint > 200) {
        Serial.print("GX: "); Serial.print(gx);
        Serial.print(" GY: "); Serial.print(gy);
        Serial.print(" DX: "); Serial.print(dx);
        Serial.print(" DY: "); Serial.print(dy);
        Serial.print(" MX: "); Serial.print(move_x);
        Serial.print(" MY: "); Serial.print(move_y);
        Serial.print(" L: "); Serial.print(leftPressed);
        Serial.print(" R: "); Serial.println(rightPressed);
        lastPrint = millis();
    }

    delay(10);
}
