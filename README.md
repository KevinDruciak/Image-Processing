# Image Processing

Kevin Druciak

MSVC / Visual Studio Community 2017 / Windows 10

All output is located in the Output folder.

![morph](.\Output\kdrucia1_HTML\kdrucia1.art.101.gif)

### addRandomNoise

I simply generated a uniform random distribution in the range (-noise, noise). From then I sampled the random distribution for each channel of every pixel in the image.

### brighten

Brightening the image is as simple as multiplying each of the channels by the brightening factor.

### luminance

In order to make the image gray, I set the channels of every pixel to the average of its channels according to the ratio 30:11:59 (R:B:G).

### contrast

Here I first calculate the average luminance of the entire image. I use that average to scale the image accordingly with the contrast factor.

![1](.\Output\kdrucia1_HTML\contrast.gif)

### saturate

I calculate the saturation of the image much in the same way that I calculate contrast. Instead of finding the average luminance for the entire image, I simply find the luminance for the pixel being operated on.

![2](.\Output\kdrucia1_HTML\saturate.gif)

### crop

I simply iterate through the pixels in the range of the given indices and output an image with those pixels.

### quantize

Quantizing the image according to the number of bits allotted we apply the following formula:

![3](.\Output\kdrucia1_HTML\quantize.gif)

### randomDither

I generated a uniform random distribution in the range (-1, 1). I then add a bit of noise to each of the pixels according to the formula:

![4](.\Output\kdrucia1_HTML\rDither.gif)

### orderedDither2x2

Ordered dither with a 2x2 matrix essentially decreases the resolution of the image 4 times. This method takes advantage of the fact that the human eye perceives groups of pixels as a single color. For example, given black and white, ordering the pixels in a a specific manner can yield different shades of gray. The dithering matrix is:

![5](.\Output\kdrucia1_HTML\oDither.gif)

I find the index into the matrix from the remainder of indices x and y from the original image. From there I find the difference of the bits-adjusted value of the pixel from its floor (c). If the index into the matrix divided by n^2 + 1 is greater than the error, the value at that pixel is the ceiling of c; else, floor of c.

### floydSteinbergDither

Floyd-Steinberg dithering distributes the error of the quantized pixel to the next pixels in the 3x3 matrix surrounding the pixel according to this image:

![6](.\Output\kdrucia1_HTML\fsDither.png)

The error calculated in Floyd-Steinberg dithering is the difference between the value of the original pixel and quantized pixel.

### blur3x3

In blur3x3, we apply a gaussian blur to the entire image according to the normalized mask:

![7](.\Output\kdrucia1_HTML\blur3x3.gif)

Every pixel has their values average by the values of the surrounding pixels weighted according to the mask.

### edgeDetect3x3

In edgeDetect3x3, we apply another mask that calculates error between pixels.

![8](.\Output\kdrucia1_HTML\edges3x3.gif)

If the values of the neighboring pixels are similar to the value of the pixel at hand, the result of applying this matrix will be close to zero. The further from zero, the more that the pixel differs from its neighbors and is thus an edge. In my implementation, I determined an edge to be a pixel whose error surpasses an arbitrary threshold. In this case, I chose 20\. This error is calculated for each color channel. Therefore, for pixels differing from their neighbors in each color channel, the output pixel is white. If only the red channel differs, the output pixel is red, etc. (Method 1)

It is worth noting that there are multiple method of displaying the edge of an image. Another option I considered was to iterate over the image once and find the max errors for each channel. Thus, on my second iteration, I output a color corresponding to how much the pixels error defers from the max error of that pixel. (Method 2)

##### Method 1:

![Method 1](.\Output\kdrucia1_HTML\EdgeDetection.bmp)

##### Method 2:

![Method 2](.\Output\kdrucia1_HTML\EdgeDetection2.bmp)

### nearestSample

This method samples the image according to the floor of the given pixel + 0.5.

### bilinearSample

This method finds the four existing pixel values surrounding the given point and takes their weighted average.

### gaussianSample

This method calculates the weighted average of the given pixel according to a gaussian distribution. The pixels sampled for the gaussian distribution are those that fall within some radius of the given pixel. The weights of the pixels considered with the function:

![9](.\Output\kdrucia1_HTML\gaussian.gif)

where d is the distance between a pixel is the radius and the center pixel. These weights are normalized and the color of the pixel is obtained.

### scaleNearest

Create blank image with the size of the final image. Iterate over the blank image and apply nearestSample to every pixel in the source image (reverse mapping). If the sampled pixel is out of bounds, set to 0\.

### scaleBilinear

Create blank image with the size of the final image. Iterate over the blank image and apply bilinearSample to every pixel (reverse mapping). If any of the four pixel being sampled are out of bounds, set to 0.

### scaleGaussian

Create blank image with the size of the final image. I chose the radius to be 1.0 / scaleFactor. If the radius is not at least 1, increment it. I iterate over the blank image and then take a gaussian sample of the source image with variance = radius / 3.

### rotateNearest

As a note for all rotation methods, I accidently coded a clockwise rotation. Since the example output displayed a counter clockwise rotation, I just negated the angle at the start of each method.

In order to display the entirety of the original image after a rotation, the original image size is changed. This new size is calculated at the start to be:

![10](.\Output\kdrucia1_HTML\newHeight.gif)

![11](.\Output\kdrucia1_HTML\newWidth.gif)

Next, I apply the rotation to the index of the pixel and obtain the coordinates (u, v). The source image is then sampled using nearestSampling to obtain the final rotated image. Any pixels that are out of bounds in the source image are set to 0\. This way, the final image appears in its entirety as if its overlaid on a black background.

### rotateBilinear

I first calculate the new height and width and then iterate over the blank final image. For each pixel in the final image, I apply bilinearSampling to the source image in order to obtain the final image.

### rotateGaussian

I first calculate the new height and width and the iterate over the blank final image. For each pixel in the final image, I apply gaussianSampling to the source image in order to obtain the final image.

### setAlpha

This method simply reads the red channel of an input image and writes them to another image's alpha channels. Since jpg and bmp are 3-channel image formats, this allows us to manually simulate an alpha channel in the composite method.

### composite

Composite takes in the overlay image with its alpha channels populated by setAlpha and convolves it with some source image. I first make sure that the two images are the same size. I then merge the images by the following formula:

![12](.\Output\kdrucia1_HTML\composite.gif)

If a pixel does not have an alpha value, I set the alpha value at that pixel to 1.0 (255).

### funFilter

My fun filter creates the effect of an oil painting.

The algorithm for the oil painting effect is actually quite simple. I iterate over the image and create a numBuckets sized array for each of the color channels and a numBuckets sized array to store the frequency of the different luminance values which gets populated for each of the pixels. I then iterate over some radius around each pixel and find its luminance normalizing that value so that it falls into one of the numBuckets buckets. I then increment the frequency array at that luminance's index. Finally, I iterate over the buckets and find the most frequently occurring luminance value. The final pixel's color becomes the value in the bucket of most frequently occurring luminance value divided by how many pixels compose the total value. Since neighboring pixels have similar luminance values, this gives a blotchy image similar to an oil painting.

Increasing the radius is computationally expensive, but makes the image blotchier like a rough oil painting. Increasing the number of buckets also makes the image blotchier. I found by testing many values that the most optimal oil painting effect is achieved through a bucket size of about 20 and a radius of about 7\. Increasing the bucket size too much makes the image too noisy. Increasing the radius too much compromises fine details. Of course these numbers are image-size specific. These are what I found to work for a 1080x1080 sized image.

## Beier-Neely Morphing:

### warp

For this method, I simply implemented the Beier-Neely morphing algorithm as outlined in the research paper. The only difference I suppose is when sampling the original. I decided to use bilinear sampling. Linear sampling results in jagged edges and Gaussian sampling is too computationally expensive. This was an important choice as I decided to create a 400-frame gif and one of my outputs.

I opted not to fix the "out of bounds" pixels. As a result the output frames have artifacts where I try to sample outside of the range of the source image.

### CrossDissolve

Here I just added the difference in color between image 1 and image 2 multiplied by some blending weight to the source image for every pixel. The dimensions of the new image is the dimensions of image 2 (destination image).

### length

This is simply the distance formula applied to the end points of a line segment.

### distance

I had a rather hard time implementing this function. I could not figure out how to calculate the distance of the point to the line without recalculating u and v as specified in the research paper. Although I could have just included implementation of the distance method in the body of the warp method, I opted to recalculate u and v anyway in the distance method.

![13](.\Output\kdrucia1_HTML\distance.png)

### perpendicular

I found a clever hack for this method where I create a new point by subtracting the second endpoint from the first and then returning a new point as (-second, first).

### GetSourcePosition

I did not implement this method. I assume this is the bulk of the warping method, but I opted to just implement that in the actual warp method itself.

## Notes on output:

#### movie/art contest:

![morph2](.\Output\kdrucia1_HTML\kdrucia1.art.101.gif)

**In order to generate the .gif movie I had to change code in the main method** that calls the morph method. **The line is question is line 132 of main1.cpp**. I wrote a script that creates all of the frames for the movie, but I could not figure out how to pass a floating point number to the method from the script (bash doesn't natively support it). As a workaround, I just divide the whole numbered threshold by the number of frames in the output gif minus 1 to get the threshold for that image.

The command used to create the movie is:

.\createseries.sh --in ./Images/new --bnMorph ./Images/new ./Lines/ --out ./newOut/

This is run from within the "evolution_of_man" folder.

*   **--in**: passes the string "--in" to the script
*   **./Images/new**: passes the first part of the file containing the source image. I iterate from 1 to 16 and append the number as well as the filetype to the end of "./Images/new" to generate something like "./Images/new14.jpg"
*   **--bnMorph**: passes the string "--bnMorph" to the script
*   **./Images/new**: passes the first part of the file containing the destination image. I iterate from 2 to 17 and append the number as well as the filetype to the end of "./Images/new".
*   **./Lines/**: passes the lines folder to the script. Inside this folder is are files named "1.txt" through "16.txt". These contains the line segment information for the morphs.
*   **--out**: passes the string "--out" to the script.
*   **./newOut**: passes the folder containing all of the output transformations. For the evolution of man gif, 400 images are generated and labeled from "1.jpg" to "400.jpg"

The script doesn't actually create the gif. It just create the 400 images. I stitched these together using [https://ezgif.com/maker](https://ezgif.com/maker).

#### 10 frames:

I used a different script to create the 10 frames of me morphing into professional soccer player, Neymar.

The command used to create the movie is:

.\createseries.sh --in .\me.jpg --bnMorph .\neymar.jpg .\me_to_neymar_lines.txt --out .\me_to_neymar_frames\out

This is run from within the "me_to_neymar" folder.

## Additional Notes:

While trying to develop a cool fun filter, I ended up writing a few methods which I never came to use. They are integrated with the existing command line parser and as such can be run on images from the command line.

#### blurNXN

This is run with the name --blurNXN and takes in a sigma and a variance. It does essentially what blur3x3 does but scaled to any radius and any sigma.

#### shiftChannel

This is run with the name --shiftChannel. The user specifies the desired channel and amount to shift it by. This method just add the specified amount to the specified channel for every pixel in the image. The channels are labeled with integers (0 for alpha, 1 for red, 2 for green, 3 for blue).