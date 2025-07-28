# 2024 Antweight robot workshop

Organized at [Kaunas Makerspace](https://makerspace.lt) in October 2024.

DIY Antweight Robot designed for an inexpensive workshop that introduces
soldering & robotics to beginners. It's fun to assemble in a group and then
fight in an arena. The robot follows Lithuanian Antweight format - 200 g weight.

The robot is controlled via a website: https://makerspace.lt/antweight-workshop-2024/

* Open the website using Use Google Chrome or a Chrome-based browser on Android. On iOS use the [Bluefy browser](https://apps.apple.com/us/app/bluefy-web-ble-browser/id1492822055). The website connects your phone to the robot's ESP32 microcontroller using the [Web Bluetooth API](https://developer.mozilla.org/en-US/docs/Web/API/Web_Bluetooth_API#browser_compatibility).

Total cost of everything (if you source 1 cell lithium battery yourself) - around 15eur.

Electronic parts:
* 2x wheels https://www.aliexpress.com/item/1005004255557549.html
* 1x DRV8833 motor driver https://www.aliexpress.com/item/1005006282457232.html
* 1x ESP32 dev board https://www.aliexpress.com/item/1005004337178335.html
* 2x N20 motors 6V 300 RPM https://www.aliexpress.com/item/33022320164.html
* 1x 9V 2A boost module https://www.aliexpress.com/item/1005005501790376.html
* 1x TP4056 1A USB Li-ion charger module https://www.aliexpress.com/item/1005005037876729.html
* 1-2x SG90 servo motor https://www.aliexpress.com/item/1005006737468107.html
* 4x JST XH2.54 2pin 24AWG 80mm cable with female connector (1x for battery, 2x for motors, 1x for charger) https://www.aliexpress.com/item/1005004500537507.html
* 4x JST XH2.54 2pin male terminals (1x for battery, 2x for motors, 1x for charger) https://www.aliexpress.com/item/4000933759536.html
* 1x MTS-103 toggle switch https://www.aliexpress.com/item/33012530737.html
* 2x 15pin 2.54mm female header (for ESP32) and 2x 6pin 2.54mm female header (for DRV8833) - you can by longer headers and cut them https://www.aliexpress.com/item/1005003642939564.html

Additionally:
* PCB - about 30€ for 25x at JLCPCB with shipping & taxes
* PETG plastic for 3D printing - ~80g one robot, recommendation - filalab.shop
* Screws:
  * 6mm round 4pcs (for mounting the PCB)
  * 6mm flat 9pcs (countersunk) (for top panel and front panel mounting)
  * 10mm flat 4pcs (countersunk) (for side panel mounting)
  * 20mm flat 8pcs (countersunk) (for motor mounting
* 1x 4.2V battery (good option is a battery from a one-time use vape, prefferably >400 mAh).

The PCB has two input voltage connectors. You can use the second connector for connecting a second battery, internal charger or another device (for example - ESC). The PCB also has two expension ports for servo / esc / led use.

* Idea, PCB design, CAD design by [Tomas](https://sobotz.com/)
* Control code by [juodumas](https://github.com/juodumas/)
