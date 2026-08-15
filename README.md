# Gyroscopic_RC_Ball
It's a gyroscopically driven remote control ball!
URL for video of it running here: https://youtu.be/vGXm_efYap4
What I made turned out to be mostly a proof of concept, as it's super hard to control. I plan to eventually fix it through a few things.
First, because all the electronics have to fit on the central ring, and the motors I have are kind of bulky, the space the central weight has to spin is very limited. This could be fixed by either making the ball bigger,, because the ring would stay the same thickness allowing the weight to be bigger, and/or getting smaller motors that can still spin the thing, making more room for the weight.
Second, if I were to use a gyroscopic sensor on the ring and a encoder on the weight motor, I could have the XIAO microcontroller calculate the angle of the weight so it could prevent it from spinning over the center axis, which is what causes the ball to vibrate weirdly.
