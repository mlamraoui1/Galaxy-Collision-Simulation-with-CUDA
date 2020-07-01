# Galaxy-Collision-Simulation-with-CUDA

This project will focus on using CUDA for an n-body simulation of the collision of our Milky Way galaxy and the Andromeda galaxy about 5 billion years from now. Galaxy collisions are nice since we have a lot of particles affected by gravity and the odds are that none of those individual particles will actually collide during the simulation.

There is a nice dataset available here containing 82,000 particles for a Milky Way /Andromeda collision:
 http://bima.astro.umd.edu/nemo/archive/#dubinski
 
To make the collision more obvious we can use just the disk and bulge locations and avoid the halo. That still leaves 49152 stars. OpenGL can easily draw that many stars but the gravity calculations will need to be simplified to do them in realtime.

This is why we decided to use CUDA: CUDA will be dealing with computing and updating the particle positions. OpenGL will be used to display the results.

The file 
