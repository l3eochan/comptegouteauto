//============Configuration du senior el system============
unsigned long set_moist = 2; //Réglage de l'humidité du sol requise par la plante remplacer les X par la valeur
bool set_night = 1; //paramètre de prise en compte de la nuit, true si la nuit est prise en compte
//=========================================================
bool night=0; //il fait nuit ou pas?, true si il fait nuit 
unsigned long press=0; //valeur de la pression atmo
unsigned long press_save=0; //Sauvegarde de la pression pour la prise d'info initiale (avant les 3h)
bool wat_lvl_crit=0; //valeur booléenne, true si niveau de l'eau est faible
unsigned long temps=0; //timer (max 49d)
unsigned long temps_save=0; //Sauvegarde de l'état précédent du chrono pour faire un timer
int br_nuit = 13; //broche détect. nuit
int br_lvl_eau = 12; //broche détect. lvl eau
int br_soil_moist = 11; //broche détect. soil moist 
int br_press = 10; //broche détect. de la press atmo
int br_start_water = 2; //broche électrovanne
int br_led_wat_low = 4;
enum {read_sensors, check_rain, begin_water, low_water}state;

void setup(){
  Serial.begin(9600);
  Serial.print("Initialized serial link... ");
//=======Init pins=======
  pinMode(br_start_water, OUTPUT);
  pinMode(br_nuit, INPUT_PULLUP);
  pinMode(br_lvl_eau, INPUT_PULLUP);
  pinMode(br_soil_moist, INPUT_PULLUP);
  pinMode(br_press, INPUT_PULLUP);
  pinMode(br_led_wat_low, OUTPUT);
  digitalWrite(br_start_water, LOW);
  digitalWrite(br_led_wat_low, LOW);
  Serial.print("Initialized pins... ");
//=======Init var=======
  temps = millis();
  temps_save = 0;
  night = 0;
  wat_lvl_crit = 0;
  press_save = digitalRead(br_press);
  state=read_sensors;
  Serial.print("Initialized variables... ");
}

void loop() {
  switch(state){
    case read_sensors:
      digitalWrite(br_start_water, LOW); //couper l'arrosage
      night = digitalRead(br_nuit); //a remplacer par un script photo résistance (avec quantum comme potentiomètre)
      if (digitalRead(br_lvl_eau) == LOW) { //s'assurer que le niveau d'eau est correct 
        Serial.println("Low water ! Switching state to low_water");
        state = low_water;
        break;
      }
      if (digitalRead(br_nuit)==LOW) {
        Serial.println("Night is true");
        // if (digitalRead(br_soil_moist) < set_moist) { //vérifier que la terre est moins humide que défini par l'utilisateur
        if (digitalRead(br_soil_moist) == LOW) {
          Serial.println("Soil is dry ! Switching state to check_rain");
          state = check_rain;
        } 
      } else {
          Serial.println("Nothing detected, Switching state to read_sensors");
          state=read_sensors;
        }
      Serial.println("Case read_sensors successfully executed using following settings: ");
      Serial.print("br_nuit: ");
      Serial.println(digitalRead(br_nuit));
      Serial.print("br_lvl_eau: ");        
      Serial.println(digitalRead(br_lvl_eau));
      Serial.print("br_soil_moist: ");
      Serial.println(digitalRead(br_soil_moist));
      Serial.println("_________");
      delay(1000);
      break;


    case check_rain:
      if (digitalRead(br_press)==LOW) { 
        Serial.println("no pressure drop detected ! switching state to begin_water");
        state = begin_water;
    } else{
        Serial.println("pressure drop detected ! switching state to read_sensors");
        state = read_sensors;
      }
      Serial.println("_________");
      break;


    case begin_water:
      digitalWrite(br_start_water, HIGH);
      Serial.println("Turned on water");
      delay(10000); //timer de 30 min (10s pour les tests) à remplacer par millis pour vérification continue du niveau d'eau
      digitalWrite(br_start_water, LOW); //couper l'arrosage
      Serial.println("Turned off water");
      Serial.println("Case read_sensors successfully executed, switching state to read_sensors ");
      Serial.println("_________");
      state=read_sensors;
      break;

    case low_water:
      while (digitalRead(br_lvl_eau) == LOW) {
        digitalWrite(br_led_wat_low, HIGH);
      }
      Serial.println("Water level is good");  
      Serial.println("Case low_water successfully executed switching state to read_sensors");
      Serial.println("_________");  
      digitalWrite(br_led_wat_low, LOW);  
      state = read_sensors;
      break;
  }
}