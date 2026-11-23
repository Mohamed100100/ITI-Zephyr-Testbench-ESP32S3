*** Settings ***
Library    ${CURDIR}/../../libraries/PwmLibrary.py    ${CURDIR}/../../config/testbench.yaml
Suite Setup       Connect
Suite Teardown    Disconnect

*** Test Cases ***
Set PWM 50 Percent
    Set Duty    0    50

Set PWM 0 Percent
    Set Duty    0    0

Set PWM 100 Percent
    Set Duty    0    100
