# Vocal Keys: A Human Voice Sampled Piano
A custom electronic keyboard that transforms human voices into musical instruments. 
Each key triggers a recorded .mp3 vocal sample mapped to a musical note, creating a personalized piano experience.

## Materials
> ESP32 S3 Dev Board *1  
> Mechanical keys *12  
> DFPlayer Mini *2  
> SD card *2  
> 4 ohm 3W speaker *2  
> B10k potentiometer slider *1
> TPS 61023 voltage level shifter *1  
> // DFPlayer needs 5V input, so a level shifter is needed for converting ESP32 3V3 output to 5V

## Form Factor
![Enclosure](kb2.jpg)

## Wiring
DFPlayer1 TX - ESP32 GPIO 5  
DFPlayer2 RX - ESP32 GPIO 4

DFPlayer2 TX - ESP32 GPIO 16
DFPlayer2 RX - ESP32 GPIO 15

Slider pin 1 - 3V3

