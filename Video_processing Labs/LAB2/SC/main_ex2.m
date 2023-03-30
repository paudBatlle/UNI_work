%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%              LABORATORY #2 
%%%              VIDEO PROCESSING 2022-2023
%%%              VIDEO SEGMENTATION - VIDEO SCENE SEGMENTATION BY 
%%%                                   SUBSPACE CLUSTERING 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% You should tune the variable "corruption" and "filter" to add or not
% noisy observations, and to consider a particular order in your temporal
% filtering. 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

clear all
close all
clc


addpath common
addpath osc
addpath libs\ncut
addpath data


for i = 1:13
    input = imread(sprintf('n%d.bmp',i));
    A(:,i) = double(input(:));
end
X = A ;
X = normalize(X);
% Solving optimization problem
lambda_1 = 0.099; % Weight coefficient to impose sparsity in affinities
lambda_2 = 0.001; % Weight coefficient to enforce temporal consistency
filter = 2; % <-------   Impose order for temporal filtering [1, 2, 4]
Z = osc_relaxed(X, lambda_1, lambda_2, filter);

% Observing the affinity matrix
figure(1)
imagesc(abs(Z) + abs(Z'))
xlabel('Frame number');
ylabel('Frame number');
n_space = 4;
% Split the video in clusters from affinity matrix Z
clusters = ncutW(abs(Z) + abs(Z'), n_space);
final_clusters = condense_clusters(clusters, 1);


% Observing the results
figure(2) 
imagesc(final_clusters);
ylabel('Label for every frame');
title('Your estimation')