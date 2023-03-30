function plotOutputNN(x,t,x_val,t_val,out,y,E,E_val)

% set figure
set(gcf,'units','normalized','position',[0,0.25,1,0.4])

% plot the errors
subplot(121), cla
plot(E, 'color', [.9 .4 0], 'linewidth', 1.5), hold on, grid on
plot(E_val, '--','color', [.0 .7 .2], 'linewidth', 1.5)
xlabel('iteraciones', 'fontsize', 12)
ylabel('error', 'fontsize', 12)
legend({'training error', 'validation error'}, 'fontsize', 12)
set(gca, 'fontsize', 12,'yscale','log','xscale','log')

% plot the output of the network
subplot(122),cla
plot(out(:,2), y,'b:','linewidth', 1.5)
hold on
plot(x(:,2), t,'ro','linestyle', 'none', 'markersize', 10)
plot(x_val(:,2), t_val,'rx','linestyle', 'none', 'markersize', 10)
set(gca, 'fontsize', 12)
xlim([-0.1 1.1]),xlabel('data'),ylabel('t')
legend({'output','entrenamiento','validacion'},'location','best')

end