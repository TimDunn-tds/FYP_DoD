function mu = runObserver(mu, u, y, ~, obs, k)
Aod = obs.Aod;
Bod = obs.Bod;
Cm  = obs.Cm;
Dm  = obs.Dm;
Lo  = obs.Lo;

mu(:,k+1) = Aod*mu(:,k) + Bod*u + Lo*(y - Cm*mu(:,k) - Dm*u);
