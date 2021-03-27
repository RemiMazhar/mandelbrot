# mandelbrot

## Introduction
This program is meant to provide a lot of different features related to the Mandelbrot set.
The [wikipedia page](https://en.wikipedia.org/wiki/Mandelbrot_set) of the Mandelbrot set gives a very good explanation of what it is and the basic idea of how to plot it.

## Main features
* Explore the Mandelbrot set
  * You can change the colormap (see below)
  * You can change the kernel used to render it (see below)
  * You can dynamically see and modify your coordinates and the level of zoom
* Generate videos of zoom into the Mandelbrot Set
* Generate single images of zooms into the Mandelbrot set

## Mathematical concepts
### The Mandelbrot set
To define the Mandelbrot set, we first define for each complex number _c_ the sequence _z<sub>n+1</sub>=z<sub>n</sub><sup>2</sup>+c_ with _z<sub>0</sub>=0_. The complex number _c_ is said to belong to the Mandelbrot set if and only if this sequence is bounded.  
Another way to say it is that you take a complex number _c_ that will remain constant, then you define a variable _z_ with an initial value of zero and you iterate _z=z<sup>2</sup>+c_. If _z_ gets bigger and bigger indefinitely, _c_ does not belong to the Mandelbrot k.  
However in order to programmatically find if a point belongs to the set, we do not need to iterate indefinitely because of a theorem that proves that if |_z_|>_2_ then the sequence will always diverge Therefore, we only need to iterate the sequence until |_z_|>_2_ to be sure that a number is outsie of the Mandelbrot set. We consider that a number is inside of the Mandelbrot set if |_z_|<_2_ after a sufficiently large number of iterations.  
We can "plot" the Mandelbrot set by coloring each point of a plane one color if its affix is in the Mandelbrot set, and another color if if the point is outside the Mandelbrot set. Here is the image we get after coloring in white points whose affix belong to the set in white and points whose affix don't belong to the set in black:
 ![Mandelbrot set](https://user-images.githubusercontent.com/80780126/112723037-e5a35c00-8f0c-11eb-8e11-18224300ea81.png)  
 The result is a bit strange but we can notice something zooming around the boundaries of the set: it has an unlimited level of details. For example, here is an image of a zoom around the point (-0.135; 0.75):  
 ![image](https://user-images.githubusercontent.com/80780126/112723201-a45f7c00-8f0d-11eb-9fb1-06f3a8ba03fb.png)  
This is because the Mandelbrot set is a [fractal](https://en.wikipedia.org/wiki/Fractal).  
The "explore" feature of this program lets you zoom in different places of the Mandelbrot set, to discover all the details of its boundaries.

### Coloring of the outside
Many parts of the Mandelbrot set are "infinitely thin" (they have a width of zero). In an image colored as described before, you can't see these parts at all. In order to see them more clearly, it would be nice to have a way to color points outside of the set to vaguely indicate how close they are to the set. There are many ways to do it, the easiest is to count after how many iterations |_z_|>_2_, which means after how many iterations it takes to discover that the point is outside the set. Then you can use this value to color the point in grayscale, for example you can divide this value by the maximum number of iterations you chose and set an RGB color accordingly. Here is how it looks with 100 iterations:  
![image](https://user-images.githubusercontent.com/80780126/112723739-5a2bca00-8f10-11eb-8b1e-cafc5ebf8e26.png)  
It already looks much nicer than the previous images, but this algorithm has some downsides: the look of the image will change a lot depending of your number of iterations, and there is a kind of aliasing because you can only have a whole number of iterations.  
If you prefer colors to black and white, you can apply a "colormap", which means mapping numbers between 0 and 1 to colors in the RGB space.  
To render the Mandelbrot set, this program uses two GLSL shaders:
* The "kernel", that makes the main computation for each pixel and outputs a value between zero and one. That's where the kind of algorithms described above to color the outside of the set may be implemented. The kernel can be changed at any time while the program is running. A few kernels are included in the /kernels folder, but you can also create your own. Most of the algorithms used in the kernels that I made come either from [there](https://en.wikipedia.org/wiki/Plotting_algorithms_for_the_Mandelbrot_set) or from [there](https://www.math.univ-toulouse.fr/~cheritat/wiki-draw/index.php/Mandelbrot_set).
* The "colormap" shader. It contains a functions "colormap" that takes a float between 0 and 1 as input and outputs a vec4 corresponding to an RGBA color. One is included in the /colormaps folder, but kbinani made over one hundred other ones downloadable [here](https://github.com/kbinani/colormap-shaders) (I made the program to be compatible with them).

### Examples of images rendered with some kernels
#### centered on (0; 0), zoom=0.2, 1000 iterations
Rendered with the default kernel, colorFrequency=1.5  
![image](https://user-images.githubusercontent.com/80780126/112724820-65cdbf80-8f15-11eb-91c7-8db5c2d34364.png)  
   
Rendered with the default kernel and the IDL_CB-RdGy colormap, colorFrequency=1.5  
 ![image](https://user-images.githubusercontent.com/80780126/112724907-c6f59300-8f15-11eb-8c9a-c30131873859.png)  
  
Rendered with the normal map kernel  
 ![image](https://user-images.githubusercontent.com/80780126/112724963-0fad4c00-8f16-11eb-993e-975ba6f701de.png)  
  
Rendered with the distance estimator kernel
 ![image](https://user-images.githubusercontent.com/80780126/112725043-69157b00-8f16-11eb-8168-00fd319711c0.png)  
 
Rendered with the external rays kernel
 ![image](https://user-images.githubusercontent.com/80780126/112725068-8a766700-8f16-11eb-8a65-88d3e8d8cd2c.png)  
 
#### centered on (-0.810618952736284; 0.203130500759388), zoom=1062.2, 10000 iterations
Rendered with the default kernel, colorFrequency=3.6  
![image](https://user-images.githubusercontent.com/80780126/112725216-610a0b00-8f17-11eb-9d64-559c63446a5a.png)  
  
Rendered with the default kernel and the IDL_CB-RdGy colormap, colorFrequency=3.6  
 ![image](https://user-images.githubusercontent.com/80780126/112725317-d7a70880-8f17-11eb-991a-3a1844bf08e9.png)  
  
Rendered with the normal map kernel  
![image](https://user-images.githubusercontent.com/80780126/112725343-0ae99780-8f18-11eb-8b51-474553446e18.png)  
  
Rendered with the distance estimator kernel  
![image](https://user-images.githubusercontent.com/80780126/112725374-2e144700-8f18-11eb-8c7a-7473e1ac9f2f.png)  
  
Rendered with the external rays kernel  
![image](https://user-images.githubusercontent.com/80780126/112725393-471cf800-8f18-11eb-943a-b0b130fbb20e.png)
