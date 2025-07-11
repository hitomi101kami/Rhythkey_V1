#include <Keyboard.h>
#include <Mouse.h>

// === Encoder 腳位定義 ===
#define ENCODER_LEFT_A 2
#define ENCODER_LEFT_B 3
#define ENCODER_RIGHT_A 4
#define ENCODER_RIGHT_B 5

int lastLeftA = HIGH;
int lastRightA = HIGH;

// === 滑動電位器腳位 ===
#define SLIDER_PIN A0
int lastSliderValue = 0;
const int SLIDER_THRESHOLD = 5;

// === 鍵盤矩陣定義 ===
const int ROWS = 2;
const int COLS = 6;
const int rowPins[ROWS] = {6, 7};       // 實際腳位需依原理圖調整
const int colPins[COLS] = {8, 9, 10, 11, 12, 13};

char keyMap[ROWS][COLS] = {
  {'A', 'S', 'D', 'F', 'G', 'H'},        // 第一列鍵值
  {'J', 'K', 'L', ';', '\'', 'Enter'}   // 第二列鍵值
};

bool keyState[ROWS][COLS] = {};

void setup() {
  // === 初始化輸入腳位 ===
  for (int r = 0; r < ROWS; r++) {
    pinMode(rowPins[r], INPUT_PULLUP);
  }
  for (int c = 0; c < COLS; c++) {
    pinMode(colPins[c], OUTPUT);
    digitalWrite(colPins[c], HIGH);
  }

  // === 初始化編碼器 ===
  pinMode(ENCODER_LEFT_A, INPUT_PULLUP);
  pinMode(ENCODER_LEFT_B, INPUT_PULLUP);
  pinMode(ENCODER_RIGHT_A, INPUT_PULLUP);
  pinMode(ENCODER_RIGHT_B, INPUT_PULLUP);

  // === 初始化滑鼠與鍵盤 ===
  Mouse.begin();
  Keyboard.begin();
}

void loop() {
  checkButtonMatrix();
  handleSlider();
  handleEncoders();
}

// === 鍵盤掃描函式 ===
void checkButtonMatrix() {
  for (int c = 0; c < COLS; c++) {
    digitalWrite(colPins[c], LOW);
    for (int r = 0; r < ROWS; r++) {
      bool pressed = !digitalRead(rowPins[r]);
      if (pressed && !keyState[r][c]) {
        Keyboard.press(keyMap[r][c]);
        keyState[r][c] = true;
      }
      if (!pressed && keyState[r][c]) {
        Keyboard.release(keyMap[r][c]);
        keyState[r][c] = false;
      }
    }
    digitalWrite(colPins[c], HIGH);
  }
}

// === 滑動電位器模擬滑鼠 ===
void handleSlider() {
  int value = analogRead(SLIDER_PIN);
  if (abs(value - lastSliderValue) > SLIDER_THRESHOLD) {
    int delta = map(value - lastSliderValue, -1023, 1023, -10, 10);
    Mouse.move(delta, 0);
    lastSliderValue = value;
  }
}

// === 編碼器模擬鍵盤輸出 ===
void handleEncoders() {
  // 左旋鈕（藍條）
  int currentLeftA = digitalRead(ENCODER_LEFT_A);
  if (currentLeftA != lastLeftA) {
    if (digitalRead(ENCODER_LEFT_B) != currentLeftA) {
      Keyboard.press('Q'); delay(5); Keyboard.release('Q');
    } else {
      Keyboard.press('R'); delay(5); Keyboard.release('R');
    }
  }
  lastLeftA = currentLeftA;

  // 右旋鈕（粉條）
  int currentRightA = digitalRead(ENCODER_RIGHT_A);
  if (currentRightA != lastRightA) {
    if (digitalRead(ENCODER_RIGHT_B) != currentRightA) {
      Keyboard.press('U'); delay(5); Keyboard.release('U');
    } else {
      Keyboard.press('P'); delay(5); Keyboard.release('P');
    }
  }
  lastRightA = currentRightA;
}
