# Some Experiments with OpenGL

Note - The Old cube code now sits inside of the oldGLCube branch. 

### Utah Teapot with Phong Lighting

<img alt="UtahTeapot.gif" src="https://github.com/abhishekingit/LearnOpenGL/blob/Main/assets/outputFiles/crazyTeapotRender.gif?raw=true" data-hpc="true" class="Box-sc-g0xbh4-0 kzRgrI" height="512px">

<img alt="UtahTeapotTopView.gif" src="https://github.com/abhishekingit/LearnOpenGL/blob/Main/assets/outputFiles/crazyTeapotTopView.gif?raw=true" data-hpc="true" class="Box-sc-g0xbh4-0 kzRgrI" height="512px">

The Normals for circular plate of the down face is slightly inaccurate which gives weird lighting glow in the GL application. I corrected the normals for up face of the circular plate model in Blender and the updated model is present in output/models/Teapot.

### Utah Teapot with Material

<img alt="MaterialTeapotRender.gif" src="https://github.com/abhishekingit/LearnOpenGL/blob/Main/assets/outputFiles/MaterialTeapotRender.gif?raw=true" data-hpc="true" class="Box-sc-g0xbh4-0 kzRgrI" height="512px">

I added material textures for Teapot and Principled BSDF material for other meshes. The new teapot_n_glassMAT blender file has the updated model. This Teapot render is missing roughness map, normal map and the metallic map. 

