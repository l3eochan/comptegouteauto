    case check_rain:
      if (digitalRead(br_press)==HIGH) { //décompte des 3h
        Serial.println("pressure drop detected ! switching state to begin_water");
        state = begin_water;
    } else{
        Serial.println("no pressure drop detected ! switching state to read_sensors");
        state = read_sensors;
      }
      Serial.println("_________");
      break;