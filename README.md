# LoRa-monitor

Intention
=========
I was sooooooo sad when I got my first LilyGo TTGO LoRa32 T3 v1.6.1 868 MHz and found out that I won't be able to use it as a public LoRa WAN gateway as TTN, for example, only accepts data packets from receivers that listen on 8 channels.

So I installed meshtastic on it. What a great project idea! Ran out of the box. Sent messages to the air. Never received anything. Too bad noone's using it.

So I wanted to run a software that receives LoRa on one channel, only, but didn't find anything that suited my needs. So I wrote my own.

Features
========
The initial version contains the following features:

* connect to your WiFi
* sync the internal clock with an NTP server
* receive LoRa packets, count them and display them on the built-in display

Feel free to improve the code. Help me improve this thing or fork your own.

Future features could include:
* push every received packet to a list (id, timestamp, ssri, payload)
* include web server that serves a list of all received data
* ...
