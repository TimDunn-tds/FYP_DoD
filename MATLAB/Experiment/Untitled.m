load('imOrig')
red = imOrig(:,:,1);
green = imOrig(:,:,2);
blue = imOrig(:,:,3);
thold = 1.1599;
mask = red<thold & green<thold & blue>thold;

red(~mask) = 255;
green(~mask) = 255;
blue(~mask) = 255;

imNew = cat(3,red,green,blue);

figure(1)
imshow(imNew);