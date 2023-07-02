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
  pinMode(10, INPUT_PULLUP); //down

  pizePin = 11; //pisk 
  resetPin = 12; //reset

  digitalWrite(resetPin, HIGH);
  pinMode(resetPin, OUTPUT); 

}
void(* resetFunc) (void) = 0; // reset proc

// zmeyka var
uint8_t zmey_x = 120 / 2;
uint8_t zmey_y = 96 / 2;
bool isZmeyAlive = true;
int zmey_score = 0;
vector<int> zmey_coordinates_x;
vector<int> zmey_coordinates_y;
int zmey_speed = 1000; // pix in milisec
bool isLeft = false;
bool isRight = true;
bool isUp = false;
bool isDown = false;

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

      if(upBut && !flag1){
        flag1 = true;
        tone(pizePin, 1000, 985);
        isUp = true;
        isDown = false;
        isRight = false;
        isLeft = false;
      }
      if(!upBut && flag1){
        flag1 = false;
      }

      if(leftBut && !flag2){
        flag2 = true;
        tone(pizePin, 1000, 985);
        isUp = false;
        isDown = false;
        isRight = false;
        isLeft = true;
      }
      if(!leftBut && flag2){
        flag2 = false;
      }

      if(rigthBut && !flag3){
        flag3 = true;
        tone(pizePin, 1000, 985);
        isUp = false;
        isDown = false;
        isRight = true;
        isLeft = false;
      }
      if(!rigthBut && flag3){
        flag3 = false;
      }
      
      if(downBut && !flag4){
        flag4 = true;
        tone(pizePin, 1000, 985);
        isUp = false;
        isDown = true;
        isRight = false;
        isLeft = false;
      }
      if(!downBut && flag4){
        flag4 = false;
      }
      if(zmey_x == 0 || zmey_x == 120 || zmey_y == 0 || zmey_y == 96){
        tone(pizePin, 1000, 985); 
        TV.clear_screen();
        isZmeyAlive = false;
      }
      if(isUp == true){
        TV.set_pixel(zmey_x, zmey_y, BLACK);
        zmey_y -= 1;
      }
      if(isDown == true){
        TV.set_pixel(zmey_x, zmey_y, BLACK);
        zmey_y += 1;
      }
      if(isRight == true){
        TV.set_pixel(zmey_x, zmey_y, BLACK);
        zmey_x += 1;
      }
      if(isLeft == true){
        TV.set_pixel(zmey_x, zmey_y, BLACK);
        zmey_x -= 1;
      }
      delay(zmey_speed);
      TV.set_pixel(zmey_x, zmey_y, WHITE);
    }
      if(isZmeyAlive == false){
        TV.set_cursor(35, 96 / 2);
        TV.print("GAME OVER");
        TV.set_cursor(35, (96 / 2) + 10);
        TV.print("score - ");
        TV.set_cursor(35 + 8, (96/2) + 10);
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
      TV.clear_screen();
      flag5 = true;
      isRes = true;
      isTankim = false;
    }
    if(!okBut && flag5){
      flag5 = false;
    }
  }

  if(isPacmanm == true){
    if(okBut && !flag5){
      TV.clear_screen();
      flag5 = true;
      isRes = true;
      isPacmanm = false;
    }
    if(!okBut && flag5){
      flag5 = false;
    }
  }

  if(isTetrism == true){
    if(okBut && !flag5){
      TV.clear_screen();
      flag5 = true;
      isRes = true;
      isTetrism = false;
    }
    if(!okBut && flag5){
      flag5 = false;
    }
  }
}
