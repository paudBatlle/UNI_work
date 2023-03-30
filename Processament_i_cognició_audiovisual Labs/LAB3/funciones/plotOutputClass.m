function plotOutputClass(data,t,data_val,t_val,data_raw,W1,W2,corr_class1,corr_class2,corr_class1_val,corr_class2_val)

%%%%% Funcion para estimar los margenes de decision
dec_fnc = @(x) double( sigmoid(W2'*[ones(1, size(x,2)); sigmoid(W1'*[ones(1, size(x,2)); x])]) > 0.5 );
limits = [(min(data_raw(:,1:2),[],1)-2)' (max(data_raw(:,1:2),[],1)+2)'];
[xx, yy, resp] = find_decision_boundary(limits, dec_fnc, 100);
figure,
set(gcf,'units','normalized','position',[0,0.25,1,0.4])
%%%%% Plot para ver los datos y los margenes de decision tras el entrenamiento
subplot(121)
h = pcolor(xx, yy, resp);
hold on
set(h, 'facealpha', 0.42), colormap(jet)
shading flat
xlim([min(xx(:)), max(xx(:))]), ylim([min(yy(:)), max(yy(:))])
plot(data(t==1,2), data(t==1,3), 'ro', 'linestyle', 'none', 'markersize', 12 ,'linewidth',2)
plot(data_val(t_val==1,2), data_val(t_val==1,3), 'rx', 'linestyle', 'none', 'markersize', 12 ,'linewidth',2)
plot(data(t==0,2), data(t==0,3), 'bo', 'linestyle', 'none', 'markersize', 12,'linewidth',2)
plot(data_val(t_val==0,2), data_val(t_val==0,3), 'bx', 'linestyle', 'none', 'markersize', 12,'linewidth',2)
set(gca, 'fontsize', 12)
xlabel('feature 1', 'fontsize', 12), ylabel('feature 2', 'fontsize', 12)
%%%%% Plot para ver el porcentaje de aciertos
subplot(122)
bar(1, corr_class1,'facecolor', [.8 .1 .1])
hold on
bar(2, corr_class2,'facecolor', [.15 .2 .7])
bar(3, corr_class1_val,'facecolor', [.8 .1 .1])
bar(4, corr_class2_val, 'facecolor', [.15 .2 .7])
ylabel('Porcentaje de aciertos', 'fontsize', 12), ylim([0, 1.05])
legend('clase 1','clase 2','location','bestoutside'),xlim([0 5])
set(gca, 'fontsize', 12, 'XTick', [1.5,3.5],'XTickLabel',{'training (o)', 'validacion (x)'} )

end