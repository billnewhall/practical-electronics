/* Tachometer

  Calculates RPM using inputs from a propeller blade-crossing sensor.

  W. Newhall 5/2023 (original)
*/

#ifndef __TACHOMETER_H__
#define __TACHOMETER_H__

#ifndef BLADE_PER_NUM_AVG
  #define BLADE_PER_NUM_AVG   12
#endif
#ifndef MAX_BLADE_TIME
  #define MAX_BLADE_TIME   500000
#endif

struct PropSpeed {
  float rpm;                // Prop rotational speed (rpm)    
  float blade_period_us;    // Average blade period (us)
  float blade_freq_hz;      // Blade crossings per second
};

class Tachometer {
  public:
    Tachometer(int, int);
    float GetAvgBladePeriod(void);
    float GetFreq(void);
    float GetRpm(void);
    PropSpeed GetPropSpeed(void);
    int num_prop_blades_;
    int blade_sensor_pin_;
    unsigned long max_blade_time_us_;
  private:
    float rpm_;
    float avg_blade_period_us_;
    float blade_freq_hz_;
};

#endif // __TACHOMETER_H__