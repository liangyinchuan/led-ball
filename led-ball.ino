// 引入库文件
#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>


// 定义中断引脚
int pbIn = 0;
// 定义测试LED状态展示引脚
int ledOut = LED_BUILTIN;
// 定义初始中断状态
volatile int state = LOW;
// 定义初始类型
int type = 1;
// 定义初始led状态
int ledState = LOW;

// 定义球相关参数
#define PIN_LEDS 6.0
#define NUMPIXELS 194

#define LIGHTNESS 0.015f
#define BRIGHTNESS 3

RgbColor BLACK(0, 0, 0);
RgbColor RED(BRIGHTNESS, 0, 0);
RgbColor GREEN(0, BRIGHTNESS, 0);
RgbColor BLUE(0, 0, BRIGHTNESS);
RgbColor YELLOW(BRIGHTNESS, BRIGHTNESS, 0);
RgbColor WHITE(BRIGHTNESS, BRIGHTNESS, BRIGHTNESS);

NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> leds(NUMPIXELS, PIN_LEDS);

NeoPixelAnimator animations(10); // NeoPixel animation management object

#define NUM_RINGS 11
#define RING_MAX_PIXELS 26
#define RING_MIN_PIXELS 8

// 定义全部灯珠信息
byte RINGS[NUM_RINGS][RING_MAX_PIXELS] = {
  {8,1,2,3,4,5,6,7},
  {22,9,10,11,12,13,14,15,16,17,18,19,20,21},
  {40,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39},
  {60,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59},
  {84,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83},
  {110,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109},
  {111,134,133,132,131,130,129,128,127,126,125,124,123,122,121,120,119,118,117,116,115,114,113,112},
  {135,154,153,152,151,150,149,148,147,146,145,144,143,142,141,140,139,138,137,136},
  {155,172,171,170,169,168,167,166,165,164,163,162,161,160,159,158,157,156},
  {173,186,185,184,183,182,181,180,179,178,177,176,175,174},
  {187,194,193,192,191,190,189,188}
};

byte RING_SIZES[NUM_RINGS] = {8, 14, 18, 20, 24, 26, 24, 20, 18, 14, 8};
unsigned long startMillis = 0;
short animation = 0;

// 按钮时延记录
unsigned long buttonTime = 0;

void setup() {
  // 定义波特率
  Serial.begin(19200);
  // 定义LED输出引脚
  pinMode(ledOut,OUTPUT);

  initRandom();
  leds.Begin();
  leds.ClearTo(BLACK);
  leds.Show();

  // 中断回调函数调用
  attachInterrupt(pbIn, stateChange, RISING);
  Serial.println("现在进入了 1 号执行模式");
  animations.StartAnimation(0, 500, rayAnimation);
}

// 检测中断接口状态
void stateChange(){

  int now = millis();
  Serial.println(buttonTime);
  if(300 > (now - buttonTime)){
      return;
  }
  
  buttonTime = now;
  
  type++;
  ledState = !ledState;
  digitalWrite(ledOut, ledState);

    switch(type){
      
      case 1:
        Serial.println("现在进入了 1 号执行模式");
        animations.StartAnimation(0, 500, rayAnimation);
      break;
      
      case 2:
        Serial.println("现在进入了 2 号执行模式");
        animations.StartAnimation(0, 1000, ringAnimation);
        break;
        
      case 3:
        Serial.println("现在进入了 3 号执行模式");
        animations.StartAnimation(0, 2000, ringsRainbow);
        break;
        
      case 4:
        Serial.println("现在进入了 4 号执行模式");
        animations.StartAnimation(0, 2000, raysRainbow);
        break;
        
        
      case 5:
        Serial.println("现在进入了 5 号执行模式");
        animations.StartAnimation(0, 6000, rainbowAnimation);
        break;
        
        
      case 6:
        Serial.println("现在进入了 6 号执行模式");
        animations.StartAnimation(0, 100, randomAnimation);
        break;
        
      default: 
        type = 1;
        Serial.println("现在进入了 1 号执行模式");
        animations.StartAnimation(0, 500, rayAnimation);
        break;
    }
    Serial.println(type);
}

void loop() {
animations.UpdateAnimations();

  leds.Show();
  delay(10);

  if (false && (startMillis == 0 || startMillis + 20000 < millis())) { // 30s
    startMillis = millis();
    switch (animation) {
      case 0: 
        animations.StartAnimation(0, 500, rayAnimation);
        break;
      case 1: 
        animations.StartAnimation(0, 1000, ringAnimation);
        break;
      case 2: 
        animations.StartAnimation(0, 2000, ringsRainbow);
        break;
      case 3: 
        animations.StartAnimation(0, 2000, raysRainbow);
        break;
      case 4: 
        animations.StartAnimation(0, 6000, rainbowAnimation);
        break;
      case 5:
        animations.StartAnimation(0, 100, randomAnimation);
        animation = -1;
        break;
    }
    animation ++;
  }
}

void initRandom() {
  // random works best with a seed that can use 31 bits
  // analogRead on a unconnected pin tends toward less than four bits
  uint32_t seed = analogRead(0);
  delay(1);

  for (int shifts = 3; shifts < 31; shifts += 3) {
    seed ^= analogRead(0) << shifts;
    delay(1);
  }

  // Serial.println(seed);
  randomSeed(seed);
}


void rainbowAnimation(const AnimationParam& param) {
  HslColor color = HslColor(param.progress, 1.0f, LIGHTNESS);
  leds.ClearTo(color);

  if (param.state == AnimationState_Completed) {
    animations.RestartAnimation(0);
  }
}
void randomAnimation(const AnimationParam& param) {
  float hue;
  HslColor color;

  if (param.state == AnimationState_Completed) {
    for (byte i = 0; i < 194; i ++) {
      hue = random(0, 1000) / 1000.0f;
      color = HslColor(hue, 1.0f, LIGHTNESS);
      leds.SetPixelColor(i, color);
    }

    animations.RestartAnimation(0);
  }
}


void raysRainbow(const AnimationParam& param) {
  HslColor color;
  float hue;

  for (int i = 0; i < RING_MAX_PIXELS; i++) {
    hue = param.progress + (float) i / (float) RING_MAX_PIXELS;
    if (hue > 1.0f) {
      hue -= 1.0f;
    }

    color = HslColor(hue, 1.0f, LIGHTNESS);
    rayColor(i, RgbColor(color));
  }

  if (param.state == AnimationState_Completed) {
    animations.RestartAnimation(0);
  }
}

void ringsRainbow(const AnimationParam& param) {
  HslColor color;
  float hue;

  for (int i = 0; i < NUM_RINGS; i++) {
    hue = param.progress + (float) i / (float) NUM_RINGS;
    if (hue > 1.0f) {
      hue -= 1.0f;
    }

    color = HslColor(hue, 1.0f, LIGHTNESS);
    ringColor(i, RgbColor(color));
  }

  if (param.state == AnimationState_Completed) {
    animations.RestartAnimation(0);
  }
}


void xmasOrbAnimation(const AnimationParam& param) {
  ringColor(0, WHITE);
  ringColor(1, RED);
  ringColor(2, RED);
  ringColor(3, RED);
  ringColor(4, RED);
  ringColor(5, WHITE);
  ringColor(6, RED);
  ringColor(7, RED);
  ringColor(8, RED);
  ringColor(9, RED);
  ringColor(10, WHITE);

  byte offset = round(param.progress);
  for (byte i = offset; i < RING_SIZES[3]; i+=2) {
    leds.SetPixelColor(RINGS[3][i] - 1, WHITE);
  }
  for (byte i = offset; i < RING_SIZES[7]; i+=2) {
    leds.SetPixelColor(RINGS[7][i] - 1, WHITE);
  }

  if (param.state == AnimationState_Completed) {
    animations.RestartAnimation(0);
  }
}

void ringAnimation(const AnimationParam& param) {
  int index = param.progress * (NUM_RINGS * 2 - 2);

  leds.ClearTo(BLACK);
  if (index < NUM_RINGS) {
    ringColor(index, BLUE);
  }
  else {
    ringColor(NUM_RINGS - (index - NUM_RINGS) - 2, BLUE);
  }

  if (param.state == AnimationState_Completed) {
    animations.RestartAnimation(0);
  }
}

void rayAnimation(const AnimationParam& param) {
  int index = param.progress * (RING_MAX_PIXELS / 2);

  if (index > 12) {
    index = 12;
  }
  leds.ClearTo(BLACK);
  rayColor(index, BLUE);
  rayColor(index + (RING_MAX_PIXELS / 2), BLUE);

  if (param.state == AnimationState_Completed) {
    animations.RestartAnimation(0);
  }
}

void rayColor(byte rayIndex, RgbColor color) {
  int pixelIndex;
  byte pixel;

  if (rayIndex >= RING_MAX_PIXELS) {
    return; // prevents out of bounds
  }

  for (byte i = 0; i < NUM_RINGS; i ++) {
    pixelIndex = round((float) RING_SIZES[i] / (float) RING_MAX_PIXELS * rayIndex);
    pixel = RINGS[i][pixelIndex];
    if (pixel == 0) {
      continue; // skip condition
    }
    leds.SetPixelColor(pixel - 1, color); // index starts from 1 (0 is stop condition)
  }
}

void ringColor(byte ringIndex, RgbColor color) {
  byte pixel;

  if (ringIndex >= NUM_RINGS) {
    return; // prevents out of bounds
  }

  for (byte i = 0; i < RING_MAX_PIXELS; i ++) {
    pixel = RINGS[ringIndex][i];
    if (pixel == 0) {
      return; // end condition
    }
    leds.SetPixelColor(pixel - 1, color); // index starts from 1 (0 is stop condition)
  }
}


RgbColor colorWheel(byte wheelPos) {
  // Input a value 0 to 255 to get a color value.
  // The colours are a transition r - g - b - back to r.
  wheelPos = 255 - wheelPos;
  if (wheelPos < 85) {
    return RgbColor(255 - wheelPos * 3, 0, wheelPos * 3);
  }
  if (wheelPos < 170) {
    wheelPos -= 85;
    return RgbColor(0, wheelPos * 3, 255 - wheelPos * 3);
  }
  wheelPos -= 170;
  return RgbColor(wheelPos * 3, 255 - wheelPos * 3, 0);
}
