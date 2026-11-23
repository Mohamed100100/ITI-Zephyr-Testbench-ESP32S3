*** Settings ***
Library    ${CURDIR}/../../libraries/UartLibrary.py    ${CURDIR}/../../config/testbench.yaml
Suite Setup       Connect
Suite Teardown    Disconnect

*** Test Cases ***
Send Text To UART1
    Send Text    hello

Receive From UART1
    ${resp}=    Receive Text
    Should Match Regexp    ${resp}    RX .*
