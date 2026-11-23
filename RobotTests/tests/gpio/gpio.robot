*** Settings ***
Library    ${CURDIR}/../../libraries/GpioLibrary.py    ${CURDIR}/../../config/testbench.yaml
Suite Setup       Connect
Suite Teardown    Disconnect

*** Test Cases ***
Configure Output Pin
    Configure Pin    7    out

Set And Get High
    Configure Pin    7    out
    Set Pin    7    1
    ${v}=    Get Pin    7
    Should Be Equal As Integers    ${v}    1

Set And Get Low
    Configure Pin    7    out
    Set Pin    7    0
    ${v}=    Get Pin    7
    Should Be Equal As Integers    ${v}    0

Configure Input Pin
    Configure Pin    8    in
    ${v}=    Get Pin    8
    Should Be Equal As Integers    ${v}    0
