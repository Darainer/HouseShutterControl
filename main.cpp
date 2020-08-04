
//#include "../repos/tinyfsm/include/tinyfsm.hpp"

#ifndef ARDUINO_H
  #define ARDUINO_H
  #include "arduino.h"
  #define test_arduino true
#else
  #define test_arduino false
#endif

//if(test_arduino){
using namespace arduino;
//}

int main()
{
  return 0;
}

enum ButtonEvent
{
  NO_TRANSITION_EVENT,
  NOTPRESSED,
  PRESSED,
  SHORT_PRESS,
  LONG_PRESS
};

class Button
{
private:
  uint32_t m_uCPin;
  ButtonEvent Event;
  bool m_isPressed{true};
  bool m_LastReadState{false};
  uint32_t m_debouncetime_ms{25};
  uint32_t m_last_statechange_ms{0};
  uint32_t short_press_ms{500};
  uint32_t long_press_ms{1000};
  bool m_switch_invert{false};

public:
  Button(uint32_t pin)
  {
    m_uCPin = pin;
    m_last_statechange_ms = millis();
  }
  void read_and_debounce_button()
  {
    bool raw_switch_reading = digitalRead(m_uCPin, m_switch_invert);
    uint32_t current_time_in_ms = millis();

    // If the switch changed, due to noise or pressing
    if (raw_switch_reading != m_LastReadState)
    {
      m_last_statechange_ms = millis(); //reset the debouncing timer
    }

    int time_in_current_state = (current_time_in_ms - m_last_statechange_ms);

    if (time_in_current_state > m_debouncetime_ms)
    {
      // whatever the reading is at, it's been there for longer than the debounce
      // delay, so take it as the actual current state
      if (raw_switch_reading != m_isPressed)
      {
        m_isPressed = raw_switch_reading;
      }
    }
  }
  ButtonEvent check_button_event()   // event should be state transition. need a state for button press
  {
    // if the switch has been pressed long enough to register a long press
    if (m_isPressed == 0)
    {
      if (Event != NO_TRANSITION_EVENT)
      {
        Event == NO_TRANSITION_EVENT;
      }
    }
    uint32_t current_time_in_ms = millis();
    int time_in_current_state = (current_time_in_ms - m_last_statechange_ms);

    if (Event != LONG_PRESS && m_isPressed && time_in_current_state > long_press_ms)
    {
      Event = LONG_PRESS;
    }
    else if (Event == NOTPRESSED || Event)

    return Event;
  }

  ButtonEvent poll_and_return_event()
  {
    read_and_debounce_button();
    ButtonEvent current_event = check_button_event();
    return current_event;
  }
};

/*

*/

// Rollladensteuerung by jurs for German Arduino-Forum
#define BUTTONPRESSED LOW
#define RELAYACTIVE LOW

enum Action
{
  NOACTION,
  UPACTION,
  DOWNACTION,
  AtTOP,
  AtBOTTOM
};

struct roll_t
{
  byte downPin;
  byte upPin;
  boolean downPressed;
  boolean upPressed;
  Action action;
  unsigned long actionStarttime;
  unsigned long actionTimeout;
  byte relayOnePin;
  byte relayTwoPin;
};

struct master_switch
{
  byte downPin;
  byte upPin;
  boolean downPressed;
  boolean upPressed;
  unsigned long actionStarttime;
  unsigned long actionTimeout;
};

roll_t rollladen[] = {
    {22, 23, false, false, NOACTION, 0, 10000L, 32, 33}, // Button-Pins 22, 24, Relay-Pins 31, 33, Timeout 10 seconds
    {24, 25, false, false, NOACTION, 0, 20000L, 34, 35}, // Button-Pins 26, 28, Relay-Pins 35, 37, Timeout 20 seconds
    {26, 27, false, false, NOACTION, 0, 20000L, 36, 37}, // Button-Pins 30, 32, Relay-Pins 39, 41, Timeout 10 seconds
    {28, 29, false, false, NOACTION, 0, 20000L, 38, 39}, // Button-Pins 34, 36, Relay-Pins 43, 45, Timeout 10 seconds
    {30, 31, false, false, NOACTION, 0, 20000L, 40, 41}, // Button-Pins 38, 40, Relay-Pins 47, 49, Timeout 10 seconds
                                                         // General taster
};

roll_t masterswitch = {20, 21, false, false, NOACTION, 0, 10000L}; // master_switch

#define ANZAHLROLLLADEN (int)5 //sizeof(rollladen)/sizeof(rollladen[0])  //ryan: why not just fixed size? sizeof(rollladen)?

void setup()
{
  Serial.begin(9600);
  for (int i = 0; i < ANZAHLROLLLADEN; i++)
  {
    if (BUTTONPRESSED == HIGH)
    {
      pinMode(rollladen[i].downPin, INPUT);
      pinMode(rollladen[i].upPin, INPUT);
    }
    else
    {
      pinMode(rollladen[i].downPin, INPUT_PULLUP);
      pinMode(rollladen[i].upPin, INPUT_PULLUP);
    }
    if (RELAYACTIVE == LOW)
    {
      digitalWrite(rollladen[i].relayOnePin, HIGH);
      digitalWrite(rollladen[i].relayTwoPin, HIGH);
    }
    pinMode(rollladen[i].relayOnePin, OUTPUT);
    pinMode(rollladen[i].relayTwoPin, OUTPUT);
  }
}

void switchRelay(byte i, Action action)
{
  Serial.print("Schalte Rolladen-");
  Serial.print(i);
  switch (action)
  {
  case NOACTION: // STOP
    digitalWrite(rollladen[i].relayOnePin, !RELAYACTIVE);
    digitalWrite(rollladen[i].relayTwoPin, !RELAYACTIVE);
    Serial.println(" STOP");
    break;
  case UPACTION: // UP
    digitalWrite(rollladen[i].relayOnePin, RELAYACTIVE);
    digitalWrite(rollladen[i].relayTwoPin, !RELAYACTIVE);
    Serial.println(" UP");
    break;
  case DOWNACTION: // DOWN
    digitalWrite(rollladen[i].relayOnePin, !RELAYACTIVE);
    digitalWrite(rollladen[i].relayTwoPin, RELAYACTIVE);
    Serial.println(" DOWN");
    break;
  }
}

void activate_all_mofos(Action action)
{
  for (int i = 0; i < ANZAHLROLLLADEN; i++)
  {
    switchRelay(i, action);
  }
}

void loop(void)
{
  byte state;

  //check master switch first
  //
  if (masterswitch.downPin == HIGH)
  {
    activate_all_mofos(DOWNACTION);
  }
  if (masterswitch.upPin == HIGH)
  {
    activate_all_mofos(UPACTION);
  }

  for (int i = 0; i < ANZAHLROLLLADEN; i++)
  {
    // Prüfen, ob Down gedrückt wurde
    state = digitalRead(rollladen[i].downPin);
    if (BUTTONPRESSED == LOW)
      state = !state;
    if (state && !rollladen[i].downPressed)
    {
      switch (rollladen[i].action)
      {
      case NOACTION:
        rollladen[i].action = DOWNACTION;
        rollladen[i].actionStarttime = millis();
        switchRelay(i, rollladen[i].action);
        break;
      default:
        if (rollladen[i].action != NOACTION)
        {
          rollladen[i].action = NOACTION;
          switchRelay(i, rollladen[i].action);
        }
      }
    }
    rollladen[i].downPressed = state;

    // Prüfen, ob Up gerdrückt wurde
    state = digitalRead(rollladen[i].upPin);
    if (BUTTONPRESSED == LOW)
      state = !state;
    if (state && !rollladen[i].upPressed)
    {
      switch (rollladen[i].action)
      {
      case NOACTION:
        rollladen[i].action = UPACTION;
        rollladen[i].actionStarttime = millis();
        switchRelay(i, rollladen[i].action);
        break;
      default:
        if (rollladen[i].action != NOACTION)
        {
          rollladen[i].action = NOACTION;
          switchRelay(i, rollladen[i].action);
        }
      }
    }
    rollladen[i].upPressed = state;

    // Prüfen auf Timeout
    if (rollladen[i].action != NOACTION && millis() - rollladen[i].actionStarttime >= rollladen[i].actionTimeout)
    {
      rollladen[i].action = NOACTION;
      switchRelay(i, rollladen[i].action);
    }
  }
  delay(50); // kleines Delay zum Entprellen der Buttons
}