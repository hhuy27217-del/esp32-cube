/*
  ESP32 3D ROTATING CUBE DEMO
  -----------------------------------------
  Hiệu ứng khối lập phương 3D xoay (wireframe) hiển thị trên màn hình TFT
  gắn với ESP32 (vd: board "Cheap Yellow Display" ESP32-2432S028,
  hoặc ESP32 + màn hình ILI9341 / ST7789 nối qua SPI).

  Thư viện cần cài (qua Library Manager trong Arduino IDE):
    - TFT_eSPI (Bodmer)
*/

#include <TFT_eSPI.h>
#include <math.h>

TFT_eSPI tft = TFT_eSPI();

float cubeVertices[8][3] = {
  {-1, -1, -1}, {1, -1, -1}, {1, 1, -1}, {-1, 1, -1},
  {-1, -1,  1}, {1, -1,  1}, {1, 1,  1}, {-1, 1,  1}
};

int cubeEdges[12][2] = {
  {0,1},{1,2},{2,3},{3,0},
  {4,5},{5,6},{6,7},{7,4},
  {0,4},{1,5},{2,6},{3,7}
};

float angleX = 0, angleY = 0;
int screenW, screenH;
float scale = 60;
float distanceZ = 4;

int prevX[8], prevY[8];

void setup() {
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  screenW = tft.width();
  screenH = tft.height();
}

void project(float v[3], float angleX, float angleY, int &outX, int &outY) {
  float y1 = v[1] * cos(angleX) - v[2] * sin(angleX);
  float z1 = v[1] * sin(angleX) + v[2] * cos(angleX);
  float x1 = v[0];

  float x2 = x1 * cos(angleY) + z1 * sin(angleY);
  float z2 = -x1 * sin(angleY) + z1 * cos(angleY);
  float y2 = y1;

  float factor = distanceZ / (distanceZ + z2);
  outX = screenW / 2 + (int)(x2 * scale * factor);
  outY = screenH / 2 + (int)(y2 * scale * factor);
}

void loop() {
  int curX[8], curY[8];

  for (int i = 0; i < 8; i++) {
    project(cubeVertices[i], angleX, angleY, curX[i], curY[i]);
  }

  if (prevX[0] != 0 || prevY[0] != 0) {
    for (int i = 0; i < 12; i++) {
      int a = cubeEdges[i][0], b = cubeEdges[i][1];
      tft.drawLine(prevX[a], prevY[a], prevX[b], prevY[b], TFT_BLACK);
    }
  }

  for (int i = 0; i < 12; i++) {
    int a = cubeEdges[i][0], b = cubeEdges[i][1];
    tft.drawLine(curX[a], curY[a], curX[b], curY[b], TFT_CYAN);
  }

  for (int i = 0; i < 8; i++) {
    prevX[i] = curX[i];
    prevY[i] = curY[i];
  }

  angleX += 0.03;
  angleY += 0.02;

  delay(20);
}
