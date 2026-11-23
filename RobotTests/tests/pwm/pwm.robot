*** Settings ***
Library    ${CURDIR}/../../libraries/TestbenchLibrary.py    ${CURDIR}/../../config/testbench.yaml
Suite Setup       Connect
Suite Teardown    Disconnect

*** Test Cases ***
Set PWM 50 Percent
    Expect Ok    pwm set 0 50

Set PWM 0 Percent
    Expect Ok    pwm set 0 0

Set PWM 100 Percent
    Expect Ok    pwm set 0 100
