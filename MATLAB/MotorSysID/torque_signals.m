


d = 0.01;
ssV = 4;
time = linspace(0,10,10/d);
t_kick = linspace(0,0.5,0.5/d);
dir = sign(ssV);
kick = dir * (3 - ((3-abs(ssV))/0.5)*t_kick);

oneV_torque = ssV.*ones(1000,1);
rev = -dir*2;
oneV_torque(1:150) = rev;
oneV_torque(150:150+length(t_kick)-1) = kick;
% figure(11); clf;
% plot(time,oneV_torque);


save('oneV_torque.mat','oneV_torque');





