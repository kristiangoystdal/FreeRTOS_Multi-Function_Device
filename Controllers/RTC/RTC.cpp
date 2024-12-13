#include "RTC.h"

FunctionPointer RTC::attachCB[6];
FunctionPointer RTC::alarmCB;

bool RTC::initialRun = true;


void RTC::attach(void (*function)(void), TimeUnit interval)
{
    //Set the function pointer
    attachCB[interval].attach(function);
    _attach(interval);
}

template<typename T>
void RTC::attach(T *object, void (T::*member)(void), TimeUnit interval)
{
    //Set the function pointer
    attachCB[interval].attach(object, member);
    _attach(interval);
}


void RTC::_attach(TimeUnit interval)
{
    //Disable IRQs, dont want them to happen while busy here
    NVIC_DisableIRQ(RTC_IRQn);

    //Set the IRQ vector
    NVIC_SetVector(RTC_IRQn, (uint32_t)&RTC::IRQHandler);

    //If this is the first time it is called, delete all interrupt sources
    //We need to do this because RTC unit isnt affected by system resets apparently
    if (initialRun) {
        LPC_RTC->CIIR = 0;
        LPC_RTC->AMR = 255;
        initialRun = false;
        LPC_RTC->ILR = 0x03;
    }

    //Set/reset correct interrupt source
    switch (interval) {
        case Second:
            LPC_RTC->CIIR |= 1;
            break;
        case Minute:
            LPC_RTC->CIIR |= 2;
            break;
        case Hour:
            LPC_RTC->CIIR |= 4;
            break;
        case Day:
            LPC_RTC->CIIR |= 56;
            break;
        case Month:
            LPC_RTC->CIIR |= 64;
            break;
        case Year:
            LPC_RTC->CIIR |= 128;
            break;
    }


    //We can always enable IRQs, since if all IRQs are disabled by the user the RTC hardware will never raise its IRQ flag anyway
    NVIC_EnableIRQ(RTC_IRQn);
}

void RTC::detach(TimeUnit interval)
{
    switch (interval) {
        case Second:
            LPC_RTC->CIIR &= ~1;
            break;
        case Minute:
            LPC_RTC->CIIR &= ~2;
            break;
        case Hour:
            LPC_RTC->CIIR &= ~4;
            break;
        case Day:
            LPC_RTC->CIIR &= ~56;
            break;
        case Month:
            LPC_RTC->CIIR &= ~64;
            break;
        case Year:
            LPC_RTC->CIIR &= ~128;
            break;
    }
    attachCB[interval].attach(NULL);
}


void RTC::alarm(void (*function)(void), tm alarmTime)
{
    //Set the function pointer
    alarmCB.attach(function);
    _alarm(alarmTime);
}

template<typename T>
void RTC::alarm(T *object, void (T::*member)(void), tm alarmTime)
{
    //Set the function pointer
    alarmCB.attach(object, member);
    _alarm(alarmTime);
}


void RTC::_alarm(tm alarmTime)
{
    //Disable IRQs, dont want them to happen while busy here
    NVIC_DisableIRQ(RTC_IRQn);

    //Set the IRQ vector
    NVIC_SetVector(RTC_IRQn, (uint32_t)&RTC::IRQHandler);

    //If this is the first time it is called, delete all interrupt sources
    //We need to do this because RTC unit isnt affected by system resets apparently
    if (initialRun) {
        LPC_RTC->CIIR = 0;
        LPC_RTC->AMR = 255;
        initialRun = false;
        LPC_RTC->ILR = 0x03;
    }

    //Set the alarm register
    if ((alarmTime.tm_sec>=0) && (alarmTime.tm_sec<60)) {
        LPC_RTC->ALSEC = alarmTime.tm_sec;
        LPC_RTC->AMR &= ~1;
    } else
        LPC_RTC->AMR |= 1;

    if ((alarmTime.tm_min>=0) && (alarmTime.tm_min<60)) {
        LPC_RTC->ALMIN = alarmTime.tm_min;
        LPC_RTC->AMR &= ~2;
    } else
        LPC_RTC->AMR |= 2;

    if ((alarmTime.tm_hour>=0) && (alarmTime.tm_hour<24)) {
        LPC_RTC->ALHOUR = alarmTime.tm_hour;
        LPC_RTC->AMR &= ~4;
    } else
        LPC_RTC->AMR |= 4;

    if ((alarmTime.tm_mday>=1) && (alarmTime.tm_mday<32)) {
        LPC_RTC->ALDOM = alarmTime.tm_mday;
        LPC_RTC->AMR &= ~8;
    } else
        LPC_RTC->AMR |= 8;

    if ((alarmTime.tm_wday>=0) && (alarmTime.tm_wday<7)) {
        LPC_RTC->ALDOW = alarmTime.tm_wday;
        LPC_RTC->AMR &= ~16;
    } else
        LPC_RTC->AMR |= 16;

    if ((alarmTime.tm_yday>0) && (alarmTime.tm_yday<367)) {
        LPC_RTC->ALDOY = alarmTime.tm_yday;
        LPC_RTC->AMR &= ~32;
    } else
        LPC_RTC->AMR |= 32;

    if ((alarmTime.tm_mon>=0) && (alarmTime.tm_mon<12)) {
        LPC_RTC->ALMON = alarmTime.tm_mon + 1;   //Different definitions
        LPC_RTC->AMR &= ~64;
    } else
        LPC_RTC->AMR |= 64;

    if ((alarmTime.tm_year>=0) && (alarmTime.tm_year<1000)) {
        LPC_RTC->ALYEAR = alarmTime.tm_year + 1900;   //Different definitions
        LPC_RTC->AMR &= ~128;
    } else
        LPC_RTC->AMR |= 128;

    //DOY and DOW register normally not set
    time_t t = time(NULL);
    LPC_RTC->DOY = localtime(&t)->tm_yday+1;
    LPC_RTC->DOW = localtime(&t)->tm_wday;

    //We can always enable IRQs, since if all IRQs are disabled by the user the RTC hardware will never raise its IRQ flag anyway
    NVIC_EnableIRQ(RTC_IRQn);
}

void RTC::alarmOff( void )
{
    LPC_RTC->AMR = 255;
    alarmCB.attach(NULL);
}


void RTC::IRQHandler( void )
{
    if ((LPC_RTC->ILR & 0x01) == 0x01) {
        //Attach interrupt
        attachCB[0].call();

        //If seconds zero
        if (LPC_RTC->SEC == 0) {
            attachCB[1].call();

            //If minutes zero
            if (LPC_RTC->MIN == 0) {
                attachCB[2].call();

                //If hours zero
                if (LPC_RTC->HOUR == 0) {
                    attachCB[3].call();

                    //If days zero
                    if (LPC_RTC->DOM == 0) {
                        attachCB[4].call();

                        //If month zero
                        if (LPC_RTC->MONTH == 0)
                            attachCB[5].call();
                    }
                }
            }
        }
    }

    if ((LPC_RTC->ILR & 0x02) == 0x02)
        alarmCB.call();



    //Reset interrupt status
    LPC_RTC->ILR = 0x03;
}

tm RTC::getDefaultTM( void ) {
    struct tm t;
    t.tm_sec = -1;   
    t.tm_min = -1;    
    t.tm_hour = -1;   
    t.tm_mday = -1;
    t.tm_wday = -1;
    t.tm_yday = -1;   
    t.tm_mon = -1;    
    t.tm_year = -1; 
    
    return t;
    }
