*** Settings ***
Library    ${CURDIR}/../libraries/CoreLibrary.py    ${CURDIR}/../config/testbench.yaml
Suite Setup       Connect
Suite Teardown    Disconnect

*** Test Cases ***
Ping Board
    ${resp}=    Ping
    Should Contain    ${resp}    PONG

Help Lists Commands
    ${resp}=    Help
    Should Contain    ${resp}    gpio
