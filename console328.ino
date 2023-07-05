#include <ArxContainer.h>
#include <TVout.h>
#include <fontALL.h>

using namespace arx;
TVout TV;

bool flag1 = false;
bool flag2 = false;
bool flag3 = false;
bool flag4 = false;
bool flag5 = false;

int pizePin;
int resetPin;

void setup() {
  TV.begin(PAL, 120, 96);
  TV.select_font(font6x8);

  Serial.begin(9600);

  pinMode(2, INPUT_PULLUP); //up
  pinMode(3, INPUT_PULLUP); //left
  pinMode(4, INPUT_PULLUP); //right
  pinMode(5, INPUT_PULLUP); //down
  pinMode(10, INPUT_PULLUP); //ok
  pinMode(6, INPUT_PULLUP); //up(jostik1,jostik2)
  pinMode(8, INPUT_PULLUP); //down(jostik1,jostik2)
  pinMode(A1, INPUT_PULLUP); //left(jostik1,jostik2)
  pinMode(A2, INPUT_PULLUP); //right(jostik1,jostik2)

  pizePin = 11; //pisk 
  resetPin = 12; //reset

  digitalWrite(resetPin, HIGH);
  pinMode(resetPin, OUTPUT); 

  randomSeed(analogRead(0));

}
void(* resetFunc) (void) = 0; // reset proc

// zmeyka var
uint8_t zmey_x = 120 / 2;
uint8_t zmey_y = 96 / 2;
bool isZmeyAlive = true;
int zmey_score = 0;
vector<int> zmey_coordinates_x = {zmey_x};
vector<int> zmey_coordinates_y = {zmey_y};
long zmey_speed = 10000; // pix in milisec
bool isLeftZ = false;
bool isRightZ = true;
bool isUpZ = false;
bool isDownZ = false;
bool isAppleZ = false;
uint8_t applez_x;
uint8_t applez_y;

// menu var
bool isRes = true;
uint8_t menu_x = 25;
uint8_t menu_y = 6;
bool isZmeym = false;
bool isTankim = false;
bool isPacmanm = false;
bool isTetrism = false;

void loop() {
  bool upBut = !digitalRead(2);
  bool leftBut = !digitalRead(3);
  bool rigthBut = !digitalRead(4);
  bool downBut = !digitalRead(5);
  bool okBut = !digitalRead(10);

  if(isRes == true){
    TV.set_cursor(menu_x, menu_y);
    TV.print(">");
    TV.set_cursor(35, 6);
    TV.print("zmeyka");
    TV.set_cursor(35, 14);
    TV.print("tanki");
    TV.set_cursor(35, 22);
    TV.print("pacman");
    TV.set_cursor(35, 30);
    TV.print("tetris");
    if(upBut && !flag1){
      flag1 = true;
      tone(pizePin, 1000, 985);
      if(menu_y > 6){
        TV.set_cursor(menu_x, menu_y);
        TV.print(" ");
        menu_y -= 8;
        TV.set_cursor(menu_x, menu_y);
        TV.print(">");
      }
    }
    if(!upBut && flag1){
      flag1 = false;
    }
    if(downBut && !flag4){
      flag4 = true;
      tone(pizePin, 1000, 985);
      if(menu_y < 30){
        TV.set_cursor(menu_x, menu_y);
        TV.print(" ");
        menu_y += 8;
        TV.set_cursor(menu_x, menu_y);
        TV.print(">");
      }

    }
    if(!downBut && flag4){
      flag4 = false;
    }
    if(okBut && !flag5){
      flag5 = true;
      tone(pizePin, 1000, 985);
      isRes = false;
      if(menu_y == 6){
        isZmeym = true;
        TV.clear_screen();
      }else if(menu_y == 14){
        isTankim = true;
        TV.clear_screen();
      }else if(menu_y == 22){
        isPacmanm = true;
        TV.clear_screen();
      }else if(menu_y == 30){
        isTetrism = true;
        TV.clear_screen();
      }
    }
    if(!okBut && flag5){
      flag5 = false;
    }
  }

  if(isZmeym == true){
    if(isZmeyAlive == true){
      TV.set_cursor(0, 0);
      TV.print(zmey_score);
      TV.set_cursor(35, 0);
      TV.print("ZMEYKA");
      // game zone 0 - 120 - x, 10 - 96 - y;
      for(int i = 0; i < 120; i++){
        TV.set_pixel(i, 10, WHITE);
        TV.set_pixel(i, 95, WHITE);
      }
      for(int i = 10; i < 96; i++){
        TV.set_pixel(0, i, WHITE);
        TV.set_pixel(119, i, WHITE);
      }

      if(isAppleZ == false){
        applez_x = random(1, 119);
        applez_y = random(11, 95);
        TV.set_pixel(applez_x, applez_y, WHITE);
        isAppleZ = true;
      }

      if(upBut && !flag1){
        flag1 = true;
        tone(pizePin, 1000, 985);
        isUpZ = true;
        isDownZ = false;
        isRightZ = false;
        isLeftZ = false;
      }
      if(!upBut && flag1){
        flag1 = false;
      }

      if(leftBut && !flag2){
        flag2 = true;
        tone(pizePin, 1000, 985);
        isUpZ = false;
        isDownZ = false;
        isRightZ = false;
        isLeftZ = true;
      }
      if(!leftBut && flag2){
        flag2 = false;
      }

      if(rigthBut && !flag3){
        flag3 = true;
        tone(pizePin, 1000, 985);
        isUpZ = false;
        isDownZ = false;
        isRightZ = true;
        isLeftZ = false;
      }
      if(!rigthBut && flag3){
        flag3 = false;
      }
      
      if(downBut && !flag4){
        flag4 = true;
        tone(pizePin, 1000, 985);
        isUpZ = false;
        isDownZ = true;
        isRightZ = false;
        isLeftZ = false;
      }
      if(!downBut && flag4){
        flag4 = false;
      }
      if(zmey_x == 0 || zmey_x == 119 || zmey_y == 10 || zmey_y == 95){
        tone(pizePin, 1000, 985); 
        TV.clear_screen();
        isZmeyAlive = false;
      }
      if(isUpZ == true){
        if(zmey_coordinates_x.size() >= 2){
          uint8_t predxz = zmey_coordinates_x[0];
          uint8_t predyz = zmey_coordinates_y[0];
          for(uint8_t i = 1; i < zmey_coordinates_x.size(); i++){
            uint8_t p_xz = zmey_coordinates_x[i];
            uint8_t p_yz = zmey_coordinates_y[i];
            zmey_coordinates_x[i] = predxz;
            predxz = p_xz;
            zmey_coordinates_y[i] = predyz;
            predyz = p_yz;
            TV.set_pixel(zmey_coordinates_x[i], zmey_coordinates_y[i], BLACK);
          }   
        }     
        if(zmey_coordinates_x.size() == 1){
          TV.set_pixel(zmey_x, zmey_y, BLACK);
        }
        zmey_y -= 1;
        zmey_coordinates_y[0] = zmey_y;
      }
      if(isDownZ == true){
        if(zmey_coordinates_x.size() >= 2){
          uint8_t predxz = zmey_coordinates_x[0];
          uint8_t predyz = zmey_coordinates_y[0];
          for(uint8_t i = 1; i < zmey_coordinates_x.size(); i++){
            uint8_t p_xz = zmey_coordinates_x[i];
            uint8_t p_yz = zmey_coordinates_y[i];
            zmey_coordinates_x[i] = predxz;
            predxz = p_xz;
            zmey_coordinates_y[i] = predyz;
            predyz = p_yz;
            TV.set_pixel(zmey_coordinates_x[i], zmey_coordinates_y[i], BLACK);
          }   
        }     
        if(zmey_coordinates_x.size() == 1){
          TV.set_pixel(zmey_x, zmey_y, BLACK);
        }
        zmey_y += 1;
        zmey_coordinates_y[0] = zmey_y;
      }
      if(isRightZ == true){
        if(zmey_coordinates_x.size() >= 2){
          uint8_t predxz = zmey_coordinates_x[0];
          uint8_t predyz = zmey_coordinates_y[0];
          for(uint8_t i = 1; i < zmey_coordinates_x.size(); i++){
            uint8_t p_xz = zmey_coordinates_x[i];
            uint8_t p_yz = zmey_coordinates_y[i];
            zmey_coordinates_x[i] = predxz;
            predxz = p_xz;
            zmey_coordinates_y[i] = predyz;
            predyz = p_yz;
            TV.set_pixel(zmey_coordinates_x[i], zmey_coordinates_y[i], BLACK);
          }   
        }     
        if(zmey_coordinates_x.size() == 1){
          TV.set_pixel(zmey_x, zmey_y, BLACK);
        }
        zmey_x += 1;
        zmey_coordinates_x[0] = zmey_x;
      }
      if(isLeftZ == true){
        if(zmey_coordinates_x.size() >= 2){
          uint8_t predxz = zmey_coordinates_x[0];
          uint8_t predyz = zmey_coordinates_y[0];
          for(uint8_t i = 1; i < zmey_coordinates_x.size(); i++){
            uint8_t p_xz = zmey_coordinates_x[i];
            uint8_t p_yz = zmey_coordinates_y[i];
            zmey_coordinates_x[i] = predxz;
            predxz = p_xz;
            zmey_coordinates_y[i] = predyz;
            predyz = p_yz;
            TV.set_pixel(zmey_coordinates_x[i], zmey_coordinates_y[i], BLACK);
          }   
        }     
        if(zmey_coordinates_x.size() == 1){
          TV.set_pixel(zmey_x, zmey_y, BLACK);
        }
        zmey_x -= 1;
        zmey_coordinates_x[0] = zmey_x;
      }
      if(zmey_x == applez_x && zmey_y == applez_y){
        zmey_score += 1;
        isAppleZ = false;
        TV.set_pixel(zmey_x, zmey_y, BLACK);
        tone(pizePin, 1000, 985); 
        if(isUpZ == true){
          zmey_coordinates_x.push_back(zmey_coordinates_x[zmey_coordinates_x.size() - 1]);
          zmey_coordinates_y.push_back(zmey_coordinates_y[zmey_coordinates_y.size() - 1] + 1);
        }
        if(isDownZ == true){
          zmey_coordinates_x.push_back(zmey_coordinates_x[zmey_coordinates_x.size() - 1]);
          zmey_coordinates_y.push_back(zmey_coordinates_y[zmey_coordinates_y.size() - 1] - 1);
        }
        if(isRightZ == true){
          zmey_coordinates_x.push_back(zmey_coordinates_x[zmey_coordinates_x.size() - 1] - 1);
          zmey_coordinates_y.push_back(zmey_coordinates_y[zmey_coordinates_y.size() - 1]);
        }
        if(isLeftZ == true){
          zmey_coordinates_x.push_back(zmey_coordinates_x[zmey_coordinates_x.size() - 1] + 1);
          zmey_coordinates_y.push_back(zmey_coordinates_y[zmey_coordinates_y.size() - 1]);
        }
      }
      if(zmey_coordinates_x.size() >= 2){
        uint8_t predxz = zmey_coordinates_x[0];
        uint8_t predyz = zmey_coordinates_y[0];
        for(uint8_t i = 1; i < zmey_coordinates_x.size(); i++){
          uint8_t p_xz = zmey_coordinates_x[i];
          uint8_t p_yz = zmey_coordinates_y[i];
          zmey_coordinates_x[i] = predxz;
          predxz = p_xz;
          zmey_coordinates_y[i] = predyz;
          predyz = p_yz;
          TV.set_pixel(zmey_coordinates_x[i], zmey_coordinates_y[i], WHITE);
        }   
      }     
      if(zmey_coordinates_x.size() == 1){
        TV.set_pixel(zmey_x, zmey_y, WHITE);
      }
      delay(zmey_speed);
    }
      if(isZmeyAlive == false){
        TV.set_cursor(35, 96 / 2);
        TV.print("GAME OVER");
        TV.set_cursor(35, (96 / 2) + 10);
        TV.print("score - ");
        TV.set_cursor(35 + 50, (96/2) + 10);
        TV.print(zmey_score);
      }
      if(okBut && !flag5){
        tone(pizePin, 1000, 985);
        flag5 = true;
        digitalWrite(resetPin, LOW);
      } 
      if(!okBut && flag5){
        flag5 = false;
      }
  }

  if(isTankim == true){
    if(okBut && !flag5){
      tone(pizePin, 1000, 985);
      flag5 = true;
      digitalWrite(resetPin, LOW);
    }
    if(!okBut && flag5){
      flag5 = false;
    }
  }

  if(isPacmanm == true){
    if(okBut && !flag5){
      tone(pizePin, 1000, 985);
      flag5 = true;
      digitalWrite(resetPin, LOW);
    }
    if(!okBut && flag5){
      flag5 = false;
    }
  }

  if(isTetrism == true){
    if(okBut && !flag5){
      tone(pizePin, 1000, 985);
      flag5 = true;
      digitalWrite(resetPin, LOW);
    }
    if(!okBut && flag5){
      flag5 = false;
    }
  }
}
