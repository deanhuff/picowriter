# Picowriter

## Description

Picowriter is a battery powered eink typewriter.  Similar to a *[Freewrite](https://getfreewrite.com/products/freewrite-smart-typewriter "Freewrite Smart Typewriter")* or *[Alphasmart](https://en.wikipedia.org/wiki/AlphaSmart "Alphasmart")*, the Picowriter is intended for writing.  Unlike these devices, the Picowriter does not have an integrated keyboard.  Rather, you bring your own USB or Bluetooth keyboard for use with the Picowriter.

## Project Goals
- Create and Edit text documents.
- Document management
  - Create new, edit & delete existing documents
  - Sync files with to a PC via a host application
- Connect to terminal servers via a SSH terminal
  - ssh configuration and key management 
- Use either mobile or at your desk
- Great battery life measured in weeks
- Eink screen

## Project Timeline
- infinite

## Hardware
- Raspberry pi pico
- Eink SPI display module
- Realtime clock module
- Bluetooth module
- Wifi module
- Rechargable battery module
- USB to SPI module
- 3 way switch
- Power button
- Bluetoothe Pairing button

## Software features
- Document metadata
  - word count
  - name
  - created, accessed, modified timestamps
  - sync status
- Display
  - Canvas (main display area)
    - mode siwtched via 3 way hardware switch.
      - Document mode
      - File management mode
      - Settings/Configuration mode
  - Status bar
    - current mode
    - wifi status
    - battery life
    - free storage
    - clock
    - document title
- Configurable editor: VIM, EMACS type features
- Pull and push documents via sync features.
