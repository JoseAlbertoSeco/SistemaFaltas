#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "ListLib.h"
#include <HardwareSerial.h>
#include <FPM.h>


WiFiMulti wifiMulti;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
int ID_ESP32 = 1;
String DIR = "--- DIRECCIÓN DEL SERVIDOR WEB ---";
StaticJsonDocument<50000> doc;
List<int> list;
int idAlus[10];
int HORAPRUEBA = 1086;

#define BUFF_SZ 768
uint8_t template_buffer[BUFF_SZ];
char *name = NULL;

HardwareSerial fserial(2);
FPM finger(&fserial);
FPM_System_Params params;

const int ledV = 27;
const int ledR = 21;

void setup() {
  //KTEUH3EAJ75D56K5
  Serial.begin(115200);
  Serial.println("TEMPLATES test");
  fserial.begin(57600);
  pinMode (ledV, OUTPUT);
  pinMode (ledR, OUTPUT);
  wifiMulti.addAP("SSID", "PASSWORD");

  if (finger.begin()) {
    finger.readParams(&params);
    Serial.println("Found fingerprint sensor!");
    Serial.print("Capacity: "); Serial.println(params.capacity);
    Serial.print("Packet length: "); Serial.println(FPM::packet_lengths[params.packet_len]);
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) yield();
  }
  finger.led_on();
  Serial.println("\n\n------------------------------------------------------------");
  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }
  wifiMulti.addAP("SSID", "PASSWORD");
  WiFi.begin("SSID", "PASSWORD");
  timeClient.begin();
}
void loop() {
  search_database();
}

void checkHora() {
  Serial.println("[HORA] ESPERANDO DEDO... ");
  timeClient.update();
  timeClient.setTimeOffset(3600);
  int horas = timeClient.getHours();
  int minutos = timeClient.getMinutes();
  int hora = (horas * 60) + minutos;
  finger.led_on();
  if (inRange(hora, 475, 477) || inRange(hora, 535, 537) || inRange(hora, 595, 597) || inRange(hora, 685, 687) || inRange(hora, 745, 747) || inRange(hora, 805, 807) || hora == HORAPRUEBA) {
    finger.led_off();
    digitalWrite (ledR, HIGH);
    readJSON();
    delay(60000);
    Serial.println("[HORA] SE HAN ACTUALIZADO LAS HUELLAS!!");
    digitalWrite (ledR, LOW);
    finger.led_on();
  }
}
/*  ------------- getHora ------------
    Obtienes la hora actual en minutos
    ----------------------------------
*/
int getHora() {
  Serial.print("[HORA] COMPROBANDO HORA ACTUAL... ");
  timeClient.update();
  timeClient.setTimeOffset(3600);
  int horas = timeClient.getHours();
  int minutos = timeClient.getMinutes();
  int hora = (horas * 60) + minutos;
  return hora;
}
/*  -------------- huellaToArray ---------------
    Como las huellas están el char[] no nos vale
    Tenemos que convertir ese char[] en un array
    Una vez convertido, se almacena en el lector
    --------------------------------------------
*/
void huellaToArray(char msg[], int pos) {
  name = strtok(msg, ",");
  int i = 0;
  int convertido = 0;
  while (name != NULL)
  {
    convertido = String(name).toInt();
    template_buffer[i] = convertido;
    //Serial.println(convertido);
    name = strtok(NULL, ",");
    i++;
  }
  for (int j = 0; j < 768; j++) {
    Serial.print(template_buffer[j]);
    Serial.print(" ");
  }
  Serial.println();
  move_template(pos, template_buffer, BUFF_SZ);
}
/*  ------------------------------------- readJSON ----------------------------------------------
    Mediante HTTP se conecta al servidor Apache
    En él lee el archivo JSON que se llama como el id del aula en el que se ubica el lector
    El JSON contiene todos los alumnos que tienen clase a esa hora, es decir, los ids y las huellas
    ---------------------------------------------------------------------------------------------
*/
void readJSON() {
  empty_database();
  // wait for WiFi connection
  list.Clear();
  if ((wifiMulti.run() == WL_CONNECTED)) {
    Serial.println("[JSON] SE VAN A LEER LAS HUELLAS DEL ARCHIVO");
    timeClient.begin();
    HTTPClient http;
    //Serial.print("\n\n[HTTP] begin...\n");
    http.begin("http://" + DIR + "/Templates/" + ID_ESP32 + ".json"); //HTTP
    //Serial.print("[HTTP] GET...\n");
    int httpCode = http.GET();
    delay(5000);
    while (httpCode != HTTP_CODE_OK) {
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);
      httpCode = http.GET();
      delay(5000);
    }
    String payload = http.getString();
    Serial.println("------ JSON CODE ------\n" + payload);
    DeserializationError error = deserializeJson(doc, payload);
    if (error) {
      Serial.println("[JSON] CONECTANDO DE NUEVO...");
      readJSON();
    }
    int count = doc["count"];
    for (int i = 0; i < count; i++) {
      if (doc["alumnos"][i]["id"] != NULL) {
        int latitude = doc["alumnos"][i]["id"];
        list.Add(latitude);
        idAlus[i] = latitude;
        Serial.print("[JSON] ID LEÍDO: " + latitude);
        String sHuella = doc["alumnos"][i]["huella"];
        char cHuella[sHuella.length()];
        sHuella.toCharArray(cHuella, sHuella.length());
        huellaToArray(cHuella, i + 1);
      }
    }
    http.end();
  } else {
    Serial.println("[JSON] ERROR, PROBLEMA CON EL WIFI...");
    readJSON();
  }
  delay(5000);
}

void move_template(uint16_t fid, uint8_t * buffer, uint16_t to_write) {
  int16_t p = finger.uploadModel();
  switch (p) {
    case FPM_OK:
      Serial.println("[MOVE_T] SUBIENDO HUELLA");
      break;
    default:
      Serial.print("[MOVE_T] ¡¡ERROR!! ");
      Serial.println(p);
      return;
  }

  yield();
  finger.writeRaw(buffer, to_write);

  p = finger.storeModel(fid);
  switch (p) {
    case FPM_OK:
      Serial.print("[MOVE] HUELLA ALMACENADA EN EL ID "); Serial.println(fid);
      break;
    default:
      Serial.print("[MOVE_T] ¡¡ERROR!!");
      Serial.println(p);
      break;
  }
  return;
}

void empty_database(void) {
  int16_t p = finger.emptyDatabase();
  if (p == FPM_OK) {
    Serial.println("[EMPTY] LECTOR VACIADO!");
  } else {
    Serial.println("[EMPTY] HA OCURRIDO UN ERROR");
  }
}

bool inRange(int val, int minimum, int maximum) {
  return ((minimum <= val) && (val <= maximum));
}
int search_database(void) {
  int16_t p = -1;

  /* first get the finger image */
  while (p != FPM_OK) {
    p = finger.getImage();
    switch (p) {
      case FPM_OK:
        Serial.println("[SEARCH] DEDO DETECTADO");
        break;
      case FPM_NOFINGER:
        checkHora();
        break;
      default:
        Serial.println("[SEARCH] ¡¡ERROR!!");
        errorFinger();
        break;
    }
    yield();
  }
  digitalWrite (ledR, LOW);
  digitalWrite (ledV, HIGH);
  /* convert it */
  p = finger.image2Tz();
  switch (p) {
    case FPM_OK:
      Serial.println("[SEARCH] IMAGEN CONVERTIDA");
      break;
    default:
      Serial.println("[SEARCH] ¡¡ERROR!!");
      errorFinger();
      return p;
  }

  /* search the database for the converted print */
  uint16_t fid, score;
  p = finger.searchDatabase(&fid, &score);

  /* now wait to remove the finger, though not necessary;
     this was moved here after the search because of the R503 sensor,
     which seems to wipe its buffers after each scan */
  Serial.println("[SEARCH] QUITE EL DEDO");
  while (finger.getImage() != FPM_NOFINGER) {
    delay(500);
  }
  Serial.println();

  if (p == FPM_OK) {
    Serial.println("[SEARCH] HUELLA CORRECTA");
  } else {
    Serial.println("[SEARCH] ¡¡ERROR!!");
    errorFinger();
    return p;
  }

  // found a match!
  Serial.print("[SEARCH] ID#"); Serial.print(fid);
  Serial.print("[SEARCH] CONTENIDO DE LIST: " + idAlus[fid - 1]);
  sendHTTP(idAlus[fid - 1]);
  digitalWrite (ledR, LOW);
  digitalWrite (ledV, LOW);
  // hacer el insert

}
void errorFinger() {
  digitalWrite (ledV, LOW);
  digitalWrite (ledR, HIGH);
  delay(800);
  digitalWrite (ledR, LOW);
}
void sendHTTP(int idAlumno) {
  if ((wifiMulti.run() == WL_CONNECTED)) {
    HTTPClient http;
    Serial.println("[SEND] ENVIANDO DATOS A INSERTAR_ALUMNO");
    String url = "http://";
    url += DIR ;
    url += "/scripts/updateFalta.php?idAlumno=";
    url += idAlumno;
    Serial.println("[URL]" + url);
    http.begin(url); //HTTP

    int httpCode = http.GET();
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.println("[SEND] DATOS ENVIADOS");
    } else {
      Serial.println("[SEND] ¡¡ERROR!!");
    }
    http.end();
  } else {
    sendHTTP(idAlumno);
  }
}
