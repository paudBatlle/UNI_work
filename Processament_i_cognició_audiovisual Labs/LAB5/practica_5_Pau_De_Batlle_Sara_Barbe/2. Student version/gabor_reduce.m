function data_red = gabor_reduce(data, reduction_factor, orientations, kern_size, lambda, phi, sigma, gamma, image_size, fig)
% data is the data matrix with pixels in the columns and images in the rows
% reduction_factor is an integer that determines the scale factor of the dimensions of the image
% orientations, kern_size, lambda, phi, sigma, gamma are the parameters of the gabor filter
% image_size is a vector with the dimensions x rows of the images: image_size (1) * image_size (2) has to be equal to size (data, 2)
% this function applies a gabor filter with several orientations and reduces the dimensionality of the result.

if any(mod(image_size(1:2), reduction_factor) ~= 0)
    error('please choose a reduction_factor multiple of both image dimensions')
end

rgb_ch = 1;
if numel(image_size)==3
    rgb_ch = 3;
end

size_red = size(data,2)/rgb_ch/reduction_factor^2;
data_red = zeros(size(data,1), size_red*length(orientations));  % init
 
kern = zeros(kern_size,kern_size,size(orientations,2));

for o = 1:size(orientations,2)
   kern(:,:,o) = gabor_kernel(kern_size, orientations(o), lambda, phi, sigma, gamma, false);
end
if fig
    figure;
    for o = 1:size(orientations,2)
    subplot(3,3,o);
    imagesc(kern(:,:,o));xlim([0.5,kern_size]);ylim([0.5,kern_size]);
    title( sprintf('\\theta = %2.2f rad',orientations(o)) );
end
end

% FOR EACH IMAGE
for i = 1:size(data,1)
    % transforms the data into an image (be careful if there are more RGB channels)
    img = reshape(data(i,:), image_size);
    if numel(image_size)==3  % if there are 3 RGB channels, in the case of the image not in the case of the digits
          % add the values ​​of the 3 channels (third dimension), using "img = sum(...)" to add the channels to the third column
          img = sum(img, 3);
    end
    img = img';
    % FOR EACH ORIENTATION OF THE GABOR
    for o = 1:size(orientations,1)
       % APPLY GABOR_KERNEL_FUNCTION
       % call the gabor_kernel_function (ideally, use a function that takes the parameters and returns the kernel)
       % and applies the convolution
       
       filtred = conv2(img, kern(o), 'same');
       
       % REDUCE IMAGE DIMENSION
       % reduce dimension
       % transforms into vector and concatenates all filtered images. Then join the vectors to get data_red
       img_red = reduce_image(filtred, reduction_factor);
       data_red(i,(1+(o-1)*size_red):(o)*size_red) = reshape(img_red, [1, numel(img_red)]);
    end
end

data_red = data_red';
