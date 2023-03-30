%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%              LABORATORY #2 
%%%              VIDEO PROCESSING 2022-2023
%%%              VIDEO SEGMENTATION - BACKGROUND SUBTRACTION 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
clear all
close all
clc

addpath data


% Obtaining the background image or the template
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Loading frames
image_template_1 = double(imread('imagen0000.png'));
image_template_2 = double(imread('imagen0001.png'));
image_template_3 = double(imread('imagen0007.png'));
image_template_4 = double(imread('imagen0008.png'));
image_template_5 = double(imread('imagen0009.png'));
image_template_6 = double(imread('imagen0010.png'));

%Hacemos la media de los frames sin pajaro y lo usamos como template
image_template = (image_template_1 + image_template_2 + image_template_3 + image_template_6 + image_template_4 + image_template_5)/6;  

% Fixing a threshold
th=10;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Showing the image template
figure(1)
imshow(uint8(image_template))

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Video Segmentation
for i = 2 : 6           %cogemos de 2 a 6 ya que las imagenes con objeto solo son esas
    
  % Loading image
  input_image=imread(sprintf('imagen00%02d.png',i));
  input_imagew = double(input_image);

  %Extracting foreground
  [foreground,cc(i),cr(i),radius,flag]=extract_object(input_imagew,image_template,th);
  if flag==0
    continue
  end

  %Observing results
  figure(2)
  clf
  imshow(input_image)
  hold on
  for c = -0.97*radius: radius/20 : 0.97*radius
      r = sqrt(radius^2-c^2);
      plot(cc(i)+c,cr(i)+r,'g.')
      plot(cc(i)+c,cr(i)-r,'g.')
  end 
  hold off

  disp('Observing detection. Press any key');  
  pause;
   
   
end

% Recovering the full trajectory
cr=size(input_image,1)-cr;
figure(100)
plot(cc(2:end),cr(2:end),'--rs','LineWidth',2,'MarkerEdgeColor','k','MarkerFaceColor','k','MarkerSize',10)
axis([0 size(input_image,2) 0 size(input_image,1)])

