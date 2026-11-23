*** Settings ***
Library    ${CURDIR}/../libraries/TestbenchLibrary.py    ${CURDIR}/../config/testbench.yaml
Suite Setup       Connect
Suite Teardown    Disconnect

*** Test Cases ***
Ping Board
    ${resp}=    Send Command    ping
    Should Contain    ${resp}    PONG

Help Lists Commands
    ${resp}=    Send Command    help
    Should Contain    ${resp}    gpio
