const int segmentPins[8] = {7, 6, 5, 11, 10, 8, 9, 4};
const int btnUp = 3;

byte digitPattern[16][8] = {
    {1,1,1,1,1,1,0,0}, // 0
    {0,1,1,0,0,0,0,0}, // 1
    {1,1,0,1,1,0,1,0}, // 2
    {1,1,1,1,0,0,1,0}, // 3
    {0,1,1,0,0,1,1,0}, // 4
    {1,0,1,1,0,1,1,0}, // 5
    {1,0,1,1,1,1,1,0}, // 6
    {1,1,1,0,0,0,0,0}, // 7
    {1,1,1,1,1,1,1,0}, // 8
    {1,1,1,1,0,1,1,0}, // 9
    {1,1,1,0,1,1,1,0}, // A
    {0,0,1,1,1,1,1,0}, // b
    {1,0,0,1,1,1,0,0}, // C
    {0,1,1,1,1,0,1,0}, // d
    {1,0,0,1,1,1,1,0}, // E
    {1,0,0,0,1,1,1,0}  // F
};
int currentDigit = 0;
bool lastUpState = HIGH;

void displayDigit(int num) 
{
    for (int i = 0; i < 8; i++)
    {
        digitalWrite(segmentPins[i], digitPattern[num][i]);
    }
}

void setup()
{
    for (int i = 0; i < 8; i++)
    {
        pinMode(segmentPins[i], OUTPUT);
    }

    pinMode(btnUp, INPUT_PULLUP);
    displayDigit(currentDigit);
}

void loop(){
    bool upState = digitalRead(btnUp);
    if (lastUpState == HIGH && upState == LOW)
    {
        currentDigit++;
        if (currentDigit > 15){
            currentDigit = 0;
        }
        displayDigit(currentDigit);
    }
    
    lastUpState = upState;
}