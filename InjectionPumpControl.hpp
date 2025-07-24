#pragma once

#include <Arduino.h>

enum class RunState : uint8_t
{
    ON,
    PAUSING,
    OFF,
    MAXTIMEEXCEEDED
};

// Pumpe hat 1,5l/h
class InjectionPumpControl
{
public:
    InjectionPumpControl(uint8_t pumpPin, TimeSpan pumpCycleRunTime, TimeSpan pumpCyclePauseTime,
                         TimeSpan pumpMaxRuntime) : runState(RunState::OFF)
    {
        this->pumpPin = pumpPin;
        this->pumpCycleRunTime = pumpCycleRunTime;
        this->pumpCyclePauseTime = pumpCyclePauseTime;
        this->pumpMaxRuntime = pumpMaxRuntime;
        this->totalPumpRuntime = TimeSpan(0);
        this->currentCycleRuntime = TimeSpan(0);
    }

    InjectionPumpControl() = delete;

    void init(uint8_t pumpPin, TimeSpan pumpCycleRunTime, TimeSpan pumpCyclePauseTime,
              TimeSpan pumpMaxRuntime)
    {
        runState = RunState::OFF;
        this->pumpPin = pumpPin;
        this->pumpCycleRunTime = pumpCycleRunTime;
        this->pumpCyclePauseTime = pumpCyclePauseTime;
        this->pumpMaxRuntime = pumpMaxRuntime;
        this->totalPumpRuntime = TimeSpan(0);
        this->currentCycleRuntime = TimeSpan(0);
    }

    void on()
    {
        // we do not inject if the water Pump is not running
        if (PoolControlContext::instance()->data.waterPumpState == false ||
            PoolControlContext::instance()->data.waterFlowSwitch == false ||
            PoolControlContext::instance()->data.error)
        {
            hasTodayAlreadySwitchedOn = false;
            runState = RunState::OFF; // also to get back from MAXTIMEEXCEEDED
            switchPump(LOW);
            return;
        }
        // we do not inject when the water pump has not yet run for some time
        DateTime elapsedAt = PoolControlContext::instance()->data.waterPumpRunningSince + PoolControlContext::instance()->config.waterPumpRuntimeBeforeInjection;
        if (PoolControlContext::instance()->data.date < elapsedAt)
        {
            switchPump(LOW);
            return;
        }

        if (!hasTodayAlreadySwitchedOn)
        {
            firstSwitchOnTime = PoolControlContext::instance()->data.date;
            hasTodayAlreadySwitchedOn = true;
            // Reset daily runtime tracking when pump starts for the first time today
            totalPumpRuntime = TimeSpan(0);
            currentCycleRuntime = TimeSpan(0);
        }

        switch (runState)
        {
        case RunState::ON:
        {
            switchPump(HIGH);
            // Update current cycle runtime
            updateCurrentCycleRuntime();

            // did it exceed the daily runtime
            if (maxRunTimeExceeded())
            {
                runState = RunState::MAXTIMEEXCEEDED;
                return;
            }
            // do we have to pause
            if (cycleRunTimeExceed())
            {
                // Add current cycle runtime to total before pausing
                totalPumpRuntime = totalPumpRuntime + currentCycleRuntime;
                pausedAt = PoolControlContext::instance()->data.date;
                runState = RunState::PAUSING;
                return;
            }
            break;
        }
        case RunState::OFF:
        {
            switchPump(LOW);
            currentCycleStartedAt = PoolControlContext::instance()->data.date;
            currentCycleRuntime = TimeSpan(0);
            runState = RunState::ON;
            break;
        }
        case RunState::PAUSING:
        {
            // pause elapsed go back to on
            switchPump(LOW);
            if (cyclePauseTimeExceed())
            {
                currentCycleStartedAt = PoolControlContext::instance()->data.date;
                currentCycleRuntime = TimeSpan(0);
                runState = RunState::ON;
            }
            break;
        }
        case RunState::MAXTIMEEXCEEDED:
        {
            switchPump(LOW);
            break;
        }
        default:
        {
            break;
        }
        }
    }

    void off()
    {
        // If pump was running, add current cycle runtime to total
        if (running && runState == RunState::ON)
        {
            updateCurrentCycleRuntime();
            totalPumpRuntime = totalPumpRuntime + currentCycleRuntime;
        }

        if (PoolControlContext::instance()->data.waterPumpState == false &&
            PoolControlContext::instance()->data.waterFlowSwitch == false) // TODO: Potential bug here, if the flow switch override has not been set to off after rincing
        {
            hasTodayAlreadySwitchedOn = false;
        }
        runState = RunState::OFF;
        switchPump(LOW);
    }

    void maintain()
    {
        if (PoolControlContext::instance()->data.waterPumpState == false &&
            PoolControlContext::instance()->data.waterFlowSwitch == false)
        {
            hasTodayAlreadySwitchedOn = false;
            // Reset runtime tracking when water pump stops
            totalPumpRuntime = TimeSpan(0);
            currentCycleRuntime = TimeSpan(0);
        }
    }

    bool isOn()
    {
        return running;
    }

private:
    uint8_t pumpPin;
    bool running{false};
    bool hasTodayAlreadySwitchedOn{false};
    DateTime firstSwitchOnTime;
    DateTime currentCycleStartedAt;
    DateTime pausedAt;
    RunState runState;

    // Track cumulative actual pump runtime
    TimeSpan totalPumpRuntime;
    TimeSpan currentCycleRuntime;

    TimeSpan pumpCycleRunTime;
    TimeSpan pumpCyclePauseTime;
    TimeSpan pumpMaxRuntime;

    bool cycleRunTimeExceed()
    {
        // Check if current cycle runtime exceeds the cycle run time limit
        if (currentCycleRuntime.totalseconds() >= pumpCycleRunTime.totalseconds())
        {
            return true;
        }
        return false;
    }

    bool cyclePauseTimeExceed()
    {
        TimeSpan pauseTime = PoolControlContext::instance()->data.date - pausedAt;
        if (pauseTime.totalseconds() >= pumpCyclePauseTime.totalseconds())
        {
            return true;
        }
        return false;
    }

    bool maxRunTimeExceeded()
    {
        // Calculate total runtime including current cycle
        TimeSpan actualTotalRuntime = totalPumpRuntime;
        if (runState == RunState::ON)
        {
            updateCurrentCycleRuntime();
            actualTotalRuntime = totalPumpRuntime + currentCycleRuntime;
        }

        if (actualTotalRuntime.totalseconds() >= pumpMaxRuntime.totalseconds())
        {
            PoolControlContext::instance()->data.warning = true;
            PoolControlContext::instance()->data.warningText = "Max injection time exceeded.";
            RealTimeClock::getFullDateTimeString(PoolControlContext::instance()->data.date, PoolControlContext::instance()->data.warningTimestamp);
            return true;
        }
        return false;
    }

    void updateCurrentCycleRuntime()
    {
        if (runState == RunState::ON)
        {
            currentCycleRuntime = PoolControlContext::instance()->data.date - currentCycleStartedAt;
        }
    }

    void switchPump(uint8_t onOff)
    {
        digitalWrite(pumpPin, onOff);
        running = onOff == 0 ? false : true;
    }
};
