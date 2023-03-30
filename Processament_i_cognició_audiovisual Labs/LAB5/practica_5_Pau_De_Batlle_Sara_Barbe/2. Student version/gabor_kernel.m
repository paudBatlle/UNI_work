function G = gabor_kernel(kern_size, theta, lambda, phi, sigma, gamma, fig)

if isempty(sigma)
    sigma = 0.562*lambda;  % from Kruizinga, Petkov 1999
end

[X, Y] = meshgrid(linspace(-7,7,kern_size),linspace(-7,7,kern_size));

%% Modify
Xp = X*cos(theta)+Y*sin(theta); % equation of x' in the pwp
Yp = -X*sin(theta)+Y*cos(theta); % equation of y' in the pwp
COS = cos(2*pi*(Xp/lambda)+phi); % equation of the cosinus part from the pwp % The sinusoid!

GAUSS = exp(-((Xp.*Xp)+pow2(gamma)*(Yp.*Yp))/(2*pow2(sigma))); % equation of the exponential part from the pwp % The gaussian! 
G = GAUSS.*COS; % equation unificating GAUSS and COS fromt the pwp % The Gabor!

%% don't modify
if fig
    allfigs = get(0,'Children');
    for i=1:length(allfigs)
        fns(i) = allfigs.Number;  % all open figures numbers
    end
    if exist('fns', 'var')
        maxfn = max(fns);  % the highest figure number
    else
        maxfn = 0;
    end
    figure(maxfn + 1)
    subplot(2,3,1)
    h1 = imagesc(COS);xlim([0.5,kern_size]),ylim([0.5,kern_size])
    axis square
    title(['Cos -> theta = ' num2str(theta)]) 
    subplot(2,3,4)
    hh1 = surf(COS);xlim([0.5,kern_size]),ylim([0.5,kern_size])
    axis square
    subplot(2,3,2)
    h2 = imagesc(GAUSS);xlim([0.5,kern_size]),ylim([0.5,kern_size])
    axis square
    title(['Gauss -> theta = ' num2str(theta)]) 
    subplot(2,3,5)
    hh2 = surf(GAUSS);xlim([0.5,kern_size]),ylim([0.5,kern_size])
    axis square
    subplot(2,3,3) 
    h3 = imagesc(G);xlim([0.5,kern_size]),ylim([0.5,kern_size])
    axis square
    title(['Gabor -> theta = ' num2str(theta)]) 
    subplot(2,3,6)
    hh3 = surf(G);xlim([0.5,kern_size]),ylim([0.5,kern_size])
    axis square
    
end