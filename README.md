#  Sample-GUI-for-Relay-Card
This is the demo application of GUI for project Relay Card. It is writen in QT. This simple application allows read and change state of relays on Relay Card. This demo application controls 4 relays, but it is simple to change the amount of relays. This is related to these repositories:
[Firmware-for-Relay-Card](https://github.com/chapppy/Firmware-for-Relay-Card "Firmware-for-Relay-Card") 
[Sample-GUI-for-Relay-Card](https://github.com/chapppy/Sample-GUI-for-Relay-Card "Sample-GUI-for-Relay-Card")
[Board-for-Relay-Card](https://github.com/chapppy/Board-for-Relay-Card "Board-for-Relay-Card")


## Instalation of STM32 Virtual COM Port Driver
The drive can be downloaded from [STM32 Virtual COM Port Driver](http://www.st.com/en/development-tools/stsw-stm32102.html "STM32 Virtual COM Port Driver"), There is guide for instalation too.

## Connection of GUI to device
The GUI find itself the Relay Card and start comminication with it. The search is based on VCP vendor description

# Example of finall GUI
![gui](doc/gui.jpg "GUI")
