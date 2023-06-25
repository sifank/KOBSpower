/*
    KOBSpower Controller

    Copyright (C) 2022 Sifan Kahale (sifan.kahale@gmail.com)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

*/
#include <defaultdevice.h>
#include <indipropertytext.h>
#include <indipropertyswitch.h>
#include <indipropertynumber.h>
#include <pi-gpio.h>
#include "config.h"

using namespace std;

class KOBSpwr : public INDI::DefaultDevice
{
public:    
    KOBSpwr();
    virtual ~KOBSpwr() = default;
    virtual bool initProperties() override;
    virtual const char *getDefaultName() override;
    virtual bool ISNewSwitch(const char *dev, const char *name, ISState *states, char *names[], int n) override;
    virtual bool ISNewText(const char *dev, const char *name, char *texts[], char *names[], int n) override;
    virtual bool ISNewNumber(const char *dev, const char *name, double values[], char *names[], int n) override;
    virtual void ISGetProperties(const char *dev) override;
    virtual bool updateProperties() override;
    virtual void TimerHit() override;
    
protected:
    virtual bool saveConfigItems(FILE *fp) override;

private:
    virtual bool   Connect() override;
    virtual bool   Disconnect() override;

    bool testConfig();
    bool switchStatus(int port, bool &sStatus);
    bool switchControl(int port, int control);
    bool setPWM(int channel, int power);
    bool getPwr();

    enum {
        PowerScript,
        Script_N,
    };
    INDI::PropertyText ScriptsTP {POWER_N};

    // Ports
    enum
    {
        DON,
        DOFF,
        DCntl_N,
    };
    INDI::PropertySwitch PortControl1SP {DCntl_N};
    INDI::PropertySwitch PortControl2SP {DCntl_N};
    INDI::PropertySwitch PortControl3SP {DCntl_N};
    INDI::PropertySwitch PortControl4SP {DCntl_N};
    INDI::PropertySwitch AllSP {DCntl_N};

    // Power
    enum
    {
        OUT1,
        OUT2,
        OUT3,
        OUT4,
        POWER_N,
    };
    INDI::PropertyText PortLabelsTP {POWER_N};
    INDI::PropertyNumber PortGpioNP {POWER_N};

    // Dew

    INDI::PropertyNumber DewCtl1NP{1};
    INDI::PropertyNumber DewCtl2NP{1};

    enum
    {
        DEW1,
        DEW2,
        DEW_N,
    };
    INDI::PropertyText DewLabelsTP {DEW_N};

    enum
    {
        PIN,
        MA1,
        MA2,
        MB1,
        MB2,
        PWMb,
        PWMa,
        DGPIO_N,
    };
    INDI::PropertyNumber DewGpioNP {DGPIO_N};

    // Power Usage
    enum
    {
        IN_VOLTS,
        PORT_CURRENT,
        DEW_CURRENT,
        SENSOR_N,
    };
    INDI::PropertyNumber PowerSensorsNP {SENSOR_N};

    // GPIO Pin assignments: Porta, b, c, d, Dew1, 2
    //int PINS[6] = {8, 11, 25, 24, 12, 26};

    // PWM values for RPI4
    int DIVIDER = 45;
    int RANGE = 100;

    char portLabel[MAXINDILABEL];
    bool portChanged=false;
    bool LastParseSuccess = true;
    size_t byte_count;
    char buf[BUFSIZ];
    int rc;
    int GPINS[4];
    int DPINS[7];
    float dewVolts, dewCurrent, dewPower, portVolts, portCurrent, portPower;

    static constexpr const char *CONFIG_TAB {"Config"};
};

