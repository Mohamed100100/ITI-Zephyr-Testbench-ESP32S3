*** Settings ***
Library    ${CURDIR}/../../libraries/TestbenchLibrary.py    ${CURDIR}/../../config/testbench.yaml
Suite Setup       Connect
Suite Teardown    Disconnect

*** Test Cases ***
Configure Output Pin
    Expect Ok    gpio config 7 out

Set And Get High
    Expect Ok    gpio config 7 out
    Expect Ok    gpio set 7 1
    Expect Val    gpio get 7    1

Set And Get Low
    Expect Ok    gpio config 7 out
    Expect Ok    gpio set 7 0
    Expect Val    gpio get 7    0

Configure Input Pin
    Expect Ok    gpio config 8 in
    Expect Val    gpio get 8    0
