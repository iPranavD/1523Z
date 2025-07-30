#include "colorsort.hpp"
#include "config.hpp"
#include <iostream>

// Global color sorting variables
IntakeMode currentIntakeMode = IDLE;
ColorMode currentColorMode = DISABLED;

// Toggles
bool basket_toggle = false;
bool scraper_toggle = false;
bool lower_toggle = false;

// Helper functions to change states (updated with mode tracking)
void setIntakeTop() {
    basket.set_value(false);
    bottomrollers.move_velocity(500);
    midrollers.move_velocity(500);
    backroller.move_velocity(-500);
    currentIntakeMode = INTAKE_TOP;
}

void setMidScoring() {
    basket.set_value(true);
    backroller.move_velocity(500);
    bottomrollers.move_velocity(500);
    midrollers.move_velocity(-500);
    currentIntakeMode = MID_SCORING;
}

void setHighScoring() {
    basket.set_value(true);
    backroller.move_velocity(500);
    bottomrollers.move_velocity(500);
    midrollers.move_velocity(500);
    currentIntakeMode = HIGH_SCORING;
}

void setLowScoring() {
    basket.set_value(true);
    bottomrollers.move_velocity(-500);
    midrollers.move_velocity(-500);
    backroller.move_velocity(500);
    currentIntakeMode = LOW_SCORING;
}

void setIdle() {
    basket.set_value(false);
    bottomrollers.move_velocity(0);
    midrollers.move_velocity(0);
    backroller.move_velocity(0);
    currentIntakeMode = IDLE;
}

/**
 * Color sorting control functions - Single toggle through RED -> BLUE -> DISABLED
 */
void toggleColorMode() {
    switch (currentColorMode) {
        case DISABLED:
            currentColorMode = RED_MODE;
            printf("Color sorting: RED MODE\n");
            controller.print(0, 0, "RED MODE");
            break;
        case RED_MODE:
            currentColorMode = BLUE_MODE;
            printf("Color sorting: BLUE MODE\n");
            controller.print(0, 0, "BLUE MODE");
            break;
        case BLUE_MODE:
            currentColorMode = DISABLED;
            printf("Color sorting: DISABLED\n");
            controller.print(0, 0, "SORT OFF");
            break;
    }
}

/**
 * Check if detected color matches target color
 */
bool isCorrectColor(int detectedHue) {
    if (currentColorMode == DISABLED) return true;
    
    int targetHue = (currentColorMode == RED_MODE) ? RED_HUE : BLUE_HUE;
    return (abs(detectedHue - targetHue) <= HUE_TOLERANCE);
}

/**
 * Accept ball - continue with current intake mode
 */
void acceptBall() {
    printf("ACCEPT: Correct color - continuing intake\n");
    controller.print(2, 0, "ACCEPT");
}

/**
 * Reject ball - back rollers ALWAYS reverse to eject
 */
void rejectBall() {
    printf("REJECT: Wrong color - ejecting ball\n");
    controller.print(2, 0, "REJECT");
    
    // Stop intake motion and reverse back roller
    bottomrollers.move_velocity(0);      // Stop bottom intake
    midrollers.move_velocity(0);         // Stop mid intake
    backroller.move_velocity(-600);      // ALWAYS reverse to eject
    
    // Wait for ejection
    pros::delay(REJECT_TIME);
    
    // Return to intake top mode
    setIntakeTop();
    
    printf("Ball ejected, returned to intake mode\n");
}

/**
 * Main color sorting update function
 * Automatically works when in INTAKE_TOP mode and color mode is not DISABLED
 */
void updateColorSorting() {
    // Only sort if in INTAKE_TOP mode and color mode is not DISABLED
    if (currentIntakeMode != INTAKE_TOP || currentColorMode == DISABLED) {
        return;
    }
    
    // Check if a ball is detected
    int proximity = optical.get_proximity();
    
    if (proximity > PROXIMITY_THRESHOLD) {
        // Ball detected! Get its color
        int detectedHue = optical.get_hue();
        
        printf("Ball detected: Hue=%d, Proximity=%d, Mode=INTAKE_TOP\n", 
               detectedHue, proximity);
        
        // Brief pause for stable reading
        pros::delay(50);
        
        // Make sorting decision
        if (isCorrectColor(detectedHue)) {
            acceptBall();
        } else {
            rejectBall();
        }
        
        // Wait to avoid multiple detections
        pros::delay(300);
    }
}

/**
 * Initialize color sorting system
 */
void initializeColorSorting() {
    setIdle();
    currentColorMode = DISABLED;
    printf("Integrated intake + color sorting system ready\n");
}