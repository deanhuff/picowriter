# Picowriter

## Description

Picowriter is a battery powered eink typewriter.  Similar to a *[Freewrite](https://getfreewrite.com/products/freewrite-smart-typewriter "Freewrite Smart Typewriter")* or *[Alphasmart](https://en.wikipedia.org/wiki/AlphaSmart "Alphasmart")*, the Picowriter is intended for writing.  Unlike these devices, the Picowriter does not have an integrated keyboard.  Rather, you bring your own USB or Bluetooth keyboard for use with the Picowriter.

## Project Goals
- Create and Edit text documents.
- Document management
  - Create new, edit & delete existing documents on device
  - Sync files with to a PC via a host application
  - Read / Write documents via USB Mass Storage Mount
- Connect to terminal servers via a SSH terminal
  - ssh configuration and key management 
- Use either mobile or at your desk
- Great battery life measured in weeks
- Eink screen

## Project Timeline
- infinite
- Check out the [Progress Report](https://github.com/deanhuff/picowriter/wiki/Progress-report)

## Hardware
- Raspberry pi pico
- Eink SPI display module
- Realtime clock module
- Bluetooth module
- Wifi module
- Rechargable battery module
- USB keyboard input via Pico USB HID Host
- 3 way switch
- Power button
- Bluetooth Pairing button

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
