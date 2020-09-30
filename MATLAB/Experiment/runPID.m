function wHat = runPID(phi_m, wHat, k)
wHat(k+1) = -1*(0.5*phi_m(k)*180/pi);
% wHat(k+1) = -1*(0.5*phi_m(k)*180/pi + 0.01*sum(phi_m(1:k-1)));