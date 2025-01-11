#ifndef RTC_H
#define RTC_H

#include "mbed.h"

/**
* Library to get access to the interrupt functionality of the LPC1768's RTC.
*
* This class is completely static: which means you don't have to create an RTC object,
* there is always one object automatically created when you include this class. Since 
* there is only one RTC, more than one would make no sense.
*
* @code
* #include "mbed.h"
* #include "RTC.h"
*
* DigitalOut led(LED1);
*
* void ledFunction( void )
* {
*     led = 1;
*     RTC::detach(RTC::Second);
* }
* 
* void displayFunction( void )
* {
*     time_t seconds = time(NULL);
*     printf("%s", ctime(&seconds));
* }
* 
* void alarmFunction( void )
* {
*     error("Not most useful alarm function");
* }
* 
* int main()
* {
*     set_time(1256729737); // Set time to Wed, 28 Oct 2009 11:35:37
* 
*     tm t = RTC::getDefaultTM();
*     t.tm_sec = 5;
*     t.tm_min = 36;
* 
*     RTC::alarm(&alarmFunction, t);
*     RTC::attach(&displayFunction, RTC::Second);
*     RTC::attach(&ledFunction, RTC::Minute);
* 
*     while(1);
* }
* @endcode
**/
class RTC {
public:
    /**
    * Available time units for interrupts
    *
    * RTC::Second, RTC::Minute, RTC::Hour,
    * RTC::Day, RTC::Month, RTC::Year
    */
    enum TimeUnit {Second, Minute, Hour, 
                    Day, Month, Year};
                    
    /**
    * Call a function when the specified time unit increases
    *
    * You can attach one function for each TimeUnit. When several are
    * attached the smalles TimeUnit is called first.
    *
    * Member functions of a class can be attached the normal way (similar to for example Ticker).
    *
    * @param function - the function to call
    * @param interval - the TimeUnit which specifies the interval
    */
    static void attach(void (*function)(void), TimeUnit interval);
    template<typename T>
        void attach(T *object, void (T::*member)(void), TimeUnit interval);
    
    /**
    * Detach an interrupt function
    *
    * @param interval - the TimeUnit of the interrupt to detach
    */
    static void detach(TimeUnit interval);
    
    /**
    * Call a function when a specified time is reached
    *
    * Only one alarm is possible. Make fields of the tm structure -1 for don't care.
    *
    * Member functions of a class can be attached the normal way (similar to for example Ticker).
    *
    * @param function - the function to call
    * @param alarmTime - tm structure which specifies when to activate the alarm
    */
    static void alarm(void (*function)(void), tm alarmTime);
    template<typename T>
        void alarm(T *object, void (T::*member)(void), tm alarmTime);
    
    /**
    * Disable the alarm
    */
    static void alarmOff( void );
    
    /**
    * Returns a default tm structure where each field is initialized
    * to -1, so it is ignored by the alarm function.
    *
    * Available fields: http://www.cplusplus.com/reference/ctime/tm/
    * Except tm_isdst all of them can be used for the alarm
    *
    * @param return - tm structure initialized to -1
    */
    static tm getDefaultTM( void );
    
    

private:
    static void IRQHandler( void );

    static FunctionPointer attachCB[6];
    static FunctionPointer alarmCB;
    
    //If someone knows a nicer way to do this, please tell me
    static bool initialRun;
    static void _attach(TimeUnit interval);
    static void _alarm(tm alarmTime);


};

#endif
