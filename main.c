include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <Wire.h< 
#include <LiquidCrystal_I2C.h   ספריית תצוגה  //  <
int getFingerprintIDez(); //

SoftwareSerial mySerial(50,52); // הגדרת חיבורי חיישן טביעת אצבע
LiquidCrystal_I2C lcd(0x27,16,4); //  הגדרת חיבורי וגודל התצוגה
uint8_t getFingerprintEnroll(uint8_t id);  //הגדרת פונקציה  הרשמה 
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
int count=1, i,k=0,parking[4][4]={0}, placei=0,   // 
 placek=0,out[4]={22,24,26,28},
in[5]={99999,30,32,34,36},h=0,j=0,mj=10,mh=10,v=18,idfinger; 
void setup()

{
for(h=0;h<5;h++) pinMode(out[h], OUTPUT);
for(j=1;j<5;j++) pinMode(in[h], INPUT);
lcd.init();

lcd.backlight();
Serial.begin(9600);
finger.begin(57600);
Serial.println("fingertest");
lcd.print("fingertest");
if (finger.verifyPassword()) {
Serial.println("Found fingerprint sensor!");
} else {
Serial.println("Did not find fingerprint sensor :(");
while (1);
}
idfinger=0;
for(i=0;i<10;i++)
{
deleteFingerprint(idfinger);
idfinger++;
}
}
void loop() // run over and over again 
{
Serial.println("Waiting for Fingerprint");
lcd.clear();
lcd.print("Waiting for");
lcd.setCursor(0, 2 );
lcd.print("FingerPrint");
getFingerprintIDez();
delay(800); //don't ned to run this at full speed.
}

int getFingerprintIDez() 
{
uint8_t p = finger.getImage(); 
if (p != FINGERPRINT_OK) return -1; // לא נקלטת טביעת אצבע
p = finger.image2Tz();
if (p != FINGERPRINT_OK) return -1; // תקלה בטביעת אצבע
p = finger.fingerFastSearch();
if (p != FINGERPRINT_OK) // טביעת אצבע לא נמצאת במערכת וצריך להפעיל את תוכנת ההרשמה של טביעת אצבע
{
uint8_t id ;
id=count;
lcd.clear(); 
Serial.print("You are number ");

Serial.print(id); 
lcd.print("You are number ");
lcd.print(id); 
delay(1000);
Serial.print("Start Sign up");
lcd.clear();
lcd.print("Start Sign up");
while (! getFingerprintEnroll(id) );
entergate(); // פתיחת שער כניסה
i=0;
while(i<3)  // בדיקת איזה תא ריק בכל הבניין
{ 
for(k=0;k<3;k++)
{
if(parking[i][k]==0)
{
parking[i][k]=id;
placei=i;
placek=k;
   i=5;
     break;
  }

}
i=i+1;
 } 
 movetree (); //הזזת מעלית לכניסה
 delay(2000);
 move();// להתחיל להזיז את המעלית לתא הרצוי 
  lcd.clear();
  lcd.print(placei);    //הדפסת מיקום שאליו המעלית זזה בתצוגה
  lcd.setCursor(0,1 );
  lcd.print(placek);
  delay(1000);
  lcd.clear();
   count=count+1;
  for(i=0;i<4;i++)    // הדפסת מערך של כל התאים
    { for(k=0;k<4;k++)
       lcd.print(parking[i][k]);
    }
       delay(10000);
  moveziro(); //הזזת מעלית לתא 0,0
  exitgate();
  delay(2000);
  movetree();
  

  return -1;   
  }





    lcd.clear();
// found a match!   טביעת אצבע נמצאת כלומר הרכב נמצא בחניה אז צריך לעבור לתוכנה שמוציאה את הרכב 
אם נקלטה טביעת אצבע והיא לא עונה על האיפים האחרים אז הטביעת אצבע נמצאת במערכת
Serial.print("Found ID #");
Serial.print(finger.fingerID); 
Serial.print(" with confidence of "); Serial.println(finger.confidence);
lcd.clear();
lcd.print("Hello Number "); 
lcd.setCursor(0,1 );
idfinger=finger.fingerID;   // השוואת המשתנה מהטביעה לidfinger
lcd.print(idfinger);
delay(2000);
lcd.clear();
i=0;
while(i<3)
{ 
for(k=0;k<3;k++)
{
if(parking[i][k]==idfinger) // חיפוש התא של הטביעה שנמצאה
{
    parking[i][k]=0;   // החלפה לתא ריק
    deleteFingerprint(idfinger);
    placei=i;
    placek=k;    // שינוי הקומות שאיליהם המעלית צריכה לזוז
     lcd.clear();
    break;
    i=5;
  }
}
i=i+1;
}
movetree ();   // הזזה לכניסה
delay(2000);
move();// להתחיל להזיז את המעלית 
lcd.print(placei);
lcd.setCursor(0,1 );
lcd.print(placek);
lcd.print("Your vehicle left"); 
lcd.setCursor(0,1 );
lcd.print("the parking lot!");
moveziro();
delay(4000); 
circle();   //להזיז את הסיבוב
delay(1000);
exitgate();   // פתיחת שער יציאה
movetree(); //הזזת מעלית לכניסה 
return finger.fingerID; 
}
uint8_t getFingerprintEnroll(uint8_t id) {
uint8_t p = -1;
Serial.println("Waiting for valid finger to enroll");
delay(2000);
lcd.clear();
lcd.print("put finger");
while (p != FINGERPRINT_OK) {
p = finger.getImage();
switch (p) {
case FINGERPRINT_OK:
Serial.println("Image taken");
lcd.clear();
lcd.print("Image taken");
break;
case FINGERPRINT_NOFINGER:
Serial.println(".");
lcd.print(".");
break;
case FINGERPRINT_PACKETRECIEVEERR:
Serial.println("Communication error");
lcd.clear();
lcd.print("Communication error");
break;
case FINGERPRINT_IMAGEFAIL:
Serial.println("Imaging error");
lcd.clear();
lcd.print("Imaging error");
break;
default:
Serial.println("Unknown error");
lcd.clear();
lcd.print("Unknown error");
lcd.setCursor(0, 2 );
lcd.print("Place same finger again");
break;
}
}
// OK success!
p = finger.image2Tz(1);
switch (p) {
case FINGERPRINT_OK:
Serial.println("Image converted");
lcd.clear();
lcd.print("Image converted");
break;
case FINGERPRINT_IMAGEMESS:
Serial.println("Image too messy");
lcd.clear();
lcd.print("Image too messy");
return p;
case FINGERPRINT_PACKETRECIEVEERR:
Serial.println("Communication error");
lcd.clear();
lcd.print("Communication error");
return p;
case FINGERPRINT_FEATUREFAIL:
Serial.println("Could not find fingerprint features");
lcd.clear();
lcd.print("Could not find fingerprint features");
return p;
case FINGERPRINT_INVALIDIMAGE:
Serial.println("Could not find fingerprint features");
lcd.clear();
lcd.print("Could not find fingerprint features");
return p;
default:
Serial.println("Unknown error");
lcd.clear();
lcd.print("Unknown error");
lcd.setCursor(0, 2 );
lcd.print("Place same finger again");
return p;
}
Serial.println("Remove finger");
lcd.clear();
lcd.print("Remove finger");
delay(2000);
p = 0;
while (p != FINGERPRINT_NOFINGER) {
p = finger.getImage();
}
p = -1;
Serial.println("Place same finger again");
lcd.clear();
lcd.print("Place same");
lcd.setCursor(0,1);
lcd.print("finger again ");
while (p != FINGERPRINT_OK) {
p = finger.getImage();
switch (p) {
case FINGERPRINT_OK:
Serial.println("Image taken");
lcd.clear();
lcd.print("Image taken :)");
lcd.setCursor(0, 2 );
lcd.print("Remove Finger");
delay(1500);
lcd.clear();
lcd.print("Sign parking");
delay(2000);
break;
case FINGERPRINT_NOFINGER:
Serial.print(".");
lcd.print(".");
break;
case FINGERPRINT_PACKETRECIEVEERR:
Serial.println("Communication error");
lcd.clear();
lcd.print("Communication error");
break;
case FINGERPRINT_IMAGEFAIL:
Serial.println("Imaging error");
lcd.clear();
lcd.print("Imaging error");
break;
default:
Serial.println("Unknown error");
lcd.clear();
lcd.print("Unknown error");
lcd.setCursor(0, 2 );
lcd.print("Place same finger again");
break;
}
}
// OK success!
p = finger.image2Tz(2);
switch (p) {
case FINGERPRINT_OK:
Serial.println("Image converted");
break;
case FINGERPRINT_IMAGEMESS:
Serial.println("Image too messy");
return p;
case FINGERPRINT_PACKETRECIEVEERR:
Serial.println("Communication error");
return p;
case FINGERPRINT_FEATUREFAIL:
Serial.println("Could not find fingerprint features");
return p;
case FINGERPRINT_INVALIDIMAGE:
Serial.println("Could not find fingerprint features");
return p;
default:
Serial.println("Unknown error");
lcd.setCursor(0, 2 );
lcd.print("Place same finger again");
return p;
}
// OK converted!
p = finger.createModel();
if (p == FINGERPRINT_OK) {
Serial.println("Prints matched!");
} else if (p == FINGERPRINT_PACKETRECIEVEERR) {
Serial.println("Communication error");
return p;
} else if (p == FINGERPRINT_ENROLLMISMATCH) {
Serial.println("Fingerprints did not match");
return p;
} else {
Serial.println("Unknown error");
lcd.setCursor(0, 2 );
lcd.print("Place same finger again");
return p;
}
p = finger.storeModel(id);
if (p == FINGERPRINT_OK) {
Serial.println("Stored!");
} else if (p == FINGERPRINT_PACKETRECIEVEERR) {
Serial.println("Communication error");
return p;
} else if (p == FINGERPRINT_BADLOCATION) {
Serial.println("Could not store in that location");
return p;
} else if (p == FINGERPRINT_FLASHERR) {
Serial.println("Error writing to flash");
return p;
} else {
Serial.println("Unknown error");
lcd.setCursor(0, 2 );
lcd.print("Place same finger again");
return p;
} //הסתיימה הרשמת קליטת האצבע
}
void move() //תת שגרה להזזת מעלית לתא הרצוי
{
while(1)
{
for(h=0;h<4;h++) // סריקת חיישנים
{
digitalWrite(out[0], HIGH);//
digitalWrite(out[1], HIGH);//
digitalWrite(out[2], HIGH);//
digitalWrite(out[3], HIGH);// 
digitalWrite(out[h], LOW);//
for(j=1;j<5;j++) 
{ 
v=digitalRead(in[j]);
   Serial.println(v);
   if(v==0) 
      {
      mj=j-1;
      mh=h;
      Serial.print("toor:");
      Serial.print(mh);
      Serial.print("                shoora:");
      Serial.println(mj);
}
}
if(mj>placei)  // אם המיקום הנוכחי גדול מהמיקום הרצוי((שורה
{
analogWrite(8,0);
analogWrite(9,130);
}
if(mj<placei) // אם המיקום הנוכחי קטן מהמיקום הרצוי((שורה
{
analogWrite(8,255);
analogWrite(9,0);
}
if(mj==placei) // אם המיקום הנוכחי שווה להמיקום הרצוי((שורה
{
analogWrite(8,0);
analogWrite(9,0);
if(mh>placek) // אם המיקום הנוכחי גדול מהמיקום הרצוי(טור)
{
analogWrite(2,0);
analogWrite(3,210);
}
if(mh<placek) // אם המיקום הנוכחי קטו מהמיקום הרצוי(טור)
{
analogWrite(2,210);
analogWrite(3,0);
}
if(mh==placek) // אם המיקום הנוכחי שווה להמיקום הרצוי(טור)
{
analogWrite(2,0);
analogWrite(3,0);
mh=placek;
}
}
delay(100);
}
if(placek==mh&placei==mj)  // אם המעלית במיקום הרצוי לגמרי אז יציאה הוא יוצא מההזזה של המעלית
break;
}
}

  void movetree () // תת שיגרה להזזת מעלית לכניסה
    {

  while(1)
    {
                for(h=0;h<4;h++)  //

{
digitalWrite(out[0], HIGH);//
digitalWrite(out[1], HIGH);//
digitalWrite(out[2], HIGH);//
digitalWrite(out[3], HIGH);// 
digitalWrite(out[h], LOW);//
for(j=1;j<5;j++) 
{

   v=digitalRead(in[j]);
   Serial.println(v);
   if(v==0) 
      {
      mj=j-1;
      mh=h;
      Serial.print("toor:");
      Serial.print(mh);
      Serial.print("                shoora:");
      Serial.println(mj);
}

}
}
if(mj>0) // אם המיקום הנוכחי גדול מאפס((שורה
{
analogWrite(8,0);
analogWrite(9,130);
}
if(mj==0) // אם המיקום הנוכחי שווה לאפס((שורה
{
analogWrite(8,0);
analogWrite(9,0);
if(mh<2) // אם המיקום הנוכחי גדול מ2((טור
{
analogWrite(2,200);
analogWrite(3,0);
}
if(mh==2) // אם המיקום הנוכחי שווה ל2((טור
{
analogWrite(2,0);
analogWrite(3,0);
mh=2;
}
delay(100);
}
if(mh==2&&mj==0) // אם המעלית במיקום הרצוי לגמרי אז יציאה הוא יוצא מההזזה של המעלית
break;
}
}

     void moveziro () // תת שגרה הזזת מעלית לתא 0,0
    {

  while(1)
    {
                for(h=0;h<4;h++)

{
digitalWrite(out[0], HIGH);//
digitalWrite(out[1], HIGH);//
digitalWrite(out[2], HIGH);//
digitalWrite(out[3], HIGH);// 
digitalWrite(out[h], LOW);//
for(j=1;j<5;j++) 
{

  v=digitalRead(in[j]);
   Serial.println(v);
   if(v==0) 
      {
      mj=j-1;
      mh=h;
      Serial.print("toor:");
      Serial.print(mh);
      Serial.print("                shoora:");
      Serial.println(mj);
}
}
if(mj>0)
{
analogWrite(8,0);
analogWrite(9,130);
}
if(mj==0)
{
analogWrite(8,0);
analogWrite(9,0);
if(mh>0)
{
analogWrite(2,0);
analogWrite(3,200);
}
if(mh==0)
{
analogWrite(2,0);
analogWrite(3,0);
mh=0;
}
}
delay(100);
}
if(mh==0&&mj==0)
break;
}
}
uint8_t deleteFingerprint(uint8_t id) {  // תת שגרה למחיקת הטביעות אצבע
uint8_t p = -1;
p = finger.deleteModel(idfinger);
if (p == FINGERPRINT_OK) {
Serial.println("Deleted!");
} else if (p == FINGERPRINT_PACKETRECIEVEERR) {
Serial.println("Communication error");
return p;
} else if (p == FINGERPRINT_BADLOCATION) {
Serial.println("Could not delete in that location");
return p;
} else if (p == FINGERPRINT_FLASHERR) {
Serial.println("Error writing to flash");
return p;
} else {
Serial.print("Unknown error: 0x"); Serial.println(p, HEX);
return p;
}

}
void exitgate() { // תת שגרה לפתוח שער יציאה
analogWrite(6,200);
analogWrite(7,0);
delay(180);
analogWrite(6,0);
delay(3000);
analogWrite(6,0);
analogWrite(7,185);
delay(180);
analogWrite(7,0);
}
void entergate() { //תת שגרה לפתוח שער כניסה
analogWrite(4,200);
analogWrite(5,0);
delay(300);
analogWrite(4,0);
delay(3000);
analogWrite(4,0);
analogWrite(5,185);
delay(300);
analogWrite(5,0);
}
void circle(){
analogWrite(12,200);
analogWrite(13,0);
delay(400);
analogWrite(12,0);
}
