#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

byte goblin[8] = {
  B01110,
  B10101,
  B11111,
  B10101,
  B01110,
  B00100,
  B01010,
  B10001
};

byte orc[8] = {
  B10001,
  B01110,
  B11111,
  B10101,
  B11111,
  B01110,
  B10101,
  B00000
};

byte dragon[8] = {
  B11000,
  B11100,
  B10110,
  B11111,
  B01110,
  B11100,
  B10100,
  B00110
};

const int hpLed1 = 2;
const int hpLed2 = 3;
const int hpLed3 = 4;
const int headBtn = 5;
const int bodyBtn = 6;
const int legBtn  = 7;
const int actionBtn = 8;
const int actionLed = 9;
int difficulty = 1;
int stage = 0;
int fightRound = 0;
bool parried;
int attackType;
int hitChance;
int playerHealth = 3;
int goblinHealth = 3;
int orcHealth = 8;
int dragonHealth = 20;

void advanceStage(){
  stage++;
}

void playerTakeDamage(){
  playerHealth--;
  if(playerHealth == 2){digitalWrite(hpLed3,LOW);}
  if(playerHealth == 1){digitalWrite(hpLed2,LOW);}
  if(playerHealth == 0){digitalWrite(hpLed1,LOW); gameOver();}
  
}

void gameOver(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("   GAME OVER!");
  lcd.setCursor(0,1);
  lcd.print("Press any button");
  waitForAnyBtn();
  stage = 0;
  fightRound = 0;
}

bool chooseDifficulty(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Difficulty?");
  lcd.setCursor(0, 1);
  lcd.print("1:G 2:B 3:R");
  int selected = waitForAttackBtn();
  if(selected == headBtn){ difficulty = 3;}
  else if(selected == bodyBtn){ difficulty = 6;}
  else if(selected == legBtn){ difficulty = 9;}
  return true;

}

bool waitForAnyBtn(){
  while(true){
    if(digitalRead(headBtn) == HIGH || digitalRead(bodyBtn) == HIGH || digitalRead(legBtn) == HIGH || digitalRead(actionBtn) == HIGH){
      delay(200);
      return true;
    }
  }
}

int waitForAttackBtn(){
  while(true){
    if(digitalRead(headBtn) == HIGH){
      delay(200);
      return headBtn;
    }
        if(digitalRead(bodyBtn) == HIGH){
      delay(200);
      return bodyBtn;
    }
        if(digitalRead(legBtn) == HIGH){
      delay(200);
      return legBtn;
    }
  }
}

bool waitForActionBtn() {
  int reactionTime = 1000 - (difficulty * 100);

  unsigned long startTime = millis();

  digitalWrite(actionLed, HIGH);

  while (millis() - startTime < reactionTime) {
    if (digitalRead(actionBtn) == HIGH) {
      digitalWrite(actionLed, LOW);
      delay(200);
      return true;
    }
  }

  digitalWrite(actionLed, LOW);
  return false;
}

void goblinEncounterScript(){
    lcd.clear();
  lcd.setCursor(0, 0);
    lcd.write(byte(0));
  lcd.print(" goblin attacks");
  lcd.setCursor(0, 1);
  lcd.print("Get Ready!");
  for(int i = 5; i > 0; i--){
    lcd.setCursor(0,1);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print("Time left: ");
    lcd.print(i);
    delay(2000);
  }
  lcd.clear();
  fightRound++;
}

void orcEncounterScript() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write(byte(1));
  lcd.print(" orc appears!");
  lcd.setCursor(0, 1);
  lcd.print("Get Ready!");

  for (int i = 5; i > 0; i--) {
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(0, 1);
    lcd.print("Time left: ");
    lcd.print(i);
    delay(2000);
  }

  lcd.clear();
  fightRound++;
}

void dragonEncounterScript() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write(byte(2));
  lcd.print(" dragon wakes!");
  lcd.setCursor(0, 1);
  lcd.print("Get Ready!");

  for (int i = 5; i > 0; i--) {
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(0, 1);
    lcd.print("Time left: ");
    lcd.print(i);
    delay(2000);
  }

  lcd.clear();
  fightRound++;
}

void goblinFightScript() { 
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write(byte(0));
  lcd.print(" incoming");
  lcd.setCursor(0, 1);
  lcd.print("strike!");
  delay(2000);

  parried = waitForActionBtn();

  if (!parried) {
    playerTakeDamage();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("You've got");
    lcd.setCursor(0, 1);
    lcd.print("HIT!");
    delay(2000);
  } 
  else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("You parried");
    lcd.setCursor(0, 1);
    lcd.print("the strike!");
    delay(2000);
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Choose Attack!");
  lcd.setCursor(0, 1);
  lcd.print("R:H B:B G:L");

  attackType = waitForAttackBtn();
  hitChance = random(101);

  if (attackType == headBtn) {
    if (hitChance > 25) {
      goblinHealth = goblinHealth - 5;

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("You hit him!");
      lcd.setCursor(0, 1);
      lcd.write(byte(0));
      lcd.print(" HP:");
      lcd.print(goblinHealth);
      delay(2000);

      if (goblinHealth <= 0) {
        advanceStage();
        fightRound = 0;
        return;
      }
    } 
    else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("You missed!");
      lcd.setCursor(0, 1);
      lcd.write(byte(0));
      lcd.print(" HP:");
      lcd.print(goblinHealth);
      delay(2000);
    }
  }

  if (attackType == bodyBtn) {
    if (hitChance > 15) {
      goblinHealth = goblinHealth - 3;

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("You hit him!");
      lcd.setCursor(0, 1);
      lcd.write(byte(0));
      lcd.print(" HP:");
      lcd.print(goblinHealth);
      delay(2000);

      if (goblinHealth <= 0) {
        advanceStage();
        fightRound = 0;
        return;
      }
    } 
    else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("You missed!");
      lcd.setCursor(0, 1);
      lcd.write(byte(0));
      lcd.print(" HP:");
      lcd.print(goblinHealth);
      delay(2000);
    }
  }

  if (attackType == legBtn) {
    goblinHealth = goblinHealth - 2;

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("You hit him!");
    lcd.setCursor(0, 1);
    lcd.write(byte(0));
    lcd.print(" HP:");
    lcd.print(goblinHealth);
    delay(2000);

    if (goblinHealth <= 0) {
      advanceStage();
      fightRound = 0;
      return;
    }
  }
}

void orcFightScript() { 
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write(byte(1));
  lcd.print(" incoming");
  lcd.setCursor(0, 1);
  lcd.print("heavy strike!");
  delay(2000);

  parried = waitForActionBtn();

  if (!parried) {
    playerTakeDamage();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Orc hit you!");
    lcd.setCursor(0, 1);
    lcd.print("-1 HP");
    delay(2000);
  } 
  else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("You parried");
    lcd.setCursor(0, 1);
    lcd.print("the orc!");
    delay(2000);
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Choose Attack!");
  lcd.setCursor(0, 1);
  lcd.print("R:H B:B G:L");

  attackType = waitForAttackBtn();
  hitChance = random(101);

  if (attackType == headBtn) {
    if (hitChance > 35) {
      orcHealth = orcHealth - 6;

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Head hit!");
      lcd.setCursor(0, 1);
      lcd.write(byte(1));
      lcd.print(" HP:");
      lcd.print(orcHealth);
      delay(2000);

      if (orcHealth <= 0) {
        advanceStage();
        fightRound = 0;
        return;
      }
    } 
    else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("You missed!");
      lcd.setCursor(0, 1);
      lcd.write(byte(1));
      lcd.print(" HP:");
      lcd.print(orcHealth);
      delay(2000);
    }
  }

  if (attackType == bodyBtn) {
    if (hitChance > 15) {
      orcHealth = orcHealth - 4;

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Body hit!");
      lcd.setCursor(0, 1);
      lcd.write(byte(1));
      lcd.print(" HP:");
      lcd.print(orcHealth);
      delay(2000);

      if (orcHealth <= 0) {
        advanceStage();
        fightRound = 0;
        return;
      }
    } 
    else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("You missed!");
      lcd.setCursor(0, 1);
      lcd.write(byte(1));
      lcd.print(" HP:");
      lcd.print(orcHealth);
      delay(2000);
    }
  }

  if (attackType == legBtn) {
    orcHealth = orcHealth - 2;

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Leg hit!");
    lcd.setCursor(0, 1);
    lcd.write(byte(1));
    lcd.print(" HP:");
    lcd.print(orcHealth);
    delay(2000);

    if (orcHealth <= 0) {
      advanceStage();
      fightRound = 0;
      return;
    }
  }
}

void dragonFightScript() { 
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write(byte(2));
  lcd.print(" fire breath");
  lcd.setCursor(0, 1);
  lcd.print("incoming!");
  delay(2000);

  parried = waitForActionBtn();

  if (!parried) {
    playerTakeDamage();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Burned by");
    lcd.setCursor(0, 1);
    lcd.print("dragon fire!");
    delay(2000);
  } 
  else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("You dodged");
    lcd.setCursor(0, 1);
    lcd.print("the flames!");
    delay(2000);
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Choose Attack!");
  lcd.setCursor(0, 1);
  lcd.print("R:H B:B G:L");

  attackType = waitForAttackBtn();
  hitChance = random(101);

  if (attackType == headBtn) {
    if (hitChance > 45) {
      dragonHealth = dragonHealth - 8;

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Head hit!");
      lcd.setCursor(0, 1);
      lcd.write(byte(2));
      lcd.print(" HP:");
      lcd.print(dragonHealth);
      delay(2000);

      if (dragonHealth <= 0) {
        advanceStage();
        fightRound = 0;
        return;
      }
    } 
    else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("You missed!");
      lcd.setCursor(0, 1);
      lcd.write(byte(2));
      lcd.print(" HP:");
      lcd.print(dragonHealth);
      delay(2000);
    }
  }

  if (attackType == bodyBtn) {
    if (hitChance > 25) {
      dragonHealth = dragonHealth - 5;

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Body hit!");
      lcd.setCursor(0, 1);
      lcd.write(byte(2));
      lcd.print(" HP:");
      lcd.print(dragonHealth);
      delay(2000);

      if (dragonHealth <= 0) {
        advanceStage();
        fightRound = 0;
        return;
      }
    } 
    else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("You missed!");
      lcd.setCursor(0, 1);
      lcd.write(byte(2));
      lcd.print(" HP:");
      lcd.print(dragonHealth);
      delay(2000);
    }
  }

  if (attackType == legBtn) {
    dragonHealth = dragonHealth - 3;

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Leg hit!");
    lcd.setCursor(0, 1);
    lcd.write(byte(2));
    lcd.print(" HP:");
    lcd.print(dragonHealth);
    delay(2000);

    if (dragonHealth <= 0) {
      advanceStage();
      fightRound = 0;
      return;
    }
  }
}
void gameWon(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("   GAME WON!");
  lcd.setCursor(0,1);
  lcd.print("Press any button");
  waitForAnyBtn();
  stage = 0;
  fightRound = 0;
}

void setup() {
  digitalWrite(hpLed1, HIGH);
  digitalWrite(hpLed2, HIGH);
  digitalWrite(hpLed3, HIGH);

  lcd.init();
  lcd.backlight();
  randomSeed(analogRead(0));

  // Store custom character in slots
  lcd.createChar(0, goblin);
  lcd.createChar(1, orc);
  lcd.createChar(2, dragon);
  for(int i = 2; i < 5; i++){
    pinMode(i, OUTPUT); // HP leds
  }
  for(int i = 5; i < 8; i++){
    pinMode(i, INPUT); // attack buttons
  }
  pinMode(8, INPUT); // DODGE BUTTON
  pinMode(9, OUTPUT); // ACTION SIGNAL LED
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Press a button");
  lcd.setCursor(0, 1);
  lcd.print("to start!");


}

void loop() {
if(stage == 0){
waitForAnyBtn();
chooseDifficulty();
advanceStage();
}
if(stage == 1){
  if(fightRound == 0){  goblinEncounterScript();}
  if(fightRound == 1){
    goblinFightScript();
  }
}
if(stage == 2){
  if(fightRound == 0){  orcEncounterScript();}
  if(fightRound == 1){
    orcFightScript();
  }
}
if(stage == 3){
  if(fightRound == 0){  dragonEncounterScript();}
  if(fightRound == 1){
    dragonFightScript();
  }
}
if(stage == 4){
  gameWon();
}






}