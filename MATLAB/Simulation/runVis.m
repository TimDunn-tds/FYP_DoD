function vid = runVis(rh, ro, phi, theta, dphi, dtheta, tout)
%% Visulisation?
% ezplot((x-1).^2+(y-1).^2-0.5.^2); pbaspect([1 1 1])
xh = 0;
yh = 0;

xo = (rh + ro).*sin(phi.signals.values);
yo = (rh + ro).*cos(phi.signals.values);

lxh = rh.*sin(theta.signals.values);
lyh = rh.*cos(theta.signals.values);

alph = (rh/ro).*(phi.signals.values - theta.signals.values);
lxo = xo + ro.*sin(alph);
lyo = yo + ro.*cos(alph);

syms x y
circ =@(a,b,r) (x - a)^2 + (y - b).^2 - r^2;

figure(2);
clf;
hold on;
axis([-0.3 0.3 -0.2 0.3]);

fimplicit(circ(xh,yh,rh),'-b','linewidth',2);
h = fimplicit(circ(xo(1),yo(1),ro),'-b','linewidth',2);
line_h = plot([0,lxh(1)],[0,lyh(1)],'-b','linewidth',2);

line_o = plot([xo(1), lxo(1)],[yo(1),lyo(1)],'-r','linewidth',2);
pbaspect([1 1 1]); 

% txt = text(-0.25,0.35, sprintf('theta = %3.0f, phi = %3.0f', theta.signals.values(1), phi.signals.values(1)));
% txt2= text(-0.25, 0.3, sprintf('dtheta = %3.0f, dphi = %3.0f', dtheta.signals.values(1)*180/pi, dphi.signals.values(1)*180/pi));

F(1) = getframe(gcf);

for i=2:1:size(tout)
    delete(h);
    delete(line_h);
    
    delete(line_o);
    
%     delete(txt);
%     delete(txt2);
    
    h = fimplicit(circ(xo(i),yo(i),ro),'-r','linewidth',2);
    line_h = plot([0,lxh(i)],[0,lyh(i)],'-b','linewidth',2);
    
    line_o = plot([xo(i), lxo(i)],[yo(i),lyo(i)],'-r','linewidth',2);
    
    caption = sprintf('t = %3.2f', i*0.01);
    title(caption);
    
%     txt = text(-0.25,0.35, sprintf('theta = %3.0f, phi = %3.0f, alpha = %3.0f', theta.signals.values(i)*180/pi,...
%         (phi.signals.values(i))*180/pi, alph(i)*180/pi));
%     txt2= text(-0.25, 0.3, sprintf('dtheta = %3.0f, dphi = %3.0f', dtheta.signals.values(i)*180/pi, dphi.signals.values(i)*180/pi));
    
    drawnow;
    pbaspect([1 1 1]);
    F(i) = getframe(gcf);
    
    
end
vid = F;