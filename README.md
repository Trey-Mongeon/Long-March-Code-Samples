# Overview

This repository contains some code from my Sophomore year game project at DigiPen Institute of Technology. This game is actually on steam if you would Like to see it for yourself: [Long March](https://store.steampowered.com/app/5206450/Long_March).

## My Role

As the only graphics programmer on the team I had the task of learning OpenGL and creating a 2D graphics engine at the same time, while also working fast enough that I did not hold back progress on the game.



### Particle System


<video src="./Videos/LongMarchParticleEfficiencyDemo.mp4" controls="controls" style="max-width: 100%;">
</video>


The thing that I am most proud of in this project is my custom particle system. Throughout development I was always excited to improve it and expand its functionality.

#### [Emitters](https://github.com/Trey-Mongeon/Long-March-Code-Samples/blob/main/Emitter.h)

At the very beginning of my process of creating my particle system I created the Emitter class. Emitters manage themselves and facilitate creating and updating their particles. To have entities own an emitter we needed to create a wrapper around the Emitter class. This wrapper class was called EmitterComponent, it is not included in these samples currently since it just allows Emitter to conform to the engines system of entities owning and being comprised of components.


#### [Particles](https://github.com/Trey-Mongeon/Long-March-Code-Samples/blob/main/Particle.h)

Particles are my favorite part of the particle system, which is fitting, because the whole system is there *for particles*. Particles are only in charge of keeping track of their data. When an Emitter calls Particle::Update on a Particle all it has to do is update its own data accordingly. When an Emitter calls Particle::AddToVertices all it has to do is give the vertex array some vertices based on its data.

#### Particle System Conclusion

At the time of making my particle system I was delighted at what I had designed and built and I often tried to think of ways of optimizing it. At the very end of the project I implemented batch rendering for particles and thought I had a very impressive system. Today, I do still feel proud of the particle system, but now that I know that I could have particles being handled on the GPU, I know exactly how to outdo my past system.



### Drawing

I'm including some of my functions used for drawing things in these samples as some context for how the batched particles are being drawn. But also because [Render.cpp](https://github.com/Trey-Mongeon/Long-March-Code-Samples/blob/main/Render.cpp) contains some of the oldest code in the project with bits of the Render function and some of the newest code in the DrawParticles function. I just thought its cool how they are right next to each other despite being from opposite ends of the project.
