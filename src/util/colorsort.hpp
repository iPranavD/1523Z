#pragma once
#include "main.h"

// Color sorting constants
constexpr float RED_HUE = 5;
constexpr float BLUE_HUE = 210;
constexpr float HUE_TOLERANCE = 15;
const int PROXIMITY_THRESHOLD = 200; // Object detection threshold

// Color sorting enums
enum IntakeMode {
    INTAKE_TOP,
    MID_SCORING,
    HIGH_SCORING,
    LOW_SCORING,
    IDLE
};

enum ColorMode {
    RED_MODE,
    BLUE_MODE,
    DISABLED
};

// External declarations for global variables
extern IntakeMode currentIntakeMode;
extern ColorMode currentColorMode;
extern bool basket_toggle;
extern bool scraper_toggle;
extern bool lower_toggle;

// Timing settings
const int REJECT_TIME = 600; // Time to reject wrong color (ms)

// Intake mode functions
void setIntakeTop();
void setMidScoring();
void setHighScoring();
void setLowScoring();
void setIdle();

// Color sorting functions
void toggleColorMode();
bool isCorrectColor(int detectedHue);
void acceptBall();
void rejectBall();
void updateColorSorting();
void initializeColorSorting();