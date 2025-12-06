# 2D and 3D sph fluid simulations

A 2D/3D SPH simulation! This code is _very_ heavily inspired from [Doyub Kim's Fluid Engine Development book](https://doyub.com/fluid-engine-development/). Following along with the book and actually typing out the required classes for particle data, particle systems, and their extensions was an enlightening study of clean software engineering. I tried to take the code a step further by templating the classes so that they are dimension agnostic, allowing you to instantiate a 2D/3D particle system just by passing in the appropriate vector. `ParticleSystem<cato::Vec2d>` for a 2D particle system and `ParticleSystem<cato::Vec3d>` for a 3D vector system. 

This is accomplished by using compile time `constexpr` to account for deviations in the 2D/3D logic. This mostly occured in neighbor search algorithms, drawing the particles, or bounding the particles. 

I had much more success with the particles in 2D as opposed to 3D, but I found it amazing how much getting good results depended on sitting there and tuning the different paramters such as the mass, target density, equation of state exponent, or speed of sound (which is basically a pressure multiplier term). 

Here's an image of the best simulation I got in 2D. After dropping a ball of particles, it actually looked like the particles splashed back up into the air while the waves on the side crashed onto themselves after colliding with the walls:

![2D drop start](imgs/fluid_splash_00.png)
![2D drop collide](imgs/fluid_splash_01.png)
![2D drop finish](imgs/Fluid_splash_02.png)

The results in 3D were a little less dramatic. I was only able to make a very goopy looking fluid after cranking up particle mass and gravity, while driving down the target density. The simulation is also pretty slow in its current configuration, so some optimizations are required for better 3D results. 
![3D splash start](imgs/3D_splash_00.png)
![3D splash mid](imgs/3D_splash_01.png)
![3D splash final](imgs/3D_splash_02.png)


Some good videso of the simulation can be found below:
1. [interactive 2D fluids](https://vimeo.com/1144029537?share=copy&fl=sv&fe=ci)
2. [Unexpected Fractals](https://vimeo.com/1144030100?share=copy&fl=sv&fe=ci)
3. [The best 2D drop](https://vimeo.com/1144030326?share=copy&fl=sv&fe=ci)
4. [3D goop](https://vimeo.com/1144031595?share=copy&fl=sv&fe=ci)


## Building and running the code. 

### Dependencies
There weren't any dependencies until I moved to 3D. All I needed was code to compile and link the shaders, so I turned to [the starter code provided here](https://clemson.instructure.com/courses/264860/files?preview=28584625). It's a bit of a mess and I really wouldn't reccommend this, but at the moment it's required to run the sph simulation. You have to take the whole common folder and place it next to the folder for this project. So something like

```
|-simple-sph_sim
|    |-bin/include/shaders/src/etc
|
|- common
|    |- include/linuxi386/etc
```

The make file takes care of the rest. It's an awful hack and only exists so I can compile the shaders. 

### Running the code
To build the code, simply `make`, then `./bin/sph_sim [number_of_particles: int]`. As the program stands at the moment, the number of particles you pass in doesn't acctually affect anything. The particles are actually spawned in a grid, the code for which can be found in `main.cpp`. Once the simulation is running, press `c` to get a printout of the controls in order to adjust/toon the simulation for better results. I suggest increasing gravity to get the 3D simulation to work a bit better. And driving up the particle masses. 

Switching between 2D and 3D actually takes a bit of work at the moment, and making this process more seamless is on the to do list. But as of right now here's a basic run down of what needs to be done:

1. In `Model.h` Change the `SPHSystemSolver` to be either a `SPHSystemSolver2d` or `SPHsystemSolver3d`
2. In `Model.cpp` in the constuction function, uncomment the block for either a 2D/3D bounding box. 
3. In `Controller.cpp` swap out the box reset to 2D/3D when `r` is pressed
3. If you want to interact with the code, uncomment the relevant code in `Model.cpp:simulate()`

## Results

The results are discussed in the pdf. 

