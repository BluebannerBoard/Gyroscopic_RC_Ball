# Gyroscopic_RC_Ball
**It's a gyroscopically driven remote control ball!**  
**Onshape CAD model url here:** https://cad.onshape.com/documents/e8027be8b30833860861e85e/w/ca8649f464d1056c32c0c05b/e/233bed9146ca392ed125c422?renderMode=0&uiState=6a8007cff24449429c0c4785  
URL for video of it running here: https://youtu.be/vGXm_efYap4  
What I made turned out to be mostly a proof of concept, as it's currently super hard to control. I plan to eventually fix it through a few things.  
First, because all the electronics have to fit on the central ring, and the motors I have are kind of bulky, the space the central weight has to spin is very limited. This could be fixed by either making the ball bigger,, because the ring would stay the same thickness allowing the weight to be bigger, and/or getting smaller motors that can still spin the thing, making more room for the weight.   
Second, if I were to use a gyroscopic sensor on the ring and a encoder on the weight motor, I could have the XIAO microcontroller calculate the angle of the weight so it could prevent it from spinning over the center axis, which is what causes the ball to vibrate weirdly.  

**Things contained in this project**  
Code to connect the IOS app "Bluetooth Serial Connect" to a Seeed Studio XIAO esp32C6 and interpret joystick commands.  
CAD files for my first (and curretnly only) iteration of the ball

**Recourses used**  
I used duck.ai (duck.go's ai chatbot tools) and the Seeed Studio wiki with it's examples to help me write the code to make the Bluetooth connection work and interpret the binary.  
I posted devlogs for this project for the Hackclub Stardance Challenge.
