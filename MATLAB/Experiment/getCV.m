function [phi, frame] = getCV(vid, cameraParams, phi_prev)
%% Unpack params
R = cameraParams.RotationMatrices(:,:,end);
T = cameraParams.TranslationVectors(end,:);

%% Get image
% Capture image from video stream
imOrig = snapshot(vid);

% Undistort the image
im = undistortImage(imOrig, cameraParams, 'OutputView', 'same');

%% Segment the circles
magnification = 100;

% Convert to HSV colour space
imHSV = rgb2hsv(im);

% Get the saturation channel.
sat = imHSV(:, :, 2);

% Attempt levels of thresholding
t = multithresh(sat,2);
imCircles = (sat > t(2));

%% Detect circles
% Find connected components.
blobAnalysis = vision.BlobAnalysis('AreaOutputPort', true,...
'CentroidOutputPort', true,...
'BoundingBoxOutputPort', true,...
'MaximumBlobArea', 5000,...
'MinimumBlobArea', 2000,...
'ExcludeBorderBlobs', true,...
'MaximumCount', 2);

[areas, centroids, boxes] = blobAnalysis(imCircles);

% Sort connected components in descending order by area
[~, idx] = sort(areas, 'Descend');

if size(boxes,1) == 2    
    % Get the two largest components.
    boxes = double(boxes(idx(1:2), :));
    centroids = double(centroids(idx(1:2),:));
    circles = [centroids, [mean(boxes(1,3:4))/2; mean(boxes(2,3:4))/2]];

    % Reduce the size of the image for display.
    scale = magnification / 100;
    imDetectedCoins = imresize(im, scale);

    % Insert labels for the circles.
    imDetectedCoins = insertObjectAnnotation(imDetectedCoins, 'circle', ...
    scale * circles(1,:), 'hand');
    imDetectedCoins = insertObjectAnnotation(imDetectedCoins, 'circle', ...
    scale * circles(2,:), 'object');
    imDetectedCoins = insertMarker(imDetectedCoins, centroids);

    frame = imDetectedCoins;

    %% Measure the circles and the angle
    % Calculate world points from image points
%     centWP = pointsToWorld(cameraParams, R, T, centroids);

    % Calculate the angle
    phi_mark = atan2(centroids(1,2) - centroids(2,2), centroids(2,1) - centroids(1,1)) + pi/2;

    phi = -1*asin(0.13*sin(pi-phi_mark)/0.18);

else
    phi = phi_prev;
    scale = magnification / 100;
    frame = imresize(im, scale);
end

    
