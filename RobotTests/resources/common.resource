*** Settings ***
Library    ${CURDIR}/../libraries/TestbenchLibrary.py    config/testbench.yaml
Suite Setup       Connect
Suite Teardown    Disconnect

*** Keywords ***
Connect
    Connect

Disconnect
    Disconnect
