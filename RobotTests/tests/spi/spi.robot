*** Settings ***
Library    ${CURDIR}/../../libraries/SpiLibrary.py    ${CURDIR}/../../config/testbench.yaml
Suite Setup       Connect
Suite Teardown    Disconnect

*** Test Cases ***
SPI Transfer
    ${resp}=    Transfer    01    02    03
    Should Match Regexp    ${resp}    RX( [0-9A-F]{2}){3}
