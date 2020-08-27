function Vd = controlAllocation(tauHat, dtheta, frictParams, motorParams)
% Extract Parameters
Ra  = motorParams.Ra;
Kw  = motorParams.Kw;
N   = motorParams.N;
p1  = motorParams.p1;
p2  = motorParams.p2;
p3  = motorParams.p3;

a1 = frictParams.a1;
a2 = frictParams.a2;
a3 = frictParams.a3;
B1 = frictParams.B2;
B2 = frictParams.B2;
B3 = frictParams.B3;
a4 = frictParams.a4;
a5 = frictParams.a5;
a6 = frictParams.a6;
B4 = frictParams.B4;
B5 = frictParams.B5;
B6 = frictParams.B6;

% Correct velocity
dtheta = dtheta.*N;

% Heaviside functions
heavi_p =@(x) 1./(1 + exp(-2.*50.*x));
heavi_m =@(x) 1./(1 + exp(-2.*50.*-x));

% Friction function
tauF = (a1.*(tanh(B1.*dtheta) - tanh(B2.*dtheta)) + a2.*tanh(B3.*dtheta) + a3.*dtheta).*heavi_p(dtheta)...
                + (a4.*(tanh(B4.*dtheta) - tanh(B5.*dtheta)) + a5.*tanh(B6.*dtheta) + a6.*dtheta).*heavi_m(dtheta);

% Motor torque
Tm = tauHat + tauF;

% Current
Ia =  p1.*Tm.^3 + p2.*Tm.^2 + p3.*Tm;

% Voltage
Vd = Kw.*dtheta + Ra.*Ia;

if Vd>6
    Vd = 6;
elseif Vd<-6
    Vd = -6;
end


