#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define BUTTON_PIN D3

// Dino settings
int dinoX = 10;
int dinoY = 45;      // ground position
int jumpY = dinoY;
bool isJumping = false;
int jumpVelocity = 0;

// Obstacle
int obsX = SCREEN_WIDTH;
int obsY = 45;
int obsSpeed = 5;   // 🔥 faster from the start

// Score
int score = 0;
int gameSpeed = 18; // 🔥 smaller delay = faster updates

void drawDino(int x, int y) {
  // Simple dino pixel art
  display.fillRect(x, y, 10, 10, SSD1306_WHITE);      // body
  display.fillRect(x + 8, y - 4, 4, 4, SSD1306_WHITE); // head
  display.fillRect(x + 2, y + 10, 2, 3, SSD1306_WHITE); // leg 1
  display.fillRect(x + 6, y + 10, 2, 3, SSD1306_WHITE); // leg 2
  display.drawPixel(x + 9, y - 5, SSD1306_WHITE);       // eye
}

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);

//dispaly the startup screen
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for (;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 20);
  display.println("DINO GAME");
  display.display();
  delay(1500);
}

void loop() {
  // Check button
  if (digitalRead(BUTTON_PIN) == LOW && !isJumping) {
    isJumping = true;
    jumpVelocity = -10; // jump force
  }

  // Update jump physics
  if (isJumping) {
    jumpY += jumpVelocity;
    jumpVelocity += 1; // gravity

    if (jumpY >= dinoY) {
      jumpY = dinoY;
      isJumping = false;
    }
  }

  // Move obstacle
  obsX -= obsSpeed;
  if (obsX < -10) {
    obsX = SCREEN_WIDTH;
    score++;

    // Speed up game every 5 points
    if (score % 5 == 0 && gameSpeed > 10) {
      obsSpeed++;
      gameSpeed -= 1; // less reduction so it's smooth
    }
  }

  // Collision detection
  if (obsX < dinoX + 12 && obsX > dinoX - 8) {
    if (jumpY + 10 >= obsY) {
      gameOver();
    }
  }

  // Draw
  display.clearDisplay();
  drawDino(dinoX, jumpY);
  display.fillRect(obsX, obsY + 5, 10, 10, SSD1306_WHITE); // obstacle

  // Score
  display.setCursor(90, 0);
  display.print("Score:");
  display.print(score);

  display.display();
  delay(gameSpeed);
}

void gameOver() {
  display.clearDisplay();
  display.setCursor(20, 20);
  display.println("GAME OVER!");
  display.setCursor(20, 40);
  display.print("Score: ");
  display.print(score);
  display.display();
  delay(2000);

  // Reset game (still fast from start)
  score = 0;
  obsX = SCREEN_WIDTH;
  obsSpeed = 5;
  gameSpeed = 18;
}