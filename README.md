# KOBS Power
INDI Kahale Observatory RPI driver for relays and PWM dew control

KOBS Power INDI Driver
=========================================
This package provides the INDI driver for DLI's Digiswitch (Web Switch Pro):
- 4 12V relays
- 2 PWM dew control
- Current monitoring for the relays and PWM (one for all)
- Input voltage monitoring 

The DLI Digiswitch driver is compatible with libindi >= v1.8.2

FEATURES
========
- Control the power (on/off) for the 4 ports
- Control cycle % for 2 PWM dew ports
- (Re)Name ports
- Power everything on or off

INSTALL
=======
- Reference the INSTALL document.

How to Use
==========

You can use the driver in any INDI-compatible client (such as KStars or Xephem) - just select 
KOBSpower from the Auxiliary list

To run the driver from the command line:

$ indiserver indi_KOBSpower # plus other needed drivers (telescope, camera, etc)

or using the indiwebserver.

You can then connect to the driver from any client, the default port is 7624.
If you're using KStars, the drivers will be automatically listed in KStars' Device Manager.

Before you Start
================

- Setup the Digiswitch

  - Log into the Digiswitch and set:
    - 'Setup': fill out 'LAN Configuration and General Network Settings'
    - 'Setup': fill out 'Administrator credentials'
    - 'Setup/Access':  check 'Allow legacy state-changing GET requests'
    - 'External APIs': check 'Allow REST-style API'
    - 'External APIs/UPnP outlet binding configuration': check all the ports you want this driver to control

  - Optionally:
    - 'Setup/Power Loss Recovery Mode': set the recovery mode
    - 'Setup/Delay': Wrong Password Lockout sets the minutes you need to wait before attempting another login (default=5 mins)
    - 'Setup/Unit Names': set the ports names (this can be changed in this driver)
    - 'Setup/Delay': set the cycle delay time (this can be changed in this driver)

- Set the authentication parameters (user, password, hostname) under the 'Options' tab
  - After authentication, click on 'Connect' under the Main Tab
- Once successfully authenticated to the DLI switch, you can (re)set the port names
- There is a slight delay between setting a switch and the indicator light

*Read the Release Notes*

OPERATIONS
==========

Config Tab
----------

![Options Tab](Assets/ConfigTab.png)


Main Tab
--------

![Main Tab](Assets/MainTab.png)

The Main Tab allows you to connect/disconnect from the Digiswitch as well as turn the ports on or off, or cycle the port if it is on.

The cycle time will be whatever is set in the Digiswitch.

This also allows you to turn all port on or off.


Options Tab
-----------

![Options Tab](Assets/OptionsTab.png)

First time access, you will need to set the user name, password and hostname of the DLI digiswitch.  

NOTE: you will need to click on the 'Connect' button on the Main Tab to continue.

Once authenticaled, the authentication fields will disappear and the port names fields will appear.


General Info Tab
----------------

![General Info Tab](Assets/GeneralTab.png)

Driver Info/Version is the version of this INDI driver

DLI Version is the firmware version reported by the Digiswitch


ISSUES
============
- Reference the Release Notes (above)



