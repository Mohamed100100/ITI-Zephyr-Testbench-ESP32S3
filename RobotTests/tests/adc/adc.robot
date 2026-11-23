*** Settings ***
Library    ${CURDIR}/../../libraries/AdcLibrary.py    ${CURDIR}/../../config/testbench.yaml
Suite Setup       Connect
Suite Teardown    Disconnect

*** Test Cases ***
Read ADC Channel 0
    ${v}=    Read Channel    0
    Should Be True    ${v} >= 0
