*** Settings ***
Resource    ${CURDIR}/../../resources/common.resource

*** Test Cases ***
Ping Board
    ${resp}=    Send Command    ping
    Should Contain    ${resp}    PONG

Help Lists Commands
    ${resp}=    Send Command    help
    Should Contain    ${resp}    gpio
