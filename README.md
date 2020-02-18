# C++ OpenGL Pathtracer
OpenGL Real-time Pathtracer using Compute Shaders
  
# Install instructions  
Follow the instructions at https://github.com/williamsandst/opengl-boilerplate (but remember the repo has a different name!)  
  
# Todo 
Implement objects as a texture, which allows dynamic object creation instead of the current hardcoded objects in the shader
Implement Hierarchical Bounding Volumes to speed up intersection testing  
  
Future ideas:
Create a more physically based pathtracer with multiple samples, which would allow for more glossy items and soft shadows. It would however negatively impact other real-time aspects of the Pathtracer.  

# Resources on the subject  
https://www.scratchapixel.com/ - Most comprehensive source online regarding raytracing and other rendering topics. Good explanations and includes code examples.  
  
http://antongerdelan.net/opengl/compute.html - Great introduction to Compute Shaders in relation to raytracing. 

https://github.com/LWJGL/lwjgl3-wiki/wiki/2.6.1.-Ray-tracing-with-OpenGL-Compute-Shaders-%28Part-I%29 - 
Website detailing the first steps of perspective correct raytracing. Good info but a bit rambly, no further tutorials as well.

http://blog.three-eyed-games.com/2018/05/03/gpu-ray-tracing-in-unity-part-1/ - Another good tutorial detailing the basics and later even very advanced topics. Only downside is it is targeted towards Unity and written in HLSL, not GLSL, but not a huge deal. 
 
http://web.archive.org/web/20070628033341/http://www.devmaster.net/articles/raytracing_series/part1.php - Old raytracing tutorial, only avaiable on archive.org 

http://raytracerchallenge.com/ - Another interesting website with ray tracing tutorials based on Tests, can be a bit confusing at times though.  

https://www.youtube.com/user/icytower103/videos - Channel showcasing a hybrid ray tracer which runs in real time with soft shadows and such, good info in comments.  
