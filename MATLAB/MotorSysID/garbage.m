R = sol(1);
L = sol(2);
Kb = sol(3);
Km = sol(4);
Kf = sol(5);
J = sol(5)/(sol(6)^2);


A = [-R/L -Kb/L; Km/J -Kf/J];
B = [1/L; 0];
C = [0 1];
D = [0];
sys_dc = ss(A,B,C,D);

sys_dc = c2d(sys_dc,0.01);