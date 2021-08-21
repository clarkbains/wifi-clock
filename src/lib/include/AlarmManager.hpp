#pragma once
#include "time.h"
#include "../../utilities/include/DelayedEvent.h"
#define AlarmMaskDOW(dow, hh, mm) (uint32)((((hh)*60) + (mm)) + ((uint32)(dow) << 12))
#define AlarmMask(hh, mm) (AlarmMaskDOW((0b1111111), (hh), (mm)))

enum DaysOfWeek
{
    SATURDAY = 1 << 6,
    FRIDAY = 1 << 5,
    THURSDAY = 1 << 4,
    WEDNESDAY = 1 << 3,
    TUESDAY = 1 << 2,
    MONDAY = 1 << 1,
    SUNDAY = 1 << 0
};

class WakeAlarm
{
public:
    WakeAlarm(uint8 dow, uint8 h, uint8 m, char *n = "ALRM") : d(5000)
    {
        d.start();
        d.setTriggerOnNextInvocation(true);
        mask = AlarmMaskDOW(dow, h, m);
        snprintf(name, sizeof(name), "%s", n);
        Serial.printf("Added %s alarm for %d:%d, dmask: %d TOD: %s\n", name, h, m, dow, getTOD());
    };

    const char *getName()
    {
        return name;
    }
    char *getTOD()
    {
        snprintf(TOD, sizeof(TOD), "%d:%d", getHour(), getMin());
        return TOD;
    }

    uint8 getHour()
    {
        uint16 mins = mask & 0xFFF;
        return mins / 60;
    }

    uint8 getMin()
    {
        uint16 mins = mask & 0xFFF;
        return mins % 60;
    }

    uint8 changeMin(int8 delta)
    {
        int16_t hrs = getHour();
        int16_t m = getMin();

        //Clear Time from mask
        mask -= mask & 0xFFF;
        m += delta;
        m = min(max(m, (int16_t)0), (int16_t)60);
        mask += (hrs * 60) + m;
    }

    uint8 changeHr(int8 delta)
    {
        int16_t h = getHour();
        int16_t m = getMin();

        //Clear Time from mask
        mask -= mask & 0xFFF;
        h += delta;
        h = min(max(h, (int16_t)0), (int16_t)23);
        mask += (h * 60) + m;
    }

    static uint8 weekDays()
    {
        return MONDAY + TUESDAY + WEDNESDAY + THURSDAY + FRIDAY;
    }
    static uint8 weekends()
    {
        return SATURDAY + SUNDAY;
    }
    void dump()
    {
        tm* current = TimeManager::now();
        uint32 currentMask = AlarmMaskDOW(
            1 << current->tm_wday,
            current->tm_hour,
            current->tm_min);

        Serial.printf("DOW: %s, Time: %s, Mask: %lu Now %lu \n", getDOW(), getTOD(),mask, currentMask);
    }
    char *getDOW()
    {
        int16_t dowMask = mask >> 12;
        uint8 count = 0;
        for (uint8 i = 0; i < 7; i++)
        {
            if (dowMask - SATURDAY >= 0)
            {
                DOW[count++] = 'S';
                dowMask -= SATURDAY;
            }
            else if (dowMask - FRIDAY >= 0)
            {
                DOW[count++] = 'F';
                dowMask -= FRIDAY;
            }
            else if (dowMask - THURSDAY >= 0)
            {
                DOW[count++] = 'R';
                dowMask -= THURSDAY;
            }
            else if (dowMask - WEDNESDAY >= 0)
            {
                DOW[count++] = 'W';
                dowMask -= WEDNESDAY;
            }
            else if (dowMask - TUESDAY >= 0)
            {
                DOW[count++] = 'T';
                dowMask -= TUESDAY;
            }
            else if (dowMask - MONDAY >= 0)
            {
                DOW[count++] = 'M';
                dowMask -= MONDAY;
            }
            else if (dowMask - SUNDAY >= 0)
            {
                DOW[count++] = 'S';
                dowMask -= SUNDAY;
            }
        }
        DOW[count] = '\0';
        return DOW;
    }

    bool isActive(tm *current)
    {
        if (!d.shouldRun())
            return false;
        
        uint32 currentMask = AlarmMaskDOW(
            1 << current->tm_wday,
            current->tm_hour,
            current->tm_min);

        bool dowMatches = ((currentMask & mask) >> 12) != 0;
        uint32 timeMask = (1 << 12) - 1;
        //  Serial.~HardwareSerial.printf("Current: %lu, Alarm: %lu\n", currentMask, mask);
        bool todMatches = (currentMask & timeMask) == (mask & timeMask);
        if (todMatches && dowMatches)
        {
            d.start();
            return true;
        }
        return false;
    };
    uint32 getMask() { return mask; };

private:
    uint32 mask;
    char name[10];
    char TOD[8];
    char DOW[8];
    DelayedEvent d;
};
class AlarmManagerInstance;

class AlarmManager
{
    friend AlarmManagerInstance;

public:
    const static uint8 MAX_ALARMS = 10;

    void update()
    {
        for (int i = 0; i < numAlarms; i++)
        {
          //  Serial.printf("Checking alarm set at %s\n", alarms[i]->getTOD());
            if (alarms[i]->isActive(TimeManager::now()))
            {
                activeAlarms[i] = alarms[i];
            }
        }
        Serial.println();
    }

    void dump() 
    {
        for (int i = 0; i < numAlarms; i++)
        {
            alarms[i]->dump();
        }
    }
    WakeAlarm *getActiveAlarm()
    {
        for (int i = 0; i < MAX_ALARMS; i++)
        {
            if (activeAlarms[i] != NULL)
            {
                WakeAlarm *retVal = activeAlarms[i];
                activeAlarms[i] = NULL;
                return retVal;
            }
        }
        return NULL;
    }

    void addAlarm(WakeAlarm *a)
    {
        if (MAX_ALARMS == numAlarms)
            return;
        alarms[numAlarms++] = a;
    }

    uint8 getNumAlarms()
    {
        return numAlarms;
    }

    WakeAlarm *getWakeAlarm(uint8 i)
    {
        if (i >= numAlarms)
            return NULL;
        return alarms[i];
    }

private:
    AlarmManager() : numAlarms(0)
    {
        for (uint8 i = 0; i < MAX_ALARMS; i++)
        {
            activeAlarms[i] = alarms[i] = NULL;
        }
    };
    WakeAlarm *activeAlarms[MAX_ALARMS];
    WakeAlarm *alarms[MAX_ALARMS];
    uint8 numAlarms;
};

class AlarmManagerInstance
{
public:
    static AlarmManager &instance()
    {
        return am;
    }

private:
    static AlarmManager am;
};