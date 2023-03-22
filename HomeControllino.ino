#include <ArxContainer.h>
#include <AceButton.h>
#include <Controllino.h>
#include <EEPROM.h>
#include <EthernetUdp.h>

using namespace ace_button;

#define NUM_INPUTS 21

#define DEBUG 1

class Event {
  public:
  short inputButton;
  uint8_t buttonState;
  uint8_t eventType;
  int milliseconds;
};
arx::vector<Event*> events;


class Configuration {
  public:
  arx::map<short,arx::vector<short int> > input2outputs;
  arx::map<short,short> coupled;
};
Configuration config;
#define COUPLE(x,y) do {   config.coupled.insert(x,y); config.coupled.insert(y,x); } while (0)

AceButton buttons[NUM_INPUTS];

void handleEvent(AceButton*, uint8_t, uint8_t);

#define CONTROLLINO_A16 CONTROLLINO_I16
#define CONTROLLINO_A17 CONTROLLINO_I17
#define CONTROLLINO_A18 CONTROLLINO_I18
#define CONTROLLINO_A19 CONTROLLINO_IN0
#define CONTROLLINO_A20 CONTROLLINO_IN1

byte mac[6];
char macstr[18];
unsigned int localPort = 8888;      // local port to listen on
EthernetUDP Udp;

void setup() {
  Serial.begin(115200);
  while (! Serial);  

  // Random MAC address stored in EEPROM
  if (EEPROM.read(1) == '#') {
    for (int i = 2; i < 6; i++) {
      mac[i] = EEPROM.read(i);
    }
  } else {
    randomSeed(analogRead(0));
    for (int i = 2; i < 6; i++) {
      mac[i] = random(0, 255);
      EEPROM.write(i, mac[i]);
    }
    EEPROM.write(1, '#');
  }
  snprintf(macstr, 18, "%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

  // Start up networking
  Ethernet.begin(mac);
  if (DEBUG) {
    Serial.print("DHCP (");
    Serial.print(macstr);
    Serial.print(")...");
    Serial.print("success: ");
    Serial.println(Ethernet.localIP());
  }

    // start UDP
  Udp.begin(localPort);
  
  pinMode(CONTROLLINO_D0, OUTPUT);
  pinMode(CONTROLLINO_D1, OUTPUT);
  pinMode(CONTROLLINO_D2, OUTPUT);
  pinMode(CONTROLLINO_D3, OUTPUT);
  pinMode(CONTROLLINO_D4, OUTPUT);
  pinMode(CONTROLLINO_D5, OUTPUT);
  pinMode(CONTROLLINO_D6, OUTPUT);
  pinMode(CONTROLLINO_D7, OUTPUT);
  pinMode(CONTROLLINO_D8, OUTPUT);
  pinMode(CONTROLLINO_D9, OUTPUT);
  pinMode(CONTROLLINO_D10, OUTPUT);
  pinMode(CONTROLLINO_D11, OUTPUT);
  pinMode(CONTROLLINO_D12, OUTPUT);
  pinMode(CONTROLLINO_D13, OUTPUT);
  pinMode(CONTROLLINO_D14, OUTPUT);
  pinMode(CONTROLLINO_D15, OUTPUT);
  pinMode(CONTROLLINO_D16, OUTPUT);
  pinMode(CONTROLLINO_D17, OUTPUT);
  pinMode(CONTROLLINO_D18, OUTPUT);
  pinMode(CONTROLLINO_D19, OUTPUT);
  pinMode(CONTROLLINO_D20, OUTPUT);
  pinMode(CONTROLLINO_D21, OUTPUT);
  pinMode(CONTROLLINO_D22, OUTPUT);
  pinMode(CONTROLLINO_D23, OUTPUT);

  pinMode(CONTROLLINO_A0, INPUT);
  buttons[0].init(CONTROLLINO_A0, LOW, 0);
  pinMode(CONTROLLINO_A1, INPUT);
  buttons[1].init(CONTROLLINO_A1, LOW, 1);
  pinMode(CONTROLLINO_A2, INPUT);
  buttons[2].init(CONTROLLINO_A2, LOW, 2);
  pinMode(CONTROLLINO_A3, INPUT);
  buttons[3].init(CONTROLLINO_A3, LOW, 3);
  pinMode(CONTROLLINO_A4, INPUT);
  buttons[4].init(CONTROLLINO_A4, LOW, 4);
  pinMode(CONTROLLINO_A5, INPUT);
  buttons[5].init(CONTROLLINO_A5, LOW, 5);
  pinMode(CONTROLLINO_A6, INPUT);
  buttons[6].init(CONTROLLINO_A6, LOW, 6);
  pinMode(CONTROLLINO_A7, INPUT);
  buttons[7].init(CONTROLLINO_A7, LOW, 7);
  pinMode(CONTROLLINO_A8, INPUT);
  buttons[8].init(CONTROLLINO_A8, LOW, 8);
  pinMode(CONTROLLINO_A9, INPUT);
  buttons[9].init(CONTROLLINO_A9, LOW, 9);
  pinMode(CONTROLLINO_A10, INPUT);
  buttons[10].init(CONTROLLINO_A10, LOW, 10);
  pinMode(CONTROLLINO_A11, INPUT);
  buttons[11].init(CONTROLLINO_A11, LOW, 11);
  pinMode(CONTROLLINO_A12, INPUT);
  buttons[12].init(CONTROLLINO_A12, LOW, 12);
  pinMode(CONTROLLINO_A13, INPUT);
  buttons[13].init(CONTROLLINO_A13, LOW, 13);
  pinMode(CONTROLLINO_A14, INPUT);
  buttons[14].init(CONTROLLINO_A14, LOW, 14);
  pinMode(CONTROLLINO_A15, INPUT);
  buttons[15].init(CONTROLLINO_A15, LOW, 15);
  pinMode(CONTROLLINO_A16, INPUT);
  buttons[16].init(CONTROLLINO_A16, LOW, 16);
  pinMode(CONTROLLINO_A17, INPUT);
  buttons[17].init(CONTROLLINO_A17, LOW, 17);
  pinMode(CONTROLLINO_A18, INPUT);
  buttons[18].init(CONTROLLINO_A18, LOW, 18);
  pinMode(CONTROLLINO_A19, INPUT);
  buttons[19].init(CONTROLLINO_A19, LOW, 19);
  pinMode(CONTROLLINO_A20, INPUT);
  buttons[20].init(CONTROLLINO_A20, LOW, 20);

  for(int idx=0; idx <=20; ++idx) {
    buttons[idx].setEventHandler(handleEvent);
  }

  arx::vector<short int> tmp;

  config.input2outputs[CONTROLLINO_A0] = tmp;
  tmp.clear();
  config.input2outputs[CONTROLLINO_A1] = tmp;
  tmp.clear();
  config.input2outputs[CONTROLLINO_A2] = tmp;
  tmp.clear();

  tmp.push_back(CONTROLLINO_D3);
  tmp.push_back(CONTROLLINO_D19); // fehler verkabelung, sollte nur 1 schalter sein
  config.input2outputs[CONTROLLINO_A3] = tmp;
  tmp.clear();
  
  tmp.push_back(CONTROLLINO_D17);
  tmp.push_back(CONTROLLINO_D8); // treppe und aufgang
  config.input2outputs[CONTROLLINO_A4] = tmp;
  tmp.clear();
  config.input2outputs[CONTROLLINO_A5] = tmp;
  tmp.clear();

  tmp.push_back(CONTROLLINO_D2);
  config.input2outputs[CONTROLLINO_A6] = tmp;
  tmp.clear();
  config.input2outputs[CONTROLLINO_A7] = tmp;
  tmp.clear();
  config.input2outputs[CONTROLLINO_A8] = tmp;
  tmp.clear();

  config.input2outputs[CONTROLLINO_A9] = tmp;
  tmp.clear();

  tmp.push_back(CONTROLLINO_D0);
  config.input2outputs[CONTROLLINO_A10] = tmp;
  tmp.clear();
  config.input2outputs[CONTROLLINO_A11] = tmp;
  tmp.clear();
  config.input2outputs[CONTROLLINO_A12] = tmp;
  tmp.clear();

  tmp.push_back(CONTROLLINO_D5);
  config.input2outputs[CONTROLLINO_A13] = tmp;
  tmp.clear();

  tmp.push_back(CONTROLLINO_D16);
  config.input2outputs[CONTROLLINO_A14] = tmp;

  tmp.clear();
  config.input2outputs[CONTROLLINO_A15] = tmp;
  tmp.clear();
  config.input2outputs[CONTROLLINO_A16] = tmp;
  tmp.clear();
  config.input2outputs[CONTROLLINO_A17] = tmp;
  tmp.clear();
  config.input2outputs[CONTROLLINO_A18] = tmp;
  tmp.clear();
  config.input2outputs[CONTROLLINO_A19] = tmp;
  tmp.clear();
  
  tmp.push_back(CONTROLLINO_D18);
  config.input2outputs[CONTROLLINO_A20] = tmp;
  tmp.clear();

  tmp.push_back(CONTROLLINO_D9);
  config.input2outputs[CONTROLLINO_A19 + 1000] = tmp;
  tmp.clear();
  COUPLE(CONTROLLINO_A19,CONTROLLINO_A20);  

  
}

void loop() {
  for(int idx=0; idx <=20; ++idx) {
    buttons[idx].check();
  }

  switch (Ethernet.maintain()) {
  case 1:
    //renewed fail
    Serial.println("Error: renewed fail");
    break;

  case 2:
    //renewed success
    Serial.println("Renewed success");
    //print your local IP address:
    Serial.print("My IP address: ");
    Serial.println(Ethernet.localIP());
    break;

  case 3:
    //rebind fail
    Serial.println("Error: rebind fail");
    break;

  case 4:
    //rebind success
    Serial.println("Rebind success");
    //print your local IP address:
    Serial.print("My IP address: ");
    Serial.println(Ethernet.localIP());
    break;

  default:
    //nothing happened
    break;
  }

  for(int idx=0; idx <=20; ++idx) {
    buttons[idx].check();
  }

  if (!events.size()) {
    return;
  }
  if (DEBUG) {
    Serial.print("We have an event. Q Size: ");
    Serial.println(events.size());
  }
  Event* event = events.front();
  int current = millis();

  if (event->milliseconds - current > 0) {
    if (config.coupled.has(event->inputButton)) {
      for (arx::vector<Event*>::iterator it = events.begin(); it != events.end(); ++it) {
        if (*it==event) {
          if (DEBUG) Serial.println("We skip the first element");
          continue;
        }
        int partner = config.coupled[event->inputButton];
        if ((*it)->inputButton == partner && 
            (*it)->buttonState == event->buttonState && 
            (*it)->eventType == event->eventType) {
          if (DEBUG) {
            Serial.print("Rewrite button to: ");
            Serial.println(min((*it)->inputButton,event->inputButton) + 1000);
          }
          Event* p = *it;
          events.erase(it);
          event->inputButton = min(p->inputButton,event->inputButton) + 1000; // rewrite to virtual PIN
          delete[] p;
          break;
        }
      }
    } else {
      if (DEBUG) Serial.println("No coupled PIN");
    }
  }
  
  if (event->milliseconds - current <= 0) {
    // execute event
    arx::vector<short int> tmp = config.input2outputs[event->inputButton];
    if (!tmp.size() && DEBUG) {
      Serial.print("No outputs found for button: ");
      Serial.println(event->inputButton);
      events.pop_front();
      delete[] event;
      return;
    }
    if (DEBUG) {
      Serial.print("Input ");
      Serial.print(event->inputButton);
      Serial.print(" - Output : ");
      for (const auto& v : tmp) {
        Serial.print(v); 
        Serial.print(";");
      }
      Serial.print(" - buttonState: ");
      Serial.println(event->buttonState);
    }
    for (const auto& v : tmp) {
      digitalWrite(v, event->buttonState);
    }
    events.pop_front();
    delete[] event;
  
  }
}

void handleEvent(AceButton* button, uint8_t eventType, uint8_t buttonState) {

  if (DEBUG) {
      Serial.print(F("  button  "));
      Serial.print(button->getPin());
      Serial.print(F("  buttonstate  "));
      Serial.print(buttonState);
      Serial.print(F("  "));
      Serial.print(millis());
      Serial.println((int)eventType);
  }

  Event* event = new Event();
  event->inputButton = button->getPin();
  event->eventType = eventType;
  event->buttonState = buttonState; // on OR off, RESP. 1 OR 0
  event->milliseconds = millis()+30; // End of life, action required
  events.push_back(event);

}
