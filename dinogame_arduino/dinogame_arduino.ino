#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>

// OLED settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_MOSI   D7  // GPIO13
#define OLED_CLK    D5  // GPIO14
#define OLED_DC     D3  // GPIO0
#define OLED_CS     D8  // GPIO15
#define OLED_RESET  D4  // GPIO2

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
  &SPI, OLED_DC, OLED_RESET, OLED_CS);

// Dino settings
int dinoY = 48;        // Ground position
int jumpHeight = 20;   // Jump height
bool isJumping = false;
int jumpProgress = 0;

// Obstacle settings
int obstacleX = SCREEN_WIDTH;

// Button pin
#define BUTTON_PIN D1  // GPIO5

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);  // Button with internal pull-up
  display.begin(SSD1306_SWITCHCAPVCC);
  display.clearDisplay();
  display.display();
}

void loop() {
  // Read button (LOW when pressed)
  if (digitalRead(BUTTON_PIN) == LOW && !isJumping) {
    isJumping = true;
    jumpProgress = 0;
  }

  // Handle jump animation
  if (isJumping) {
    if (jumpProgress < 10) dinoY -= 2;       // going up
    else if (jumpProgress < 20) dinoY += 2;  // going down
    jumpProgress++;
    if (jumpProgress >= 20) {
      isJumping = false;
      dinoY = 48; // back to ground
    }
  }

  // Move obstacle
  obstacleX -= 3;
  if (obstacleX < -10) obstacleX = SCREEN_WIDTH;

  // Collision detection
  if (obstacleX < 20 && obstacleX > 5 && dinoY > 40) {
    gameOver();
  }

  // Draw everything
  display.clearDisplay();

  // Ground line
  display.drawLine(0, 63, SCREEN_WIDTH, 63, SSD1306_WHITE);

  // Dino (10x10 square)
  display.fillRect(5, dinoY, 10, 10, SSD1306_WHITE);

  // Obstacle (cactus block)
  display.fillRect(obstacleX, 53, 8, 10, SSD1306_WHITE);

  display.display();

  delay(50);
}

void gameOver() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(20, 20);
  display.println("GAME OVER");
  display.display();
  delay(2000);
  obstacleX = SCREEN_WIDTH; // reset obstacle
}
