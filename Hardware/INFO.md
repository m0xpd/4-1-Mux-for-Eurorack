# This folder contains:

Eagle files defining the schematics and PCB designs for the main multiplexer and the slave unit.

Kicad projects, describing Front Panels for the main multiplexer and the slave.

Initial versions of the pcb's and the front panels have been fabricated at https://jlcpcb.com/ (usual disclaimer)
who delivered their usual high quality service. 

You can see the result of uploading the main multiplexer pcb gerbers to JLPCB's quote page here: 

![Screenshot 2023-03-06 at 16 45 21](https://user-images.githubusercontent.com/3152962/223183638-2cd88200-6267-4d22-893b-cc97f0fcc78d.png)

There is also an Eagle library for the Thonk Low-profile illuminated push button, used in the design of the main unit for 
manual input selection and current input display. 

![low-profile-led-button-main](https://user-images.githubusercontent.com/3152962/223185038-6319f849-978c-4478-ae12-134c0944e22d.jpg)

There is a data sheet at Thonk: https://www.thonk.co.uk/wp-content/uploads/2021/12/LowProfileButton.pdf, but I could find no Eagle library, so I made 
a footprint and a symbol - the former is fine, but the latter is naive and clumsy (as you'll see in the schematic).
