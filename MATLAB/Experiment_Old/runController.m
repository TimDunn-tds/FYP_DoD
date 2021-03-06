function tauHat = runController(mup, ref, LQG)
% Extract states
% theta    = mup(1);
% phi      = mup(2);
% dtheta   = mup(3);

% Extract controller matrices
Nu      = LQG.Nu;
Nx      = LQG.Nx;
Kc      = LQG.Kc;

tauHat = (Nu + Kc*Nx)*ref + Kc*mup;

maxT = 0.8;
if tauHat>maxT
    tauHat = maxT;
elseif tauHat<-maxT
    tauHat = -maxT;
end


