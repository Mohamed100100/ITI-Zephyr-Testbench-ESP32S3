*** Settings ***
Library    ${CURDIR}/../../libraries/TestbenchLibrary.py    ${CURDIR}/../../config/testbench.yaml
Suite Setup       Connect
Suite Teardown    Disconnect

*** Test Cases ***
Read ADC Channel 0
    ${resp}=    Send Command    adc read 0
    Should Match Regexp    ${resp}    VAL \\d+
