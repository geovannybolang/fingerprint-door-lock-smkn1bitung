/*************************************************** 
 PROJECT FINGERPRINT DOOR LOCK TKJ LABORATORY
 FINAL CODE : AUGUST 2, 2020
 FINAL INSTALLATION : AUGUST 4, 2020
 PROCESSING TIME : 3 DAYS SINCE JULY 31, 2020
 ****************************************************/


#include <Adafruit_Fingerprint.h>
#include <IRremote.h>
#define lock 12
const int RECV_PIN = 11; // lokasi pin data dari IR Recaiver
IRrecv irrecv(RECV_PIN);
decode_results results;
const int BUTTON_PIN = 4;
const int buzzerPin = 5; 
const int  ledHijau = 7;
const int ledKuning = 6;
SoftwareSerial mySerial(2, 3); //RX = D2, TX = D3

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup()  
{
  pinMode (lock,OUTPUT);
  digitalWrite(lock,HIGH);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode (buzzerPin, OUTPUT);
  pinMode (ledHijau, OUTPUT);
  pinMode (ledKuning, OUTPUT);
  Serial.begin(9600);
  irrecv.enableIRIn();
  while (!Serial);  
  delay(100);
  Serial.println("Tunggu Sebentar ");
  unsigned long data;

  finger.begin(57600);
  delay(5);
  if (finger.verifyPassword()) {
    Serial.println("Fingerprint siap diginakan");
  } else {
    Serial.println("Fingerprint eror :( ");
    Serial.println("Coba cek pengkabelan ");
    while (1) { delay(1); }
  }

  finger.getTemplateCount();
  Serial.print("Terdapat "); Serial.print(finger.templateCount); Serial.println(" sidik jari yang terdaftar");
  Serial.println("Silahkan tempelkan jarimu");
}
unsigned long data;
void loop()                     
{
  getFingerprintIDez();
  delay(50);            
}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
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
      return p;
  }
  
  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence); 
  return finger.fingerID;
}

int getFingerprintIDez() {
  digitalWrite(ledHijau, HIGH);
  digitalWrite(ledHijau,LOW);
  
  
  if (irrecv.decode(&results)) {
    Serial.println(results.value);
    data = results.value;
    irrecv.resume();
    switch (data){
      case 3880798391: // 3880798391 adalah kode nomor tombol yang ada di remote
    digitalWrite(buzzerPin, HIGH);
    delay(100);
    digitalWrite(buzzerPin, LOW);
    delay(100);
    digitalWrite(buzzerPin, HIGH);
    delay(100);
    digitalWrite(buzzerPin, HIGH);
    delay(100);
    digitalWrite(buzzerPin, LOW);
    delay(100);
    digitalWrite(lock, LOW);
    digitalWrite(ledKuning, HIGH);
    delay(3000); 
    delay(2000);
    digitalWrite(ledKuning, LOW); 
        break;
    }
  }
  int buttonState = digitalRead(BUTTON_PIN); // read new state

  //PUSH BUTTON CONDITION
  if (buttonState == LOW) { //TURN ON PUSH BUTTON
    digitalWrite(buzzerPin, HIGH);
    delay(100);
    digitalWrite(buzzerPin, LOW);
    delay(100);
    digitalWrite(buzzerPin, HIGH);
    delay(100);
    digitalWrite(buzzerPin, HIGH);
    delay(100);
    digitalWrite(buzzerPin, LOW);
    delay(100);
    digitalWrite(lock, LOW);
    digitalWrite(ledKuning, HIGH);
    delay(3000); 
    delay(2000);
    digitalWrite(ledKuning, LOW);
  }
  else
  if (buttonState == HIGH) { //TURN OFF PUSH BUTTON
    digitalWrite(lock, HIGH);
  }
  
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  // JIKA FINGERPRINT TERBACA
  Serial.print("Terdeteksi jari no : "); Serial.print(finger.fingerID); 
  Serial.print(" Dengan keakuratan : "); Serial.print(finger.confidence-8); Serial.println("%"); 
  //FINGERPRINT CONDITION
  if (finger.confidence>30){ 
    Serial.println("Pintu terbuka"); //TURN ON PUSH BUTTON 
    digitalWrite(buzzerPin, HIGH);
    delay(100);
    digitalWrite(buzzerPin, LOW);
    delay(100);
    digitalWrite(buzzerPin, HIGH);
    delay(100);
    digitalWrite(buzzerPin, HIGH);
    delay(100);
    digitalWrite(buzzerPin, LOW);
    delay(100);
    digitalWrite(lock, LOW);
    digitalWrite(ledKuning, HIGH);
    delay(3000); 
    delay(2000);
    digitalWrite(ledKuning, LOW);
  }
  
  return finger.fingerID;

  
   
}
