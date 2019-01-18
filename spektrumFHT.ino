#define LOG_OUT 1 // use the log output function
#define FHT_N 256         // ширина спектра 
#define LOG_OUT 1

#include <FHT.h>


#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

#define AUDIO_IN 0
double prevVolts = 100.0;


// Настройки 

#define INPUT_GAIN 1.0    // коэффициент усиления входного сигнала
#define LOW_PASS 50       // нижний порог чувствительности шумов (нет скачков при отсутствии звука)
#define MAX_COEF 1.1      // коэффициент, который делает "максимальные" пики чуть меньше максимума, для более приятного восприятия
#define NORMALIZE 0       // нормализовать пики (столбики низких и высоких частот будут одинаковой длины при одинаковой громкости) (1 вкл, 0 выкл)
#define INPUT_GAIN 1.5    // коэффициент усиления входного сигнала
#define LOW_PASS 30       // нижний порог чувствительности шумов (нет скачков при отсутствии звука)
#define MAX_COEF 1.1      // коэффициент, который делает "максимальные" пики чуть меньше максимума, для более приятного восприятия
#define NORMALIZE 0       // нормализовать пики (столбики низких и высоких частот будут одинаковой длины при одинаковой громкости) (1 вкл, 0 выкл)
#define DEF_GAIN 50       // максимальный порог по умолчанию (при MANUAL_GAIN или AUTO_GAIN игнорируется)




byte posOffset[17] = {2, 3, 4, 6, 8, 10, 12, 14, 16, 20, 25, 30, 35, 60, 80, 100, 120};
int gain = DEF_GAIN;   // усиление по умолчанию


void setup() {
  Serial.begin(115200);

  sbi(ADCSRA, ADPS2);
  cbi(ADCSRA, ADPS1);
  sbi(ADCSRA, ADPS0);

  analogReference(EXTERNAL);
}

void loop() {
    char c;
    if (Serial.available() > 0 ) 
    {
      while (Serial.available() > 0 ) 
        c = Serial.read();
      String b;  
      analyzeAudio();   // функция FHT, забивает массив fht_log_out[] величинами по спектру
      for (int i = 0; i < 128; i ++) {
        // вот здесь сразу фильтруем весь спектр по минимальному LOW_PASS
        if (fht_log_out[i] < LOW_PASS) fht_log_out[i] = 0;
  
        // усиляем сигнал
//        fht_log_out[i] = (float)fht_log_out[i] * INPUT_GAIN;
  
        // уменьшаем громкость высоких частот (пропорционально частоте) если включено
        if (NORMALIZE) fht_log_out[i] = (float)fht_log_out[i] / ((float)1 + (float)i / 128);
        b = String((byte)fht_log_out[i], HEX);
        Serial.print(b);
      }
      Serial.println();
    }
    delay(100);
}


void analyzeAudio() {
  for (int i = 0 ; i < FHT_N ; i++) {
    int sample = analogRead(AUDIO_IN);
    fht_input[i] = sample/4; // put real data into bins
  }
  fht_window();  // window the data for better frequency response
  fht_reorder(); // reorder the data before doing the fht
  fht_run();     // process the data in the fht
  fht_mag_log(); // take the output of the fht
}


