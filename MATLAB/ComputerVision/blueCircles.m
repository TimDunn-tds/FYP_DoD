close all
%% Prepare calibration images
numImages = 8;
files = cell(1, numImages);

for i = 1:numImages
    files{i} = fullfile('checkerboardTest', sprintf('img_%d.jpg', i));
end

% Display a calibration image
magnification = 50;
I = imread(files{3});
figure(1);
imshow(I, 'InitialMagnification', magnification);
title('One of the images');


%% Estimate Camera Parameters
[imagePoints, boardSize, imagesUsed] = detectCheckerboardPoints(files);

% Check that the checkerdboard pattern has been correctly recognised
files = files(imagesUsed);
figure(2);
for i = 1:numel(files)
  I = imread(files{i});
  subplot(3, 3, i);
  imshow(I);
  hold on;
  plot(imagePoints(:,1,i),imagePoints(:,2,i),'ro');
end

% Generate world coordinates of the board corners in the pattern-centric
% coordinate system, with the upper-left corner at 0,0
squareSize = 24; % [mm]
worldPoints = generateCheckerboardPoints(boardSize, squareSize);

% Calibrate the camera
imageSize = [size(I,1), size(I,2)];
cameraParams = estimateCameraParameters(imagePoints, worldPoints,...
                'ImageSize', imageSize);

% Evaluate calibration accuracy.
figure(3); 
showReprojectionErrors(cameraParams);
title('Reprojection Errors');


%% Pick the image to be used for measuring
imOrig = imread(fullfile('checkerboardTest','img_4.jpg'));
figure(4);
imshow(imOrig, 'InitialMagnification', magnification);
title('Input Image');

%% Undistort the image
[im, newOrigin] = undistortImage(imOrig, cameraParams, 'OutputView', 'full');
figure(5); 
imshow(im, 'InitialMagnification', magnification);
title('Undistorted Image');

%% Segment the coins
clc;
% Convert to HSV colour space
imHSV = rgb2hsv(im);

figure(6); 
imshow(imHSV, 'InitialMagnification', magnification);
title('HSV version of test image');

% Get the saturation channel.
sat = imHSV(:, :, 2);

% Threshold the image
t = graythresh(sat);
imCoin = (sat > t);

figure(7); 
imshow(imCoin, 'InitialMagnification', magnification);
title('Segmented Circles');

% % Attempt levels of thresholding
% t2 = multithresh(sat,3);
% % imCoin2 = (sat > t2(3) & sat < t2(4));
% 
% figure(8); 
% imshow(imCoin2, 'InitialMagnification', magnification);
% title('Segmented Coins');
% 
% 
% % Segment image into the threshold levels
% segIm = imquantize(sat,t2);
% % Convert segmented image into RGB
% imRGB = label2rgb(segIm);
% figure(9);
% imshow(imRGB, 'InitialMagnification', magnification);


% % Extract only the circle satuation levels
% imCoin2 = (sat > t2(2));
% figure(10); 
% imshow(imCoin2, 'InitialMagnification', magnification);
% title('Segmented Coins');



%% Detect circles
% Find connected components.
blobAnalysis = vision.BlobAnalysis('AreaOutputPort', true,...
    'CentroidOutputPort', true,...
    'BoundingBoxOutputPort', true,...
    'MinimumBlobArea', 200, 'ExcludeBorderBlobs', true);
[areas, centroid, boxes] = blobAnalysis(imCoin(1:263,:));

% Sort connected components in descending order by area
[~, idx] = sort(areas, 'Descend');

% Get the two largest components.
boxes = double(boxes(idx(1:2), :));
centroid = double(centroid(idx(1:2),:));

% Append radius
circles = [centroid, [mean(boxes(1,3:4))/2; mean(boxes(2,3:4))/2]];
% Reduce the size of the image for display.
scale = magnification / 100;
imDetectedCoins = imresize(im, scale);

% Insert labels for the coins.
% imDetectedCoins = insertObjectAnnotation(imDetectedCoins, 'rectangle', ...
%     scale * boxes, 'blue circle');
imDetectedCoins = insertObjectAnnotation(imDetectedCoins, 'circle', ...
    scale * circles, 'blue circle');
figure; imshow(imDetectedCoins);
title('Detected Circles');


%% Compute Extrinsics
% Detect the checkerboard.
[imagePoints, boardSize] = detectCheckerboardPoints(im);

% Adjust the imagePoints so that they are expressed in the coordinate system
% used in the original image, before it was undistorted.  This adjustment
% makes it compatible with the cameraParameters object computed for the original image.
imagePoints = imagePoints + newOrigin; % adds newOrigin to every row of imagePoints

% Compute rotation and translation of the camera.
[R, t] = extrinsics(imagePoints, worldPoints, cameraParams);


%% Measure the first coin
% Adjust upper left corners of bounding boxes for coordinate system shift 
% caused by undistortImage with output view of 'full'. This would not be
% needed if the output was 'same'. The adjustment makes the points compatible
% with the cameraParameters of the original image.
boxes = boxes + [newOrigin, 0, 0]; % zero padding is added for width and height

% Get the top-left and the top-right corners.
box1 = double(boxes(1, :));
imagePoints1 = [box1(1:2); ...
                box1(1) + box1(3), box1(2)];

% Get the world coordinates of the corners            
worldPoints1 = pointsToWorld(cameraParams, R, t, imagePoints1);

% Compute the diameter of the coin in millimeters.
d = worldPoints1(2, :) - worldPoints1(1, :);
diameterInMillimeters = hypot(d(1), d(2));
fprintf('Measured diameter of first circle = %0.2f mm\n', diameterInMillimeters);



% Get the top-left and the top-right corners.
box2 = double(boxes(2, :));
imagePoints2 = [box2(1:2); ...
                box2(1) + box2(3), box2(2)];

% Apply the inverse transformation from image to world            
worldPoints2 = pointsToWorld(cameraParams, R, t, imagePoints2);            

% Compute the diameter of the coin in millimeters.
d = worldPoints2(2, :) - worldPoints2(1, :);
diameterInMillimeters = hypot(d(1), d(2));
fprintf('Measured diameter of the other circle = %0.2f mm\n', diameterInMillimeters);


%% Measure distance between the coins
ip_box1_cent = [box1(1) + box1(3)/2, box1(2) + box1(4)/2];
ip_box2_cent = [box2(1) + box2(3)/2, box2(2) + box2(4)/2];

wp_bo1_cent = pointsToWorld(cameraParams, R, t, ip_box1_cent);
wp_bo2_cent = pointsToWorld(cameraParams, R, t, ip_box2_cent);

dist = hypot((wp_bo1_cent(1) - wp_bo2_cent(1)), (wp_bo1_cent(2) - wp_bo2_cent(2)));

fprintf('Measured distance between the two circle centres = %0.2f mm\n', dist);

%% Measure angle between the coins
theta = atan2((wp_bo2_cent(1) - wp_bo1_cent(1)), (wp_bo2_cent(2) - wp_bo1_cent(2)));

fprintf('Measured angle between the two circle centres = %0.2f deg from positive y axis\n', rad2deg(theta));

