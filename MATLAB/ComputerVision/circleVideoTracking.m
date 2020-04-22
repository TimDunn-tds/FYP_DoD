clear; clc;
% vid = videoinput('winvideo');
vid = webcam();
vid.Resolution = '640x480';

for i = 1:100
    %% Segment the coins
%     im = getsnapshot(vid);
    im = snapshot(vid);
    
    magnification = 50;

    % Convert to HSV colour space
    imHSV = rgb2hsv(im);

    % Get the saturation channel.
    sat = imHSV(:, :, 2);

    % Attempt levels of thresholding
    t = multithresh(sat,4);
    imCircles = (sat > t(3));
% 
%     figure(8); 
%     imshow(imCoin2, 'InitialMagnification', magnification);
%     title('Segmented Coins');
    

    %% Detect circles
    % Find connected components.
    blobAnalysis = vision.BlobAnalysis('AreaOutputPort', true,...
    'CentroidOutputPort', false,...
    'BoundingBoxOutputPort', true,...
    'MinimumBlobArea', 200, 'ExcludeBorderBlobs', true);
    [areas, boxes] = blobAnalysis(imCircles);

    % Sort connected components in descending order by area
    [~, idx] = sort(areas, 'Descend');
    
    if size(boxes,1) > 1      
        % Get the two largest components.
        boxes = double(boxes(idx(1:2), :));
    end
    
    % Reduce the size of the image for display.
    scale = magnification / 100;
    imDetectedCoins = imresize(im, scale);

    % Insert labels for the circles.
    imDetectedCoins = insertObjectAnnotation(imDetectedCoins, 'rectangle', ...
    scale * boxes(1,:), 'hand');
    imDetectedCoins = insertObjectAnnotation(imDetectedCoins, 'rectangle', ...
    scale * boxes(2,:), 'object');
    figure(1);
    imshow(imDetectedCoins);
    txt = ['Detected circles. Time =',num2str(i)];
    title(txt);
    drawnow;
    pause(0.1);

end



















clear vid;