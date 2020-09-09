function mu = runObserver(mu, u, y, plantModel, obs, k)
Aod = obs.Aod;
Bod = obs.Bod;
Cm  = plantModel.Cm;
Dm  = plantModel.Dm;
Lo  = obs.Lo;

mu(:,k) = Aod*mu(:,k-1) + Bod*u + Lo*(y - Cm*mu(:,k-1) - Dm*u);
