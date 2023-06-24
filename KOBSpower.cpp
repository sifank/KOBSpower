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

    TODO:

*/

#include "KOBSpower.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#pragma GCC diagnostic ignored "-Wformat-truncation="

// declare an auto pointer to KOBSpwr
static std::unique_ptr<KOBSpwr> kpdriver(new KOBSpwr());

///////////////////////////////////////////////////////////////
const char *KOBSpwr::getDefaultName()
{
    return "KOBSpower";
}

///////////////////////////////////////////////////////////////
KOBSpwr::KOBSpwr()
{
    setVersion(KOBSpower_VERSION_MAJOR, KOBSpower_VERSION_MINOR);
}

///////////////////////////////////////////////////////////////
//  Connect
///////////////////////////////////////////////////////////////
bool KOBSpwr::Connect()
{
    SetTimer(POLLMS);

    // TODO add test for existence
    // TODO add ability to set and save default states

    // ATTENTION need to find proper place for this
    // setup port gpio array
    setup();
    for (int i=1; i==POWER_N; i++) {
        setup_gpio(int(PortGpioNP[i].getValue()), OUTPUT, 0);  // all off initially
    }

    DEBUG(INDI::Logger::DBG_SESSION, "KOBS power connected successfully.");
    return true;

}

///////////////////////////////////////////////////////////////
//  Disconnect
///////////////////////////////////////////////////////////////
bool KOBSpwr::Disconnect()
{
    //cleanup();
    DEBUG(INDI::Logger::DBG_SESSION, "KOBS power disconnected successfully.");
    return true;
}

///////////////////////////////////////////////////////////////
//  initProperties
///////////////////////////////////////////////////////////////
bool KOBSpwr::initProperties()
{
    // We init parent properties first
    INDI::DefaultDevice::initProperties();

    setDriverInterface(AUX_INTERFACE);
    addAuxControls();

    ////////////////
    //MAIN Tab
    ////////////////
    // Port 1
    bool sStatus = false;

    if(!KOBSpwr::switchStatus(OUT1, sStatus))
        DEBUG(INDI::Logger::DBG_ERROR, "Problem getting status");
    PortControl1SP[DON].fill("P1ON", "On", sStatus ? ISS_ON : ISS_OFF);
    PortControl1SP[DOFF].fill("P1OFF", "Off", sStatus ? ISS_OFF : ISS_ON);
    PortControl1SP.fill(getDeviceName(), "PORT_1", PortLabelsTP[OUT1].getText(), MAIN_CONTROL_TAB, IP_RW, ISR_ATMOST1, 60, IPS_OK);

    // Port 2
    if(!KOBSpwr::switchStatus(OUT2, sStatus))
        DEBUG(INDI::Logger::DBG_ERROR, "Problem getting status");
    PortControl2SP[DON].fill("P2ON", "On", sStatus ? ISS_ON : ISS_OFF);
    PortControl2SP[DOFF].fill("P2OFF", "Off", sStatus ? ISS_OFF : ISS_ON);
    PortControl2SP.fill(getDeviceName(), "PORT_2", PortLabelsTP[OUT2].getText(), MAIN_CONTROL_TAB, IP_RW, ISR_ATMOST1, 60, IPS_OK);

    // Port 3
    if(!KOBSpwr::switchStatus(OUT3, sStatus))
        DEBUG(INDI::Logger::DBG_ERROR, "Problem getting status");
    PortControl3SP[DON].fill("P3ON", "On", sStatus ? ISS_ON : ISS_OFF);
    PortControl3SP[DOFF].fill("P3OFF", "Off", sStatus ? ISS_OFF : ISS_ON);
    PortControl3SP.fill(getDeviceName(), "PORT_3", PortLabelsTP[OUT3].getText(), MAIN_CONTROL_TAB, IP_RW, ISR_ATMOST1, 60, IPS_OK);

    // Port 4
    if(!KOBSpwr::switchStatus(OUT4, sStatus))
        DEBUG(INDI::Logger::DBG_ERROR, "Problem getting status");
    PortControl4SP[DON].fill("P4ON", "On", sStatus ? ISS_ON : ISS_OFF);
    PortControl4SP[DOFF].fill("P4OFF", "Off", sStatus ? ISS_OFF : ISS_ON);
    PortControl4SP.fill(getDeviceName(), "PORT_4", PortLabelsTP[OUT4].getText(), MAIN_CONTROL_TAB, IP_RW, ISR_ATMOST1, 60, IPS_OK);

    // Dew Cntl
    // TODO retrieve current power setting and set below
    DewCtl1NP[0].fill("Dew0", "Dew %", "%i", 0, 100, 10, 0);
    DewCtl1NP.fill(getDeviceName(), "DewCtl1", DewLabelsTP[DEW1].getText(), MAIN_CONTROL_TAB, IP_RW, 60, IPS_OK);

    // TODO retrieve current power setting and set below
    DewCtl2NP[0].fill("Dew1", "Dew %", "%i", 0, 100, 10, 0);
    DewCtl2NP.fill(getDeviceName(), "DewCtl2", DewLabelsTP[DEW2].getText(), MAIN_CONTROL_TAB, IP_RW, 60, IPS_OK);

    // Power (A) displays
    PowerSensorsNP[IN_VOLTS].fill("INVOLTS", "Volts-in", "%.2f", 0, 100, 10, 0);
    PowerSensorsNP[PORT_CURRENT].fill("PortP", "Port Current", "%.2f", 0, 100, 10, 0);
    PowerSensorsNP[DEW_CURRENT].fill("DewP", "Dew Current", "%.2f", 0, 100, 10, 0);
    PowerSensorsNP.fill(getDeviceName(), "POWERSTAT", "Power", MAIN_CONTROL_TAB, IP_RO, 60, IPS_OK);

    // All On/Off/Profile
    AllSP[DON].fill("ALL_ON", "All On", ISS_OFF);
    AllSP[DOFF].fill("ALL_OFF", "All Off", ISS_OFF);
    AllSP.fill(getDeviceName(), "ALL", "All Ports", MAIN_CONTROL_TAB, IP_RW, ISR_ATMOST1, 60, IPS_OK);

    ////////////////
    //Options Tab
    ////////////////
    //Port Labels - must define before pulling out names below
    PortLabelsTP[OUT1].fill("PORTLABEL1", "Port 1", "Port 1");
    PortLabelsTP[OUT2].fill("PORTLABEL2", "Port 2", "Port 2");
    PortLabelsTP[OUT3].fill("PORTLABEL3", "Port 3", "Port 3");
    PortLabelsTP[OUT4].fill("PORTLABEL4", "Port 4", "Port 4");
    PortLabelsTP.fill(getDeviceName(), "PORTLABELS", "Labels", OPTIONS_TAB, IP_WO, 60, IPS_IDLE);

    DewLabelsTP[DEW1].fill("Dew1", "Dew 1", "Dew 1");
    DewLabelsTP[DEW2].fill("Dew2", "Dew 2", "Dew 2");
    DewLabelsTP.fill(getDeviceName(), "DEWLABELS", "Labels", OPTIONS_TAB, IP_WO, 60, IPS_IDLE);

    ////////////////
    // Config tabs
    ////////////////
    // Port GPIO pins
    PortGpioNP[OUT1].fill("PGPIO1", "Port A", "%i", 0, 26, 1, PortGpioNP[OUT1].getValue());
    PortGpioNP[OUT2].fill("PGPIO2", "Port B", "%i", 0, 26, 1, 11);
    PortGpioNP[OUT3].fill("PGPIO3", "Port C", "%i", 0, 26, 1, 25);
    PortGpioNP[OUT4].fill("PGPIO4", "Port D", "%i", 0, 26, 1, 24);
    PortGpioNP.fill(getDeviceName(), "PGPIO", "Power GPIO Pins", CONFIG_TAB, IP_RW, 60, IPS_OK);

    // Dew GPIO pins
    DewGpioNP[PIN].fill("DGPIN", "PIN", "%i", 0, 26, 1, 18);
    DewGpioNP[MA1].fill("DGMA1", "MA 1", "%i", 0, 26, 1, 20);
    DewGpioNP[MA2].fill("DGMA2", "MA 2", "%i", 0, 26, 1, 21);
    DewGpioNP[MB1].fill("DGMB1", "MB 1", "%i", 0, 26, 1, 6);
    DewGpioNP[MB2].fill("DGMB2", "MB 2", "%i", 0, 26, 1, 13);
    DewGpioNP[PWMb].fill("DGPWMA", "PWM A", "%i", 0, 26, 1, 12);
    DewGpioNP[PWMa].fill("DGPWMB", "PWM B", "%i", 0, 26, 1, 26);
    DewGpioNP.fill(getDeviceName(), "DEWIO", "DEW GPIO Pins", CONFIG_TAB, IP_RW, 60, IPS_OK);

    // python power script location
    ScriptsTP[PowerScript].fill("PWRSCP", "Path", "KOBScurrent.py");
    ScriptsTP.fill(getDeviceName(), "SCRPT", "Power Script", CONFIG_TAB, IP_RW, 60, IPS_OK);

    return true;
}

///////////////////////////////////////////////////////////////
//  updateProperties
///////////////////////////////////////////////////////////////
bool KOBSpwr::updateProperties()
{

    // Call parent update properties first
    INDI::DefaultDevice::updateProperties();

    if (isConnected()) {

        // paint the device names
        PortControl1SP.fill(getDeviceName(), "PORT_1", PortLabelsTP[OUT1].getText(), MAIN_CONTROL_TAB, IP_RW, ISR_ATMOST1, 60, IPS_OK);
        PortControl2SP.fill(getDeviceName(), "PORT_2", PortLabelsTP[OUT2].getText(), MAIN_CONTROL_TAB, IP_RW, ISR_ATMOST1, 60, IPS_OK);
        PortControl3SP.fill(getDeviceName(), "PORT_3", PortLabelsTP[OUT3].getText(), MAIN_CONTROL_TAB, IP_RW, ISR_ATMOST1, 60, IPS_OK);
        PortControl4SP.fill(getDeviceName(), "PORT_4", PortLabelsTP[OUT4].getText(), MAIN_CONTROL_TAB, IP_RW, ISR_ATMOST1, 60, IPS_OK);

        DewCtl1NP.fill(getDeviceName(), "DewCtl1", DewLabelsTP[DEW1].getText(), MAIN_CONTROL_TAB, IP_RW, 60, IPS_OK);
        DewCtl2NP.fill(getDeviceName(), "DewCtl2", DewLabelsTP[DEW2].getText(), MAIN_CONTROL_TAB, IP_RW, 60, IPS_OK);

        // We're connected, paint the tabs
        defineProperty(PortControl1SP);
        defineProperty(PortControl2SP);
        defineProperty(PortControl3SP);
        defineProperty(PortControl4SP);
        defineProperty(PortLabelsTP);
        defineProperty(DewLabelsTP);
        defineProperty(DewCtl1NP);
        defineProperty(DewCtl2NP);
        defineProperty(PowerSensorsNP);
        defineProperty(ScriptsTP);
        defineProperty(PortGpioNP);
        defineProperty(DewGpioNP);
        defineProperty(AllSP);

    }
    else
    {
        // We're disconnected
        deleteProperty(PortControl1SP.getName());
        deleteProperty(PortControl2SP.getName());
        deleteProperty(PortControl3SP.getName());
        deleteProperty(PortControl4SP.getName());
        deleteProperty(PowerSensorsNP.getName());
        deleteProperty(PortLabelsTP.getName());
        deleteProperty(DewLabelsTP.getName());
        deleteProperty(DewCtl1NP.getName());
        deleteProperty(DewCtl2NP.getName());
        deleteProperty(ScriptsTP.getName());
        deleteProperty(PortGpioNP.getName());
        deleteProperty(DewGpioNP.getName());
        deleteProperty(AllSP.getName());
    }
    return true;
}

///////////////////////////////////////////////////////////////
//  NewSwitch
///////////////////////////////////////////////////////////////
bool KOBSpwr::ISNewSwitch (const char *dev, const char *name, ISState *states, char *names[], int n)
{
    if (dev != nullptr && strcmp(dev, getDeviceName()) == 0)
    {
        // Control Ports
        portChanged = false;

        // Port 1
        if (PortControl1SP.isNameMatch(name))
        {
            PortControl1SP.update(states, names, n);
            if (PortControl1SP[DON].getState() == ISS_ON) {
                if (!KOBSpwr::switchControl(OUT1, DON))
                    DEBUG(INDI::Logger::DBG_ERROR, "Could not turn port 1 on");
            }
            else if (PortControl1SP[DOFF].getState() == ISS_ON) {
                if (!KOBSpwr::switchControl(OUT1, DOFF))
                    DEBUG(INDI::Logger::DBG_ERROR, "Could not turn port 1 off");
            }
            PortControl1SP.setState(IPS_OK);
            PortControl1SP.apply();
            portChanged = true;
        }

        // Port 2
        if (PortControl2SP.isNameMatch(name))
        {
            PortControl2SP.update(states, names, n);
            if (PortControl2SP[DON].getState() == ISS_ON) {
                if (!KOBSpwr::switchControl(OUT2, DON))
                    DEBUG(INDI::Logger::DBG_ERROR, "Could not turn port 2 on");
            }
            else if (PortControl2SP[DOFF].getState() == ISS_ON) {
                if (!KOBSpwr::switchControl(OUT2, DOFF))
                    DEBUG(INDI::Logger::DBG_ERROR, "Could not turn port 2 off");
            }
            PortControl2SP.setState(IPS_OK);
            PortControl2SP.apply();
            portChanged = true;
        }

        // Port 3
        if (PortControl3SP.isNameMatch(name))
        {
            PortControl3SP.update(states, names, n);
            if (PortControl3SP[DON].getState() == ISS_ON) {
                if (!KOBSpwr::switchControl(OUT3, DON))
                    DEBUG(INDI::Logger::DBG_ERROR, "Could not turn port 3 on");
            }
            else if (PortControl3SP[DOFF].getState() == ISS_ON) {
                if (!KOBSpwr::switchControl(OUT3, DOFF))
                    DEBUG(INDI::Logger::DBG_ERROR, "Could not turn port 3 off");
            }
            PortControl3SP.setState(IPS_OK);
            PortControl3SP.apply();
            portChanged = true;
        }

        // Port 4
        if (PortControl4SP.isNameMatch(name))
        {
            PortControl4SP.update(states, names, n);
            if (PortControl4SP[DON].getState() == ISS_ON) {
                if (!KOBSpwr::switchControl(OUT4, DON))
                    DEBUG(INDI::Logger::DBG_ERROR, "Could not turn port 4 on");
            }
            else if (PortControl4SP[DOFF].getState() == ISS_ON) {
                if (!KOBSpwr::switchControl(OUT4, DOFF))
                    DEBUG(INDI::Logger::DBG_ERROR, "Could not turn port 4 off");
            }
            PortControl4SP.setState(IPS_OK);
            PortControl4SP.apply();
            portChanged = true;
        }

        if (portChanged) {
            AllSP[DOFF].setState(ISS_OFF);
            AllSP[DON].setState(ISS_OFF);
            AllSP.setState(IPS_OK);
        }

        // All On
        if (AllSP.isNameMatch(name))
        {
            AllSP.update(states, names, n);
            
            // ALLON
            if(AllSP[DON].getState() == ISS_ON) {
                if (KOBSpwr::switchControl(OUT1, DON))
                    PortControl1SP[DON].setState(ISS_ON);
                else
                    DEBUG(INDI::Logger::DBG_ERROR, "Could not turn port 1 on");

                if (KOBSpwr::switchControl(OUT2, DON))
                    PortControl2SP[DON].setState(ISS_ON);
                else
                    DEBUG(INDI::Logger::DBG_ERROR, "Could not turn port 2 on");

                if (KOBSpwr::switchControl(OUT3, DON))
                    PortControl3SP[DON].setState(ISS_ON);
                else
                    DEBUG(INDI::Logger::DBG_ERROR, "Could not turn port 3 on");

                if (KOBSpwr::switchControl(OUT4, DON))
                    PortControl4SP[DON].setState(ISS_ON);
                else
                    DEBUG(INDI::Logger::DBG_ERROR, "Could not turn port 4 on");

                AllSP[DON].setState(ISS_ON);
                AllSP[DOFF].setState(ISS_OFF);
                AllSP.setState(IPS_OK);
                AllSP.apply();
            }
            
            // All Off
            else if(AllSP[DON].getState() == ISS_OFF) {
                if (KOBSpwr::switchControl(OUT1, DOFF))
                    PortControl1SP[DOFF].setState(ISS_ON);
                else
                    DEBUG(INDI::Logger::DBG_ERROR, "Could not turn port 1 off");

                if (KOBSpwr::switchControl(OUT2, DOFF))
                    PortControl2SP[DOFF].setState(ISS_ON);
                else
                    DEBUG(INDI::Logger::DBG_ERROR, "Could not turn port 2 off");

                if (KOBSpwr::switchControl(OUT3, DOFF))
                    PortControl3SP[DOFF].setState(ISS_ON);
                else
                    DEBUG(INDI::Logger::DBG_ERROR, "Could not turn port 3 off");

                if (KOBSpwr::switchControl(OUT4, DOFF))
                    PortControl4SP[DOFF].setState(ISS_ON);
                else
                    DEBUG(INDI::Logger::DBG_ERROR, "Could not turn port 4 off");
                
                AllSP[DOFF].setState(ISS_ON);
                AllSP[DON].setState(ISS_OFF);
                AllSP.setState(IPS_OK);
                AllSP.apply();
            }
        }
    }

    return INDI::DefaultDevice::ISNewSwitch (dev, name, states, names, n);
}

///////////////////////////////////////////////////////////////
//   NewText
///////////////////////////////////////////////////////////////
bool KOBSpwr::ISNewText (const char *dev, const char *name, char *texts[], char *names[], int n)
{
    if (dev != nullptr && strcmp(dev, getDeviceName()) == 0)
    {
        // Port names
        if (PortLabelsTP.isNameMatch(name))
        {
            PortLabelsTP.update(texts, names, n);

            //DEBUG(INDI::Logger::DBG_WARNING, "You will need to Options/Save then Disconnect/Connect for name(s) to appear on the Main Tab");

            PortLabelsTP.setState(IPS_OK);
            PortLabelsTP.apply();
        }

        // Dew names
        if (DewLabelsTP.isNameMatch(name))
        {
            DewLabelsTP.update(texts, names, n);

            //DEBUG(INDI::Logger::DBG_WARNING, "You will need to Options/Save then Disconnect/Connect for name(s) to appear on the Main Tab");

            DewLabelsTP.setState(IPS_OK);
            DewLabelsTP.apply();
        }

        // Power python script name
        if (ScriptsTP.isNameMatch(name))
        {
            ScriptsTP.update(texts, names, n);

            //DEBUG(INDI::Logger::DBG_WARNING, "You will need to Options/Save then Disconnect/Connect for name(s) to appear on the Main Tab");

            ScriptsTP.setState(IPS_OK);
            ScriptsTP.apply();
        }
    }
    return INDI::DefaultDevice::ISNewText (dev, name, texts, names, n);
}

//////////////////////////////////////////////////////////////////////////////
/// Number field updated
//////////////////////////////////////////////////////////////////////////////
bool KOBSpwr::ISNewNumber(const char *dev, const char *name, double values[], char *names[], int n)
{
    // TODO add dew
    if (dev != nullptr && strcmp(dev, getDeviceName()) == 0)
    {
        if(PortGpioNP.isNameMatch(name)) {
            PortGpioNP.update(values, names, n);
            //for (int i; i==POWER_N; i++) {
                //PortGpioNP[i].setValue(int(PortGpioNP[i].getValue()));
                //PortGpioNP[i].value = int(PortGpioNP[i].getValue());
                //LOGF_INFO("Out%i: %i", i, int(PortGpioNP[i].getValue()));
            //}
            //PortGpioNP[OUT1].getValue();
            //PortGpioNP[OUT2].getValue();
            //PortGpioNP[OUT3].getValue();
            //PortGpioNP[OUT4].getValue();
            //IDSetNumber(&PortGpioNP, nullptr);
            //PortGpioNP.setState(IPS_OK);
            PortGpioNP.apply();
        }

        if(DewGpioNP.isNameMatch(name)) {
            DewGpioNP.update(values, names, n);
            //for (int i; i==DGPIO_N; i++) {
                //DewGpioNP[i].setValue(int(DewGpioNP[i].getValue()));
                //DewGpioNP[i].value = int(DewGpioNP[i].getValue());
            //}
            DewGpioNP.apply();
        }

        if(DewCtl1NP.isNameMatch(name)) {
            DewCtl1NP.update(values, names, n);
            //int dew1per = int(DewCtl1NP[0].getValue());
            //DewCtl1NP[0].value = dew1per;
            //DewCtl1NP[0].setValue(dew1per);

            KOBSpwr::setPWM(PWMb, int(DewCtl1NP[0].getValue()));
            DewCtl1NP.apply();
        }

        if(DewCtl2NP.isNameMatch(name)) {
            DewCtl2NP.update(values, names, n);
            //DewCtl2NP[0].value = DewCtl2NP[0].getValue();
            KOBSpwr::setPWM(PWMa, int(DewCtl2NP[0].getValue()));
            DewCtl2NP.apply();
        }

    }
    return INDI::DefaultDevice::ISNewNumber(dev, name, values, names, n);
}

///////////////////////////////////////////////////////////////
//   Timer Hit
///////////////////////////////////////////////////////////////
void KOBSpwr::TimerHit()
{
    if(!isConnected())
        return;

    // update switch status
    bool sStatus = false;
    if(!KOBSpwr::switchStatus(OUT1, sStatus))
        DEBUG(INDI::Logger::DBG_ERROR, "Problem getting status");
    PortControl1SP[DON].setState(sStatus ? ISS_ON : ISS_OFF);
    PortControl1SP[DOFF].setState(sStatus ? ISS_OFF : ISS_ON);
    PortControl1SP.apply();

    if(!KOBSpwr::switchStatus(OUT2, sStatus))
        DEBUG(INDI::Logger::DBG_ERROR, "Problem getting status");
    PortControl2SP[DON].setState(sStatus ? ISS_ON : ISS_OFF);
    PortControl2SP[DOFF].setState(sStatus ? ISS_OFF : ISS_ON);
    PortControl2SP.apply();

    if(!KOBSpwr::switchStatus(OUT3, sStatus))
        DEBUG(INDI::Logger::DBG_ERROR, "Problem getting status");
    PortControl3SP[DON].setState(sStatus ? ISS_ON : ISS_OFF);
    PortControl3SP[DOFF].setState(sStatus ? ISS_OFF : ISS_ON);
    PortControl3SP.apply();

    if(!KOBSpwr::switchStatus(OUT4, sStatus))
        DEBUG(INDI::Logger::DBG_ERROR, "Problem getting status");
    PortControl4SP[DON].setState(sStatus ? ISS_ON : ISS_OFF);
    PortControl4SP[DOFF].setState(sStatus ? ISS_OFF : ISS_ON);
    PortControl4SP.apply();

    // TODO add dew

    // update the current values
    getPwr();
    PowerSensorsNP[IN_VOLTS].setValue((dewVolts + portVolts) /2 );
    PowerSensorsNP[PORT_CURRENT].setValue(portCurrent);
    PowerSensorsNP[DEW_CURRENT].setValue(dewCurrent);
    PowerSensorsNP.apply();

    // and tell INDI to do this again ...
    SetTimer(POLLMS);
    return;
}

///////////////////////////////////////////////////////////////
// GetProperties
///////////////////////////////////////////////////////////////
void KOBSpwr::ISGetProperties(const char *dev)
{
    INDI::DefaultDevice::ISGetProperties(dev);
    // TODO do we really have to defineProperty here?
    defineProperty(PortLabelsTP);
    defineProperty(DewLabelsTP);

    loadConfig(true, PortLabelsTP.getName());
    loadConfig(true, DewLabelsTP.getName());
    loadConfig(true, PortGpioNP.getName());
    loadConfig(true, DewGpioNP.getName());
    loadConfig(true, ScriptsTP.getName());
}

///////////////////////////////////////////////////////////////
// saveConfigItems
///////////////////////////////////////////////////////////////
bool KOBSpwr::saveConfigItems(FILE *fp)
{
    IUSaveConfigText(fp, PortLabelsTP);
    IUSaveConfigText(fp, DewLabelsTP);
    IUSaveConfigNumber(fp, PortGpioNP);
    IUSaveConfigNumber(fp, DewGpioNP);
    IUSaveConfigText(fp, ScriptsTP);
    return true;
}

//////////////////// Communicatiion Functions /////////////////////////////

///////////////////////// READ Functions //////////////////////

///////////////////////////////////////////////////////////////
// get switch status
///////////////////////////////////////////////////////////////
bool KOBSpwr::switchStatus(int rport, bool &sStatus)
{
    // TODO add test for connection
    int port = int(PortGpioNP[rport].getValue());
    //LOGF_INFO("Requested port: %i GPIO pins: %i", rport, port);

    setup();
    if (input_gpio(port)) {
        sStatus = false;
        return true;
    }
    else {
        sStatus = true;
        return true;
    }
}

///////////////////////// WRITE functions /////////////////////
///////////////////////////////////////////////////////////////
//   control switch (on/off)
///////////////////////////////////////////////////////////////
bool KOBSpwr::switchControl(int rport, int control)
{
    setup();
    int port = int(PortGpioNP[rport].getValue());
    output_gpio(port, control);

    // TODO add test for completion

    return true;
}

///////////////////////////////////////////////////////////////
//   control switch (on/off)
///////////////////////////////////////////////////////////////
bool KOBSpwr::setPWM(int channel, int power)
{
    setup();
    setup_gpio(int(DewGpioNP[PIN].getValue()), INPUT, 1);    // in, pullup
    setup_gpio(int(DewGpioNP[MA1].getValue()), OUTPUT, 0); // out, no pullup
    setup_gpio(int(DewGpioNP[MA2].getValue()), OUTPUT, 0);
    setup_gpio(int(DewGpioNP[MB1].getValue()), OUTPUT, 0);
    setup_gpio(int(DewGpioNP[MB2].getValue()), OUTPUT, 0);

    // set motors to forward
    output_gpio(int(DewGpioNP[MA1].getValue()), LOW);
    output_gpio(int(DewGpioNP[MA2].getValue()), HIGH);
    output_gpio(int(DewGpioNP[MB1].getValue()), LOW);
    output_gpio(int(DewGpioNP[MB2].getValue()), HIGH);

    pwmSetGpio(int(DewGpioNP[channel].getValue()));
    pwmSetMode(PWM_MODE_MS);
    pwmSetClock(DIVIDER);
    pwmSetRange(int(DewGpioNP[channel].getValue()), RANGE);
    pwmWrite(int(DewGpioNP[channel].getValue()), power);

    return true;
}

///////////////////////////////////////////////////////////////
//   update power stats
///////////////////////////////////////////////////////////////
bool KOBSpwr::getPwr()
{
      // if LastParseSucess was false, means we had error with script, no need repeating this
    if(LastParseSuccess) {

        char cmd[51];
        snprintf(cmd, 50, "/usr/share/indi/scripts/%s", ScriptsTP[PowerScript].text);

        if (access(cmd, F_OK|X_OK) == -1) {
            LOGF_ERROR("Cannot use script [%s], check its existence and permissions", cmd);
            LastParseSuccess = false;
            return false;
        }

        LOGF_DEBUG("Run script: %s", cmd);
        FILE* handle;
        handle = popen(cmd, "r");
        if (handle == nullptr) {
            LOGF_ERROR("Failed to run script [%s]", strerror(errno));
            LastParseSuccess = false;
            return false;
        }

        byte_count = fread(buf, 1, BUFSIZ - 1, handle);
        pclose(handle);
        buf[byte_count] = 0;
        if (byte_count == 0) {
            LOGF_ERROR("No output from script [%s]", cmd);
            LastParseSuccess = false;
            return false;
        }

        LOGF_DEBUG("Read %d bytes output [%s]", byte_count, buf);

        rc = sscanf(buf, "%f,%f,%f,%f,%f,%f", &dewVolts, &dewCurrent, &dewPower, &portVolts, &portCurrent, &portPower);

        if (rc < 1)  {
            LOGF_ERROR("Failed to parse input %s", buf);
            LastParseSuccess = false;
            return false;
        }
    }
    return true;

}


