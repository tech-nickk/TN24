/*
Extended Expression header file for Adafruit SSD1306 OLED display
Contains additional facial expressions animations
*/

#ifndef expressions_h
#define expressions_h

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Arduino.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

extern Adafruit_SSD1306 display;

static unsigned long prevBlinkTime = 0;
static bool happy_state = false;
static int q = -20;

void close() {
    display.clearDisplay();
    
    // Filled rounded rectangles for closed eyes
    display.fillRoundRect(5, 19, 55, 18, 6, WHITE);
    display.fillRoundRect(67, 19, 55, 18, 6, WHITE);
    
    // Clear top portions to create closed eye effect
    display.fillRect(5, 1, 55, 18, BLACK);
    display.fillRect(67, 1, 55, 18, BLACK);
    
    display.display();
}

// normal expression
void normal() {
    display.clearDisplay();
    display.fillRoundRect(8, 12, 50, 35, 9, WHITE);
    display.fillRoundRect(70, 12, 50, 35, 9, WHITE);
    display.display();
    happy_state = false;
}

void blink() {
    unsigned long currentTime = millis();
    
    // Blink every 2 seconds
    if(currentTime - prevBlinkTime > 2000) {
        close();
        delay(50);  // Keep eyes closed briefly
        normal();
        prevBlinkTime = currentTime;
    }
}

void sad() {
    for(int i = 0; i <= 15; i += 3) {
        display.clearDisplay();
        
        // Draw base eyes
        display.fillRoundRect(8, 18, 50, 29, 9, WHITE);
        display.fillRoundRect(70, 18, 50, 29, 9, WHITE);
        
        // Draw sad eyebrows using triangles
        display.fillTriangle(3, 14, 64, 14, 3, 21+i, BLACK);
        display.fillTriangle(68, 14, 124, 21+i, 124, 14, BLACK);
        
        display.display();
    }
}

void upset() {
    if(!happy_state) {
        display.clearDisplay();
        
        display.fillRoundRect(8, 12, 50, 35, 9, WHITE);
        display.fillRoundRect(70, 12, 50, 35, 9, WHITE);
        
        // Clear top portion for upset expression
        display.fillRect(8, q, 50, 35, BLACK);
        display.fillRect(70, q, 50, 35, BLACK);
        
        display.display();
        
        if(q <= -7) { q += 3; }
    }
}

void happy() {
    for(int i = 62; i > 58; i -= 3) {
        display.clearDisplay();
        
        // Base eyes
        display.fillRoundRect(8, 12, 50, 35, 11, WHITE);
        display.fillRoundRect(70, 12, 50, 35, 11, WHITE);
        
        // Happy curves using circles
        display.fillCircle(33, i, 38, BLACK);
        display.fillCircle(95, i, 38, BLACK);
        
        display.display();
    }
    happy_state = true;
}

void cute() {
    for(int i = 0; i <= 2; i += 2) {
        display.clearDisplay();
        
        display.fillRoundRect(8, 12, 50, 35, 12, WHITE);
        display.fillRoundRect(70, 12, 50, 35, 12, WHITE);
        
        display.fillCircle(30, 66-i, 40, BLACK);
        display.fillCircle(98, 66-i, 40, BLACK);
        
        display.display();
    }
    happy_state = true;
}

void angry() {
    for(int i = 0; i <= 15; i += 3) {
        display.clearDisplay();
        
        display.fillRoundRect(8, 18, 50, 29, 9, WHITE);
        display.fillRoundRect(70, 18, 50, 29, 9, WHITE);
        
        display.fillTriangle(3, 14, 64, 18+i, 124, 14, BLACK);
        
        display.display();
    }
}

void sleepy() {
    display.clearDisplay();
    
    // Droopy eyes
    for(int i = 0; i <= 10; i += 2) {
        display.clearDisplay();
        display.fillRoundRect(8, 12+i, 50, 25, 9, WHITE);
        display.fillRoundRect(70, 12+i, 50, 25, 9, WHITE);
        
        // Small 'z' letters floating up
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(100-i, 40-i);
        display.print("z");
        display.setCursor(110-i, 30-i);
        display.print("z");
        display.setCursor(120-i, 20-i);
        display.print("z");
        
        display.display();
        delay(100);
    }
}

void wink() {
    display.clearDisplay();
    
    // Right eye normal
    display.fillRoundRect(70, 12, 50, 35, 9, WHITE);
    
    // Left eye closed
    display.fillRoundRect(5, 19, 55, 18, 6, WHITE);
    display.fillRect(5, 1, 55, 18, BLACK);
    
    display.display();
    delay(300);
    normal();
}

void surprised() {
    display.clearDisplay();
    
    // Wide open circular eyes
    display.fillCircle(33, 30, 20, WHITE);
    display.fillCircle(95, 30, 20, WHITE);
    
    // Raised eyebrows
    display.drawLine(13, 5, 53, 5, WHITE);
    display.drawLine(75, 5, 115, 5, WHITE);
    
    display.display();
}

void confused() {
    display.clearDisplay();
    
    // Asymmetric eyes
    display.fillRoundRect(8, 12, 50, 35, 9, WHITE);
    display.fillCircle(95, 30, 20, WHITE);
    
    // Raised eyebrow on one side
    display.drawLine(75, 5, 115, 15, WHITE);
    
    display.display();
}

void love() {
    display.clearDisplay();
    
    // Heart-shaped eyes
    for(int i = 0; i < 2; i++) {
        int x = (i == 0) ? 33 : 95;
        
        // Draw heart shape
        display.fillCircle(x-7, 25, 10, WHITE);
        display.fillCircle(x+7, 25, 10, WHITE);
        display.fillTriangle(x-15, 30, x+15, 30, x, 45, WHITE);
    }
    
    display.display();
}

void dizzy() {
    display.clearDisplay();
    
    // Spiral eyes
    for(int i = 0; i < 2; i++) {
        int centerX = (i == 0) ? 33 : 95;
        int centerY = 30;
        
        // Draw spiral pattern
        for(int r = 0; r < 15; r += 3) {
            display.drawCircle(centerX, centerY, r, WHITE);
            // Add crossing lines
            display.drawLine(centerX-r, centerY-r, centerX+r, centerY+r, WHITE);
            display.drawLine(centerX-r, centerY+r, centerX+r, centerY-r, WHITE);
        }
    }
    
    display.display();
}

void thinking() {
    display.clearDisplay();
    
    // Slightly narrowed eyes
    display.fillRoundRect(8, 15, 50, 25, 9, WHITE);
    display.fillRoundRect(70, 15, 50, 25, 9, WHITE);
    
    // Thought bubble
    display.fillCircle(110, 15, 3, WHITE);
    display.fillCircle(115, 10, 4, WHITE);
    display.fillCircle(122, 5, 5, WHITE);
    
    display.display();
}

void mischievous() {
    display.clearDisplay();
    
    // Slanted eyes
    display.fillRoundRect(8, 12, 50, 25, 9, WHITE);
    display.fillRoundRect(70, 18, 50, 25, 9, WHITE);
    
    // Evil eyebrows
    display.drawLine(8, 5, 58, 15, WHITE);
    display.drawLine(70, 15, 120, 5, WHITE);
    
    display.display();
}

void crying() {
    for(int i = 0; i < 15; i += 3) {
        display.clearDisplay();
        
        // Sad eyes
        display.fillRoundRect(8, 18, 50, 29, 9, WHITE);
        display.fillRoundRect(70, 18, 50, 29, 9, WHITE);
        
        // Tears
        display.fillRoundRect(20, 45+i, 3, 5, 1, WHITE);
        display.fillRoundRect(82, 45+i, 3, 5, 1, WHITE);
        
        display.display();
        delay(100);
    }
}

void nervous() {
    for(int i = 0; i < 4; i++) {
        display.clearDisplay();
        
        // Shaking eyes
        display.fillRoundRect(8 + (i%2)*2, 12, 50, 35, 9, WHITE);
        display.fillRoundRect(70 + (i%2)*2, 12, 50, 35, 9, WHITE);
        
        // Sweat drop
        display.fillRoundRect(115, 20+i*2, 3, 5, 1, WHITE);
        
        display.display();
        delay(100);
    }
}

#endif
