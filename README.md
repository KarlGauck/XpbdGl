# Extended Position Based Dynamics
This project is my attempt to implement a general purpose physics-engine.
It is written in C++ and OpenGl is used for visualisation.

### Implemented:
- Default XPBD solver algorithm (lots of redundancy (no polymorphism, bad))
- CollisionConstraints, MaxPositionConstraints (BoxConstraints), DistanceConstraints
- Dynamic constraint handlers
- Spacial hashing for dynamic handlers
- Scenes + Support for different solver methods (like verlet etc. (none implemented yet))
- Basic SPH densityconstraint
  
### Planned Features include:
- Fluid Dynamics using SPH kernel functions
- Entitycomponentsystem
- Rigid Bodies
- Soft Bodies
- Coupeling between different objects (fluid and rigid bodies for example)

### Current Problem: 
- Bugs with spatial hashing#
- stability of simulation
- accuracy of simulation

### TODO:
- Refactor XPBD-Engine

# Images
![image](https://github.com/KarlGauck/XpbdGl/assets/91132264/518a712d-8bc2-4996-b1cd-1be8dfbb110a)
![image](https://github.com/KarlGauck/XpbdGl/assets/91132264/97f574d7-4f07-436f-96ad-9e73d258cfe6)

![image](https://github.com/KarlGauck/XpbdGl/assets/91132264/2e626114-48be-404d-aa27-3d471777cc50)


