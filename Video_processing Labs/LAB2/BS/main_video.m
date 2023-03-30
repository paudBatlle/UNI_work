%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%              LABORATORY #2 
%%%              VIDEO PROCESSING 2022-2023
%%%              VIDEO SEGMENTATION - BACKGROUND SUBTRACTION 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
clear all
close all
clc

addpath data

% Loading input data
video=VideoReader('input-video.avi');

% Extracting frames (a set of them or, all of them). 10 frames in the example
frames=read(video,[11 20]);
th =10;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Video Segmentation
for i = 15 : 18
    
  % Loading image
  input_image = double(read(video,i+1));
  aux = double(read(video,[i-5,i]));
  image_template = median(aux,4);
  
  %Extracting foreground
  [foreground,cc(i),cr(i),radius,flag]=extract_object(input_image,image_template,th);
  if flag==0
    continue
  end

  %Observing results
  figure(2)
  clf
  imshow(uint8(input_image))
  hold on
  for c = -0.97*radius: radius/20 : 0.97*radius
      r = sqrt(radius^2-c^2);
      plot(cc(i)+c,cr(i)+r,'g.')
      plot(cc(i)+c,cr(i)-r,'g.')
  end 
  hold off

  disp('Observing detection. Press any key');  
  %pause;
   
   
end

% Recovering the full trajectory
cr=size(input_image,1)-cr;
figure(100)
plot(cc(2:end),cr(2:end),'--rs','LineWidth',2,'MarkerEdgeColor','k','MarkerFaceColor','k','MarkerSize',10)
axis([0 size(input_image,2) 0 size(input_image,1)])

