*** Settings ***
Resource    ../../resources/common.resource
Suite Setup       Connect
Suite Teardown    Disconnect

*** Test Cases ***
SPI Transfer
    ${resp}=    Send Command    spi xfer 01 02 03
    Should Match Regexp    ${resp}    RX( [0-9A-F]{2}){3}
