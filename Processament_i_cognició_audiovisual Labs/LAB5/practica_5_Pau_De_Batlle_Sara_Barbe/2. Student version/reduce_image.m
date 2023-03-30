function img_red = reduce_image(img, reduction_factor)
% function to reduce the dimensionality of an image
% img is an image (2D matrix)
% reduction_factor is an integer that determines the scale factor of the dimensions of the image

rf = reduction_factor;

img_size = size(img);

% for each cell of the reduced image
for r = 1:img_size(1)/rf  % loop of the rows
   for c = 1:img_size(2)/rf  % loop of the columns
       % takes the maximum value of the receptive field of the cell
       img_red(r,c) = max(max(img((1+rf*(r-1)):(rf+rf*(r-1)),(1+rf*(c-1)):(rf+rf*(c-1)))));
   end
end

end
