
#define HARD_REV_FIXED    1
#define HARD_REV_COOLANT  2
#define HARD_REV_OIL  3

byte checkEngineProtect(void);
byte checkRevLimit(void);
byte checkBoostLimit(void);
byte checkOilPressureLimit(void);
byte checkAFRLimit(void);