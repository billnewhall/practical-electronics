#ifndef __TACHOMETER_H__
#define __TACHOMETER_H__

#ifndef BLADE_PER_NUM_AVG
  #define BLADE_PER_NUM_AVG   12
#endif
#ifndef MAX_BLADE_TIME
  #define MAX_BLADE_TIME   500000
#endif

class Tachometer {
  public:
    Tachometer(int, int);
    float GetAvgBladePeriod(void);
    float GetFreq(void);
    float GetRpm(void);
    int num_prop_blades_;
    int blade_sensor_pin_;
    unsigned long max_blade_time_us_;
  private:
};

#endif // __TACHOMETER_H__